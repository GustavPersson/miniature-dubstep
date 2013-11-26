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

#include "InstanceShapePlane.h"
#include "OBB.h"
#include "Vertex.h"
#include "Effect.h"

using namespace gfx;


InstanceShapePlane::InstanceShapePlane(ID3D10Device *pDevice, Effect *pEffect) : InstanceObject(pDevice, pEffect)
{
	setEffectTechIndex(0);
	setEffectPassIndex(RenderObject::ShaderPasses_MultiTex);
	setEffectLayoutIndex(RenderObject::ShaderPasses_MultiTex);
	setFlagShadow(false);
	mpWeights = NULL;
	mFlagUpdateWeights = false;
	mWidth = 10;
	mHeight = 10;
	mUVScale = 1;
	mOffsetX = 1.0f;
	mOffsetZ = 1.0f;
	mArraySlice = 0;
	mArrayIndex2 = 0;
	mArrayIndex3 = 0;
	mArrayIndex4 = 0;
	mTextureID2 = 0;
	mTextureID3 = 0;
	mTextureID4 = 0;
	setBoundingVolume(myNew OBB(-(float)(mWidth / 2), -1.0f, -(float)(mHeight / 2), (float)(mWidth / 2), 1.0f, (float)(mHeight / 2), -1));
	getBoundingVolume()->setParent((void *)this);
}

InstanceShapePlane::InstanceShapePlane(const InstanceShapePlane &plane) : InstanceObject(plane)
{
	//Copy weights.
	mpWeights = myNew D3DXVECTOR4[plane.getVertexCount()];
	for(unsigned int n = 0; n < plane.getVertexCount(); ++n)
	{
		mpWeights[n] = plane.mpWeights[n];
	}
	mFlagUpdateWeights = plane.mFlagUpdateWeights;
	mWidth = plane.mWidth;
	mHeight = plane.mHeight;
	mUVScale = plane.mUVScale;
	mOffsetX = plane.mOffsetX;
	mOffsetZ = plane.mOffsetZ;
	mArraySlice = plane.mArraySlice;
	mArrayIndex2 = plane.mArrayIndex2;
	mArrayIndex3 = plane.mArrayIndex3;
	mArrayIndex4 = plane.mArrayIndex4;
	mTextureID2 = plane.mTextureID2;
	mTextureID3 = plane.mTextureID3;
	mTextureID4 = plane.mTextureID4;
}

InstanceShapePlane::~InstanceShapePlane()
{
	if(mpWeights)
	{
		delete [] mpWeights;
		mpWeights = NULL;
	}
}

const InstanceShapePlane &InstanceShapePlane::operator=(const InstanceShapePlane &plane)
{
	if(this != &plane)
	{
		InstanceObject::operator=(plane);
		//Copy weights.
		if(mpWeights)
		{
			delete [] mpWeights;
			mpWeights = NULL;
		}
		mpWeights = myNew D3DXVECTOR4[plane.getVertexCount()];
		for(unsigned int n = 0; n < plane.getVertexCount(); ++n)
		{
			mpWeights[n] = plane.mpWeights[n];
		}
		mFlagUpdateWeights = plane.mFlagUpdateWeights;
		mWidth = plane.mWidth;
		mHeight = plane.mHeight;
		mUVScale = plane.mUVScale;
		mOffsetX = plane.mOffsetX;
		mOffsetZ = plane.mOffsetZ;
		mArraySlice = plane.mArraySlice;
		mArrayIndex2 = plane.mArrayIndex2;
		mArrayIndex3 = plane.mArrayIndex3;
		mArrayIndex4 = plane.mArrayIndex4;
		mTextureID2 = plane.mTextureID2;
		mTextureID3 = plane.mTextureID3;
		mTextureID4 = plane.mTextureID4;
	}
	return *this;
}

RenderObject *InstanceShapePlane::clone()
{
	return myNew InstanceShapePlane(*this);
}

void InstanceShapePlane::update(float deltaTime)
{
	InstanceObject::update(deltaTime);
}

