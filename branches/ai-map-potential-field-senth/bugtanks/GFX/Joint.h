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

#ifndef JOINT_H
#define JOINT_H

#include "../Utilities/Macros.h"
#include <d3dx10.h>
#include <vector>

namespace gfx
{
	/**
	 * A joint in a skeleton.
	 * used to create animations.
	 */
	class Joint
	{
		public:
			/**
			 * Constructor.
			 */
			Joint();

			/**
			 * Destructor.
			 */
			~Joint();

			/**
			 * Sets the position.
			 * @param position . Position.
			 */
			void setPosition(const D3DXVECTOR3 &position);

			/**
			 * Sets the rotation.
			 * @param rotation . Rotation.
			 */
			void setRotation(const D3DXQUATERNION &rotation);

			/**
			 * Sets the orientation.
			 * @param orientation . Orientation.
			 */
			inline void setOrientation(const D3DXQUATERNION &orientation){mOrientation = orientation;}

			/**
			 * Sets the toWorld matrix.
			 * Should not be used.
			 * @param toWorld . ToWorld matrix.
			 */
			void setToWorld(const D3DXMATRIX &toWorld);

			/**
			 * Sets the to toParent matrix.
			 * Should not be used.
			 * @param toParent . ToParent matrix.
			 */
			void setToParent(const D3DXMATRIX &toParent);

			/**
			 * Sets the bind inverse matrices for the skeleton.
			 * Should only be used on the root.
			 */
			void setBindPose();

			/**
			 * Returns the position.
			 */
			const D3DXVECTOR3 &getPosition() const;

			/**
			 * Returns the rotation.
			 */
			const D3DXQUATERNION &getRotation() const;

			/**
			 * Returns the toWorld matrix.
			 */
			const D3DXMATRIX &getToWorld() const;

			/**
			 * Returns the toParent matrix.
			 */
			const D3DXMATRIX &getToParent() const;

			/**
			 * Returns the bind inverse matrix.
			 */
			const D3DXMATRIX &getBindInverse() const;

			/**
			 * Adds a child.
			 * @param child . The child.
			 */
			void addChild(Joint* child);

			/**
			 * Updates the skeleton.
			 * Should only be used on the root.
			 * @param root . If its the root.
			 */
			void update(bool root);

		private:
			D3DXVECTOR3 mPosition;
			D3DXQUATERNION mRotation;
			D3DXQUATERNION mOrientation;
			D3DXMATRIX mToWorld;
			D3DXMATRIX mToParent;
			D3DXMATRIX mBindInverse;
			std::vector<Joint*> mChildren; 
	};
}

#endif