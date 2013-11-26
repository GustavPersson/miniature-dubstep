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
* Quad rendering
*/

#ifndef __SCREENQUAD_H__
#define __SCREENQUAD_H__

#include <d3d10.h>
#include <d3dx10.h>
#include "../Utilities/Macros.h"

class ScreenQuad
{
public:
	ScreenQuad(ID3D10Device* device);
	~ScreenQuad();

	void bindTexture(ID3D10ShaderResourceView* texture);
	void render();
	void createFX();

private:
	ID3D10Device						*mpDevice;
	ID3D10EffectShaderResourceVariable	*mTexture;
	ID3D10Buffer						*mVB;
	ID3D10InputLayout					*mInputLayout;
	ID3D10Effect						*fxEffect;
	ID3D10EffectTechnique				*fxTechnique;

	int	mNumVertices;
	int	mNumIndices;

};

#endif