#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 #include <errno.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    thread_func_args->thread_complete_success = true;

    /** sleep for @param wait_to_obtain_ms number of milliseconds */
    int ret = usleep(thread_func_args->wait_to_obtain_ms * 1000);
    if (ret != 0)
    {
        ERROR_LOG("usleep failed with error: %s", strerror(errno));
        thread_func_args->thread_complete_success = false;
        goto thread_exit;
    }

    /** obtain the mutex in @param mutex */
    ret = pthread_mutex_lock(thread_func_args->mutex);
    if (ret != 0)
    {
        ERROR_LOG("pthread_mutex_lock failed with error: %s", strerror(ret));
        thread_func_args->thread_complete_success = false;
        goto thread_exit;
    }

    /** sleep for @param wait_to_release_ms number of milliseconds */
    ret = usleep(thread_func_args->wait_to_release_ms * 1000);
    if (ret != 0)
    {
        ERROR_LOG("usleep failed with error: %s", strerror(errno));
        thread_func_args->thread_complete_success = false;
    }

    ret = pthread_mutex_unlock(thread_func_args->mutex);
    if (ret != 0)
    {
        ERROR_LOG("pthread_mutex_unlock failed with error: %s", strerror(ret));
        thread_func_args->thread_complete_success = false;
    }
    
    thread_exit:
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    bool return_value = true;
    // Allocate memory for thread data
    struct thread_data* thread_func_args = malloc(sizeof(struct thread_data));
    if(thread_func_args == NULL)
    {
        ERROR_LOG("malloc failed with error: %s", strerror(errno));
        return_value = false;
        goto func_exit;
    }

    // Setup mutex and wait arguments
    thread_func_args->mutex = mutex;
    thread_func_args->wait_to_obtain_ms = wait_to_obtain_ms;
    thread_func_args->wait_to_release_ms = wait_to_release_ms;

    // Pass thread_data to created thread using threadfunc() as entry point
    int ret = pthread_create(thread, NULL, threadfunc, thread_func_args);
    if(ret != 0)
    {
        ERROR_LOG("pthread_create failed with error: %s", strerror(ret));
        return_value = false;
    }

    func_exit:
    return return_value;
}

