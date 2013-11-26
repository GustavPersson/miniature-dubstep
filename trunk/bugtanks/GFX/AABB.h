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

#ifndef AABB_H
#define AABB_H

#include "CollisionObject.h" 
#include "Sphere.h"
#include "OBB.h"

namespace gfx
{

/**
 * A axis aligned bounding box that can be placed in quadtree.
 */
class AABB : public CollisionObject
{
public:
	   /**
		*	Constructor.
		*	@param minX . Minimum x-position.
		*	@param minY . Minimum y-position.
		*	@param minZ . Minimum z-position.
		*	@param maxX . Maximum x-position.
		*	@param maxY . Maximum y-position.
		*	@param maxZ . Maximum z-position.
		*   @param id . The id.
		*/
        AABB(float minX,float minY,float minZ,float maxX,float maxY,float maxZ,int id);

		/**
		 * Clone function.
		 */
		virtual CollisionObject *clone();

		/**
	    *	Returns true if there is a collision.
		*   @param pCollisionObject . The CollisionObject that you check for collision against.
		*   @param calculateNormal . If you want the normal of the collision.
		*/
        bool collision(const CollisionObject * pCollisionObject, bool calculateNormal);

		/**
	     *	Returns true if a ray hits this CollisionObject.
		 *  @param start . The origin of the ray.
		 *  @param direction . The direction of the ray.
		 */
		bool intersection(const D3DXVECTOR3 &start, const D3DXVECTOR3 &direction) const;

		/**
		*	Moves this CollisionObject.
		*	@param x . x-translation.
		*	@param y . y-translation.
		*	@param z . z-translation.
		*/
		void move(float x,float y,float z);

		/**
		 * Sets the position.
		 * @param position . The position.
		 */
		void setPosition(const D3DXVECTOR3 &position);

		/**
	     * Scales the object in proportion to the original size.
		 * @param scale . The scale.
		 */
		void scale(const D3DXVECTOR3 &scale);

		/**
		 *	Returns the minimum x-postition.
		 */
        float getMinX() const;

		/**
		 *	Returns the minimum y-position.
		 */
        float getMinY() const;

		/**
		 *	Returns the minimum z-position.
		 */
        float getMinZ() const;

		/**
		 *	Returns the maximum x-position.
		 */
        float getMaxX() const;

		/**
		 *	Returns the maximum y-position.
		 */
        float getMaxY() const;

		/**
		 *	Returns the maximum z-position.
		 */
        float getMaxZ() const;

		/**
		 * Returns the center.
		 */
		inline const D3DXVECTOR3 &getCenter() const {return mCenter;}

		/**
		 * Returns a corner.
		 */
		inline const D3DXVECTOR3 &getCorner(unsigned int index) const {return mCorners[index];}
private:
		void updateCorners();

        float mMinX;
        float mMinY;
        float mMinZ;
        float mMaxX;
        float mMaxY;
        float mMaxZ;

		D3DXVECTOR3 mBaseSize;
		D3DXVECTOR3 mCenter;
		D3DXVECTOR3 mCorners[8];
};

}

#endif
