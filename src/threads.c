// src/threads.c
#include "../include/threads.h"
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

ThreadPool create_thread_pool() {
    ThreadPool thr_p;
    thr_p.size = 0;
    thr_p.capacity = BASE_CAPACITY;
    thr_p.pool = (Thread**)malloc(sizeof(Thread*) * BASE_CAPACITY);
    assert(thr_p.pool != NULL);

    pthread_mutex_init(&thr_p.lock, NULL);
    return thr_p;
}

void add_to_pool(ThreadPool* thread_pool, Thread* thread) {
    pthread_mutex_lock(&thread_pool->lock);

    if (thread_pool->size == thread_pool->capacity) {
        thread_pool->capacity *= 2;
        Thread** temp = (Thread**)realloc(thread_pool->pool, sizeof(Thread*) * thread_pool->capacity);
        assert(temp != NULL);
        thread_pool->pool = temp;
    }

    for (int i = 0; i < thread_pool->size; i++) {
        if (thread_pool->pool[i] == NULL) {
            thread_pool->pool[i] = thread;
            thread_pool->size++;
        }
    }

    pthread_mutex_unlock(&thread_pool->lock);
}

void remove_from_pool(ThreadPool *thread_pool, Thread *thread) {
    pthread_mutex_lock(&thread_pool->lock);

    for (int i = 0; i < thread_pool->size; i++) {
        if (thread_pool->pool[i]->thread_id == thread->thread_id) {
            thread_pool->pool[i] = NULL;
            thread_pool->size--;
        }
    }

    pthread_mutex_unlock(&thread_pool->lock);
}
