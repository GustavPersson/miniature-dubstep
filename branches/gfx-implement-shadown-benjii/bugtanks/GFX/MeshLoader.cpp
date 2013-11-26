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

using namespace gfx;
using std::vector;
using std::string;
using std::fstream;
using std::ios_base;

MeshLoader::MeshLoader() : mMeshVector(), mStringInput(), mImportFile()
{
	mIntInput = 0;
	mFloatInput = 0.0f;
	mDoubleInput = 0.0;
}

MeshLoader::MeshLoader(const MeshLoader &loader) : mMeshVector(), mStringInput(), mImportFile()
{
	mIntInput = loader.mIntInput;
	mFloatInput = loader.mFloatInput;
	mDoubleInput = loader.mDoubleInput;
}

MeshLoader::~MeshLoader()
{
	closeImportFile();
	deleteMeshStorage();
}

const MeshLoader &MeshLoader::operator=(const MeshLoader &loader)
{
	if(this != &loader)
	{

	}
	return *this;
}

Mesh *MeshLoader::importMesh(const char *filePath, ID3D10Device *pDevice, Effect *pEffect)
{
	Mesh *pMesh = NULL;
	Subset *pSubset = NULL;
	ID3D10Buffer *pVertexBufferStatic = NULL;
	ID3D10Buffer *pVertexBufferDynamic = NULL;
	ID3D10Buffer *pIndexBuffer = NULL;
	unsigned int nVertices = 0;
	unsigned int nIndices = 0;

	closeImportFile();
	mImportFile.open(filePath, ios_base::in);
	if(mImportFile.is_open())
	{
		readGeometryFile(filePath);
		mImportFile.close();
	}

	pMesh = myNew Mesh(pDevice, pEffect);
	for(unsigned int n = 0; n < mMeshVector.size(); ++n)
	{
		if(!createVertexBufferStatic(n, pDevice, nVertices, &pVertexBufferStatic))
		{
			DEBUG_MESSAGE("Error: GFX: MeshLoader: Error creating static vertex buffer.")
			return NULL;
		}
		if(!createVertexBufferDynamic(n, pDevice, nVertices, &pVertexBufferDynamic))
		{
			DEBUG_MESSAGE("Error: GFX: MeshLoader: Error creating dynamic vertex buffer.")
			return NULL;
		}
		if(!createIndexBufferAdjacent(n, pDevice, nIndices, &pIndexBuffer))
		{
			DEBUG_MESSAGE("Error: GFX: MeshLoader: Error creating index buffer.")
			return NULL;
		}

		pSubset = myNew Subset(pDevice, pEffect);
		pSubset->loadBuffers(nVertices, pVertexBufferStatic, pVertexBufferDynamic, nIndices, pIndexBuffer);
		CollisionObject *pBV = createBVSphere(n);
		pMesh->setBoundingVolume(pBV);
		pMesh->addSubset(pSubset);
	}
	return pMesh;
}

InstanceMesh *MeshLoader::importInstanceMesh(const char *filePath, ID3D10Device *pDevice, Effect *pEffect)
{
	InstanceMesh *pMesh = NULL;
	InstanceSubset *pSubset = NULL;
	ID3D10Buffer *pVertexBufferStatic = NULL;
	ID3D10Buffer *pIndexBuffer = NULL;
	unsigned int nVertices = 0;
	unsigned int nIndices = 0;

	closeImportFile();
	mImportFile.open(filePath, ios_base::in);
	if(mImportFile.is_open())
	{
		readGeometryFileNonIndexed(filePath);
		mImportFile.close();
	}

	pMesh = myNew InstanceMesh(pDevice, pEffect);
	pMesh->setFlagInstanceBase(true);
	for(unsigned int n = 0; n < mMeshVector.size(); ++n)
	{
		if(!createVertexBufferStatic(n, pDevice, nVertices, &pVertexBufferStatic))
		{
			DEBUG_MESSAGE("Error: GFX: MeshLoader: Error creating static vertex buffer.")
			return NULL;
		}
		if(!createIndexBufferAdjacent(n, pDevice, nIndices, &pIndexBuffer))
		{
			DEBUG_MESSAGE("Error: GFX: MeshLoader: Error creating index buffer.")
			return NULL;
		}

		pSubset = myNew InstanceSubset(pDevice, pEffect);
		pSubset->setFlagInstanceBase(true);
		pSubset->loadBuffers(nVertices, pVertexBufferStatic, nIndices, pIndexBuffer);
		CollisionObject *pBV = createBVSphere(n);
		pSubset->setBoundingVolume(pBV);
		pMesh->addSubset(pSubset);
	}
	pMesh->setInstanceCountMax(1);
	return pMesh;
}

