/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
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

#ifndef __LOGICQUADTREE_H__
#define __LOGICQUADTREE_H__

#include "../Utilities/Macros.h"
#include "../GFX/DX10Module.h"


namespace logic
{
	class ActorHandler;

/**
 * This class contains a copy of the quadtree from gfx, used for colission tests.
 */

class LogicQuadTree
{
public:
	/**
	* The destructor.
	*/
	~LogicQuadTree();

	/**
	* Returns the only instance of this object.
	* @return Returns the instance of this object
	*/
	static LogicQuadTree* getInstance();

	/**
	* Calculates if a ray intersects with any bounding volumes.
	* @param origin The origin of the ray
	* @param direction The direction the ray is pointed at
	* @param actorID The id of the actor that fired the ray
	* @param bulletID The id of the bullet that is fired
	* @param beam defines if the ray belongs to a beam or something else (different behavior)
	*/
	void collisionRay(const D3DXVECTOR3 &origin, const D3DXVECTOR3 &direction, KeyType actorID, KeyType bulletID, bool beam);

	/**
	* Checks collision between one object and all other objects.
	*/
	void collisionObject(KeyType mId, float deltatime);

	void collisionMunitionStatic(KeyType mId);

	/**
	* Updates the quadtree from gfx.
	*/
	void updateQuadTree();

	/**
	 * Retrieves a pointer to the quad tree.
	 * @return Returns a pointer to mQuadTree.
	 */
	inline gfx::QuadTree *getQuadTree(){return &mQuadTree;}

private:
	/**
	* The constructor.
	*/
	LogicQuadTree();

	std::vector<gfx::CollisionObject *> mBVs;
	gfx::QuadTree mQuadTree;
	static LogicQuadTree *mpsInstance;
	ActorHandler *mpActorHandler;

};
}
#endif