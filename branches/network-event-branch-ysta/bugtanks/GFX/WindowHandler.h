/**
 * @file
 * @author Martin <mail@mail.nu>, Victor Lundberg <lundberg.victor@gmail.com>
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

#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include "WindowData.h"
#include "DXBase.h"

namespace gfx
{
	/**
	 * Class for creating and managing a window.
	 */
	class WindowHandler : public DXBase
	{
		public:
			/**
			 * Default constructor.
			 */
			WindowHandler();

			/**
			 * Default destructor.
			 */
			virtual ~WindowHandler();

			/**
			 * Creates the window.
			 * @param title Title of the window.
			 * @param width Width of the window.
			 * @param height Height of the window.
			 */
			HRESULT initWindow(const char *title, long width, long height);

			/**
			 * Returns a const reference to the HWND.
			 * @return Const reference to mWindow.
			 */
			inline const HWND &getHWND() const {return mWindow;}

			/**
			 * Returns a pointer to the HWND.
			 * @return Pointer to mWindow.
			 */
			inline HWND *getHWNDPointer(){return &mWindow;}

			/**
			 * Returns a const reference to the instance handle.
			 * @return Const reference to mInstance.
			 */
			inline const HINSTANCE &getInstance() const {return mInstance;}

			/**
			 * Returns a pointer to the instance handle.
			 * @return Pointer to mInstance.
			 */
			inline HINSTANCE *getInstancePointer(){return &mInstance;}

		protected:
			/**
			 * Default copy constructor.
			 */
			WindowHandler(const WindowHandler &handler);

			/**
			 * Default assignment operator.
			 */
			const WindowHandler &operator=(const WindowHandler &handler);

		private:
			HWND mWindow;
			HINSTANCE mInstance;
			long mWindowWidth;
			long mWindowHeight;

			/**
			 * Default window proc.
			 */
			static LRESULT WINAPI windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}

#endif