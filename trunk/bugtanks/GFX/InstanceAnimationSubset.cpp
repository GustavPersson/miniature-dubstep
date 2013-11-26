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

#include "InstanceAnimationSubset.h"
#include "Vertex.h"

using namespace gfx;

InstanceAnimationSubset::InstanceAnimationSubset(ID3D10Device *pDevice, Effect *pEffect)
	: InstanceSubset(pDevice, pEffect)
{
	setType(RenderObject::RenderTypes_AnimationObject);
	setEffectPassIndex(RenderObject::ShaderPasses_Animation);
	setEffectLayoutIndex(RenderObject::ShaderPasses_Animation);
	mAnimationIndex = -1;
	mAnimationTime = 0.0f;
	mAnimationSpeedFactor = 1.0f;
	mAnimationLength = 0.0f;
	mPlayOnce = false;
}

InstanceAnimationSubset::InstanceAnimationSubset(const InstanceAnimationSubset &subset)
	: InstanceSubset(subset)
{
	mAnimationIndex = subset.mAnimationIndex;
	mAnimationTime = subset.mAnimationTime;
	mAnimationSpeedFactor = subset.mAnimationSpeedFactor;
	mAnimationLength = subset.mAnimationLength;
	mPlayOnce = subset.mPlayOnce;
}

InstanceAnimationSubset::~InstanceAnimationSubset()
{

}
	
const InstanceAnimationSubset &InstanceAnimationSubset::operator=(const InstanceAnimationSubset &subset)
{
	if(this != &subset)
	{
		InstanceSubset::operator=(subset);
		mAnimationIndex = subset.mAnimationIndex;
		mAnimationTime = subset.mAnimationTime;
		mAnimationSpeedFactor = subset.mAnimationSpeedFactor;
		mAnimationLength = subset.mAnimationLength;
		mPlayOnce = subset.mPlayOnce;
	}
	return *this;
}

RenderObject *InstanceAnimationSubset::clone()
{
	return myNew InstanceAnimationSubset(*this);
}

void InstanceAnimationSubset::update(float deltaTime)
{
	InstanceSubset::update(deltaTime);

	mAnimationTime += deltaTime * mAnimationSpeedFactor;
}

bool InstanceAnimationSubset::updateVertexBufferDynamic()
{
	if(InstanceObject::updateVertexBufferDynamic())
	{
		VertexAnimationDynamic data;
		data.arrayIndex = getTextureArrayIndex();
		data.textureID = getTextureID();
		data.color = (const float *)getColor();
		data.animationInfo = D3DXVECTOR2_16F((float)mAnimationIndex + 0.01f, mAnimationTime);
		data.transform = getMatrixTransform();

		D3D10_BOX box;
		box.left = sizeof(VertexAnimationDynamic) * getInstanceIndex();
		box.right = sizeof(VertexAnimationDynamic) * (getInstanceIndex() + 1);
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;

		getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)&data, 0, 0);
		return true;
	}
	return false;
}

bool InstanceAnimationSubset::updateVertexBufferDynamicFromBase()
{
	VertexAnimationDynamic *pData = myNew VertexAnimationDynamic[getInstanceCount()];

	unsigned int renderCount = 0;
	int indexOffset = 0;
	for(std::vector<InstanceObject *>::const_iterator instanceIter = getInstanceList().begin();
		instanceIter != getInstanceList().end(); ++instanceIter)
	{
		if((*instanceIter)->getFlagRender() && !(*instanceIter)->getFlagCulled())
		{
			++renderCount;
			InstanceAnimationSubset *pInstance = (InstanceAnimationSubset *)(*instanceIter);
			pData[instanceIter - getInstanceList().begin() - indexOffset].arrayIndex = pInstance->getTextureArrayIndex();
			pData[instanceIter - getInstanceList().begin() - indexOffset].textureID = pInstance->getTextureID();
			pData[instanceIter - getInstanceList().begin() - indexOffset].color = (const float *)pInstance->getColor();
			pData[instanceIter - getInstanceList().begin() - indexOffset].animationInfo = D3DXVECTOR2_16F((float)pInstance->mAnimationIndex * 1.01f, pInstance->mAnimationTime);
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
	box.right = sizeof(VertexAnimationDynamic) * getRenderCount();
	box.top = 0;
	box.bottom = 1;
	box.front = 0;
	box.back = 1;

	getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)pData, 0, 0);

	delete [] pData;
	return true;
}