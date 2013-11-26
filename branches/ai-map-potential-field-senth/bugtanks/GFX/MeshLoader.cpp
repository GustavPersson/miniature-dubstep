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

#include "MeshLoader.h"
#include "OBB.h"

using namespace gfx;
using std::fstream;
using std::ios_base;
using std::string;
using std::vector;

MeshLoader::MeshLoader(ID3D10Device *pDevice, Effect *pEffect) : mGeometryFile(),
	mMaterialFile(), mAnimationFile(), mWeightFile()
{
	mpDevice = pDevice;
	mpEffect = pEffect;
}

MeshLoader::MeshLoader(const MeshLoader &loader) : mGeometryFile(),
	mMaterialFile(), mAnimationFile(), mWeightFile()
{
	mpDevice = loader.mpDevice;
	mpEffect = loader.mpEffect;
}

MeshLoader::~MeshLoader()
{
	closeAllFiles();
	clearMaterialList();
	clearMeshList();
}

const MeshLoader &MeshLoader::operator=(const MeshLoader &loader)
{
	if(this != &loader)
	{

	}
	return *this;
}

bool MeshLoader::openGeometryFile(const char *filePath)
{
	if(mGeometryFile.mFile.is_open())
	{
		mGeometryFile.mFile.close();
	}

	mGeometryFile.mLocation = 0;

	//Open file in binary mode.
	mGeometryFile.mFile.open(filePath, ios_base::in | ios_base::binary);
	if(mGeometryFile.mFile.is_open())
	{
		return true;
	}
	return false;
}

bool MeshLoader::openMaterialFile(const char *filePath)
{
	if(mMaterialFile.mFile.is_open())
	{
		mMaterialFile.mFile.close();
	}

	mMaterialFile.mLocation = 0;

	//Open file in binary mode.
	mMaterialFile.mFile.open(filePath, ios_base::in | ios_base::binary);
	if(mMaterialFile.mFile.is_open())
	{
		return true;
	}
	return false;
}

bool MeshLoader::openAnimationFile(const char *filePath)
{
	if(mAnimationFile.mFile.is_open())
	{
		mAnimationFile.mFile.close();
	}

	mAnimationFile.mLocation = 0;

	//Open file in binary mode.
	mAnimationFile.mFile.open(filePath, ios_base::in | ios_base::binary);
	if(mAnimationFile.mFile.is_open())
	{
		return true;
	}
	return false;
}

bool MeshLoader::openWeightFile(const char *filePath)
{
	if(mWeightFile.mFile.is_open())
	{
		mWeightFile.mFile.close();
	}

	mWeightFile.mLocation = 0;

	//Open file in binary mode.
	mWeightFile.mFile.open(filePath, ios_base::in | ios_base::binary);
	if(mWeightFile.mFile.is_open())
	{
		return true;
	}
	return false;
}

InstanceMesh *MeshLoader::importInstanceMesh()
{
	//Import materials.
	if(!importMaterial())
	{
		ERROR_MESSAGE("Failed to import InstanceMesh.")
		return NULL;
	}

	//Import geometry.
	if(!importGeometry())
	{
		ERROR_MESSAGE("Failed to import InstanceMesh.")
		return NULL;
	}

	//Calculate adjacent indices for each polygon.
	if(!calculateAdjacentIndices())
	{
		ERROR_MESSAGE("Failed to import InstanceMesh.")
		return NULL;
	}

	//Calculate averaged vertex normals.
	if(!calculateVertexNormals())
	{
		ERROR_MESSAGE("Failed to import InstanceMesh.")
		return NULL;
	}

	InstanceMesh *pMesh = myNew InstanceMesh(mpDevice, mpEffect);
	pMesh->setFlagInstanceBase(true);

	for(vector<Mesh *>::iterator meshIter = mMeshList.begin();
		meshIter != mMeshList.end(); ++meshIter)
	{
		//Create buffers.
		ID3D10Buffer *pVertexBufferStatic = NULL;
		ID3D10Buffer *pIndexBuffer = NULL;
		if(!createVertexBufferStatic((*meshIter), &pVertexBufferStatic))
		{
			return NULL;
		}
		if(!createIndexBuffer((*meshIter), &pIndexBuffer))
		{
			return NULL;
		}

		//Create subset.
		InstanceSubset *pSubset = myNew InstanceSubset(mpDevice, mpEffect);
		pSubset->setFlagInstanceBase(true);
		pSubset->loadBuffers((*meshIter)->mIndexCount, pVertexBufferStatic, (*meshIter)->mIndexCount, pIndexBuffer);

		//Create bounding volume.
		CollisionObject *pBV = createBoundingVolumeOBB((*meshIter));
		pSubset->setBoundingVolume(pBV);

		pMesh->addSubset(pSubset);
	}

	pMesh->setInstanceCountMax(1);

	return pMesh;
}

