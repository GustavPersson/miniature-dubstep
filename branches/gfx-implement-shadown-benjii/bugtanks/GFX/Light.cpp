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
 * Class to handle lighting
 */

#include "Light.h"

using namespace gfx;

Light::Light()
{
	mLight.pos		= D3DXVECTOR3(0,0,0);
	mLight.dir		= D3DXVECTOR3(0,0,0);
	mLight.ambient	= D3DXCOLOR(0,0,0,1);
	mLight.diffuse	= D3DXCOLOR(0,0,0,1);
	mLight.specular = D3DXCOLOR(0,0,0,1);
	mLight.power	= 0.0f;
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

void Light::MatrixOrthoLH(float width, float height, float np, float fp)
{
	D3DXMatrixOrthoLH(&mProjM, width, height, np, fp);
}

void Light::MatrixLookAtLH()
{
	D3DXMatrixLookAtLH(&mViewM, &mLight.pos, &mLight.dir, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
}