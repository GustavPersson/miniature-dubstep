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
	mTextures128 = vector<Texture*>();
	mTextures256 = vector<Texture*>();
	mTextures512 = vector<Texture*>();
	mTextures1024 = vector<Texture*>();
}

TextureHandler::~TextureHandler()
{
	for(unsigned int i=0;i<mTextures.size();i++)
	{
		SAFE_DELETE(mTextures[i]);
	}

	mTextures.clear();
	mTextures128.clear();
	mTextures256.clear();
	mTextures512.clear();
	mTextures1024.clear();
}

bool TextureHandler::getTextureIndex(const char *textureName, int &arrayIndex, int &textureID)
{
	vector<Texture *>::iterator textureIter;
	arrayIndex = 0;
	for(textureIter = mTextures128.begin(); textureIter != mTextures128.end();
		++textureIter)
	{
		if((*textureIter)->getName() == textureName)
		{
			textureID = textureIter - mTextures128.begin();
			return true;
		}
	}
	arrayIndex = 1;
	for(textureIter = mTextures256.begin(); textureIter != mTextures256.end();
		++textureIter)
	{
		if((*textureIter)->getName() == textureName)
		{
			textureID = textureIter - mTextures256.begin();
			return true;
		}
	}
	arrayIndex = 2;
	for(textureIter = mTextures512.begin(); textureIter != mTextures512.end();
		++textureIter)
	{
		if((*textureIter)->getName() == textureName)
		{
			textureID = textureIter - mTextures512.begin();
			return true;
		}
	}
	arrayIndex = 3;
	for(textureIter = mTextures1024.begin(); textureIter != mTextures1024.end();
		++textureIter)
	{
		if((*textureIter)->getName() == textureName)
		{
			textureID = textureIter - mTextures1024.begin();
			return true;
		}
	}
	return false;
}

bool TextureHandler::getTextureIndex(const std::string &textureName, int &arrayIndex, int &textureID)
{
	vector<Texture *>::iterator textureIter;
	arrayIndex = 0;
	for(textureIter = mTextures128.begin(); textureIter != mTextures128.end();
		++textureIter)
	{
		if((*textureIter)->getName() == textureName)
		{
			textureID = textureIter - mTextures128.begin();
			return true;
		}
	}
	arrayIndex = 1;
	for(textureIter = mTextures256.begin(); textureIter != mTextures256.end();
		++textureIter)
	{
		if((*textureIter)->getName() == textureName)
		{
			textureID = textureIter - mTextures256.begin();
			return true;
		}
	}
	arrayIndex = 2;
	for(textureIter = mTextures512.begin(); textureIter != mTextures512.end();
		++textureIter)
	{
		if((*textureIter)->getName() == textureName)
		{
			textureID = textureIter - mTextures512.begin();
			return true;
		}
	}
	arrayIndex = 3;
	for(textureIter = mTextures1024.begin(); textureIter != mTextures1024.end();
		++textureIter)
	{
		if((*textureIter)->getName() == textureName)
		{
			textureID = textureIter - mTextures1024.begin();
			return true;
		}
	}
	return false;
}

