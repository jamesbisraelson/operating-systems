//threadwrappers.h
//the .h file for threadwrappers.c
#ifndef THREADWRAPPERS_H
#define THREADWRAPPERS_H

#include <pthread.h>

int wrappedMutexLock(pthread_mutex_t* mutex);
int wrappedMutexTrylock(pthread_mutex_t* mutex);
int wrappedMutexUnlock(pthread_mutex_t* mutex);
int wrappedMutexInit(pthread_mutex_t* mutex,  const  pthread_mutexattr_t* mutexattr);
int wrappedPthreadCreate(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine) (void*), void* arg);
int wrappedPthreadJoin(pthread_t thread, void** retval);
int wrappedCondSignal(pthread_cond_t* cond);
int wrappedCondWait(pthread_cond_t* cond, pthread_mutex_t* mutex);

#endif
