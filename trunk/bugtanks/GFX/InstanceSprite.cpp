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

#include "InstanceSprite.h"
#include "CollisionObject.h"
#include "Vertex.h"
#include "Effect.h"

using namespace gfx;
using std::vector;

InstanceSprite::InstanceSprite(ID3D10Device *pDevice, Effect *pEffect) : InstanceObject(pDevice, pEffect)
{
	setEffectTechIndex(0);
	setEffectPassIndex(RenderObject::ShaderPasses_GUI);
	setEffectLayoutIndex(RenderObject::ShaderPasses_GUI);
	setFlagShadow(false);
}

InstanceSprite::InstanceSprite(const InstanceSprite &sprite) : InstanceObject(sprite)
{
	setEffectTechIndex(0);
	setEffectPassIndex(RenderObject::ShaderPasses_GUI);
	setEffectLayoutIndex(RenderObject::ShaderPasses_GUI);
	setFlagShadow(false);
}

InstanceSprite::~InstanceSprite()
{

}

const InstanceSprite &InstanceSprite::operator=(const InstanceSprite &sprite)
{
	if(this != &sprite)
	{
		InstanceObject::operator=(sprite);
	}
	return *this;
}

RenderObject *InstanceSprite::clone()
{
	return myNew InstanceSprite(*this);
}

void InstanceSprite::update(float deltaTime)
{
	InstanceObject::update(deltaTime);
}

InstanceObject *InstanceSprite::createInstance()
{
	if(getFlagInstanceBase())
	{
		if(getInstanceCount() < getInstanceCountMax())
		{
			InstanceObject *pNewInstance = (InstanceObject *)clone();
			pNewInstance->setFlagInstanceBase(false);
			pNewInstance->setInstanceIndex(getInstanceCount());
			linkVertexBufferDynamic(pNewInstance);
			addInstance(pNewInstance);
			return pNewInstance;
		}
	}
	return NULL;
}

InstanceObject *InstanceSprite::createInstanceSorted(float depth)
{
	if(getFlagInstanceBase())
	{
		if(getInstanceCount() < getInstanceCountMax())
		{
			InstanceObject *pNewInstance = (InstanceObject *)clone();
			pNewInstance->setFlagInstanceBase(false);
			pNewInstance->setInstanceIndex(getInstanceCount());
			pNewInstance->setPositionZ(depth);
			linkVertexBufferDynamic(pNewInstance);
			addInstanceSorted(pNewInstance);
			return pNewInstance;
		}
	}
	return NULL;
}

void InstanceSprite::loadBuffers()
{
	if(getFlagInstanceBase())
	{
		createVertexBufferStatic();
		createIndexBuffer();
	}
}

bool InstanceSprite::updateVertexBufferDynamic()
{
	if(InstanceObject::updateVertexBufferDynamic())
	{
		VertexGUIDynamic data;
		data.color = (const float *)getColor();
		data.arrayIndex = getTextureArrayIndex();
		data.textureID = getTextureID();
		data.transform = getMatrixTransform();

		D3D10_BOX box;
		box.left = sizeof(VertexGUIDynamic) * getInstanceIndex();
		box.right = sizeof(VertexGUIDynamic) * (getInstanceIndex() + 1);
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;

		getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)&data, 0, 0);
		return true;
	}
	return false;
}

bool InstanceSprite::updateVertexBufferDynamicFromBase()
{
	VertexGUIDynamic *pData = myNew VertexGUIDynamic[getInstanceCount()];

	unsigned int renderCount = 0;
	int indexOffset = 0;
	for(std::vector<InstanceObject *>::const_iterator instanceIter = getInstanceList().begin();
		instanceIter != getInstanceList().end(); ++instanceIter)
	{
		if((*instanceIter)->getFlagRender())
		{
			++renderCount;
			pData[instanceIter - getInstanceList().begin() - indexOffset].color = (const float *)(*instanceIter)->getColor();
			pData[instanceIter - getInstanceList().begin() - indexOffset].arrayIndex = (*instanceIter)->getTextureArrayIndex();
			pData[instanceIter - getInstanceList().begin() - indexOffset].textureID = (*instanceIter)->getTextureID();
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
	box.right = sizeof(VertexGUIDynamic) * getRenderCount();
	box.top = 0;
	box.bottom = 1;
	box.front = 0;
	box.back = 1;

	getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)pData, 0, 0);

	delete [] pData;
	return true;
}

void InstanceSprite::createVertexBufferStatic()
{
	setVertexCount(4);

	VertexGUIStatic vertices[4];

	vertices[0].position = D3DXVECTOR4_16F(0.0f, -1.0f, 1.0f, 1.0f);
	vertices[0].UV = D3DXVECTOR2_16F(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR4_16F(0.0f,  0.0f, 1.0f, 1.0f);
	vertices[1].UV = D3DXVECTOR2_16F(0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR4_16F(1.0f,  0.0f, 1.0f, 1.0f);
	vertices[2].UV = D3DXVECTOR2_16F(1.0f, 0.0f);

	vertices[3].position = D3DXVECTOR4_16F(1.0f, -1.0f, 1.0f, 1.0f);
	vertices[3].UV = D3DXVECTOR2_16F(1.0f, 1.0f);

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
}

void InstanceSprite::createVertexBufferDynamic()
{

}

void InstanceSprite::createIndexBuffer()
{
	setIndexCount(6);

	unsigned int indices[6];

	//Front faces.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

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
}

void InstanceSprite::createIndexBufferAdjacent()
{
	setIndexCount(12);

	unsigned int indices[12];

	//Front faces.
	indices[0] = 0;
	indices[1] = 0;
	indices[2] = 1;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 0;

	indices[6] = 0;
	indices[7] = 0;
	indices[8] = 2;
	indices[9] = 0;
	indices[10] = 3;
	indices[11] = 0;

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
}