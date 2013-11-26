/**
 * @BTMayaExporter.h
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
 * 
 * @section DESCRIPTION
 *
 * Simple exporter for Maya files.
 * See documentation on the Wiki for details.
 */

#ifndef BT_MAYA_EXPORTER_H
#define BT_MAYA_EXPORTER_H

#define WIN32
#define _WINDOWS
#define NT_APP				//External Maya-application (not a plug-in).
#define REQUIRE_IOSTREAM

#define LINEBREAK "\x0D\x0A" //Carriage return + line feed.

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <vector>
#include <maya\MLibrary.h>
#include <maya\MIOStream.h>
#include <maya\MGlobal.h>
#include <maya\MFileIO.h>
#include <maya\MItDependencyGraph.h>
#include <maya\MItDependencyNodes.h>
#include <maya\MItDag.h>
#include <maya\MItMeshPolygon.h>
#include <maya\MItKeyframe.h>
#include <maya\MDagPath.h>
#include <maya\MDagPathArray.h>
#include <maya\MFnDagNode.h>
#include <maya\MFnAttribute.h>
#include <maya\MFnMesh.h>
#include <maya\MItGeometry.h>
#include <maya\MAnimControl.h>
#include <maya\MFnIkJoint.h>
#include <maya\MFloatPointArray.h>
#include <maya\MMatrix.h>
#include <maya\MQuaternion.h>
#include <maya\MFnSkinCluster.h>
#include <maya\MEulerRotation.h>
#include <d3dx10.h>

#pragma comment(lib, "Foundation.lib")
#pragma comment(lib, "OpenMaya.lib")
#pragma comment(lib, "OpenMayaAnim.lib")

//Sample frequency for animations (24 frames per second).
#define TIME_SAMPLE MTime::kFilm

/**
 * Stores a mesh node and relevant
 *  geometry arrays.
 */
struct MeshGeometry
{
	MDagPathArray mMesh;
	MFloatPointArray mVertexArray;
	MFloatVectorArray mNormalArray;
	MFloatVectorArray mTangentArray;
	MFloatArray mUArray;
	MFloatArray mVArray;
	MDoubleArray mJointWeights;
};

/**
 * Stores an animation name together
 *  with its keyframes.
 */
struct Animation
{
	MString mName;
	MTimeArray mKeyframeArray;
};

/**
 * Data structures for binary export.
 */
struct MaterialData
{
	D3DXCOLOR mAmbient;
	D3DXCOLOR mDiffuse;
	D3DXCOLOR mSpecular;
};

struct MeshData
{
	int mVertexCount;
	int mPolygonCount;
};

struct PolygonData
{
	int mIndex1;
	int mIndex2;
	int mIndex3;
	D3DXVECTOR3 mNormal;
	D3DXVECTOR3 mTangent;
	D3DXVECTOR2 mUV1;
	D3DXVECTOR2 mUV2;
	D3DXVECTOR2 mUV3;
	int mMaterialID;
};

struct JointData
{
	int mID;
	int mParentID;
	D3DXVECTOR3 mTranslation;
	D3DXVECTOR4 mRotation;
	D3DXVECTOR4 mOrientation;
};

class BTMayaExporter
{
	public:
		/**
		 * Default constructor.
		 */
		BTMayaExporter();

		/**
		 * Default destructor.
		 */
		virtual ~BTMayaExporter();

		/**
		 * Starts the exporter and prompts input file.
		 */
		bool startExporter();

	protected:
		/**
		 * Default copy constructor.
		 * The exporter should never be copied so
		 *  this constructor is empty.
		 */
		BTMayaExporter(const BTMayaExporter &exporter);

		/**
		 * Default assignment operator.
		 * The exporter should never be copied so
		 *  this operator is empty.
		 */
		const BTMayaExporter &operator=(const BTMayaExporter &exporter);

	private:
		/**
		 * Writes a text string to the specified file.
		 * @param file Reference to the output file.
		 * @param text Text string to write.
		 * @param location Location to write the data block. Returns
		 *  the new location.
		 * @return Returns true if successful.
		 */
		bool writeText(std::fstream &file, const std::string &text, unsigned int &location);

