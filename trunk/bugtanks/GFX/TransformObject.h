/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
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

#ifndef TRANSFORM_OBJECT_H
#define TRANSFORM_OBJECT_H
#include "RenderObject.h"

namespace gfx
{
	struct Material
	{
		/**
		 * Constructor
		 */
		Material()
		{
			index		= -1;
			diffuse		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			specular	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			ambient		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}

		/**
		 * Constructor
		 */
		Material(const Material &mtrl)
		{
			index		= mtrl.index;
			diffuse		= mtrl.diffuse;
			specular	= mtrl.specular;
			ambient		= mtrl.ambient;
		}

		const Material &operator=(const Material &mtrl)
		{
			if(this != &mtrl)
			{
				index		= mtrl.index;
				diffuse		= mtrl.diffuse;
				specular	= mtrl.specular;
				ambient		= mtrl.ambient;
			}

			return *this;
		}

		int			index;
		D3DXCOLOR	diffuse;
		D3DXCOLOR	specular;
		D3DXCOLOR	ambient;
	};

	/**
	 * Abstract class containing transformation functionality.
	 */
	class TransformObject : public RenderObject
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			TransformObject(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			TransformObject(const TransformObject &object);

			/**
			 * Default destructor.
			 */
			virtual ~TransformObject() = 0;

			/**
			 * Default assignment operator.
			 */
			const TransformObject &operator=(const TransformObject &object);

			/**
			 * Clones the object.
			 */
			virtual RenderObject *clone() = 0;

			/**
			 * Updates the object.
			 * Functionality varies depending on object type.
			 */
			virtual void update(float deltaTime) = 0;

			/**
			 * Renders the object.
			 * Method varies depending on object type.
			 */
			virtual bool render() = 0;

			/**
			 * Increases the scale vector.
			 * @param offset Vector to add to mScale.
			 */
			void increaseScale(const D3DXVECTOR3 &offset);

			/**
			 * Increases the X scale modifier.
			 * @param x Value to add to mScale.x.
			 */
			void increaseScaleX(float x);

			/**
			 * Increases the Y scale modifier.
			 * @param y Value to add to mScale.y.
			 */
			void increaseScaleY(float y);

			/**
			 * Increases the Z scale modifier.
			 * @param z Value to add to mScale.z.
			 */
			void increaseScaleZ(float z);

			/**
			 * Increases the rotation vector.
			 * @param offset Vector to add to mRotation.
			 */
			void increaseRotation(const D3DXVECTOR3 &offset);

			/**
			 * Increases the X axis rotation.
			 * @param x Value to add to mRotation.x.
			 */
			void increaseRotationX(float x);

			/**
			 * Increases the Y axis rotation.
			 * @param y Value to add to mRotation.y.
			 */
			void increaseRotationY(float y);

			/**
			 * Increases the Z axis rotation.
			 * @param z Value to add to mRotation.z.
			 */
			void increaseRotationZ(float z);

			/**
			 * Sets the scale vector.
			 * @param scale New scale vector.
			 */
			void setScale(const D3DXVECTOR3 &scale);

			/**
			 * Sets the X scale modifier.
			 * @param x New X scale modifier.
			 */
			void setScaleX(float x);

			/**
			 * Sets the Y scale modifier.
			 * @param y New Y scale modifier.
			 */
			void setScaleY(float y);

			/**
			 * Sets the Z scale modifier.
			 * @param z New Z scale modifier.
			 */
			void setScaleZ(float z);

			/**
			 * Sets the rotation vector.
			 * @param rotation New rotation vector.
			 */
			void setRotation(const D3DXVECTOR3 &rotation);

			/**
			 * Sets the X axis rotation.
			 * @param x New X axis rotation.
			 */
			void setRotationX(float x);

			/**
			 * Sets the Y axis rotation.
			 * @param y New Y axis rotation.
			 */
			void setRotationY(float y);

			/**
			 * Sets the Z axis rotation.
			 * @param z New Z axis rotation.
			 */
			void setRotationZ(float z);

			/**
			 * Retrieves the scale vector.
			 * @return Returns mScale.
			 */
			inline const D3DXVECTOR3 &getScale() const {return mScale;}

			/**
			 * Retrieves the X scale modifier.
			 * @return Returns mScale.x.
			 */
			inline float getScaleX() const {return mScale.x;}

			/**
			 * Retrieves the Y scale modifier.
			 * @return Returns mScale.y.
			 */
			inline float getScaleY() const {return mScale.y;}

			/**
			 * Retrieves the Z scale modifier.
			 * @return Returns mScale.z.
			 */
			inline float getScaleZ() const {return mScale.z;}

			/**
			 * Retrieves the rotation vector.
			 * @return Returns mRotation.
			 */
			inline const D3DXVECTOR3 &getRotation() const {return mRotation;}

			/**
			 * Retrieves the X axis rotation.
			 * @return Returns mRotation.x.
			 */
			inline float getRotationX() const {return mRotation.x;}

			/**
			 * Retrieves the Y axis rotation.
			 * @return Returns mRotation.y.
			 */
			inline float getRotationY() const {return mRotation.y;}

			/**
			 * Retrieves the Z axis rotation.
			 * @return Returns mRotation.z.
			 */
			inline float getRotationZ() const {return mRotation.z;}

			/**
			 * Retrieves the transformation matrix.
			 * @return Returns mMatrixTransform.
			 */
			inline const D3DXMATRIX &getMatrixTransform() const {return mMatrixTransform;}

			/**
			 * Retrieves the translation matrix.
			 * @return Returns mMatrixTranslate.
			 */
			inline const D3DXMATRIX &getMatrixTranslate() const {return mMatrixTranslate;}

			/**
			 * Retrieves the scale matrix.
			 * @return Returns mMatrixScale.
			 */
			inline const D3DXMATRIX &getMatrixScale() const {return mMatrixScale;}

			/**
			 * Retrieves the rotation matrix.
			 * @return Returns mMatrixRotate.
			 */
			inline const D3DXMATRIX &getMatrixRotate() const {return mMatrixRotate;}

		protected:
			/**
			 * Calculates the transformation matrix.
			 */
			void updateMatrixTransform();

			/**
			 * Calculates the translation matrix.
			 */
			void updateMatrixTranslate();

			/**
			 * Calculates the scale matrix.
			 */
			void updateMatrixScale();

			/**
			 * Calculates the rotation matrix.
			 */
			void updateMatrixRotate();

			/**
			 * Sets the transformation matrix.
			 * @param transform New transformation matrix.
			 */
			inline void setMatrixTransform(const D3DXMATRIX &transform){mMatrixTransform = transform;}

			/**
			 * Sets the translation matrix.
			 * @param translate New translation matrix.
			 */
			inline void setMatrixTranslate(const D3DXMATRIX &translate){mMatrixTranslate = translate;}

			/**
			 * Sets the scale matrix.
			 * @param scale New scale matrix.
			 */
			inline void setMatrixScale(const D3DXMATRIX &scale){mMatrixScale = scale;}

			/**
			 * Sets the rotation matrix.
			 * @param rotate New rotation matrix.
			 */
			inline void setMatrixRotate(const D3DXMATRIX &rotate){mMatrixRotate = rotate;}

		private:
			D3DXVECTOR3 mScale;
			D3DXVECTOR3 mRotation;
			D3DXMATRIX mMatrixTransform;
			D3DXMATRIX mMatrixTranslate;
			D3DXMATRIX mMatrixScale;
			D3DXMATRIX mMatrixRotate;
			Material   mMaterial;
	};
}

#endif