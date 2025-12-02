#ifndef C_TALK_THREADS_H
#define C_TALK_THREADS_H

#include <netinet/in.h>
#include <pthread.h>

#define BASE_CAPACITY 15

typedef struct {
    int socket_fd;
    struct sockaddr_in address;
} ClientContext;

typedef struct Thread {
    pthread_t thread_id;
    ClientContext context;
    int is_active;
} Thread;

typedef struct ThreadPool {
    int size;
    int capacity;
    Thread** pool;
    pthread_mutex_t lock; // Mutex pour protéger l'accès au pool
} ThreadPool;

ThreadPool create_thread_pool();
void add_to_pool(ThreadPool* thread_pool, Thread* thread);
void remove_from_pool(ThreadPool* thread_pool, Thread* thread);
void destroy_thread_pool(ThreadPool* thread_pool); // Pour nettoyer à la fin

#endif