		/**
		 * Writes a block of data to the specified file.
		 *  Note: the file should be opened with the binary flag.
		 * @param file Reference to the output file.
		 * @param pData Pointer to the data block.
		 * @param dataSize Size of the data block.
		 * @param location Location to write the data block. Returns
		 *  the new location.
		 * @return Returns true if successful.
		 */
		bool write(std::fstream &file, const char *pData, unsigned int dataSize, unsigned int &location);

		/**
		 * Retrieves the color information from a node and places it in a D3DXCOLOR.
		 * @param materialNode Material node containing the color information.
		 * @param colorType String name of the color type to retrieve.
		 * @param color Outgoing reference to a D3DXCOLOR.
		 * @return Returns true if successful.
		 */
		bool getColor(MFnDependencyNode &materialNode, const std::string &colorType, D3DXCOLOR &color);

		/**
		 * Packs material information to a MaterialData structure.
		 * @param material Reference to a material node.
		 * @param data Outgoing reference to a MaterialData structure.
		 * @return Returns true if successful.
		 */
		bool packMaterial(const MObject &material, MaterialData &data);

		/**
		 * Packs mesh information to a MeshData structure.
		 * @param pMesh Pointer to a mesh storage.
		 * @data Outgoing reference to a MeshData structure.
		 * @return Returns true if successful.
		 */
		bool packMesh(MeshGeometry *pMesh, MeshData &data);

		/**
		 * Packs polygon information to a PolygonData structure.
		 * @param pMesh Pointer to a mesh storage.
		 * @param polygonIter Reference to a polygon iterator of the current mesh.
		 * @param data Outgoing reference to a PolygonData structure.
		 * @return Returns true if successful.
		 */
		bool packPolygon(MeshGeometry *pMesh, MItMeshPolygon &polygonIter, PolygonData &data);

		/**
		 * Packs joint information to a JointData structure.
		 * @param joint Reference to the joint function set.
		 * @param jointID ID of the joint.
		 * @param data Outgoing reference to a JointData structure.
		 * @return Returns true if successful.
		 */
		bool packJoint(MFnIkJoint &joint, int jointID, JointData &data);

		/**
		 * Initializes the Maya library.
		 * @return True if successful.
		 */
		bool initializeMaya();

		/**
		 * Clear up used resources.
		 */
		void cleanUpMaya();

		/**
		 * Checks if a MObject contains any subtypes of MFn::kBlendNodeBase.
		 * @param node MObject to check.
		 * @return True if any subtypes are found.
		 */
		bool isBlendNode(MObject &node);

		/**
		 * Retrieves the material ID of a polygon's connected surface shader.
		 * @param shaderArray Array of all surface shaders connected to a mesh.
		 * @param shaderIndexArray Array storing shader-for-each-polygon indicies.
		 * @param polygonIndex Index of the polygon.
		 * @return The material ID or -1.
		 */
		int getPolygonMaterial(MObjectArray &shaderArray, MIntArray &shaderIndexArray, unsigned int polygonIndex);

		/**
		 * Adds the MTime-reference to the MTimeArray if there aren't any
		 *  references with the same time-value.
		 * @param time MTime to add to the array.
		 * @param timeArray Array to add the MTime to.
		 */
		void addTimeUnique(MTime &time, MTimeArray &timeArray);

		/**
		 * Sorts a MTimeArray in ascending order.
		 * @param timeArray Array to sort.
		 */
		void sortTimeArray(MTimeArray &timeArray);

		/**
		 * Retrieves the parent ID of a joint.
		 * @param joint Child joint of the requested parent.
		 */
		int getJointParentID(MFnIkJoint &joint);

		/**
		 * Opens the four export files mExportGeometry,
		 *  mExportMaterial, mExportJoint and mExportAnimation.
		 * @return True if successful.
		 */
		bool createExportFiles();

		/**
		 * Opens the four export files mExportGeometry,
		 *  mExportMaterial, mExportJoint and mExportAnimation.
		 * @return True if successful.
		 */
		bool createExportFilesBinary();

		/**
		 * Closes the four export files.
		 */
		void closeExportFiles();

		/**
		 * Begins exporting a model located at filePath.
		 * @param filePath String containing the absolute file path of the model.
		 * @param faceClockwise True if polygon should be constructed clockwise.
		 *  Note: not implemented yet.
		 * @return True if successful.
		 */
		bool exportModel(const char *filePath, bool faceClockwise);

