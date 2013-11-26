/**
 * @file
 * @author Martin Jönsson <snowman.mrgames@gmail.com>, Victor Lundberg <lundberg.victor@gmail.com>
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

#include "RenderHandler.h"

using namespace gfx;
using std::vector;

RenderHandler::RenderHandler()
{
	mpSwapChain = NULL;
	mpStateSolid = NULL;
	mFlagWindowed = true;
	mpBackBuffer = NULL;
}

RenderHandler::RenderHandler(const RenderHandler &handler)
{
	//Unrefer parameter.
	handler;
}

RenderHandler::~RenderHandler()
{
	if(mpSwapChain)
	{
		mpSwapChain->Release();
		mpSwapChain = NULL;
	}
	SAFE_RELEASE(mpStateSolid);
	SAFE_DELETE(mpBackBuffer);
}

const RenderHandler &RenderHandler::operator=(const RenderHandler &handler)
{
	if(this != &handler)
	{

	}
	return *this;
}

void RenderHandler::start()
{
	mpDevice->RSSetViewports(1, &mpBackBuffer->getViewPort());

	ID3D10RenderTargetView *RTV = mpBackBuffer->getRenderTargetView();
	mpDevice->OMSetRenderTargets(1, &RTV, mpBackBuffer->getDepthStencilView());

	mpDevice->ClearRenderTargetView(mpBackBuffer->getRenderTargetView(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	mpDevice->ClearDepthStencilView(mpBackBuffer->getDepthStencilView(), D3D10_CLEAR_DEPTH, 1.0f, 0);
}

void RenderHandler::render()
{

}

void RenderHandler::renderVector(const std::vector<RenderObject *> &renderObjects)
{
	for(vector<RenderObject *>::const_iterator n = renderObjects.begin();
		n != renderObjects.end(); ++n)
	{
		(*n)->render();
	}
}

void RenderHandler::renderShadowVector(const std::vector<RenderObject *> &renderObjects)
{
	for(vector<RenderObject *>::const_iterator n = renderObjects.begin();
		n != renderObjects.end(); ++n)
	{
		if((*n)->getFlagShadow())
		{
			int oldPass = (*n)->getEffectPassIndex();
			//Animated meshes should use the SHADOW_ANIMATION pass.
			if((*n)->getType() == RenderObject::RenderTypes_AnimationObject)
			{
				(*n)->setEffectPassIndex(RenderObject::ShaderPasses_ShadowAnimation);
			}
			else
			{
				(*n)->setEffectPassIndex(RenderObject::ShaderPasses_Shadow);
			}
			(*n)->render();
			(*n)->setEffectPassIndex(oldPass);
		}
	}
}

void RenderHandler::end()
{
	if(mpSwapChain)
	{
		mpSwapChain->Present(1, 0);
	}
}

HRESULT RenderHandler::initRenderer(const HWND &hwnd, ID3D10Device *pDevice, bool swapChain)
{
	mpDevice = pDevice;
	if(swapChain)
	{
		if(FAILED(createSwapChain(hwnd, pDevice)))
		{
			return E_FAIL;
		}
	}

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	if(swapChain)
		mpBackBuffer = myNew RenderableTex(mpDevice, mpSwapChain, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	else
		mpBackBuffer = myNew RenderableTex(mpDevice, mpSwapChain, 2048, 2048);

	if(swapChain)
	{
		if(FAILED(mpBackBuffer->createBackBuffer()))
		{
			return E_FAIL;
		}
	}
	else
	{
		if(FAILED(mpBackBuffer->createRenderTarget()))
		{
			return E_FAIL;
		}
	}

	if(swapChain)
	{
	
		if(FAILED(mpBackBuffer->createDepthStencilMS()))
		{
			return E_FAIL;
		}
	}
	else
	{
		if(FAILED(mpBackBuffer->createDepthStencil()))
		{
			return E_FAIL;
		}
	}

	mpDevice->RSSetState(mpStateSolid);
	return S_OK;
}

HRESULT RenderHandler::createSwapChain(const HWND &hwnd, ID3D10Device *pDevice)
{
	//Retrieve the IDXGIFactory.
	IDXGIDevice *pDXGIDevice = NULL;
	if(FAILED(pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice)))
	{
		ERROR_MESSAGE("Failed to retrieve the IDXGIDevice.")
		return E_FAIL;
	}

	IDXGIAdapter *pDXGIAdapter = NULL;
	if(FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter)))
	{
		ERROR_MESSAGE("Failed to retrieve the IDXGIAdapter.")
		return E_FAIL;
	}

	IDXGIFactory *pDXGIFactory;
	if(FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pDXGIFactory)))
	{
		ERROR_MESSAGE("Failed to retrieve the IDXGIFactory.")
		return E_FAIL;
	}

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	DXGI_SWAP_CHAIN_DESC scDesc;
	scDesc.BufferCount = 2;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.Width = clientRect.right - clientRect.left;
	scDesc.BufferDesc.Height = clientRect.bottom - clientRect.top;
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.OutputWindow = hwnd;
	scDesc.Flags = 0;
	scDesc.SampleDesc.Count = 8;
	scDesc.SampleDesc.Quality = 0;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Windowed = mFlagWindowed;

	if(FAILED(pDXGIFactory->CreateSwapChain(pDevice, &scDesc, &mpSwapChain)))
	{
		ERROR_MESSAGE("Failed to create swap chain.")
		return E_FAIL;
	}

	pDXGIFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES);

	/*if(pDXGIFactory)
	{
		pDXGIFactory->Release();
		pDXGIFactory = NULL;
	}*/


	return S_OK;
}

HRESULT RenderHandler::createRasterStates()
{
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
    rasterDesc.AntialiasedLineEnable	= false;
	
	if(FAILED(mpDevice->CreateRasterizerState(&rasterDesc, &mpStateSolid)))
	{
		ERROR_MESSAGE("Failed to create rasterizer state.")
		return E_FAIL;
	}
	return S_OK;
}