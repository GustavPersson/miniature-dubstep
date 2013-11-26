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

#ifndef RENDERABLE_TEX_H
#define RENDERABLE_TEX_H

#include <d3dx10.h>
#include "../Utilities/Macros.h"

namespace gfx
{
	/**
	 * Class for creating render target views and depth stencil views.
	 */
	class RenderableTex
	{
	public:
		/**
		 * Default constructor.
		 * @param device Pointer to the ID3D10Device.
		 * @param swapChain Pointer to the IDXGISwapChain.
		 * @param width Width of the client area.
		 * @param height Height of the client area.
		 */
		RenderableTex(ID3D10Device*	device, IDXGISwapChain* swapChain, UINT width, UINT height);

		/**
		 * Default destructor.
		 */
		~RenderableTex();

		/**
		 * Retrieves the SRV for the RTV.
		 * @return Returns the SRV for the RTV.
		 */
		ID3D10ShaderResourceView* getRenderSRV();

		/**
		 * Retrieves the SRV for the DSV.
		 * @return Returns the SRV for the DSV.
		 */
		ID3D10ShaderResourceView* getDepthSRV();

		/**
		 * Retrieves the RTV.
		 * @return Returns the RTV.
		 */
		ID3D10RenderTargetView* getRenderTargetView();

		/**
		 * Retrieves the DSV.
		 * @return Returns the DSV.
		 */
		ID3D10DepthStencilView*	getDepthStencilView();

		/**
		 * Retrieves the view port.
		 * @return Returns a const reference to the view port.
		 */
		inline const D3D10_VIEWPORT &getViewPort() const {return mViewPort;}

		/**
		 * Creates the RTV as the backbuffer to the swap chain.
		 * @return Return S_OK or E_FAIL.
		 */
		HRESULT createBackBuffer();

		/**
		 * Creates the RTV as a Texture2D.
		 * @return Returns S_OK or E_FAIL.
		 */
		HRESULT createRenderTarget();

		/**
		 * Creates the DSV as a Texture2D.
		 * @return Returns S_OK or E_FAIL.
		 */
		HRESULT createDepthStencil();

	protected:
		/**
		 * Default copy constructor.
		 */
		RenderableTex(const RenderableTex &renderTex);

		/**
		 * Default assignment operator.
		 */
		const RenderableTex &operator=(const RenderableTex &renderTex);

	private:
		ID3D10RenderTargetView*				mpRTV;
		ID3D10DepthStencilView*				mpDSV;
		ID3D10ShaderResourceView*			mpRenderSRV;
		ID3D10ShaderResourceView*			mpDepthSRV;
		D3D10_VIEWPORT						mViewPort;

		ID3D10Device*						mpDevice;
		IDXGISwapChain*						mpSwapChain;

		UINT								mWidth;
		UINT								mHeight;
	};
}

#endif