		/**
		 * Begins exporting a model located at filePath.
		 * @param filePath String containing the absolute file path of the model.
		 * @param faceClockwise True if polygon should be constructed clockwise.
		 *  Note: not implemented yet.
		 * @return True if successful.
		 */
		bool exportModelBinary(const char *filePath, bool faceClockwise);

		/**
		 * Loads and stores material information.
		 * @return True if successful.
		 */
		bool saveMaterialInformation();

		/**
		 * Loads and stores mesh information.
		 * @return True if successful.
		 */
		bool saveMeshInformation();

		/**
		 * Loads and stores joint information.
		 * @return True if successful.
		 */
		bool saveJointInformation();

		/**
		 * Loads and stores joint weight information.
		 * @return True if successful.
		 */
		bool saveJointWeightInformation();

		/**
		 * Loads and stores animation information.
		 * @return True if successful.
		 */
		bool saveAnimationInformation();

		/**
		 * Exports materials and connected textures to file.
		 * @return True if successful.
		 */
		bool exportMaterials();

		/**
		 * Exports materials and connected textures to file.
		 * @return True if successful.
		 */
		bool exportMaterialsBinary();

		/**
		 * Exports color information to file.
		 *  Called from exportMaterials().
		 * @param materialNode Dependency node of the material.
		 * @param colorType String name of the color attribute.
		 * @return True if successful.
		 */
		bool exportColor(MFnDependencyNode &materialNode, const std::string &colorType);

		/**
		 * Exports texture location to file.
		 *  Called from exportMaterials().
		 *  Only exports textures connected to the "color"-channel.
		 * @param diffuseChannel Dependency node of the material.
		 * @return True if successful.
		 */
		bool exportTexture(MFnDependencyNode &diffuseChannel);

		/**
		 * Exports meshes, vertices and polygons to file.
		 * @return True if successful.
		 */
		bool exportGeometry();

		/**
		 * Exports meshes, vertices and polygons to file.
		 * @return True if successful.
		 */
		bool exportGeometryBinary();

		/**
		 * Exports initial joint transforms, ID and parent information to file.
		 * @return True if successful.
		 */
		bool exportJoints();

		/**
		 * Exports initial joint transforms, ID and parent information to file.
		 * @return True if successful.
		 */
		bool exportJointsBinary();

		/**
		 * Exports the current joint transform of a joint to file.
		 *  Called from exportJoints().
		 * @return True if successful.
		 */
		bool exportJointTransform(MFnIkJoint &joint);

		/**
		 * Exports joint weights to file.
		 *  Stored on a joints per vertex basis.
		 * @return True if successful.
		 */
		bool exportJointWeights();

		/**
		 * Exports joint weights to file.
		 *  Stored on a joints per vertex basis.
		 * @return True if successful.
		 */
		bool exportJointWeightsBinary();

		/**
		 * Exports joint weights to file.
		 *  Stored on a joints per vertex basis.
		 *  Does not use indexed vertices.
		 * @return True if successful.
		 */
		bool exportJointWeightsNonIndexed();

		/**
		 * Exports joint weights to file.
		 *  Stored on a joints per vertex basis.
		 *  Does not use indexed vertices.
		 * @return True if successful.
		 */
		bool exportJointWeightsNonIndexedBinary();

		/**
		 * Exports animation names, keyframes and joint transforms.
		 * @return True if successful.
		 */
		bool exportAnimations();

		/**
		 * Exports animation names, keyframes and joint transforms.
		 * @return True if successful.
		 */
		bool exportAnimationsBinary();

		/**
		 * Exports joint transforms for all joints for the current time.
		 *  Called from exportAnimations().
		 * @return True if successful.
		 */
		bool exportJointTransforms();

		std::string mLoadPath;
		std::string mSavePath;
		std::fstream mExportGeometry;
		std::fstream mExportMaterial;
		std::fstream mExportAnimation;
		std::fstream mExportWeights;
		unsigned int mGeometryLocation;
		unsigned int mMaterialLocation;
		unsigned int mAnimationLocation;
		unsigned int mWeightLocation;
		MStatus mStatus;

		std::vector<MeshGeometry *> mMeshVector;
		std::vector<Animation *> mAnimationVector;
		MObjectArray mMaterialArray;
		MDagPathArray mJointArray;
};

#endif