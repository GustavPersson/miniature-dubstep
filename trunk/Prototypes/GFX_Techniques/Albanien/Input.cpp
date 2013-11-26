#include "Input.h"
#include "DX10Util.h"

Input::Input( const HINSTANCE* hInst, const HWND* hWnd, const BYTE inputs )
{
	mDIDKeyboard = 0;
	mDIDMouse    = 0;
	mInputs		 = inputs;

	ZeroMemory( &mMouseBuffer, sizeof(mMouseBuffer) );

	// Create input object
	HRESULT hr = DirectInput8Create( *hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDIObj, NULL );
	if( FAILED( hr ) )
	{
		MessageBox(0, "DirectInput8Create failed in Input.cpp...", 0, 0);
		PostQuitMessage(0);
	}

	//////// KEYBOARD ////////
	if( mInputs & UINPUT_KEYBOARD )
	{
		// Create keyboard device
		hr = mDIObj->CreateDevice( GUID_SysKeyboard, &mDIDKeyboard, NULL );
		if( FAILED( hr ) )
		{
			MessageBox(0, "Failed to create keyboard device in Input.cpp...", 0, 0);
			PostQuitMessage(0);
		}

		// Set data formats
		hr = mDIDKeyboard->SetDataFormat( &c_dfDIKeyboard );
		if( FAILED( hr ) )
		{
			MessageBox(0, "Failed to set keyboard dataformat in Input.cpp...", 0, 0);
			PostQuitMessage(0);
		}

		// Set co-op levels
		hr = mDIDKeyboard->SetCooperativeLevel( *hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
		if( FAILED( hr ) )
		{
			MessageBox(0, "Failed to set Keyboard co-op level in Input.cpp...", 0, 0);
			PostQuitMessage(0);
		}

		// Aquire
		hr = mDIDKeyboard->Acquire();
		if( FAILED( hr ) )
		{
			MessageBox(0, "Failed to Aquire Keyboard in Input.cpp...", 0, 0);
			PostQuitMessage(0);
		}
	}

	//////// MOUSE ////////
	if( mInputs & UINPUT_MOUSE )
	{
		// Create mouse device
		hr = mDIObj->CreateDevice( GUID_SysMouse, &mDIDMouse, NULL );
		if( FAILED( hr ) )
		{
			MessageBox(0, "Failed to create mouse device in Input.cpp...", 0, 0);
			PostQuitMessage(0);
		}

		// Set data formats
		hr = mDIDMouse->SetDataFormat( &c_dfDIMouse );
		if( FAILED( hr ) )
		{
			MessageBox(0, "Failed to set mouse dataformat in Input.cpp...", 0, 0);
			PostQuitMessage(0);
		}

		// Set co-op levels
		hr = mDIDMouse->SetCooperativeLevel( *hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
		if( FAILED( hr ) )
		{
			MessageBox(0, "Failed to set Mouse co-op level in Input.cpp...", 0, 0);
			PostQuitMessage(0);
		}

		// Aquire
		hr = mDIDMouse->Acquire();
		if( FAILED( hr ) )
		{
			MessageBox(0, "Failed to Aquire Mouse in Input.cpp...", 0, 0);
			PostQuitMessage(0);
		}
	}

	//Aquire
	Update();
}

Input::~Input()
{
	if( mDIObj )
	{
		if( mDIDKeyboard )
		{
			mDIDKeyboard->Unacquire();
			mDIDKeyboard->Release();
			mDIDKeyboard = 0;
		}

		if( mDIDMouse )
		{
			mDIDMouse->Unacquire();
			mDIDMouse->Release();
			mDIDMouse = 0;
		}
		mDIObj->Release();
		mDIObj = 0;
	}
}

void Input::Update()
{
	HRESULT hr;
	if( mInputs & UINPUT_KEYBOARD )
	{
		hr = mDIDKeyboard->GetDeviceState( sizeof( mKBBuffer ), (LPVOID)&mKBBuffer );
		if( FAILED( hr ) )
		{
			hr = mDIDKeyboard->Acquire();
			while( hr == DIERR_INPUTLOST )
				hr = mDIDKeyboard->Acquire();
		}
	}

	if( mInputs & UINPUT_MOUSE )
	{
		hr = mDIDMouse->GetDeviceState( sizeof( mMouseBuffer ), (LPVOID)&mMouseBuffer );
		if( FAILED( hr ) )
		{
			hr = mDIDMouse->Acquire();
			while( hr == DIERR_INPUTLOST )
				hr = mDIDMouse->Acquire();
		}
	}
}

BOOL Input::KeyDown( const BYTE& key )
{
	if( !(mInputs & UINPUT_KEYBOARD) )
		return FALSE;

	return KEYDOWN( mKBBuffer, key );
}

BOOL Input::ButtonDown( const BYTE& button )
{
	if( !(mInputs & UINPUT_MOUSE) )
		return FALSE;

	return BUTTONDOWN( mMouseBuffer, button );
}

bool Input::MouseMovement( float& dx, float& dy )
{
	if( !(mInputs & UINPUT_MOUSE) )
		return false;
	else if( mMouseBuffer.lX != 0 ||
		mMouseBuffer.lY != 0 )
	{
		dx = (float)mMouseBuffer.lX;
		dy = (float)mMouseBuffer.lY;

		return true;
	}
	return false;
}