void TextureHandler::addTexture(std::string fileName, std::string textureName)
{
	ID3D10ShaderResourceView * rv;
	ID3D10Texture2D * GPUTexture;
	ID3D10Texture2D * CPUTexture;

	D3DX10_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo,sizeof(D3DX10_IMAGE_LOAD_INFO));
	loadInfo.Width = D3DX10_DEFAULT;
	loadInfo.Height =  D3DX10_DEFAULT;
	loadInfo.BindFlags = 0;
	loadInfo.Depth = D3DX10_DEFAULT;
	loadInfo.Filter = D3DX10_DEFAULT;
	loadInfo.FirstMipLevel = D3DX10_DEFAULT;
	loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	loadInfo.MipFilter = D3DX10_DEFAULT;
	loadInfo.MiscFlags = D3DX10_DEFAULT;
	loadInfo.pSrcInfo =  0;
	loadInfo.Usage = D3D10_USAGE_STAGING;
	loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_READ | D3D10_CPU_ACCESS_WRITE;

	HRESULT hr = D3DX10CreateTextureFromFile(mpDevice, fileName.c_str(), &loadInfo, NULL, (ID3D10Resource**)&CPUTexture, NULL );

	if(FAILED(hr))
	{
		ERROR_MESSAGE("Failed to create CPU texture from file: "<<fileName);
		return;
	}

	D3D10_TEXTURE2D_DESC texDesc;
	CPUTexture->GetDesc(&texDesc);
	
	if(texDesc.Width > 1024)
	{
		ERROR_MESSAGE("Can't load textures wider than 1024 pixels. filename: "<<fileName);
		return;
	}
	else if(texDesc.Height > 1024)
	{
		ERROR_MESSAGE("Can't load textures higher than 1024 pixels. filename: "<<fileName);
		return;
	}

	SizeTypes type;
	if(texDesc.Width > 512 || texDesc.Height > 512)
	{
		type = TextureHandler::SizeType_1024;
	}
	else if(texDesc.Width > 256 || texDesc.Height > 256)
	{
		type = TextureHandler::SizeType_512;
	}
	else if(texDesc.Width > 128 || texDesc.Height > 128)
	{
		type = TextureHandler::SizeType_256;
	}
	else
	{
		type = TextureHandler::SizeType_128;
	}

	D3D10_MAPPED_TEXTURE2D texMap;
	CPUTexture->Map(0, D3D10_MAP_WRITE, 0, &texMap);

	((byte*)texMap.pData)[0] = percent(texDesc.Width, type);
	((byte*)texMap.pData)[1] = percent(texDesc.Height, type);

	((byte*)texMap.pData)[texDesc.Width] = 255;

	ZeroMemory(&loadInfo,sizeof(D3DX10_IMAGE_LOAD_INFO));
	loadInfo.Width = D3DX10_DEFAULT;
	loadInfo.Height =  D3DX10_DEFAULT;
	loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	loadInfo.Depth = D3DX10_DEFAULT;
	loadInfo.Filter = D3DX10_DEFAULT;
	loadInfo.FirstMipLevel = D3DX10_DEFAULT;
	loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	loadInfo.MipFilter = D3DX10_DEFAULT;
	loadInfo.MiscFlags = D3DX10_DEFAULT;
	loadInfo.pSrcInfo =  0;
	loadInfo.Usage = D3D10_USAGE_DEFAULT;
	loadInfo.CpuAccessFlags = 0;

	hr = D3DX10CreateTextureFromFile(mpDevice, fileName.c_str(), &loadInfo, NULL, (ID3D10Resource**)&GPUTexture, NULL );

	if(FAILED(hr))
	{
		ERROR_MESSAGE("Failed to create GPU texture from file: "<<fileName);
		return;
	}

	byte * n00b = myNew byte[1024 * 1024 * 4];

	for(unsigned int y=0; y<texDesc.Height; y++)
	{
		for(unsigned int x=0; x<texDesc.Width; x++)
		{
			n00b[y * texDesc.Width * 4 + x * 4 + 0] = ((byte*)texMap.pData)[y * texDesc.Width * 4 + x * 4 + 0];
			n00b[y * texDesc.Width * 4 + x * 4 + 1] = ((byte*)texMap.pData)[y * texDesc.Width * 4 + x * 4 + 1];
			n00b[y * texDesc.Width * 4 + x * 4 + 2] = ((byte*)texMap.pData)[y * texDesc.Width * 4 + x * 4 + 2];
			n00b[y * texDesc.Width * 4 + x * 4 + 3] = 255;
		}
	}

	CPUTexture->Unmap(0);

	//n00b[0] = 255;//percent(texDesc.Width);
	//n00b[1] = 255;//100;//percent(texDesc.Height);

	mpDevice->UpdateSubresource(GPUTexture, D3D10CalcSubresource(0, 0, 1), NULL, n00b, texDesc.Width * 4, texDesc.Width * texDesc.Height * 4);

	SAFE_DELETE(n00b);

	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;	

	hr = mpDevice->CreateShaderResourceView(GPUTexture,&viewDesc,&rv);
	GPUTexture->Release();

	if(FAILED(hr))
	{
		ERROR_MESSAGE("Failed to create GPU resource View. Filename: "<<fileName);
		ERROR_MESSAGE(DXGetErrorDescriptionA(hr));
		return;
	}

	Texture * texture = myNew Texture(rv, CPUTexture, textureName);

	if(texDesc.Width > 512 || texDesc.Height > 512)
	{
		mTextures1024.push_back(texture);
	}
	else if(texDesc.Width > 256 || texDesc.Height > 256)
	{
		mTextures512.push_back(texture);
	}
	else if(texDesc.Width > 128 || texDesc.Height > 128)
	{
		mTextures256.push_back(texture);
	}
	else
	{
		mTextures128.push_back(texture);
	}

	mTextures.push_back(texture);
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

	ERROR_MESSAGE("Could not find a texture with the name: "<<textureName);
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

	ID3D10Texture2D *GPUTexArray = NULL;

	D3D10_TEXTURE2D_DESC arrayDesc;
	ZeroMemory(&arrayDesc, sizeof(D3D10_TEXTURE2D_DESC));

	for(int i=0;i<nrOfTextures;i++)
	{
		int pos = textureNames.find_first_of(",");

		string name;

		if(pos==(int)string::npos)
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
						ERROR_MESSAGE("Failed to create GPUTexArray.");
						return NULL;
					}
				}
				else
				{
					D3D10_TEXTURE2D_DESC texDesc;
					texture->GetDesc(&texDesc);

					if(texDesc.Width!=arrayDesc.Width)
					{
						ERROR_MESSAGE("Trying to create a texture array from textures with different width.");
						return NULL;
					}
					else if(texDesc.Height!=arrayDesc.Height)
					{
						ERROR_MESSAGE("Trying to create a texture array from textures with different height.");
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
			ERROR_MESSAGE("Could not find a texture with the name: "<<name);
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
		ERROR_MESSAGE("Failed to create Tex2DArray ShaderResourceView.");
		return NULL;
	}

	return texArraySRV;
}

