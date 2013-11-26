#pragma once

#include "DX10Util.h"
#include "Vertex.h"
#include "Light.h"
#include "ShadowMap.h"
#include <iostream>

//Forward decl
HRESULT CreateRandomMatrices();

#define MAX_INST 20
#define MAX_INT	 20

class Cube
{
private:
	ID3D10Device* gDevice;

	ID3D10Buffer* mVB[2];	// 2 VB, en för norma, tex och positions-data och en för transformationsmatriser
	ID3D10Buffer* mIB;		// Endast en IB behövs

	D3DXMATRIX	  mWorldM;
	D3DXMATRIX	  mViewM;
	D3DXMATRIX	  mProjM;
	D3DXMATRIX	  mScaleM;

	D3DXMATRIX	  mTranslationM;
	D3DXMATRIX	  mRotXM;
	D3DXMATRIX	  mRotYM;
	D3DXMATRIX	  mRotZM;

	D3DXVECTOR3	  mPosV;
	Material	  mMtrl;

	float		  mRotX;
	float		  mRotY;
	float		  mRotZ;
	float		  mScale;

	int			  mNumVertices;

	int			  mNumIndices;

	bool		  mRebuildWorldMatrix;

	//Shader variables
	ID3D10EffectShaderResourceVariable* mFXTextureVariable;
	ID3D10EffectShaderResourceVariable* mFxTextureN;
	ID3D10EffectShaderResourceVariable* mfxShadowT;
	ID3D10EffectMatrixVariable*			mFXWVPM; 
	ID3D10EffectMatrixVariable*			mFxWorldM;
	ID3D10EffectMatrixVariable*			fxLWVP;
	ID3D10ShaderResourceView*			mTextureRV;
	ID3D10ShaderResourceView*			mTextureNRV;
	ID3D10EffectTechnique*				mRenderTechnique;
	ID3D10EffectTechnique*				mShadowTechnique;		// shadow
	ID3D10InputLayout*					mInputLayout;
	ID3D10InputLayout*					mInputLayout2;
	ID3D10Texture2D*					mTexture;
	ID3D10Effect*						mRenderEffect;

	ID3D10EffectVariable*		fxLight;
	ID3D10EffectVariable*		fxMtrl;
	ID3D10EffectVariable*		fxEyePos;

	// Shadow map handle
	ShadowMap* shadowMap;

	/**
	*	Loading Default.fx and create the effect
	*/
	void CreateFX();

public:
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;
	/**
	*	Constructor.
	*	@param width. Box width. Default 5.0f.
	*/
	Cube(int nrOfInstances, const float width = 5.0f, ID3D10Device* device = NULL, const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	/**
	*	Destructor
	*/
	~Cube();

	/**
	*	Render the vertex buffer.
	*/
	void Render();

	/**
	*	Render the vertex buffer.
	*/
	void RenderShadow();

	void bindShadow(ID3D10ShaderResourceView* texture);

	/**
	*	Scale. Scale the Cube in x, y and z with the param.
	*	@param scale. Factor to scale with.
	*/
	void Scale(const float scale);

	/**
	*	Update.
	*	@param dt. Delta time.
	*	@param view. Camera view matrix.
	*	@param proj. Camera proj matrix.
	*/
	void Update(const D3DXMATRIX& view, const D3DXMATRIX& proj, const D3DXMATRIX &lview, const D3DXMATRIX &lproj, D3DXVECTOR3& eyePos, Light light);

	/**
	*	Move.
	*	@param x. Steps in x-axis.
	*	@param y. Steps in y-axis.
	*	@param z. Steps in z-axis.
	*/
	void Move(const float x, const float y, const float z);

	/**
	*	Rotate the Cube around X with the param value.
	*	@param angle. Rotation angle
	*/
	void RotateX(const float angle);

	/**
	*	Rotate the Cube around Y with the param value.
	*	@param angle. Rotation angle
	*/
	void RotateY(const float angle);

	/**
	*	Rotate the Cube around Z with the param value.
	*	@param angle. Rotation angle
	*/
	void RotateZ(const float angle);

	void updateTexture(ID3D10ShaderResourceView *tex);
};

