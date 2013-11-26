#include "Camera.h"


Camera::Camera(D3DXVECTOR3 &pos, D3DXVECTOR3 &dir, D3DXVECTOR3 &up)
{
	// Init view matrix
	D3DXVECTOR3 look = dir - pos;		// Calculate the z-axis
	D3DXVec3Normalize(&look, &look);

	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &up, &look);	// Calculate the x-axis
	D3DXVec3Normalize(&right, &right);

	D3DXVECTOR3 u;
	D3DXVec3Cross(&u, &look, &right);	// Calculate the y-axis
	D3DXVec3Normalize(&u, &u);

	mPosV   = pos;
	mRightV = right;
	mUpV    = u;
	mLookV  = look;

	BuildView();

	// Init proj matrix
	PerspectiveFovLH();
}

Camera::~Camera()
{
}

void Camera::PerspectiveFovLH(float aspect, float fov, float nz, float fz)
{
	D3DXMatrixPerspectiveFovLH(&mProjM, fov, aspect, nz, 10000.0f);
}

void Camera::BuildView()
{
	D3DXVec3Normalize(&mLookV, &mLookV);
	
	D3DXVec3Cross(&mUpV, &mLookV, &mRightV);
	D3DXVec3Normalize(&mUpV, &mUpV);

	D3DXVec3Cross(&mRightV, &mUpV, &mLookV);
	D3DXVec3Normalize(&mRightV, &mRightV);

	float x = -D3DXVec3Dot(&mPosV, &mRightV);
	float y = -D3DXVec3Dot(&mPosV, &mUpV);
	float z = -D3DXVec3Dot(&mPosV, &mLookV);

	mViewM(0,0) = mRightV.x; 
	mViewM(1,0) = mRightV.y; 
	mViewM(2,0) = mRightV.z; 
	mViewM(3,0) = x;   

	mViewM(0,1) = mUpV.x;
	mViewM(1,1) = mUpV.y;
	mViewM(2,1) = mUpV.z;
	mViewM(3,1) = y;  

	mViewM(0,2) = mLookV.x; 
	mViewM(1,2) = mLookV.y; 
	mViewM(2,2) = mLookV.z; 
	mViewM(3,2) = z;   

	mViewM(0,3) = 0.0f;
	mViewM(1,3) = 0.0f;
	mViewM(2,3) = 0.0f;
	mViewM(3,3) = 1.0f;
}

void Camera::GetVP(D3DXMATRIX &view, D3DXMATRIX &proj)
{
	view = mViewM;
	proj = mProjM;
}

void Camera::update()
{
	BuildView();	
}

void Camera::Move(D3DXVECTOR3 &dv)
{
	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
	dir += (mLookV * dv.z);
	dir += (mRightV * dv.x);

	D3DXVec3Normalize(&dir, &dir);
	mPosV += dir; // * 2 för snabbare förflyttning


	BuildView();
}

void Camera::RotateX(float angle)
{
	D3DXMATRIX r;
	D3DXMatrixRotationAxis(&r, &mRightV, angle);
	D3DXVec3TransformCoord(&mLookV, &mLookV, &r);
	D3DXVec3TransformCoord(&mUpV, &mUpV, &r);

	BuildView();
}

void Camera::RotateY(float angle)
{
	D3DXMATRIX r;
	/*D3DXMatrixRotationAxis(&r, &mUpV, angle);
	D3DXVec3TransformCoord(&mLookV, &mLookV, &r);
	D3DXVec3TransformCoord(&mRightV, &mRightV, &r);*/

	D3DXMatrixRotationY(&r, angle);
	D3DXVec3TransformCoord(&mRightV, &mRightV, &r);
	D3DXVec3TransformCoord(&mUpV, &mUpV, &r);
	D3DXVec3TransformCoord(&mLookV, &mLookV, &r);
	// Förklara varför koden inte ger fisheye effekt, medan MatrixRotationAxis gör

	BuildView();
}

void Camera::RotateZ(float angle)
{
	D3DXMATRIX r;
	D3DXMatrixRotationAxis(&r, &mLookV, angle);
	D3DXVec3TransformCoord(&mUpV, &mUpV, &r);
	D3DXVec3TransformCoord(&mRightV, &mRightV, &r);

	BuildView();
}

D3DXVECTOR3& Camera::getEyePos()
{
	return mPosV;
}

void Camera::setPosition(float x, float y, float z)
{
	mPosV = D3DXVECTOR3(x, y, z);
}

bool Camera::IsVisible(Cube* cube)
{
	D3DXVECTOR3 tMin, tMax;
	
	for(int i = 0;i < 6;i++)
	{
		D3DXVECTOR3 vec(mFrustumPlanes[i].a, mFrustumPlanes[i].b, mFrustumPlanes[i].c);

		if(mFrustumPlanes[i].a > 0)
		{
			tMin.x = cube->vMin.x;
			tMax.x = cube->vMax.x;
		}
		else
		{
			tMin.x = cube->vMax.x;
			tMax.x = cube->vMin.x;
		}
		if(mFrustumPlanes[i].b > 0)
		{
			tMin.y = cube->vMin.y;
			tMax.y = cube->vMax.y;
		}
		else
		{
			tMin.y = cube->vMax.y;
			tMax.y = cube->vMin.y;
		}
		if(mFrustumPlanes[i].c > 0)
		{
			tMin.z = cube->vMin.z;
			tMax.z = cube->vMax.z;
		}
		else
		{
			tMin.z = cube->vMax.z;
			tMax.z = cube->vMin.z;
		}

		if(D3DXVec3Dot(&vec, &tMin) + mFrustumPlanes[i].d > 0)
			return false;
	}

	return true;
}