//#ifndef MESH_H
//#define MESH_H
//
//#include "../GFX/Common.h"
//#include "../GFX/Vertex.h"
//
//class Mesh: public TransformObject
//{
//public:
//	Terrain(ID3D10Device* device);
//	~Terrain();
//
//	void createFX();
//	void Terrain::LoadHeightmap();
//
//	float Terrain::getHeight(UINT x, UINT z);
//	bool Terrain::inBounds(UINT z, UINT x);
//
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
//	Material	  mMtrl;
//	bool		  mRebuildWorldMatrix;
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
//};
//
//#endif