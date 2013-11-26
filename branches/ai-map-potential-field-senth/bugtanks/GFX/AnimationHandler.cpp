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

#include "AnimationHandler.h"

using namespace gfx;
using namespace std;

AnimationHandler::AnimationHandler(ID3D10Device * pDevice)
{
	mAnimations = vector<Animation*>();
	mpDevice = pDevice;
}

AnimationHandler::~AnimationHandler()
{
	for(unsigned int i=0;i<mAnimations.size();i++)
	{
		SAFE_DELETE(mAnimations[i]);
	}
}

void AnimationHandler::addAnimation(Animation *pAnimation)
{
	mAnimations.push_back(pAnimation);
}

ID3D10ShaderResourceView *AnimationHandler::getTextureArray() const
{
	ID3D10Texture2D * pTextureArray = NULL;

	D3D10_TEXTURE2D_DESC arrayDesc;
	ZeroMemory(&arrayDesc, sizeof(D3D10_TEXTURE2D_DESC));

	arrayDesc.Width = 1024;
	arrayDesc.Height = 1024;
	arrayDesc.MipLevels = 1;
	arrayDesc.Format = DXGI_FORMAT_R32_FLOAT;
	arrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	arrayDesc.Usage = D3D10_USAGE_DEFAULT;
	arrayDesc.ArraySize = mAnimations.size();
	arrayDesc.CPUAccessFlags = 0;
	arrayDesc.SampleDesc.Count = 1;
	arrayDesc.SampleDesc.Quality = 0;
	arrayDesc.MiscFlags = 0;

	if(FAILED(mpDevice->CreateTexture2D(&arrayDesc, NULL, &pTextureArray)))
	{
		ERROR_MESSAGE("AnimationHandler: Failed to create texture array.");
		return NULL;
	}

	for(unsigned int animationIndex=0; animationIndex<mAnimations.size(); animationIndex++)
	{
		unsigned int nrOfJoints = mAnimations[animationIndex]->getFrame(0)->getSize();

		float * pTexels = myNew float[1024 * 1024];

		for(unsigned int i=0;i<1024*1024;i++)
		{
			pTexels[i] = 0.0f;
		}

		pTexels[0] = (float)(mAnimations[animationIndex]->getSize() + 0.01f);
		pTexels[1] = mAnimations[animationIndex]->getTimePerFrame();

		for(unsigned int frameIndex = 0; frameIndex < mAnimations[animationIndex]->getSize(); frameIndex++)
		{
			for(unsigned int matrixIndex = 0; matrixIndex < nrOfJoints; matrixIndex++)
			{
				unsigned int index = 1024 + frameIndex * 1024 + matrixIndex * 16;
				D3DXMATRIX matrix = mAnimations[animationIndex]->getFrame(frameIndex)->getMatrix(matrixIndex);
				
				pTexels[index + 0] = matrix._11;
				pTexels[index + 1] = matrix._12;
				pTexels[index + 2] = matrix._13;
				pTexels[index + 3] = matrix._14;
				pTexels[index + 4] = matrix._21;
				pTexels[index + 5] = matrix._22;
				pTexels[index + 6] = matrix._23;
				pTexels[index + 7] = matrix._24;
				pTexels[index + 8] = matrix._31;
				pTexels[index + 9] = matrix._32;
				pTexels[index + 10] = matrix._33;
				pTexels[index + 11] = matrix._34;
				pTexels[index + 12] = matrix._41;
				pTexels[index + 13] = matrix._42;
				pTexels[index + 14] = matrix._43;
				pTexels[index + 15] = matrix._44;
			}
		}

		mpDevice->UpdateSubresource(pTextureArray, D3D10CalcSubresource(0, animationIndex, 1), NULL, pTexels, 1024 * 4, 1024 * 4 * 1024);

		SAFE_DELETE(pTexels);
	}

	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));

	viewDesc.Format = arrayDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2DArray.ArraySize = mAnimations.size();
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.MipLevels = 1;
	viewDesc.Texture2DArray.MostDetailedMip = 0;

	ID3D10ShaderResourceView * pResourceView;

	if(FAILED(mpDevice->CreateShaderResourceView(pTextureArray, &viewDesc, &pResourceView)))
	{
		ERROR_MESSAGE("AnimationHandler: Failed to create ShaderResourceView.");
		return NULL;
	}
	
	pTextureArray->Release();

	return pResourceView;
}

