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

#ifndef MESH_LOADER_H
#define MESH_LOADER_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../Utilities/Macros.h"
#include "Mesh.h"
#include "InstanceMesh.h"
#include "InstanceAnimationMesh.h"
#include "Animation.h"
#include "Vertex.h"

namespace gfx
{
	class MeshLoader
	{
		public:
			/**
			 * Default constructor.
			 */
			MeshLoader(ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Default copy constructor.
			 */
			MeshLoader(const MeshLoader &loader);

			/**
			 * Default destructor.
			 */
			virtual ~MeshLoader();

			/**
			 *	Default assignment operator.
			 */
			const MeshLoader &operator=(const MeshLoader &loader);

			/**
			 * Attempts to open a geometry file in preparation for the import methods.
			 * @param filePath Absolute/relative path of the file.
			 * @return Returns true if successful.
			 */
			bool openGeometryFile(const char *filePath);

			/**
			 * Attempts to open a material file in preparation for the import methods.
			 * @param filePath Absolute/relative path of the file.
			 * @return Returns true if successful.
			 */
			bool openMaterialFile(const char *filePath);

			/**
			 * Attempts to open an animation file in preparation for the import methods.
			 * @param filePath Absolute/relative path of the file.
			 * @return Returns true if successful.
			 */
			bool openAnimationFile(const char *filePath);

			/**
			 * Attempts to open a weight file in preparation for the import methods.
			 * @param filePath Absolute/relative path of the file.
			 * @return Returns true if successful.
			 */
			bool openWeightFile(const char *filePath);

			/**
			 * Imports an instanced mesh base.
			 *  A geometry and material file must be available.
			 * @return Returns a pointer to the newly created instance base.
			 */
			InstanceMesh *importInstanceMesh();

			/**
			 * Imports an animated instanced mesh base.
			 *  A geometry, material and weight file must be available.
			 * @return Returns a pointer to the newly created instance base.
			 */
			InstanceAnimationMesh *importInstanceAnimationMesh();

			/**
			 * Imports an animation.
			 *  An animation file must be available.
			 * @return Returns a pointer to the newly created animation.
			 */
			Animation *importAnimation();

		protected:

		private:
			struct Material
			{
				D3DXCOLOR mAmbient;
				D3DXCOLOR mDiffuse;
				D3DXCOLOR mSpecular;

				Material() : mAmbient(), mDiffuse(), mSpecular()
				{

				}

				~Material()
				{

				}
			};

			struct Vertex
			{
				D3DXVECTOR4 mPosition;
				D3DXVECTOR4 mNormal;
				D3DXVECTOR4 mTangent;
				D3DXVECTOR4 mJoints;
				D3DXVECTOR4 mWeights;

				Vertex() : mPosition(0.0f, 0.0f, 0.0f, 0.0f),
					mNormal(0.0f, 0.0f, 0.0f, 0.0f), mTangent(0.0f, 0.0f, 0.0f, 0.0f),
					mJoints(0.0f, 0.0f, 0.0f, 0.0f), mWeights(0.0f, 0.0f, 0.0f, 0.0f)
				{

				}

				~Vertex()
				{

				}
			};

			struct Polygon
			{
				unsigned int mIndices[3];
				int mAdjacent[3];
				D3DXVECTOR4 mNormal;
				D3DXVECTOR4 mTangent;
				D3DXVECTOR2 mUV[3];
				int mMaterialIndex;

				Polygon() : mNormal(0.0f, 0.0f, 0.0f, 0.0f),
					mTangent(0.0f, 0.0f, 0.0f, 0.0f)
				{
					mIndices[0] = 0;
					mIndices[1] = 0;
					mIndices[2] = 0;
					mAdjacent[0] = 0;
					mAdjacent[1] = 0;
					mAdjacent[2] = 0;
					mUV[0] = D3DXVECTOR2(0.0f, 0.0f);
					mUV[1] = D3DXVECTOR2(0.0f, 0.0f);
					mUV[2] = D3DXVECTOR2(0.0f, 0.0f);
				}

				~Polygon()
				{

				}
			};

			struct Mesh
			{
				int mIndexCount;
				int mVertexCount;
				int mPolygonCount;
				std::vector<unsigned int> mIndices;
				std::vector<Vertex> mVertices;
				std::vector<Polygon> mPolygons;

				Mesh() : mIndices(), mVertices(), mPolygons()
				{
					mIndexCount = 0;
					mVertexCount = 0;
					mPolygonCount = 0;
				}

				~Mesh()
				{

				}
			};

			struct FileReader
			{
				std::fstream mFile;
				unsigned int mLocation;

				FileReader() : mFile()
				{
					mLocation = 0;
				}

				~FileReader()
				{

				}
			};

			/**
			 * Reads a text string from the provided file.
			 *  First reads the size of the string then the corresponding
			 *  number of characters.
			 * @param file FileReader of the file to read from.
			 * @param text Outgoing reference to the read text string.
			 * @return Returns true if successful.
			 */
			bool readText(FileReader &file, std::string &text);

			/**
			 * Reads a block of data from the provided file.
			 * @param file FileReader of the file to read from.
			 * @param pData Outgoing pointer to the read data.
			 * @param dataSize Size of the data block to read.
			 * @return Returns true if successful.
			 */
			bool read(FileReader &file, char *pData, unsigned int dataSize);

			/**
			 * Closes all open FileReaders.
			 */
			void closeAllFiles();

			/**
			 * Imports materials from the open material file.
			 *  Stores them in the mMaterialList.
			 * @return Returns true if successful.
			 */
			bool importMaterial();

			/**
			 * Imports mesh information from the open geometry file.
			 *  Stores them in the mMeshList.
			 * @return Returns true if successful.
			 */
			bool importGeometry();

			/**
			 * Imports vertex weight information from the open weight file.
			 *  Stores them in the mMeshList.
			 * @return Returns true if successful.
			 */
			bool importWeights();

			/**
			 * Deletes all materials from the mMaterialList.
			 */
			void clearMaterialList();

			/**
			 * Deletes all meshes from the mMeshList.
			 */
			void clearMeshList();

			/**
			 * Retrieves the ambient color of a material.
			 *  Returns white color if the provided material index is incorrect.
			 * @param materialIndex Index of the material.
			 * @param ambient Outgoing reference to the ambient color.
			 * @return Returns true if successful.
			 */
			bool getMaterialAmbient(int materialIndex, D3DXCOLOR &ambient);

			/**
			 * Retrieves the diffuse color of a material.
			 *  Returns white color if the provided material index is incorrect.
			 * @param materialIndex Index of the material.
			 * @param diffuse Outgoing reference to the diffuse color.
			 * @return Returns true if successful.
			 */
			bool getMaterialDiffuse(int materialIndex, D3DXCOLOR &diffuse);

			/**
			 * Retrieves the specular color of a material.
			 *  Returns white color if the provided material index is incorrect.
			 * @param materialIndex Index of the material.
			 * @param specular Outgoing reference to the specular color.
			 * @return Returns true if successful.
			 */
			bool getMaterialSpecular(int materialIndex, D3DXCOLOR &specular);

			/**
			 * Calculates adjacent information for all meshes in the mMeshList.
			 * @return Returns true if successful.
			 */
			bool calculateAdjacentIndices();

			/**
			 * Averages vertex normals for all meshes in the mMeshList.
			 * @return Returns true if successful.
			 */
			bool calculateVertexNormals();

			/**
			 * Creates a static vertex buffer using the VertexTransform structure.
			 * @param pMesh Pointer to the mesh to create the buffer for.
			 * @param pVertexBuffer Outgoing pointer to the newly created vertex buffer.
			 * @return Returns true if successful.
			 */
			bool createVertexBufferStatic(Mesh *pMesh, ID3D10Buffer **pVertexBuffer);

			/**
			 * Creates a static vertex buffer using the VertexAnimation structure.
			 * @param pMesh Pointer to the mesh to create the buffer for.
			 * @param pVertexBuffer Outgoing pointer to the newly created vertex buffer.
			 * @return Returns true if successful.
			 */
			bool createAnimationVertexBufferStatic(Mesh *pMesh, ID3D10Buffer **pVertexBuffer);

			/**
			 * Creates an index buffer.
			 * @param pMesh Pointer to the mesh to create the buffer for.
			 * @param pIndexBuffer Outgoing pointer to the newly created index buffer.
			 * @return Returns true if successful.
			 */
			bool createIndexBuffer(Mesh *pMesh, ID3D10Buffer **pIndexBuffer);

			/**
			 * Creates an index buffer with adjacent information.
			 * @param pMesh Pointer to the mesh to create the buffer for.
			 * @param pIndexBuffer Outgoing pointer to the newly created index buffer.
			 * @return Returns true if successful.
			 */
			bool createIndexBufferAdjacent(Mesh *pMesh, ID3D10Buffer **pIndexBuffer);

			/**
			 * Creates an OBB bounding volume for the provided mesh.
			 * @param pMesh Pointer to the mesh to create the bounding volume for.
			 * @return Returns true if successful.
			 */
			CollisionObject *createBoundingVolumeOBB(Mesh *pMesh);

			/**
			 * Creates a sphere bounding volume for the provided mesh.
			 * @param pMesh Pointer to the mesh to create the bounding volume for.
			 * @return Returns true if successful.
			 */
			CollisionObject *createBoundingVolumeSphere(Mesh *pMesh);

			ID3D10Device *mpDevice;
			Effect *mpEffect;
			FileReader mGeometryFile;
			FileReader mMaterialFile;
			FileReader mAnimationFile;
			FileReader mWeightFile;
			std::vector<Material *> mMaterialList;
			std::vector<Mesh *> mMeshList;
	};
}

#endif