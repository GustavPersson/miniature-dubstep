#ifndef _INPUT__H
#define _INPUT__H

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define KEYDOWN(buffer, key)    (buffer[key]			& 0x80)
#define BUTTONDOWN(buffer, key) (buffer.rgbButtons[key] & 0x80)

#define UINPUT_MOUSE     0x01
#define UINPUT_KEYBOARD  0x02

/**
*	Input class. Interface to access user input from keyboard and mouse.
*	@author Simon Sönnby
*/
class Input
{
private:
	BYTE mInputs;				/**< Store the types of inputs. */
	BYTE mKBBuffer[256];		/**< Store the keyboard inputs. */
	DIMOUSESTATE mMouseBuffer;	/**< Store the mouse inputs. */
	
	LPDIRECTINPUT8		 mDIObj;
	LPDIRECTINPUTDEVICE8 mDIDKeyboard;	
	LPDIRECTINPUTDEVICE8 mDIDMouse;		/**< Mouse device. Can handle up to 4 buttons. */

public:
	/**
	*	Constructor.
	*	@param hInst. Engines HINSTANCE.
	*	@param hWnd. Engines HWND.
	*	@param inputs. Devices to access. Default MOUSE and KEYBOARD
	*/
	Input( const HINSTANCE* hInst, const HWND* hWnd, const BYTE inputs = UINPUT_MOUSE | UINPUT_KEYBOARD );

	/**
	*	Default destructor
	*/ 
	~Input();

	/**
	*	Update the input states.
	*/ 
	void Update();

	/**
	*	KeyDown. Return true if the param key is pressed.
	*	@param key. Key to check.
	*	@return True if the key was pressed.
	*/ 
	BOOL KeyDown( const BYTE& key );

	/**
	*	ButtonDown. Return true if the param button is pressed.
	*	@param button. Button to check.
	*	@return True if the button was pressed.
	*/ 
	BOOL ButtonDown( const BYTE& button );

	/**
	*	Set the param to delta movement and return true if the mouse was moved. The Z-Coordinate is
	*	not used here.
	*	@param dx. The mouse movement in delta x.
	*	@param dy. The mouse movement in delta y.
	*	@return True if the mouse was moved.
	*/ 
	bool MouseMovement( float& dx, float& dy );
};

#endif