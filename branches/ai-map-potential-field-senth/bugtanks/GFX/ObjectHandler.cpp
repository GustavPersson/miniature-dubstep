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

#include "ObjectHandler.h"

using namespace gfx;
using std::vector;

ObjectHandler::ObjectHandler() : mObjectList(), mCulledObjects(), mMeshModelList(), mObjectInstanceList(),
			mMeshInstanceList(), mGUIInstanceList()
{
	mpQuadTree = myNew QuadTree(-10000.0f, -10000.0f, 10000.0f, 10000.0f, 3);
	mpFrustum = myNew Frustum();
}

ObjectHandler::ObjectHandler(const ObjectHandler &handler) : mObjectList(), mCulledObjects(), mMeshModelList(),
			mObjectInstanceList(), mMeshInstanceList(), mGUIInstanceList()
{
	//TO DO: Copy object list.
	handler;
}

ObjectHandler::~ObjectHandler()
{
	removeAllObjects();
	removeAllMeshModels();
	removeAllObjectInstanceBases();
	removeAllMeshInstanceBases();
	removeAllGUIInstanceBases();
	SAFE_DELETE(mpQuadTree);
	SAFE_DELETE(mpFrustum);
}

const ObjectHandler &ObjectHandler::operator=(const ObjectHandler &handler)
{
	if(this != &handler)
	{

	}
	return *this;
}

void ObjectHandler::updateObjects(float deltaTime)
{
	//Update standard objects.
	for(vector<RenderObject *>::iterator n = mObjectList.begin();
		n != mObjectList.end(); ++n)
	{
		(*n)->update(deltaTime);
	}
	//Update instance objects and meshes.
	for(vector<InstanceObject *>::iterator baseIter = mObjectInstanceList.begin();
			baseIter != mObjectInstanceList.end(); ++baseIter)
	{
		(*baseIter)->update(deltaTime);
	}
	for(vector<InstanceMesh *>::iterator baseIter = mMeshInstanceList.begin();
			baseIter != mMeshInstanceList.end(); ++baseIter)
	{
		(*baseIter)->update(deltaTime);
	}
	for(vector<InstanceSprite *>::iterator baseIter = mGUIInstanceList.begin();
			baseIter != mGUIInstanceList.end(); ++baseIter)
	{
		(*baseIter)->update(deltaTime);
	}
}

