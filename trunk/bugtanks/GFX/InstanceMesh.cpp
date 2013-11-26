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

#include "InstanceMesh.h"
#include "InstanceSubset.h"
#include <string>

using namespace gfx;
using std::vector;

InstanceMesh::InstanceMesh(ID3D10Device *pDevice, Effect *pEffect) : InstanceObject(pDevice, pEffect), mSubsetList()
{
	mFlagUpdateAbsolute = true;
}

InstanceMesh::InstanceMesh(const InstanceMesh &mesh) : InstanceObject(mesh), mSubsetList()
{
	mFlagUpdateAbsolute = mesh.mFlagUpdateAbsolute;
}

InstanceMesh::~InstanceMesh()
{
	if(getFlagInstanceBase())
	{
		removeAllSubsets();
	}
}

const InstanceMesh &InstanceMesh::operator=(const InstanceMesh &mesh)
{
	if(this != &mesh)
	{
		InstanceObject::operator=(mesh);
		mFlagUpdateAbsolute = mesh.mFlagUpdateAbsolute;
	}
	return *this;
}

RenderObject *InstanceMesh::clone()
{
	return myNew InstanceMesh(*this);
}

void InstanceMesh::update(float deltaTime)
{
	InstanceObject::update(deltaTime);
	if(getFlagMoved())
	{
		if(mFlagUpdateAbsolute)
		{
			updateTransformAbsolute();
		}
		else
		{
			updateTransformRelative();
		}
	}
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->update(deltaTime);
	}
}

bool InstanceMesh::render()
{
	if(getFlagInstanceBase())
	{
		if(RenderObject::render())
		{
			for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
				n != mSubsetList.end(); ++n)
			{
				(*n)->render();
			}
			return true;
		}
	}
	return false;
}

InstanceObject *InstanceMesh::createInstance()
{
	if(getFlagInstanceBase())
	{
		if(getInstanceCount() < getInstanceCountMax())
		{
			InstanceMesh *newInstance = (InstanceMesh *)clone();
			newInstance->setFlagInstanceBase(false);
			newInstance->setInstanceIndex(getInstanceCount());
			for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
				n != mSubsetList.end(); ++n)
			{
				newInstance->addSubset((InstanceSubset *)((*n)->createInstance()));
			}
			addInstance(newInstance);
			return newInstance;
		}
	}
	return NULL;
}

void InstanceMesh::removeInstance(InstanceObject *pInstance)
{
	if(getFlagInstanceBase())
	{
		//Remove subset instances.
		for(vector<InstanceObject *>::const_iterator meshIter = getInstanceList().begin();
			meshIter != getInstanceList().end(); ++meshIter)
		{
			//Find the correct mesh instance.
			if((*meshIter) == pInstance)
			{
				InstanceMesh *meshInstance = (InstanceMesh *)(*meshIter);
				//Remove each subset instance.
				for(vector<InstanceSubset *>::iterator subsetIter = mSubsetList.begin();
					subsetIter != mSubsetList.end(); ++subsetIter)
				{
					(*subsetIter)->removeInstance(meshInstance->getSubsetByIndex(subsetIter - mSubsetList.begin()));
				}
			}
		}
		//Remove mesh instance.
		InstanceObject::removeInstance(pInstance);
	}
}

void InstanceMesh::removeInstance(int instanceID)
{
	if(getFlagInstanceBase())
	{
		//Remove subset instances.
		for(vector<InstanceObject *>::const_iterator meshIter = getInstanceList().begin();
			meshIter != getInstanceList().end(); ++meshIter)
		{
			//Find the correct mesh instance.
			if((*meshIter)->getID() == instanceID)
			{
				InstanceMesh *meshInstance = (InstanceMesh *)(*meshIter);
				//Remove each subset instance.
				for(vector<InstanceSubset *>::iterator subsetIter = mSubsetList.begin();
					subsetIter != mSubsetList.end(); ++subsetIter)
				{
					(*subsetIter)->removeInstance(meshInstance->getSubsetByIndex(subsetIter - mSubsetList.begin()));
				}
			}
		}
		//Remove mesh instance.
		InstanceObject::removeInstance(instanceID);
	}
}

void InstanceMesh::removeInstance(const char *instanceName)
{
	if(getFlagInstanceBase())
	{
		//Remove subset instances.
		for(vector<InstanceObject *>::const_iterator meshIter = getInstanceList().begin();
			meshIter != getInstanceList().end(); ++meshIter)
		{
			//Find the correct mesh instance.
			if((*meshIter)->getNameString() == instanceName)
			{
				InstanceMesh *meshInstance = (InstanceMesh *)(*meshIter);
				//Remove each subset instance.
				for(vector<InstanceSubset *>::iterator subsetIter = mSubsetList.begin();
					subsetIter != mSubsetList.end(); ++subsetIter)
				{
					(*subsetIter)->removeInstance(meshInstance->getSubsetByIndex(subsetIter - mSubsetList.begin()));
				}
			}
		}
		//Remove mesh instance.
		InstanceObject::removeInstance(instanceName);
	}
}

void InstanceMesh::removeAllInstances()
{
	if(getFlagInstanceBase())
	{
		//Remove subset instances.
		for(vector<InstanceObject *>::const_iterator meshIter = getInstanceList().begin();
			meshIter != getInstanceList().end(); ++meshIter)
		{
			InstanceMesh *meshInstance = (InstanceMesh *)(*meshIter);
			//Remove each subset instance.
			for(vector<InstanceSubset *>::iterator subsetIter = mSubsetList.begin();
				subsetIter != mSubsetList.end(); ++subsetIter)
			{
				(*subsetIter)->removeInstance(meshInstance->getSubsetByIndex(subsetIter - mSubsetList.begin()));
			}
		}
		//Remove mesh instances.
		InstanceObject::removeAllInstances();
	}
}

