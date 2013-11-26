/**
 * @file
 * @author Unknown?
 * @version 1.0
 * Copyright (©) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "Camera.h"

using namespace gfx;

Camera::Camera(long width, long height, const D3DXVECTOR3 &pos)
{
	// Temporary variables
	D3DXVECTOR3 dir	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXVECTOR3 look = dir - pos;
	D3DXVec3Normalize(&look, &look);

	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &up, &look);
	D3DXVec3Normalize(&right, &right);

	D3DXVECTOR3 u;
	D3DXVec3Cross(&u, &look, &right);
	D3DXVec3Normalize(&u, &u);

	mPosV   = pos;
	mRightV = right;
	mUpV    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	mLookV  = look;
	
	MatrixLookAtLH();
	PerspectiveFovLH((float)(width / height), (float)D3DX_PI * 0.25f, 0.5f, 10000.0f);
	D3DXMatrixOrthoLH(&mOrtProjM, static_cast<float>(width), static_cast<float>(height), 0.0f, 10000.0f);

	mRotation = 0.0f;
	mRotationY = 0.0f;
	mRotationZ = 0.0f;
	mRotSpeed = 0.0f;

	mCamOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	mRotating = false;
	mDirection = 0;
}

Camera::~Camera()
{
}

void Camera::PerspectiveFovLH(float aspect, float fov, float nz, float fz)
{
	D3DXMatrixPerspectiveFovLH(&mProjM, fov, aspect, nz, fz);
}

void Camera::MatrixLookAtLH()
{	
	D3DXVECTOR3 result = mTarget - mPosV;
	
	D3DXVECTOR3 zAxis;
	D3DXVec3Normalize(&zAxis, &result);
	D3DXVECTOR3 xAxis;
	D3DXVec3Cross(&result, &mUpV, &zAxis);
	D3DXVec3Normalize(&xAxis, &result);
	D3DXVECTOR3 yAxis;
	D3DXVec3Cross(&yAxis, &zAxis, &xAxis);

	mViewM._11 = xAxis.x; mViewM._12 = yAxis.x; mViewM._13 = zAxis.x; mViewM._14 = 0;
	mViewM._21 = xAxis.y; mViewM._22 = yAxis.y; mViewM._23 = zAxis.y; mViewM._24 = 0;
	mViewM._31 = xAxis.z; mViewM._32 = yAxis.z; mViewM._33 = zAxis.z; mViewM._34 = 0;
	
	mViewM._41 = -D3DXVec3Dot(&xAxis, &mPosV); 
	mViewM._42 = -D3DXVec3Dot(&yAxis, &mPosV); 
	mViewM._43 = -D3DXVec3Dot(&zAxis, &mPosV); 
	mViewM._44 = 1;

	mRotation = 0.0f;
}

void Camera::update()
{
	MatrixLookAtLH();	
	if(mRotating)
	{
		RotateX(static_cast<float>(mDirection));
	}
}

void Camera::move(const D3DXVECTOR3 &dv)
{
	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
	dir += (mLookV * dv.z);
	dir += (mRightV * dv.x);

	D3DXVec3Normalize(&dir, &dir);
	mPosV += dir * 3; // * 2 för snabbare förflyttning

	MatrixLookAtLH();
}

void Camera::setOffset(float x, float y, float z)
{
	mCamOffset = D3DXVECTOR3(x, y, z);
}

void Camera::setOffset(const D3DXVECTOR3 &pos)
{
	mCamOffset = pos;
}

void Camera::setPosition(float x, float y, float z)
{	
	mTarget = D3DXVECTOR3(x, y, z);

	D3DXQUATERNION t;
	D3DXMATRIX r;
	D3DXQuaternionRotationYawPitchRoll(&t, mRotSpeed*mRotation, 0, 0);
	
	D3DXMatrixRotationQuaternion(&r, &t);
	D3DXVec3TransformCoord(&mCamOffset, &mCamOffset, &r);

	mPosV = mTarget + mCamOffset;
}

void Camera::setPosition(const D3DXVECTOR3 &pos)
{
	mPosV = pos;
}

void Camera::setRotateSpeed(float rotateSpeed)
{
	mRotSpeed = rotateSpeed;
}

void Camera::setTarget(float x, float y, float z)
{
	mTarget = D3DXVECTOR3(x, y, z);

	mPosV = mTarget + mCamOffset;
}

void Camera::setTarget(const D3DXVECTOR3 &target)
{
	mLookV = target;
}

void Camera::RotateX(float angle)
{
	mRotation = angle;
}

void Camera::RotateY(float angle)
{
	mRotationY = angle;
}

void Camera::RotateZ(float angle)
{
	mRotationZ = angle;
}

void Camera::GetVPMatrices(D3DXMATRIX &view, D3DXMATRIX &proj, D3DXMATRIX &ortProj)
{
	view = mViewM;
	proj = mProjM;
	ortProj = mOrtProjM;
}

void Camera::rotationDirection(int direction)
{
	mDirection += direction;
	if(mDirection == 0)
	{
		mRotating = false;
	}
	else
	{
		mRotating = true;
	}
}