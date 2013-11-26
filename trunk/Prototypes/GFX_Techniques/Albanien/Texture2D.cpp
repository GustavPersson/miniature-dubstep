#include "Texture2D.h"

Texture2D::Texture2D(int width, int height, bool colorMap)
{
	// Set variables
	mWidth		= width;
	mHeight		= height;

	mViewPort.Width		= mWidth;
	mViewPort.Height	= mHeight;
	mViewPort.MinDepth	= 0.0f;
	mViewPort.MaxDepth	= 1.0f;
	mViewPort.TopLeftX	= 0;
	mViewPort.TopLeftY	= 0;

	mpRenderTargets		= new RenderTargetViews;
	mpDepthBufferRV		= NULL;
	mpColorBufferRV		= NULL;
}

Texture2D::~Texture2D()
{
	SAFE_DELETE(mpRenderTargets);

//	SAFE_RELEASE(mpDepthBufferRV);
	SAFE_RELEASE(mpColorBufferRV);
}

void Texture2D::begin()
{
	ID3D10RenderTargetView* renderTargets[1] = {mpRenderTargets->RTV};

	// Save current render targets
	//mpDevice->OMGetRenderTargets(1, &epRenderTargets->RTV, &epRenderTargets->DSV);

	// Replace them with new ones for the shadow map
	mpDevice->OMSetRenderTargets(1, &mpRenderTargets->RTV, mpRenderTargets->DSV);
	mpDevice->RSSetViewports(1, &mViewPort);

	if(mpRenderTargets->RTV)
		mpDevice->ClearRenderTargetView(mpRenderTargets->RTV, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	mpDevice->ClearDepthStencilView(mpRenderTargets->DSV, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
}

void Texture2D::end()
{
	//ID3D10RenderTargetView* mpRenderTargets[1] = {epRenderTargets->RTV};
	//mpDevice->OMSetRenderTargets(1, &epRenderTargets->RTV, epRenderTargets->DSV);
}

void Texture2D::setDevice(ID3D10Device* device)
{
	mpDevice = device;
}

void Texture2D::setRenderTargets(RenderTargetViews* RTV)
{
	mpRenderTargets = RTV;
}

void Texture2D::setViewPort(D3D10_VIEWPORT* viewPort)
{
	mViewPort.Width		= viewPort->Width;
	mViewPort.Height	= viewPort->Height;
	mViewPort.MinDepth	= viewPort->MinDepth;
	mViewPort.MaxDepth	= viewPort->MaxDepth;
	mViewPort.TopLeftX	= viewPort->TopLeftX;
	mViewPort.TopLeftY	= viewPort->TopLeftY;

	mpDevice->RSSetViewports(1, &mViewPort);
}

RenderTargetViews* Texture2D::getRenderTargets()
{
	return mpRenderTargets;
}

void Texture2D::getViewPort(D3D10_VIEWPORT& viewPort)
{
	viewPort.Width		= mViewPort.Width;
	viewPort.Height		= mViewPort.Height;
	viewPort.MinDepth	= mViewPort.MinDepth;
	viewPort.MaxDepth	= mViewPort.MaxDepth;
	viewPort.TopLeftX	= mViewPort.TopLeftX;
	viewPort.TopLeftY	= mViewPort.TopLeftY;
}

void Texture2D::createDepthTexture()
{
	ID3D10Texture2D* mpDepthBuffer;

	// Create a depth texture
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width					= mWidth;
	descDepth.Height				= mHeight;
	descDepth.MipLevels				= 1;
	descDepth.ArraySize				= 1;
	descDepth.Format				= DXGI_FORMAT_R32_TYPELESS;
	descDepth.SampleDesc.Count		= 1;
	descDepth.SampleDesc.Quality	= 0;
	descDepth.Usage					= D3D10_USAGE_DEFAULT;
	descDepth.BindFlags				= D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags		= 0;
	descDepth.MiscFlags				= 0;
	
	if(FAILED(mpDevice->CreateTexture2D(&descDepth, NULL, &mpDepthBuffer)))
	{
		DEBUG_WINDOW("Failed to create depth texture");
		return;
	}

	// Create the depth stencil view
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format				= DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension		= D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice	= 0;
	if(FAILED(mpDevice->CreateDepthStencilView(mpDepthBuffer, &descDSV, &mpRenderTargets->DSV)))
	{
		DEBUG_WINDOW("Failed to create depth stencil view");
		return;
	}

	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format						= DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension				= D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels			= descDepth.MipLevels;
	srvDesc.Texture2D.MostDetailedMip	= 0;
	if(FAILED(mpDevice->CreateShaderResourceView(mpDepthBuffer, &srvDesc, &mpDepthBufferRV)))
	{
		DEBUG_WINDOW("Failed to create depth buffer shader resource view");
		return;
	}

	SAFE_RELEASE(mpDepthBuffer);
}

void Texture2D::createColorTexture(DXGI_FORMAT format)
{
	ID3D10Texture2D* mpColorBuffer;

	// Create a color texture
	D3D10_TEXTURE2D_DESC descColor;
	descColor.Width					= mWidth;
	descColor.Height				= mHeight;
	descColor.MipLevels				= 1;
	descColor.ArraySize				= 1;
	descColor.Format				= format;
	descColor.SampleDesc.Count		= 1;
	descColor.SampleDesc.Quality	= 0;
	descColor.Usage					= D3D10_USAGE_DEFAULT;
	descColor.BindFlags				= D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	descColor.CPUAccessFlags		= 0;
	descColor.MiscFlags				= D3D10_RESOURCE_MISC_GENERATE_MIPS;
	
	// Create the color texture
	if(FAILED(mpDevice->CreateTexture2D(&descColor, NULL, &mpColorBuffer)))
	{
		DEBUG_WINDOW("Failed to create color texture");
		return;
	}

	// Create a render target view for color map
	if(FAILED(mpDevice->CreateRenderTargetView(mpColorBuffer, NULL, &mpRenderTargets->RTV)))
	{
		DEBUG_WINDOW("Failed to render target view for the color texture");
		return;
	}

	// Create the shader resource view
	if(FAILED(mpDevice->CreateShaderResourceView(mpColorBuffer, NULL, &mpColorBufferRV)))
	{
		DEBUG_WINDOW("Failed to create color shader resource view");
		return;
	}

	SAFE_RELEASE(mpColorBuffer);
}

void Texture2D::createBackBuffer(IDXGISwapChain* swapChain)
{
	ID3D10Texture2D* pBackBuffer;
	if(FAILED(swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer)))
		return;

	if(FAILED(mpDevice->CreateRenderTargetView(pBackBuffer, NULL, &mpRenderTargets->RTV)))
	{
		MessageBox(0, "Failed to create mRenderTargetView", 0, 0);
		return;
	}

	mpDevice->OMSetRenderTargets(1, &mpRenderTargets->RTV, NULL);
	SAFE_RELEASE(pBackBuffer);
}