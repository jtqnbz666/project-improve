#ifndef __LOCK_H__
#define __LOCK_H__

#include "ostype.h"
class CLock {
public:
    CLock();
    virtual ~CLock();
    void lock();
    void unlock();
    pthread_mutex_t& getMutex() { return m_lock; }
    virtual bool try_lock();
private:
    pthread_mutex_t m_lock;
};

class CRWLock {
public: 
    CRWLock();
    virtual ~CRWLock();
    void rlock();
    void wlock();
    void unlock();
    bool try_rlock();
    bool try_wlock();
private:
    pthread_rwlock_t m_lock;
};

class CAutoRWLock {
public:
    CAutoRWLock(CRWLock* pLock, bool bRLock = true);
    virtual ~CAutoRWLock();
private:
    CRWLock* m_pLock;
};

class CAutoLock {
public:
    CAutoLock(CLock* pLock);
    virtual ~CAutoLock();
private:
    CLock* m_pLock;
};


#endif