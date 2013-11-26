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

#include "Animation.h"

using namespace gfx;

Animation::Animation(ID3D10Device * pDevice, std::string name)
{
	mTimePerFrame = 1.0f;
	mName = name;

	mpDevice = pDevice;

	/*std::ifstream file = std::ifstream();
	file.open(filename.c_str());

	mFrames = std::vector<Frame*>();

	std::vector<Joint*> joints = std::vector<Joint*>();

	std::string input = "";
	bool bindPose = true;
	unsigned int jointID = 0;
	D3DXVECTOR3 translation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQUATERNION rotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXQUATERNION orientation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);

	while(!file.eof())
	{
		file >> input;

		if(input=="Keyframe:")
		{
			joints[0]->update(true);

			if(bindPose)
			{
				joints[0]->setBindPose();
				bindPose = false;
			}
			else
			{
				Frame * frame = myNew Frame();

				for(unsigned int i=0;i<joints.size();i++)
				{
					D3DXMATRIX matrix = joints[i]->getBindInverse() * joints[i]->getToWorld();
					frame->addMatrix(matrix);
				}

				mFrames.push_back(frame);
			}
		}
		else if(input=="JointID:")
		{
			
			file >> input;
			jointID = atoi(input.c_str());

			if(bindPose)
			{
				joints.push_back(myNew Joint());
			}
		}
		else if(input=="ParentID:")
		{
			if(bindPose)
			{
				file >> input;
				int parentID = atoi(input.c_str());

				if(parentID >= 0)
				{
					joints[parentID]->addChild(joints[jointID]);
				}
			}
		}
		else if(input=="Translation:")
		{
			file >> input;
			translation.x = (float)atof(input.c_str());
			file >> input;
			translation.y = (float)atof(input.c_str());
			file >> input;
			translation.z = (float)atof(input.c_str());

			joints[jointID]->setPosition(translation);
		}
		else if(input=="Rotation:")
		{
			file >> input;
			rotation.x = (float)atof(input.c_str());
			file >> input;
			rotation.y = (float)atof(input.c_str());
			file >> input;
			rotation.z = (float)atof(input.c_str());
			file >> input;
			rotation.w = (float)atof(input.c_str());

			joints[jointID]->setRotation(rotation);
		}
		else if(input=="Orientation:")
		{
			file >> input;
			orientation.x = (float)atof(input.c_str());
			file >> input;
			orientation.y = (float)atof(input.c_str());
			file >> input;
			orientation.z = (float)atof(input.c_str());
			file >> input;
			orientation.w = (float)atof(input.c_str());

			joints[jointID]->setOrientation(orientation);
		}

	}
	
	SAFE_DELETE(joints[0]);
	
	file.close();*/
}

Animation::~Animation()
{
	for(unsigned int i=0;i<mFrames.size();i++)
	{
		SAFE_DELETE(mFrames[i]);
	}
}

void Animation::addFrame(Frame *pFrame)
{
	mFrames.push_back(pFrame);
}

Frame * Animation::getFrame(unsigned int index) const
{
	return mFrames[index];
}

unsigned int Animation::getSize() const
{
	return mFrames.size();
}

ID3D10ShaderResourceView * Animation::getAsTexture() const
{
	D3D10_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D10_TEXTURE2D_DESC));

	desc.Width = 1024;
	desc.Height = 1024;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D10_USAGE_DYNAMIC;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	ID3D10Texture2D *pTexture = NULL;
	mpDevice->CreateTexture2D(&desc, NULL, &pTexture);

	D3D10_MAPPED_TEXTURE2D mappedTex;
	pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex);

	unsigned int nrOfJoints = mFrames[0]->getSize();

	float* pTexels = (float*)mappedTex.pData;

	pTexels[0] = (float)mFrames.size();
	pTexels[1] = mTimePerFrame;

	for(unsigned int frameIndex = 0; frameIndex < mFrames.size(); frameIndex++)
	{
		for(unsigned int matrixIndex = 0; matrixIndex < nrOfJoints; matrixIndex++)
		{
			unsigned int index = 1024 + frameIndex * 1024 + matrixIndex * 16;
			D3DXMATRIX matrix = mFrames[frameIndex]->getMatrix(matrixIndex);
			
			pTexels[index + 0] = matrix._11;
			pTexels[index + 1] = matrix._12;
			pTexels[index + 2] = matrix._13;
			pTexels[index + 3] = matrix._14;
			pTexels[index + 4] = matrix._21;
			pTexels[index + 5] = matrix._22;
			pTexels[index + 6] = matrix._23;
			pTexels[index + 7] = matrix._24;
			pTexels[index + 8] = matrix._31;
			pTexels[index + 9] = matrix._32;
			pTexels[index + 10] = matrix._33;
			pTexels[index + 11] = matrix._34;
			pTexels[index + 12] = matrix._41;
			pTexels[index + 13] = matrix._42;
			pTexels[index + 14] = matrix._43;
			pTexels[index + 15] = matrix._44;
		}
	}

	pTexture->Unmap( D3D10CalcSubresource(0, 0, 1) );

	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC));

	viewDesc.Format = desc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;

	ID3D10ShaderResourceView * pResourceView;

	mpDevice->CreateShaderResourceView(pTexture, &viewDesc, &pResourceView);

	pTexture->Release();

	return pResourceView;
}

void Animation::setTimePerFrame(float timePerFrame)
{
	mTimePerFrame = timePerFrame;
}

float Animation::getTimePerFrame() const
{
	return mTimePerFrame;
}

std::string Animation::getName() const
{
	return mName;
}