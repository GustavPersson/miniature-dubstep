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

#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H
#include <d3d10.h>
#include <d3dx10.h>
#include "../Utilities/Macros.h"
#include "Effect.h"
#include "AABB.h"
#include "Sphere.h"

namespace gfx
{
	namespace RenderTypes
	{
		enum Types
		{
			RENDER_OBJECT = 1,
			TRANSFORM_OBJECT,
			BUFFER_OBJECT,
			INSTANCE_OBJECT
		};
	}

	namespace ShapeTypes
	{
		enum Types
		{
			TRIANGLE = 1,
			CUBE
		};
	}

	/**
	 * Abstract base class for all renderable objects.
	 */
	class RenderObject
	{
		public:
			/**
			 * Default constructor.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to an Effect.
			 */
			RenderObject(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			RenderObject(const RenderObject &object);

			/**
			 * Default destructor.
			 */
			virtual ~RenderObject() = 0;

			/**
			 * Default assignment operator.
			 */
			const RenderObject &operator=(const RenderObject &object);

			/**
			 * Clones the object.
			 */
			virtual RenderObject *clone() = 0;

			/**
			 * Updates the object.
			 * Functionality varies depending on object type.
			 */
			virtual void update() = 0;

			/**
			 * Renders the object.
			 * Method varies depending on object type.
			 */
			virtual bool render() = 0;

			/**
			 * Increases the position vector.
			 * @param offset Vector to add to mPosition.
			 */
			void increasePosition(const D3DXVECTOR3 &offset);

			/**
			 * Increases the X coordinate.
			 * @param x Value to add to mPosition.X.
			 */
			void increasePositionX(float x);

			/**
			 * Increases the Y coordinate.
			 * @param y Value to add to mPosition.y.
			 */
			void increasePositionY(float y);

			/**
			 * Increases the Z coordinate.
			 * @param z Value to add to mPosition.z.
			 */
			void increasePositionZ(float z);

			/**
			 * Sets the object ID.
			 * @param ID New ID.
			 */
			virtual void setID(int ID);

			/**
			 * Sets the object name.
			 * @param name New name in the form of a const char pointer.
			 */
			inline void setName(const char *name){mName = name;}

			/**
			 * Sets the object name.
			 * @param name New name in the form of a string.
			 */
			inline void setName(const std::string &name){mName = name;}

			/**
			 * Sets the position vector.
			 * @param position New position vector.
			 */
			void setPosition(const D3DXVECTOR3 &position);

			/**
			 * Sets the X coordinate.
			 * @param x New X coordinate.
			 */
			void setPositionX(float x);

			/**
			 * Sets the Y coordinate.
			 * @param y New Y coordinate.
			 */
			void setPositionY(float y);

			/**
			 * Sets the Z coordinate.
			 * @param z New Z coordinate.
			 */
			void setPositionZ(float z);

			/**
			 * Sets the color.
			 * @param color New D3DXCOLOR.
			 */
			virtual void setColor(const D3DXCOLOR &color){mColor = color;}

			/**
			 * Sets the texture ID.
			 *  Note: This is the texture index in the Texture2DArray.
			 * @param textureID New texture ID.
			 */
			virtual void setTextureID(int textureID){mTextureID = textureID;}

			/**
			 * Sets the render flag (if the object should be rendered).
			 * @param render New value for mFlagRender.
			 */
			virtual inline void setFlagRender(bool render){mFlagRender = render;}

			/**
			 * Sets the effect tech index.
			 * @param techIndex New value for mEffectTechIndex.
			 */
			virtual inline void setEffectTechIndex(unsigned int techIndex){mEffectTechIndex = techIndex;}

			/**
			 * Replaces the bounding volume of this object.
			 * @param pBV New bounding volume to set.
			 */
			void setBoundingVolume(CollisionObject *pBV);

			/**
			 * Retrieves the object's render type.
			 * @return Returns mType.
			 */
			inline const int &getType() const {return mType;}

			/**
			 * Retrieves the object's ID.
			 * @return Returns mID.
			 */
			inline const int &getID() const {return mID;}

			/**
			 * Retrieves the object's name.
			 * @return Returns mName in the form of a const char pointer.
			 */
			inline const char *getName() const {return mName.c_str();}

			/**
			 * Retrieves the object's name.
			 * @return Returns mName.
			 */
			inline const std::string &getNameString() const {return mName;}

			/**
			 * Retrieves the position vector.
			 * @return Returns mPosition.
			 */
			inline const D3DXVECTOR3 &getPosition() const {return mPosition;}

			/**
			 * Retrieves the X coordinate.
			 * @return Returns mPosition.x.
			 */
			inline const float &getPositionX() const {return mPosition.x;}

			/**
			 * Retrieves the Y coordinate.
			 * @return Returns mPosition.y.
			 */
			inline const float &getPositionY() const {return mPosition.y;}

			/**
			 * Retrieves the Z coordinate.
			 * @return Returns mPosition.z.
			 */
			inline const float &getPositionZ() const {return mPosition.z;}

			/**
			 * Retrieves the color.
			 * @return Returns mColor.
			 */
			inline const D3DXCOLOR &getColor() const {return mColor;}

			/**
			 * Retrieves the texture ID.
			 *  Note: This is the texture index in the Texture2DArray.
			 * @return Returns mTextureID.
			 */
			inline const int &getTextureID() const {return mTextureID;}

			/**
			 * Retrieves the render flag.
			 * @return Returns mFlagRender.
			 */
			inline const bool &getFlagRender() const {return mFlagRender;}

			/**
			 * Retrieves the effect technique index.
			 * @return Returns mEffectTechIndex.
			 */
			inline const unsigned int &getEffectTechIndex() const {return mEffectTechIndex;}

			/**
			 * Retrieves the bounding volume of this object.
			 * @return Returns mBoundingVolume;
			 */
			CollisionObject *getBoundingVolume();

		protected:
			/**
			 * Sets the object's render type.
			 * @param type New type of the object.
			 */
			inline void setType(int type){mType = type;}

			/**
			 * Sets the moved flag (forces all transforms to be recalculated).
			 * @param moved New value for mFlagMoved.
			 */
			inline void setFlagMoved(bool moved){mFlagNewMoved = moved;}

			/**
			 * Retrieves a pointer to the ID3D10Device.
			 * @return Returns mpDevice.
			 */
			inline ID3D10Device *getDevice(){return mpDevice;}

			/**
			 * Retrieves a pointer to the effect.
			 * @return Returns mpEffect.
			 */
			inline Effect *getEffect(){return mpEffect;}

			/**
			 * Retrieves the moved flag.
			 * @return Returns mFlagMoved.
			 */
			inline const bool &getFlagMoved() const {return mFlagMoved;}

		private:
			int mType;
			int mID;
			std::string mName;
			ID3D10Device *mpDevice;
			Effect *mpEffect;
			D3DXVECTOR3 mPosition;
			D3DXCOLOR mColor;
			int mTextureID;
			bool mFlagMoved;
			bool mFlagNewMoved;
			bool mFlagRender;
			unsigned int mEffectTechIndex;
			CollisionObject *mpBoundingVolume;
	};
}

#endif