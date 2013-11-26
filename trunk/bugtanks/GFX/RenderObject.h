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
#include "../Utilities/Macros.h"
#include <d3d10.h>
#include <d3dx10.h>
#include "../Logic/KeyType.h"

#define TOPOLOGY D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST

namespace gfx
{
	//Forward declarations.
	class Effect;
	class CollisionObject;

	/**
	 * Abstract base class for all renderable objects.
	 */
	class RenderObject
	{
		public:
			enum RenderTypes
			{
				RenderTypes_RenderObject = 1,
				RenderTypes_TransformObject,
				RenderTypes_BufferObject,
				RenderTypes_InstanceObject,
				RenderTypes_AnimationObject
			};

			enum ShapeTypes
			{
				ShapeTypes_Triangle = 1,
				ShapeTypes_Cube,
				ShapeTypes_Plane,
				ShapeTypes_Sprite
			};

			enum ShaderPasses
			{
				ShaderPasses_All = -1,
				ShaderPasses_GUI = 0,
				ShaderPasses_Cel = 1,
				ShaderPasses_Animation,
				ShaderPasses_MultiTex,
				ShaderPasses_Shadow,
				ShaderPasses_ShadowAnimation,
				ShaderPasses_Particle
			};

			enum ParticleOffsets
			{
				ParticleOffsets_SmallExplo = 0,
				ParticleOffsets_BigExplo = 2,
				ParticleOffsets_TankExplo = 4,
				ParticleOffsets_GunSparks = 6,
				ParticleOffsets_Pulse = 8,
				ParticleOffsets_Beam = 10,
				ParticleOffsets_Flame = 12
				
			};

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
			virtual void update(float deltaTime) = 0;

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
			 * Sets the object's model name.
			 * @param name New name in the form of a const char pointer.
			 */
			inline void setModelName(const char *name){mModelName = name;}

			/**
			 * Sets the object's model name.
			 * @param name New name in the form of a string.
			 */
			inline void setModelName(const std::string &name){mModelName = name;}

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
			virtual inline void setColor(const D3DXCOLOR &color){mColor = color;}

			/**
			 * Sets the ambient lighting
			 * @param ambient New D3DXCOLOR.
			 */
			virtual inline void setAmbient(const D3DXCOLOR &ambient){mAmbient = ambient;}

			/**
			 * Sets the diffuse lighting
			 * @param diffuse New D3DXCOLOR.
			 */
			virtual inline void setSpecular(const D3DXCOLOR &diffuse){mDiffuse = diffuse;}

			/**
			 * Sets the texture array index.
			 *  Note: This is the index of the texture array to be used.
			 * @param arrayIndex New array index.
			 */
			virtual inline void setTextureArrayIndex(int arrayIndex){mTextureArrayIndex = arrayIndex;}

			/**
			 * Sets the texture ID.
			 *  Note: This is the texture index in the Texture2DArray.
			 * @param textureID New texture ID.
			 */
			virtual inline void setTextureID(int textureID){mTextureID = textureID;}

			/**
			 * Sets the render flag (if the object should be rendered).
			 * @param render New value for mFlagRender.
			 */
			virtual inline void setFlagRender(bool render){mFlagRender = render;}

			/**
			 * Sets the shadow flag (if shadows should be rendered for the given object)
			 * @param shadow New value for mFlagShadows
			 */
			virtual inline void setFlagShadow(bool shadow){mFlagShadow = shadow;}

			/**
			 * Sets which render pass to render
			 * @param index Index of render pass (where -1 represents rendering all passes)
			 */
			virtual inline void setEffectPassIndex(int index) { mEffectPassIndex = index; }

			/**
			 * Sets the effect tech index.
			 *  Note: this is currently used as the input layout selection index.
			 *  The real technique index is always 0.
			 * @param index New value for mEffectTechIndex.
			 */
			virtual inline void setEffectTechIndex(unsigned int index){mEffectTechIndex = index;}

			/**
			 * Sets the effects layout index
			 */
			virtual inline void setEffectLayoutIndex(int index) { mEffectLayoutIndex = index; }

			/**
			 * Replaces the bounding volume of this object.
			 * @param pBV New bounding volume to set.
			 */
			virtual void setBoundingVolume(CollisionObject *pBV);

