/**
 * @file
 * @author Martin Jönsson <snowman.mrgames@gmail.com>, Victor Lundberg <lundberg.victor@gmail.com>
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

#include "ShaderHandler.h"

using namespace gfx;
using std::vector;

ShaderHandler::ShaderHandler() : mEffectList()
{
	mpDevice = NULL;
}

ShaderHandler::ShaderHandler(const ShaderHandler &handler)
{
	mpDevice = handler.mpDevice;
}

ShaderHandler::~ShaderHandler()
{
	removeAllEffects();
}

const ShaderHandler &ShaderHandler::operator=(const ShaderHandler &handler)
{
	if(this != &handler)
	{
		mpDevice = handler.mpDevice;
	}
	return *this;
}

void ShaderHandler::init(ID3D10Device *pDevice)
{
	mpDevice = pDevice;
}

Effect *ShaderHandler::getEffect(unsigned int index)
{
	if(index > 0 && index < mEffectList.size())
	{
		return mEffectList.at(index);
	}
	return NULL;
}

Effect *ShaderHandler::getEffect(const char *name)
{
	for(vector<Effect *>::iterator n = mEffectList.begin();
		n != mEffectList.end(); ++n)
	{
		if(name == (*n)->getNameString())
		{
			return (*n);
		}
	}
	return NULL;
}

HRESULT ShaderHandler::addEffect(const char *fileName, const char *effectName)
{
	Effect *effect = myNew Effect(effectName, mpDevice);
	if(FAILED(effect->createEffect(fileName)))
	{
		delete effect;
		return E_FAIL;
	}
	mEffectList.push_back(effect);
	return S_OK;
}

void ShaderHandler::removeAllEffects()
{
	for(vector<Effect *>::iterator n = mEffectList.begin();
		n != mEffectList.end(); ++n)
	{
		delete (*n);
		(*n) = NULL;
	}
	mEffectList.clear();
}