/**
 * @BTMayaExporter.cpp
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

#include "BTMayaExporter.h"

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::string;
using std::fstream;
using std::ios_base;

BTMayaExporter::BTMayaExporter() : mLoadPath(), mSavePath(), mExportGeometry(),
								   mExportMaterial(), mExportAnimation(), mExportWeights(),
								   mStatus(), mMeshVector(), mAnimationVector(),
								   mMaterialArray(), mJointArray()
{
	mGeometryLocation = 0;
	mMaterialLocation = 0;
	mAnimationLocation = 0;
	mWeightLocation = 0;
}

BTMayaExporter::BTMayaExporter(const BTMayaExporter &exporter) : mLoadPath(), mSavePath(), mExportGeometry(),
																 mExportMaterial(), mExportAnimation(), mExportWeights(),
																 mStatus(), mMeshVector(), mAnimationVector(),
																 mMaterialArray(), mJointArray()
{
	mGeometryLocation = 0;
	mMaterialLocation = 0;
	mAnimationLocation = 0;
	mWeightLocation = 0;
}

BTMayaExporter::~BTMayaExporter()
{
	closeExportFiles();
	cleanUpMaya();
}

const BTMayaExporter &BTMayaExporter::operator=(const BTMayaExporter &exporter)
{
	if(this != &exporter)
	{
		mGeometryLocation = 0;
		mMaterialLocation = 0;
		mAnimationLocation = 0;
		mWeightLocation = 0;
	}
	return *this;
}

bool BTMayaExporter::startExporter()
{
	cout << "BugTanks Maya Exporter" << endl;
	if(!initializeMaya())
	{
		return false;
	}

	bool faceClockwise = true;

	while(true)
	{
		cout << "Enter filepath for .mb/.ma-file \n (use escape character such as C:\\\\Models\\\\model.mb): ";
		getline(cin, mLoadPath);

		exportModelBinary(mLoadPath.c_str(), faceClockwise);

		break;
	}
	return true;
}

bool BTMayaExporter::writeText(std::fstream &file, const std::string &text, unsigned int &location)
{
	if(file.is_open())
	{
		file.seekp(location);
		int size = text.size();
		file.write((const char *)&size, sizeof(int));
		location += sizeof(int);
		file.write((const char *)&text[0], size);
		location += size;
		return true;
	}
	return false;
}

bool BTMayaExporter::write(std::fstream &file, const char *pData, unsigned int dataSize, unsigned int &location)
{
	if(file.is_open())
	{
		file.seekp(location);
		file.write(pData, dataSize);
		location += dataSize;
		return true;
	}
	return false;
}

bool BTMayaExporter::getColor(MFnDependencyNode &materialNode, const std::string &colorType, D3DXCOLOR &color)
{
	MPlug colorPlug;
	string plugName;

	plugName = colorType + "R";
	colorPlug = materialNode.findPlug(MString(plugName.c_str()), true);
	color.r = colorPlug.asFloat(MDGContext::fsNormal);

	plugName = colorType + "G";
	colorPlug = materialNode.findPlug(MString(plugName.c_str()), true);
	color.g = colorPlug.asFloat(MDGContext::fsNormal);

	plugName = colorType + "B";
	colorPlug = materialNode.findPlug(MString(plugName.c_str()), true);
	color.b = colorPlug.asFloat(MDGContext::fsNormal);

	plugName = colorType + "A";
	colorPlug = materialNode.findPlug(MString(plugName.c_str()), true);
	//Note: always use 1.0f for alpha.
	color.a = 1.0f;
	return true;
}

bool BTMayaExporter::packMaterial(const MObject &material, MaterialData &data)
{
	data.mAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	data.mDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	data.mSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	switch(material.apiType())
	{
			case MFn::kLambert:
				getColor(MFnDependencyNode(material), string("ambientColor"), data.mAmbient);
				getColor(MFnDependencyNode(material), string("color"), data.mDiffuse);
				break;

			case MFn::kPhong:
				getColor(MFnDependencyNode(material), string("ambientColor"), data.mAmbient);
				getColor(MFnDependencyNode(material), string("color"), data.mDiffuse);
				getColor(MFnDependencyNode(material), string("specularColor"), data.mSpecular);
				break;

			case MFn::kBlinn:
				getColor(MFnDependencyNode(material), string("ambientColor"), data.mAmbient);
				getColor(MFnDependencyNode(material), string("color"), data.mDiffuse);
				getColor(MFnDependencyNode(material), string("specularColor"), data.mSpecular);
				break;

			default:
				return false;
	}
	return true;
}

bool BTMayaExporter::packMesh(MeshGeometry *pMesh, MeshData &data)
{
	data.mVertexCount = pMesh->mVertexArray.length();
	MItMeshPolygon polyIter = MItMeshPolygon(pMesh->mMesh[0]);
	data.mPolygonCount = polyIter.count();
	return true;
}

bool BTMayaExporter::packPolygon(MeshGeometry *pMesh, MItMeshPolygon &polygonIter, PolygonData &data)
{
	MFnMesh mesh = MFnMesh(pMesh->mMesh[0]);
	MIntArray indexArray;
	polygonIter.getVertices(indexArray);
	if(indexArray.length() == 3)
	{
		//Indices.
		data.mIndex1 = indexArray[0];
		data.mIndex2 = indexArray[1];
		data.mIndex3 = indexArray[2];

		//Normal.
		unsigned int normalIndex = polygonIter.normalIndex(0);
		D3DXVECTOR3 normal;
		normal.x = pMesh->mNormalArray[normalIndex].x;
		normal.y = pMesh->mNormalArray[normalIndex].y;
		normal.z = pMesh->mNormalArray[normalIndex].z;
		data.mNormal = normal;

		//Tangent.
		unsigned int tangentIndex = polygonIter.tangentIndex(0);
		D3DXVECTOR3 tangent;
		tangent.x = pMesh->mTangentArray[tangentIndex].x;
		tangent.y = pMesh->mTangentArray[tangentIndex].y;
		tangent.z = pMesh->mTangentArray[tangentIndex].z;
		data.mTangent = tangent;

		//Texture coordinates.
		int UVIndex = 0;
		D3DXVECTOR2 UV;
		polygonIter.getUVIndex(0, UVIndex);
		UV.x = pMesh->mUArray[UVIndex];
		UV.y = pMesh->mVArray[UVIndex];
		data.mUV1 = UV;
		polygonIter.getUVIndex(1, UVIndex);
		UV.x = pMesh->mUArray[UVIndex];
		UV.y = pMesh->mVArray[UVIndex];
		data.mUV2 = UV;
		polygonIter.getUVIndex(2, UVIndex);
		UV.x = pMesh->mUArray[UVIndex];
		UV.y = pMesh->mVArray[UVIndex];
		data.mUV3 = UV;

		//Material ID.
		MObjectArray shaderArray;
		MIntArray shaderIndexArray;
		mesh.getConnectedShaders(0, shaderArray, shaderIndexArray);

		int materialID = getPolygonMaterial(shaderArray, shaderIndexArray, polygonIter.index());
		data.mMaterialID = materialID;
	}
	else
	{
		cout << "Error: non-triangular polygon detected. Please only use triangles." << endl;
		cout << "Mesh: " << mesh.name() << endl;
		if(polygonIter.isPlanar())
		{
			cout << "Polygon is planar." << endl;
		}
		else
		{
			cout << "Polygon is not planar." << endl;
		}
		cout << "Attempts to continue export with missing polygon..." << endl;
	}
	return true;
}

bool BTMayaExporter::packJoint(MFnIkJoint &joint, int jointID, JointData &data)
{
	data.mID = jointID;
	data.mParentID = getJointParentID(joint);

	MVector translation;
	MQuaternion rotation;
	MQuaternion orientation;
	translation = joint.getTranslation(MSpace::kTransform);
	joint.getRotation(rotation);
	joint.getOrientation(orientation);

	D3DXVECTOR3 translationVector;
	translationVector.x = (float)translation.x;
	translationVector.y = (float)translation.y;
	translationVector.z = (float)translation.z;
	data.mTranslation = translationVector;

	D3DXVECTOR4 rotationVector;
	rotationVector.x = (float)rotation.x;
	rotationVector.y = (float)rotation.y;
	rotationVector.z = (float)rotation.z;
	rotationVector.w = (float)rotation.w;
	data.mRotation = rotationVector;

	D3DXVECTOR4 orientationVector;
	orientationVector.x = (float)orientation.x;
	orientationVector.y = (float)orientation.y;
	orientationVector.z = (float)orientation.z;
	orientationVector.w = (float)orientation.w;
	data.mOrientation = orientationVector;

	return true;
}

bool BTMayaExporter::initializeMaya()
{
	mStatus = MLibrary::initialize("BugTanks Maya Exporter", true);
	if(mStatus.error())
	{
		mStatus.perror("Error: MLibrary::initialize().");
		return false;
	}
	return true;
}

void BTMayaExporter::cleanUpMaya()
{
	for(vector<MeshGeometry *>::iterator meshIter = mMeshVector.begin();
		meshIter != mMeshVector.end(); ++meshIter)
	{
		if((*meshIter))
		{
			delete (*meshIter);
			(*meshIter) = NULL;
		}
	}
	for(vector<Animation *>::iterator animIter = mAnimationVector.begin();
		animIter != mAnimationVector.end(); ++animIter)
	{
		if((*animIter))
		{
			delete (*animIter);
			(*animIter) = NULL;
		}
	}
	MLibrary::cleanup(0);
}

bool BTMayaExporter::isBlendNode(MObject &node)
{
	if(node.hasFn(MFn::kBlendNodeBase))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeAdditiveRotation))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeAdditiveScale))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeBoolean))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeDouble))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeDoubleAngle))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeDoubleLinear))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeEnum))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeFloat))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeFloatAngle))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeFloatLinear))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeInt16))
	{
		return true;
	}
	if(node.hasFn(MFn::kBlendNodeInt32))
	{
		return true;
	}
	return false;
}

int BTMayaExporter::getPolygonMaterial(MObjectArray &shaderArray, MIntArray &shaderIndexArray, unsigned int polygonIndex)
{
	unsigned int shaderIndex = shaderIndexArray[polygonIndex];
	if(shaderIndex >= 0 && shaderIndex < shaderArray.length())
	{
		MPlug surfacePlug;
		MPlugArray surfaceArray;
		surfacePlug = MFnDependencyNode(shaderArray[shaderIndex]).findPlug(MString("surfaceShader"), true);
		surfacePlug.connectedTo(surfaceArray, true, false);
		if(surfaceArray.length() > 0)
		{
			for(unsigned int materialIndex = 0; materialIndex < mMaterialArray.length(); ++materialIndex)
			{
				if(mMaterialArray[materialIndex] == surfaceArray[0].node())
				{
					return (int)materialIndex;
				}
			}
		}
	}
	return -1;
}

void BTMayaExporter::addTimeUnique(MTime &time, MTimeArray &timeArray)
{
	for(unsigned int n = 0; n < timeArray.length(); ++n)
	{
		if(timeArray[n] == time)
		{
			return;
		}
	}
	timeArray.append(time);
}

void BTMayaExporter::sortTimeArray(MTimeArray &timeArray)
{

}

int BTMayaExporter::getJointParentID(MFnIkJoint &joint)
{
	MFnIkJoint parent = joint.parent(0);
	for(unsigned int n = 0; n < mJointArray.length(); ++n)
	{
		if(parent.object() == mJointArray[n].node())
		{
			return n;
		}
	}
	return -1;
}

bool BTMayaExporter::createExportFiles()
{
	//Find the index of the dot before the filetype.
	int subStringLength = mLoadPath.find_last_of(".", mLoadPath.size() - 1);

	//Savepath for the Geometry export file.
	mSavePath = mLoadPath.substr(0, subStringLength) + "Geometry.txt";

	cout << "Output file: " << mSavePath.c_str() << endl;

	mExportGeometry.open(mSavePath.c_str(), ios_base::out | ios_base::trunc);
	if(!mExportGeometry.is_open())
	{
		cout << "Error: fstream::open()." << endl;
		return false;
	}

	//Savepath for the Material export file.
	mSavePath = mLoadPath.substr(0, subStringLength) + "Material.txt";

	cout << "Output file: " << mSavePath.c_str() << endl;

	mExportMaterial.open(mSavePath.c_str(), ios_base::out | ios_base::trunc);
	if(!mExportMaterial.is_open())
	{
		cout << "Error: fstream::open()." << endl;
		return false;
	}

	//Savepath for the Animation export file.
	mSavePath = mLoadPath.substr(0, subStringLength) + "Animation.txt";

	cout << "Output file: " << mSavePath.c_str() << endl;

	mExportAnimation.open(mSavePath.c_str(), ios_base::out | ios_base::trunc);
	if(!mExportAnimation.is_open())
	{
		cout << "Error: fstream::open()." << endl;
		return false;
	}

	//Savepath for the Weights export file.
	mSavePath = mLoadPath.substr(0, subStringLength) + "Weights.txt";

	cout << "Output file: " << mSavePath.c_str() << endl;

	mExportWeights.open(mSavePath.c_str(), ios_base::out | ios_base::trunc);
	if(!mExportWeights.is_open())
	{
		cout << "Error: fstream::open()." << endl;
		return false;
	}
	return true;
}

bool BTMayaExporter::createExportFilesBinary()
{
	//Find the index of the dot before the filetype.
	int subStringLength = mLoadPath.find_last_of(".", mLoadPath.size() - 1);

	//Savepath for the Geometry export file.
	mSavePath = mLoadPath.substr(0, subStringLength) + "Geometry.btb";

	cout << "Output file: " << mSavePath.c_str() << endl;

	mExportGeometry.open(mSavePath.c_str(), ios_base::out | ios_base::binary | ios_base::trunc);
	if(!mExportGeometry.is_open())
	{
		cout << "Error: fstream::open()." << endl;
		return false;
	}

	//Savepath for the Material export file.
	mSavePath = mLoadPath.substr(0, subStringLength) + "Material.btb";

	cout << "Output file: " << mSavePath.c_str() << endl;

	mExportMaterial.open(mSavePath.c_str(), ios_base::out | ios_base::binary | ios_base::trunc);
	if(!mExportMaterial.is_open())
	{
		cout << "Error: fstream::open()." << endl;
		return false;
	}

	//Savepath for the Animation export file.
	mSavePath = mLoadPath.substr(0, subStringLength) + "Animation.btb";

	cout << "Output file: " << mSavePath.c_str() << endl;

	mExportAnimation.open(mSavePath.c_str(), ios_base::out | ios_base::binary | ios_base::trunc);
	if(!mExportAnimation.is_open())
	{
		cout << "Error: fstream::open()." << endl;
		return false;
	}

	//Savepath for the Weights export file.
	mSavePath = mLoadPath.substr(0, subStringLength) + "Weights.btb";

	cout << "Output file: " << mSavePath.c_str() << endl;

	mExportWeights.open(mSavePath.c_str(), ios_base::out | ios_base::binary | ios_base::trunc);
	if(!mExportWeights.is_open())
	{
		cout << "Error: fstream::open()." << endl;
		return false;
	}
	return true;
}

void BTMayaExporter::closeExportFiles()
{
	if(mExportGeometry.is_open())
	{
		mExportGeometry.close();
	}
	if(mExportMaterial.is_open())
	{
		mExportMaterial.close();
	}
	if(mExportAnimation.is_open())
	{
		mExportAnimation.close();
	}
	if(mExportWeights.is_open())
	{
		mExportWeights.close();
	}
}

bool BTMayaExporter::exportModel(const char *filePath, bool faceClockwise)
{
	mStatus = MFileIO::newFile(true);
	if(mStatus.error())
	{
		mStatus.perror("Error: MFileIO::newFile().");
		return false;
	}

	mStatus = MFileIO::open(filePath);
	if(mStatus.error())
	{
		mStatus.perror("Error: MFileIO::open().");
		return false;
	}

	//Run MEL-command to delete construction history.
	mStatus = MGlobal::executeCommand("delete -ch");
	if(mStatus.error())
	{
		mStatus.perror("Error: MGlobal::executeCommand().");
		return false;
	}

	if(!createExportFiles())
	{
		return false;
	}

	if(!saveMaterialInformation())
	{
		cout << "Failed to save material information." << endl;
		cout << "Unable to continue." << endl;
		return false;
	}
	if(!saveMeshInformation())
	{
		cout << "Failed to save mesh information." << endl;
		cout << "Unable to continue." << endl;
		return false;
	}
	if(!saveJointInformation())
	{
		cout << "Failed to save joint information." << endl;
		cout << "Continues..." << endl;
	}
	if(!saveJointWeightInformation())
	{
		cout << "Failed to save joint weight information." << endl;
		cout << "Continues..." << endl;
	}
	if(!saveAnimationInformation())
	{
		cout << "Failed to save animation information." << endl;
		cout << "Continues..." << endl;
	}

	if(!exportMaterials())
	{
		cout << "Failed to export materials." << endl;
	}
	if(!exportGeometry())
	{
		cout << "Failed to export geometry." << endl;
	}
	if(!exportAnimations())
	{
		cout << "Failed to export animations." << endl;
	}
	if(!exportJointWeights())
	{
		cout << "Failed to export joint weights." << endl;
	}

	return true;
}

bool BTMayaExporter::exportModelBinary(const char *filePath, bool faceClockwise)
{
	mStatus = MFileIO::newFile(true);
	if(mStatus.error())
	{
		mStatus.perror("Error: MFileIO::newFile().");
		return false;
	}

	mStatus = MFileIO::open(filePath);
	if(mStatus.error())
	{
		mStatus.perror("Error: MFileIO::open().");
		return false;
	}

	//Run MEL-command to delete construction history.
	mStatus = MGlobal::executeCommand("delete -ch");
	if(mStatus.error())
	{
		mStatus.perror("Error: MGlobal::executeCommand().");
		return false;
	}

	if(!createExportFilesBinary())
	{
		return false;
	}

	if(!saveMaterialInformation())
	{
		cout << "Failed to save material information." << endl;
		cout << "Unable to continue." << endl;
		return false;
	}
	if(!saveMeshInformation())
	{
		cout << "Failed to save mesh information." << endl;
		cout << "Unable to continue." << endl;
		return false;
	}
	if(!saveJointInformation())
	{
		cout << "Failed to save joint information." << endl;
		cout << "Continues..." << endl;
	}
	if(!saveJointWeightInformation())
	{
		cout << "Failed to save joint weight information." << endl;
		cout << "Continues..." << endl;
	}
	if(!saveAnimationInformation())
	{
		cout << "Failed to save animation information." << endl;
		cout << "Continues..." << endl;
	}

	if(!exportMaterialsBinary())
	{
		cout << "Failed to export materials." << endl;
	}
	if(!exportGeometryBinary())
	{
		cout << "Failed to export geometry." << endl;
	}
	if(!exportAnimationsBinary())
	{
		cout << "Failed to export animations." << endl;
	}
	if(!exportJointWeightsBinary())
	{
		cout << "Failed to export joint weights." << endl;
	}

	return true;
}

bool BTMayaExporter::saveMaterialInformation()
{
	//Search for material nodes (derives from kLambert).
	MItDependencyNodes materialIter(MFn::kLambert);
	for(; !materialIter.isDone(); materialIter.next())	
	{
		mMaterialArray.append(materialIter.thisNode());
	}
	return true;
}

bool BTMayaExporter::saveMeshInformation()
{
	//Find mesh nodes.
	MDagPath DAGPath;
	MItDag DAGIter(MItDag::kBreadthFirst, MFn::kMesh);
	for(; !DAGIter.isDone(); DAGIter.next())
	{
		DAGIter.getPath(DAGPath);
		MFnDagNode DAGNode = DAGPath.node();

		//Ignore intermediate objects as they aren't rendered.
		if(DAGNode.isIntermediateObject())
		{
			continue;
		}

		MeshGeometry *meshStorage = new MeshGeometry();
		//Store DAG-path and geometry information.
		MFnMesh mesh(DAGPath);
		meshStorage->mMesh.append(mesh.dagPath());
		mesh.getPoints(meshStorage->mVertexArray, MSpace::kObject);
		mesh.getNormals(meshStorage->mNormalArray, MSpace::kObject);
		mesh.getTangents(meshStorage->mTangentArray, MSpace::kObject);
		mesh.getUVs(meshStorage->mUArray, meshStorage->mVArray);

		mMeshVector.push_back(meshStorage);
	}
	return true;
}

bool BTMayaExporter::saveJointInformation()
{
	MDagPath DAGPath;

	//Search for joints and update the jointArray.
	MItDag DAGIter(MItDag::kBreadthFirst, MFn::kJoint);
	for(; !DAGIter.isDone(); DAGIter.next())
	{
		DAGIter.getPath(DAGPath);
		mJointArray.append(DAGPath);
	}
	return true;
}

bool BTMayaExporter::saveJointWeightInformation()
{
	//Go through each mesh.
	for(vector<MeshGeometry *>::iterator meshIter = mMeshVector.begin();
		meshIter != mMeshVector.end(); ++meshIter)
	{
		MeshGeometry *meshStorage = (*meshIter);
		MDagPath meshDAG = meshStorage->mMesh[0];

		//There should only be one skin per mesh.
		MItDependencyNodes skinIter(MFn::kSkinClusterFilter);
		for(; !skinIter.isDone(); skinIter.next())
		{
			MFnSkinCluster skinCluster(skinIter.thisNode());
			MDagPathArray influenceArray;
			skinCluster.influenceObjects(influenceArray);

			//Iterate through each vertex in the mesh.
			MItGeometry vertexIter(meshDAG);
			for(; !vertexIter.isDone(); vertexIter.next())
			{
				for(unsigned int jointIndex = 0; jointIndex < mJointArray.length(); ++jointIndex)
				{
					MDagPath jointDAG = mJointArray[jointIndex];

					//The joint's index in the cluster's influence array.
					unsigned int influenceIndex = skinCluster.indexForInfluenceObject(jointDAG);

					MDoubleArray vertexWeight;
					mStatus = skinCluster.getWeights(meshDAG, vertexIter.currentItem(), influenceIndex, vertexWeight);
					if(!mStatus.error())
					{
						meshStorage->mJointWeights.append(vertexWeight[0]);
					}
				}
			}
		}
	}
	return true;
}

bool BTMayaExporter::saveAnimationInformation()
{
	MString animName = "";
	//Find animation name.
	MItDependencyNodes DGLayerIter(MFn::kAnimLayer);
	for(; !DGLayerIter.isDone(); DGLayerIter.next())
	{
		MPlug plugOverride;
		MFnDependencyNode DGLayerNode(DGLayerIter.thisNode());
		//Ignore BaseAnimation-layer.
		if(DGLayerNode.name() == "BaseAnimation")
		{
			continue;
		}

		//Use the name of the first override-layer as animation name.
		plugOverride = DGLayerNode.findPlug("override", true);
		if(plugOverride.asBool(MDGContext::fsNormal))
		{
			animName = DGLayerNode.name();
			break;
		}
	}

	if(animName == "")
	{
		cout << "Override layer not found." << endl;
		return false;
	}

	Animation *animationStorage = new Animation();
	animationStorage->mName = animName;

	//Store keyframes.
	//Go through all stored joints.
	for(unsigned int jointIndex = 0; jointIndex < mJointArray.length(); ++jointIndex)
	{
		//Look after kAnimCurves.
		MItDependencyGraph jointConnections(mJointArray[jointIndex].node(), MFn::kAnimCurve,
											MItDependencyGraph::kUpstream,
											MItDependencyGraph::kDepthFirst,
											MItDependencyGraph::kNodeLevel);
		for(; !jointConnections.isDone(); jointConnections.next())
		{
			MFnAnimCurve animCurve(jointConnections.currentItem());
			MItKeyframe keyframeIter(animCurve.object());
			//Go through all keyframes for the animation curve.
			for(; !keyframeIter.isDone(); keyframeIter.next())
			{
				//Store all *new* keyframes in the mKeyframeArray.
				addTimeUnique(keyframeIter.time(), animationStorage->mKeyframeArray);
			}
		}
	}
	mAnimationVector.push_back(animationStorage);
	return true;
}

bool BTMayaExporter::exportMaterials()
{
	mExportMaterial << "NumberOfMaterials: " << mMaterialArray.length() << LINEBREAK;
	MPlug colorPlug;
	for(unsigned int n = 0; n < mMaterialArray.length(); ++n)
	{
		switch(mMaterialArray[n].apiType())
		{
			case MFn::kLambert:
				mExportMaterial << "LambertMaterial" << LINEBREAK;
				mExportMaterial << "MaterialID: " << n << LINEBREAK;
				mExportMaterial << "Ambient: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("ambientColor"));
				mExportMaterial << "Diffuse: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("color"));
				mExportMaterial << "Incandescence: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("incandescence"));
				mExportMaterial << "Transparency: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("transparency"));
				exportTexture(MFnDependencyNode(mMaterialArray[n]));
				mExportMaterial << LINEBREAK;
				break;

			case MFn::kPhong:
				mExportMaterial << "PhongMaterial" << LINEBREAK;
				mExportMaterial << "MaterialID: " << n << LINEBREAK;
				mExportMaterial << "Ambient: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("ambientColor"));
				mExportMaterial << "Diffuse: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("color"));
				mExportMaterial << "Specular: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("specularColor"));
				mExportMaterial << "Incandescence: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("incandescence"));
				mExportMaterial << "Transparency: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("transparency"));
				exportTexture(MFnDependencyNode(mMaterialArray[n]));
				mExportMaterial << LINEBREAK;
				break;

			case MFn::kBlinn:
				mExportMaterial << "BlinnMaterial" << LINEBREAK;
				mExportMaterial << "MaterialID: " << n << LINEBREAK;
				mExportMaterial << "Ambient: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("ambientColor"));
				mExportMaterial << "Diffuse: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("color"));
				mExportMaterial << "Specular: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("specularColor"));
				mExportMaterial << "Incandescence: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("incandescence"));
				mExportMaterial << "Transparency: ";
				exportColor(MFnDependencyNode(mMaterialArray[n]), string("transparency"));
				exportTexture(MFnDependencyNode(mMaterialArray[n]));
				mExportMaterial << LINEBREAK;
				break;
		}
	}
	return true;
}

bool BTMayaExporter::exportMaterialsBinary()
{
	//Write the number of materials.
	unsigned int materialCount = mMaterialArray.length();
	write(mExportMaterial, (const char *)&materialCount, sizeof(int), mMaterialLocation);

	MPlug colorPlug;
	for(unsigned int n = 0; n < materialCount; ++n)
	{
		//Pack the color information for the material.
		MaterialData data;
		if(!packMaterial(mMaterialArray[n], data))
		{
			return false;
		}

		//Write the color information.
		write(mExportMaterial, (const char *)&data, sizeof(MaterialData), mMaterialLocation);
	}
	return true;
}

bool BTMayaExporter::exportColor(MFnDependencyNode &materialNode, const string &colorType)
{
	MColor color;
	MPlug colorPlug;
	string plugName;

	plugName = colorType + "R";
	colorPlug = materialNode.findPlug(MString(plugName.c_str()), true);
	color.r = colorPlug.asFloat(MDGContext::fsNormal);

	plugName = colorType + "G";
	colorPlug = materialNode.findPlug(MString(plugName.c_str()), true);
	color.g = colorPlug.asFloat(MDGContext::fsNormal);

	plugName = colorType + "B";
	colorPlug = materialNode.findPlug(MString(plugName.c_str()), true);
	color.b = colorPlug.asFloat(MDGContext::fsNormal);

	plugName = colorType + "A";
	colorPlug = materialNode.findPlug(MString(plugName.c_str()), true);
	color.a = colorPlug.asFloat(MDGContext::fsNormal);

	mExportMaterial << color.r << ' ' << color.g << ' ' << color.b << ' ' << color.a << LINEBREAK;
	return true;
}

bool BTMayaExporter::exportTexture(MFnDependencyNode &diffuseChannel)
{
	MPlug colorPlug;
	MPlugArray colorPlugArray;
	colorPlug = diffuseChannel.findPlug(MString("color"), true);
	colorPlug.connectedTo(colorPlugArray, true, false);

	bool foundTexture = false;
	for(unsigned int n = 0; n < colorPlugArray.length(); ++n)
	{
		if(colorPlugArray[n].node().hasFn(MFn::kFileTexture))
		{
			MFnDependencyNode textureNode(colorPlugArray[n].node());
			MPlug texturePathPlug = textureNode.findPlug("fileTextureName", true);
			MString texturePathString = texturePathPlug.asString(MDGContext::fsNormal);
			int start = texturePathString.rindexW('/') + 1;
			MString relativeTexturePath = texturePathString.substring(start, texturePathString.length());
			mExportMaterial << "Texture: " << relativeTexturePath.asChar() << LINEBREAK;
			foundTexture = true;
			break;
		}
	}
	if(!foundTexture)
	{
		mExportMaterial << "Texture: None" << LINEBREAK;
	}
	return true;
}

bool BTMayaExporter::exportGeometry()
{
	mExportGeometry << "NumberOfMeshes: " << mMeshVector.size() << LINEBREAK;
	for(vector<MeshGeometry *>::iterator meshIter = mMeshVector.begin();
		meshIter != mMeshVector.end(); ++meshIter)
	{
		MeshGeometry *meshStorage = (*meshIter);
		MFnMesh mesh = MFnMesh(meshStorage->mMesh[0]);
		mExportGeometry << "MeshName: " << mesh.name().asChar() << LINEBREAK;
		mExportGeometry << "NumberOfVertices: " << meshStorage->mVertexArray.length() << LINEBREAK;
		for(unsigned int n = 0; n < meshStorage->mVertexArray.length(); ++n)
		{
			meshStorage->mVertexArray[n].cartesianize();
			mExportGeometry << meshStorage->mVertexArray[n].x << ' ' << meshStorage->mVertexArray[n].y
						<< ' ' << meshStorage->mVertexArray[n].z << LINEBREAK;
		}
		mExportGeometry << LINEBREAK;

		MItMeshPolygon polygonIter(mesh.dagPath(), MObject::kNullObj);
		int normalIndex = 0;
		int tangentIndex = 0;
		int UVIndex = 0;
		MObjectArray shaderArray;
		MIntArray shaderIndexArray;
		mesh.getConnectedShaders(0, shaderArray, shaderIndexArray);
		mExportGeometry << "NumberOfPolygons: " << polygonIter.count() << LINEBREAK;
		for(; !polygonIter.isDone(); polygonIter.next())
		{
			MIntArray indexArray;
			polygonIter.getVertices(indexArray);
			if(indexArray.length() == 3)
			{
				mExportGeometry << "VertexIndicies: ";
				mExportGeometry << indexArray[0] << ' ' << indexArray[1] << ' '
						<< indexArray[2] << LINEBREAK;

				mExportGeometry << "Normal: ";
				normalIndex = polygonIter.normalIndex(0);
				mExportGeometry << meshStorage->mNormalArray[normalIndex].x << ' ' << meshStorage->mNormalArray[normalIndex].y
						<< ' ' << meshStorage->mNormalArray[normalIndex].z << LINEBREAK;

				mExportGeometry << "Tangent: ";
				tangentIndex = polygonIter.tangentIndex(0);
				mExportGeometry << meshStorage->mTangentArray[tangentIndex].x << ' ' << meshStorage->mTangentArray[tangentIndex].y
						<< ' ' << meshStorage->mTangentArray[tangentIndex].z << LINEBREAK;

				mExportGeometry << "UV1: ";
				polygonIter.getUVIndex(0, UVIndex);
				mExportGeometry << meshStorage->mUArray[UVIndex] << ' ' << meshStorage->mVArray[UVIndex] << ' ' << LINEBREAK;

				mExportGeometry << "UV2: ";
				polygonIter.getUVIndex(1, UVIndex);
				mExportGeometry << meshStorage->mUArray[UVIndex] << ' ' << meshStorage->mVArray[UVIndex] << ' ' << LINEBREAK;

				mExportGeometry << "UV3: ";
				polygonIter.getUVIndex(2, UVIndex);
				mExportGeometry << meshStorage->mUArray[UVIndex] << ' ' << meshStorage->mVArray[UVIndex] << LINEBREAK;

				mExportGeometry << "MaterialIndex: ";
				mExportGeometry << getPolygonMaterial(shaderArray, shaderIndexArray, polygonIter.index()) << LINEBREAK;
				mExportGeometry << LINEBREAK;
			}
			else
			{
				cout << "Error: non-triangular polygon detected. Please only use triangles." << endl;
				cout << "Mesh: " << mesh.name() << endl;
				if(polygonIter.isPlanar())
				{
					cout << "Polygon is planar." << endl;
				}
				else
				{
					cout << "Polygon is not planar." << endl;
				}
				cout << "Attempts to continue export with missing polygon..." << endl;
			}
		}
		mExportGeometry << LINEBREAK;
	}
	return true;
}

bool BTMayaExporter::exportGeometryBinary()
{
	//Write number of meshes.
	unsigned int meshCount = mMeshVector.size();
	write(mExportGeometry, (const char *)&meshCount, sizeof(unsigned int), mGeometryLocation);

	for(vector<MeshGeometry *>::iterator meshIter = mMeshVector.begin();
		meshIter != mMeshVector.end(); ++meshIter)
	{
		MFnMesh mesh = MFnMesh((*meshIter)->mMesh[0]);

		//Write name.
		string name = mesh.name().asChar();
		writeText(mExportGeometry, name, mGeometryLocation);

		//Write mesh data.
		MeshData meshData;
		packMesh((*meshIter), meshData);
		write(mExportGeometry, (const char *)&meshData, sizeof(MeshData), mGeometryLocation);

		for(unsigned int n = 0; n < (*meshIter)->mVertexArray.length(); ++n)
		{
			(*meshIter)->mVertexArray[n].cartesianize();

			D3DXVECTOR3 position;
			position.x = (*meshIter)->mVertexArray[n].x;
			position.y = (*meshIter)->mVertexArray[n].y;
			position.z = (*meshIter)->mVertexArray[n].z;

			//Write vertex position.
			write(mExportGeometry, (const char *)&position, sizeof(D3DXVECTOR3), mGeometryLocation);
		}

		MItMeshPolygon polygonIter(mesh.dagPath(), MObject::kNullObj);
		for(; !polygonIter.isDone(); polygonIter.next())
		{
			//Write polygon data.
			PolygonData polygonData;
			packPolygon((*meshIter), polygonIter, polygonData);
			write(mExportGeometry, (const char *)&polygonData, sizeof(PolygonData), mGeometryLocation);
		}

	}

	return true;
}

bool BTMayaExporter::exportJoints()
{
	mExportAnimation << "NumberOfJoints: " << mJointArray.length() << LINEBREAK;
	//Move joints to bind pose.
	MGlobal::executeCommand(MString("dagPose -restore -global -bindPose"));
	int parentID = -1;
	for(unsigned int n = 0; n < mJointArray.length(); ++n)
	{
		MFnIkJoint joint = MFnIkJoint(mJointArray[n]);
		parentID = this->getJointParentID(joint);
		mExportAnimation << "JointID: " << n << LINEBREAK;
		mExportAnimation << "ParentID: " << parentID << LINEBREAK;
		mExportAnimation << "JointName: " << joint.name().asChar() << LINEBREAK;
		exportJointTransform(joint);
	}
	return true;
}

bool BTMayaExporter::exportJointsBinary()
{
	//Write joint count.
	unsigned int jointCount = mJointArray.length();
	write(mExportAnimation, (const char *)&jointCount, sizeof(unsigned int), mAnimationLocation);

	//Move joints to bind pose.
	MGlobal::executeCommand(MString("dagPose -restore -global -bindPose"));

	//Write all joints.
	for(unsigned int n = 0; n < mJointArray.length(); ++n)
	{
		MFnIkJoint joint = MFnIkJoint(mJointArray[n]);
		JointData data;
		packJoint(joint, n, data);
		write(mExportAnimation, (const char *)&data, sizeof(JointData), mAnimationLocation);
	}
	return true;
}

bool BTMayaExporter::exportJointTransform(MFnIkJoint &joint)
{
	MVector translation;
	//Use quaternions instead of matrices.
	MQuaternion rotation;
	MQuaternion orientation;
	translation = joint.getTranslation(MSpace::kTransform);
	joint.getRotation(rotation);
	joint.getOrientation(orientation);

	mExportAnimation << "Translation: ";
	mExportAnimation << translation.x << ' ' << translation.y
					 << ' ' << translation.z << LINEBREAK;
	mExportAnimation << "Rotation: ";
	mExportAnimation << rotation.x << ' ' << rotation.y << ' '
					 << rotation.z << ' ' << rotation.w << LINEBREAK;
	mExportAnimation << "Orientation: ";
	mExportAnimation << orientation.x << ' ' << orientation.y << ' '
					 << orientation.z << ' ' << orientation.w << LINEBREAK;
	mExportAnimation << LINEBREAK;

	return true;
}

bool BTMayaExporter::exportJointWeights()
{
	mExportWeights << "NumberOfJoints: " << mJointArray.length() << LINEBREAK;
	mExportWeights << "NumberOfMeshes: " << mMeshVector.size() << LINEBREAK;
	double weight = 0.0f;
	//Iterate through each mesh.
	for(vector<MeshGeometry *>::iterator meshIter = mMeshVector.begin();
		meshIter != mMeshVector.end(); ++meshIter)
	{
		mExportWeights << "NumberOfVertices: " << (*meshIter)->mVertexArray.length() << LINEBREAK;
		//Iterate through each vertex.
		for(unsigned int nVertex = 0; nVertex < (*meshIter)->mVertexArray.length(); ++nVertex)
		{
			mExportWeights << "VertexID: " << nVertex << LINEBREAK;
			//Weights are stored on a joints per vertex basis.
			for(unsigned int nJoint = 0; nJoint < mJointArray.length(); ++nJoint)
			{
				//Weight for the current vertex/joint.
				weight = (*meshIter)->mJointWeights[nVertex * mJointArray.length() + nJoint];
				mExportWeights << "JointID: " << nJoint << LINEBREAK;
				mExportWeights << "JointWeight: " << weight << LINEBREAK;
			}
		}
	}
	return true;
}

bool BTMayaExporter::exportJointWeightsBinary()
{
	//Write joint count.
	unsigned int jointCount = mJointArray.length();
	write(mExportWeights, (const char *)&jointCount, sizeof(unsigned int), mWeightLocation);

	//Write mesh count.
	unsigned int meshCount = mMeshVector.size();
	write(mExportWeights, (const char *)&meshCount, sizeof(unsigned int), mWeightLocation);

	float weight = 0.0f;
	//Iterate through each mesh.
	for(vector<MeshGeometry *>::iterator meshIter = mMeshVector.begin();
		meshIter != mMeshVector.end(); ++meshIter)
	{
		//Write number of vertices.
		unsigned int vertexCount = (*meshIter)->mVertexArray.length();
		write(mExportWeights, (const char *)&vertexCount, sizeof(unsigned int), mWeightLocation);

		//Iterate through each vertex.
		for(unsigned int nVertex = 0; nVertex < (*meshIter)->mVertexArray.length(); ++nVertex)
		{
			//Write vertex index.
			write(mExportWeights, (const char *)&nVertex, sizeof(unsigned int), mWeightLocation);

			//Weights are stored on a joints per vertex basis.
			for(unsigned int nJoint = 0; nJoint < mJointArray.length(); ++nJoint)
			{
				//Write joint index.
				write(mExportWeights, (const char *)&nJoint, sizeof(unsigned int), mWeightLocation);

				//Wwrite for the current vertex/joint.
				weight = (float)(*meshIter)->mJointWeights[nVertex * mJointArray.length() + nJoint];
				write(mExportWeights, (const char *)&weight, sizeof(float), mWeightLocation);
			}
		}
	}
	return true;
}

bool BTMayaExporter::exportJointWeightsNonIndexed()
{
	mExportWeights << "NumberOfJoints: " << mJointArray.length() << LINEBREAK;
	mExportWeights << "NumberOfMeshes: " << mMeshVector.size() << LINEBREAK;
	double weight = 0.0f;
	int nVertex = 0;
	int nIndex = 0;
	//Iterate through each mesh.
	for(vector<MeshGeometry *>::iterator meshIter = mMeshVector.begin();
		meshIter != mMeshVector.end(); ++meshIter)
	{
		MItMeshPolygon polygonIter((*meshIter)->mMesh[0], MObject::kNullObj);
		mExportWeights << "NumberOfVertices: " << polygonIter.count() * 3 << LINEBREAK;
		//Iterate through each polygon.
		for(; !polygonIter.isDone(); polygonIter.next())
		{
			//Retrieve indices of the polygon's vertices.
			MIntArray indexArray;
			polygonIter.getVertices(indexArray);
			if(indexArray.length() == 3)
			{
				//Iterate through the three vertices in the polygon.
				for(int n = 0; n < 3; ++n)
				{
					mExportWeights << "VertexID: " << nVertex << LINEBREAK;
					//Weights are stored on a joints per vertex basis.
					for(unsigned int nJoint = 0; nJoint < mJointArray.length(); ++nJoint)
					{
						//Weight for the current vertex/joint.
						weight = (*meshIter)->mJointWeights[indexArray[n] * mJointArray.length() + nJoint];
						mExportWeights << "JointID: " << nJoint << LINEBREAK;
						mExportWeights << "JointWeight: " << weight << LINEBREAK;
					}
					++nVertex;
				}
			}
			else
			{
				cout << "Error: non-triangular polygon detected. Please only use triangles." << endl;
				return false;
			}
		}
	}
	return true;
}

bool BTMayaExporter::exportJointWeightsNonIndexedBinary()
{
	return true;
}

bool BTMayaExporter::exportAnimations()
{
	exportJoints();

	mExportAnimation << "NumberOfAnimations: " << mAnimationVector.size() << LINEBREAK;
	for(vector<Animation *>::iterator animIter = mAnimationVector.begin();
		animIter != mAnimationVector.end(); ++animIter)
	{
		Animation *animStorage = (*animIter);
		mExportAnimation << "AnimationName: " << animStorage->mName.asChar() << LINEBREAK;

		double startTime = MAnimControl::animationStartTime().asUnits(TIME_SAMPLE);
		double endTime = MAnimControl::animationEndTime().asUnits(TIME_SAMPLE);
		mExportAnimation << "StartTime: " << startTime << LINEBREAK;
		mExportAnimation << "EndTime: " << endTime << LINEBREAK;
		mExportAnimation << "NumberOfKeyframes: " << animStorage->mKeyframeArray.length() << LINEBREAK;
		mExportAnimation << LINEBREAK;

		//Use MAnimControl to set the current animation time and sample joint transforms.
		for(unsigned int n = 0; n < animStorage->mKeyframeArray.length(); ++n)
		{
			mExportAnimation << "Keyframe: " << animStorage->mKeyframeArray[n].asUnits(TIME_SAMPLE) << LINEBREAK;
			MAnimControl::setCurrentTime(animStorage->mKeyframeArray[n]);
			exportJointTransforms();
		}
	}
	return true;
}

bool BTMayaExporter::exportAnimationsBinary()
{
	//Export the joints in bindpose.
	exportJointsBinary();

	for(vector<Animation *>::iterator animIter = mAnimationVector.begin();
		animIter != mAnimationVector.end(); ++animIter)
	{
		Animation *animStorage = (*animIter);
		mExportAnimation << "AnimationName: " << animStorage->mName.asChar() << LINEBREAK;

		//Write animation name.
		string name = animStorage->mName.asChar();
		writeText(mExportAnimation, name, mAnimationLocation);

		float startTime = (float)MAnimControl::animationStartTime().asUnits(TIME_SAMPLE);
		float endTime = (float)MAnimControl::animationEndTime().asUnits(TIME_SAMPLE);
		unsigned int frameCount = animStorage->mKeyframeArray.length();

		//Write start time.
		write(mExportAnimation, (const char *)&startTime, sizeof(float), mAnimationLocation);

		//Write end time.
		write(mExportAnimation, (const char *)&endTime, sizeof(float), mAnimationLocation);

		//Write frame count.
		write(mExportAnimation, (const char *)&frameCount, sizeof(unsigned int), mAnimationLocation);

		//Use MAnimControl to set the current animation time and sample joint transforms.
		for(unsigned int n = 0; n < animStorage->mKeyframeArray.length(); ++n)
		{
			float frameTime = (float)animStorage->mKeyframeArray[n].asUnits(TIME_SAMPLE);
			write(mExportAnimation, (const char *)&frameTime, sizeof(float), mAnimationLocation);

			MAnimControl::setCurrentTime(animStorage->mKeyframeArray[n]);
			//Write all joints.
			for(unsigned int n = 0; n < mJointArray.length(); ++n)
			{
				MFnIkJoint joint = MFnIkJoint(mJointArray[n]);
				JointData data;
				packJoint(joint, n, data);
				write(mExportAnimation, (const char *)&data, sizeof(JointData), mAnimationLocation);
			}
		}

		//Only export first animation.
		break;
	}
	return true;
}

bool BTMayaExporter::exportJointTransforms()
{
	MMatrix transform;
	MVector translation;
	//Use quaternions instead of matrices.
	MQuaternion rotation;
	MQuaternion orientation;
	for(unsigned int n = 0; n < mJointArray.length(); ++n)
	{
		MFnIkJoint joint(mJointArray[n]);
		translation = joint.getTranslation(MSpace::kTransform);
		joint.getRotation(rotation);
		joint.getOrientation(orientation);

		mExportAnimation << LINEBREAK;
		mExportAnimation << "JointID: " << n << LINEBREAK;
		mExportAnimation << "Translation: ";
		mExportAnimation << translation.x << ' ' << translation.y
						 << ' ' << translation.z << LINEBREAK;
		mExportAnimation << "Rotation: ";
		mExportAnimation << rotation.x << ' ' << rotation.y << ' '
						 << rotation.z << ' ' << rotation.w << LINEBREAK;
		mExportAnimation << "Orientation: ";
		mExportAnimation << orientation.x << ' ' << orientation.y << ' '
						 << orientation.z << ' ' << orientation.w << LINEBREAK;
		mExportAnimation << LINEBREAK;
	}
	return true;
}