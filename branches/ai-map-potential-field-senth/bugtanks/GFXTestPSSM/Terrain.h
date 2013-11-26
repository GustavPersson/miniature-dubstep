//#pragma once
//
//#include "../GFX/DX10Module.h"
//#include "Material.h"
//
//#include <vector>
//#include <fstream>
//#include <iostream>
//
//using namespace std;
//
//class Terrain
//{
//private:
//	ID3D10Device* gDevice;
//
//	D3DXMATRIX	  mWorldM;
//	D3DXMATRIX	  mViewM;
//	D3DXMATRIX	  mProjM;
//
//	D3DXMATRIX	  mScaleM;
//	D3DXMATRIX	  mTranslationM;
//	D3DXMATRIX	  mRotXM;
//	D3DXMATRIX	  mRotYM;
//	D3DXMATRIX	  mRotZM;
//
//	D3DXVECTOR3	  mPosV;
//
//	Material	  mMtrl;
//
//	bool		  mRebuildWorldMatrix;
//
//	// Storlek
//	UINT  numVertCol;				// Antal kolumner
//	UINT  numVertRow;				// och rader (storleken på terrängen)
//	UINT  mCellSize;				// Storleken på en cell
//	float mHeightScale;				// Skalningsfaktor
//	std::vector<float> mHeightMap;	// Spara heightmappen i vektorn
//
//	ID3DX10Mesh* mMesh;
//
//	// Recourse variables
//	ID3D10EffectMatrixVariable* fxWVP;
//	ID3D10EffectMatrixVariable* fxWorldM;
//	ID3D10EffectVariable*		fxLight;
//	ID3D10EffectVariable*		fxMtrl;
//	ID3D10EffectVariable*		fxEyePos;
//	ID3D10Effect*				mfxRenderEffect;
//
//	ID3D10EffectShaderResourceVariable* mfxBlendT;
//	ID3D10EffectShaderResourceVariable* mfxDirtT;
//	ID3D10EffectShaderResourceVariable* mfxGrassT;
//	ID3D10EffectShaderResourceVariable* mfxSandT;
//
//	// Textures
//	ID3D10ShaderResourceView* mBlendRV;
//	ID3D10ShaderResourceView* mDirtRV;
//	ID3D10ShaderResourceView* mGrassRV;
//	ID3D10ShaderResourceView* mSandRV;
//
//	// Shader binds
//	ID3D10EffectTechnique*	mfxRenderTechnique;
//	ID3D10InputLayout*		mInputLayout;
//
//	void CreateFX();
//
//public:
//	Terrain(ID3D10Device* device, const UINT cols, const UINT rows, const UINT cellSize, const float heightScale);
//	~Terrain();
//
//	void Update(D3DXVECTOR3 &eyePos, D3DXMATRIX &mViewM, const D3DXMATRIX &mProjM, Light light);
//	void Render();
//
//	void LoadHeightmap();
//
//	float getHeight(UINT x, UINT z);
//	bool inBounds(UINT x, UINT z);
//};