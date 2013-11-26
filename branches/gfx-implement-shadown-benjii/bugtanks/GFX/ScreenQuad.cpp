#include "ScreenQuad.h"
#include <iostream>

ScreenQuad::ScreenQuad(ID3D10Device* device)
{
	mpDevice		= device;
	mNumVertices	= 4;

	createFX();
	fxTechnique = fxEffect->GetTechniqueByName("RenderQuad");
	mTexture	= fxEffect->GetVariableByName("mTexture")->AsShaderResource();
}

ScreenQuad::~ScreenQuad()
{
}

void ScreenQuad::createFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
  
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	D3DX10CreateEffectFromFile("../../Shaders/BasicQuad.fx", 
							NULL, 
							NULL, 
							"fx_4_0",
							D3D10_SHADER_ENABLE_STRICTNESS, 
							0, 
							mpDevice, 
							NULL, 
							NULL, 
							&fxEffect, 
							&compilationErrors,
							&hr);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			SAFE_RELEASE(compilationErrors);
		}

	}
}

void ScreenQuad::bindTexture(ID3D10ShaderResourceView* texture)
{
	mTexture->SetResource(texture);
}

void ScreenQuad::render()
{
	mpDevice->IASetInputLayout(NULL);
	mpDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mpDevice->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	
	fxTechnique->GetPassByIndex(0)->Apply(0);
	mpDevice->Draw(mNumVertices, 0);
}