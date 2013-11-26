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

#include "InstanceObject.h"
#include "Effect.h"

using namespace gfx;
using std::vector;

InstanceObject::InstanceObject(ID3D10Device *pDevice, Effect *pEffect) : BufferObject(pDevice, pEffect), mInstanceList()
{
	setType(RenderTypes::INSTANCE_OBJECT);
	setEffectTechIndex(0);
	setEffectLayoutIndex(ShaderPasses::CEL);
	mFlagInstanceBase = false;
	mFlagUpdateFromBase = false;
	mFlagCulled = false;
	mInstanceCount = 0;
	mInstanceCountMax = 0;
	mRenderCount = 0;
	mInstanceIndex = 0;
	setFlagCopyBuffers(false);
	setFlagReleaseBuffers(false);
}

InstanceObject::InstanceObject(const InstanceObject &object) : BufferObject(object)
{
	setType(RenderTypes::INSTANCE_OBJECT);
	mFlagInstanceBase = object.mFlagInstanceBase;
	mFlagUpdateFromBase = object.mFlagUpdateFromBase;
	mFlagCulled = object.mFlagCulled;
	mInstanceCount = object.mInstanceCount;
	mInstanceCountMax = object.mInstanceCountMax;
	mRenderCount = object.mRenderCount;
	mInstanceIndex = object.mInstanceIndex;
}

InstanceObject::~InstanceObject()
{
	removeAllInstances();
}

const InstanceObject &InstanceObject::operator=(const InstanceObject &object)
{
	if(this != &object)
	{
		BufferObject::operator=(object);
		mFlagInstanceBase = object.mFlagInstanceBase;
		mFlagUpdateFromBase = object.mFlagUpdateFromBase;
		mFlagCulled = object.mFlagCulled;
		mInstanceCount = object.mInstanceCount;
		mInstanceCountMax = object.mInstanceCountMax;
		mRenderCount = object.mRenderCount;
		mInstanceIndex = object.mInstanceIndex;
	}
	return *this;
}

RenderObject *InstanceObject::clone()
{
	return NULL;
}

void InstanceObject::update(float deltaTime)
{
	if(mFlagInstanceBase)
	{
		for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
			n != mInstanceList.end(); ++n)
		{
			(*n)->update(deltaTime);
		}
	}
	if(!mFlagInstanceBase)
	{
		BufferObject::update(deltaTime);
	}
}

bool InstanceObject::render()
{
	if(mFlagInstanceBase)
	{
		if(TransformObject::render())
		{
			unsigned int nInstances = mInstanceCount;
			if(mInstanceCount <= 0)
			{
				return false;
			}
			if(mFlagUpdateFromBase)
			{
				updateVertexBufferDynamicFromBase();
				nInstances = mRenderCount;
			}
			ID3D10Buffer *buffers[2];
			buffers[0] = getVertexBufferStatic();
			buffers[1] = getVertexBufferDynamic();
			unsigned int stride[2];
			stride[0] = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSizeStatic();
			stride[1] = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSizeDynamic();
			unsigned int offset[2];
			offset[0] = 0;
			offset[1] = 0;
			getDevice()->IASetVertexBuffers(0, 2, buffers, stride, offset);
			getDevice()->IASetIndexBuffer(getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
			getDevice()->IASetPrimitiveTopology(TOPOLOGY);
			getDevice()->IASetInputLayout(getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getDXInputLayoutInstanced());

			ID3D10EffectTechnique *pEffectTechnique = getEffect()->getEffect()->GetTechniqueByIndex(getEffectTechIndex());
			if(getEffectPassIndex() == -1)
			{
				D3D10_TECHNIQUE_DESC techDesc;
				pEffectTechnique->GetDesc(&techDesc);
				for(unsigned int n = 0; n < techDesc.Passes; ++n)
				{
					pEffectTechnique->GetPassByIndex(n)->Apply(0);
					getDevice()->DrawIndexedInstanced(getIndexCount(), nInstances, 0, 0, 0);
				}
				return true;
			}
			else
			{
				pEffectTechnique->GetPassByIndex(getEffectPassIndex())->Apply(0);
				getDevice()->DrawIndexedInstanced(getIndexCount(), nInstances, 0, 0, 0);

				return true;
			}
		}
	}
	return false;
}

InstanceObject *InstanceObject::createInstance()
{
	if(mFlagInstanceBase)
	{
		++mInstanceCount;
	}
	return NULL;
}

InstanceObject *InstanceObject::getInstance(int instanceID)
{
	if(mFlagInstanceBase)
	{
		for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
			n != mInstanceList.end(); ++n)
		{
			if((*n)->getID() == instanceID)
			{
				return (*n);
			}
		}
	}
	return NULL;
}

InstanceObject *InstanceObject::getInstance(const char *instanceName)
{
	if(mFlagInstanceBase)
	{
		for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
			n != mInstanceList.end(); ++n)
		{
			if((*n)->getNameString() == instanceName)
			{
				return (*n);
			}
		}
	}
	return NULL;
}

void InstanceObject::removeInstance(InstanceObject *pInstance)
{
	if(mFlagInstanceBase)
	{
		for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
			n != mInstanceList.end(); ++n)
		{
			if((*n) == pInstance)
			{
				delete (*n);
				(*n) = NULL;
				mInstanceList.erase(n);
				--mInstanceCount;
				updateInstanceIndex();
				break;
			}
		}
	}
}

