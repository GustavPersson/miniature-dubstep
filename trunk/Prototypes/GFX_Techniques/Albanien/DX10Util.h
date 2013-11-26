#pragma once
#include <d3dx10.h>
#include <dxerr.h>

#define INFINITY 100000000

#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

D3DX10INLINE float RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

D3DX10INLINE float RandF(float a, float b)
{
	return a + RandF()*(b-a);
}