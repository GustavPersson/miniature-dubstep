#pragma once

#include "DX10App.h"
#include "Cube.h"

class Camera
{
public:
	D3DXMATRIX mViewM;
	D3DXMATRIX mProjM;

	D3DXVECTOR3 mPosV;		// Cameras position
	D3DXVECTOR3 mRightV;	// x-axis
	D3DXVECTOR3 mUpV;		// y-axis
	D3DXVECTOR3 mLookV;		// z-axis

	D3DXPLANE mFrustumPlanes[6];

	void BuildView();		// Builds the view matrix (same as D3DXMatrixLookAtLH(..))

	Camera(D3DXVECTOR3& pos, D3DXVECTOR3& dir, D3DXVECTOR3& up);
	~Camera();

	void PerspectiveFovLH( const float aspect	= 800.0f / 600.0f,
						   const float fov		= (float)D3DX_PI * 0.25f, 
						   const float nz		= 1.0f, 
						   const float fz		= 10000.0f );

	void Move(D3DXVECTOR3& dv);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);

	void GetVP(D3DXMATRIX& view, D3DXMATRIX& proj);	// Returns view and projection matrises
	D3DXVECTOR3& getEyePos();

	void update();

	void setPosition(float x, float y, float z);

	bool IsVisible(Cube* cube);
};