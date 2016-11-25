#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

// #define USE_PTHREAD
// #define USE_SEMAPHORE

#if (defined(USE_PTHREAD) && defined(USE_SEMAPHORE))
#error "env setting is wrong."
#endif

#define THREAD_NUM (5)
pthread_t threads[THREAD_NUM];

#ifdef USE_PTHREAD
pthread_mutex_t mutex;

void lock_init()
{
    pthread_mutex_init(&mutex, NULL);
}

void lock_destroy()
{
    pthread_mutex_destroy(&mutex);
}

void lock()
{
    pthread_mutex_lock(&mutex);
}

void unlock()
{
    pthread_mutex_unlock(&mutex);
}
#endif

#ifdef USE_SEMAPHORE
sem_t sem;

void lock_init()
{
    sem_init(&sem, 0, 1);
}

void lock_destroy()
{
    sem_destroy(&sem);
}

void lock()
{
    sem_wait(&sem);
}

void unlock()
{
    sem_post(&sem);
}
#endif

uint64_t value = 0;
#define END_COUNT (0x3FFFFFFL)

void *run(void *arg)
{
    while(1)
    {
        lock();

        value++;
        if(value > END_COUNT)
        {
            unlock();
            break;
        }

        unlock();
    }

    return NULL;
}

int main()
{
    int i;

    lock_init();

    for(i = 0 ; i != THREAD_NUM ; i++)
    {
        pthread_create(&threads[i], NULL, run, NULL);
    }

    for(i = 0 ; i != THREAD_NUM ; i++)
    {
        pthread_join(threads[i], NULL);
    }

    lock_destroy();
}