void ObjectHandler::updateCulling()
{
	mCulledObjects.clear();

	//Always add all instance bases as they don't have any bounding volumes.
	for(vector<InstanceObject *>::iterator baseIter = mObjectInstanceList.begin();
		baseIter != mObjectInstanceList.end(); ++baseIter)
	{
		mCulledObjects.push_back((*baseIter));
	}
	for(vector<InstanceMesh *>::iterator baseIter = mMeshInstanceList.begin();
			baseIter != mMeshInstanceList.end(); ++baseIter)
	{
		mCulledObjects.push_back((*baseIter));
	}
	//Add GUI sprites last.
	for(vector<InstanceSprite *>::iterator GUIIter = mGUIInstanceList.begin();
		GUIIter != mGUIInstanceList.end(); ++GUIIter)
	{
		mCulledObjects.push_back((*GUIIter));
	}

	/*if(mpCamera!=NULL)
	{
		D3DXMATRIX view;
		D3DXMATRIX projection;
		D3DXMATRIX ortProjection;
		mpCamera->GetVPMatrices(view, projection, ortProjection);
		mpFrustum->update(view,projection);

		//Always add all instance bases as they don't have any bounding volumes.
		for(vector<InstanceObject *>::iterator baseIter = mObjectInstanceList.begin();
			baseIter != mObjectInstanceList.end(); ++baseIter)
		{
			mCulledObjects.push_back((*baseIter));
		}
		for(vector<InstanceMesh *>::iterator baseIter = mMeshInstanceList.begin();
				baseIter != mMeshInstanceList.end(); ++baseIter)
		{
			mCulledObjects.push_back((*baseIter));
		}

		//Add all objects' bounding volumes to BVs.
		vector<CollisionObject *> BVs;

		//Standard objects.
		for(vector<RenderObject *>::iterator n = mObjectList.begin();
			n != mObjectList.end(); ++n)
		{
			if((*n)->getBoundingVolume() && (*n)->getFlagRender())
			{
				BVs.push_back((*n)->getBoundingVolume());
			}
		}

		//Object instances.
		for(vector<InstanceObject *>::iterator baseIter = mObjectInstanceList.begin();
			baseIter != mObjectInstanceList.end(); ++baseIter)
		{
			for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
				instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
			{
				if((*instanceIter)->getBoundingVolume() && (*instanceIter)->getFlagRender())
				{
					(*instanceIter)->setFlagCulled(true);
					BVs.push_back((*instanceIter)->getBoundingVolume());
				}
			}
		}

		//Mesh instances.
		for(vector<InstanceMesh *>::iterator baseIter = mMeshInstanceList.begin();
			baseIter != mMeshInstanceList.end(); ++baseIter)
		{
			for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
				instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
			{
				InstanceMesh *meshInstance = (InstanceMesh *)(*instanceIter);
				for(vector<InstanceSubset *>::const_iterator subsetIter = meshInstance->getSubsetList().begin();
					subsetIter != meshInstance->getSubsetList().end(); ++subsetIter)
				{
					if((*subsetIter)->getBoundingVolume() && (*subsetIter)->getFlagRender())
					{
						(*subsetIter)->setFlagCulled(true);
						BVs.push_back((*subsetIter)->getBoundingVolume());
					}
				}
			}
		}

		//Add all bounding volumes in BVs to the quad-tree.
		mpQuadTree->update(&BVs);

		//Cull BVs using the quad-tree.
		vector<CollisionObject *> collisionObjects;
		mpQuadTree->collisionCheck(mpFrustum, &collisionObjects);
		for(vector<CollisionObject *>::iterator BVIter = collisionObjects.begin();
			BVIter != collisionObjects.end(); ++BVIter)
		{
			RenderObject *object = (RenderObject *)(*BVIter)->getParent();
			if(object)
			{
				switch(object->getType())
				{
					case RenderTypes::BUFFER_OBJECT:
						mCulledObjects.push_back(object);
						break;

					case RenderTypes::INSTANCE_OBJECT:
						((InstanceObject *)object)->setFlagCulled(false);
						break;

					default:
						break;
				}
			}
		}

		//Add GUI sprites last.
		for(vector<InstanceSprite *>::iterator GUIIter = mGUIInstanceList.begin();
			GUIIter != mGUIInstanceList.end(); ++GUIIter)
		{
			mCulledObjects.push_back((*GUIIter));
		}
	}*/
}

RenderObject *ObjectHandler::getObject(int objectID)
{
	//Check objects.
	for(vector<RenderObject *>::iterator n = mObjectList.begin();
		n != mObjectList.end(); ++n)
	{
		if((*n)->getID() == objectID)
		{
			return (*n);
		}
	}
	//Check object instances.
	for(vector<InstanceObject *>::iterator n = mObjectInstanceList.begin();
		n != mObjectInstanceList.end(); ++n)
	{
		RenderObject *pInstance = (*n)->getInstance(objectID);
		if(pInstance)
		{
			return pInstance;
		}
	}
	//Check mesh instances.
	for(vector<InstanceMesh *>::iterator n = mMeshInstanceList.begin();
		n != mMeshInstanceList.end(); ++n)
	{
		RenderObject *pInstance = (*n)->getInstance(objectID);
		if(pInstance)
		{
			return pInstance;
		}
	}
	return NULL;
}

RenderObject *ObjectHandler::getObject(const char *objectName)
{
	//Check objects.
	for(vector<RenderObject *>::iterator n = mObjectList.begin();
		n != mObjectList.end(); ++n)
	{
		if((*n)->getName() == objectName)
		{
			return (*n);
		}
	}
	//Check object instances.
	for(vector<InstanceObject *>::iterator n = mObjectInstanceList.begin();
		n != mObjectInstanceList.end(); ++n)
	{
		RenderObject *pInstance = (*n)->getInstance(objectName);
		if(pInstance)
		{
			return pInstance;
		}
	}
	//Check mesh instances.
	for(vector<InstanceMesh *>::iterator n = mMeshInstanceList.begin();
		n != mMeshInstanceList.end(); ++n)
	{
		RenderObject *pInstance = (*n)->getInstance(objectName);
		if(pInstance)
		{
			return pInstance;
		}
	}
	return NULL;
}

