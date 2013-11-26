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
	 * Constructor.
	 * @param minX . Minimum x-position.
	 * @param minY . Minimum y-position.
	 * @param minZ . Minimum z-position.
	 * @param maxX . Maximum x-position.
	 * @param maxY . Maximum y-position.
	 * @param maxZ . Maximum z-position.
	 * @param id . The id.
	 */
	OBB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, int id);

	/**
     * Clone function.
	 */
    virtual CollisionObject *clone();

	/**
	 *	Returns true if there is a collision.
	 *  @param pCollisionObject . The CollisionObject that you check for collision against.
	 *  @param calculateNormal . If you want the normal of the collision (not used).
	 */
	bool collision(const CollisionObject * pCollisionObject, bool calculateNormal);

	/**
	 *	Returns true if a ray hits this CollisionObject.
     *  @param start . The origin of the ray.
	 *  @param direction . The direction of the ray.
	 */
	bool intersection(const D3DXVECTOR3 &start, const D3DXVECTOR3 &direction);

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
	 * Returns a corner.
	 * @param index . The index of the corner.
	 */
	inline const D3DXVECTOR3 &getCorner(int index) const;

	/**
	 * Returns a axis.
	 * @param index . The index of the axis.
	 */
	inline const D3DXVECTOR3 &getAxis(int index) const;

	/**
	 * Returns the center.
	 */
	inline const D3DXVECTOR3 &getCenter() const;

	/**
	 * Rotates the OBB around the x-axis.
	 * @param angle . The angle.
	 */
	void rotateX(float angle);

	/**
	 * Rotates the OBB around the y-axis.
	 * @param angle . The angle.
	 */
	void rotateY(float angle);

	/**
	 *	Rotates the OBB around the z-axis.
	 *	@param angle . The angle.
	 */
	void rotateZ(float angle);

	/**
     * Calculates the distance from the supplied point to the center
	 *  of the bounding volume.
	 * @param point Point to calculate distance from.
	 * @return Returns the distance from the supplied point.
	 */
	float getDistance(const D3DXVECTOR3 &point);

	/**
	 * Retrieves the size along the X-axis.
	 * @return Returns mSizeX.
	 */
	inline float getSizeX() const {return mSizeX;}

	/**
	 * Retrieves the size along the Y-axis.
	 * @return Returns mSizeY.
	 */
	inline float getSizeY() const {return mSizeY;}

	/**
	 * Retrieves the size along the Z-axis.
	 * @return Returns mSizeZ.
	 */
	inline float getSizeZ() const {return mSizeZ;}

	void setCollisionNormal(D3DXVECTOR3 collisionNormal);

private:
	void update();

	D3DXVECTOR3 mBaseSize;
	D3DXVECTOR3 mCorners[8];
	D3DXVECTOR3 mAxis[3];
	D3DXVECTOR3 mCenter;
	D3DXVECTOR3 mCenterOffset;
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
