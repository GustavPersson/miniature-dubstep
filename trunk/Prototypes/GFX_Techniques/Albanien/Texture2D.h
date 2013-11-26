#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <d3d10.h>
#include <d3dx10.h>

#include "Macros.h"

/*
 * Structure for render target views
 */
struct RenderTargetViews
{
	ID3D10DepthStencilView* DSV; /**< Depth Stencil View */
	ID3D10RenderTargetView* RTV; /**< Render Target View */

	/*
	 * Constructor
	 */
	RenderTargetViews()
	{
		DSV = NULL;
		RTV = NULL;
	}

	/*
	 * Destructor
	 */
	~RenderTargetViews()
	{
		SAFE_RELEASE(DSV);
		SAFE_RELEASE(RTV);
	}
};

class Texture2D
{
public:
	ID3D10ShaderResourceView*	mpDepthBufferRV;
	ID3D10ShaderResourceView*	mpColorBufferRV;

	/*
	 * Constructor
	 */
	Texture2D(int width, int height, bool colorMap = true);

	/*
	 * Destructor
	 */
	~Texture2D();

	/*
	 * Allows rendering to texture buffers
	 */
	void begin();
	
	/*
	 * Disables rendering to texture buffers
	 */
	void end();

	/*
	 * Binds device to instance
	 */
	void setDevice(ID3D10Device* device);

	/*
	 * Sets render targets to the ones assigned to the texture buffers
	 */
	void setRenderTargets(RenderTargetViews* RTV);

	/*
	 * Sets viewport
	 */
	void setViewPort(D3D10_VIEWPORT* mViewPort);

	/*
	 * Returns the render target views structure
	 */
	RenderTargetViews* getRenderTargets();

	/*
	 * Returns viewport
	 */
	void getViewPort(D3D10_VIEWPORT& viewPort);

	/*
	 * Creates a depth texture to render to
	 */
	void createDepthTexture();
	
	/*
	 * Creates a color texture to render to
	 */
	void createColorTexture(DXGI_FORMAT format);

	void createBackBuffer(IDXGISwapChain* swapChain);

private:
	int mWidth;
	int mHeight;

	ID3D10Device*				mpDevice;
	D3D10_VIEWPORT				mViewPort;

	RenderTargetViews*			mpRenderTargets; /**< Render targets for this instance */
	RenderTargetViews*			epRenderTargets; /**< Render targets from other instances */
};

#endif