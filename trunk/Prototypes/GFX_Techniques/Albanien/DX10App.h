#pragma once

#include <windows.h>
#include <stdio.h>
#include <d3d10.h>
#include <d3dx10.h>

#include "Vertex.h"
#include "Texture2D.h"

class DX10App
{
private:
	HWND					hMainWnd;
	HINSTANCE				hInstance;

	ID3D10Device*			gDevice;
	IDXGISwapChain*			mSwapChain;

	ID3D10RasterizerState*	mSolidMode;		/**< Used to set the rendering behavior. */
	ID3D10RasterizerState*	mWireframeMode;	/**< Used to set the rendering behavior. */

	int						mWidth;
	int						mHeight;

	bool					mShowMouse;

	bool Init(char* title, int width, int height);

public:
	DX10App(char* title, int width, int height, bool hideMouse);
	~DX10App();

	int Run();

	void Start();	// Clear views
	void End();		// Swap buffers

	ID3D10Device*	getDevice();
	HINSTANCE*		getHINSTANCE();
	HWND*			getHWND();
	Texture2D*		mSwapBuffers;
};