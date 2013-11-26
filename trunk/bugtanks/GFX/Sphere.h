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

#ifndef SPHERE_H
#define SPHERE_H

#include "CollisionObject.h"
#include "AABB.h"

namespace gfx
{

/**
 * A Sphere that can be put in the qaudtree.
 */
class Sphere : public CollisionObject
{
public:
		/**
		 * Constructor. 
		 * @param x . x-position of the center.
		 * @param y . y-position of the center.
		 * @param z . z-position of the center.
		 * @param r . radius.
		 * @param id . The id.
		 */
        Sphere(float x,float y,float z,float r,int id);

		/**
		 * Clone function.
		 */
		virtual CollisionObject *clone();

		/**
	     *	Returns true if there is a collision.
		 *  @param pCollisionObject . The CollisionObject that you check for collision against.
		 *  @param calculateNormal . If you want the normal of the collision.
		 */
		bool collision(const CollisionObject * pCollisionObject, bool calculateNormal);

		/**
	     * Returns true if a ray hits this CollisionObject.
		 * @param start Start vector of the ray.
		 * @param direction Direction of the ray.
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
		virtual void setPosition(const D3DXVECTOR3 &position);

		/**
	     * Scales the object in proportion to the original size.
		 * @param scale . The scale.
		 */
		virtual void scale(const D3DXVECTOR3 &scale);

		/**
		 * returns x position.
		 */
		const float getX();

		/**
		 * returns y position.
		 */
		const float getY();

		/**
		 * returns z position.
		 */
		const float getZ();

		/**
		 * returns radius.
		 */
		const float getR();
private:
        float mX;
        float mY;
        float mZ;
        float mR;
		float mBaseR;
		D3DXVECTOR3 mCollisionNormal;
};

}

#endif