ID3D10ShaderResourceView * TextureHandler::getTextureArraySize(std::string textureNames) const
{
	unsigned int nrOfTextures = 1;

	for(unsigned int i=0;i<textureNames.size();i++)
	{
		if(textureNames[i]==',')
		{
			nrOfTextures++;
		}
	}

	vector<ID3D10Texture2D*> tempTextures = vector<ID3D10Texture2D*>();

	ID3D10Texture2D *GPUTexArray = NULL;

	D3D10_TEXTURE2D_DESC arrayDesc;
	ZeroMemory(&arrayDesc, sizeof(D3D10_TEXTURE2D_DESC));

	for(unsigned int i=0;i<nrOfTextures;i++)
	{
		int pos = textureNames.find_first_of(",");

		string name;

		if(pos==(int)string::npos)
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
				tempTextures.push_back(texture);

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
				}
				else
				{
					D3D10_TEXTURE2D_DESC texDesc;
					texture->GetDesc(&texDesc);
					
					if(texDesc.Width > arrayDesc.Width)
					{
						arrayDesc.Width = texDesc.Width;
					}

					if(texDesc.Height > arrayDesc.Height)
					{
						arrayDesc.Height = texDesc.Height;
					}
				}
				textureFound = true;
				break;
			}
		}

		if(!textureFound)
		{
			ERROR_MESSAGE("Could not find a texture with the name: "<<name);
			return NULL;
		}
	}

	if(FAILED(mpDevice->CreateTexture2D(&arrayDesc, NULL, &GPUTexArray)))
	{
		ERROR_MESSAGE("Failed to create GPUTexArray.");
		return NULL;
	}

	for(unsigned int i=0;i<nrOfTextures;i++)
	{
		D3D10_TEXTURE2D_DESC texDesc;
		tempTextures[i]->GetDesc(&texDesc);
		
		D3D10_BOX box;
		box.left = 0;
		box.right = texDesc.Width;
		box.top = 0;
		box.bottom = texDesc.Height;
		box.front = 0;
		box.back = 1;
		
		D3D10_MAPPED_TEXTURE2D texMap;

		tempTextures[i]->Map(0,D3D10_MAP_READ,0,&texMap);
				
		mpDevice->UpdateSubresource(GPUTexArray, D3D10CalcSubresource(0,i,1), &box, texMap.pData, texMap.RowPitch, 1);

		tempTextures[i]->Unmap(0);
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
		ERROR_MESSAGE("Failed to create Tex2DArray ShaderResourceView.");
		return NULL;
	}

	return texArraySRV;
}

