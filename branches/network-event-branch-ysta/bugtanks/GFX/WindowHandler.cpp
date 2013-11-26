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

#include "WindowHandler.h"

using namespace gfx;

WindowHandler::WindowHandler() : DXBase()
{
	mWindow = NULL;
	mInstance = NULL;
	mWindowWidth = 0;
	mWindowHeight = 0;
}

WindowHandler::WindowHandler(const WindowHandler &handler) : DXBase(handler)
{
	mWindow = handler.mWindow;
	mInstance = handler.mInstance;
	mWindowWidth = handler.mWindowWidth;
	mWindowHeight = handler.mWindowHeight;
}

WindowHandler::~WindowHandler()
{

}

const WindowHandler &WindowHandler::operator=(const WindowHandler &handler)
{
	if(this != &handler)
	{
		DXBase::operator=(handler);
		mWindow = handler.mWindow;
		mInstance = handler.mInstance;
		mWindowWidth = handler.mWindowWidth;
		mWindowHeight = handler.mWindowHeight;
	}
	return *this;
}

HRESULT WindowHandler::initWindow(const char *title, long width, long height)
{
	mInstance = GetModuleHandle(0);
	mWindowWidth = width;
	mWindowHeight = height;
	
	WNDCLASS windowClass;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc	= WindowHandler::windowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = mInstance;
	windowClass.hIcon = LoadIcon(mInstance, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(mInstance, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "MainWindow";
	if(!RegisterClass(&windowClass))
	{
		return E_FAIL;
	}

	mWindow = CreateWindowA("MainWindow", "BugTanks", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
							CW_USEDEFAULT, CW_USEDEFAULT, mWindowWidth, mWindowHeight, NULL, NULL, mInstance, NULL);
	if(!mWindow)
	{
		return E_FAIL;
	}

	ShowWindow(mWindow, SW_SHOW);
	return S_OK;
}

LRESULT WindowHandler::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			return 0;

		default:
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}