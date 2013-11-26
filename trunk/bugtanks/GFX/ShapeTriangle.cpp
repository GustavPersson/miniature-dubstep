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

#include "ShapeTriangle.h"
#include "Sphere.h"
#include "Vertex.h"
#include "Effect.h"

using namespace gfx;

ShapeTriangle::ShapeTriangle(ID3D10Device *pDevice, Effect *pEffect) : BufferObject(pDevice, pEffect)
{
	createVertexBufferStatic();
	createVertexBufferDynamic();
	createIndexBuffer();
	setBoundingVolume(myNew Sphere(0.0f, 0.0f, 0.0f, 10.0f, -1));
}

ShapeTriangle::ShapeTriangle(const ShapeTriangle &triangle) : BufferObject(triangle)
{

}

ShapeTriangle::~ShapeTriangle()
{

}
	
const ShapeTriangle &ShapeTriangle::operator=(const ShapeTriangle &triangle)
{
	if(this != &triangle)
	{
		BufferObject::operator=(triangle);
	}
	return *this;
}

RenderObject *ShapeTriangle::clone()
{
	return myNew ShapeTriangle(*this);
}

void ShapeTriangle::update(float deltaTime)
{
	BufferObject::update(deltaTime);
}

bool ShapeTriangle::updateVertexBufferDynamic()
{
	VertexTransformDynamic *pData;
	getVertexBufferDynamic()->Map(D3D10_MAP_WRITE_DISCARD, 0, (void **)&pData);
	for(unsigned int n = 0; n < getVertexCount(); ++n)
	{
		pData[n].arrayIndex = getTextureArrayIndex();
		pData[n].textureID = getTextureID();
		pData[n].color = (const float *)getColor();
		pData[n].transform = getMatrixTransform();
	}
	getVertexBufferDynamic()->Unmap();
	return true;
}

void ShapeTriangle::createVertexBufferStatic()
{
	setVertexCount(3);

	D3DXCOLOR diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR specular(1.0f, 1.0f, 1.0f, 1.0f);

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

void ShapeTriangle::createVertexBufferDynamic()
{
	setVertexCount(3);

	VertexTransformDynamic vertices[3];
	vertices[0].textureID = -1;
	vertices[0].color = (const float *)getColor();
	vertices[0].transform = getMatrixTransform();

	vertices[1].textureID = -1;
	vertices[1].color = (const float *)getColor();
	vertices[1].transform = getMatrixTransform();

	vertices[2].textureID = -1;
	vertices[2].color = (const float *)getColor();
	vertices[2].transform = getMatrixTransform();

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSizeDynamic() * getVertexCount();
	bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = vertices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, &bufferData, getVertexBufferDynamicPointer())))
	{
		ERROR_MESSAGE("Error creating vertex buffer.")
	}
}

void ShapeTriangle::createIndexBuffer()
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

void ShapeTriangle::createIndexBufferAdjacent()
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