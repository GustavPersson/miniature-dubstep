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
		*	@param minX minimum x-position
		*	@param minY minimum y-position
		*	@param minZ minimum z-position
		*	@param maxX maximum x-position
		*	@param maxY maximum y-position
		*	@param maxZ maximum z-position
		*   @param id id
		*/
        AABB(float minX,float minY,float minZ,float maxX,float maxY,float maxZ,int id);

		virtual CollisionObject *clone();

		/**
	    *	Returns true if there is a collision.
		*   @param pCollisionObject The CollisionObject that you check for collision against.
		*/
        bool collision(const CollisionObject * pCollisionObject) const;

		/**
	    *	Returns true if a ray hits this CollisionObject.
		*	@param xStart The x-position for the origin of the ray. 
		*	@param yStart The y-position for the origin of the ray.
		*	@param zStart The z-position for the origin of the ray.
		*	@param xDir The x-direction of the ray.
		*	@param yDir The y-direction of the ray.
		*	@param zDir The z-direction of the ray.
		*	@param pLenght The lenght to the closest intersection (if there is one).
		*/
		bool intersection(float xStart, float yStart, float zStart, float xDir, float yDir, float zDir,float * pLenght) const;

		void move(float x,float y,float z);

		void setPosition(const D3DXVECTOR3 &position);

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
private:
        float mMinX;
        float mMinY;
        float mMinZ;
        float mMaxX;
        float mMaxY;
        float mMaxZ;

		D3DXVECTOR3 mBaseSize;
};

}

#endif