void MeshLoader::closeImportFile()
{
	if(mImportFile.is_open())
	{
		mImportFile.close();
	}
}

void MeshLoader::deleteMeshStorage()
{
	for(vector<MeshStorage>::iterator meshIter = mMeshVector.begin();
		meshIter != mMeshVector.end(); ++meshIter)
	{
		if((*meshIter).mVerticesStatic)
		{
			delete [] (*meshIter).mVerticesStatic;
			(*meshIter).mVerticesStatic = NULL;
		}
		if((*meshIter).mVerticesDynamic)
		{
			delete [] (*meshIter).mVerticesDynamic;
			(*meshIter).mVerticesDynamic = NULL;
		}
		if((*meshIter).mIndicies)
		{
			delete [] (*meshIter).mIndicies;
			(*meshIter).mIndicies = NULL;
		}
	}
	mMeshVector.clear();
}

bool MeshLoader::readGeometryFile(const char *filePath)
{
	deleteMeshStorage();
	int nMesh = 0;
	int nVertex = 0;
	int nPolygon = 0;
	string meshName;
	mImportFile >> mStringInput;
	if(mStringInput.compare("NumberOfMeshes:") == 0)
	{
		mImportFile >> mIntInput;
		nMesh = mIntInput;
		for(int meshIndex = 0; meshIndex < nMesh; ++meshIndex)
		{
			//Create mesh storage.
			MeshStorage storage;

			//Name of the mesh.
			mImportFile >> mStringInput;
			if(mStringInput.compare("MeshName:") == 0)
			{
				mImportFile >> meshName;
			}

			//Vertices.
			mImportFile >> mStringInput;
			if(mStringInput.compare("NumberOfVertices:") == 0)
			{
				mImportFile >> mIntInput;
				nVertex = mIntInput;

				//Allocate vertex data.
				storage.nVertex = nVertex;
				storage.mVerticesStatic = myNew VertexTransformStatic[nVertex];
				storage.mVerticesDynamic = myNew VertexTransformDynamic[nVertex];

				//Load vertex coordinates.
				for(int vertexIndex = 0; vertexIndex < nVertex; ++vertexIndex)
				{
					//X.
					mImportFile >> mFloatInput;
					storage.mVerticesStatic[vertexIndex].position.x = mFloatInput;
					//Y.
					mImportFile >> mFloatInput;
					storage.mVerticesStatic[vertexIndex].position.y = mFloatInput;
					//Z.
					mImportFile >> mFloatInput;
					storage.mVerticesStatic[vertexIndex].position.z = mFloatInput;
				}
			}

			//Polygons.
			int index1 = 0;
			int index2 = 0;
			int index3 = 0;
			D3DXVECTOR3 normal;
			D3DXVECTOR3 tangent;
			mImportFile >> mStringInput;
			if(mStringInput.compare("NumberOfPolygons:") == 0)
			{
				mImportFile >> mIntInput;
				nPolygon = mIntInput;

				//Allocate index data.
				storage.nIndex = nPolygon * 3;
				storage.mIndicies = myNew unsigned int[storage.nIndex];

				for(int polyIndex = 0; polyIndex < nPolygon; ++polyIndex)
				{
					//Indicies.
					mImportFile >> mStringInput;
					if(mStringInput.compare("VertexIndicies:") == 0)
					{
						//Index 1.
						mImportFile >> mIntInput;
						index1 = mIntInput;
						storage.mIndicies[polyIndex * 3] = mIntInput;
						//Index 2.
						mImportFile >> mIntInput;
						index2 = mIntInput;
						storage.mIndicies[polyIndex * 3 + 1] = mIntInput;
						//Index 3.
						mImportFile >> mIntInput;
						index3 = mIntInput;
						storage.mIndicies[polyIndex * 3 + 2] = mIntInput;
					}

					//Normals.
					mImportFile >> mStringInput;
					if(mStringInput.compare("Normal:") == 0)
					{
						//Normal X.
						mImportFile >> mFloatInput;
						normal.x = mFloatInput;
						//Normal Y.
						mImportFile >> mFloatInput;
						normal.y = mFloatInput;
						//Normal Z.
						mImportFile >> mFloatInput;
						normal.z = mFloatInput;

						//To do: interpolate normals.
						storage.mVerticesStatic[index1].normal = normal;
						storage.mVerticesStatic[index2].normal = normal;
						storage.mVerticesStatic[index3].normal = normal;
					}

					//Tangents.
					mImportFile >> mStringInput;
					if(mStringInput.compare("Tangent:") == 0)
					{
						//Tangent X.
						mImportFile >> mFloatInput;
						tangent.x = mFloatInput;
						//Tangent Y.
						mImportFile >> mFloatInput;
						tangent.y = mFloatInput;
						//Tangent Z.
						mImportFile >> mFloatInput;
						tangent.z = mFloatInput;

						//To do: interpolate tangents.
						storage.mVerticesStatic[index1].tangent = tangent;
						storage.mVerticesStatic[index2].tangent = tangent;
						storage.mVerticesStatic[index3].tangent = tangent;
					}

					//UV1.
					mImportFile >> mStringInput;
					if(mStringInput.compare("UV1:") == 0)
					{
						//UV1 X.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[index1].UV.x = mFloatInput;
						//UV1 Y.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[index1].UV.y = mFloatInput;
					}

					//UV2.
					mImportFile >> mStringInput;
					if(mStringInput.compare("UV2:") == 0)
					{
						//UV2 X.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[index2].UV.x = mFloatInput;
						//UV2 Y.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[index2].UV.y = mFloatInput;
					}

					//UV3.
					mImportFile >> mStringInput;
					if(mStringInput.compare("UV3:") == 0)
					{
						//UV3 X.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[index3].UV.x = mFloatInput;
						//UV3 Y.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[index3].UV.y = mFloatInput;
					}

					//Material index.
					mImportFile >> mStringInput;
					if(mStringInput.compare("MaterialIndex:") == 0)
					{
						mImportFile >> mIntInput;
					}

					storage.mVerticesDynamic[index1].textureID = 0;
					storage.mVerticesDynamic[index2].textureID = 0;
					storage.mVerticesDynamic[index3].textureID = 0;
					storage.mVerticesDynamic[index1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					storage.mVerticesDynamic[index2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					storage.mVerticesDynamic[index3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					D3DXMATRIX MIdentity;
					D3DXMatrixIdentity(&MIdentity);
					storage.mVerticesDynamic[index1].transform = MIdentity;
					storage.mVerticesDynamic[index2].transform = MIdentity;
					storage.mVerticesDynamic[index3].transform = MIdentity;
				}
			}
			mMeshVector.push_back(storage);
		}
	}
	return true;
}

bool MeshLoader::readGeometryFileNonIndexed(const char *filePath)
{
	deleteMeshStorage();
	int nMesh = 0;
	int nVertex = 0;
	VertexTransformStatic *pTempVertices = NULL;
	int nPolygon = 0;
	int nIndices = 0;

	string meshName;
	mImportFile >> mStringInput;
	if(mStringInput.compare("NumberOfMeshes:") == 0)
	{
		mImportFile >> mIntInput;
		nMesh = mIntInput;
		for(int meshIndex = 0; meshIndex < nMesh; ++meshIndex)
		{
			//Create mesh storage.
			MeshStorage storage;
			findNIndices(filePath, meshIndex, nIndices);

			//Name of the mesh.
			mImportFile >> mStringInput;
			if(mStringInput.compare("MeshName:") == 0)
			{
				mImportFile >> meshName;
			}

			//Vertices.
			mImportFile >> mStringInput;
			if(mStringInput.compare("NumberOfVertices:") == 0)
			{
				mImportFile >> mIntInput;
				nVertex = mIntInput;

				//Allocate vertex data.
				pTempVertices = myNew VertexTransformStatic[nVertex];
				storage.nVertex = nIndices;
				storage.mVerticesStatic = myNew VertexTransformStatic[nIndices];
				storage.mVerticesDynamic = myNew VertexTransformDynamic[nIndices];

				//Load vertex coordinates.
				for(int vertexIndex = 0; vertexIndex < nVertex; ++vertexIndex)
				{
					//X.
					mImportFile >> mFloatInput;
					pTempVertices[vertexIndex].position.x = mFloatInput;
					//Y.
					mImportFile >> mFloatInput;
					pTempVertices[vertexIndex].position.y = mFloatInput;
					//Z.
					mImportFile >> mFloatInput;
					pTempVertices[vertexIndex].position.z = mFloatInput;
				}
			}

			//Polygons.
			int index1 = 0;
			int index2 = 0;
			int index3 = 0;
			D3DXVECTOR3 normal;
			D3DXVECTOR3 tangent;
			mImportFile >> mStringInput;
			if(mStringInput.compare("NumberOfPolygons:") == 0)
			{
				mImportFile >> mIntInput;
				nPolygon = mIntInput;

				//Allocate index data.
				storage.nIndex = nIndices;
				storage.mIndicies = myNew unsigned int[storage.nIndex];

				for(int polyIndex = 0; polyIndex < nPolygon; ++polyIndex)
				{
					//Indicies.
					mImportFile >> mStringInput;
					if(mStringInput.compare("VertexIndicies:") == 0)
					{
						//Index 1.
						mImportFile >> mIntInput;
						index1 = mIntInput;
						storage.mIndicies[polyIndex * 3] = polyIndex * 3;
						//Index 2.
						mImportFile >> mIntInput;
						index2 = mIntInput;
						storage.mIndicies[polyIndex * 3 + 1] = polyIndex * 3 + 1;
						//Index 3.
						mImportFile >> mIntInput;
						index3 = mIntInput;
						storage.mIndicies[polyIndex * 3 + 2] = polyIndex * 3 + 2;
					}

					storage.mVerticesStatic[polyIndex * 3].position = pTempVertices[index1].position;
					storage.mVerticesStatic[polyIndex * 3 + 1].position = pTempVertices[index2].position;
					storage.mVerticesStatic[polyIndex * 3 + 2].position = pTempVertices[index3].position;

					//Normals.
					mImportFile >> mStringInput;
					if(mStringInput.compare("Normal:") == 0)
					{
						//Normal X.
						mImportFile >> mFloatInput;
						normal.x = mFloatInput;
						//Normal Y.
						mImportFile >> mFloatInput;
						normal.y = mFloatInput;
						//Normal Z.
						mImportFile >> mFloatInput;
						normal.z = mFloatInput;

						storage.mVerticesStatic[polyIndex * 3].normal = normal;
						storage.mVerticesStatic[polyIndex * 3 + 1].normal = normal;
						storage.mVerticesStatic[polyIndex * 3 + 2].normal = normal;
					}

					//Tangents.
					mImportFile >> mStringInput;
					if(mStringInput.compare("Tangent:") == 0)
					{
						//Tangent X.
						mImportFile >> mFloatInput;
						tangent.x = mFloatInput;
						//Tangent Y.
						mImportFile >> mFloatInput;
						tangent.y = mFloatInput;
						//Tangent Z.
						mImportFile >> mFloatInput;
						tangent.z = mFloatInput;

						storage.mVerticesStatic[polyIndex * 3].tangent = tangent;
						storage.mVerticesStatic[polyIndex * 3 + 1].tangent = tangent;
						storage.mVerticesStatic[polyIndex * 3 + 2].tangent = tangent;
					}

					//UV1.
					mImportFile >> mStringInput;
					if(mStringInput.compare("UV1:") == 0)
					{
						//UV1 X.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[polyIndex * 3].UV.x = mFloatInput;
						//UV1 Y.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[polyIndex * 3].UV.y = mFloatInput;
					}

					//UV2.
					mImportFile >> mStringInput;
					if(mStringInput.compare("UV2:") == 0)
					{
						//UV2 X.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[polyIndex * 3 + 1].UV.x = mFloatInput;
						//UV2 Y.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[polyIndex * 3 + 1].UV.y = mFloatInput;
					}

					//UV3.
					mImportFile >> mStringInput;
					if(mStringInput.compare("UV3:") == 0)
					{
						//UV3 X.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[polyIndex * 3 + 2].UV.x = mFloatInput;
						//UV3 Y.
						mImportFile >> mFloatInput;
						storage.mVerticesStatic[polyIndex * 3 + 2].UV.y = mFloatInput;
					}

					//Material index.
					mImportFile >> mStringInput;
					if(mStringInput.compare("MaterialIndex:") == 0)
					{
						mImportFile >> mIntInput;
					}

					storage.mVerticesDynamic[polyIndex * 3].textureID = 0;
					storage.mVerticesDynamic[polyIndex * 3 + 1].textureID = 0;
					storage.mVerticesDynamic[polyIndex * 3 + 2].textureID = 0;
					storage.mVerticesDynamic[polyIndex * 3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					storage.mVerticesDynamic[polyIndex * 3 + 1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					storage.mVerticesDynamic[polyIndex * 3 + 2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					D3DXMATRIX MIdentity;
					D3DXMatrixIdentity(&MIdentity);
					storage.mVerticesDynamic[polyIndex * 3].transform = MIdentity;
					storage.mVerticesDynamic[polyIndex * 3 + 1].transform = MIdentity;
					storage.mVerticesDynamic[polyIndex * 3 + 2].transform = MIdentity;
				}
			}
			mMeshVector.push_back(storage);
			if(pTempVertices)
			{
				delete [] pTempVertices;
				pTempVertices = NULL;
			}
		}
	}
	return true;
}

bool MeshLoader::createVertexBufferStatic(unsigned int meshIndex, ID3D10Device *pDevice, unsigned int &nVertices, ID3D10Buffer **pVertexBuffer)
{ 
	vector<MeshStorage>::iterator meshIter = mMeshVector.begin();
	meshIter += meshIndex;

	nVertices = (*meshIter).nVertex;

	D3D10_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTransformStatic) * (*meshIter).nVertex;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = (*meshIter).mVerticesStatic;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if(FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, pVertexBuffer)))
	{
		DEBUG_MESSAGE("Error: GFX: MeshLoader: Error creating static vertex buffer.");
		return false;
	}
	return true;
}

bool MeshLoader::createVertexBufferDynamic(unsigned int meshIndex, ID3D10Device *pDevice, unsigned int &nVertices, ID3D10Buffer **pVertexBuffer)
{ 
	vector<MeshStorage>::iterator meshIter = mMeshVector.begin();
	meshIter += meshIndex;

	nVertices = (*meshIter).nVertex;
	D3D10_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexTransformDynamic) * (*meshIter).nVertex;
	vertexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = (*meshIter).mVerticesDynamic;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if(FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, pVertexBuffer)))
	{
		DEBUG_MESSAGE("Error: GFX: MeshLoader: Error creating dynamic vertex buffer.");
		return false;
	}
	return true;
}

