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

#include "DXBase.h"

using namespace gfx;

DXBase::DXBase()
{
	mpDevice = NULL;
}

DXBase::DXBase(const DXBase &base)
{
	base;
}

DXBase::~DXBase()
{
	if(mpDevice)
	{
		mpDevice->Release();
		mpDevice = NULL;
	}
}

const DXBase &DXBase::operator=(const DXBase &base)
{
	if(this != &base)
	{

	}
	return *this;
}

HRESULT DXBase::initDevice()
{
	UINT createDeviceFlags = 0;

	#ifdef _DEBUG
		createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
	#endif

	D3D10_DRIVER_TYPE driverType = D3D10_DRIVER_TYPE_NULL;
	D3D10_DRIVER_TYPE driverTypes[] =
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverType = sizeof(driverTypes) / sizeof(driverTypes[0]);

	for(UINT driverTypeIndex = 0; driverTypeIndex < numDriverType; ++driverTypeIndex)
	{
		driverType = driverTypes[driverTypeIndex];
		if(SUCCEEDED(D3D10CreateDevice(NULL, driverType, NULL, createDeviceFlags, D3D10_SDK_VERSION, &mpDevice)))
		{
			break;
		}
	}
	return S_OK;
}