InstanceAnimationMesh *MeshLoader::importInstanceAnimationMesh()
{
	//Import materials.
	if(!importMaterial())
	{
		ERROR_MESSAGE("Failed to import InstanceAnimationMesh.")
		return NULL;
	}

	//Import geometry.
	if(!importGeometry())
	{
		ERROR_MESSAGE("Failed to import InstanceAnimationMesh.")
		return NULL;
	}

	//Import weights.
	if(!importWeights())
	{
		ERROR_MESSAGE("Failed to import InstanceAnimationMesh.")
		return NULL;
	}

	//Calculate adjacent indices for each polygon.
	if(!calculateAdjacentIndices())
	{
		ERROR_MESSAGE("Failed to import InstanceAnimationMesh.")
		return NULL;
	}

	//Calculate averaged vertex normals.
	if(!calculateVertexNormals())
	{
		ERROR_MESSAGE("Failed to import InstanceAnimationMesh.")
		return NULL;
	}

	InstanceAnimationMesh *pMesh = myNew InstanceAnimationMesh(mpDevice, mpEffect);
	pMesh->setFlagInstanceBase(true);

	for(vector<Mesh *>::iterator meshIter = mMeshList.begin();
		meshIter != mMeshList.end(); ++meshIter)
	{
		//Create buffers.
		ID3D10Buffer *pVertexBufferStatic = NULL;
		ID3D10Buffer *pIndexBuffer = NULL;
		if(!createAnimationVertexBufferStatic((*meshIter), &pVertexBufferStatic))
		{
			return NULL;
		}
		if(!createIndexBuffer((*meshIter), &pIndexBuffer))
		{
			return NULL;
		}

		//Create subset.
		InstanceAnimationSubset *pSubset = myNew InstanceAnimationSubset(mpDevice, mpEffect);
		pSubset->setFlagInstanceBase(true);
		pSubset->loadBuffers((*meshIter)->mIndexCount, pVertexBufferStatic, (*meshIter)->mIndexCount, pIndexBuffer);

		//Create bounding volume.
		CollisionObject *pBV = createBoundingVolumeOBB((*meshIter));
		pSubset->setBoundingVolume(pBV);

		pMesh->addSubset(pSubset);
	}

	pMesh->setInstanceCountMax(1);

	return pMesh;
}

