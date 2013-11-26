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

Camera::Camera(const D3DXVECTOR3 &pos)
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
	PerspectiveFovLH(800.0f / 600.0f, (float)D3DX_PI * 0.25f, 0.5f, 10000.0f);
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
	D3DXMatrixLookAtLH(&mViewM, &mPosV, &mLookV, &mUpV);
}

void Camera::update()
{
	MatrixLookAtLH();	
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

void Camera::setPosition(float x, float y, float z)
{
	mPosV = D3DXVECTOR3(x, y, z);
}

void Camera::setPosition(const D3DXVECTOR3 &pos)
{
	mPosV = pos;
}

void Camera::setTarget(float x, float y, float z)
{
	mLookV = D3DXVECTOR3(x, y, z);
}

void Camera::setTarget(const D3DXVECTOR3 &target)
{
	mLookV = target;
}

void Camera::RotateX(float angle)
{
	D3DXMATRIX r;
	D3DXMatrixRotationAxis(&r, &mRightV, angle);
	D3DXVec3TransformCoord(&mLookV, &mLookV, &r);
	D3DXVec3TransformCoord(&mUpV, &mUpV, &r);
}

void Camera::RotateY(float angle)
{
	D3DXMATRIX r;

	D3DXMatrixRotationY(&r, angle);
	D3DXVec3TransformCoord(&mRightV, &mRightV, &r);
	D3DXVec3TransformCoord(&mUpV, &mUpV, &r);
	D3DXVec3TransformCoord(&mLookV, &mLookV, &r);
}

void Camera::RotateZ(float angle)
{
	D3DXMATRIX r;
	D3DXMatrixRotationAxis(&r, &mLookV, angle);
	D3DXVec3TransformCoord(&mUpV, &mUpV, &r);
	D3DXVec3TransformCoord(&mRightV, &mRightV, &r);
}

void Camera::GetVPMatrices(D3DXMATRIX &view, D3DXMATRIX &proj)
{
	view = mViewM;
	proj = mProjM;
}