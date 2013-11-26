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

#ifndef MOUSE_MARKER_H
#define MOUSE_MARKER_H
#include <d3dx10.h>

namespace editor
{
	/**
	 * Class containing methods for selecting
	 *  and performing actions on game objects
	 *  in the editor.
	 */
	class MouseMarker
	{
		public:
			/**
			 * Current action/state that the marker performs.
			 */
			enum MarkerState
			{
				MarkerState_None = 1,
				MarkerState_Select = 2,
				MarkerState_Move = 4,
				MarkerState_Create = 8
			};

			/**
			 * Current type of object that the marker
			 *  has selected.
			 */
			enum MarkerType
			{
				MarkerType_None = 1,
				MarkerType_Object = 2,
				MarkerType_Actor = 4,
				MarkerType_Trigger = 8
			};

			/**
			 * Struct for storing a ray with an origin
			 *  and a direction.
			 */
			struct Ray
			{
				D3DXVECTOR3 mOrigin;
				D3DXVECTOR3 mDirection;

				Ray() : mOrigin(0.0f, 0.0f, 0.0f), mDirection(0.0f, 0.0f, 0.0f)
				{

				}

				~Ray(){}
			};

			/**
			 * Default constructor.
			 */
			MouseMarker();

			/**
			 * Default copy constructor.
			 */
			MouseMarker(const MouseMarker &marker);

			/**
			 * Default destructor.
			 */
			virtual ~MouseMarker();

			/**
			 * Default assignment operator.
			 */
			const MouseMarker &operator=(const MouseMarker &marker);

			/**
			 * Calculates the mouse ray.
			 */
			void updateMouseRay();

			/**
			 * Calculates the mouse's position on the game plane (Y = 0).
			 */
			void updateMousePlanePosition();

			/**
			 * Updates rotation and scale.
			 * @param deltaTime Time since last update.
			 */
			void updateTransform(float deltaTime);

			/**
			 * Resets rotation and scale.
			 */
			void resetTransform();

			/**
			 * Updates the current object's transformation.
			 */
			void updateObject();

			/**
			 * Updates the current object's position.
			 */
			void updateObjectPosition();

			/**
			 * Updates the current object's rotation.
			 */
			void updateObjectRotation();

			/**
			 * Updates the current object's scale.
			 */
			void updateObjectScale();

			/**
			 * Selects an object from the LogicQuadTree.
			 *  Selects based on the current mouse ray.
			 * @param types Allowed object types.
			 * @param overrideObject True if the current object can be overridden.
			 * @return Returns true if an object was selected.
			 */
			bool selectObject(int types, bool overrideObject);

			/**
			 * Clears the current object.
			 */
			void clearObject();

			/**
			 * Removes (deletes) the current object.
			 */
			void removeObject();

			/**
			 * Sets the X-coordinate of the mouse.
			 * @param x New value for mMouseX.
			 */
			inline void setMouseX(long x){mMouseX = x;}

			/**
			 * Sets the Y-coordinate of the mouse.
			 * @param y New value for mMouseY.
			 */
			inline void setMouseY(long y){mMouseY = y;}

			/**
			 * Sets the X- and Y-coordinate of the mouse.
			 * @param x New value for mMouseX.
			 * @param y New Value for mMouseY.
			 */
			inline void setMouseCoordinates(long x, long y){mMouseX = x; mMouseY = y;}

			/**
			 * Sets the marker state.
			 * @param state New value for mMarkerState.
			 */
			inline void setMarkerState(int state){mMarkerState = state;}

			/**
			 * Sets the marker type.
			 * @param type New value for mMarkerType.
			 */
			inline void setMarkerType(int type){mMarkerType = type;}

			/**
			 * Sets the object pointer.
			 *  Note: don't forget to set mMarkerType as well!
			 * @param pObject New object pointer.
			 */
			inline void setObject(void *pObject){mpObject = pObject;}

			/**
			 * Sets the object scale flag.
			 * @param scale New value for mFlagScale.
			 */
			inline void setFlagObjectScale(bool scale){mFlagScale = scale;}

			/**
			 * Sets the object rotate flag.
			 * @param rotate New value for mFlagRotate.
			 */
			inline void setFlagObjectRotate(bool rotate){mFlagRotate = rotate;}

			/**
			 * Sets the object scale speed.
			 * @param speed New value for mSpeedScale.
			 */
			inline void setSpeedObjectScale(float speed){mSpeedScale = speed;}

			/**
			 * Sets the object rotate speed.
			 * @param speed New value for mSpeedRotate.
			 */
			inline void setSpeedObjectRotate(float speed){mSpeedRotate = speed;}

			/**
			 * Retrieves the X-coordinate of the mouse.
			 * @return Returns mMouseX.
			 */
			inline long getMouseX() const {return mMouseX;}

			/**
			 * Retrieves the Y-coordinate of the mouse.
			 * @return Returns mMouseY.
			 */
			inline long getMouseY() const {return mMouseY;}

			/**
			 * Retrieves a const reference to the mouse ray.
			 * @return Returns a const reference to mMouseRay.
			 */
			inline const Ray &getMouseRay() const {return mMouseRay;}

			/**
			 * Retrieves the plane position (Y = 0) of the mouse.
			 * @return Returns a const reference to mPlanePosition.
			 */
			inline const D3DXVECTOR3 &getPlanePosition() const {return mPlanePosition;}

			/**
			 * Retrieves a direction vector based on the current rotation.
			 *  Should be supplied when creating actors.
			 * @param direction Outgoing reference to the direction vector.
			 */
			void getDirection(D3DXVECTOR3 &direction) const;

			/**
			 * Retrieves a direction vector based on a supplied angle.
			 * @param direction Outgoing reference to the direction vector.
			 * @param angleY Rotation angle around the Y-axis.
			 */
			void getDirection(D3DXVECTOR3 &direction, float angleY) const;

			/**
			 * Retrieves the marker state.
			 * @return Returns mMarkerState.
			 */
			inline int getMarkerState() const {return mMarkerState;}

			/**
			 * Retrieves the marker type.
			 * @return Returns mMarkerType.
			 */
			inline int getMarkerType() const {return mMarkerType;}

			/**
			 * Retrieves the object pointer.
			 * @return Returns mpObject.
			 */
			inline void *getObject(){return mpObject;}

		protected:
			/**
			 * Attempts to snap the object to the grid.
			 */
			void snapToGrid();

		private:
			long mMouseX;
			long mMouseY;
			Ray mMouseRay;
			D3DXVECTOR3 mPlanePosition;
			int mMarkerState;
			int mMarkerType;
			bool mFlagGrid;
			unsigned int mGridSizeX;
			unsigned int mGridSizeZ;
			float mGridAngle;
			void *mpObject;
			D3DXVECTOR3 mObjectScale;
			D3DXVECTOR3 mObjectRotation;
			float mSpeedScale;
			float mSpeedRotate;
			bool mFlagScale;
			bool mFlagRotate;
	};
}

#endif