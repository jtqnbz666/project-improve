#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sys/mount.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>
#include "FileManager.h"
#include "FileLin.h"
#include "StringUtils.h"
#include "util.h"
#include "Base64.h"

using namespace std;
namespace msfs {
int FileManager::initDir() {
    bool isExist = File::isExist(m_disk);
    if(!isExist) {
        u64 ret = File::mkdirNoRecursion(m_disk);
        if(ret) {
            log("The dir[%s] set error for code[%d], \
				    its parent dir may no exists", m_disk, ret);
			return -1;
        }
    }
    //255 × 255
    char first[10] = {0};
    char second[10] = {0};
    for(int i = 0; i <= FIRST_DIR_MAX; i ++) {
        snprintf(first, 5, "%03d", i);
        string tmp = string(m_disk) + "/" + string(first);
        int code = File::mkdirNoRecursion(tmp.c_str());
        if(code && (errno != EEXIST)) {
            log("Create dir[%s] errnr[%d]", tmp.c_str(), errno);
            return -1;
        }
        for(int j = 0; j <= SECOND_DIR_MAX; j ++) {
            snprintf(second, 5, "%03d", j);
            string tmp2 = tmp + "/" + string(second);
            code = File::mkdirNoRecursion(tmp2.c_str());
            if(code && (errno != EEXIST)) {
                log("create dir[%s] error[%d]", tmp2.c_str(), errno);
                return -1;
            }
            memset(second, 0x0, 10);
        }
        memset(first, 0x0, 10);
    }
    return 0;
}

string FileManager::createFileRelatePath() {
    char first[10] = {0};
    char second[10] = {0};
    m_cs.Enter(); //锁
    snprintf(first, 5, "%03d", (int)getFirstDir());
    snprintf(second, 5, "%03d", (int)getSecondDir());
    m_cs.Leave();

    //使用时间作为文件存储信息的一部分
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    u64 usec = tv.tv_sec * 1000000 + tv.tv_usec;
    u64 tid = (u64)pthread_self();
    char unique[40];
    snprintf(unique, 30, "%llu_%llu", usec, tid);
    string path = "/" + string(first) + "/" + string(second) + "/" + string(unique);
    return string(path);
}

int FileManager::uploadFile(const char* type, const void* content, u32 size, char* url, char* ext) {
    //检查文件大小
    if(size > MAX_FILE_SIZE_PER_FILE) {
        log("File size[%d] should less than [%d]", size, MAX_FILE_SIZE_PER_FILE);
        return -1;
    }

    //追加类型后缀
    string path = createFileRelatePath();
    if(ext) {
        path += "_" + string(ext);
    } else {
        path += "." + string(type);
    }

    //construct url with group num..
    string groups("g0");
    string fullUrl = groups + path;
    strncpy(url, fullUrl.c_str(), strlen(fullUrl.c_str()));
    
    //insert map file cache
    m_cs.Enter();
    insertEntry(fullUrl, (u64)size, content);
    updateMapCache();
    m_cs.Leave();

    //打开并写入文件，然后关闭
    string absPath = string(m_disk) + path;
    File* tmpFile = new File(absPath.c_str());
    tmpFile->create();
    tmpFile->write(0, size, content);//0是偏移量
    delete tmpFile;
    tmpFile = nullptr;

    //增加总文件数量
    m_filesCs.Enter();
    m_totFiles++;
    m_filesCs.Leave();
    return 0;
}

int FileManager::getRelatePathByUrl(const string& url, string& path) {
    string::size_type pos = url.find("/");
    if(string::npos == pos) {
        log("Url [%s] format illegal.", url.c_str());
        return -1;
    }
    path = url.substr(pos);
    return 0;
}

int FileManager::getAbsPathByUrl(const string& url, string& path) {
    string relate;
    if(getRelatePathByUrl(url, relate)) {
        log("Get path from url[%s] error", url.c_str());
        return -1;
    }
    path = string(m_disk) + relate;
    return 0;
}

FileManager::Entry* FileManager::getOrCreateEntry(const std::string&url, bool create) {
    m_cs.Enter();
    EntryMap::iterator it = m_map.find(url);
    if(it != m_map.end()) {
        log("the map has the file while url:%s", url.c_str());
        m_cs.Leave();
        return it->second;
    }
    
    if(!create) {
        m_cs.Leave();
        return nullptr;
    }
    string path;        
    //得到绝对路径
    if(getAbsPathByUrl(url, path)) {
        log("Get abs path from url[%s] error", url.c_str());
        m_cs.Leave();
        return nullptr;
    }
    
    struct stat buf;
    if(stat(path.c_str(), &buf) == -1) {
        m_cs.Leave();
        return nullptr;
    }

    if(!S_ISREG(buf.st_mode)) {
        m_cs.Leave();
        return nullptr;
    }
    
    Entry* e = new Entry();
    u64 fileSize = File::getFileSize(path.c_str());
    e->m_fileSize = fileSize;
    e->m_fileContent = new ::byte[fileSize];
    memset(e->m_fileContent, 0x0, fileSize);
    e->m_lastAccess = time(0);
    File* tmpFile = new File(path.c_str());
    tmpFile->open();
    int ret = tmpFile->read(0, fileSize, e->m_fileContent); // 0是偏移量
    if(ret) {
        log("read file error while url:%s", url.c_str());
        delete e;
        e = nullptr;
        delete tmpFile;
        tmpFile = nullptr;
        m_cs.Leave();
        return nullptr;
    }
    delete tmpFile;
    tmpFile = nullptr;

    std::pair<map<std::string, Entry*>::iterator, bool> result;
    result = m_map.insert(EntryMap::value_type(url, e));
    //第一个result.first是存入数据的地址，result.second是成功与否的bool值
    if(result.second == false) {
        log("Insert url[%s] to file map error", url.c_str());
        delete e;
        e = nullptr;
    }
    updateMapCache();
    m_cs.Leave();
    return e;
}  

int FileManager::downloadFileByUrl(char* url, void* buf, u32* size) {
    Entry* en = getOrCreateEntry(url, true);
    if(!en) {
        log("download file error, while url:%s", url);
        return -1;
    }
    memcpy(buf, en->m_fileContent, en->m_fileSize);
    *size = (u32)en->m_fileSize;
    en->m_lastAccess = time(nullptr);
    return 0;
}

void FileManager::updateMapCache() {
    size_t currSize = m_map.size();
    if(currSize > MAX_FILE_IN_MAP) {
        EntryMap::iterator it = m_map.begin();
        int times = abs(int(MAX_FILE_IN_MAP - currSize));
        while(it != m_map.end() && times) {
            delete it->second;
            m_map.erase(it ++);
            times--;
        }
        it = m_map.begin();
        while(it != m_map.end() && times) {
            time_t currTime = time(nullptr);
            if(currTime - it->second->m_lastAccess > 2 * 60 * 60) { //2h
                delete it->second;
                m_map.erase(it ++);
            }
        }
    }
}

int FileManager::insertEntry(const std::string& url, size_t fileSize, const void* content) {
    //先看看是否已经存在
    if(m_map.size()) {
        EntryMap::iterator it = m_map.find(url);
        if(it != m_map.end()) return -1;
    }
    Entry* e = new Entry();
    e->m_fileSize = fileSize;
    e->m_fileContent = new ::byte[fileSize];
    e->m_lastAccess = time(NULL);
    memcpy(e->m_fileContent, content, fileSize);

    m_cs.Enter();
    pair<map<std::string, Entry*>::iterator, bool>ret;
    ret = m_map.insert(EntryMap::value_type(url, e));
    if(ret.second == false) {
        delete e;
        e = nullptr;
    }
    updateMapCache();
    m_cs.Leave();
}

void FileManager::releaseFileCache(const std::string& url) {
    m_cs.Enter();
    const Entry* entry = getEntry(url);
    if(!entry) {
        log("map has not the url:%s", url.c_str());
        m_cs.Leave();
        return;
    }
    delete []entry->m_fileContent;
    m_map.erase(url);
    m_cs.Leave();
    return;
}

}