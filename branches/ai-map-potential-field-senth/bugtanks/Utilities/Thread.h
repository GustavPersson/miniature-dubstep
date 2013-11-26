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
*/

#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include "../Utilities/Macros.h"

namespace utilities
{
const int MAX_THREADS = 5;

/**
* Base class for threads
*/
class Thread
{
public:
	/**
	 * Constructor
	 */
	Thread();

	/**
	 * Destructor
	 */
	virtual ~Thread();

	/**
	 * Creates a thread
	 */
	pthread_t create();

	/**
	 * Exits thread
	 */
	void exit();

	/**
	 * Checks if thread is running or not
	 */
	bool getQuit();

	/**
	 * Locks a thread
	 */
	void lock(int index);

	/**
	 * Unlocks a thread
	 */
	void unlock(int index);

	/**
	 * Returns instance
	 */
	//virtual Thread* getInstance() = 0;

	/**
	 * Returns thread handle
	 */
	inline virtual pthread_t getThreadHandle() const {return mThread;}

	/**
	 * Returns if the thread is running or not
	 */
	inline virtual bool getRunFlag() const {return isRunning;}

	/**
	 * Thread function
	 */
	static void* run(void* arg);

	/**
	 * Wrapper for the thread
	 */
	virtual void mainLoop() = 0;

protected:

	/**
	 * Sets thread to running or shut down
	 * @param flag True if thread is running
	 */
	inline virtual void setRunFlag(bool flag){isRunning = flag;}

private:
	pthread_t		mThread;
	pthread_mutex_t mEventMutex[MAX_THREADS];
	pthread_mutex_t mQuitMutex;

	bool			isRunning;
	bool			quit;
};
}

#endif