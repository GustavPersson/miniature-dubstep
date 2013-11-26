#pragma once

#include <d3dx10.h>

void InitVertexStructures();
void DestroyVertexStructures();

struct VertexPosTex
{
	VertexPosTex() : pos( 0.0f, 0.0f, 0.0f ), texC(0.0f, 0.0f) {}
	VertexPosTex(const D3DXVECTOR3& p, const D3DXVECTOR3& t) : pos(p), texC(t) {} 
	VertexPosTex(const float x,  const float y,  const float z, const float tx, const float ty) : pos(x, y, z), texC(tx, ty) {}

	D3DXVECTOR3 pos;
	D3DXVECTOR2 texC;

	static const UINT NUM_ELEMENTS = 2;
	static D3D10_INPUT_ELEMENT_DESC* inputDesc;
};

/**
*	VertexPosTexNor struct containing the position, texture coord and normal.
*/
struct VertexPosTexNorm
{
	VertexPosTexNorm() : pos( 0.0f, 0.0f, 0.0f ), tex( 0.0f, 0.0f ), norm( 0.0f, 0.0f, 0.0f ) {}
	VertexPosTexNorm( const D3DXVECTOR3& p, const D3DXVECTOR3& n, const D3DXVECTOR2& t ) : pos( p ), tex( t ), norm( n ) {} 
	VertexPosTexNorm( const float x,  const float y,  const float z, 
					  const float nx, const float ny, const float nz, 
					  const float u,  const float v ) : pos(x, y, z), tex(u, v), norm(nx, ny, nz) {}

	// Elements
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
	D3DXVECTOR2 tex;

	static const UINT				 NUM_ELEMENTS = 3;
	static D3D10_INPUT_ELEMENT_DESC* inputDesc;
};

struct VertexPosTanNorTex
{
	VertexPosTanNorTex() : pos( 0.0f, 0.0f, 0.0f ), tan( 0.0f, 0.0f, 0.0f ), norm( 0.0f, 0.0f, 0.0f ), tex( 0.0f, 0.0f ) {}
	VertexPosTanNorTex( const D3DXVECTOR3& p, const D3DXVECTOR3& t2, const D3DXVECTOR3& n, const D3DXVECTOR2& t ) : pos( p ), tan( t2 ), norm( n ), tex( t ) {} 
	VertexPosTanNorTex( const float x,  const float y,  const float z, 
					  const float tx, const float ty, const float tz, 
					  const float nx, const float ny, const float nz, 
					  const float u,  const float v ) : pos(x, y, z), tan(tx, ty, tz), norm(nx, ny, nz), tex(u, v) {}

	// Elements
	D3DXVECTOR3 pos;
	D3DXVECTOR3 tan;
	D3DXVECTOR3 norm;
	D3DXVECTOR2 tex;

	static const UINT				 NUM_ELEMENTS = 4;
	static D3D10_INPUT_ELEMENT_DESC* inputDesc;
};

struct VertexPosTexNormMat
{

	VertexPosTexNormMat() : pos( 0.0f, 0.0f, 0.0f ), norm( 0.0f, 0.0f, 0.0f ), tex( 0.0f, 0.0f ) {}
	VertexPosTexNormMat( const D3DXVECTOR3& p, const D3DXVECTOR3& n, const D3DXVECTOR2& t ) : pos( p ), norm( n ), tex( t ) {} 
	VertexPosTexNormMat( const float x,  const float y,  const float z, 
					  const float nx, const float ny, const float nz, 
					  const float u,  const float v,
					  D3DXMATRIX m0, D3DXMATRIX m1, D3DXMATRIX m2, D3DXMATRIX m3) : pos(x, y, z), norm(nx, ny, nz), tex(u, v), mat0(m0), mat1(m1), mat2(m2), mat3(m3) {}

	// Elements
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
	D3DXVECTOR2 tex;
	D3DXMATRIX mat0;
	D3DXMATRIX mat1;
	D3DXMATRIX mat2;
	D3DXMATRIX mat3;

	static const UINT					NUM_ELEMENTS = 7;
	static D3D10_INPUT_ELEMENT_DESC*	inputDesc;
};