/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
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
 *	Input class interface that uses dinput. Lets you access keyboard and mouse.
 *	This will class will be used through InputState.
 */

#include "InputHandler.h"

using namespace logic;

InputHandler::InputHandler(const HINSTANCE *pHInstance, const HWND *pHWnd, const BYTE inputs)
{
	mDiDKeyboard	= 0;
	mDiDMouse		= 0;
	mInputs			= inputs;

	ZeroMemory(&mMouseBuffer, sizeof(mMouseBuffer));

	//initialize the input object
	HRESULT hr = DirectInput8Create(*pHInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDiObj, NULL);
	if(FAILED(hr))
	{
		MessageBox(0, "DirectInput8Create failed in InputHandler.cpp", 0, 0);
		PostQuitMessage(0);
	}

	if(mInputs & UINPUT_KEYBOARD)
	{
		//initialize the keyboard input device
		hr = mDiObj->CreateDevice(GUID_SysKeyboard, &mDiDKeyboard, NULL);
		if(FAILED(hr))
		{
			MessageBox(0, "Failed to create keyboard device in InputHandler.cpp", 0, 0);
			PostQuitMessage(0);
		}

		hr = mDiDKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if(FAILED(hr))
		{
			MessageBox(0, "Failed to create keyboard dataformat in InputHandler.cpp", 0, 0);
			PostQuitMessage(0);
		}

		hr = mDiDKeyboard->SetCooperativeLevel(*pHWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(FAILED(hr))
		{
			MessageBox(0, "Failed to create keyboard co-op level in InputHandler.cpp", 0, 0);
			PostQuitMessage(0);
		}

		hr = mDiDKeyboard->Acquire();
		if(FAILED(hr))
		{
			MessageBox(0, "Failed to acquire keyboard in InputHandler.cpp", 0, 0);
			PostQuitMessage(0);
		}
	}
	
	if(mInputs & UINPUT_MOUSE)
	{
		//initialize the mouse input device
		hr = mDiObj->CreateDevice(GUID_SysMouse, &mDiDMouse, NULL);
		if(FAILED(hr))
		{
			MessageBox(0, "Failed to create mouse device in InputHandler.cpp", 0, 0);
			PostQuitMessage(0);
		}

		hr = mDiDMouse->SetDataFormat(&c_dfDIMouse);
		if(FAILED(hr))
		{
			MessageBox(0, "Failed to create mouse dataformat in InputHandler.cpp", 0, 0);
			PostQuitMessage(0);
		}

		hr = mDiDMouse->SetCooperativeLevel(*pHWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(FAILED(hr))
		{
			MessageBox(0, "Failed to create mouse co-op level in InputHandler.cpp", 0, 0);
			PostQuitMessage(0);
		}

		hr = mDiDMouse->Acquire();
		if(FAILED(hr))
		{
			MessageBox(0, "Failed to acquire mouse in InputHandler.cpp", 0, 0);
			PostQuitMessage(0);
		}
	}

	update();
}

InputHandler::~InputHandler()
{
	if(mDiObj)
	{
		if(mDiDKeyboard)
		{
			mDiDKeyboard->Unacquire();
			mDiDKeyboard->Release();
			mDiDKeyboard = 0;
		}

		if(mDiDMouse)
		{
			mDiDMouse->Unacquire();
			mDiDMouse->Release();
			mDiDMouse = 0;
		}

		mDiObj->Release();
		mDiObj = 0;
	}
}

void InputHandler::update()
{
	HRESULT hr;
	
	if(mInputs & UINPUT_KEYBOARD)
	{
		//update the state of the keyboard
		hr = mDiDKeyboard->GetDeviceState(sizeof(mKbBuffer), (LPVOID)&mKbBuffer);
		if(FAILED(hr))
		{
			hr = mDiDKeyboard->Acquire();
			while(hr == DIERR_INPUTLOST)
				hr = mDiDKeyboard->Acquire();
		}
	}

	
	if(mInputs & UINPUT_MOUSE)
	{
		//update the state of the mouse
		hr = mDiDMouse->GetDeviceState(sizeof(mMouseBuffer), (LPVOID)&mMouseBuffer);
		if(FAILED(hr))
		{
			hr = mDiDMouse->Acquire();
			while(hr == DIERR_INPUTLOST)
				hr = mDiDMouse->Acquire();
		}
	}
}

bool InputHandler::keyDown(const BYTE &key)
{
	if(!(mInputs & UINPUT_KEYBOARD))
		return false;
	
	return KEYDOWN(mKbBuffer, key);
}

bool InputHandler::buttonDown(const BYTE &button)
{
	if(!(mInputs & UINPUT_MOUSE))
		return false;
	
	return BUTTONDOWN(mMouseBuffer, button-1);
}

bool InputHandler::keyReleased(const BYTE &key)
{
	if(!(mInputs & UINPUT_KEYBOARD))
		return true;
	
	return !KEYDOWN(mKbBuffer, key);
}

bool InputHandler::buttonReleased(const BYTE &button)
{
	if(!(mInputs & UINPUT_MOUSE))
		return true;
	
	return !BUTTONDOWN(mMouseBuffer, button-1);
}

bool InputHandler::mouseMovement(float &dx, float &dy)
{
	if(!(mInputs & UINPUT_MOUSE))
		return false;

	//if the mouse is moved
	else if(mMouseBuffer.lX != 0 || mMouseBuffer.lY != 0)
	{
		//set dx to the moved x value and dy to the moved y value and return true
		dx = (float)mMouseBuffer.lX;
		dy = (float)mMouseBuffer.lY;
		return true;
	}

	return false;
}