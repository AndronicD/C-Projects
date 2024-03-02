#include "os_threadpool.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* === TASK === */

/* Creates a task that thread must execute */
os_task_t *task_create(void *arg, void (*f)(void *))
{
    // TODO
    os_task_t *task = (os_task_t*)calloc(1, sizeof(os_task_t));
    task->argument = arg;
    task->task = f;
    return task;
}

/* Add a new task to threadpool task queue */
void add_task_in_queue(os_threadpool_t *tp, os_task_t *t)
{
    // TODO
    pthread_mutex_lock(&(tp->taskLock));
    os_task_queue_t *new_task = (os_task_queue_t*)calloc(1, sizeof(os_task_queue_t));
    new_task->task = t;
    new_task->next = NULL;
    
    os_task_queue_t *ptr = tp->tasks;
    
    if(ptr != NULL){
        while(ptr->next != NULL){
            ptr = ptr->next;
        }
        ptr->next = new_task;
    }
    else{
        tp->tasks = new_task;
    }
    pthread_mutex_unlock(&(tp->taskLock));
}

/* Get the head of task queue from threadpool */
os_task_t *get_task(os_threadpool_t *tp)
{
    pthread_mutex_lock(&(tp->taskLock));
    os_task_t *task = NULL;
    if (tp->tasks != NULL) {
        task = tp->tasks->task;
        os_task_queue_t *head = tp->tasks;
        tp->tasks = tp->tasks->next;
        free(head);
    }
    pthread_mutex_unlock(&(tp->taskLock));
    return task;
}

/* === THREAD POOL === */

/* Initialize the new threadpool */
os_threadpool_t *threadpool_create(unsigned int nTasks, unsigned int nThreads)
{
    // TODO
    os_threadpool_t *tp = (os_threadpool_t *)calloc(1, sizeof(os_threadpool_t));
    if (tp == NULL) {
        return NULL;
    }

    tp->should_stop = 0;
    tp->num_threads = nThreads;

    tp->threads = (pthread_t *) malloc(nThreads * sizeof(pthread_t));
    if (tp->threads == NULL) {
        free(tp);
        return NULL;
    }

    tp->tasks = NULL;
    pthread_mutex_init(&(tp->taskLock), NULL);

    for (unsigned int i = 0; i < nThreads; i++) {
        pthread_create(&(tp->threads[i]), NULL, thread_loop_function, tp);
    }

    return tp;
}

/* Loop function for threads */
void *thread_loop_function(void *args)
{
    // TODO
    os_threadpool_t *tp = (os_threadpool_t *) args;
    os_task_t *task;
    while (1) {
        if (tp->should_stop) {
            break;
        }
        task = get_task(tp);
        if (task == NULL) {
            continue;
        }
        task->task(task->argument);
    }
    return NULL;
}

/* Stop the thread pool once a condition is met */
void threadpool_stop(os_threadpool_t *tp, int (*processingIsDone)(os_threadpool_t *))
{
    // TODO
    while (1) {
        if (processingIsDone(tp)) {
            tp->should_stop = 1;
            break;
        }
    }
    for (unsigned int i = 0; i < tp->num_threads; i++) {
        pthread_join(tp->threads[i], NULL);
    }
}