Animation *MeshLoader::importAnimation()
{
	vector<Joint> jointVector;

	//Read number of joints.
	unsigned int jointCount = 0;
	read(mAnimationFile, (char *)&jointCount, sizeof(unsigned int));

	//Allocate joint data.
	Joint *pJoints = myNew Joint[jointCount];
	Joint *pRoot = NULL;

	//Read joints in bind pose.
	for(unsigned int nJoint = 0; nJoint < jointCount; ++nJoint)
	{
		//Read joint ID.
		int jointID = 0;
		read(mAnimationFile, (char *)&jointID, sizeof(int));

		//Read parent ID.
		int parentID = 0;
		read(mAnimationFile, (char *)&parentID, sizeof(int));

		if(parentID >= 0)
		{
			pJoints[parentID].addChild(&pJoints[nJoint]);
		}
		else
		{
			//Root found.
			pRoot = &pJoints[nJoint];
		}

		//Read translation. Invert Z-axis.
		D3DXVECTOR3 translation;
		read(mAnimationFile, (char *)&translation, sizeof(D3DXVECTOR3));
		translation.z *= -1;
		pJoints[nJoint].setPosition(translation);

		//Read rotation. Invert Z-axis and angle.
		D3DXQUATERNION rotation;
		read(mAnimationFile, (char *)&rotation, sizeof(D3DXQUATERNION));
		rotation.z *= -1;
		rotation.w *= -1;
		pJoints[nJoint].setRotation(rotation);

		//Read orientation. Invert Z-axis and angle.
		D3DXQUATERNION orientation;
		read(mAnimationFile, (char *)&orientation, sizeof(D3DXQUATERNION));
		orientation.z *= -1;
		orientation.w *= -1;
		pJoints[nJoint].setOrientation(orientation);
	}

	if(!pRoot)
	{
		ERROR_MESSAGE("Joint root not found.")
		return NULL;
	}

	//Set bind pose for the joints.
	pRoot->update(true);
	pRoot->setBindPose();

	//Read animation name.
	string name = "";
	readText(mAnimationFile, name);

	Animation *pAnimation = myNew Animation(mpDevice, name);

	//Read start time.
	float startTime = 0;
	read(mAnimationFile, (char *)&startTime, sizeof(float));

	//Read end time.
	float endTime = 0;
	read(mAnimationFile, (char *)&endTime, sizeof(float));

	//Read number of frames.
	unsigned int frameCount = 0;
	read(mAnimationFile, (char *)&frameCount, sizeof(unsigned int));

	for(unsigned int nFrame = 0; nFrame < frameCount; ++nFrame)
	{
		//Read frame time.
		float time = 0;
		read(mAnimationFile, (char *)&time, sizeof(float));

		//Read new joint transforms.
		for(unsigned int nJoint = 0; nJoint < jointCount; ++nJoint)
		{
			//Read joint ID.
			int jointID = 0;
			read(mAnimationFile, (char *)&jointID, sizeof(int));

			//Read parent ID.
			int parentID = 0;
			read(mAnimationFile, (char *)&parentID, sizeof(int));

			//Read translation. Invert Z-axis.
			D3DXVECTOR3 translation;
			read(mAnimationFile, (char *)&translation, sizeof(D3DXVECTOR3));
			translation.z *= -1;
			pJoints[nJoint].setPosition(translation);

			//Read rotation. Invert Z-axis and angle.
			D3DXQUATERNION rotation;
			read(mAnimationFile, (char *)&rotation, sizeof(D3DXQUATERNION));
			rotation.z *= -1;
			rotation.w *= -1;
			pJoints[nJoint].setRotation(rotation);

			//Read orientation. Invert Z-axis and angle.
			D3DXQUATERNION orientation;
			read(mAnimationFile, (char *)&orientation, sizeof(D3DXQUATERNION));
			orientation.z *= -1;
			orientation.w *= -1;
			pJoints[nJoint].setOrientation(orientation);
		}

		//Update joint root.
		pRoot->update(true);

		//Create a new frame and calculate its matrix.
		Frame *pFrame = myNew Frame();
		for(unsigned int nJoint = 0; nJoint < jointCount; ++nJoint)
		{
			D3DXMATRIX matrix = pJoints[nJoint].getBindInverse() * pJoints[nJoint].getToWorld();
			pFrame->addMatrix(matrix);
		}
		pAnimation->addFrame(pFrame);
	}

	//Deallocate joint data.
	delete [] pJoints;

	return pAnimation;
}

bool MeshLoader::readText(FileReader &file, std::string &text)
{
	if(file.mFile.is_open())
	{
		//Set location of the get pointer.
		file.mFile.seekg(file.mLocation);

		//Read text size.
		unsigned int textSize = 0;
		file.mFile.read((char *)&textSize, sizeof(unsigned int));

		//Read text.
		char *pText = myNew char[textSize + 1];
		file.mFile.read(pText, textSize);

		//Append null termination.
		pText[textSize] = 0;

		//Increase file location.
		file.mLocation += sizeof(unsigned int) + textSize;

		text = pText;

		delete [] pText;

		return true;
	}

	ERROR_MESSAGE("Could not read text data. File not open.")
	return false;
}

bool MeshLoader::read(FileReader &file, char *pData, unsigned int dataSize)
{
	if(file.mFile.is_open())
	{
		//Set location of the get pointer.
		file.mFile.seekg(file.mLocation);

		//Read data block.
		file.mFile.read(pData, dataSize);

		//Increase file location.
		file.mLocation += dataSize;

		return true;
	}

	ERROR_MESSAGE("Could not read data block. File not open.")
	return false;
}

void MeshLoader::closeAllFiles()
{
	//Close all open files.
	if(mGeometryFile.mFile.is_open())
	{
		mGeometryFile.mFile.close();
		mGeometryFile.mLocation = 0;
	}
	if(mMaterialFile.mFile.is_open())
	{
		mMaterialFile.mFile.close();
		mMaterialFile.mLocation = 0;
	}
	if(mAnimationFile.mFile.is_open())
	{
		mAnimationFile.mFile.close();
		mAnimationFile.mLocation = 0;
	}
	if(mWeightFile.mFile.is_open())
	{
		mWeightFile.mFile.close();
		mWeightFile.mLocation = 0;
	}
}

