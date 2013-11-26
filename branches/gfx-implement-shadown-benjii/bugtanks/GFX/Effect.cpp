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

#include "Effect.h"

using namespace gfx;
using std::string;

Effect::Effect(const char *effectName, ID3D10Device *pDevice) : mInputLayoutHandler(pDevice)
{
	mpDevice = pDevice;
	mpEffect = NULL;
	mName = effectName;
	mpFXVCameraPosition = NULL;
	mpFXVCameraDirection = NULL;
	mpFXMVP = NULL;
	mpFXMV = NULL;
	mpFXMP = NULL;
	mpFXTextureArray = NULL;
	mpFXShadowMap = NULL;
}

Effect::Effect(const Effect &effect) : mInputLayoutHandler(effect.mpDevice)
{
	mpDevice = effect.mpDevice;
	mpEffect = effect.mpEffect;
	mName = effect.mName;
	mpFXVCameraPosition = NULL;
	mpFXVCameraDirection = NULL;
	mpFXMVP = NULL;
	mpFXMV = NULL;
	mpFXMP = NULL;
	mpFXTextureArray = NULL;
	mpFXShadowMap = NULL;
}

Effect::~Effect()
{
	if(mpEffect)
	{
		mpEffect->Release();
		mpEffect = NULL;
	}
}

const Effect &Effect::operator=(const Effect &effect)
{
	if(this != &effect)
	{
		mpDevice = effect.mpDevice;
		mpEffect = effect.mpEffect;
		mName = effect.mName;
	}
	return *this;
}

HRESULT Effect::createEffect(const char *fileName)
{	
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	#ifdef _DEBUG
	{
		shaderFlags = D3D10_SHADER_DEBUG;
	}
	#endif

	if(FAILED(D3DX10CreateEffectFromFile(fileName, NULL, NULL, 
						"fx_4_0", shaderFlags, 0, mpDevice, NULL, 
						NULL, &mpEffect, NULL, NULL)))
	{
		DEBUG_MESSAGE("Error: GFX: Effect: Error creating effect from file.")
		return E_FAIL;
	}

	mpFXViewportSize = mpEffect->GetVariableByName("ViewportSize");
	mpFXVCameraPosition = mpEffect->GetVariableByName("VCameraPosition");
	mpFXVCameraDirection = mpEffect->GetVariableByName("VCameraDirection");
	mpFXLVP = mpEffect->GetVariableByName("LVP")->AsMatrix();
	mpFXLV = mpEffect->GetVariableByName("LV")->AsMatrix();
	mpFXLP = mpEffect->GetVariableByName("LP")->AsMatrix();
	mpFXMVP = mpEffect->GetVariableByName("MVP")->AsMatrix();
	mpFXMV = mpEffect->GetVariableByName("MV")->AsMatrix();
	mpFXMP = mpEffect->GetVariableByName("MP")->AsMatrix();
	mpFXMOrtP = mpEffect->GetVariableByName("MOrtP")->AsMatrix();
	mpFXTextureArray = mpEffect->GetVariableByName("TextureArray")->AsShaderResource();
	mpFXShadowMap = mpEffect->GetVariableByName("ShadowMap")->AsShaderResource();

	
	mpFXAnimations = mpEffect->GetVariableByName("animations")->AsShaderResource();

	ID3D10EffectVariable * joints = mpEffect->GetVariableByName("g_joints");

	float tmp[96];

	for(int i=0;i<24;i++)
	{
		tmp[i*4] = 0;
		tmp[i*4+1] = 0;
		tmp[i*4+2] = 0;
		tmp[i*4+3] = 0;
	}

	joints->SetRawValue(&tmp,0,96);

	ID3D10EffectVariable * weights = mpEffect->GetVariableByName("g_weights");

	for(int i=0;i<24;i++)
	{
		tmp[i*4] = 0;
		tmp[i*4+1] = 0;
		tmp[i*4+2] = 0;
		tmp[i*4+3] = 0;
	}

	tmp[0] = 0;

	weights->SetRawValue(&tmp,0,96);

	return S_OK;
}

HRESULT Effect::createInputLayout(unsigned int techIndex, VertexTypes::Types vertexType)
{
	return mInputLayoutHandler.createInputLayout(mpEffect, techIndex, vertexType);
}

void Effect::updateViewportSize(const D3DXVECTOR2 &size)
{
	mpFXViewportSize->SetRawValue((void *)&size, 0, sizeof(D3DXVECTOR2));
}

void Effect::updateLightMatrices(const D3DXMATRIX &V, const D3DXMATRIX &P)
{
	mpFXLVP->SetMatrix((float *)&(V * P));
	mpFXLV->SetMatrix((float *)&V);
	mpFXLP->SetMatrix((float *)&P);
}


void Effect::updateVCameraPosition(const D3DXVECTOR3 &position)
{
	mpFXVCameraPosition->SetRawValue((void *)&position, 0, sizeof(D3DXVECTOR3));
}

void Effect::updateVCameraDirection(const D3DXVECTOR3 &direction)
{
	mpFXVCameraDirection->SetRawValue((void *)&direction, 0, sizeof(D3DXVECTOR3));
}

void Effect::updateCameraMatrices(const D3DXMATRIX &V, const D3DXMATRIX &P, const D3DXMATRIX &ortP)
{
	mpFXMVP->SetMatrix((float *)&(V * P));
	mpFXMV->SetMatrix((float *)&V);
	mpFXMP->SetMatrix((float *)&P);
	mpFXMOrtP->SetMatrix((float *)&ortP);
}

void Effect::updateTextureArray(ID3D10ShaderResourceView *pTexture)
{
	mpFXTextureArray->SetResource(pTexture);
}

void Effect::updateAnimations(ID3D10ShaderResourceView *pTexture)
{
	mpFXAnimations->SetResource(pTexture);
}