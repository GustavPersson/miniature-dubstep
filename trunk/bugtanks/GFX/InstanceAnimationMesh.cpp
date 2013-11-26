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

#include "InstanceAnimationMesh.h"
#include "DX10Module.h"

using namespace gfx;
using std::vector;

InstanceAnimationMesh::InstanceAnimationMesh(ID3D10Device *pDevice, Effect *pEffect)
	: InstanceMesh(pDevice, pEffect)
{
	setType(RenderObject::RenderTypes_AnimationObject);
	setEffectPassIndex(RenderObject::ShaderPasses_Animation);
	setEffectLayoutIndex(RenderObject::ShaderPasses_Animation);
}

InstanceAnimationMesh::InstanceAnimationMesh(const InstanceAnimationMesh &mesh)
	: InstanceMesh(mesh)
{

}

InstanceAnimationMesh::~InstanceAnimationMesh()
{

}

const InstanceAnimationMesh &InstanceAnimationMesh::operator=(const InstanceAnimationMesh &mesh)
{
	if(this != &mesh)
	{
		InstanceMesh::operator=(mesh);
	}
	return *this;
}

RenderObject *InstanceAnimationMesh::clone()
{
	return myNew InstanceAnimationMesh(*this);
}

InstanceObject *InstanceAnimationMesh::createInstance()
{
	if(getFlagInstanceBase())
	{
		if(getInstanceCount() < getInstanceCountMax())
		{
			InstanceAnimationMesh *newInstance = (InstanceAnimationMesh *)clone();
			newInstance->setFlagInstanceBase(false);
			newInstance->setInstanceIndex(getInstanceCount());
			for(vector<InstanceSubset *>::const_iterator n = getSubsetList().begin();
				n != getSubsetList().end(); ++n)
			{
				newInstance->addSubset((InstanceSubset *)((*n)->createInstance()));
			}
			addInstance(newInstance);
			return newInstance;
		}
	}
	return NULL;
}

void InstanceAnimationMesh::setAnimation(const char *animationName)
{
	int index = DX10Module::getInstance()->getAnimationIndex(animationName);

	for(vector<InstanceSubset *>::const_iterator n = getSubsetList().begin();
				n != getSubsetList().end(); ++n)
	{
		((InstanceAnimationSubset *)(*n))->setAnimationIndex(index);
	}
}

void InstanceAnimationMesh::setAnimation(const std::string &animationName)
{
	int index = DX10Module::getInstance()->getAnimationIndex(animationName);

	for(vector<InstanceSubset *>::const_iterator n = getSubsetList().begin();
				n != getSubsetList().end(); ++n)
	{
		((InstanceAnimationSubset *)(*n))->setAnimationIndex(index);
	}
}

void InstanceAnimationMesh::setAnimationIndex(int index)
{
	for(vector<InstanceSubset *>::const_iterator n = getSubsetList().begin();
				n != getSubsetList().end(); ++n)
	{
		((InstanceAnimationSubset *)(*n))->setAnimationIndex(index);
	}
}

void InstanceAnimationMesh::setAnimationSpeedFactor(float factor)
{
	for(vector<InstanceSubset *>::const_iterator n = getSubsetList().begin();
				n != getSubsetList().end(); ++n)
	{
		((InstanceAnimationSubset *)(*n))->setAnimationSpeedFactor(factor);
	}
}