			/**
			 * Replaces the logic bounding volume of this object.
			 * @param pBV New bounding volume to set.
			 */
			virtual void setLogicBoundingVolume(CollisionObject *pBV);

			/**
			 * Sets the current actor pointer.
			 * @param pActor Pointer to the new actor.
			 */
			virtual inline void setActor(void *pActor){mpActor = pActor;}

			/**
			 * Retrieves the object's render type.
			 * @return Returns mType.
			 */
			inline int getType() const {return mType;}

			/**
			 * Retrieves the object's ID.
			 * @return Returns mID.
			 */
			inline int getID() const {return mID;}

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
			 * Retrieves the object's model name.
			 * @return Returns mName in the form of a const char pointer.
			 */
			inline const char *getModelName() const {return mModelName.c_str();}

			/**
			 * Retrieves the object's model name.
			 * @return Returns mName.
			 */
			inline const std::string &getModelNameString() const {return mModelName;}

			/**
			 * Retrieves the position vector.
			 * @return Returns mPosition.
			 */
			inline const D3DXVECTOR3 &getPosition() const {return mPosition;}

			/**
			 * Retrieves the X coordinate.
			 * @return Returns mPosition.x.
			 */
			inline float getPositionX() const {return mPosition.x;}

			/**
			 * Retrieves the Y coordinate.
			 * @return Returns mPosition.y.
			 */
			inline float getPositionY() const {return mPosition.y;}

			/**
			 * Retrieves the Z coordinate.
			 * @return Returns mPosition.z.
			 */
			inline float getPositionZ() const {return mPosition.z;}

			/**
			 * Retrieves the color.
			 * @return Returns mColor.
			 */
			inline const D3DXCOLOR &getColor() const {return mColor;}

			/**
			 * Retrieves the texture array index.
			 *  Note: This is the index of the texture array to be used.
			 * @return Returns mTextureArrayIndex.
			 */
			inline int getTextureArrayIndex() const {return mTextureArrayIndex;}

			/**
			 * Retrieves the texture ID.
			 *  Note: This is the texture index in the Texture2DArray.
			 * @return Returns mTextureID.
			 */
			inline int getTextureID() const {return mTextureID;}

			/**
			 * Retrieves the render flag.
			 * @return Returns mFlagRender.
			 */
			inline bool getFlagRender() const {return mFlagRender;}
			
			/**
			 * Retrieves the shadow flag.
			 * @return Returns mFlagShadow.
			 */
			inline bool getFlagShadow() const {return mFlagShadow;}

			/**
			 * Returns the shader pass used for rendering
			 * @return Returns mEffectPassIndex
			 */
			inline int getEffectPassIndex() const { return mEffectPassIndex; }

			/**
			 * Retrieves the effect technique index.
			 * @return Returns mEffectTechIndex.
			 */
			inline unsigned int getEffectTechIndex() const {return mEffectTechIndex;}

			/**
			 * Returns the layout index for the current pass
			 * @return Returns mEffectLayoutIndex
			 */
			inline int getEffectLayoutIndex() const { return mEffectLayoutIndex; }

			/**
			 * Retrieves the bounding volume of this object.
			 * @return Returns mpBoundingVolume;
			 */
			CollisionObject *getBoundingVolume();

			/**
			 * Retrieves the logic bounding volume of this object.
			 * @return Returns mpBoundingVolume;
			 */
			inline CollisionObject *getLogicBoundingVolume(){return mpLogicBoundingVolume;}

			/**
			 * Retrieves the actor pointer.
			 * @return Returns mpActor.
			 */
			inline void *getActor(){return mpActor;}

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
			inline bool getFlagMoved() const {return mFlagMoved;}

		private:
			int mType;
			int mID;
			std::string mName;
			std::string mModelName;
			ID3D10Device *mpDevice;
			Effect *mpEffect;
			D3DXVECTOR3 mPosition;
			D3DXCOLOR mColor;
			D3DXCOLOR mAmbient;
			D3DXCOLOR mDiffuse;
			int mTextureArrayIndex;
			int mTextureID;
			bool mFlagMoved;
			bool mFlagNewMoved;
			bool mFlagRender;
			bool mFlagShadow;
			int mEffectPassIndex;
			unsigned int mEffectTechIndex;
			int mEffectLayoutIndex;
			CollisionObject *mpBoundingVolume;
			CollisionObject *mpLogicBoundingVolume;
			void *mpActor;
	};
}

#endif