/**
 * @file
 * @author Benjamin Golba <benjii.g@gmail.com>
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
 * 
 * @section DESCRIPTION
 *
 * Light source settings
 */

#include "Light.h"

using namespace gfx;

Light::Light()
{
}

Light::Light(D3DXVECTOR3 pos, D3DXVECTOR3 dir, D3DXCOLOR amb, D3DXCOLOR diff, D3DXCOLOR spec, float power) 
{
	mLight.pos		= pos;
	mLight.dir		= dir;
	mLight.ambient	= amb;
	mLight.diffuse	= diff;
	mLight.specular = spec;
	mLight.power	= power;
}

Light::~Light() 
{
}

void Light::setLightPos(D3DXVECTOR3 pos) 
{
	mLight.pos = pos;
}

void Light::setLightDir(D3DXVECTOR3 dir) 
{
	mLight.dir = dir;
}

void Light::setAmbientLight(D3DXCOLOR amb) 
{
	mLight.ambient = amb;
}

void Light::setDiffuseLight(D3DXCOLOR diff) 
{ 
	mLight.diffuse = diff;
}

void Light::setSpecularLight(D3DXCOLOR spec) 
{ 
	mLight.specular = spec;
}

void Light::setLightPower(float power) 
{ 
	mLight.power = power;
}

D3DXVECTOR3 Light::getLightPos() 
{ 
	return mLight.pos;
}

D3DXVECTOR3 Light::getLightDir() 
{ 
	return mLight.dir;
}

D3DXCOLOR Light::getAmbientLight() 
{ 
	return mLight.ambient;
}

D3DXCOLOR Light::getDiffuseLight() 
{ 
	return mLight.diffuse;
}

D3DXCOLOR Light::getSpecularLight() 
{ 
	return mLight.specular;
}

float Light::getLightPower() 
{ 
	return mLight.power;
}