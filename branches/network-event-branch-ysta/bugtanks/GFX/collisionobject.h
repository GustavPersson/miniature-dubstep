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
	   /**
	    *	Constructor.
		*	@className The type of the CollisionObject (AABB,OBB,Sphere).
		*/
	   CollisionObject(std::string className,int id);

	   /**
	    * Clone function.
		*/
	   virtual CollisionObject *clone();

	   /**
	    *	Returns true if there is a collision.
		*   @param co The CollisionObject that you check for collision against.
		*/
       virtual bool collision(const CollisionObject * pCollisionObject) const;

	   /**
	    *	Returns true if a ray hits this CollisionObject.
		*	@param xStart The x-position for the origin of the ray. 
		*	@param yStart The y-position for the origin of the ray.
		*	@param zStart The z-position for the origin of the ray.
		*	@param xDir The x-direction of the ray.
		*	@param xDir The y-direction of the ray.
		*	@param xDir The z-direction of the ray.
		*	@param lenght The lenght to the closest intersection (if there is one).
		*/
	   virtual bool intersection(float xStart, float yStart, float zStart, float xDir, float yDir, float zDir,float * pLenght) const;

	   /**
	    *	Moves this CollisionObject.
		*	@param x x-translation.
		*	@param y y-translation.
		*	@param z z-translation.
		*/
	   virtual void move(float x,float y,float z);

	   /**
	    * Sets the position.
		* @param position The position.
		*/
	   virtual void setPosition(const D3DXVECTOR3 &position);

	   /**
	    * Scales the bounding volume.
		* @param scale New scale based on the original size.
		*/
	   virtual void scale(const D3DXVECTOR3 &scale);

	   /**
	    *	Returns the class name.
		*/
	   std::string getClassName() const;

	   /**
	    * Sets the id.
		* @param id the id.
		*/
	   void setID(int id);

	   /**
	    * Returns the id.
		*/
	   int getID() const;

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

private:
	   std::string mClassName;
	   int mID;
	   void *mpParent;
};

}

#endif
