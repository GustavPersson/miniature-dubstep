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

#include "Subset.h"

using namespace gfx;

Subset::Subset(ID3D10Device *pDevice, Effect *pEffect) : BufferObject(pDevice, pEffect)
{

}

Subset::Subset(const Subset &subset) : BufferObject(subset)
{

}

Subset::~Subset()
{

}

const Subset &Subset::operator=(const Subset &subset)
{
	if(this != &subset)
	{
		BufferObject::operator=(subset);
	}
	return *this;
}

RenderObject *Subset::clone()
{
	return myNew Subset(*this);
}

void Subset::update()
{
	BufferObject::update();
}

void Subset::loadBuffers(unsigned int vertexCount, ID3D10Buffer *pVertexBufferStatic, ID3D10Buffer *pVertexBufferDynamic,
						 unsigned int indexCount, ID3D10Buffer *pIndexBuffer)
{
	setVertexCount(vertexCount);
	setVertexBufferStatic(pVertexBufferStatic);
	setVertexBufferDynamic(pVertexBufferDynamic);
	setIndexCount(indexCount);
	setIndexBuffer(pIndexBuffer);
}

bool Subset::updateVertexBufferDynamic()
{
	VertexTransformDynamic *pData;
	getVertexBufferDynamic()->Map(D3D10_MAP_WRITE_DISCARD, 0, (void **)&pData);
	for(unsigned int n = 0; n < getVertexCount(); ++n)
	{
		pData[n].color = getColor();
		pData[n].transform = getMatrixTransform();
	}
	getVertexBufferDynamic()->Unmap();
	return true;
}

void Subset::createVertexBufferStatic()
{

}

void Subset::createVertexBufferDynamic()
{

}

void Subset::createIndexBuffer()
{

}