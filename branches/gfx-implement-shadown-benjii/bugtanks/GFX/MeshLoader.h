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
#include "../Utilities/Macros.h"
#include "Mesh.h"
#include "InstanceMesh.h"

namespace gfx
{
	/**
	 * Struct for temporary storing mesh information.
	 * Meshes use the VertexTransform input layout.
	 */
	struct MeshStorage
	{
		int nVertex;
		VertexTransformStatic *mVerticesStatic;
		VertexTransformDynamic *mVerticesDynamic;
		int nIndex;
		unsigned int *mIndicies;

		/**
		 * Default constructor.
		 */
		MeshStorage()
		{
			nVertex = 0;
			mVerticesStatic = NULL;
			mVerticesDynamic = NULL;
			nIndex = 0;
			mIndicies = NULL;
		}

		/**
		 * Default copy constructor.
		 * Shallow copy.
		 */
		MeshStorage(const MeshStorage &storage)
		{
			nVertex = storage.nVertex;
			mVerticesStatic = storage.mVerticesStatic;
			mVerticesDynamic = storage.mVerticesDynamic;
			nIndex = storage.nIndex;
			mIndicies = storage.mIndicies;
		}

		/**
		 * Default destructor.
		 * Should not delete either of the storage pointers.
		 */
		~MeshStorage(){};

		/**
		 * Default assignment operator.
		 * Shallow copy.
		 */
		const MeshStorage &operator=(const MeshStorage &storage)
		{
			if(this != &storage)
			{
				nVertex = storage.nVertex;
				mVerticesStatic = storage.mVerticesStatic;
				mVerticesDynamic = storage.mVerticesDynamic;
				nIndex = storage.nIndex;
				mIndicies = storage.mIndicies;
			}
			return *this;
		}
	};

	/**
	 * Class that loads a mesh.
	 * Loads geometry, joints, animations and material information.
	 */
	class MeshLoader
	{
		public:
			/**
			 * Default constructor.
			 */
			MeshLoader();

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
			 * Imports a mesh and stores the geometry information in the supplied buffer.
			 *  Only imports geometry.
			 * @param filePath Absolute/relative filepath to an exported mesh.txt-file.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to the used Effect.
			 * @return Returns a pointer to the created Mesh.
			 */
			Mesh *importMesh(const char *filePath, ID3D10Device *pDevice, Effect *pEffect);

			/**
			 * Imports an instanced mesh and stores the geometry information in the supplied buffer.
			 *  Only imports geometry.
			 * @param filePath Absolute/relative filepath to an exported mesh.txt-file.
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param pEffect Pointer to the used Effect.
			 * @return Returns a pointer to the created Mesh.
			 */
			InstanceMesh *importInstanceMesh(const char *filePath, ID3D10Device *pDevice, Effect *pEffect);

		protected:

		private:
			/**
			 * Closes all import files.
			 */
			void closeImportFile();

			/**
			 * Deletes all mesh storages.
			 */
			void deleteMeshStorage();

			/**
			 * Reads a geometry file and inputs the data into the mMeshVector.
			 * @return True if successful.
			 */
			bool readGeometryFile(const char *filePath);

			/**
			 * Reads a geometry file while ignoring indexing information and inputs the data
			 *  into the mMeshVector.
			 * @return True if successful.
			 */
			bool readGeometryFileNonIndexed(const char *filePath);

			/**
			 * Creates a static vertex buffer for the data in a MeshStorage.
			 * @param meshIndex Index of the mesh (subset).
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param nVertices Outgoing reference to the number of vertices in the subset.
			 * @param pVertexBuffer Double pointer to the static vertex buffer.
			 * @return Returns true if successful.
			 */
			bool createVertexBufferStatic(unsigned int meshIndex, ID3D10Device *pDevice, unsigned int &nVertices, ID3D10Buffer **pVertexBuffer);

			/**
			 * Creates a dynamic vertex buffer for the data in a MeshStorage.
			 * @param meshIndex Index of the mesh (subset).
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param nVertices Outgoing reference to the number of vertices in the subset.
			 * @param pVertexBuffer Double pointer to the dynamic vertex buffer.
			 * @return Returns true if successful.
			 */
			bool createVertexBufferDynamic(unsigned int meshIndex, ID3D10Device *pDevice, unsigned int &nVertices, ID3D10Buffer **pVertexBuffer);

			/**
			 * Creates an index buffer for the data in a MeshStorage.
			 * @param meshIndex Index of the mesh (subset).
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param nIndices Outgoing reference to the number of indices in the subset.
			 * @param pIndexBuffer Double pointer to the index buffer.
			 * @return Returns true if successful.
			 */
			bool createIndexBuffer(unsigned int meshIndex, ID3D10Device *pDevice,  unsigned int &nIndices, ID3D10Buffer **pIndexBuffer);

			/**
			 * Creates an index buffer containing adjacency information for the data in a MeshStorage.
			 * @param meshIndex Index of the mesh (subset).
			 * @param pDevice Pointer to the ID3D10Device.
			 * @param nIndices Outgoing reference to the number of indices in the subset.
			 * @param pIndexBuffer Double pointer to the index buffer.
			 * @return Returns true if successful.
			 */
			bool createIndexBufferAdjacent(unsigned int meshIndex, ID3D10Device *pDevice,  unsigned int &nIndices, ID3D10Buffer **pIndexBuffer);

			/**
			 * Calculates the number of indices in a geometry file.
			 * @param filePath Absolute/relative path to the file.
			 * @param meshIndex Index of the submesh/subset to calculate indices for.
			 * @param nIndices Outgoing reference to the number of indices.
			 * @return Returns true if successful.
			 */
			bool findNIndices(const char *filePath, int meshIndex, int &nIndices);

			/**
			 * Checks if a vertex with the supplied position exists in a certain polygon in the supplied
			 *  vertex array.
			 * @param vertex Position of the vertex to check for.
			 * @param nPolygon Index of the polygon in the supplied index buffer (index of polygon's first vertex / 3).
			 * @param pVertices Pointer to the vertex array.
			 * @param pIndices Pointer to the index array.
			 * @return Returns true if the vertex exists in the polygon.
			 */
			bool vertexInPolygon(const D3DXVECTOR3 &vertex, int nPolygon, VertexTransformStatic *pVertices, unsigned int *pIndices);

			/**
			 * Retrieves the index of the non included vertex of a polygon.
			 * @param vertex1 Position of the first vertex of the polygon.
			 * @param vertex2 Position of the second vertex of the polygon.
			 * @param nPolygon Index of the polygon in the supplied index buffer (index of polygon's first vertex / 3).
			 * @param pVertices Pointer to the vertex array.
			 * @param pIndices Pointer to the index array.
			 * @return Returns the index of the third vertex or -1.
			 */
			unsigned int getIndexOfNonIncludedVertex(const D3DXVECTOR3 &vertex1, const D3DXVECTOR3 &vertex2, int nPolygon,
													 VertexTransformStatic *pVertices, unsigned int *pIndices);

			CollisionObject *createBVSphere(unsigned int meshIndex);

			std::vector<MeshStorage> mMeshVector;
			std::string mStringInput;
			int mIntInput;
			float mFloatInput;
			double mDoubleInput;
			std::fstream mImportFile;
	};
}

#endif