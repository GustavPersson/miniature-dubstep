/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
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

#ifndef OBJECT_HANDLER_H
#define OBJECT_HANDLER_H
#include <vector>
#include "Mesh.h"
#include "InstanceMesh.h"
#include "QuadTree.h"
#include "Frustum.h"
#include "Camera.h"

namespace gfx
{
	/**
	 * Handler that stores all RenderObjects in the scene.
	 * Performs culling operations.
	 */
	class ObjectHandler
	{
		public:
			/**
			 * Default constructor.
			 */
			ObjectHandler();

			/**
			 * Default destructor.
			 */
			virtual ~ObjectHandler();

			/**
			 * Updates all objects in the mObjectList vector.
			 */
			void updateObjects();

			/**
			 * Perform culling operations on all objects in the mObjectList.
			 * Adds the resulting visible objects in the mCulledObjects vector.
			 */
			void updateCulling();

			/**
			 * Retrieves an object from the mObjectList based on its ID.
			 * @param objectID ID of the object.
			 * @return Returns a pointer to the RenderObject.
			 */
			RenderObject *getObject(int objectID);

			/**
			 * Retrieves an object from the mObjectList based on its name.
			 * @param objectName Name of the object.
			 * @return Returns a pointer to the RenderObject.
			 */
			RenderObject *getObject(const char *objectName);

			/**
			 * Adds a new object to the mObjectList vector.
			 * The object should not be deleted outside of this class.
			 * @param pObject Pointer to the RenderObject to add.
			 */
			void addObject(RenderObject *pObject);

			/**
			 * Removes/deletes an object from the mObjectList vector.
			 * Will cause the mCulledObjects vector to the cleared.
			 * @param pObject Pointer to the RenderObject to remove.
			 */
			void removeObject(RenderObject *pObject);

			/**
			 * Removes/deletes an object from the mObjectList vector.
			 * Will cause the mCulledObjects vector to the cleared.
			 * @param objectID ID of the object to remove.
			 */
			void removeObject(int objectID);

			/**
			 * Removes/deletes an object from the mObjectList vector.
			 * Will cause the mCulledObjects vector to the cleared.
			 * @param objectName Name of the object to remove.
			 */
			void removeObject(const char *objectName);

			/**
			 * Removes/deletes all objects from the mObjectList vector.
			 * Will cause the mCulledObjects vector to the cleared.
			 */
			void removeAllObjects();

			/**
			 * Creates a new mesh from a mesh model and adds it
			 *  to the mObjectList.
			 * @param modelID ID of the mesh model.
			 * @return Returns a pointer to the new mesh.
			 */
			Mesh *addMeshFromModel(int modelID);

			/**
			 * Creates a new mesh from a mesh model and adds it
			 *  to the mObjectList.
			 * @param modelName Name of the mesh model.
			 * @return Returns a pointer to the new mesh.
			 */
			Mesh *addMeshFromModel(const char *modelName);

			/**
			 * Adds a mesh model to mMeshModelList.
			 * @param pModel Pointer to the mesh model to add.
			 */
			void addMeshModel(Mesh *pModel);

			/**
			 * Retrieves a mesh model based on its ID.
			 * @param modelID ID of the mesh model.
			 * @return Returns a pointer to the mesh model.
			 */
			Mesh *getMeshModel(int modelID);

			/**
			 * Retrieves a mesh model based on its name.
			 * @param modelName Name of the mesh model.
			 * @return Returns a pointer to the mesh model.
			 */
			Mesh *getMeshModel(const char *modelName);

			/**
			 * Removes (deletes) a mesh model.
			 * @param pModel Pointer to the mesh model to remove.
			 */
			void removeMeshModel(Mesh *pModel);

			/**
			 * Removes (deletes) a mesh model.
			 * @param modelID ID of the mesh model to remove.
			 */
			void removeMeshModel(int modelID);

			/**
			 * Removes (deletes) a mesh model.
			 * @param modelName Name of the mesh model to remove.
			 */
			void removeMeshModel(const char *modelName);

			/**
			 * Removes (deletes) all mesh models.
			 */
			void removeAllMeshModels();

			/**
			 * Adds an instance of an instance base.
			 *  The instance is not added to any lists but can be retrieved from its base.
			 * @param instanceID ID of the instance to derive from.
			 * @return Returns a pointer to the new instance.
			 */
			InstanceObject *addObjectFromInstance(int instanceID);

			/**
			 * Adds an instance of an instance base.
			 *  The instance is not added to any lists but can be retrieved from its base.
			 * @param instanceID ID of the instance to derive from.
			 * @return Returns a pointer to the new instance.
			 */
			InstanceObject *addObjectFromInstance(const char *instanceName);

