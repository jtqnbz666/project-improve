# 验证主线程优先处理信号，其他线程也可以处理信号
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

void signal_handler(int signo) {
    printf("Thread %ld received signal %d\n", pthread_self(), signo);
    sleep(5);
    printf("test");
}

void* thread_func(void* arg) {
    while (1) {
        printf("Thread %ld running...\n", pthread_self());
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    pthread_create(&thread1, NULL, thread_func, NULL);
    pthread_create(&thread2, NULL, thread_func, NULL);

    while (1) {
        printf("Main thread %ld running...\n", pthread_self());
        sleep(1);
    }

    return 0;
}
