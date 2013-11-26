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
* Renders scene to shadow map
*/

#ifndef __SHADOW_MAP_H__
#define __SHADOW_MAP_H__

#include "Texture2D.h"
#include <d3d10.h>
#include "Light.h"

class ShadowMap: public Texture2D
{
public:
	ShadowMap(ID3D10Device* device, int width, int height);
	~ShadowMap();

	void createFX(ID3D10Effect* renderEffect);
	void setLightPosition(Light light);
	void setDepthMap(ID3D10ShaderResourceView* depthMap);

	ID3D10EffectTechnique* getRenderTechnique();

private:
	ID3D10Effect						*mpRenderEffect;
	ID3D10EffectMatrixVariable			*fxLightWVPM;
	ID3D10EffectShaderResourceVariable	*fxDepthMap;
	ID3D10EffectTechnique				*mpRenderTechnique;

	D3DXMATRIX mLightM;
	D3DXMATRIX mLightProjM;
	D3DXMATRIX mLightViewM;
};

#endif