			/**
			 * Adds a new instance base.
			 * @param pInstance Pointer to the instance base to add.
			 */
			void addObjectInstanceBase(InstanceObject *pInstance);

			/**
			 * Retrieves an instance base.
			 * @param instanceID ID of the instance base to retrieve.
			 */
			InstanceObject *getObjectInstanceBase(int instanceID);

			/**
			 * Retrieves an instance base.
			 * @param instanceName Name of the instance base to retrieve.
			 */
			InstanceObject *getObjectInstanceBase(const char *instanceName);

			/**
			 * Removes (deletes) an instance base.
			 * @param pInstance Pointer to the instance to remove.
			 */
			void removeObjectInstanceBase(InstanceObject *pInstance);

			/**
			 * Removes (deletes) an instance base.
			 * @param instanceID ID of the instance to remove.
			 */
			void removeObjectInstanceBase(int instanceID);

			/**
			 * Removes (deletes) an instance base.
			 * @param instanceName Name of the instance to remove.
			 */
			void removeObjectInstanceBase(const char *instanceName);

			/**
			 * Removes (deletes) all instance bases.
			 */
			void removeAllObjectInstanceBases();

			/**
			 * Adds an instance of an instance base.
			 *  The instance is not added to any lists but can be retrieved from its base.
			 * @param instanceID ID of the instance to derive from.
			 * @return Returns a pointer to the new instance.
			 */
			InstanceMesh *addMeshFromInstance(int instanceID);

			/**
			 * Adds an instance of an instance base.
			 *  The instance is not added to any lists but can be retrieved from its base.
			 * @param instanceName Name of the instance to derive from.
			 * @return Returns a pointer to the new instance.
			 */
			InstanceMesh *addMeshFromInstance(const char *instanceName);

			/**
			 * Adds a new instance base.
			 * @param pInstance Pointer to the instance base to add.
			 */
			void addMeshInstanceBase(InstanceMesh *pInstance);

			/**
			 * Retrieves an instance base.
			 * @param instanceID ID of the instance base to retrieve.
			 */
			InstanceMesh *getMeshInstanceBase(int instanceID);

			/**
			 * Retrieves an instance base.
			 * @param instanceName Name of the instance base to retrieve.
			 */
			InstanceMesh *getMeshInstanceBase(const char *instanceName);

			/**
			 * Removes (deletes) an instance base.
			 * @param pInstance Pointer to the instance to remove.
			 */
			void removeMeshInstanceBase(InstanceMesh *pInstance);

			/**
			 * Removes (deletes) an instance base.
			 * @param instanceID ID of the instance to remove.
			 */
			void removeMeshInstanceBase(int instanceID);

			/**
			 * Removes (deletes) an instance base.
			 * @param instanceName Name of the instance to remove.
			 */
			void removeMeshInstanceBase(const char *instanceName);

			/**
			 * Removes (deletes) all instance bases.
			 */
			void removeAllMeshInstanceBases();

			/**
			 * Retrieves a const reference to the mCulledObjects vector.
			 * @return Returns a const reference to the mCulledObjects vector.
			 */
			inline const std::vector<RenderObject *> &getCulledObjects() const {return mCulledObjects;}

			/**
			 * Sets the current camera.
			 */
			void setCamera(Camera *pCamera);

			/**
			 * Copies bounding volumes.
			 * @param pBVs Pointer to a CollisionObject vector to copy to.
			 */
			void copyBoundingVolumes(std::vector<CollisionObject *> *pBVs);

		protected:
			/**
			 * Default copy constructor.
			 */
			ObjectHandler(const ObjectHandler &handler);

			/**
			 * Default assignment operator.
			 */
			const ObjectHandler &operator=(const ObjectHandler &handler);

			/**
			 * Removes an instance from an instance base.
			 * @param pInstance Pointer to the instance to remove.
			 */
			void removeInstance(RenderObject *pInstance);

			/**
			 * Removes an instance from an instance base.
			 * @param instanceID ID of the instance to remove.
			 */
			void removeInstance(int instanceID);

			/**
			 * Removes an instance from an instance base.
			 * @param instanceName Name of the instance to remove.
			 */
			void removeInstance(const char *instanceName);

		private:
			std::vector<RenderObject *> mObjectList;
			std::vector<RenderObject *> mCulledObjects;
			std::vector<Mesh *> mMeshModelList;
			std::vector<InstanceObject *> mObjectInstanceList;
			std::vector<InstanceMesh *> mMeshInstanceList;
			QuadTree *mpQuadTree;
			Frustum *mpFrustum;
			Camera *mpCamera;
	};
};

#endif