bool MeshLoader::importMaterial()
{
	clearMaterialList();
	if(!mMaterialFile.mFile.is_open())
	{
		ERROR_MESSAGE("Material file not open. Please open the required files before importing a model.")
		return false;
	}

	//Read the number of materials.
	unsigned int materialCount = 0;
	read(mMaterialFile, (char *)&materialCount, sizeof(materialCount));

	//Read materials.
	for(unsigned int nMaterial = 0; nMaterial < materialCount; ++nMaterial)
	{
		Material *pMaterial = myNew Material();

		//Read ambient.
		read(mMaterialFile, (char *)&pMaterial->mAmbient, sizeof(D3DXCOLOR));

		//Read diffuse.
		read(mMaterialFile, (char *)&pMaterial->mDiffuse, sizeof(D3DXCOLOR));

		//Read specular.
		read(mMaterialFile, (char *)&pMaterial->mSpecular, sizeof(D3DXCOLOR));

		mMaterialList.push_back(pMaterial);
	}

	return true;
}

bool MeshLoader::importGeometry()
{
	clearMeshList();
	if(!mGeometryFile.mFile.is_open())
	{
		ERROR_MESSAGE("Geometry file not open. Please open the required files before importing a model.")
		return false;
	}

	//Read the number of meshes.
	unsigned int meshCount = 0;
	read(mGeometryFile, (char *)&meshCount, sizeof(unsigned int));

	//Read meshes.
	for(unsigned int nMesh = 0; nMesh < meshCount; ++nMesh)
	{
		Mesh *pMesh = myNew Mesh();

		//Read name.
		string name = "";
		readText(mGeometryFile, name);

		//Read number of vertices.
		read(mGeometryFile, (char *)&pMesh->mVertexCount, sizeof(int));

		//Read number of polygons.
		read(mGeometryFile, (char *)&pMesh->mPolygonCount, sizeof(int));

		//Index count is PolygonCount * 3.
		pMesh->mIndexCount = pMesh->mPolygonCount * 3;

		//Mesh indices go from 0 to mIndexCount.
		for(int nIndex = 0; nIndex < pMesh->mIndexCount; ++nIndex)
		{
			pMesh->mIndices.push_back(nIndex);
		}

		//Read vertices.
		for(int nVertex = 0; nVertex < pMesh->mVertexCount; ++nVertex)
		{
			Vertex vertex;

			//Read vertex position. Only read three elements.
			read(mGeometryFile, (char *)&vertex.mPosition, sizeof(D3DXVECTOR3));
			vertex.mPosition.w = 1.0f;
			//Invert Z.
			vertex.mPosition.z *= -1;

			pMesh->mVertices.push_back(vertex);
		}

		//Read polygons.
		for(int nPolygon = 0; nPolygon < pMesh->mPolygonCount; ++nPolygon)
		{
			Polygon polygon;
			
			//Read polygon's indices.
			read(mGeometryFile, (char *)polygon.mIndices, sizeof(int) * 3);

			//Read polygon normal. Only read three elements.
			read(mGeometryFile, (char *)&polygon.mNormal, sizeof(D3DXVECTOR3));
			polygon.mNormal.w = 0.0f;
			//Invert Z.
			polygon.mNormal.z *= -1;

			//Read polygon tangent. Only read three elements.
			read(mGeometryFile, (char *)&polygon.mTangent, sizeof(D3DXVECTOR3));
			polygon.mTangent.w = 0.0f;
			//Invert Z.
			polygon.mTangent.z *= -1;

			//Read UV coordinates for the polygon's three vertices.
			read(mGeometryFile, (char *)polygon.mUV, sizeof(D3DXVECTOR2) * 3);

			//Read material index.
			read(mGeometryFile, (char *)&polygon.mMaterialIndex, sizeof(int));

			pMesh->mPolygons.push_back(polygon);
		}

		mMeshList.push_back(pMesh);
	}

	return true;
}

