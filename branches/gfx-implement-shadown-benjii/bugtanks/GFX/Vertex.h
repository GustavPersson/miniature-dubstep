/**
 * @file
 * @author Benjamin Golba <benjii.g@gmail.com>, Victor Lundberg <lundberg.victor@gmail.com>
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

#ifndef VERTEX_H
#define VERTEX_H
#include "../Utilities/Macros.h"
#include <d3dx10.h>

namespace gfx
{
	namespace VertexTypes
	{
		enum Types
		{
			VERTEX_TRANSFORM = 1
		};
	}

	/**
	 * Initializes all static D3D10_INPUT_ELEMENT_DESC-structs.
	 */
	void InitVertexStructures();

	/**
	 * Deletes all static D3D10_INPUT_ELEMENT_DESC-structs.
	 */
	void DestroyVertexStructures();

	/**
	 * Vertex with position, normal, tangent, UV, color, transform matrix.
	 */
	struct VertexTransform
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR2 UV;
		int textureID;
		D3DXCOLOR color;
		D3DXMATRIX transform;

		VertexTransform() : position(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f),
							tangent(0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f),
							color(0.0f, 0.0f, 0.0f, 0.0f)
		{
			D3DXMatrixIdentity(&transform);
		}

		~VertexTransform()
		{

		}

		static const unsigned int NUM_ELEMENTS = 10;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Instanced version of VertexTransform.
	 */
	struct VertexTransformInstance
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR2 UV;
		int textureID;
		D3DXCOLOR color;
		D3DXMATRIX transform;

		VertexTransformInstance() : position(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f),
									tangent(0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f),
									color(0.0f, 0.0f, 0.0f, 0.0f)
		{
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexTransformInstance()
		{

		}

		static const unsigned int NUM_ELEMENTS = 10;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Static part of VertexTransform.
	 */
	struct VertexTransformStatic
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR2 UV;

		VertexTransformStatic() : position(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f),
								  tangent(0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f)
		{

		}

		~VertexTransformStatic()
		{

		}
	};

	/**
	 * Dynamic part of VertexTransform.
	 */
	struct VertexTransformDynamic
	{
		int textureID;
		D3DXCOLOR color;
		D3DXMATRIX transform;

		VertexTransformDynamic() : color(0.0f, 0.0f, 0.0f, 0.0f)
		{
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexTransformDynamic()
		{

		}
	};
}

#endif