#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include "CommType.h"
#include "Task.h"
#include <pthread.h>
#include <list>
#include <unordered_map>
#include "Router.h"
using namespace std;

class CTask;

class ThreadNotify {
public:
    ThreadNotify();
    ~ThreadNotify();
    void Lock() { pthread_mutex_lock(&m_mutex); }
    void Unlock() { pthread_mutex_unlock(&m_mutex); }
    void Wait() { pthread_cond_wait(&m_cond, &m_mutex);}
    void Signal() { pthread_cond_signal(&m_cond); }
private:
    pthread_mutex_t         m_mutex;
    pthread_mutexattr_t     m_mutexattr;

    pthread_cond_t          m_cond;
};



class WorkerThread {
    public: 
    WorkerThread();
    ~WorkerThread();

    static void* StartRoutine(void* arg);

    void Start();
    void Execute();
    void PushTask(CTask* pTask);

    void SetThreadIdx(uint32_t idx) {m_thread_idx = idx;}
private:
    
    uint32_t        m_thread_idx;
    uint32_t        m_task_cnt;
    pthread_t       m_thread_id;
    ThreadNotify   m_thread_notify;
    list<CTask*>    m_task_list;    //每个线程都有自己的任务队列，减少了锁竞争
};


class ThreadPool {
public:
   
    int Init(uint32_t worker_size);
    void AddTask(CTask* pTask);
    void Destory();
    static ThreadPool* Instance();
private:
    ThreadPool();
    virtual ~ThreadPool();

    uint32_t            m_worker_size;
    WorkerThread*      m_worker_list;
    static ThreadPool*  threadPool;
};

#endif 