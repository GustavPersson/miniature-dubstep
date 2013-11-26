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

#include "InstanceShapeTriangle.h"
#include "Vertex.h"
#include "Effect.h"

using namespace gfx;


InstanceShapeTriangle::InstanceShapeTriangle(ID3D10Device *pDevice, Effect *pEffect) : InstanceObject(pDevice, pEffect)
{

}

InstanceShapeTriangle::InstanceShapeTriangle(const InstanceShapeTriangle &cube) : InstanceObject(cube)
{

}

InstanceShapeTriangle::~InstanceShapeTriangle()
{

}

const InstanceShapeTriangle &InstanceShapeTriangle::operator=(const InstanceShapeTriangle &cube)
{
	if(this != &cube)
	{
		InstanceObject::operator=(cube);
	}
	return *this;
}

RenderObject *InstanceShapeTriangle::clone()
{
	return myNew InstanceShapeTriangle(*this);
}

void InstanceShapeTriangle::update(float deltaTime)
{
	InstanceObject::update(deltaTime);
}

InstanceObject *InstanceShapeTriangle::createInstance()
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

void InstanceShapeTriangle::loadBuffers()
{
	if(getFlagInstanceBase())
	{
		createVertexBufferStatic();
		createIndexBuffer();
	}
}

bool InstanceShapeTriangle::updateVertexBufferDynamic()
{
	if(InstanceObject::updateVertexBufferDynamic())
	{
		VertexTransformDynamic data;
		data.arrayIndex = getTextureArrayIndex();
		data.textureID = getTextureID();
		data.color = (const float *)getColor();
		data.transform = getMatrixTransform();

		D3D10_BOX box;
		box.left = sizeof(VertexTransformDynamic) * getInstanceIndex();
		box.right = sizeof(VertexTransformDynamic) * (getInstanceIndex() + 1);
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;

		getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)&data, 0, 0);
		return true;
	}
	return false;
}

bool InstanceShapeTriangle::updateVertexBufferDynamicFromBase()
{
	VertexTransformDynamic *pData = myNew VertexTransformDynamic[getInstanceCount()];

	unsigned int renderCount = 0;
	int indexOffset = 0;
	for(std::vector<InstanceObject *>::const_iterator instanceIter = getInstanceList().begin();
		instanceIter != getInstanceList().end(); ++instanceIter)
	{
		if((*instanceIter)->getFlagRender() && !(*instanceIter)->getFlagCulled())
		{
			++renderCount;
			pData[instanceIter - getInstanceList().begin() - indexOffset].arrayIndex = (*instanceIter)->getTextureArrayIndex();
			pData[instanceIter - getInstanceList().begin() - indexOffset].textureID = (*instanceIter)->getTextureID();
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
	box.right = sizeof(VertexTransformDynamic) * getRenderCount();
	box.top = 0;
	box.bottom = 1;
	box.front = 0;
	box.back = 1;

	getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)pData, 0, 0);

	delete [] pData;
	return true;
}

void InstanceShapeTriangle::createVertexBufferStatic()
{
	setVertexCount(3);

	D3DXCOLOR diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR specular(0.0f, 0.0f, 0.0f, 1.0f);

	VertexTransformStatic vertices[3];
	vertices[0].position = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR4_16F(0.0f, 0.0f, 0.0f, 0.0f);
	vertices[0].tangent = D3DXVECTOR4_16F(0.0f, 0.0f, 0.0f, 0.0f);
	vertices[0].UV = D3DXVECTOR2_16F(0.0f, 0.0f);
	vertices[0].diffuse = (const float *)diffuse;
	vertices[0].specular = (const float *)specular;

	vertices[1].position = D3DXVECTOR4_16F(-1.0f, -1.0f, 0.0f, 1.0f);
	vertices[1].normal = D3DXVECTOR4_16F(0.0f, 0.0f, 0.0f, 0.0f);
	vertices[1].tangent = D3DXVECTOR4_16F(0.0f, 0.0f, 0.0f, 0.0f);
	vertices[1].UV = D3DXVECTOR2_16F(0.0f, 0.0f);
	vertices[1].diffuse = (const float *)diffuse;
	vertices[1].specular = (const float *)specular;

	vertices[2].position = D3DXVECTOR4_16F(-0.5f, 1.0f, 0.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR4_16F(0.0f, 0.0f, 0.0f, 0.0f);
	vertices[2].tangent = D3DXVECTOR4_16F(0.0f, 0.0f, 0.0f, 0.0f);
	vertices[2].UV = D3DXVECTOR2_16F(0.0f, 0.0f);
	vertices[2].diffuse = (const float *)diffuse;
	vertices[2].specular = (const float *)specular;

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

void InstanceShapeTriangle::createVertexBufferDynamic()
{

}

void InstanceShapeTriangle::createIndexBuffer()
{
	setIndexCount(3);

	unsigned int indices[3];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

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

void InstanceShapeTriangle::createIndexBufferAdjacent()
{
	setIndexCount(6);

	unsigned int indices[6];
	indices[0] = 0;
	indices[1] = 0;
	indices[2] = 1;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 0;

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