bool MeshLoader::createIndexBuffer(unsigned int meshIndex, ID3D10Device *pDevice, unsigned int &nIndices, ID3D10Buffer **pIndexBuffer)
{
	vector<MeshStorage>::iterator meshIter = mMeshVector.begin();
	meshIter += meshIndex;

	nIndices = (*meshIter).nIndex;
	D3D10_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * (*meshIter).nIndex;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = (*meshIter).mIndicies;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if(FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, pIndexBuffer)))
	{
		DEBUG_MESSAGE("Error: GFX: MeshLoader: Error creating index buffer.");
		return false;
	}
	return true;
}

bool MeshLoader::createIndexBufferAdjacent(unsigned int meshIndex, ID3D10Device *pDevice, unsigned int &nIndices, ID3D10Buffer **pIndexBuffer)
{
	vector<MeshStorage>::iterator meshIter = mMeshVector.begin();
	meshIter += meshIndex;

	unsigned int *pAdjacentIndices = myNew unsigned int[(*meshIter).nIndex * 2];
	D3DXVECTOR3 vertex1;
	D3DXVECTOR3 vertex2;

	for(int nPolygon = 0; nPolygon < (*meshIter).nIndex / 3; ++nPolygon)
	{
		//Add original vertices at even locations.
		pAdjacentIndices[nPolygon * 6] = (*meshIter).mIndicies[nPolygon * 3];
		pAdjacentIndices[nPolygon * 6 + 2] = (*meshIter).mIndicies[nPolygon * 3 + 1];
		pAdjacentIndices[nPolygon * 6 + 4] = (*meshIter).mIndicies[nPolygon * 3 + 2];

		//Find adjacent indices.
		unsigned int adj[3] = {0, 0, 0};

		for(int nPolygon2 = 0; nPolygon2 < (*meshIter).nIndex / 3; ++nPolygon2)
		{
			if(nPolygon2 == nPolygon)
			{
				continue;
			}

			//adj[0]
			vertex1 = (*meshIter).mVerticesStatic[pAdjacentIndices[nPolygon * 6]].position;
			vertex2 = (*meshIter).mVerticesStatic[pAdjacentIndices[nPolygon * 6 + 2]].position;
			if(vertexInPolygon(vertex1, nPolygon2, (*meshIter).mVerticesStatic, (*meshIter).mIndicies))
			{
				if(vertexInPolygon(vertex2, nPolygon2, (*meshIter).mVerticesStatic, (*meshIter).mIndicies))
				{
					adj[0] = getIndexOfNonIncludedVertex(vertex1, vertex2, nPolygon2, (*meshIter).mVerticesStatic, (*meshIter).mIndicies);
				}
			}

			//adj[1]
			vertex1 = (*meshIter).mVerticesStatic[pAdjacentIndices[nPolygon * 6 + 2]].position;
			vertex2 = (*meshIter).mVerticesStatic[pAdjacentIndices[nPolygon * 6 + 4]].position;
			if(vertexInPolygon(vertex1, nPolygon2, (*meshIter).mVerticesStatic, (*meshIter).mIndicies))
			{
				if(vertexInPolygon(vertex2, nPolygon2, (*meshIter).mVerticesStatic, (*meshIter).mIndicies))
				{
					adj[1] = getIndexOfNonIncludedVertex(vertex1, vertex2, nPolygon2, (*meshIter).mVerticesStatic, (*meshIter).mIndicies);
				}
			}

			//adj[2]
			vertex1 = (*meshIter).mVerticesStatic[pAdjacentIndices[nPolygon * 6]].position;
			vertex2 = (*meshIter).mVerticesStatic[pAdjacentIndices[nPolygon * 6 + 4]].position;
			if(vertexInPolygon(vertex1, nPolygon2, (*meshIter).mVerticesStatic, (*meshIter).mIndicies))
			{
				if(vertexInPolygon(vertex2, nPolygon2, (*meshIter).mVerticesStatic, (*meshIter).mIndicies))
				{
					adj[2] = getIndexOfNonIncludedVertex(vertex1, vertex2, nPolygon2, (*meshIter).mVerticesStatic, (*meshIter).mIndicies);
				}
			}
		}

		//Add adjacent vertices.
		pAdjacentIndices[nPolygon * 6 + 1] = adj[0];
		pAdjacentIndices[nPolygon * 6 + 3] = adj[1];
		pAdjacentIndices[nPolygon * 6 + 5] = adj[2];
	}

	nIndices = (*meshIter).nIndex * 2;
	D3D10_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * (*meshIter).nIndex * 2;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = pAdjacentIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if(FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, pIndexBuffer)))
	{
		DEBUG_MESSAGE("Error: GFX: MeshLoader: Error creating index buffer.");
		return false;
	}
	delete [] pAdjacentIndices;
	return true;
}