ID3D10ShaderResourceView * TextureHandler::getTextureArraySize(SizeTypes sizeType) const
{
	ID3D10Texture2D *GPUTexArray = NULL;

	D3D10_TEXTURE2D_DESC arrayDesc;
	ZeroMemory(&arrayDesc, sizeof(D3D10_TEXTURE2D_DESC));

	if(sizeType == SizeType_128)
	{
		if(mTextures128.empty())
		{
			return NULL;
		}
		for(unsigned int i=0; i<mTextures128.size(); i++)
		{
			ID3D10Texture2D * texture = mTextures128[i]->getCPUTexture();

			D3D10_TEXTURE2D_DESC texDesc;
			texture->GetDesc(&texDesc);

			if(i == 0)
			{
				ZeroMemory(&arrayDesc, sizeof(D3D10_TEXTURE2D_DESC));
				arrayDesc.Width = 128;
				arrayDesc.Height = 128;
				arrayDesc.MipLevels = 1;
				arrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				arrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
				arrayDesc.Usage = D3D10_USAGE_DEFAULT;
				arrayDesc.ArraySize = mTextures128.size();
				arrayDesc.SampleDesc = texDesc.SampleDesc;
				arrayDesc.CPUAccessFlags = 0;
				arrayDesc.MiscFlags = 0;

				if(FAILED(mpDevice->CreateTexture2D(&arrayDesc, NULL, &GPUTexArray)))
				{
					ERROR_MESSAGE("Failed to create GPUTexArray.");
					return NULL;
				}
			}
		
			D3D10_BOX box;
			box.left = 0;
			box.right = texDesc.Width;
			box.top = 0;
			box.bottom = texDesc.Height;
			box.front = 0;
			box.back = 1;
		
			D3D10_MAPPED_TEXTURE2D texMap;
			texture->Map(0,D3D10_MAP_READ,0,&texMap);
				
			mpDevice->UpdateSubresource(GPUTexArray, D3D10CalcSubresource(0,i,1), &box, texMap.pData, texMap.RowPitch, 1);

			texture->Unmap(0);
		}

		D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
		SRVDesc.Format = arrayDesc.Format;
		SRVDesc.Texture2DArray.ArraySize = mTextures128.size();
		SRVDesc.Texture2DArray.FirstArraySlice = 0;
		SRVDesc.Texture2DArray.MipLevels = 1;
		SRVDesc.Texture2DArray.MostDetailedMip = 0;

		ID3D10ShaderResourceView *texArraySRV = NULL;

		HRESULT error = mpDevice->CreateShaderResourceView(GPUTexArray, &SRVDesc, &texArraySRV);
	
		if(FAILED(error))
		{
			ERROR_MESSAGE("Failed to create Tex2DArray ShaderResourceView.");
			ERROR_MESSAGE(DXGetErrorDescriptionA(error));
			return NULL;
		}

		return texArraySRV;
	}
	else if(sizeType == SizeType_256)
	{
		if(mTextures256.empty())
		{
			return NULL;
		}
		for(unsigned int i=0; i<mTextures256.size(); i++)
		{
			ID3D10Texture2D * texture = mTextures256[i]->getCPUTexture();

			D3D10_TEXTURE2D_DESC texDesc;
			texture->GetDesc(&texDesc);

			if(i == 0)
			{
				ZeroMemory(&arrayDesc, sizeof(D3D10_TEXTURE2D_DESC));
				arrayDesc.Width = 256;
				arrayDesc.Height = 256;
				arrayDesc.MipLevels = 1;
				arrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				arrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
				arrayDesc.Usage = D3D10_USAGE_DEFAULT;
				arrayDesc.ArraySize = mTextures256.size();
				arrayDesc.SampleDesc = texDesc.SampleDesc;
				arrayDesc.CPUAccessFlags = 0;
				arrayDesc.MiscFlags = 0;

				if(FAILED(mpDevice->CreateTexture2D(&arrayDesc, NULL, &GPUTexArray)))
				{
					ERROR_MESSAGE("Failed to create GPUTexArray.");
					return NULL;
				}
			}
			
			D3D10_BOX box;
			box.left = 0;
			box.right = texDesc.Width;
			box.top = 0;
			box.bottom = texDesc.Height;
			box.front = 0;
			box.back = 1;
		
			D3D10_MAPPED_TEXTURE2D texMap;
			texture->Map(0,D3D10_MAP_READ,0,&texMap);
				
			mpDevice->UpdateSubresource(GPUTexArray, D3D10CalcSubresource(0,i,1), &box, texMap.pData, texMap.RowPitch, 1);

			texture->Unmap(0);
		}

		D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.Texture2DArray.ArraySize = mTextures256.size();
		SRVDesc.Texture2DArray.FirstArraySlice = 0;
		SRVDesc.Texture2DArray.MipLevels = 1;
		SRVDesc.Texture2DArray.MostDetailedMip = 0;

		ID3D10ShaderResourceView *texArraySRV = NULL;

		HRESULT error = mpDevice->CreateShaderResourceView(GPUTexArray, &SRVDesc, &texArraySRV);

		if(FAILED(error))
		{
			ERROR_MESSAGE("Failed to create Tex2DArray ShaderResourceView.");
			ERROR_MESSAGE(DXGetErrorDescriptionA(error));
			return NULL;
		}

		return texArraySRV;
	}else if(sizeType == SizeType_512)
	{
		if(mTextures512.empty())
		{
			return NULL;
		}
		for(unsigned int i=0; i<mTextures1024.size(); i++)
		{
			ID3D10Texture2D * texture = mTextures512[i]->getCPUTexture();

			D3D10_TEXTURE2D_DESC texDesc;
			texture->GetDesc(&texDesc);

			if(i == 0)
			{
				ZeroMemory(&arrayDesc, sizeof(D3D10_TEXTURE2D_DESC));
				arrayDesc.Width = 512;
				arrayDesc.Height = 512;
				arrayDesc.MipLevels = 1;
				arrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				arrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
				arrayDesc.Usage = D3D10_USAGE_DEFAULT;
				arrayDesc.ArraySize = mTextures512.size();
				arrayDesc.SampleDesc = texDesc.SampleDesc;
				arrayDesc.CPUAccessFlags = 0;
				arrayDesc.MiscFlags = 0;

				if(FAILED(mpDevice->CreateTexture2D(&arrayDesc, NULL, &GPUTexArray)))
				{
					ERROR_MESSAGE("Failed to create GPUTexArray.");
					return NULL;
				}
			}
			
			D3D10_BOX box;
			box.left = 0;
			box.right = texDesc.Width;
			box.top = 0;
			box.bottom = texDesc.Height;
			box.front = 0;
			box.back = 1;
		
			D3D10_MAPPED_TEXTURE2D texMap;
			texture->Map(0,D3D10_MAP_READ,0,&texMap);
				
			mpDevice->UpdateSubresource(GPUTexArray, D3D10CalcSubresource(0,i,1), &box, texMap.pData, texMap.RowPitch, 1);

			texture->Unmap(0);
		}

		D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.Texture2DArray.ArraySize = mTextures512.size();
		SRVDesc.Texture2DArray.FirstArraySlice = 0;
		SRVDesc.Texture2DArray.MipLevels = 1;
		SRVDesc.Texture2DArray.MostDetailedMip = 0;

		ID3D10ShaderResourceView *texArraySRV = NULL;

		HRESULT error = mpDevice->CreateShaderResourceView(GPUTexArray, &SRVDesc, &texArraySRV);

		if(FAILED(error))
		{
			ERROR_MESSAGE("Failed to create Tex2DArray ShaderResourceView.");
			ERROR_MESSAGE(DXGetErrorDescriptionA(error));
			return NULL;
		}

		return texArraySRV;
	}
	else if(sizeType == SizeType_1024)
	{
		if(mTextures1024.empty())
		{
			return NULL;
		}
		for(unsigned int i=0; i<mTextures1024.size(); i++)
		{
			ID3D10Texture2D * texture = mTextures1024[i]->getCPUTexture();

			D3D10_TEXTURE2D_DESC texDesc;
			texture->GetDesc(&texDesc);

			if(i == 0)
			{
				ZeroMemory(&arrayDesc, sizeof(D3D10_TEXTURE2D_DESC));
				arrayDesc.Width = 1024;
				arrayDesc.Height = 1024;
				arrayDesc.MipLevels = 1;
				arrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				arrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
				arrayDesc.Usage = D3D10_USAGE_DEFAULT;
				arrayDesc.ArraySize = mTextures1024.size();
				arrayDesc.SampleDesc = texDesc.SampleDesc;
				arrayDesc.CPUAccessFlags = 0;
				arrayDesc.MiscFlags = 0;

				if(FAILED(mpDevice->CreateTexture2D(&arrayDesc, NULL, &GPUTexArray)))
				{
					ERROR_MESSAGE("Failed to create GPUTexArray.");
					return NULL;
				}
			}
			
			D3D10_BOX box;
			box.left = 0;
			box.right = texDesc.Width;
			box.top = 0;
			box.bottom = texDesc.Height;
			box.front = 0;
			box.back = 1;
		
			D3D10_MAPPED_TEXTURE2D texMap;
			texture->Map(0,D3D10_MAP_READ,0,&texMap);
				
			mpDevice->UpdateSubresource(GPUTexArray, D3D10CalcSubresource(0,i,1), &box, texMap.pData, texMap.RowPitch, 1);

			texture->Unmap(0);
		}

		D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.Texture2DArray.ArraySize = mTextures1024.size();
		SRVDesc.Texture2DArray.FirstArraySlice = 0;
		SRVDesc.Texture2DArray.MipLevels = 1;
		SRVDesc.Texture2DArray.MostDetailedMip = 0;

		ID3D10ShaderResourceView *texArraySRV = NULL;

		HRESULT error = mpDevice->CreateShaderResourceView(GPUTexArray, &SRVDesc, &texArraySRV);

		if(FAILED(error))
		{
			ERROR_MESSAGE("Failed to create Tex2DArray ShaderResourceView.");
			ERROR_MESSAGE(DXGetErrorDescriptionA(error));
			return NULL;
		}

		return texArraySRV;
	}

	return NULL;
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

byte TextureHandler::percent(unsigned int size, SizeTypes textureSize)
{
	byte result = 0;
	switch(textureSize)
	{
		case TextureHandler::SizeType_128:
			result = (byte)((size / 128.0f) * 255.0f);
			break;

		case TextureHandler::SizeType_256:
			result = (byte)((size / 256.0f) * 255.0f);
			break;

		case TextureHandler::SizeType_512:
			result = (byte)((size / 512.0f) * 255.0f);
			break;

		case TextureHandler::SizeType_1024:
			result = (byte)((size / 1024.0f) * 255.0f);
			break;

		default:
			break;
	}
	return result;
}