bool MeshLoader::importWeights()
{
	//Read number of joints.
	unsigned int jointCount = 0;
	read(mWeightFile, (char *)&jointCount, sizeof(unsigned int));

	//Read number of meshes.
	unsigned int meshCount = 0;
	read(mWeightFile, (char *)&meshCount, sizeof(unsigned int));

	//Note: Animated files only support one mesh at the moment.
	if(meshCount > 1)
	{
		ERROR_MESSAGE("Only a single mesh/subset is supported for animated models at the moment.")
		return false;
	}

	for(vector<Mesh *>::iterator meshIter = mMeshList.begin();
		meshIter != mMeshList.end(); ++meshIter)
	{
		Mesh *pMesh = (*meshIter);
		pMesh->mIndexCount;

		//Read number of vertices.
		unsigned int vertexCount = 0;
		read(mWeightFile, (char *)&vertexCount, sizeof(unsigned int));

		//The read vertex count should be the same as in the stored mesh.
		if(vertexCount != pMesh->mVertices.size())
		{
			ERROR_MESSAGE("Vertex count in weight file and stored mesh are not equal.")
			return false;
		}

		//Weights are stored on a joints-per-vertex basis.
		for(unsigned int nVertex = 0; nVertex < vertexCount; ++nVertex)
		{
			//Read vertex ID.
			int vertexID = 0;
			read(mWeightFile, (char *)&vertexID, sizeof(int));

			D3DXVECTOR4 joints(0.0f, 0.0f, 0.0f, 0.0f);
			D3DXVECTOR4 weights(0.01f, 0.01f, 0.01f, 0.01f);
			for(unsigned int nJoint = 0; nJoint < jointCount; ++nJoint)
			{
				//Read joint ID.
				int jointID = 0;
				read(mWeightFile, (char *)&jointID, sizeof(int));

				//Read weight.
				float weight = 0.0f;
				read(mWeightFile, (char *)&weight, sizeof(float));

				//Add the weight if it's greater than any of the lowest existing weight.
				if(weight > 0.1f)
				{
					int lowestIndex = 0;
					float lowestValue = 2.0f;
					for(int n = 0; n < 4; ++n)
					{
						if(weights[n] < lowestValue)
						{
							lowestIndex = n;
							lowestValue = weights[n];
						}
					}

					if(weight > lowestValue)
					{
						joints[lowestIndex] = (float)jointID + 0.01f;
						weights[lowestIndex] = weight;
					}
				}
			}

			pMesh->mVertices[nVertex].mJoints = joints;
			pMesh->mVertices[nVertex].mWeights = weights;
		}

	}
	return true;
}

void MeshLoader::clearMaterialList()
{
	for(vector<Material *>::iterator materialIter = mMaterialList.begin();
		materialIter != mMaterialList.end(); ++materialIter)
	{
		delete (*materialIter);
		(*materialIter) = NULL;
	}
	mMaterialList.clear();
}

void MeshLoader::clearMeshList()
{
	for(vector<Mesh *>::iterator meshIter = mMeshList.begin();
		meshIter != mMeshList.end(); ++meshIter)
	{
		delete (*meshIter);
		(*meshIter) = NULL;
	}
	mMeshList.clear();
}

bool MeshLoader::getMaterialAmbient(int materialIndex, D3DXCOLOR &ambient)
{
	if(materialIndex >= 0 && materialIndex < (int)mMaterialList.size())
	{
		ambient = mMaterialList[materialIndex]->mAmbient;
		return true;
	}

	//Return white as default color.
	ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	return false;
}

bool MeshLoader::getMaterialDiffuse(int materialIndex, D3DXCOLOR &diffuse)
{
	if(materialIndex >= 0 && materialIndex < (int)mMaterialList.size())
	{
		diffuse = mMaterialList[materialIndex]->mDiffuse;
		return true;
	}

	//Return white as default color.
	diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	return false;
}

bool MeshLoader::getMaterialSpecular(int materialIndex, D3DXCOLOR &specular)
{
	if(materialIndex >= 0 && materialIndex < (int)mMaterialList.size())
	{
		specular = mMaterialList[materialIndex]->mSpecular;
		return true;
	}

	//Return white as default color.
	specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	return false;
}

