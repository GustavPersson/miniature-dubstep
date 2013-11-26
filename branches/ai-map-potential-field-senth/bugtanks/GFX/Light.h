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

#ifndef LIGHT_H
#define LIGHT_H

#include "Common.h"

namespace gfx
{

struct LightStruct
{
	LightStruct()
	{
		ZeroMemory(this, sizeof(LightStruct));
	}

	D3DXVECTOR3 pos;
	float		pad1;

	D3DXVECTOR3 dir;
	float		pad2;

	D3DXCOLOR	ambient;
	D3DXCOLOR	diffuse;
	D3DXCOLOR	specular;
};

class Light
{
public:

	/**
	 * Constructor
	 */
	Light();

	/**
	 * Constructor
	 * @param pos   Light position
	 * @param dir   Light direction
	 * @param amb   Ambient
	 * @param diff  Diffuse
	 * @param spec  Specular
	 * @param power Power of ambient, diffuse and specular lighting
	 */
	Light(D3DXVECTOR3 pos, D3DXVECTOR3 dir, D3DXCOLOR amb, D3DXCOLOR diff, D3DXCOLOR spec, float power);

	/**
	 * Destructor.
	 */
	~Light();

	/**
	 * Sets light frustum (projection matrix)
	 * @param width Width of the frustum
	 * @param height Height of the frustum
	 * @param near Near plane
	 * @param far Far plane
	 */
	void MatrixOrthoLH(float width, float height, float near, float far);

	/**
	 * Updates view matrix
	 */
	void MatrixLookAtLH();

	/**
	 * Calculate vector between target and position
	 */
	void calcTargetVector(const D3DXVECTOR3 &target);

	/**
	 * Sets light position
	 * @param pos Position
	 */
	inline void setLightPos(D3DXVECTOR3 pos) { mLight.pos = pos; }

	/**
	 * Sets light direction
	 * @param dir Direction
	 */
	inline void setLightDir(D3DXVECTOR3 dir) { mLight.dir = dir; }

	/**
	 * Sets ambient lighting
	 * @param amb Ambient color
	 */
	inline void setAmbientLight(D3DXCOLOR amb) { mLight.ambient = amb; }

	/**
	 * Sets diffuse lighting
	 * @param diff Diffuse color
	 */
	inline void setDiffuseLight(D3DXCOLOR diff) { mLight.diffuse = diff; }

	/**
	 * Sets specular lighting
	 * @param spec Specular color
	 */
	inline void setSpecularLight(D3DXCOLOR spec) { mLight.specular = spec; }

	/**
	 * Returns light position
	 */
	inline const D3DXVECTOR3 getLightPos() { return mLight.pos; }

	/**
	 * Returns light direction
	 */
	inline const D3DXVECTOR3 getLightDir() { return mLight.dir; }

	/**
	 * Returns ambient lighting
	 */
	inline const D3DXCOLOR getAmbientLight() { return mLight.ambient; }

	/**
	 * Returns diffuse lighting
	 */
	inline const D3DXCOLOR getDiffuseLight() { return mLight.diffuse; }

	/**
	 * Returns specular lighting
	 */
	inline const D3DXCOLOR getSpecularLight() { return mLight.specular; }

	/**
	 * Returns light struct
	 */
	inline const LightStruct getLightStruct() { return mLight; }

	/**
	 * Returns lights projection matrix
	 */
	inline const D3DXMATRIX getProjMatrix() { return mProjM; }
	
	/**
	 * Returns lights view matrix
	 */
	inline const D3DXMATRIX getViewMatrix() { return mViewM; }

private:
	LightStruct mLight;
	D3DXMATRIX	mProjM, 
				mViewM;
};

}
#endif