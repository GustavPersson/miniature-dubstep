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

#ifndef DXBASE_H
#define DXBASE_H
#include <d3d10.h>
#include <d3dx10.h>
#include "../Utilities/Macros.h"

namespace gfx
{
	/**
	 * Base class for the DirectX module. Initialises the ID3D10Device.
	 */
	class DXBase
	{
		public:
			/**
			 * Default constructor.
			 */
			DXBase();

			/**
			 * Default destructor.
			 */
			virtual ~DXBase();

			/**
			 * Initialises the ID3D10Device.
			 * @return Returns S_OK or E_FAIL.
			 */
			HRESULT initDevice();

			/**
			 * Retrieves the ID3D10Device.
			 * @return Returns a pointer to the ID3D10Device.
			 */
			inline ID3D10Device *getDevice(){return mpDevice;}

		protected:
			/**
			 * Default copy constructor.
			 */
			DXBase(const DXBase &base);

			/**
			 * Default assignment operator.
			 */
			const DXBase &operator=(const DXBase &base);

		private:
			ID3D10Device *mpDevice;
	};
}

#endif