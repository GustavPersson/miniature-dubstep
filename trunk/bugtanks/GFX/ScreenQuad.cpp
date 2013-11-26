/**
* @file
* @author Benjamin Golba <benjii.g@gmail.com>
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

#include "ScreenQuad.h"
#include <iostream>

ScreenQuad::ScreenQuad(ID3D10Device* device)
{
	mpDevice		= device;
	mNumVertices	= 4;

	createFX();
	fxTechnique = fxEffect->GetTechniqueByName("RenderQuad");
	mTexture	= fxEffect->GetVariableByName("mTexture")->AsShaderResource();
}

ScreenQuad::~ScreenQuad()
{
}

void ScreenQuad::createFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
  
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	D3DX10CreateEffectFromFile("../../Shaders/BasicQuad.fx", 
							NULL, 
							NULL, 
							"fx_4_0",
							shaderFlags, 
							0, 
							mpDevice, 
							NULL, 
							NULL, 
							&fxEffect, 
							&compilationErrors,
							&hr);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			SAFE_RELEASE(compilationErrors);
		}

	}
}

void ScreenQuad::bindTexture(ID3D10ShaderResourceView* texture)
{
	mTexture->SetResource(texture);
}

void ScreenQuad::render()
{
	mpDevice->IASetInputLayout(NULL);
	mpDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mpDevice->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	
	fxTechnique->GetPassByIndex(0)->Apply(0);
	mpDevice->Draw(mNumVertices, 0);
}