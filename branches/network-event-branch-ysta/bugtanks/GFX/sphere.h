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
		 * @param x x-position of the center.
		 * @param y y-position of the center.
		 * @param z z-position of the center.
		 * @param r radius.
		 */
        Sphere(float x,float y,float z,float r,int id);

		virtual CollisionObject *clone();

		bool collision(const CollisionObject * pCollisionObject) const;

		bool intersection(float xStart, float yStart, float zStart, float xDir, float yDir, float zDir,float * pLenght) const;

		void move(float x,float y,float z);

		virtual void setPosition(const D3DXVECTOR3 &position);

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
};

}

#endif
