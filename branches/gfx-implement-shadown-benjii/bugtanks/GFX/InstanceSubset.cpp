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

#include "InstanceSubset.h"

using namespace gfx;

InstanceSubset::InstanceSubset(ID3D10Device *pDevice, Effect *pEffect) : InstanceObject(pDevice, pEffect)
{

}

InstanceSubset::InstanceSubset(const InstanceSubset &subset) : InstanceObject(subset)
{

}

InstanceSubset::~InstanceSubset()
{

}

const InstanceSubset &InstanceSubset::operator=(const InstanceSubset &subset)
{
	if(this != &subset)
	{
		InstanceObject::operator=(subset);
	}
	return *this;
}

RenderObject *InstanceSubset::clone()
{
	return myNew InstanceSubset(*this);
}

void InstanceSubset::update()
{
	InstanceObject::update();
}

InstanceObject *InstanceSubset::createInstance()
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

void InstanceSubset::loadBuffers(unsigned int vertexCount, ID3D10Buffer *pVertexBufferStatic,
								 unsigned int indexCount, ID3D10Buffer *pIndexBuffer)
{
	if(getFlagInstanceBase())
	{
		setVertexCount(vertexCount);
		setVertexBufferStatic(pVertexBufferStatic);
		setIndexCount(indexCount);
		setIndexBuffer(pIndexBuffer);
	}
}

bool InstanceSubset::updateVertexBufferDynamic()
{
	if(InstanceObject::updateVertexBufferDynamic())
	{
		VertexTransformDynamic data;
		data.textureID = getTextureID();
		data.color = getColor();
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

bool InstanceSubset::updateVertexBufferDynamicFromBase()
{
	VertexTransformDynamic *pData = myNew VertexTransformDynamic[getInstanceCount()];

	unsigned int renderCount = 0;
	int indexOffset = 0;
	for(std::vector<InstanceObject *>::const_iterator instanceIter = getInstanceList().begin();
		instanceIter != getInstanceList().end(); ++instanceIter)
	{
		if((*instanceIter)->getFlagRender())
		{
			++renderCount;
			pData[instanceIter - getInstanceList().begin() - indexOffset].textureID = (*instanceIter)->getTextureID();
			pData[instanceIter - getInstanceList().begin() - indexOffset].color = (*instanceIter)->getColor();
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
	box.right = sizeof(VertexTransformDynamic) * getInstanceCount();
	box.top = 0;
	box.bottom = 1;
	box.front = 0;
	box.back = 1;

	getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)pData, 0, 0);

	delete [] pData;
	return true;
}

void InstanceSubset::createVertexBufferStatic()
{

}

void InstanceSubset::createVertexBufferDynamic()
{

}

void InstanceSubset::createIndexBuffer()
{

}