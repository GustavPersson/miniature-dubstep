/**
 * @file
 * @author Carl Nordlander <cand05@student.bth.se>
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

#include "Texture.h"

using namespace gfx;
using namespace std;

Texture::Texture(ID3D10ShaderResourceView* pGPUTexture,ID3D10Texture2D* pCPUTexture,std::string name)
{
	mpGPUTexture = pGPUTexture;
	mpCPUTexture = pCPUTexture;
	mName = name;
}

Texture::~Texture()
{
	mpGPUTexture->Release();
	mpCPUTexture->Release();
}

ID3D10ShaderResourceView* Texture::getGPUTexture()
{
	return mpGPUTexture;
}

ID3D10Texture2D* Texture::getCPUTexture()
{
	return mpCPUTexture;
}

string Texture::getName()
{
	return mName;
}