InstanceObject *InstanceShapePlane::createInstance()
{
	if(getFlagInstanceBase())
	{
		if(getInstanceCount() < getInstanceCountMax())
		{
			InstanceObject *pNewInstance = (InstanceObject *)clone();
			pNewInstance->setFlagInstanceBase(false);
			pNewInstance->setInstanceIndex(getInstanceCount());
			linkVertexBufferStatic(pNewInstance);
			linkVertexBufferDynamic(pNewInstance);
			addInstance(pNewInstance);
			return pNewInstance;
		}
	}
	return NULL;
}

void InstanceShapePlane::loadBuffers()
{
	if(getFlagInstanceBase())
	{
		createVertexBufferStatic();
		createIndexBuffer();
	}
}

void InstanceShapePlane::updateWeightMap(ID3D10Resource *pWeightMap, int index)
{
	if(mFlagUpdateWeights)
	{
		//Create a data pointer with 16-bits floats.
		D3DXVECTOR4_16F *pData = myNew D3DXVECTOR4_16F[getVertexCount()];
		for(unsigned int n = 0; n < getVertexCount(); ++n)
		{
			pData[n] = (const float *)mpWeights[n];
		}

		//Update the array slice.
		getDevice()->UpdateSubresource(pWeightMap, D3D10CalcSubresource(0, mArraySlice, 1), NULL, (void *)pData,
			sizeof(D3DXVECTOR4_16F) * mWidth, sizeof(D3DXVECTOR4_16F) * getVertexCount());

		//Free the data.
		delete [] pData;

		mFlagUpdateWeights = false;
	}
}

void InstanceShapePlane::setWeights(const D3DXVECTOR3 &position, float radius, const D3DXVECTOR4 &weights)
{
	//Translate to local space.
	D3DXVECTOR3 markerPos = position - getPosition();
	markerPos.y = 0.0f;

	D3DXVECTOR3 vertexPos;
	D3DXVECTOR3 vertexDistance;
	//Go through each vertex.
	for(int z = 0; z < mHeight; ++z)	
	{
		for(int x = 0; x < mWidth; ++x)
		{
			vertexPos.x = (x - mWidth / 2) * mOffsetX * getScale().x;
			vertexPos.z = (z - mHeight / 2) * mOffsetZ * getScale().z;
			vertexPos.y = 0.0f;
			vertexDistance = markerPos - vertexPos;
			if(D3DXVec3Length(&vertexDistance) < radius)
			{
				mpWeights[z * mWidth + x] = weights;
			}
		}
	}
	mFlagUpdateWeights = true;
}

void InstanceShapePlane::increaseWeights(const D3DXVECTOR3 &position, float radius, const D3DXVECTOR4 &weightOffset)
{
	//Translate to local space.
	D3DXVECTOR3 markerPos = position - getPosition();
	markerPos.y = 0.0f;

	D3DXVECTOR3 vertexPos;
	D3DXVECTOR3 vertexDistance;
	//Go through each vertex.
	for(int z = 0; z < mHeight; ++z)	
	{
		for(int x = 0; x < mWidth; ++x)
		{
			vertexPos.x = (x - mWidth / 2) * mOffsetX * getScale().x;
			vertexPos.z = (z - mHeight / 2) * mOffsetZ * getScale().z;
			vertexPos.y = 0.0f;
			vertexDistance = markerPos - vertexPos;
			if(D3DXVec3Length(&vertexDistance) < radius)
			{
				mpWeights[z * mWidth + x] += weightOffset;
				D3DXVec4Normalize(&mpWeights[z * mWidth + x], &mpWeights[z * mWidth + x]);
			}
		}
	}
	mFlagUpdateWeights = true;
}

void InstanceShapePlane::setWeights(const std::vector<D3DXVECTOR4> &weights)
{
	if(weights.size() != getVertexCount())
	{
		ERROR_MESSAGE("Invalid amount of texture weights.")
		return;
	}

	for(unsigned int n = 0; n < getVertexCount(); ++n)
	{
		mpWeights[n] = weights[n];
	}
	mFlagUpdateWeights = true;
}

void InstanceShapePlane::getWeights(std::vector<D3DXVECTOR4> &weights)
{
	for(unsigned int n = 0; n < getVertexCount(); ++n)
	{
		weights.push_back(mpWeights[n]);
	}
}

