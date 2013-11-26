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

#ifndef CAMERA_H
#define CAMERA_H
#include <d3d10.h>
#include <d3dx10.h>

namespace gfx
{
	/**
	 * Class for handling camera movement and matrix calculation.
	 */
	class Camera
	{
		public:
			/**
			 * Constructor,
			 * @param pos Camera position,
			 */
			Camera(long width, long height,const D3DXVECTOR3 &pos);

			/**
			 * Destructor.
			 */
			virtual ~Camera();

			/**
			 * Initiates projection matrix
			 * @param aspect        Aspect ratio
			 * @param fov           Field of view
			 * @param nz            Near plane
			 * @param fz            Far plane
			 */
			void PerspectiveFovLH( const float aspect	= 800.0f / 600.0f,
								   const float fov		= (float)D3DX_PI * 0.25f, 
								   const float nz		= 0.5f, 
								   const float fz		= 10000.0f );

			/**
			 * Initiates projection matrix
			 */
			void MatrixLookAtLH();

			/**
			 * Updates view matrix
			 */
			void update();

			/**
			 * Updates view matrix
			 */
			void move(const D3DXVECTOR3 &dv);

			/**
			 * Increases the position vector.
			 * @param x X value to add.
			 * @param y Y value to add.
			 * @param z Z value to add.
			 */
			inline void increasePosition(float x, float y, float z){mPosV += D3DXVECTOR3(x, y, z);}

			/**
			 * Increases the position vector.
			 * @param pos Vector to add.
			 */
			inline void increasePosition(const D3DXVECTOR3 &pos){mPosV += pos;}

			/**
			 * Increases the target/look vector.
			 * @param x X value to add.
			 * @param y Y value to add.
			 * @param z Z value to add.
			 */
			inline void increaseTarget(float x, float y, float z){mLookV += D3DXVECTOR3(x, y, z);}

			/**
			 * Increases the target/look vector.
			 * @param target Vector to add.
			 */
			inline void increaseTarget(const D3DXVECTOR3 &target){mLookV += target;}

			/**
			 * Sets camera position (by floats)
			 * @param x             x
			 * @param y             y
			 * @param z             z
			 */
			void setPosition(float x, float y, float z);

			/**
			 * Sets camera position (by vector)
			 * @param pos Vector with camera positions
			 */
			void setPosition(const D3DXVECTOR3 &pos);

			/**
			 * Sets camera target (by floats)
			 * @param x             x
			 * @param y             y
			 * @param z             z
			 */
			void setTarget(float x, float y, float z);

			/**
			 * Sets camera target (by vector)
			 * @param target Vector with camera target
			 */
			void setTarget(const D3DXVECTOR3 &target);

			/**
			 * Rotates around x-axis
			 * @param angle         Angle to rotate
			 */
			void RotateX(float angle);

			/**
			 * Rotates around y-axis
			 * @param angle         Angle to rotate
			 */
			void RotateY(float angle);

			/**
			 * Rotates around y-axis
			 * @param angle         Angle to rotate
			 */
			void RotateZ(float angle);

			/**
			 * Sets view and projection matrices
			 * @param view  View matrix
			 * @param proj  Projection matrix
			 * @param ortProj Orthogonal projection matrix.
			 */
			void GetVPMatrices(D3DXMATRIX& view, D3DXMATRIX& proj, D3DXMATRIX &ortProj);
			
			/**
			 * Returns camera position
			 */
			inline const D3DXVECTOR3 &getPosition() const {return mPosV;}

			/**
			 * Returns camera target
			 */
			inline const D3DXVECTOR3 &getTarget() const {return mLookV;}

		private:
			D3DXMATRIX mViewM;
			D3DXMATRIX mProjM;
			D3DXMATRIX mOrtProjM;

			D3DXVECTOR3 mPosV;		// Cameras position
			D3DXVECTOR3 mRightV;	// x-axis
			D3DXVECTOR3 mUpV;		// y-axis
			D3DXVECTOR3 mLookV;		// z-axis
	};

}

#endif