void ObjectHandler::addObject(RenderObject *pObject)
{
	mObjectList.push_back(pObject);
}

void ObjectHandler::removeObject(RenderObject *pObject)
{
	mCulledObjects.clear();
	for(vector<RenderObject *>::iterator n = mObjectList.begin();
		n != mObjectList.end(); ++n)
	{
		if((*n) == pObject)
		{
			if((*n))
			{
				delete (*n);
				(*n) = NULL;
			}
			mObjectList.erase(n);
			break;
		}
	}

	//Not found. Check instances.
	removeInstance(pObject);
}

void ObjectHandler::removeObject(int objectID)
{
	mCulledObjects.clear();
	for(vector<RenderObject *>::iterator n = mObjectList.begin();
		n != mObjectList.end(); ++n)
	{
		if((*n)->getID() == objectID)
		{
			if((*n))
			{
				delete (*n);
				(*n) = NULL;
			}
			mObjectList.erase(n);
			break;
		}
	}

	//Not found. Check instances.
	removeInstance(objectID);
}

void ObjectHandler::removeObject(const char *objectName)
{
	mCulledObjects.clear();
	for(vector<RenderObject *>::iterator n = mObjectList.begin();
		n != mObjectList.end(); ++n)
	{
		if((*n)->getNameString() == objectName)
		{
			if((*n))
			{
				delete (*n);
				(*n) = NULL;
			}
			mObjectList.erase(n);
			break;
		}
	}

	//Not found. Check instances.
	removeInstance(objectName);
}

void ObjectHandler::removeAllObjects()
{
	mCulledObjects.clear();
	for(vector<RenderObject *>::iterator n = mObjectList.begin();
		n != mObjectList.end(); ++n)
	{
		if((*n))
		{
			delete (*n);
			(*n) = NULL;
		}
	}
	mObjectList.clear();
}

Mesh *ObjectHandler::addMeshFromModel(int modelID)
{
	Mesh *pNewMesh = NULL;
	for(vector<Mesh *>::iterator n = mMeshModelList.begin();
		n != mMeshModelList.end(); ++n)
	{
		if((*n)->getID() == modelID)
		{
			pNewMesh = (Mesh *)((*n)->clone());
			mObjectList.push_back(pNewMesh);
			break;
		}
	}
	return pNewMesh;
}

Mesh *ObjectHandler::addMeshFromModel(const char *modelName)
{
	Mesh *pNewMesh = NULL;
	for(vector<Mesh *>::iterator n = mMeshModelList.begin();
		n != mMeshModelList.end(); ++n)
	{
		if((*n)->getNameString() == modelName)
		{
			pNewMesh = (Mesh *)((*n)->clone());
			mObjectList.push_back(pNewMesh);
			break;
		}
	}
	return pNewMesh;
}

void ObjectHandler::addMeshModel(Mesh *pModel)
{
	mMeshModelList.push_back(pModel);
}

Mesh *ObjectHandler::getMeshModel(int modelID)
{
	for(vector<Mesh *>::iterator n = mMeshModelList.begin();
		n != mMeshModelList.end(); ++n)
	{
		if((*n)->getID() == modelID)
		{
			return (*n);
		}
	}
	return NULL;
}

Mesh *ObjectHandler::getMeshModel(const char *modelName)
{
	for(vector<Mesh *>::iterator n = mMeshModelList.begin();
		n != mMeshModelList.end(); ++n)
	{
		if((*n)->getNameString() == modelName)
		{
			return (*n);
		}
	}
	return NULL;
}

void ObjectHandler::removeMeshModel(Mesh *pModel)
{
	for(vector<Mesh *>::iterator n = mMeshModelList.begin();
		n != mMeshModelList.end(); ++n)
	{
		if((*n) == pModel)
		{
			delete (*n);
			(*n) = NULL;
			mMeshModelList.erase(n);
			break;
		}
	}
}