bool InstanceShapePlane::updateVertexBufferDynamic()
{
	if(InstanceObject::updateVertexBufferDynamic())
	{
		VertexMultiTextureDynamic data;
		data.arraySlice = mArraySlice;
		data.arrayIndex = D3DXVECTOR4_16F((float)getTextureArrayIndex() + 0.01f, (float)mArrayIndex2 + 0.01f, (float)mArrayIndex3 + 0.01f, (float)mArrayIndex4 + 0.01f);
		data.textureID = D3DXVECTOR4_16F((float)getTextureID() + 0.01f, (float)mTextureID2 + 0.01f, (float)mTextureID3 + 0.01f, (float)mTextureID4 + 0.01f);
		data.color = (const float *)getColor();
		data.transform = getMatrixTransform();

		D3D10_BOX box;
		box.left = sizeof(VertexMultiTextureDynamic) * getInstanceIndex();
		box.right = sizeof(VertexMultiTextureDynamic) * (getInstanceIndex() + 1);
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;

		getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)&data, 0, 0);
		return true;
	}
	return false;
}

bool InstanceShapePlane::updateVertexBufferDynamicFromBase()
{
	VertexMultiTextureDynamic *pData = myNew VertexMultiTextureDynamic[getInstanceCount()];

	unsigned int renderCount = 0;
	int indexOffset = 0;
	for(std::vector<InstanceObject *>::const_iterator instanceIter = getInstanceList().begin();
		instanceIter != getInstanceList().end(); ++instanceIter)
	{
		if((*instanceIter)->getFlagRender() && !(*instanceIter)->getFlagCulled())
		{
			++renderCount;

			unsigned int arraySlice = ((InstanceShapePlane *)(*instanceIter))->getArraySlice();
			float arrayIndex1 = ((InstanceShapePlane *)(*instanceIter))->getTextureArrayIndex() + 0.01f;
			float arrayIndex2 = ((InstanceShapePlane *)(*instanceIter))->mArrayIndex2 + 0.01f;
			float arrayIndex3 = ((InstanceShapePlane *)(*instanceIter))->mArrayIndex3 + 0.01f;
			float arrayIndex4 = ((InstanceShapePlane *)(*instanceIter))->mArrayIndex4 + 0.01f;
			float textureID1 = ((InstanceShapePlane *)(*instanceIter))->getTextureID() + 0.01f;
			float textureID2 = ((InstanceShapePlane *)(*instanceIter))->mTextureID2 + 0.01f;
			float textureID3 = ((InstanceShapePlane *)(*instanceIter))->mTextureID3 + 0.01f;
			float textureID4 = ((InstanceShapePlane *)(*instanceIter))->mTextureID4 + 0.01f;

			pData[instanceIter - getInstanceList().begin() - indexOffset].arraySlice = arraySlice;
			pData[instanceIter - getInstanceList().begin() - indexOffset].arrayIndex = D3DXVECTOR4_16F(arrayIndex1, arrayIndex2,
				arrayIndex3, arrayIndex4);
			pData[instanceIter - getInstanceList().begin() - indexOffset].textureID = D3DXVECTOR4_16F(textureID1,
				textureID2, textureID3, textureID4);
			pData[instanceIter - getInstanceList().begin() - indexOffset].color = (const float *)(*instanceIter)->getColor();
			pData[instanceIter - getInstanceList().begin() - indexOffset].transform = (*instanceIter)->getMatrixTransform();
		}
		else
		{
			++indexOffset;
		}
	}

	setRenderCount(renderCount);

	D3D10_BOX box;
	box.left = 0;
	box.right = sizeof(VertexMultiTextureDynamic) * getRenderCount();
	box.top = 0;
	box.bottom = 1;
	box.front = 0;
	box.back = 1;

	getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)pData, 0, 0);

	delete [] pData;
	return true;
}

