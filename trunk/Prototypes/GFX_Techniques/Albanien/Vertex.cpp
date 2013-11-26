#include "Vertex.h"

D3D10_INPUT_ELEMENT_DESC* VertexPosTex::inputDesc			= 0;
D3D10_INPUT_ELEMENT_DESC* VertexPosTexNorm::inputDesc		= 0;
D3D10_INPUT_ELEMENT_DESC* VertexPosTexNormMat::inputDesc	= 0;
D3D10_INPUT_ELEMENT_DESC* VertexPosTanNorTex::inputDesc		= 0;

void InitVertexStructures()
{
	// VertexPos
	D3D10_INPUT_ELEMENT_DESC vertexPosTex[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	// VertexPosTexNorm
	D3D10_INPUT_ELEMENT_DESC vertexPosTexNormMat[] = 
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,  D3D10_INPUT_PER_VERTEX_DATA,		0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D10_INPUT_PER_VERTEX_DATA,		0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D10_INPUT_PER_VERTEX_DATA,		0 },
		{ "mTransform",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 0,  D3D10_INPUT_PER_INSTANCE_DATA,	1 },
		{ "mTransform",	1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 16, D3D10_INPUT_PER_INSTANCE_DATA,	1 },
		{ "mTransform",	2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 32, D3D10_INPUT_PER_INSTANCE_DATA,	1 },
		{ "mTransform",	3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 48, D3D10_INPUT_PER_INSTANCE_DATA,	1 },
	};

	VertexPosTexNormMat::inputDesc = new D3D10_INPUT_ELEMENT_DESC[VertexPosTexNormMat::NUM_ELEMENTS];
	for(UINT i = 0; i < VertexPosTexNormMat::NUM_ELEMENTS; ++i)
	{
		VertexPosTexNormMat::inputDesc[i] = vertexPosTexNormMat[i];
	}

	// VertexPosTexNorm
	D3D10_INPUT_ELEMENT_DESC vertexPosTexNorm[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	VertexPosTexNorm::inputDesc = new D3D10_INPUT_ELEMENT_DESC[VertexPosTexNorm::NUM_ELEMENTS];
	for(UINT i = 0; i < VertexPosTexNorm::NUM_ELEMENTS; ++i)
	{
		VertexPosTexNorm::inputDesc[i] = vertexPosTexNorm[i];
	}
	
	D3D10_INPUT_ELEMENT_DESC vertexPosTanNorTex[] =
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	VertexPosTanNorTex::inputDesc = new D3D10_INPUT_ELEMENT_DESC[VertexPosTanNorTex::NUM_ELEMENTS];
	for(UINT i = 0; i < VertexPosTanNorTex::NUM_ELEMENTS; ++i)
		VertexPosTanNorTex::inputDesc[i] = vertexPosTanNorTex[i];
}

void DestroyVertexStructures()
{
	delete [] VertexPosTex::inputDesc;
	delete [] VertexPosTexNorm::inputDesc;
	delete [] VertexPosTanNorTex::inputDesc;
}