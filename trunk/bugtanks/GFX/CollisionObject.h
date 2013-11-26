/**
 * @file
 * @author Carl Nordlander <cand05@student.bth.se>
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

#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H

#include "../Utilities/Macros.h"
#include "../Logic/KeyType.h"
#include <string>
#include <d3dx10.h>

namespace gfx
{

/**
 * A base class for the objects that can be placed in the quadtree.
 */
class CollisionObject
{
public:
	   enum CollisionTypes
	   {
			CollisionType_Invalid,
			CollisionType_Sphere,
			CollisionType_AABB,
			CollisionType_OBB,
			CollisionType_Frustum
	   };
	   /**
	    *	Constructor.
		*	@param className . The type of the CollisionObject (AABB,OBB,Sphere).
		*	@param id . The id of the CollisionObject.
		*/
	   CollisionObject(std::string className, logic::KeyType id);

	   /**
	    * Clone function.
		*/
	   virtual CollisionObject *clone();

	   /**
	    *	Returns true if there is a collision.
		*   @param pCollisionObject . The CollisionObject that you check for collision against.
		*   @param calculateNormal . If you want the normal of the collision.
		*/
	   virtual bool collision(const CollisionObject * pCollisionObject, bool calculateNormal = false) = 0;

	   /**
	    *  Returns true if a ray hits this CollisionObject.
		*  @param start . The origin of the ray.
		*  @param direction . The direction of the ray.
		*/
	   virtual bool intersection(const D3DXVECTOR3 &start, const D3DXVECTOR3 &direction);

	   /**
	    *	Moves this CollisionObject.
		*	@param x . x-translation.
		*	@param y . y-translation.
		*	@param z . z-translation.
		*/
	   virtual void move(float x, float y, float z);

	   /**
	    * Sets the position.
		* @param position . The position.
		*/
	   virtual void setPosition(const D3DXVECTOR3 &position);

	   /**
	    * Scales the object in proportion to the original size.
		* @param scale . The scale.
		*/
	   virtual void scale(const D3DXVECTOR3 &scale);

	   virtual void rotateX(float angle);

	   virtual void rotateY(float angle);

	   virtual void rotateZ(float angle);

	   /**
	    * Calculates the distance from the supplied point to the center
		*  of the bounding volume.
		* @param point Point to calculate distance from.
		* @return Returns the distance from the supplied point.
		*/
	   virtual float getDistance(const D3DXVECTOR3 &point);

	   /**
	    *	Returns the class name.
		*/
	   inline const std::string &getClassName() const {return mClassName;}

	   inline CollisionTypes getCollisionType() const {return mCollisionType;}

	   /**
	    * Sets the id.
		* @param id . The id.
		*/
	   void setID(logic::KeyType id);

	   /**
	    * Returns the id.
		*/
	   inline logic::KeyType getID() const {return mID;}

	   /**
	    * Sets the collision object's parent.
		* @param pParent Void pointer to the parent object.
		*/
	   inline void setParent(void *pParent){mpParent = pParent;}

	   /**
	    * Retrieves the parent pointer.
		* @return Returns mpParent.
		*/
	   inline void *getParent(){return mpParent;}

	   /**
	    * Returns the normal of the last collision.
		*/
	   inline const D3DXVECTOR3 &getCollisionNormal() const {return mCollisionNormal;}

	   /**
		* Sets the collision normal.
		* Should only be used by other CollisionObjects.
		* @param collisionNormal . The collision normal.
		*/
	   virtual void setCollisionNormal(D3DXVECTOR3 collisionNormal);

	   inline const D3DXVECTOR3 &getCenterToCenterVectorNormalized() {return mCenterToCenterVectorNormalized;}

	   inline const D3DXVECTOR3 &getIntersectionPoint() const {return mIntersectionPoint;}

protected:
		D3DXVECTOR3 mIntersectionPoint;
		D3DXVECTOR3 mCollisionNormal;
		D3DXVECTOR3 mCenterToCenterVectorNormalized;

private:
	   std::string mClassName;
	   CollisionTypes mCollisionType;
	   logic::KeyType mID;
	   void *mpParent;
};

}

#endif