void InstanceShapePlane::createVertexBufferStatic()
{
	int nVertices = mWidth * mHeight;
	setVertexCount(nVertices);

	D3DXCOLOR diffuse(0.5f, 0.5f, 0.5f, 1.0f);
	D3DXCOLOR specular(0.0f, 0.0f, 0.0f, 1.0f);

	VertexMultiTextureStatic *vertices = myNew VertexMultiTextureStatic[nVertices];

	int index = 0;
	for(int z = 0; z < mHeight; ++z)
	{
		for(int x = 0; x < mWidth; ++x)
		{
			index = z * mWidth + x;
			vertices[index].position = D3DXVECTOR4_16F((x - mWidth / 2) * mOffsetX, 0.0f, (z - mHeight / 2) * mOffsetZ, 1.0f);
			vertices[index].normal = D3DXVECTOR4_16F(0.0f, 1.0f, 0.0f, 0.0f);
			vertices[index].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
			vertices[index].UV = D3DXVECTOR2_16F((float)x / ((mWidth - 1) / mUVScale), (float)z / ((mHeight - 1) / mUVScale));
			vertices[index].diffuse = (const float *)diffuse;
			vertices[index].specular = (const float *)specular;
		}
	}

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSizeStatic() * getVertexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = vertices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, &bufferData, getVertexBufferStaticPointer())))
	{
		ERROR_MESSAGE("Error creating vertex buffer.")
	}

	delete [] vertices;

	if(mpWeights)
	{
		delete [] mpWeights;
		mpWeights = NULL;
	}
	mpWeights = myNew D3DXVECTOR4[getVertexCount()];
	for(unsigned int n = 0; n < getVertexCount(); ++n)
	{
		mpWeights[n] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 0.0f);
	}
	mFlagUpdateWeights = true;
}

void InstanceShapePlane::createVertexBufferDynamic()
{

}

void InstanceShapePlane::createIndexBuffer()
{
	int nFaces = (mWidth - 1) * (mHeight - 1) * 2;
	int nIndices = nFaces * 3;
	setIndexCount(nIndices);

	unsigned int *indices = myNew unsigned int[nIndices];
	int current = 0;
	int currentVertex = 0;
	for(int z = 0; z < mHeight - 1; ++z)	
	{
		for(int x = 0; x < mWidth - 1; ++x)
		{
			currentVertex = z * mWidth + x;

			//First face of the quad.
			indices[current] = currentVertex;
			indices[current + 1] = currentVertex + mWidth;
			indices[current + 2] = currentVertex + 1;
		
			//Second face of the quad.
			indices[current + 3] = currentVertex + 1;
			indices[current + 4] = currentVertex + mWidth;
			indices[current + 5] = currentVertex + mWidth + 1;

			current += 6;
		}
	}

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(unsigned int) * getIndexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = indices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, &bufferData, getIndexBufferPointer())))
	{
		ERROR_MESSAGE("Error creating index buffer.")
	}

	delete [] indices;
}

void InstanceShapePlane::createIndexBufferAdjacent()
{
	int nFaces = (mWidth - 1) * (mHeight - 1) * 2;
	int nIndices = nFaces * 6;
	setIndexCount(nIndices);

	unsigned int *indices = myNew unsigned int[nIndices];
	int current = 0;
	int currentVertex = 0;
	for(int z = 0; z < mHeight - 1; ++z)	
	{
		for(int x = 0; x < mWidth - 1; ++x)
		{
			currentVertex = z * mWidth + x;

			//First face of the quad.
			indices[current] = currentVertex;
			indices[current + 1] = 0;
			indices[current + 2] = currentVertex + mWidth;
			indices[current + 3] = 0;
			indices[current + 4] = currentVertex + 1;
			indices[current + 5] = 0;
		
			//Second face of the quad.
			indices[current + 6] = currentVertex + 1;
			indices[current + 7] = 0;
			indices[current + 8] = currentVertex + mWidth;
			indices[current + 9] = 0;
			indices[current + 10] = currentVertex + mWidth + 1;
			indices[current + 11] = 0;

			current += 12;
		}
	}

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(unsigned int) * getIndexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = indices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, &bufferData, getIndexBufferPointer())))
	{
		ERROR_MESSAGE("Error creating index buffer.")
	}

	delete [] indices;
}