ID3D10ShaderResourceView *AnimationHandler::getTextureArrayVector() const
{
	ID3D10Texture2D * pTextureArray = NULL;

	D3D10_TEXTURE2D_DESC arrayDesc;
	ZeroMemory(&arrayDesc, sizeof(D3D10_TEXTURE2D_DESC));

	arrayDesc.Width = 1024;
	arrayDesc.Height = 1024;
	arrayDesc.MipLevels = 1;
	arrayDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	arrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	arrayDesc.Usage = D3D10_USAGE_DEFAULT;
	arrayDesc.ArraySize = mAnimations.size();
	arrayDesc.CPUAccessFlags = 0;
	arrayDesc.SampleDesc.Count = 1;
	arrayDesc.SampleDesc.Quality = 0;
	arrayDesc.MiscFlags = 0;

	if(FAILED(mpDevice->CreateTexture2D(&arrayDesc, NULL, &pTextureArray)))
	{
		ERROR_MESSAGE("AnimationHandler: Failed to create texture array.");
		return NULL;
	}

	for(unsigned int animationIndex=0; animationIndex<mAnimations.size(); animationIndex++)
	{
		unsigned int nrOfJoints = mAnimations[animationIndex]->getFrame(0)->getSize();

		D3DXCOLOR * pTexels = myNew D3DXCOLOR[1024 * 1024];

		for(unsigned int i=0;i<1024*1024;i++)
		{
			pTexels[i].r = 0.0f;
			pTexels[i].g = 0.0f;
			pTexels[i].b = 0.0f;
			pTexels[i].a = 0.0f;
		}

		pTexels[0].r = (float)mAnimations[animationIndex]->getSize();
		pTexels[0].g = mAnimations[animationIndex]->getTimePerFrame();

		for(unsigned int frameIndex = 0; frameIndex < mAnimations[animationIndex]->getSize(); frameIndex++)
		{
			for(unsigned int matrixIndex = 0; matrixIndex < nrOfJoints; matrixIndex++)
			{
				unsigned int index = 1024 + frameIndex * 1024 + matrixIndex * 4;
				D3DXMATRIX matrix = mAnimations[animationIndex]->getFrame(frameIndex)->getMatrix(matrixIndex);
				
				pTexels[index + 0].r = matrix._11;
				pTexels[index + 0].g = matrix._12;
				pTexels[index + 0].b = matrix._13;
				pTexels[index + 0].a = matrix._14;

				pTexels[index + 1].r = matrix._21;
				pTexels[index + 1].g = matrix._22;
				pTexels[index + 1].b = matrix._23;
				pTexels[index + 1].a = matrix._24;

				pTexels[index + 2].r = matrix._31;
				pTexels[index + 2].g = matrix._32;
				pTexels[index + 2].b = matrix._33;
				pTexels[index + 2].a = matrix._34;

				pTexels[index + 3].r = matrix._41;
				pTexels[index + 3].g = matrix._42;
				pTexels[index + 3].b = matrix._43;
				pTexels[index + 3].a = matrix._44;
			}
		}

		mpDevice->UpdateSubresource(pTextureArray, D3D10CalcSubresource(0, animationIndex, 1), NULL, pTexels, 1024 * sizeof(D3DXCOLOR), 1024 * 1024 * sizeof(D3DXCOLOR));

		SAFE_DELETE(pTexels);
	}

	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));

	viewDesc.Format = arrayDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2DArray.ArraySize = mAnimations.size();
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.MipLevels = 1;
	viewDesc.Texture2DArray.MostDetailedMip = 0;

	ID3D10ShaderResourceView * pResourceView;

	if(FAILED(mpDevice->CreateShaderResourceView(pTextureArray, &viewDesc, &pResourceView)))
	{
		ERROR_MESSAGE("AnimationHandler: Failed to create ShaderResourceView.");
		return NULL;
	}
	
	pTextureArray->Release();

	return pResourceView;
}

Animation * AnimationHandler::getAnimation(std::string name)
{
	for(unsigned int i=0;i<mAnimations.size();i++)
	{
		if(mAnimations[i]->getName()==name)
		{
			return mAnimations[i];
		}
	}
	return NULL;
}

int AnimationHandler::getAnimationIndex(const char *name)
{
	for(unsigned int nAnimation = 0; nAnimation < mAnimations.size(); ++nAnimation)
	{
		if(mAnimations[nAnimation]->getName() == name)
		{
			return nAnimation;
		}
	}
	return -1;
}

int AnimationHandler::getAnimationIndex(const std::string &name)
{
	for(unsigned int nAnimation = 0; nAnimation < mAnimations.size(); ++nAnimation)
	{
		if(mAnimations[nAnimation]->getName() == name)
		{
			return nAnimation;
		}
	}
	return -1;
}