void ObjectHandler::removeMeshModel(int modelID)
{
	for(vector<Mesh *>::iterator n = mMeshModelList.begin();
		n != mMeshModelList.end(); ++n)
	{
		if((*n)->getID() == modelID)
		{
			delete (*n);
			(*n) = NULL;
			mMeshModelList.erase(n);
			break;
		}
	}
}

void ObjectHandler::removeMeshModel(const char *modelName)
{
	for(vector<Mesh *>::iterator n = mMeshModelList.begin();
		n != mMeshModelList.end(); ++n)
	{
		if((*n)->getNameString() == modelName)
		{
			delete (*n);
			(*n) = NULL;
			mMeshModelList.erase(n);
			break;
		}
	}
}

void ObjectHandler::removeAllMeshModels()
{
	for(vector<Mesh *>::iterator n = mMeshModelList.begin();
		n != mMeshModelList.end(); ++n)
	{
		delete (*n);
		(*n) = NULL;
	}
	mMeshModelList.clear();
}

InstanceObject *ObjectHandler::addObjectFromInstance(int instanceID)
{
	InstanceObject *pNewObject = NULL;
	for(vector<InstanceObject *>::iterator n = mObjectInstanceList.begin();
		n != mObjectInstanceList.end(); ++n)
	{
		if((*n)->getID() == instanceID)
		{
			pNewObject = (InstanceObject *)((*n)->createInstance());
			break;
		}
	}
	return pNewObject;
}

InstanceObject *ObjectHandler::addObjectFromInstance(const char *instanceName)
{
	InstanceObject *pNewObject = NULL;
	for(vector<InstanceObject *>::iterator n = mObjectInstanceList.begin();
		n != mObjectInstanceList.end(); ++n)
	{
		if((*n)->getNameString() == instanceName)
		{
			pNewObject = (InstanceObject *)((*n)->createInstance());
			break;
		}
	}
	return pNewObject;
}

void ObjectHandler::addObjectInstanceBase(InstanceObject *pInstance)
{
	mObjectInstanceList.push_back(pInstance);
}

InstanceObject *ObjectHandler::getObjectInstanceBase(int instanceID)
{
	for(vector<InstanceObject *>::iterator n = mObjectInstanceList.begin();
		n != mObjectInstanceList.end(); ++n)
	{
		if((*n)->getID() == instanceID)
		{
			return (*n);
		}
	}
	return NULL;
}

InstanceObject *ObjectHandler::getObjectInstanceBase(const char *instanceName)
{
	for(vector<InstanceObject *>::iterator n = mObjectInstanceList.begin();
		n != mObjectInstanceList.end(); ++n)
	{
		if((*n)->getNameString() == instanceName)
		{
			return (*n);
		}
	}
	return NULL;
}

void ObjectHandler::removeObjectInstanceBase(InstanceObject *pInstance)
{
	for(vector<InstanceObject *>::iterator n = mObjectInstanceList.begin();
		n != mObjectInstanceList.end(); ++n)
	{
		if((*n) == pInstance)
		{
			delete (*n);
			(*n) = NULL;
			mObjectInstanceList.erase(n);
			break;
		}
	}
}

void ObjectHandler::removeObjectInstanceBase(int instanceID)
{
	for(vector<InstanceObject *>::iterator n = mObjectInstanceList.begin();
		n != mObjectInstanceList.end(); ++n)
	{
		if((*n)->getID() == instanceID)
		{
			delete (*n);
			(*n) = NULL;
			mObjectInstanceList.erase(n);
			break;
		}
	}
}

void ObjectHandler::removeObjectInstanceBase(const char *instanceName)
{
	for(vector<InstanceObject *>::iterator n = mObjectInstanceList.begin();
		n != mObjectInstanceList.end(); ++n)
	{
		if((*n)->getNameString() == instanceName)
		{
			delete (*n);
			(*n) = NULL;
			mObjectInstanceList.erase(n);
			break;
		}
	}
}

