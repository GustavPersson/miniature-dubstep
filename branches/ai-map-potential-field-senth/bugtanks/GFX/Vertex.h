/**
 * @file
 * @author Benjamin Golba <benjii.g@gmail.com>
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
			VERTEX_TRANSFORM = 1,
			VERTEX_ANIMATION,
			VERTEX_MULTI_TEXTURE,
			VERTEX_GUI,
			VERTEX_PARTICLES
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
		D3DXVECTOR4_16F position;
		D3DXVECTOR4_16F normal;
		D3DXVECTOR4_16F tangent;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F diffuse;
		D3DXVECTOR4_16F specular;
		int arrayIndex;
		int textureID;
		D3DXVECTOR4_16F color;
		D3DXMATRIX transform;

		VertexTransform() : position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 0.0f),
			tangent(0.0f, 0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f), diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			specular(0.0f, 0.0f, 0.0f, 1.0f), color(1.0f, 1.0f, 1.0f, 1.0f)
		{
			arrayIndex = 0;
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexTransform()
		{

		}

		static const unsigned int NUM_ELEMENTS = 13;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Instanced version of VertexTransform.
	 */
	struct VertexTransformInstance
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR4_16F normal;
		D3DXVECTOR4_16F tangent;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F diffuse;
		D3DXVECTOR4_16F specular;
		int arrayIndex;
		int textureID;
		D3DXVECTOR4_16F color;
		D3DXMATRIX transform;

		VertexTransformInstance() : position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 0.0f),
			tangent(0.0f, 0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f), diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			specular(0.0f, 0.0f, 0.0f, 1.0f), color(1.0f, 1.0f, 1.0f, 1.0f)
		{
			arrayIndex = 0;
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexTransformInstance()
		{

		}

		static const unsigned int NUM_ELEMENTS = 13;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Static part of VertexTransform.
	 */
	struct VertexTransformStatic
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR4_16F normal;
		D3DXVECTOR4_16F tangent;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F diffuse;
		D3DXVECTOR4_16F specular;

		VertexTransformStatic() : position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 0.0f),
			tangent(0.0f, 0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f), diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			specular(0.0f, 0.0f, 0.0f, 1.0f)
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
		int arrayIndex;
		int textureID;
		D3DXVECTOR4_16F color;
		D3DXMATRIX transform;

		VertexTransformDynamic() : color(1.0f, 1.0f, 1.0f, 1.0f)
		{
			arrayIndex = 0;
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexTransformDynamic()
		{

		}
	};

	/**
	 * Vertex for animated meshes.
	 */
	struct VertexAnimation
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR4_16F normal;
		D3DXVECTOR4_16F tangent;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F diffuse;
		D3DXVECTOR4_16F specular;
		D3DXVECTOR4_16F joints;
		D3DXVECTOR4_16F weights;
		int arrayIndex;
		int textureID;
		D3DXVECTOR4_16F color;
		D3DXVECTOR2_16F animationInfo;
		D3DXMATRIX transform;

		VertexAnimation() : position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 0.0f),
			tangent(0.0f, 0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f), diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			specular(0.0f, 0.0f, 0.0f, 1.0f), joints(0.0f, 0.0f, 0.0f, 0.0f),
			weights(0.0f, 0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f, 1.0f), animationInfo(0.0f, 0.0f)
		{
			arrayIndex = 0;
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexAnimation()
		{

		}

		static const unsigned int NUM_ELEMENTS = 16;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Instanced version of VertexAnimation.
	 */
	struct VertexAnimationInstance
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR4_16F normal;
		D3DXVECTOR4_16F tangent;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F diffuse;
		D3DXVECTOR4_16F specular;
		D3DXVECTOR4_16F joints;
		D3DXVECTOR4_16F weights;
		int arrayIndex;
		int textureID;
		D3DXVECTOR4_16F color;
		D3DXVECTOR2_16F animationInfo;
		D3DXMATRIX transform;

		VertexAnimationInstance() : position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 0.0f),
			tangent(0.0f, 0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f), diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			specular(0.0f, 0.0f, 0.0f, 1.0f), joints(0.0f, 0.0f, 0.0f, 0.0f),
			weights(0.0f, 0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f, 1.0f), animationInfo(0.0f, 0.0f)
		{
			arrayIndex = 0;
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexAnimationInstance()
		{

		}

		static const unsigned int NUM_ELEMENTS = 16;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Static part of VertexAnimation.
	 */
	struct VertexAnimationStatic
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR4_16F normal;
		D3DXVECTOR4_16F tangent;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F diffuse;
		D3DXVECTOR4_16F specular;
		D3DXVECTOR4_16F joints;
		D3DXVECTOR4_16F weights;

		VertexAnimationStatic() : position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 0.0f),
			tangent(0.0f, 0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f), diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			specular(0.0f, 0.0f, 0.0f, 1.0f), joints(0.0f, 0.0f, 0.0f, 0.0f),
			weights(0.0f, 0.0f, 0.0f, 0.0f)
		{

		}

		~VertexAnimationStatic()
		{

		}
	};

	/**
	 * Dynamic part of VertexAnimation.
	 */
	struct VertexAnimationDynamic
	{
		int arrayIndex;
		int textureID;
		D3DXVECTOR4_16F color;
		D3DXVECTOR2_16F animationInfo;
		D3DXMATRIX transform;

		VertexAnimationDynamic() : color(1.0f, 1.0f, 1.0f, 1.0f), animationInfo(0.0f, 0.0f)
		{
			arrayIndex = 0;
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexAnimationDynamic()
		{

		}
	};

	/**
	 * Vertex for multitexturable objects.
	 */
	struct VertexMultiTexture
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR4_16F normal;
		D3DXVECTOR4_16F tangent;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F diffuse;
		D3DXVECTOR4_16F specular;
		unsigned int arraySlice;
		D3DXVECTOR4_16F arrayIndex;
		D3DXVECTOR4_16F textureID;
		D3DXVECTOR4_16F color;
		D3DXMATRIX transform;

		VertexMultiTexture() : position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 0.0f),
			tangent(0.0f, 0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f), diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			specular(0.0f, 0.0f, 0.0f, 1.0f), arrayIndex(0.0f, 0.0f, 0.0f, 0.0f),
			textureID(0.0f, 0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f, 1.0f)
		{
			arraySlice = 0;
			D3DXMatrixIdentity(&transform);
		}

		~VertexMultiTexture()
		{

		}

		static const unsigned int NUM_ELEMENTS = 14;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Instanced version of VertexMultiTexture.
	 */
	struct VertexMultiTextureInstance
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR4_16F normal;
		D3DXVECTOR4_16F tangent;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F diffuse;
		D3DXVECTOR4_16F specular;
		unsigned int arraySlice;
		D3DXVECTOR4_16F arrayIndex;
		D3DXVECTOR4_16F textureID;
		D3DXVECTOR4_16F color;
		D3DXMATRIX transform;

		VertexMultiTextureInstance() : position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 0.0f),
			tangent(0.0f, 0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f), diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			specular(0.0f, 0.0f, 0.0f, 1.0f), arrayIndex(0.0f, 0.0f, 0.0f, 0.0f),
			textureID(0.0f, 0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f, 1.0f)
		{
			arraySlice = 0;
			D3DXMatrixIdentity(&transform);
		}

		~VertexMultiTextureInstance()
		{

		}

		static const unsigned int NUM_ELEMENTS = 14;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Static part VertexMultiTexture.
	 */
	struct VertexMultiTextureStatic
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR4_16F normal;
		D3DXVECTOR4_16F tangent;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F diffuse;
		D3DXVECTOR4_16F specular;

		VertexMultiTextureStatic() : position(0.0f, 0.0f, 0.0f, 1.0f), normal(0.0f, 0.0f, 0.0f, 0.0f),
			tangent(0.0f, 0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f), diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			specular(0.0f, 0.0f, 0.0f, 1.0f)
		{

		}

		~VertexMultiTextureStatic()
		{

		}
	};

	/**
	 * Dynamic part of VertexMultiTexture.
	 */
	struct VertexMultiTextureDynamic
	{
		unsigned int arraySlice;
		D3DXVECTOR4_16F arrayIndex;
		D3DXVECTOR4_16F textureID;
		D3DXVECTOR4_16F color;
		D3DXMATRIX transform;

		VertexMultiTextureDynamic() : color(1.0f, 1.0f, 1.0f, 1.0f), arrayIndex(0.0f, 0.0f, 0.0f, 0.0f),
			textureID(0.0f, 0.0f, 0.0f, 0.0f)
		{
			arraySlice = 0;
			D3DXMatrixIdentity(&transform);
		}

		~VertexMultiTextureDynamic()
		{

		}
	};

	/**
	 * Vertex structure for particles
	 */
	struct VertexParticles
	{
		D3DXVECTOR3		position;
		D3DXVECTOR3		velocity;
		D3DXVECTOR2		size;
		float			age;
		int				type;
		int				id;

		VertexParticles() : position(0.0f, 0.0f, 0.0f), velocity(0.0f, 0.0f, 0.0f), size(1.0f, 1.0f)
		{
			age		= 0.0f;
			type	= -1;
			id		= -1;
		}

		~VertexParticles()
		{
		}

		static const unsigned int NUM_ELEMENTS = 6;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Vertex with position, UV, color, texture ID and transform.
	 */
	struct VertexGUI
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F color;
		int arrayIndex;
		int textureID;
		D3DXMATRIX transform;

		VertexGUI() : position(0.0f, 0.0f, 0.0f, 1.0f), UV(0.0f, 0.0f),
			color(1.0f, 1.0f, 1.0f, 1.0f)
		{
			arrayIndex = 0;
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexGUI()
		{

		}

		static const unsigned int NUM_ELEMENTS = 9;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Instanced version of VertexGUI.
	 */
	struct VertexGUIInstance
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR2_16F UV;
		D3DXVECTOR4_16F color;
		int arrayIndex;
		int textureID;
		D3DXMATRIX transform;

		VertexGUIInstance() : position(0.0f, 0.0f, 0.0f, 1.0f), UV(0.0f, 0.0f),
			color(1.0f, 1.0f, 1.0f, 1.0f)
		{
			arrayIndex = 0;
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexGUIInstance()
		{

		}

		static const unsigned int NUM_ELEMENTS = 9;
		static D3D10_INPUT_ELEMENT_DESC *pInputDesc;
	};

	/**
	 * Static part of VertexGUI.
	 */
	struct VertexGUIStatic
	{
		D3DXVECTOR4_16F position;
		D3DXVECTOR2_16F UV;

		VertexGUIStatic() : position(0.0f, 0.0f, 0.0f, 1.0f), UV(0.0f, 0.0f)
		{

		}

		~VertexGUIStatic()
		{

		}
	};

	/**
	 * Dynamic part of VertexGUI.
	 */
	struct VertexGUIDynamic
	{
		D3DXVECTOR4_16F color;
		int arrayIndex;
		int textureID;
		D3DXMATRIX transform;

		VertexGUIDynamic() : color(1.0f, 1.0f, 1.0f, 1.0f)
		{
			arrayIndex = 0;
			textureID = -1;
			D3DXMatrixIdentity(&transform);
		}

		~VertexGUIDynamic()
		{

		}
	};
}

#endif