#ifndef _LOCK_H_
#define _LOCK_H_

#include "pthread.h"

// auto lock ,lock when construct, unlock when destruct
struct auto_lock_t
{
	pthread_mutex_t *plock;

	auto_lock_t(pthread_mutex_t *ptr) : plock(ptr)
	{
		pthread_mutex_lock(plock);
	}
	
	~auto_lock_t()
	{
		pthread_mutex_unlock(plock);
	}
		
};


#endif
