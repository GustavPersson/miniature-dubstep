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

#include "TextureHandler.h"

using namespace gfx;
using namespace std;

TextureHandler::TextureHandler(ID3D10Device *pDevice)
{
	mpDevice = pDevice;
	mTextures = vector<Texture*>();
}

TextureHandler::~TextureHandler()
{
	for(unsigned int i=0;i<mTextures.size();i++)
	{
		SAFE_DELETE(mTextures[i]);
	}

	mTextures.clear();
}

void TextureHandler::addTexture(std::string fileName, std::string textureName)
{
	ID3D10ShaderResourceView * rv;
	ID3D10Texture2D * GPUTexture;
	ID3D10Texture2D * CPUTexture;

	HRESULT hr = D3DX10CreateTextureFromFile(mpDevice, fileName.c_str(), NULL, NULL, (ID3D10Resource**)&GPUTexture, NULL );

	if(FAILED(hr))
	{
		DEBUG_MESSAGE("Failed to create GPU texture from file: "<<fileName);
		return;
	}

	D3DX10_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo,sizeof(D3DX10_IMAGE_LOAD_INFO));
	loadInfo.Width = D3DX10_DEFAULT;
	loadInfo.Height =  D3DX10_DEFAULT;
	loadInfo.BindFlags = 0;
	loadInfo.Depth = D3DX10_DEFAULT;
	loadInfo.Filter = D3DX10_DEFAULT;
	loadInfo.FirstMipLevel = D3DX10_DEFAULT;
	loadInfo.Format = DXGI_FORMAT_FROM_FILE;
	loadInfo.MipFilter = D3DX10_DEFAULT;
	loadInfo.MiscFlags = D3DX10_DEFAULT;
	loadInfo.pSrcInfo =  0;
	loadInfo.Usage = D3D10_USAGE_STAGING;
	loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_READ;

	hr = D3DX10CreateTextureFromFile(mpDevice, fileName.c_str(), &loadInfo, NULL, (ID3D10Resource**)&CPUTexture, NULL );

	if(FAILED(hr))
	{
		DEBUG_MESSAGE("Failed to create CPU texture from file: "<<fileName);
		return;
	}

	D3D10_TEXTURE2D_DESC texDesc;
	GPUTexture->GetDesc(&texDesc);

	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;	

	hr = mpDevice->CreateShaderResourceView(GPUTexture,&viewDesc,&rv);
	GPUTexture->Release();

	if(FAILED(hr))
	{
		DEBUG_MESSAGE("Failed to create GPU resource View. Filename: "<<fileName);
		return;
	}

	mTextures.push_back(myNew Texture(rv,CPUTexture,textureName));
}

ID3D10ShaderResourceView * TextureHandler::getTexture(std::string textureName) const
{
	for(unsigned int i=0;i<mTextures.size();i++)
	{
		if(mTextures[i]->getName()==textureName)
		{			
			return mTextures[i]->getGPUTexture();
		}
	}

	DEBUG_MESSAGE("Could not find a texture with the name: "<<textureName);
	return NULL;
}

ID3D10ShaderResourceView * TextureHandler::getTextureArray(std::string textureNames) const
{
	int nrOfTextures = 1;

	for(unsigned int i=0;i<textureNames.size();i++)
	{
		if(textureNames[i]==',')
		{
			nrOfTextures++;
		}
	}

	ID3D10Texture2D *GPUTexArray;

	D3D10_TEXTURE2D_DESC arrayDesc;

	for(int i=0;i<nrOfTextures;i++)
	{
		int pos = textureNames.find_first_of(",");

		string name;

		if(pos==string::npos)
		{
			name = textureNames;
			textureNames.clear();
		}
		else
		{
			name = textureNames.substr(0,pos);
			textureNames.erase(0,pos+1);
		}

		bool textureFound = false;

		for(unsigned int j=0;j<mTextures.size();j++)
		{
			if(name==mTextures[j]->getName())
			{
				ID3D10Texture2D * texture;
				texture = mTextures[j]->getCPUTexture();

				if(i==0)
				{
					D3D10_TEXTURE2D_DESC firstTexDesc;
					texture->GetDesc(&firstTexDesc);

					ZeroMemory(&arrayDesc, sizeof(D3D10_TEXTURE2D_DESC));
					arrayDesc.Width = firstTexDesc.Width;
					arrayDesc.Height = firstTexDesc.Height;
					arrayDesc.MipLevels = 1;
					arrayDesc.Format = firstTexDesc.Format;
					arrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
					arrayDesc.Usage = D3D10_USAGE_DEFAULT;
					arrayDesc.ArraySize = nrOfTextures;
					arrayDesc.SampleDesc = firstTexDesc.SampleDesc;
					arrayDesc.CPUAccessFlags = 0;
					arrayDesc.MiscFlags = 0;

					if(FAILED(mpDevice->CreateTexture2D(&arrayDesc, NULL, &GPUTexArray)))
					{
						DEBUG_MESSAGE("Failed to create GPUTexArray.");
						return NULL;
					}
				}
				else
				{
					D3D10_TEXTURE2D_DESC texDesc;
					texture->GetDesc(&texDesc);

					if(texDesc.Width!=arrayDesc.Width)
					{
						DEBUG_MESSAGE("Trying to create a texture array from textures with different width.");
						return NULL;
					}
					else if(texDesc.Height!=arrayDesc.Height)
					{
						DEBUG_MESSAGE("Trying to create a texture array from textures with different height.");
						return NULL;
					}
				}

				D3D10_MAPPED_TEXTURE2D texMap;

				texture->Map(0,D3D10_MAP_READ,0,&texMap);

				unsigned int SRIndex = D3D10CalcSubresource(0,i,1);
				
				mpDevice->UpdateSubresource(GPUTexArray, SRIndex, NULL, texMap.pData, texMap.RowPitch, 1);

				texture->Unmap(0);

				textureFound = true;
				break;
			}
		}

		if(!textureFound)
		{
			DEBUG_MESSAGE("Could not find a texture with the name: "<<name);
			return NULL;
		}
	}

	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	SRVDesc.Format = arrayDesc.Format;
	SRVDesc.Texture2DArray.ArraySize = nrOfTextures;
	SRVDesc.Texture2DArray.FirstArraySlice = 0;
	SRVDesc.Texture2DArray.MipLevels = 1;
	SRVDesc.Texture2DArray.MostDetailedMip = 0;

	ID3D10ShaderResourceView *texArraySRV = NULL;

	if(FAILED(mpDevice->CreateShaderResourceView(GPUTexArray, &SRVDesc, &texArraySRV)))
	{
		DEBUG_MESSAGE("Failed to create Tex2DArray ShaderResourceView.");
		return NULL;
	}

	return texArraySRV;
}

const TextureHandler &TextureHandler::operator=(const TextureHandler &handler)
{
	if(this != &handler)
	{
		mTextures = handler.mTextures;
		mpDevice = handler.mpDevice;
	}
	return *this;
}