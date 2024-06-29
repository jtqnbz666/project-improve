#include "EventDispatch.h"
#include "BaseConn.h"
EventDispatch* EventDispatch::m_eventDispatchIns = NULL;

EventDispatch::EventDispatch() {
    running = false;
    m_epfd = epoll_create(1024);
    if(m_epfd == -1) {
        printf("epoll_create failed");
    }

    timerEvents = new TimerManager();
}

EventDispatch::~EventDispatch() {
    close(m_epfd);
}

EventDispatch* EventDispatch::Instance() {
    if(m_eventDispatchIns == NULL) {
        m_eventDispatchIns = new EventDispatch();
    }

    return m_eventDispatchIns;
}

void EventDispatch::AddEvent(int fd, uint8_t socket_event) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLPRI | EPOLLERR | EPOLLHUP;
    ev.data.fd = fd;
    if(epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) != 0) {
        printf("epoll_ctl() failed, errno = %d", errno);
    }
}

void EventDispatch::RemoveEvent(int fd, uint8_t socket_event) {
    if(epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, NULL) != 0) {
        printf("epoll_ctl failed, errno = %d", errno);
    }
}


void EventDispatch::StartDispatch(uint32_t wait_timeout) {
    struct epoll_event events[1024];
    int nfds = 0;
    if(running) return;
    running = true;
    wait_timeout = 100; // 之后加上定时任务再改进
    while(running) {
        nfds = epoll_wait(m_epfd, events, 1024, timerEvents->TimeToSleep());
        for(int i = 0; i < nfds; i ++) {
            int ev_fd = events[i].data.fd;
            printf("触发了%d, event:%d\n", ev_fd, events[i].events);
            auto bConn =ConnManager::Instance().GetConn(ev_fd);
            if(!bConn) {
                printf("没有找到socket信息\n");
                continue;
            }
            if(events[i].events & EPOLLIN) {
                bConn->OnRead();
            }
            if(events[i].events & EPOLLOUT) {
                bConn->OnWrite();
            }
            if(events[i].events & (EPOLLPRI | EPOLLERR | EPOLLHUP)) {
                bConn->OnClose();
            }
        }

        while (timerEvents->CheckTimer());
    }
}
