#include "transfer_task_manager.h"
#include "base/pb/protocol/IM.BaseDefine.pb.h"
#include "base/util.h"
#include "config_util.h"
#include "file_client_conn.h"

using namespace IM::BaseDefine;

TransferTaskManager::TransferTaskManager() {

}

void TransferTaskManager::OnTimer(uint64_t tick) {

    //                              taskid与任务的映射表
    for(TransferTaskMap::iterator it = transfer_tasks_.begin(); it != transfer_tasks_.end(); ) {
        BaseTransferTask* task = it->second;
        if(task == nullptr) {   //任务被删除了，也就直接剔除了
            transfer_tasks_.erase(it++);
            continue;
        }
        if(task->state() != kTransferTaskStateWaitingUpload &&
        task->state() == kTransferTaskStateTransferDone) { //发送完毕
            long esp = time(NULL) - task->create_time();
            if(esp > ConfigUtil::GetInstance()->GetTaskTimeout()) { //1h
                if(task->GetFromConn()) { //获取发送方client
                    FileClientConn* conn = reinterpret_cast<FileClientConn*>(task->GetFromConn());
                    conn->ClearTransferTask();
                }
                if(task->GetToConn()) { //获取接收方client
                    FileClientConn* conn = reinterpret_cast<FileClientConn*>(task->GetToConn());
                    conn->ClearTransferTask();
                }
                delete task;
                transfer_tasks_.erase(it ++);
                continue;
            }
        }
        it++;
    }
}

BaseTransferTask* TransferTaskManager::NewTransferTask(uint32_t trans_mode, const std::string& task_id, uint32_t from_user_id, uint32_t to_user_id, const std::string& file_name, uint32_t file_size) {
    BaseTransferTask* transfer_task = nullptr;
    TransferTaskMap::iterator it = transfer_tasks_.find(task_id);
    if(it == transfer_tasks_.end()) {
        if(trans_mode == IM::BaseDefine::FILE_TYPE_ONLINE) {
            transfer_task = new OnlineTransferTask(task_id, from_user_id, to_user_id, file_name, file_size);
        } else if (trans_mode == IM::BaseDefine::FILE_TYPE_OFFLINE) {
            transfer_task = new OfflineTransferTask(task_id, from_user_id, to_user_id, file_name, file_size);
        } else {
            log("Invalid trans_mode = %d", trans_mode);
        }
        if(transfer_task) {
            transfer_tasks_.insert(std::make_pair(task_id, transfer_task));
        }
    } else {
        log("Task existed by task_id=%s, ?", task_id.c_str());
    }
    return transfer_task;
}

OfflineTransferTask* TransferTaskManager::NewTransferTask(const std::string& task_id, uint32_t to_user_id) {
    OfflineTransferTask* transfer_task = OfflineTransferTask::LoadFromDisk(task_id, to_user_id);
    if(transfer_task) {
        transfer_tasks_.insert(make_pair(task_id, transfer_task));
    }
    return transfer_task;
}

bool TransferTaskManager::DeleteTransferTaskByConnClose(const std::string& task_id) {
    bool rv = false;
    TransferTaskMap::iterator it = transfer_tasks_.find(task_id);
    if(it != transfer_tasks_.end()) {
        BaseTransferTask* transfer_task = it->second;
        if(transfer_task->GetTransMode() == FILE_TYPE_ONLINE) { //在线传输
            if(transfer_task->GetFromConn() == nullptr && transfer_task->GetToConn() == nullptr) {
                delete transfer_task;
                transfer_tasks_.erase(it);
                rv = true;
            }
        } else {    //离线传输
            if(transfer_task->state() != kTransferTaskStateWaitingUpload) {//不是等待上传状态
                delete transfer_task;
                transfer_tasks_.erase(it);
                rv = true;
            }
        }
    }
    return rv;
}

bool TransferTaskManager::DeleteTransferTask(const std::string& task_id) {
    bool rv = false;
    TransferTaskMap::iterator it = transfer_tasks_.find(task_id);
    if(it != transfer_tasks_.end()) {
        delete it->second;
        transfer_tasks_.erase(it);
    }
    return rv;
}