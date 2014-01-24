/**
 * @file
 * @author Gustav Persson <gustav@alltomminrov.se>
 * @version 1.0
 * Copyright (©) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 * 
 * concurrentQueue was grabbed from
 * http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
 * and was written by Anthony Williams
 *
 */

#ifndef __CONCURRENTQUEUE_H__
#define __CONCURRENTQUEUE_H__

#include <queue>
#include <pthread.h>
#include <boost/shared_ptr.hpp>


namespace logic
{

/*
* A thread safe queue to handle threading and events, among other things.
*/
template<typename Data>

/**
 * Modified to use pthreads instead of boost threads.
 */
class ConcurrentQueue
{
public:
	/**
	* Constructor. Initializes mutex and condvar.
	*/
	explicit ConcurrentQueue()
	{
		pthread_mutex_init(&mMutex, NULL);
		pthread_cond_init(&mCondVar, NULL);
	}
	
	/**
	* Push an object on the queue.
	* @param data the object to enqueue.
	*/
	void push(Data const& data)
	{
		int res = pthread_mutex_lock(&mMutex);
		mQueue.push(data);
		res = pthread_mutex_unlock(&mMutex);
		res = pthread_cond_signal(&mCondVar);
	}

	/**
	* Check if the queue is empty.
	* @returns True if queue is empty, false if not.
	*/
	bool empty() const
	{
		bool result;
		pthread_mutex_lock(&mMutex);
		result = mQueue.empty();
		pthread_mutex_unlock(&mMutex);
		return result;
	}

	/**
	* Tries to pop the queue.
	* @returns True if successful, false if not.
	* @param poppedValue The pointer for storing the popped value.
	*/
	bool tryPop(Data& poppedValue)
	{
		pthread_mutex_lock(&mMutex);
		while(mQueue.empty())
		{
			pthread_mutex_unlock(&mMutex);
			return false;
		}

		poppedValue = mQueue.front();
		mQueue.pop();
		pthread_mutex_unlock(&mMutex);
		return true;
	}

	/**
	* Tries to pop the queue. If the queue is empty, the thread
	* waits for it to fill back up.
	* @param poppedValue The pointer for storing the popped value.
	*/
	void waitAndPop(Data& poppedValue)
	{
		pthread_mutex_lock(&mMutex);

		while(mQueue.empty())
		{
			pthread_cond_wait(&mCondVar, &mMutex);
		}

		poppedValue = mQueue.front();
		mQueue.pop();
		pthread_mutex_unlock(&mMutex);
	}

private:	
	std::queue<Data> mQueue;
	pthread_mutex_t mMutex;
	pthread_cond_t mCondVar;
};

}
#endif