bool MeshLoader::calculateAdjacentIndices()
{
	for(vector<Mesh *>::iterator meshIter = mMeshList.begin();
		meshIter != mMeshList.end(); ++meshIter)
	{
		Mesh *pMesh = (*meshIter);
		//Go through each polygon.
		for(int nPolygonI = 0; nPolygonI < pMesh->mPolygonCount; ++nPolygonI)
		{

			//Go through each polygon except the current one.
			for(int nPolygonJ = 0; nPolygonJ < pMesh->mPolygonCount; ++nPolygonJ)
			{
				if(nPolygonI == nPolygonJ)
				{
					continue;
				}

				//Check the two polygons for shared indices.
				int shareCount = 0;
				int sharedIndex[2] = {0};
				for(int nIndexI = 0; nIndexI < 3; ++nIndexI)
				{
					for(int nIndexJ = 0; nIndexJ < 3; ++nIndexJ)
					{
						if(pMesh->mPolygons[nPolygonI].mIndices[nIndexI] ==
							pMesh->mPolygons[nPolygonJ].mIndices[nIndexJ])
						{
							sharedIndex[shareCount] = nIndexI;
							++shareCount;
						}
					}
				}

				//Two shared indices means that the polygons are adjacent.
				if(shareCount == 2)
				{
					int adjacentIndex = 0;
					unsigned int adjacentValue = 0;

					//Find adjacent index.
					if(sharedIndex[0] == 0 && sharedIndex[1] == 1)
					{
						adjacentIndex = 0;
					}
					else if(sharedIndex[0] == 1 && sharedIndex[1] == 2)
					{
						adjacentIndex = 1;
					}
					else if(sharedIndex[0] == 0 && sharedIndex[1] == 2)
					{
						adjacentIndex = 2;
					}

					//Find adjacent value.
					for(int nIndex = 0; nIndex < 3; ++nIndex)
					{
						//Check for excluded index.
						if(pMesh->mPolygons[nPolygonJ].mIndices[nIndex] !=
							pMesh->mPolygons[nPolygonI].mIndices[sharedIndex[0]] &&
							pMesh->mPolygons[nPolygonJ].mIndices[nIndex] !=
							pMesh->mPolygons[nPolygonI].mIndices[sharedIndex[1]])
						{
							adjacentValue = nPolygonJ * 3 + nIndex;// pMesh->mPolygons[nPolygonJ].mIndices[nIndex];
							break;
						}
					}
					pMesh->mPolygons[nPolygonI].mAdjacent[adjacentIndex] = adjacentValue;
				}

			}
		}
	}
	return true;
}