void ObjectHandler::removeAllObjectInstanceBases()
{
	for(vector<InstanceObject *>::iterator n = mObjectInstanceList.begin();
		n != mObjectInstanceList.end(); ++n)
	{
		if((*n))
		{
			delete (*n);
			(*n) = NULL;
		}
	}
	mObjectInstanceList.clear();
}

InstanceMesh *ObjectHandler::addMeshFromInstance(int instanceID)
{
	InstanceMesh *pNewMesh = NULL;
	for(vector<InstanceMesh *>::iterator n = mMeshInstanceList.begin();
		n != mMeshInstanceList.end(); ++n)
	{
		if((*n)->getID() == instanceID)
		{
			pNewMesh = (InstanceMesh *)((*n)->createInstance());
			break;
		}
	}
	return pNewMesh;
}

InstanceMesh *ObjectHandler::addMeshFromInstance(const char *instanceName)
{
	InstanceMesh *pNewMesh = NULL;
	for(vector<InstanceMesh *>::iterator n = mMeshInstanceList.begin();
		n != mMeshInstanceList.end(); ++n)
	{
		if((*n)->getNameString() == instanceName)
		{
			pNewMesh = (InstanceMesh *)((*n)->createInstance());
			break;
		}
	}
	return pNewMesh;
}

void ObjectHandler::addMeshInstanceBase(InstanceMesh *pInstance)
{
	mMeshInstanceList.push_back(pInstance);
}

InstanceMesh *ObjectHandler::getMeshInstanceBase(int instanceID)
{
	for(vector<InstanceMesh *>::iterator n = mMeshInstanceList.begin();
		n != mMeshInstanceList.end(); ++n)
	{
		if((*n)->getID() == instanceID)
		{
			return (*n);
		}
	}
	return NULL;
}

InstanceMesh *ObjectHandler::getMeshInstanceBase(const char *instanceName)
{
	for(vector<InstanceMesh *>::iterator n = mMeshInstanceList.begin();
		n != mMeshInstanceList.end(); ++n)
	{
		if((*n)->getNameString() == instanceName)
		{
			return (*n);
		}
	}
	return NULL;
}

void ObjectHandler::removeMeshInstanceBase(InstanceMesh *pInstance)
{
	for(vector<InstanceMesh *>::iterator n = mMeshInstanceList.begin();
		n != mMeshInstanceList.end(); ++n)
	{
		if((*n) == pInstance)
		{
			delete (*n);
			(*n) = NULL;
			mMeshInstanceList.erase(n);
			break;
		}
	}
}

void ObjectHandler::removeMeshInstanceBase(int instanceID)
{
	for(vector<InstanceMesh *>::iterator n = mMeshInstanceList.begin();
		n != mMeshInstanceList.end(); ++n)
	{
		if((*n)->getID() == instanceID)
		{
			delete (*n);
			(*n) = NULL;
			mMeshInstanceList.erase(n);
			break;
		}
	}
}

void ObjectHandler::removeMeshInstanceBase(const char *instanceName)
{
	for(vector<InstanceMesh *>::iterator n = mMeshInstanceList.begin();
		n != mMeshInstanceList.end(); ++n)
	{
		if((*n)->getNameString() == instanceName)
		{
			delete (*n);
			(*n) = NULL;
			mMeshInstanceList.erase(n);
			break;
		}
	}
}

void ObjectHandler::removeAllMeshInstanceBases()
{
	for(vector<InstanceMesh *>::iterator n = mMeshInstanceList.begin();
		n != mMeshInstanceList.end(); ++n)
	{
		delete (*n);
		(*n) = NULL;
	}
	mMeshInstanceList.clear();
}

void ObjectHandler::addGUIInstanceBase(InstanceSprite *pInstance)
{
	mGUIInstanceList.push_back(pInstance);
}

InstanceSprite *ObjectHandler::getGUIInstanceBase(int instanceID)
{
	for(vector<InstanceSprite *>::iterator n = mGUIInstanceList.begin();
		n != mGUIInstanceList.end(); ++n)
	{
		if((*n)->getID() == instanceID)
		{
			return (*n);
		}
	}
	return NULL;
}

