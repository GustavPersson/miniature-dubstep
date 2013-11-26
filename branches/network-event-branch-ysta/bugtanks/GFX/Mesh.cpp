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

#include "Mesh.h"

using namespace gfx;
using std::vector;

Mesh::Mesh(ID3D10Device *pDevice, Effect *pEffect) : TransformObject(pDevice, pEffect), mSubsetList()
{
	mFlagUpdateAbsolute = true;
}

Mesh::Mesh(const Mesh &mesh) : TransformObject(mesh), mSubsetList()
{
	mFlagUpdateAbsolute = mesh.mFlagUpdateAbsolute;
	copySubsetsFrom(mesh);
}

Mesh::~Mesh()
{
	removeAllSubsets();
}

const Mesh &Mesh::operator=(const Mesh &mesh)
{
	if(this != &mesh)
	{
		TransformObject::operator=(mesh);
		mFlagUpdateAbsolute = mesh.mFlagUpdateAbsolute;
		copySubsetsFrom(mesh);
	}
	return *this;
}

RenderObject *Mesh::clone()
{
	return myNew Mesh(*this);
}

void Mesh::update()
{
	TransformObject::update();
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
	for(vector<Subset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->update();
	}
}

bool Mesh::render()
{
	if(RenderObject::render())
	{
		for(vector<Subset *>::iterator n = mSubsetList.begin();
			n != mSubsetList.end(); ++n)
		{
			(*n)->render();
		}
		return true;
	}
	return false;
}

void Mesh::updateTransformAbsolute()
{
	for(vector<Subset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setPosition(getPosition());
		(*n)->setScale(getScale());
		(*n)->setRotation(getRotation());
	}
}

void Mesh::updateTransformRelative()
{
	for(vector<Subset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setPosition(getPosition());
		(*n)->setScale(getScale());
	}
}

void Mesh::addSubset(Subset *pSubset)
{
	mSubsetList.push_back(pSubset);
}

Subset *Mesh::getSubsetByIndex(unsigned int subsetIndex)
{
	if(subsetIndex >= 0 && subsetIndex < mSubsetList.size())
	{
		return (*(mSubsetList.begin() + subsetIndex));
	}
	return NULL;
}

Subset *Mesh::getSubset(int subsetID)
{
	for(vector<Subset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		if((*n)->getID() == subsetID)
		{
			return (*n);
		}
	}
	return NULL;
}

Subset *Mesh::getSubset(const char *subsetName)
{
	for(vector<Subset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		if((*n)->getNameString() == subsetName)
		{
			return (*n);
		}
	}
	return NULL;
}

void Mesh::removeSubset(Subset *pSubset)
{
	for(vector<Subset *>::iterator n = mSubsetList.begin();
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

void Mesh::removeSubset(int subsetID)
{
	for(vector<Subset *>::iterator n = mSubsetList.begin();
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

void Mesh::removeSubset(const char *subsetName)
{
	for(vector<Subset *>::iterator n = mSubsetList.begin();
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

void Mesh::removeAllSubsets()
{
	for(vector<Subset *>::iterator n = mSubsetList.begin();
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

void Mesh::setColor(const D3DXCOLOR &color)
{
	RenderObject::setColor(color);
	for(vector<Subset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setColor(color);
	}
}

void Mesh::setTextureID(int textureID)
{
	RenderObject::setTextureID(textureID);
	for(vector<Subset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setTextureID(textureID);
	}
}
	
void Mesh::setFlagRender(bool render)
{
	RenderObject::setFlagRender(render);
	for(vector<Subset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setFlagRender(render);
	}
}

void Mesh::setEffectTechIndex(unsigned int techIndex)
{
	RenderObject::setEffectTechIndex(techIndex);
	for(vector<Subset *>::iterator n = mSubsetList.begin();
		n != mSubsetList.end(); ++n)
	{
		(*n)->setEffectTechIndex(techIndex);
	}
}

void Mesh::setFlagUpdateAbsolute(bool updateAbsolute)
{
	mFlagUpdateAbsolute = updateAbsolute;
}

void Mesh::copySubsetsFrom(const Mesh &sourceMesh)
{
	removeAllSubsets();
	for(vector<Subset *>::const_iterator n = sourceMesh.mSubsetList.begin();
		n != sourceMesh.mSubsetList.end(); ++n)
	{
		Subset *pNewSubset = (Subset *)((*n)->clone());
		addSubset(pNewSubset);
	}
}