bool MeshLoader::calculateVertexNormals()
{
	for(vector<Mesh *>::iterator meshIter = mMeshList.begin();
		meshIter != mMeshList.end(); ++meshIter)
	{
		Mesh *pMesh = (*meshIter);

		//Go through each vertex.
		for(int nVertex = 0; nVertex < pMesh->mVertexCount; ++nVertex)
		{
			D3DXVECTOR4 averagedNormal(0.0f, 0.0f, 0.0f, 0.0f);
			float maxY = 0.0f;

			//Go through each polygon.
			for(int nPolygon = 0; nPolygon < pMesh->mPolygonCount; ++nPolygon)
			{
				//Add the polygon's normal if it contains the vertex.
				for(int nIndex = 0; nIndex < 3; ++nIndex)
				{
					if(pMesh->mPolygons[nPolygon].mIndices[nIndex] == (unsigned int)nVertex)
					{
						if(pMesh->mPolygons[nPolygon].mNormal.y > maxY)
						{
							maxY = pMesh->mPolygons[nPolygon].mNormal.y;
						}
						averagedNormal += pMesh->mPolygons[nPolygon].mNormal;
						break;
					}
				}
			}

			if(averagedNormal.y < maxY)
			{
				if(maxY > 0.1f)
				{
					averagedNormal.y = maxY;
				}
			}

			D3DXVec4Normalize(&averagedNormal, &averagedNormal);
			pMesh->mVertices[nVertex].mNormal = averagedNormal;

			//Tangent is not used.
			pMesh->mVertices[nVertex].mTangent = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
	return true;
}

bool MeshLoader::createVertexBufferStatic(Mesh *pMesh, ID3D10Buffer **pVertexBuffer)
{
	//Allocate memory.
	VertexTransformStatic *pVertices = myNew VertexTransformStatic[pMesh->mIndexCount];

	//Copy information from vertices stored by vertex-per-polygon.
	for(int nPolygon = 0; nPolygon < pMesh->mPolygonCount; ++nPolygon)
	{
		Polygon *pPolygon = &pMesh->mPolygons[nPolygon];

		for(int nVertex = 0; nVertex < 3; ++nVertex)
		{
			//Use per-vertex positions/normals/tangents.
			pVertices[nPolygon * 3 + nVertex].position = (const float *)pMesh->mVertices[pPolygon->mIndices[nVertex]].mPosition;
			pVertices[nPolygon * 3 + nVertex].normal = (const float *)pMesh->mVertices[pPolygon->mIndices[nVertex]].mNormal;
			pVertices[nPolygon * 3 + nVertex].tangent = (const float *)pMesh->mVertices[pPolygon->mIndices[nVertex]].mTangent;

			pVertices[nPolygon * 3 + nVertex].UV = (const float *)pPolygon->mUV[nVertex];
			
			//Materials.
			D3DXCOLOR material;
			getMaterialDiffuse(pPolygon->mMaterialIndex, material);
			pVertices[nPolygon * 3 + nVertex].diffuse = (const float *)material;

			getMaterialSpecular(pPolygon->mMaterialIndex, material);
			pVertices[nPolygon * 3 + nVertex].specular = (const float *)material;
		}
	}

	//Create vertex buffer.
	D3D10_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTransformStatic) * pMesh->mIndexCount;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = pVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if(FAILED(mpDevice->CreateBuffer(&vertexBufferDesc, &vertexData, pVertexBuffer)))
	{
		delete [] pVertices;
		ERROR_MESSAGE("Failed to create static vertex buffer.")
		return false;
	}
	delete [] pVertices;
	return true;
}

bool MeshLoader::createAnimationVertexBufferStatic(Mesh *pMesh, ID3D10Buffer **pVertexBuffer)
{
	//Allocate memory.
	VertexAnimationStatic *pVertices = myNew VertexAnimationStatic[pMesh->mIndexCount];

	//Copy information from vertices stored by vertex-per-polygon.
	for(int nPolygon = 0; nPolygon < pMesh->mPolygonCount; ++nPolygon)
	{
		Polygon *pPolygon = &pMesh->mPolygons[nPolygon];

		for(int nVertex = 0; nVertex < 3; ++nVertex)
		{
			//Use per-vertex positions/normals/tangents.
			pVertices[nPolygon * 3 + nVertex].position = (const float *)pMesh->mVertices[pPolygon->mIndices[nVertex]].mPosition;
			pVertices[nPolygon * 3 + nVertex].normal = (const float *)pMesh->mVertices[pPolygon->mIndices[nVertex]].mNormal;
			pVertices[nPolygon * 3 + nVertex].tangent = (const float *)pMesh->mVertices[pPolygon->mIndices[nVertex]].mTangent;

			pVertices[nPolygon * 3 + nVertex].UV = (const float *)pPolygon->mUV[nVertex];

			//Joint IDs and weights.
			pVertices[nPolygon * 3 + nVertex].joints = (const float *)pMesh->mVertices[pPolygon->mIndices[nVertex]].mJoints;
			pVertices[nPolygon * 3 + nVertex].weights = (const float *)pMesh->mVertices[pPolygon->mIndices[nVertex]].mWeights;
			
			//Materials.
			D3DXCOLOR material;
			getMaterialDiffuse(pPolygon->mMaterialIndex, material);
			pVertices[nPolygon * 3 + nVertex].diffuse = (const float *)material;

			getMaterialSpecular(pPolygon->mMaterialIndex, material);
			pVertices[nPolygon * 3 + nVertex].specular = (const float *)material;
		}
	}

	//Create vertex buffer.
	D3D10_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexAnimationStatic) * pMesh->mIndexCount;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = pVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if(FAILED(mpDevice->CreateBuffer(&vertexBufferDesc, &vertexData, pVertexBuffer)))
	{
		delete [] pVertices;
		ERROR_MESSAGE("Failed to create static vertex buffer.")
		return false;
	}
	delete [] pVertices;
	return true;
}

bool MeshLoader::createIndexBuffer(Mesh *pMesh, ID3D10Buffer **pIndexBuffer)
{
	//Allocate memory for indices.
	unsigned int *pIndices = myNew unsigned int[pMesh->mIndexCount];
	for(int nPolygon = 0; nPolygon < pMesh->mPolygonCount; ++nPolygon)
	{
		pIndices[nPolygon * 3] = nPolygon * 3 + 2;
		pIndices[nPolygon * 3 + 1] = nPolygon * 3 + 1;
		pIndices[nPolygon * 3 + 2] = nPolygon * 3;
	}

	D3D10_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * pMesh->mIndexCount;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if(FAILED(mpDevice->CreateBuffer(&indexBufferDesc, &indexData, pIndexBuffer)))
	{
		delete [] pIndices;
		ERROR_MESSAGE("Failed to create index buffer.");
		return false;
	}
	delete [] pIndices;
	return true;
}

