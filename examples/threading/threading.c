#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;

    usleep((thread_func_args->waitForMutexMs*1000));

    if (pthread_mutex_lock(thread_func_args->mutex) == 0)
    {
        usleep((thread_func_args->releaseMutexMs*1000));
        if (pthread_mutex_unlock(thread_func_args->mutex) == 0)
        {
            thread_func_args->thread_complete_success = true;
        }
    }

    return thread_func_args;
}

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    bool result;
    struct thread_data *threadData;

    // Allocate memory for thread_data
    threadData = (struct thread_data*) malloc(sizeof(struct thread_data));

    // Setup wait arguments and mutex
    threadData->waitForMutexMs = wait_to_obtain_ms;
    threadData->releaseMutexMs = wait_to_release_ms;
    threadData->mutex = mutex;
    threadData->thread_complete_success = false;

    if (pthread_create(thread, NULL, &threadfunc, threadData) == 0)
    {
        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}
