/**
 *  Copyright (C) 2011-2012  Juho Vähä-Herttua
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#ifndef THREADS_H
#define THREADS_H

#if defined(WIN32)
#include <windows.h>

#define sleepms(x) Sleep(x)

typedef HANDLE thread_handle_t;

#define THREAD_RETVAL DWORD WINAPI
#define THREAD_CREATE(handle, func, arg) \
	handle = CreateThread(NULL, 0, func, arg, 0, NULL)
#define THREAD_JOIN(handle) do { WaitForSingleObject((handle), INFINITE); CloseHandle(handle); } while(0)

typedef SRWLOCK mutex_handle_t;

#define MUTEX_CREATE(handle) InitializeSRWLock(&(handle))
#define MUTEX_LOCK(handle) AcquireSRWLockExclusive(&(handle))
#define MUTEX_UNLOCK(handle) ReleaseSRWLockExclusive(&(handle))
#define MUTEX_DESTROY(handle) do {} while(0)

typedef CONDITION_VARIABLE cond_handle_t;

#define COND_CREATE(handle) InitializeConditionVariable(&(handle))
#define COND_WAIT(handle, mutex) SleepConditionVariableSRW(&(handle), &(mutex), INFINITE, 0)
#define COND_TIMEDWAIT(handle, mutex, millis) SleepConditionVariableSRW(&(handle), &(mutex), (millis), 0)
#define COND_SIGNAL(handle) WakeConditionVariable(&(handle))
#define COND_BROADCAST(handle) WakeAllConditionVariable(&(handle))
#define COND_DESTROY(handle) do {} while(0)

#else /* Use pthread library */

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#define sleepms(x) usleep((x)*1000)

typedef pthread_t thread_handle_t;

#define THREAD_RETVAL void *
#define THREAD_CREATE(handle, func, arg) \
	if (pthread_create(&(handle), NULL, func, arg)) handle = 0
#define THREAD_JOIN(handle) pthread_join(handle, NULL)

typedef pthread_mutex_t mutex_handle_t;

#define MUTEX_CREATE(handle) pthread_mutex_init(&(handle), NULL)
#define MUTEX_LOCK(handle) pthread_mutex_lock(&(handle))
#define MUTEX_UNLOCK(handle) pthread_mutex_unlock(&(handle))
#define MUTEX_DESTROY(handle) pthread_mutex_destroy(&(handle))

typedef pthread_cond_t cond_handle_t;

#define COND_CREATE(handle) pthread_cond_init(&(handle), NULL)
#define COND_WAIT(handle, mutex) pthread_cond_wait(&(handle), &(mutex))
#define COND_TIMEDWAIT(handle, mutex, millis) do { \
	struct timeval _tv; \
	struct timespec _ts; \
	_ts.tv_sec = _tv.tv_sec; \
	_ts.tv_sec += (millis) / 1000; \
	_ts.tv_nsec = (long) _tv.tv_usec * 1000; \
	_ts.tv_nsec += (long) ((millis) % 1000) * 1000000; \
	pthread_cond_timedwait(&(handle), &(mutex), &_ts); \
} while (0)
#define COND_SIGNAL(handle) pthread_cond_signal(&(handle))
#define COND_BROADCAST(handle) pthread_cond_broadcast(&(handle))
#define COND_DESTROY(handle) pthread_cond_destroy(&(handle))

#endif

#endif /* THREADS_H */
