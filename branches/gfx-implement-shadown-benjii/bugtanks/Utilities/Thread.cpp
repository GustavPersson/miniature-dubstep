/**
* @file
* @author Benjamin Golba <benjii.g@gmail.com>
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
* @section DESCRIPTION
*
* Base class for threads
*/

#include "Thread.h"

using namespace utilities;

Thread::Thread()
{
	isRunning = false;
	quit = false;
}

Thread::~Thread()
{
	//exit();
	//pthread_join(mThread, NULL);
	
	for(int i = 0; i < MAX_THREADS; i++)
	{
		SAFE_MUTEX_DESTROY(&mEventMutex[i]);
	}

	SAFE_MUTEX_DESTROY(&mQuitMutex);
}

pthread_t Thread::create()
{
	pthread_mutex_init(&mQuitMutex, NULL);
	for(int i = 0; i < MAX_THREADS; i++)
	{
		pthread_mutex_init(&mEventMutex[i], NULL);
	}

	pthread_create(&mThread, NULL, run, (void*)this);

	quit = false;

	return mThread;
}

void Thread::exit()
{
	pthread_mutex_lock(&mQuitMutex);
	quit = true;
	pthread_mutex_unlock(&mQuitMutex);
}

void Thread::lock(int index)
{
	int r = pthread_mutex_lock(&mEventMutex[index]);
}

void Thread::unlock(int index)
{
	int r = pthread_mutex_unlock(&mEventMutex[index]);
}

bool Thread::getQuit()
{
	return quit;
}

void* Thread::run(void* arg)
{
	Thread* pThread = reinterpret_cast<Thread*>(arg);
	pThread->mainLoop();
	pthread_exit(NULL);
	return NULL;
}