void InstanceObject::removeInstance(int instanceID)
{
	if(mFlagInstanceBase)
	{
		for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
			n != mInstanceList.end(); ++n)
		{
			if((*n)->getID() == instanceID)
			{
				delete (*n);
				(*n) = NULL;
				mInstanceList.erase(n);
				--mInstanceCount;
				updateInstanceIndex();
				break;
			}
		}
	}
}

void InstanceObject::removeInstance(const char *instanceName)
{
	if(mFlagInstanceBase)
	{
		for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
			n != mInstanceList.end(); ++n)
		{
			if((*n)->getNameString() == instanceName)
			{
				delete (*n);
				(*n) = NULL;
				mInstanceList.erase(n);
				--mInstanceCount;
				updateInstanceIndex();
				break;
			}
		}
	}
}

void InstanceObject::removeAllInstances()
{
	if(mFlagInstanceBase)
	{
		for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
			n != mInstanceList.end(); ++n)
		{
			if((*n))
			{
				delete (*n);
				(*n) = NULL;
			}
		}
		mInstanceList.clear();
		mInstanceCount = 0;
	}
}

void InstanceObject::setFlagShadow(bool shadow)
{
	RenderObject::setFlagShadow(shadow);
	for(vector<InstanceObject *>::iterator instanceIter = mInstanceList.begin();
		instanceIter != mInstanceList.end(); ++instanceIter)
	{
		(*instanceIter)->setFlagShadow(shadow);
	}
}

void InstanceObject::setEffectPassIndex(int index)
{
	RenderObject::setEffectPassIndex(index);
	for(vector<InstanceObject *>::iterator instanceIter = mInstanceList.begin();
		instanceIter != mInstanceList.end(); ++instanceIter)
	{
		(*instanceIter)->setEffectPassIndex(index);
	}
}

void InstanceObject::setFlagInstanceBase(bool instanceBase)
{
	mFlagInstanceBase = instanceBase;
	setFlagReleaseBuffers(instanceBase);
}

void InstanceObject::setFlagUpdateFromBase(bool updateBase)
{
	mFlagUpdateFromBase = updateBase;
	for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
		n != mInstanceList.end(); ++n)
	{
		(*n)->setFlagUpdateFromBase(updateBase);
	}
}

void InstanceObject::setInstanceCountMax(unsigned int max)
{
	mInstanceCountMax = max;
	createEmptyVertexBufferDynamic();
}

bool InstanceObject::updateVertexBufferDynamic()
{
	if(mFlagUpdateFromBase)
	{
		return false;
	}
	return true;
}

bool InstanceObject::updateVertexBufferDynamicFromBase()
{
	return true;
}

void InstanceObject::createVertexBufferStatic()
{

}

void InstanceObject::createVertexBufferDynamic()
{

}

void InstanceObject::createEmptyVertexBufferDynamic()
{
	if(mFlagInstanceBase)
	{
		if(mInstanceCountMax > 0)
		{
			clearVertexBufferDynamic();
			D3D10_BUFFER_DESC bufferDesc;
			bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
			bufferDesc.ByteWidth = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSizeDynamic() * mInstanceCountMax;
			bufferDesc.Usage = D3D10_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			if(FAILED(getDevice()->CreateBuffer(&bufferDesc, NULL, getVertexBufferDynamicPointer())))
			{
				ERROR_MESSAGE("Error creating vertex buffer.")
			}
			linkVertexBufferDynamic();
		}
	}
}

void InstanceObject::createIndexBuffer()
{

}

void InstanceObject::createIndexBufferAdjacent()
{

}

void InstanceObject::addInstance(InstanceObject *pInstance)
{
	if(mFlagInstanceBase)
	{
		mInstanceList.push_back(pInstance);
		++mInstanceCount;
	}
}

void InstanceObject::addInstanceSorted(InstanceObject *pInstance)
{
	if(getFlagInstanceBase())
	{
		float depth = pInstance->getPositionZ();
		bool added = false;
		for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
			n != mInstanceList.end(); ++n)
		{
			if((*n)->getPositionZ() > depth)
			{
				mInstanceList.insert(n, pInstance);
				added = true;
				break;
			}
		}
		if(!added)
		{
			mInstanceList.push_back(pInstance);
		}
		++mInstanceCount;
	}
}

void InstanceObject::updateInstanceIndex()
{
	for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
		n != mInstanceList.end(); ++n)
	{
		(*n)->setInstanceIndex(n - mInstanceList.begin());
	}
}

void InstanceObject::linkVertexBufferDynamic(InstanceObject *pInstance)
{
	pInstance->setVertexBufferDynamic(getVertexBufferDynamic());
}

void InstanceObject::linkVertexBufferDynamic()
{
	for(vector<InstanceObject *>::iterator n = mInstanceList.begin();
		n != mInstanceList.end(); ++n)
	{
		(*n)->setVertexBufferDynamic(getVertexBufferDynamic());
	}
}

void InstanceObject::linkVertexBufferStatic(InstanceObject *pInstance)
{
	pInstance->setVertexBufferStatic(getVertexBufferStatic());
}