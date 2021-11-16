//threadwrappers.c
//all the wrapper functions for thread and mutex functionality
#include <errno.h>
#include <stdio.h>
#include "threadwrappers.h"

int statusCheck(int s) {
	if ((errno = s) != 0) {
		perror(NULL);
	}
	return s;
}

int wrappedPthreadCreate(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine)(void *), void* arg) {
	return statusCheck(pthread_create(thread, attr, start_routine, arg));
}

int wrappedPthreadJoin(pthread_t thread, void** retval) {
	return statusCheck(pthread_join(thread, retval));
}

int wrappedCondSignal(pthread_cond_t* cond) {
	return statusCheck(pthread_cond_signal(cond));
}

int wrappedCondWait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
	return statusCheck(pthread_cond_wait(cond, mutex));
}

int wrappedMutexLock(pthread_mutex_t* mutex) {
	return statusCheck(pthread_mutex_lock(mutex));
}

int wrappedMutexTrylock(pthread_mutex_t* mutex) {
	return statusCheck(pthread_mutex_trylock(mutex));
}

int wrappedMutexUnlock(pthread_mutex_t* mutex) {
	return statusCheck(pthread_mutex_unlock(mutex));
}

int wrappedMutexInit(pthread_mutex_t* mutex,  const  pthread_mutexattr_t* mutexattr) {
	return statusCheck(pthread_mutex_init(mutex, mutexattr));
}


