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

#include "Vertex.h"

using namespace gfx;

D3D10_INPUT_ELEMENT_DESC *VertexTransform::pInputDesc = NULL;
D3D10_INPUT_ELEMENT_DESC *VertexTransformInstance::pInputDesc = NULL;

void gfx::InitVertexStructures()
{
	//VertexTransform
	D3D10_INPUT_ELEMENT_DESC vertexTransform[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXID", 0, DXGI_FORMAT_R32_SINT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	VertexTransform::pInputDesc = myNew D3D10_INPUT_ELEMENT_DESC[VertexTransform::NUM_ELEMENTS];
	for(unsigned int i = 0; i < VertexTransform::NUM_ELEMENTS; ++i)
	{
		VertexTransform::pInputDesc[i] = vertexTransform[i];
	}

	//VertexTransformInstance
	D3D10_INPUT_ELEMENT_DESC vertexTransformInstance[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXID", 0, DXGI_FORMAT_R32_SINT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},
		{"TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},
		{"TRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},
		{"TRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},
		{"TRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}
	};

	VertexTransformInstance::pInputDesc = myNew D3D10_INPUT_ELEMENT_DESC[VertexTransformInstance::NUM_ELEMENTS];
	for(unsigned int i = 0; i < VertexTransformInstance::NUM_ELEMENTS; ++i)
	{
		VertexTransformInstance::pInputDesc[i] = vertexTransformInstance[i];
	}
}

void gfx::DestroyVertexStructures()
{
	if(VertexTransform::pInputDesc)
	{
		delete [] VertexTransform::pInputDesc;
		VertexTransform::pInputDesc = NULL;
	}
	if(VertexTransformInstance::pInputDesc)
	{
		delete [] VertexTransformInstance::pInputDesc;
		VertexTransformInstance::pInputDesc = NULL;
	}
}