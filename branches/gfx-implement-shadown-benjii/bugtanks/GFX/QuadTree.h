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

#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <iostream>
#include "../Utilities/Macros.h"
#include "CollisionObject.h"
#include "AABB.h"
#include "../Logic/KeyHandler.h"

namespace gfx
{

/**
 * A quadtree that can be used to check for collisions.
 */
class QuadTree
{
public:
	/**
	 * Constructor.
	 * @param minX minimum x-position
	 * @param minZ minimum z-position
	 * @param maxX maximum x-position
	 * @param maxZ maximum z-position
	 * @param levels number of levels (1 level : 4 leaf nodes, 2 levels : 16 leaf nodes , etc..)
	 */
	QuadTree(float minX,float minZ,float maxX,float maxZ,int levels);

	/**
	 *	Destructor.
	 */
	~QuadTree();

	/**
	 *	 Removes all objects from the QuadTree and the adds objects.
	 *   @param pCollisionObjects . A vector of CollisionObject that are added to the QuadTree.
	 */
	void update(std::vector<CollisionObject*> * pCollisionObjects);

	/**
	 *	Ray intersection test.
	 *  @param xStart The x-position for the origin of the ray. 
	 *  @param yStart The y-position for the origin of the ray.
	 *  @param zStart The z-position for the origin of the ray.
	 *  @param xDir The x-direction of the ray.
	 *  @param xDir The y-direction of the ray.
	 *  @param xDir The z-direction of the ray.
	 *	@param maxLenght The max lenght of the ray.
	 *  @param lenght The lenght to the closest intersection (if there is one).
	 *  @param pOutput The ids of the objects that has been hit. 
	 */
	void intersection(float xStart,float yStart,float zStart,float xDir,float yDir,float zDir,float maxLenght,float * lenght, logic::KeyType actorID, std::vector<logic::KeyType> * pOutput) const;

	/**
	 *	Checks if objects collide with an object.
	 *  @param pCollisionObjects . The CollisionObject to check for collisions against.
	 *  @param pOutput . A the objects that collided with the object.
     */
	void collisionCheck(const CollisionObject * pCollisionObject, std::vector<CollisionObject*> * pOutput) const;

private:
	bool addCollisionObjectToNodes(CollisionObject * co);

	AABB * mpAABB;
	bool mLeaf;
	QuadTree* mpChildren[4];
	std::vector<CollisionObject*> mCollisionObjects;
};

}

#endif