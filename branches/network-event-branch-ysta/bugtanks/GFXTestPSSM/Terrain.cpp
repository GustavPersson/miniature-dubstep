//#include "Terrain.h"
//
//void Terrain::Update(D3DXVECTOR3 &eyePos, D3DXMATRIX &mViewM, const D3DXMATRIX &mProjM, Light light)
//{
//	if(mRebuildWorldMatrix)
//	{
//		mWorldM				= mScaleM * (mRotXM * mRotYM * mRotZM) * mTranslationM;
//		mRebuildWorldMatrix = false;
//	}
//
//	fxWVP		->SetMatrix((float*)&(mWorldM * mViewM * mProjM));
//	fxWorldM	->SetMatrix((float*)&mWorldM);
//	fxLight		->SetRawValue(&light, 0, sizeof(light));
//	fxMtrl		->SetRawValue(&mMtrl, 0, sizeof(mMtrl));
//	fxEyePos	->SetRawValue(&eyePos, 0, sizeof(D3DXVECTOR3));
//
//	mfxBlendT	->SetResource(mBlendRV);
//	mfxDirtT	->SetResource(mDirtRV);
//	mfxGrassT	->SetResource(mGrassRV);
//	mfxSandT	->SetResource(mSandRV);
//}
//
//void Terrain::Render()
//{
//	gDevice->IASetInputLayout(mInputLayout);
//
//	D3D10_TECHNIQUE_DESC techDesc;
//	mfxRenderTechnique->GetDesc(&techDesc);
//	for(UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		mfxRenderTechnique->GetPassByIndex(p)->Apply(0);
//		mMesh->DrawSubset(0);
//	}
//}
//
//float Terrain::getHeight(UINT x, UINT z)
//{	
//	float avg = 0.0f;
//	float num = 0.0f;
//
//	if(inBounds(x, z))
//	{
//		for(UINT m = z-1; m <= z+1; ++m)
//		{
//			for(UINT n = x-1; n <= x+1; ++n)
//			{
//				if(inBounds(x,z))
//				{
//					avg += mHeightMap[m*numVertCol + n];
//					num += 1.0f;
//				}
//			}
//		}
//	}
//
//	return (avg / num);
//}
//
//bool Terrain::inBounds(UINT z, UINT x)
//{
//	return x >= 0 && x < numVertRow && z >= 0 && z < numVertCol;
//}