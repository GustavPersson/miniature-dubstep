#include "ShadowMap.h"

ShadowMap::ShadowMap(ID3D10Device* device, int width, int height)
:Texture2D(device, width, height)
{
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::createFX(ID3D10Effect* renderEffect)
{
	mpRenderTechnique	= renderEffect->GetTechniqueByName("RenderShadowMap");
	fxLightWVPM			= renderEffect->GetVariableByName("fxLightWVPM")->AsMatrix();
	fxDepthMap			= renderEffect->GetVariableByName("fxDepthMap")->AsShaderResource();
}

ID3D10EffectTechnique* ShadowMap::getRenderTechnique()
{
	return mpRenderTechnique;
}

void ShadowMap::setLightPosition(Light light)
{
	D3DXMatrixPerspectiveFovLH(&mLightProjM, (float)D3DX_PI * 0.5f, 1.0f, 0.0f, 1024.0f);
	D3DXMatrixLookAtLH(&mLightViewM, &light.pos, &D3DXVECTOR3(256.0f, 0.0f, 256.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	
	fxLightWVPM->SetMatrix((float*)&(mLightProjM * mLightViewM));
}

void ShadowMap::setDepthMap(ID3D10ShaderResourceView* depthMap)
{
	fxDepthMap->SetResource(depthMap);
}