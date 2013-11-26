/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
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

#include "WorldPlane.h"
#include "DX10Module.h"
#include "../Logic/KeyType.h"

using namespace gfx;
using namespace logic;
using std::vector;

WorldPlane::WorldPlane(int tileX, int tileZ, float tileSizeX, float tileSizeZ) : mPosition(0.0f, 0.0f, 0.0f)
{
	mTileX = tileX;
	mTileZ = tileZ;
	mTileSizeX = tileSizeX;
	mTileSizeZ = tileSizeZ;
	mTileCount = 0;
	mpTiles = NULL;
	mpWeightMap = NULL;
	mpWeightMapSRV = NULL;
	mFlagUpdateRequired = true;
	mFlagWeightUpdateRequired = true;
	mFlagSRVUpdateRequired = true;
	createTiles();
}

WorldPlane::WorldPlane(const WorldPlane &plane) : mPosition(plane.mPosition)
{
	mTileX = plane.mTileX;
	mTileZ = plane.mTileZ;
	mTileSizeX = plane.mTileSizeX;
	mTileSizeZ = plane.mTileSizeZ;
	mTileCount = plane.mTileCount;
	mpTiles = NULL;
	mpWeightMap = NULL;
	mpWeightMapSRV = NULL;
	mFlagUpdateRequired = true;
	mFlagWeightUpdateRequired = true;
	mFlagSRVUpdateRequired = true;
	createTiles();
}

WorldPlane::~WorldPlane()
{
	removeTiles();
	if(mpWeightMap)
	{
		mpWeightMap->Release();
		mpWeightMap = NULL;
	}
}

const WorldPlane &WorldPlane::operator=(const WorldPlane &plane)
{
	if(this != &plane)
	{
		mTileX = plane.mTileX;
		mTileZ = plane.mTileZ;
		mTileSizeX = plane.mTileSizeX;
		mTileSizeZ = plane.mTileSizeZ;
		mTileCount = plane.mTileCount;
		mpTiles = NULL;
		mpWeightMap = NULL;
		mpWeightMapSRV = NULL;
		mFlagUpdateRequired = true;
		mFlagWeightUpdateRequired = true;
		mFlagSRVUpdateRequired = true;
		createTiles();
	}
	return *this;
}

void WorldPlane::update()
{
	if(mFlagUpdateRequired)
	{
		updateTiles();
		mFlagUpdateRequired = false;
	}
	if(mFlagWeightUpdateRequired && !mFlagSRVUpdateRequired)
	{
		updateTileWeightMap();
		mFlagWeightUpdateRequired = false;
	}
}

void WorldPlane::setWeights(const D3DXVECTOR3 &position, float radius, const D3DXVECTOR4 &weights)
{
	for(int n = 0; n < mTileCount; ++n)
	{
		mpTiles[n]->setWeights(position, radius, weights);
	}
	mFlagWeightUpdateRequired = true;
}

void WorldPlane::increaseWeights(const D3DXVECTOR3 &position, float radius, const D3DXVECTOR4 &weightOffset)
{
	for(int n = 0; n < mTileCount; ++n)
	{
		mpTiles[n]->increaseWeights(position, radius, weightOffset);
	}
	mFlagWeightUpdateRequired = true;
}

void WorldPlane::SRVUpdated()
{
	mFlagSRVUpdateRequired = false;
	mFlagWeightUpdateRequired = true;
}

void WorldPlane::removeTiles()
{
	if(mpTiles)
	{
		for(int n = 0; n < mTileCount; ++n)
		{
			//Remove all instances.
			DX10Module::getInstance()->removeRenderObject(mpTiles[n]);
		}
		delete [] mpTiles;
		mpTiles = NULL;
	}
}

void WorldPlane::setPosition(const D3DXVECTOR3 &position)
{
	mPosition = position;
	mFlagUpdateRequired = true;
}

void WorldPlane::setTileX(int tileX)
{
	mTileX =  tileX;
	mFlagUpdateRequired = true;
}

void WorldPlane::setTileZ(int tileZ)
{
	mTileZ = tileZ;
	mFlagUpdateRequired = true;
}

void WorldPlane::setTileSizeX(float tileSizeX)
{
	mTileSizeX = tileSizeX;
	mFlagUpdateRequired = true;
}

void WorldPlane::setTileSizeZ(float tileSizeZ)
{
	mTileSizeZ = tileSizeZ;
	mFlagUpdateRequired = true;
}

void WorldPlane::setWeights(const std::vector<D3DXVECTOR4> &weights)
{
	vector<D3DXVECTOR4> tileWeights;
	int nVertexIndex = 0;
	for(int n = 0; n < mTileCount; ++n)
	{
		tileWeights.clear();
		for(unsigned int nVertex = 0; nVertex < mpTiles[n]->getVertexCount(); ++nVertex)
		{
			tileWeights.push_back(weights[nVertexIndex]);
			++nVertexIndex;
		}
		mpTiles[n]->setWeights(tileWeights);
	}
	mFlagWeightUpdateRequired = true;
}

void WorldPlane::getWeights(std::vector<D3DXVECTOR4> &weights)
{
	weights.clear();
	for(int n = 0; n < mTileCount; ++n)
	{
		mpTiles[n]->getWeights(weights);
	}
}

