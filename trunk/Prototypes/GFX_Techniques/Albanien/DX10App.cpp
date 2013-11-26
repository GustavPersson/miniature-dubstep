#include "DX10App.h"

LRESULT CALLBACK 
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Handle some specific messages.  Note that if we handle a message,
	// we should return 0.
	switch( msg )
	{
		// In the case the escape key was pressed, then
		// destroy the main application window.
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			DestroyWindow(hWnd);
		return 0;

		// In the case of a destroy message, then send a 
		// quit message, which will terminate the message loop.
	case WM_DESTROY: 
		PostQuitMessage(0); 
		return 0;
	}

	// Forward any other messages we did not handle above to the default window procedure.
	// Note that our window procedure must return the return value of DefWindowProc.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

DX10App::DX10App(char* title, int width, int height, bool showMouse)
{
	HWND hMainWnd	= NULL;
	mShowMouse		= showMouse;
	mSwapBuffers	= new Texture2D(width, height);
	ShowCursor(showMouse);

	Init(title, width, height);
}

DX10App::~DX10App()
{
	gDevice->ClearState();
	
	if(mSwapChain)
		mSwapChain->Release();
	SAFE_DELETE(mSwapBuffers);

	DestroyVertexStructures();
}

bool DX10App::Init(char* title, int width, int height)
{
	hInstance = GetModuleHandle(0);

	InitVertexStructures();

	mWidth	= width;
	mHeight = height;
	// To create a window we need to describe some of
	// it's characteristics by filling out a WNDCLASS structure
	WNDCLASS wc;

	wc.style			= CS_HREDRAW | CS_VREDRAW;		// Repaints window when resized
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;					// Main application instance
	wc.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= "Albanien";
	
	// Registers WNDCLASS with windows so that we can create a window based on it
	if(!RegisterClass(&wc))
	{
		MessageBox(0, "RegisterClass failed.", 0, 0);
		return false;
	}

	// Grab all window border values to adjust the window to the size we want
	RECT rect;
	rect.top	= (long)0;
	rect.left	= (long)(0);
	rect.right	= (long)width;
	rect.bottom	= (long)height;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	hMainWnd = CreateWindow(
				"Albanien",				// Registered WNDCLASS instance to use
				title,					// Window title
				WS_OVERLAPPEDWINDOW,	// Style flags
				200/*CW_USEDEFAULT*/,			// x-coordinate
				200/*CW_USEDEFAULT*/,			// y-coordinate
				rect.right - rect.left,	// Width (adjusted width)
				rect.bottom - rect.top,	// Height (adjusted height)
				0,						// Parent window
				0,						// Menu handle
				hInstance,				// App instance
				0						//
			);

	UINT createDeviceFlags  = 0;
		 createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG; // If debug mode

	D3D10_DRIVER_TYPE driverTypes[] = 
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE
	};

	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount							= 1;
	sd.BufferDesc.Width						= width;
	sd.BufferDesc.Height					= height;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow							= hMainWnd;
	sd.SampleDesc.Count						= 1;
	sd.SampleDesc.Quality					= 0;
	sd.Windowed								= TRUE;

	D3D10_DRIVER_TYPE driverType;
	HRESULT hr;
	// i == driverTypeIndex
	for(UINT i = 0; i < numDriverTypes; i++)
	{
		driverType	= driverTypes[i];
		hr			= D3D10CreateDeviceAndSwapChain(
													NULL,
													driverType,
													NULL,
													createDeviceFlags,
													D3D10_SDK_VERSION,
													&sd,
													&mSwapChain,
													&gDevice
													);

		// If first driver type succeeded, break, else try the other one
		if(SUCCEEDED(hr))
			break;
	}
	printf("Device and swap chain created.\n");
	
	mSwapBuffers->setDevice(gDevice);

	//create rastersizer desc
	D3D10_RASTERIZER_DESC rasterDesc;
	rasterDesc.FillMode					= D3D10_FILL_SOLID;
	rasterDesc.CullMode					= D3D10_CULL_BACK;
    rasterDesc.FrontCounterClockwise	= false;
    rasterDesc.DepthBias				= false;
    rasterDesc.DepthBiasClamp			= 0;
    rasterDesc.SlopeScaledDepthBias		= 0;
    rasterDesc.DepthClipEnable			= true;
    rasterDesc.ScissorEnable			= false;
    rasterDesc.MultisampleEnable		= true;
    rasterDesc.AntialiasedLineEnable	= true;
	gDevice->CreateRasterizerState( &rasterDesc, &mSolidMode );

	rasterDesc.FillMode					= D3D10_FILL_WIREFRAME;
	rasterDesc.CullMode					= D3D10_CULL_BACK;
    rasterDesc.FrontCounterClockwise	= false;
    rasterDesc.DepthBias				= false;
    rasterDesc.DepthBiasClamp			= 0;
    rasterDesc.SlopeScaledDepthBias		= 0;
    rasterDesc.DepthClipEnable			= false;
    rasterDesc.ScissorEnable			= false;
    rasterDesc.MultisampleEnable		= false;
    rasterDesc.AntialiasedLineEnable	= false;
	
	gDevice->CreateRasterizerState( &rasterDesc, &mWireframeMode );
	gDevice->RSSetState(mSolidMode);

	mSwapBuffers->createBackBuffer(mSwapChain);
	printf("Render target view created.\n");

	mSwapBuffers->createDepthTexture();

	// Viewport settings
	D3D10_VIEWPORT vp;
	vp.Width	= width;
	vp.Height	= height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mSwapBuffers->setViewPort(&vp);

	printf("DX10 initiated.\n\n");
	ShowWindow(hMainWnd, 1);

	return true;
}

int DX10App::Run()
{
	MSG mMsg = {0};

	if(PeekMessage(&mMsg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&mMsg);
		DispatchMessage(&mMsg);
	}

	return mMsg.message;
}

void DX10App::Start()
{
	if(!mShowMouse)
	{
		WINDOWPLACEMENT wPos;
		GetWindowPlacement( hMainWnd, &wPos );
		SetCursorPos(wPos.rcNormalPosition.left + 100, wPos.rcNormalPosition.top + 100);
	}

	mSwapBuffers->begin();

	
	//gDevice->ClearDepthStencilView( mSwapBuffers->getRenderTargets()->DSV, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);

	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	gDevice->OMSetBlendState(0, blendFactor, 0xffffffff);
}

void DX10App::End()
{
	mSwapChain->Present(0, 0);
}

ID3D10Device* DX10App::getDevice()
{
	return gDevice;
}

HINSTANCE* DX10App::getHINSTANCE()
{
	return &hInstance;
}

HWND* DX10App::getHWND()
{
	return &hMainWnd;
}