InstanceSprite *ObjectHandler::getGUIInstanceBase(const char *instanceName)
{
	for(vector<InstanceSprite *>::iterator n = mGUIInstanceList.begin();
		n != mGUIInstanceList.end(); ++n)
	{
		if((*n)->getNameString() == instanceName)
		{
			return (*n);
		}
	}
	return NULL;
}

void ObjectHandler::removeGUIInstanceBase(int instanceID)
{
	for(vector<InstanceSprite *>::iterator n = mGUIInstanceList.begin();
		n != mGUIInstanceList.end(); ++n)
	{
		if((*n)->getID() == instanceID)
		{
			delete (*n);
			(*n) = NULL;
			mGUIInstanceList.erase(n);
		}
	}
}

void ObjectHandler::removeGUIInstanceBase(const char *instanceName)
{
	for(vector<InstanceSprite *>::iterator n = mGUIInstanceList.begin();
		n != mGUIInstanceList.end(); ++n)
	{
		if((*n)->getNameString() == instanceName)
		{
			delete (*n);
			(*n) = NULL;
			mGUIInstanceList.erase(n);
		}
	}
}

void ObjectHandler::removeAllGUIInstanceBases()
{
	for(vector<InstanceSprite *>::iterator n = mGUIInstanceList.begin();
		n != mGUIInstanceList.end(); ++n)
	{
		if((*n))
		{
			delete (*n);
			(*n) = NULL;
		}
	}
	mGUIInstanceList.clear();
}

void ObjectHandler::getAllInstanceBases(std::vector<RenderObject *> &baseVector)
{
	//Add all object instance bases.
	for(vector<InstanceObject *>::iterator objectIter = mObjectInstanceList.begin();
		objectIter != mObjectInstanceList.end(); ++objectIter)
	{
		baseVector.push_back((*objectIter));
	}

	//Add all mesh instance bases.
	for(vector<InstanceMesh *>::iterator meshIter = mMeshInstanceList.begin();
		meshIter != mMeshInstanceList.end(); ++meshIter)
	{
		baseVector.push_back((*meshIter));
	}
}

void ObjectHandler::setCamera(Camera *pCamera)
{
	mpCamera = pCamera;
}

void ObjectHandler::copyBoundingVolumes(std::vector<CollisionObject *> *pBVs)
{
	//Standard objects.
	//Note: The instance base for each instance type is located in mObjectList.
	/*for(vector<RenderObject *>::iterator n = mObjectList.begin();
		n != mObjectList.end(); ++n)
	{
		if((*n)->getBoundingVolume() && (*n)->getFlagRender())
		{
			pBVs->push_back((*n)->getBoundingVolume());
		}
	}

	//Object instances.
	for(vector<InstanceObject *>::iterator baseIter = mObjectInstanceList.begin();
		baseIter != mObjectInstanceList.end(); ++baseIter)
	{
		for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
			instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
		{
			if((*instanceIter)->getBoundingVolume() && (*instanceIter)->getFlagRender() || (*instanceIter)->getBoundingVolume()->getID() == logic::KEY_TRIGGER)
			{
				pBVs->push_back((*instanceIter)->getBoundingVolume());
			}
		}
	}

	//Mesh Instances.
	for(vector<InstanceMesh *>::iterator baseIter = mMeshInstanceList.begin();
		baseIter != mMeshInstanceList.end(); ++baseIter)
	{
		for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
			instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
		{
			InstanceMesh *meshInstance = (InstanceMesh *)(*instanceIter);
			for(vector<InstanceSubset *>::const_iterator subsetIter = meshInstance->getSubsetList().begin();
				subsetIter != meshInstance->getSubsetList().end(); ++subsetIter)
			{
				if((*subsetIter)->getBoundingVolume() && (*subsetIter)->getFlagRender())
				{
					pBVs->push_back((*subsetIter)->getBoundingVolume());
				}
			}
		}
	}*/


	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX ortProjection;
	mpCamera->GetVPMatrices(view, projection, ortProjection);
	mpFrustum->update(view,projection);


	//Add all objects' bounding volumes to BVs.
	vector<CollisionObject *> BVs;

	//Object instances.
	for(vector<InstanceObject *>::iterator baseIter = mObjectInstanceList.begin();
		baseIter != mObjectInstanceList.end(); ++baseIter)
	{
		for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
			instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
		{
			if((*instanceIter)->getBoundingVolume() && (*instanceIter)->getFlagRender() || (*instanceIter)->getBoundingVolume()->getID() == logic::KEY_TRIGGER)
			{
				BVs.push_back((*instanceIter)->getBoundingVolume());
			}
		}
	}

	//Mesh instances.
	for(vector<InstanceMesh *>::iterator baseIter = mMeshInstanceList.begin();
		baseIter != mMeshInstanceList.end(); ++baseIter)
	{
		for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
			instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
		{
			InstanceMesh *meshInstance = (InstanceMesh *)(*instanceIter);
			for(vector<InstanceSubset *>::const_iterator subsetIter = meshInstance->getSubsetList().begin();
				subsetIter != meshInstance->getSubsetList().end(); ++subsetIter)
			{
				if((*subsetIter)->getBoundingVolume() && (*subsetIter)->getFlagRender())
				{
					BVs.push_back((*subsetIter)->getBoundingVolume());
				}
			}
		}
	}

	//Add all bounding volumes in BVs to the quad-tree.
	mpQuadTree->update(&BVs);

	//Cull BVs using the quad-tree.
	vector<CollisionObject *> collisionObjects;
	mpQuadTree->collisionCheck(mpFrustum, pBVs);
}

