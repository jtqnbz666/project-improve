#include <stdlib.h>
#include "ThreadPool.h"
#include <iostream>

WorkerThread::WorkerThread() {
    m_task_cnt = 0;
}

WorkerThread::~WorkerThread() {

}


void* WorkerThread::StartRoutine(void* arg) {
    WorkerThread* pThread = (WorkerThread*)arg;
    pThread->Execute();
    return NULL;
}

void WorkerThread::Start() {
    
    (void)pthread_create(&m_thread_id, NULL, StartRoutine, this);
}

void WorkerThread::Execute() {
    printf("thread_%d create!\n", m_thread_id);
    while(true) {
        m_thread_notify.Lock();
        
        while(m_task_list.empty()) {
            m_thread_notify.Wait();
        }
        
        CTask* pTask = m_task_list.front();
        m_task_list.pop_front();
        m_thread_notify.Unlock();
        
        pTask->Run();

        delete pTask;

        m_task_cnt ++;
        //log(%d have the execute %d task\n", m_thread_idx,  m_task_cnt);
    }
}

void WorkerThread::PushTask(CTask* pTask) {
    m_thread_notify.Lock();
    m_task_list.push_back(pTask);
    m_thread_notify.Signal();
    m_thread_notify.Unlock();
}


ThreadPool* ThreadPool::threadPool = NULL;


ThreadPool::ThreadPool( ) {
    m_worker_size = 0;
    m_worker_list = nullptr;
}

int ThreadPool::Init(uint32_t workSize) {
    m_worker_size = workSize;
    m_worker_list = new WorkerThread[m_worker_size];
    if(!m_worker_list) return 1;
    for(uint32_t i = 0; i < m_worker_size; i ++) {
        m_worker_list[i].SetThreadIdx(i);
        m_worker_list[i].Start();
    }
    return 0;
}

ThreadPool::~ThreadPool() {

}

ThreadPool* ThreadPool::Instance() {
    if(threadPool == NULL) {
        threadPool = new ThreadPool();
    }

    return threadPool;
}


void ThreadPool::Destory() {
    if(m_worker_list) {
        delete [] m_worker_list;
    }
}

void ThreadPool::AddTask(CTask* pTask) {
    uint32_t thread_idx = random() % m_worker_size; //随机
    m_worker_list[thread_idx].PushTask(pTask);
}



ThreadNotify::ThreadNotify() {
    pthread_mutexattr_init(&m_mutexattr);
    pthread_mutexattr_settype(&m_mutexattr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_mutex, &m_mutexattr);
    pthread_cond_init(&m_cond, NULL);
}

ThreadNotify::~ThreadNotify() {
    pthread_mutexattr_destroy(&m_mutexattr);
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
}
