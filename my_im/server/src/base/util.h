#ifndef __UTIL_H__
#define __UTIL_H__

#define _CRT_SECURE_NO_DEPRECATE //C4996

#include "ostype.h"
#include "UtilPdu.h"
#include "Lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slog/slog_api.h"
#include <strings.h>
#include <sys/stat.h>
#include <assert.h>

#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define NOTUSED_ARG(v) ((void)v)
//used this to remove warning C4100, unreferenced parameter

class CRefObject {      //用于给fd 做引用计数
public:
    CRefObject();
    virtual ~CRefObject();

    void SetLock(CLock* lock) { m_lock = lock; }
    void AddRef();
    void ReleaseRef();
private:
    int             m_refCount;
    CLock*          m_lock;
};

#define LOG_MODULE_IM       "IM"

extern CSLog  g_imlog;  //日志

//strrchr, 在 __FILE__ 中查找最后一个/ 出现的位置， 并返回从它开始的后边所有内容
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#define log(fmt, args...)  g_imlog.Info("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define log_debug(fmt, args...)  g_imlog.Debug("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define log_warn(fmt, args...)  g_imlog.Warn("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define log_error(fmt, args...)  g_imlog.Error("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define log_fatal(fmt, args...)  g_imlog.Fatal("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)

uint64_t get_tick_count();
void util_sleep(uint32_t millisecond) ;

class CStrExplode {
public:
    CStrExplode(char* str, char seperator);
    virtual ~CStrExplode();
    
    uint32_t GetItemCnt() { return m_item_cnt; }
    char* GetItem(uint32_t idx) { return m_item_list[idx]; }
private:
    uint32_t    m_item_cnt;
    char**      m_item_list;
};

char* replaceStr(char* pSrc, char oldChar, char newChar);
string int2string(uint32_t user_id);
uint32_t string2int(const string& value);
void replace_mark(string& str, string& new_value, uint32_t&begin_pos);
void replace_mark(string& str, uint32_t new_value, uint32_t&begin_pos);

void writePid();

//对url进行编码的常见3中方式： 转16进制， base64， urlencode
inline unsigned char toHex(const unsigned char& x);
inline unsigned char fromHex(const unsigned char& x);
string URLEncode(const string& sIn);
string URLDecode(const string& sIn);

int64_t get_file_size(const char* path);


//在src_str中找到sub_str出现的位置并返回
const char* memfind(const char* src_str, size_t src_len, const char* sub_str, size_t sub_len, bool flag = true);


#endif 