void ObjectHandler::removeInstance(RenderObject *pInstance)
{
	//Instanced objects.
	for(vector<InstanceObject *>::iterator baseIter = mObjectInstanceList.begin();
		baseIter != mObjectInstanceList.end(); ++baseIter)
	{
		for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
			instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
		{
			if((*instanceIter) == pInstance)
			{
				(*baseIter)->removeInstance((InstanceObject *)pInstance);
				return;
			}
		}
	}

	//Instanced meshes.
	for(vector<InstanceMesh *>::iterator baseIter = mMeshInstanceList.begin();
		baseIter != mMeshInstanceList.end(); ++baseIter)
	{
		for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
			instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
		{
			if((*instanceIter) == pInstance)
			{
				(*baseIter)->removeInstance((InstanceObject *)pInstance);
				return;
			}
		}
	}
}

void ObjectHandler::removeInstance(int instanceID)
{
	//Instanced objects.
	for(vector<InstanceObject *>::iterator baseIter = mObjectInstanceList.begin();
		baseIter != mObjectInstanceList.end(); ++baseIter)
	{
		for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
			instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
		{
			if((*instanceIter)->getID() == instanceID)
			{
				(*baseIter)->removeInstance(instanceID);
				break;
			}
		}
	}

	//Instanced meshes.
	for(vector<InstanceMesh *>::iterator baseIter = mMeshInstanceList.begin();
		baseIter != mMeshInstanceList.end(); ++baseIter)
	{
		for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
			instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
		{
			if((*instanceIter)->getID() == instanceID)
			{
				(*baseIter)->removeInstance(instanceID);
				break;
			}
		}
	}
}

void ObjectHandler::removeInstance(const char *instanceName)
{
	//Instanced objects.
	for(vector<InstanceObject *>::iterator baseIter = mObjectInstanceList.begin();
		baseIter != mObjectInstanceList.end(); ++baseIter)
	{
		for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
			instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
		{
			if((*instanceIter)->getNameString() == instanceName)
			{
				(*baseIter)->removeInstance(instanceName);
				break;
			}
		}
	}

	//Instanced meshes.
	for(vector<InstanceMesh *>::iterator baseIter = mMeshInstanceList.begin();
		baseIter != mMeshInstanceList.end(); ++baseIter)
	{
		for(vector<InstanceObject *>::const_iterator instanceIter = (*baseIter)->getInstanceList().begin();
			instanceIter != (*baseIter)->getInstanceList().end(); ++instanceIter)
		{
			if((*instanceIter)->getNameString() == instanceName)
			{
				(*baseIter)->removeInstance(instanceName);
				break;
			}
		}
	}
}