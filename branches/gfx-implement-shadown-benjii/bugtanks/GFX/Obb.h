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

#ifndef OBB_H
#define OBB_H

#include <iostream>
#include "CollisionObject.h"
#include "AABB.h"
#include "Sphere.h"

namespace gfx
{

/**
 * A oriented bounding box that can be placed in the quadtree.
 */
class OBB : public CollisionObject
{
public:
	/**
	 *	Constructor.
	 * @param minX minimum x-position
	 * @param minX minimum y-position
	 * @param minZ minimum z-position
	 * @param maxX maximum x-position
	 * @param maxX maximum y-position
	 * @param maxZ maximum z-position
	 */
	OBB(float minX,float minY,float minZ,float maxX,float maxY,float maxZ,int id);

	bool collision(const CollisionObject * pCollisionObject) const;

	bool intersection(float xStart, float yStart, float zStart, float xDir, float yDir, float zDir,float * pLenght) const;

	void move(float x,float y,float z);

	/**
	 * Returns a corner.
	 * @param index The index of the corner.
	 */
	D3DXVECTOR3 getCorner(int index) const;

	/**
	 * Returns a axis.
	 * @param index The index of the axis.
	 */
	D3DXVECTOR3 getAxis(int index) const;

	/**
	 * Returns the center.
	 */
	D3DXVECTOR3 getCenter() const;

	/**
	 * Rotates the OBB around the x-axis.
	 * @param angle The angle.
	 */
	void rotateX(float angle);

	/**
	 * Rotates the OBB around the y-axis.
	 * @param angle The angle.
	 */
	void rotateY(float angle);

	/**
	 *	Rotates the OBB around the z-axis.
	 *	@param angle The angle.
	 */
	void rotateZ(float angle);

	//void printCorners() const;

private:
	void update();

	D3DXVECTOR3 mCorners[8];
	D3DXVECTOR3 mAxis[3];
	D3DXVECTOR3 mCenter;
	D3DXMATRIX mRotXM;
	D3DXMATRIX mRotYM;
	D3DXMATRIX mRotZM;
	float mRotX;
	float mRotY;
	float mRotZ;
	float mSizeX;
	float mSizeY;
	float mSizeZ;
};

}

#endif