bool MeshLoader::createIndexBufferAdjacent(Mesh *pMesh, ID3D10Buffer **pIndexBuffer)
{
	//Allocate memory for indices and adjacent information.
	unsigned int *pIndices = myNew unsigned int[pMesh->mIndexCount * 2];
	for(int nPolygon = 0; nPolygon < pMesh->mPolygonCount; ++nPolygon)
	{
		Polygon *pPolygon = &pMesh->mPolygons[nPolygon];

		pIndices[nPolygon * 6] = nPolygon * 3 + 2;
		pIndices[nPolygon * 6 + 1] = pPolygon->mAdjacent[1];
		pIndices[nPolygon * 6 + 2] = nPolygon * 3 + 1;
		pIndices[nPolygon * 6 + 3] = pPolygon->mAdjacent[0];
		pIndices[nPolygon * 6 + 4] = nPolygon * 3;
		pIndices[nPolygon * 6 + 5] = pPolygon->mAdjacent[2];
	}

	D3D10_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * pMesh->mIndexCount * 2;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if(FAILED(mpDevice->CreateBuffer(&indexBufferDesc, &indexData, pIndexBuffer)))
	{
		delete [] pIndices;
		ERROR_MESSAGE("Failed to create index buffer.");
		return false;
	}
	delete [] pIndices;
	return true;
}

CollisionObject *MeshLoader::createBoundingVolumeOBB(Mesh *pMesh)
{
	CollisionObject *pBV = NULL;

	//Set min/max values.
	float minX = FLT_MAX;
	float maxX = -FLT_MAX;
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	float minZ = FLT_MAX;
	float maxZ = -FLT_MAX;
	D3DXVECTOR4 position;

	for(int nVertex = 0; nVertex < pMesh->mVertexCount; ++nVertex)
	{
		position = pMesh->mVertices[nVertex].mPosition;
		//Find the corners of the mesh.
		if(position.x < minX)
		{
			minX = position.x;
		}
		else if(position.x > maxX)
		{
			maxX = position.x;
		}
		if(position.y < minY)
		{
			minY = position.y;
		}
		else if(position.y > maxY)
		{
			maxY = position.y;
		}
		if(position.z < minZ)
		{
			minZ = position.z;
		}
		else if(position.z > maxZ)
		{
			maxZ = position.z;
		}
	}
	pBV = myNew OBB(minX, minY, minZ, maxX, maxY, maxZ, -1);
	pBV->setParent(NULL);

	return pBV;
}

CollisionObject *MeshLoader::createBoundingVolumeSphere(Mesh *pMesh)
{
	CollisionObject *pBV = NULL;

	//Set min/max values.
	float distance = 0.0f;
	float maxRadius = -FLT_MAX;
	float minX = FLT_MAX;
	float maxX = -FLT_MAX;
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	float minZ = FLT_MAX;
	float maxZ = -FLT_MAX;
	D3DXVECTOR4 position;
	D3DXVECTOR4 length;

	//Find the min/max vertex positions of the mesh.
	for(int nVertex = 0; nVertex < pMesh->mVertexCount; ++nVertex)
	{
		position = pMesh->mVertices[nVertex].mPosition;

		if(position.x < minX)
		{
			minX = position.x;
		}
		else if(position.x > maxX)
		{
			maxX = position.x;
		}
		if(position.y < minY)
		{
			minY = position.y;
		}
		else if(position.y > maxY)
		{
			maxY = position.y;
		}
		if(position.z < minZ)
		{
			minZ = position.z;
		}
		else if(position.z > maxZ)
		{
			maxZ = position.z;
		}
	}

	D3DXVECTOR4 center;
	center.x = (minX + maxX) * 0.5f;
	center.y = (minY + maxY) * 0.5f;
	center.z = (minZ + maxZ) * 0.5f;
	center.w = 1.0f;
	
	//Find the length to the farthest corner.
	for(int nVertex = 0; nVertex < pMesh->mVertexCount; ++nVertex)
	{
		position = pMesh->mVertices[nVertex].mPosition;
		length = position - center;
		distance = D3DXVec4Length(&length);
		if(distance > maxRadius)
		{
			maxRadius = distance;
		}
	}

	pBV = myNew Sphere(center.x, center.y, center.z, maxRadius, -1);

	return pBV;
}