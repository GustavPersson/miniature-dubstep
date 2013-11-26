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
	mpFXTGameTime = NULL;
	mpFXTDeltaTime = NULL;
	mpFXMVP = NULL;
	mpFXMV = NULL;
	mpFXMP = NULL;
	mpFXTextureArray128 = NULL;
	mpFXTextureArray256 = NULL;
	mpFXTextureArray512 = NULL;
	mpFXTextureArray1024 = NULL;
	mpFXShadowMap = NULL;
	mpFXFontArray = NULL;
	mpFXWorldPlaneArray = NULL;
	mpFXParticleStorage = NULL;
}

Effect::Effect(const Effect &effect) : mInputLayoutHandler(effect.mpDevice)
{
	mpDevice = effect.mpDevice;
	mpEffect = effect.mpEffect;
	mName = effect.mName;
	mpFXVCameraPosition = NULL;
	mpFXVCameraDirection = NULL;
	mpFXTGameTime = NULL;
	mpFXTDeltaTime = NULL;
	mpFXMVP = NULL;
	mpFXMV = NULL;
	mpFXMP = NULL;
	mpFXTextureArray128 = NULL;
	mpFXTextureArray256 = NULL;
	mpFXTextureArray512 = NULL;
	mpFXTextureArray1024 = NULL;
	mpFXShadowMap = NULL;
	mpFXFontArray = NULL;
	mpFXWorldPlaneArray = NULL;
	mpFXParticleStorage = NULL;
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
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_OPTIMIZATION_LEVEL3;
	#ifdef _DEBUG
	{
		shaderFlags |= D3D10_SHADER_DEBUG;
	}
	#endif

	ID3D10Blob* compilationErrors = 0;
	if(FAILED(D3DX10CreateEffectFromFile(fileName, NULL, NULL, 
						"fx_4_0", shaderFlags, 0, mpDevice, NULL, 
						NULL, &mpEffect, &compilationErrors, NULL)))
	{
		ERROR_MESSAGE((char*)compilationErrors->GetBufferPointer());
		ERROR_MESSAGE("Failed to create effect from file.")
		return E_FAIL;
	}

	mpFXTGameTime = mpEffect->GetVariableByName("tGameTime")->AsScalar();
	mpFXTDeltaTime = mpEffect->GetVariableByName("tDeltaTime")->AsScalar();
	mpFXViewportSize = mpEffect->GetVariableByName("ViewportSize");
	mpFXVCameraPosition = mpEffect->GetVariableByName("VCameraPosition");
	mpFXVCameraDirection = mpEffect->GetVariableByName("VCameraDirection");
	mpFXVLight = mpEffect->GetVariableByName("VLight");
	mpFXLVP = mpEffect->GetVariableByName("LVP")->AsMatrix();
	mpFXMVP = mpEffect->GetVariableByName("MVP")->AsMatrix();
	mpFXMV = mpEffect->GetVariableByName("MV")->AsMatrix();
	mpFXMP = mpEffect->GetVariableByName("MP")->AsMatrix();
	mpFXMOrtP = mpEffect->GetVariableByName("MOrtP")->AsMatrix();
	mpFXTextureArray128 = mpEffect->GetVariableByName("TextureArray128")->AsShaderResource();
	mpFXTextureArray256 = mpEffect->GetVariableByName("TextureArray256")->AsShaderResource();
	mpFXTextureArray512 = mpEffect->GetVariableByName("TextureArray512")->AsShaderResource();
	mpFXTextureArray1024 = mpEffect->GetVariableByName("TextureArray1024")->AsShaderResource();
	mpFXShadowMap = mpEffect->GetVariableByName("ShadowMap")->AsShaderResource();
	mpFXFontArray = mpEffect->GetVariableByName("FontArray")->AsShaderResource();
	mpFXWorldPlaneArray = mpEffect->GetVariableByName("WorldPlaneArray")->AsShaderResource();
	mpFXAnimations = mpEffect->GetVariableByName("Animations")->AsShaderResource();
	mpFXParticleStorage	= mpEffect->GetVariableByName("ParticleStorage")->AsShaderResource();
	
	return S_OK;
}

HRESULT Effect::createInputLayout(unsigned int techIndex, unsigned int passIndex, VertexTypes::Types vertexType)
{
	return mInputLayoutHandler.createInputLayout(mpEffect, techIndex, passIndex, vertexType);
}

void Effect::updateGameTime(float gameTime, float deltaTime)
{
	mpFXTGameTime->SetFloat(gameTime);
	mpFXTDeltaTime->SetFloat(deltaTime);
}

void Effect::updateViewportSize(const D3DXVECTOR2 &size)
{
	mpFXViewportSize->SetRawValue((void *)&size, 0, sizeof(D3DXVECTOR2));
}

void Effect::updateVLight(const LightStruct &light)
{
	mpFXVLight->SetRawValue((void *)&light, 0, sizeof(LightStruct));
}

void Effect::updateLightMatrices(const D3DXMATRIX &V, const D3DXMATRIX &P)
{
	D3DXMATRIX VP = V * P;
	mpFXLVP->SetMatrix((float *)&VP);
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
	D3DXMATRIX VP = V * P;
	mpFXMVP->SetMatrix((float *)&VP);
	mpFXMV->SetMatrix((float *)&V);
	mpFXMP->SetMatrix((float *)&P);
	mpFXMOrtP->SetMatrix((float *)&ortP);
}

void Effect::updateTextureArray128(ID3D10ShaderResourceView *pTexture)
{
	if(pTexture)
	{
		mpFXTextureArray128->SetResource(pTexture);
	}
}

void Effect::updateTextureArray256(ID3D10ShaderResourceView *pTexture)
{
	if(pTexture)
	{
		mpFXTextureArray256->SetResource(pTexture);
	}
}

void Effect::updateTextureArray512(ID3D10ShaderResourceView *pTexture)
{
	if(pTexture)
	{
		mpFXTextureArray512->SetResource(pTexture);
	}
}

void Effect::updateTextureArray1024(ID3D10ShaderResourceView *pTexture)
{
	if(pTexture)
	{
		mpFXTextureArray1024->SetResource(pTexture);
	}
}

void Effect::updateFontArray(ID3D10ShaderResourceView *pTexture)
{
	mpFXFontArray->SetResource(pTexture);
}

void Effect::updateWorldPlaneArray(ID3D10ShaderResourceView *pWeightMap)
{
	mpFXWorldPlaneArray->SetResource(pWeightMap);
}

void Effect::updateParticleStorage(ID3D10ShaderResourceView *pParticleStorage)
{
	mpFXParticleStorage->SetResource(pParticleStorage);
}

void Effect::updateAnimations(ID3D10ShaderResourceView *pTexture)
{
	mpFXAnimations->SetResource(pTexture);
}