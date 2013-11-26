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

#include "RenderableTex.h"

using namespace gfx;

RenderableTex::RenderableTex(ID3D10Device*	device, IDXGISwapChain* swapChain, UINT width, UINT height) : mViewPort()
{
	mpRTV = NULL;
	mpDSV = NULL;
	mpRenderSRV = NULL;
	mpDepthSRV = NULL;
	mpDevice = device;
	mpSwapChain = swapChain;
	mWidth = width;
	mHeight = height;
	mViewPort.Height = height;
	mViewPort.Width = width;
	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
}

RenderableTex::~RenderableTex()
{
	
}

RenderableTex::RenderableTex(const RenderableTex &renderTex) : mViewPort()
{
	mpRTV = renderTex.mpRTV;
	mpDSV = renderTex.mpDSV;
	mpRenderSRV = renderTex.mpRenderSRV;
	mpDepthSRV = renderTex.mpDepthSRV;
	mpDevice = renderTex.mpDevice;
	mpSwapChain = renderTex.mpSwapChain;
	mWidth = renderTex.mWidth;
	mHeight = renderTex.mHeight;
	mViewPort.Height = mHeight;
	mViewPort.Width = mWidth;
	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
}

const RenderableTex &RenderableTex::operator =(const RenderableTex &renderTex)
{
	if(this != &renderTex)
	{
		mpRTV = renderTex.mpRTV;
		mpDSV = renderTex.mpDSV;
		mpRenderSRV = renderTex.mpRenderSRV;
		mpDepthSRV = renderTex.mpDepthSRV;
		mpDevice = renderTex.mpDevice;
		mpSwapChain = renderTex.mpSwapChain;
		mWidth = renderTex.mWidth;
		mHeight = renderTex.mHeight;
		mViewPort.Height = mHeight;
		mViewPort.Width = mWidth;
		mViewPort.TopLeftX = 0;
		mViewPort.TopLeftY = 0;
		mViewPort.MinDepth = 0.0f;
		mViewPort.MaxDepth = 1.0f;
	}
	return *this;
}

ID3D10ShaderResourceView* RenderableTex::getRenderSRV()
{
	return mpRenderSRV;
}

ID3D10ShaderResourceView* RenderableTex::getDepthSRV()
{
	return mpDepthSRV;
}

ID3D10RenderTargetView* RenderableTex::getRenderTargetView()
{
	return mpRTV;
}

ID3D10DepthStencilView* RenderableTex::getDepthStencilView()
{
	return mpDSV;
}

HRESULT RenderableTex::createBackBuffer()
{
	HRESULT hr;
	ID3D10Texture2D* backBuffer;
	hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBuffer);
	if(FAILED(hr))
	{
		DEBUG_MESSAGE("Error: GFX: RenderableTex::createBackBuffer(). GetBuffer().")
		return hr;
	}

	hr = mpDevice->CreateRenderTargetView(backBuffer, NULL, &mpRTV);
	if(FAILED(hr))
	{
		DEBUG_MESSAGE("Error: GFX: RenderableTex::createBackBuffer(). CreateRenderTargetView().")
		return hr;
	}
	SAFE_RELEASE(backBuffer);

	return S_OK;
}

HRESULT RenderableTex::createRenderTarget()
{
	HRESULT hr;
	D3D10_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D10_TEXTURE2D_DESC));
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.Width = mWidth;

	ID3D10Texture2D *temp;

	mpDevice->CreateTexture2D(&texDesc, 0, &temp);
	hr = mpDevice->CreateRenderTargetView(temp, NULL, &mpRTV);
	if(FAILED(hr))
	{
		DEBUG_MESSAGE("Error: GFX: RenderableTex::createRenderTarget(). CreateRenderTargetView().")
		return hr;
	}
	
	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;

	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	hr = mpDevice->CreateShaderResourceView(temp, &viewDesc, &mpRenderSRV);
	if(FAILED(hr))
	{
		DEBUG_MESSAGE("Error: GFX: RenderableTex::createRenderTarget(). CreateShaderResourceView().")
		return hr;
	}

	SAFE_RELEASE(temp);

	return S_OK;
}

HRESULT RenderableTex::createDepthStencil()
{
	HRESULT hr;
	D3D10_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D10_TEXTURE2D_DESC));
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.Width = mWidth;

	ID3D10Texture2D *temp;

	mpDevice->CreateTexture2D(&texDesc, 0, &temp);

	D3D10_DEPTH_STENCIL_VIEW_DESC depthDesc;
	depthDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Texture2D.MipSlice = 0;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;

	hr = mpDevice->CreateDepthStencilView(temp, &depthDesc, &mpDSV);
	if(FAILED(hr))
	{
		DEBUG_MESSAGE("Error: GFX: RenderableTex::createDepthStencil(). CreateDepthStencilView().")
		return hr;
	}
	
	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;

	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Format = DXGI_FORMAT_R32_FLOAT;

	hr = mpDevice->CreateShaderResourceView(temp, &viewDesc, &mpDepthSRV);
	if(FAILED(hr))
	{
		DEBUG_MESSAGE("Error: GFX: RenderableTex::createDepthStencil(). CreateShaderResourceView().")
		return hr;
	}

	SAFE_RELEASE(temp);
	
	return S_OK;
}