bool MeshLoader::findNIndices(const char *filePath, int meshIndex, int &nIndices)
{
	string stringInput;
	int intInput = 0;
	int index = 0;
	fstream tempStream(filePath, ios_base::in);
	if(tempStream.is_open())
	{
		while(!tempStream.eof())
		{
			tempStream >> stringInput;
			if(stringInput.compare("NumberOfPolygons:") == 0)
			{
				if(index == meshIndex)
				{
					tempStream >> intInput;
					nIndices = intInput * 3;
					break;
				}
				++index;
			}
		}
		tempStream.close();
		return true;
	}
	return false;
}

bool MeshLoader::vertexInPolygon(const D3DXVECTOR3 &vertex, int nPolygon, VertexTransformStatic *pVertices, unsigned int *pIndices)
{
	for(int nIndex = nPolygon * 3; nIndex < nPolygon * 3 + 3; ++nIndex)
	{
		if(pVertices[pIndices[nIndex]].position == vertex)
		{
			return true;
		}
	}
	return false;
}

unsigned int MeshLoader::getIndexOfNonIncludedVertex(const D3DXVECTOR3 &vertex1, const D3DXVECTOR3 &vertex2, int nPolygon,
													 VertexTransformStatic *pVertices, unsigned int *pIndices)
{
	for(int nIndex = nPolygon * 3; nIndex < nPolygon * 3 + 3; ++nIndex)
	{
		if(pVertices[pIndices[nIndex]].position != vertex1)
		{
			if(pVertices[pIndices[nIndex]].position != vertex2)
			{
				return pIndices[nIndex];
			}
		}
	}
	return -1;
}

CollisionObject *MeshLoader::createBVSphere(unsigned int meshIndex)
{
	vector<MeshStorage>::iterator meshIter = mMeshVector.begin();
	meshIter += meshIndex;

	Sphere *BV = NULL;
	float maxR = 0.0f;
	float distance = 0.0f;
	for(int n = 0; n < (*meshIter).nVertex; ++n)
	{
		distance = D3DXVec3Length(&(*meshIter).mVerticesStatic[n].position);
		if(distance > maxR)
		{
			maxR = distance;
		}
	}
	BV = myNew Sphere(0.0f, 0.0f, 0.0f, maxR, -1);
	return BV;
}