void InstanceMesh::setInstanceCountMax(unsigned int max)
{
	InstanceObject::setInstanceCountMax(max);
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setInstanceCountMax(max);
	}
}

void InstanceMesh::updateTransformAbsolute()
{
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setPosition(getPosition());
		(*n)->setScale(getScale());
		(*n)->setRotation(getRotation());
	}
}

void InstanceMesh::updateTransformRelative()
{
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setPosition(getPosition());
		(*n)->setScale(getScale());
	}
	(*mSubsetList.begin())->setRotation(getRotation());
}

void InstanceMesh::addSubset(InstanceSubset *pSubset)
{
	if(getFlagInstanceBase())
	{
		if(pSubset->getFlagInstanceBase())
		{
			mSubsetList.push_back(pSubset);
		}
	}
	else
	{
		if(!pSubset->getFlagInstanceBase())
		{
			mSubsetList.push_back(pSubset);
		}
	}
}

InstanceSubset *InstanceMesh::getSubsetByIndex(unsigned int subsetIndex)
{
	if(subsetIndex >= 0 && subsetIndex < mSubsetList.size())
	{
		return (*(mSubsetList.begin() + subsetIndex));
	}
	return NULL;
}

InstanceSubset *InstanceMesh::getSubset(int subsetID)
{
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		if((*n)->getID() == subsetID)
		{
			return (*n);
		}
	}
	return NULL;
}

InstanceSubset *InstanceMesh::getSubset(const char *subsetName)
{
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		if((*n)->getNameString() == subsetName)
		{
			return (*n);
		}
	}
	return NULL;
}

void InstanceMesh::removeSubset(InstanceSubset *pSubset)
{
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		if((*n) == pSubset)
		{
			delete (*n);
			(*n) = NULL;
			mSubsetList.erase(n);
			break;
		}
	}
}

void InstanceMesh::removeSubset(int subsetID)
{
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		if((*n)->getID() == subsetID)
		{
			delete (*n);
			(*n) = NULL;
			mSubsetList.erase(n);
			break;
		}
	}
}

void InstanceMesh::removeSubset(const char *subsetName)
{
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		if((*n)->getNameString() == subsetName)
		{
			delete (*n);
			(*n) = NULL;
			mSubsetList.erase(n);
			break;
		}
	}
}

void InstanceMesh::removeAllSubsets()
{
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		if((*n))
		{
			delete (*n);
			(*n) = NULL;
		}
	}
	mSubsetList.clear();
}

void InstanceMesh::setID(int ID)
{
	RenderObject::setID(ID);
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setID(ID);
	}
}

void InstanceMesh::setColor(const D3DXCOLOR &color)
{
	RenderObject::setColor(color);
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setColor(color);
	}
}

void InstanceMesh::setTextureArrayIndex(int arrayIndex)
{
	RenderObject::setTextureArrayIndex(arrayIndex);
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setTextureArrayIndex(arrayIndex);
	}
}

void InstanceMesh::setTextureID(int textureID)
{
	RenderObject::setTextureID(textureID);
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setTextureID(textureID);
	}
}

void InstanceMesh::setFlagRender(bool render)
{
	RenderObject::setFlagRender(render);
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setFlagRender(render);
	}
}

void InstanceMesh::setFlagShadow(bool shadow)
{
	RenderObject::setFlagShadow(shadow);
	for(vector<InstanceSubset *>::iterator subsetIter = mSubsetList.begin();
		subsetIter != mSubsetList.end(); ++subsetIter)
	{
		(*subsetIter)->setFlagShadow(shadow);
	}
}

void InstanceMesh::setEffectPassIndex(int index)
{
	RenderObject::setEffectPassIndex(index);
	for(vector<InstanceSubset *>::iterator subsetIter = mSubsetList.begin();
		subsetIter != mSubsetList.end(); ++subsetIter)
	{
		(*subsetIter)->setEffectPassIndex(index);
	}
}

void InstanceMesh::setEffectTechIndex(unsigned int index)
{
	RenderObject::setEffectTechIndex(index);
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setEffectTechIndex(index);
	}
}

void InstanceMesh::setActor(void *pActor)
{
	RenderObject::setActor(pActor);
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setActor(pActor);
	}
}

void InstanceMesh::setFlagInstanceBase(bool instanceBase)
{
	InstanceObject::setFlagInstanceBase(instanceBase);
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setFlagInstanceBase(instanceBase);
	}
}

void InstanceMesh::setFlagUpdateFromBase(bool updateBase)
{
	InstanceObject::setFlagUpdateFromBase(updateBase);
	for(vector<InstanceSubset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setFlagUpdateFromBase(updateBase);
	}
}

void InstanceMesh::setFlagUpdateAbsolute(bool updateAbsolute)
{
	mFlagUpdateAbsolute = updateAbsolute;
}

bool InstanceMesh::updateVertexBufferDynamic()
{
	return false;
}

bool InstanceMesh::updateVertexBufferDynamicFromBase()
{
	return false;
}

void InstanceMesh::createVertexBufferStatic()
{

}

void InstanceMesh::createVertexBufferDynamic()
{

}

void InstanceMesh::createEmptyVertexBufferDynamic()
{

}

void InstanceMesh::createIndexBuffer()
{

}

void InstanceMesh::createIndexBufferAdjacent()
{

}