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

#ifndef RENDER_HANDLER_H
#define RENDER_HANDLER_H
#include <d3d10.h>
#include <d3dx10.h>
#include "../Utilities/Macros.h"
#include <vector>
#include "RenderObject.h"
#include "RenderableTex.h"
#include "ShaderHandler.h"
#include "Camera.h"

namespace gfx
{
	/**
	 * Main renderer for the GFX component.
	 */
	class RenderHandler
	{
		public:
			/**
			 * Default constructor.
			 */
			RenderHandler();

			/**
			 * Default destructor.
			 */
			virtual ~RenderHandler();

			/**
			 * Initialises the swap chain, backbuffer and standard settings.
			 * @param hwnd Const reference to the HWND.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @return Returns S_OK or E_FAIL.
			 */
			HRESULT initRenderer(const HWND &hwnd, ID3D10Device *pDevice, bool swapChain = true);

			/**
			 * Starts rendering to the backbuffer.
			 */
			void start();

			/**
			 * Renders. Does nothing at the moment.
			 */
			void render();

			/**
			 * Renders a vector of objects.
			 * @param renderObjects Const reference to a vector holding RenderObject pointers.
			 */
			void renderVector(const std::vector<RenderObject *> &renderObjects);

			/**
			 * Renders a vector of objects which should be shadowed.
			 * @param renderObjects Const reference to a vector holding RenderObject pointers.
			 */
			void renderShadowVector(const std::vector<RenderObject *> &renderObjects);

			/**
			 * Ends rendering to the backbuffer and presents the image.
			 */
			void end();
			
			RenderableTex*						mpBackBuffer; // temp

		protected:
			/**
			 * Default copy constructor.
			 */
			RenderHandler(const RenderHandler &handler);

			/**
			 * Default assignment operator.
			 */
			const RenderHandler &operator=(const RenderHandler &handler);

		private:
			/**
			 * Creates the swap chain.
			 * @param hwnd Const reference to the HWND.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @return Returns S_OK or E_FAIL.
			 */
			HRESULT createSwapChain(const HWND &hwnd, ID3D10Device *pDevice);

			/**
			 * Creates the standard rasterizer states.
			 * @return Returns S_OK or E_FAIL.
			 */
			HRESULT createRasterStates();

			ID3D10Device*						mpDevice;
			IDXGISwapChain*						mpSwapChain;
			ID3D10RasterizerState*				mpStateSolid;
			D3D10_VIEWPORT						mViewPort;
			//RenderableTex*						mpBackBuffer;
			bool								mFlagWindowed;
	};
}

#endif