void WorldPlane::updateTiles()
{
	if(!mpTiles)
	{
		createTiles();
	}

	D3DXVECTOR3 tilePos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 tileScale(1.0f, 1.0f, 1.0f);
	int tileXHalf = mTileX / 2;
	int tileZHalf = mTileZ / 2;
	for(int z = 0; z < mTileZ; ++z)
	{
		for(int x = 0; x < mTileX; ++x)
		{
			//Calculate position of the tile's center.
			tilePos.x = (x - tileXHalf) * mTileSizeX + mTileSizeX * 0.5f;
			tilePos.z = (z - tileZHalf) * mTileSizeZ + mTileSizeZ * 0.5f;

			//Calculate scale of the tile.
			tileScale.x = (float)mTileSizeX / (mpTiles[z * mTileX + x]->getWidth() - 1);
			tileScale.z = (float)mTileSizeZ / (mpTiles[z * mTileX + x]->getHeight() -1);

			mpTiles[z * mTileX + x]->setPosition(tilePos);
			mpTiles[z * mTileX + x]->setScale(tileScale);
		}
	}
}

bool WorldPlane::updateTileWeightMap()
{
	if(!mpTiles)
	{
		return false;
	}
	for(int n = 0; n < mTileCount; ++n)
	{
		mpTiles[n]->updateWeightMap(mpWeightMap, n);
	}
	return true;
}

void WorldPlane::createPlaneBase()
{
	//Attempt to retrieve the world plane base instance.
	InstanceObject *pPlaneBase = DX10Module::getInstance()->getObjectInstanceBase("WorldPlane");
	
	//Create a new instance base if it doesn't exist.
	if(!pPlaneBase)
	{
		DX10Module::getInstance()->loadObjectShapeInstance(RenderObject::ShapeTypes_Plane, 5000, "WorldPlane", 5000);
		DX10Module::getInstance()->getObjectInstanceBase("WorldPlane")->setFlagUpdateFromBase(false);
	}
}

void WorldPlane::createTiles()
{
	createPlaneBase();
	removeTiles();

	//Texture array indices and texture indices.
	int a1 = 0;
	int a2 = 0;
	int a3 = 0;
	int a4 = 0;
	int t1 = -1;
	int t2 = -1;
	int t3 = -1;
	int t4 = -1;

	//Retrieve all textures. They use static names.
	DX10Module::getInstance()->getTextureIndex("WorldPlane01", a1, t1);
	DX10Module::getInstance()->getTextureIndex("WorldPlane02", a2, t2);
	DX10Module::getInstance()->getTextureIndex("WorldPlane03", a3, t3);
	DX10Module::getInstance()->getTextureIndex("WorldPlane04", a4, t4);

	mTileCount = mTileX * mTileZ;
	mpTiles = myNew InstanceShapePlane *[mTileCount];
	for(int n = 0; n < mTileCount; ++n)
	{
		//Create the tiles.
		mpTiles[n] = (InstanceShapePlane *)DX10Module::getInstance()->createObjectInstanced("WorldPlane");
		mpTiles[n]->setID(logic::KEY_GROUND.convertToGfxId());
		mpTiles[n]->setArraySlice(n);
		mpTiles[n]->setTextureArrayIndex(a1);
		mpTiles[n]->setTextureArrayIndex2(a2);
		mpTiles[n]->setTextureArrayIndex3(a3);
		mpTiles[n]->setTextureArrayIndex4(a4);
		mpTiles[n]->setTextureID(t1);
		mpTiles[n]->setTextureID2(t2);
		mpTiles[n]->setTextureID3(t3);
		mpTiles[n]->setTextureID4(t4);
	}

	createWeightMap();
}

void WorldPlane::createWeightMap()
{
	//Return if no tiles are available.
	if(!mpTiles)
	{
		return;
	}

	ID3D10Device *pDevice = DX10Module::getInstance()->getDevice();

	//Release any current weight maps.
	if(mpWeightMap)
	{
		mpWeightMap->Release();
		mpWeightMap = NULL;
	}
	if(mpWeightMapSRV)
	{
		mpWeightMapSRV->Release();
		mpWeightMapSRV = NULL;
	}

	//Get width and height (in number of vertices) from the first tile.
	unsigned int width = mpTiles[0]->getWidth();
	unsigned int height = mpTiles[0]->getHeight();

	D3D10_TEXTURE2D_DESC arrayDesc;
	arrayDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	arrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	arrayDesc.Usage = D3D10_USAGE_DEFAULT;
	arrayDesc.CPUAccessFlags = 0;
	arrayDesc.ArraySize = mTileCount;
	arrayDesc.Width = width;
	arrayDesc.Height = height;
	arrayDesc.MipLevels = 1;
	arrayDesc.MiscFlags = 0;
	arrayDesc.SampleDesc.Count = 1;
	arrayDesc.SampleDesc.Quality = 0;

	if(FAILED(pDevice->CreateTexture2D(&arrayDesc, NULL, &mpWeightMap)))
	{
		ERROR_MESSAGE("Failed to create weight map texture.")
	}

	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	SRVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	SRVDesc.Texture2DArray.ArraySize = mTileCount;
	SRVDesc.Texture2DArray.FirstArraySlice = 0;
	SRVDesc.Texture2DArray.MipLevels = 1;
	SRVDesc.Texture2DArray.MostDetailedMip = 0;

	if(FAILED(pDevice->CreateShaderResourceView(mpWeightMap, &SRVDesc, &mpWeightMapSRV)))
	{
		ERROR_MESSAGE("Failed to create weight map shader resource.")
	}

	mFlagSRVUpdateRequired = true;
}