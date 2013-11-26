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

#include "InstanceShapeCube.h"
#include "OBB.h"
#include "Vertex.h"
#include "Effect.h"

using namespace gfx;


InstanceShapeCube::InstanceShapeCube(ID3D10Device *pDevice, Effect *pEffect) : InstanceObject(pDevice, pEffect)
{
	setBoundingVolume(myNew OBB(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1));
	getBoundingVolume()->setParent((void *)this);
}

InstanceShapeCube::InstanceShapeCube(const InstanceShapeCube &cube) : InstanceObject(cube)
{

}

InstanceShapeCube::~InstanceShapeCube()
{

}

const InstanceShapeCube &InstanceShapeCube::operator=(const InstanceShapeCube &cube)
{
	if(this != &cube)
	{
		InstanceObject::operator=(cube);
	}
	return *this;
}

RenderObject *InstanceShapeCube::clone()
{
	return myNew InstanceShapeCube(*this);
}

void InstanceShapeCube::update(float deltaTime)
{
	InstanceObject::update(deltaTime);
}

InstanceObject *InstanceShapeCube::createInstance()
{
	if(getFlagInstanceBase())
	{
		if(getInstanceCount() < getInstanceCountMax())
		{
			InstanceObject *pNewInstance = (InstanceObject *)clone();
			pNewInstance->setFlagInstanceBase(false);
			pNewInstance->setInstanceIndex(getInstanceCount());
			linkVertexBufferDynamic(pNewInstance);
			addInstance(pNewInstance);
			return pNewInstance;
		}
	}
	return NULL;
}

void InstanceShapeCube::loadBuffers()
{
	if(getFlagInstanceBase())
	{
		createVertexBufferStatic();
		createIndexBuffer();
	}
}

bool InstanceShapeCube::updateVertexBufferDynamic()
{
	if(InstanceObject::updateVertexBufferDynamic())
	{
		VertexTransformDynamic data;
		data.arrayIndex = getTextureArrayIndex();
		data.textureID = getTextureID();
		data.color = (const float *)getColor();
		data.transform = getMatrixTransform();

		D3D10_BOX box;
		box.left = sizeof(VertexTransformDynamic) * getInstanceIndex();
		box.right = sizeof(VertexTransformDynamic) * (getInstanceIndex() + 1);
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;

		getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)&data, 0, 0);
		return true;
	}
	return false;
}

bool InstanceShapeCube::updateVertexBufferDynamicFromBase()
{
	VertexTransformDynamic *pData = myNew VertexTransformDynamic[getInstanceCount()];

	unsigned int renderCount = 0;
	int indexOffset = 0;
	for(std::vector<InstanceObject *>::const_iterator instanceIter = getInstanceList().begin();
		instanceIter != getInstanceList().end(); ++instanceIter)
	{
		if((*instanceIter)->getFlagRender() && !(*instanceIter)->getFlagCulled())
		{
			++renderCount;
			pData[instanceIter - getInstanceList().begin() - indexOffset].arrayIndex = (*instanceIter)->getTextureArrayIndex();
			pData[instanceIter - getInstanceList().begin() - indexOffset].textureID = (*instanceIter)->getTextureID();
			pData[instanceIter - getInstanceList().begin() - indexOffset].color = (const float *)(*instanceIter)->getColor();
			pData[instanceIter - getInstanceList().begin() - indexOffset].transform = (*instanceIter)->getMatrixTransform();
		}
		else
		{
			++indexOffset;
		}
	}

	setRenderCount(renderCount);

	D3D10_BOX box;
	box.left = 0;
	box.right = sizeof(VertexTransformDynamic) * getRenderCount();
	box.top = 0;
	box.bottom = 1;
	box.front = 0;
	box.back = 1;

	getDevice()->UpdateSubresource(getVertexBufferDynamic(), 0, &box, (void *)pData, 0, 0);

	delete [] pData;
	return true;
}

void InstanceShapeCube::createVertexBufferStatic()
{
	setVertexCount(24);

	VertexTransformStatic vertices[24];

	D3DXCOLOR diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR specular(0.0f, 0.0f, 0.0f, 1.0f);

	//Front faces.
	vertices[0].position = D3DXVECTOR4_16F(-1.0f, -1.0f, -1.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR4_16F(0.0f, 0.0f, -1.0f, 0.0f);
	vertices[0].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[0].UV = D3DXVECTOR2_16F(0.0f, 1.0f);
	vertices[0].diffuse = (const float *)diffuse;
	vertices[0].specular = (const float *)specular; 

	vertices[1].position = D3DXVECTOR4_16F(-1.0f,  1.0f, -1.0f, 1.0f);
	vertices[1].normal = D3DXVECTOR4_16F(0.0f, 0.0f, -1.0f, 0.0f);
	vertices[1].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[1].UV = D3DXVECTOR2_16F(0.0f, 0.0f);
	vertices[1].diffuse = (const float *)diffuse;
	vertices[1].specular = (const float *)specular; 

	vertices[2].position = D3DXVECTOR4_16F(1.0f,  1.0f, -1.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR4_16F(0.0f, 0.0f, -1.0f, 0.0f);
	vertices[2].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[2].UV = D3DXVECTOR2_16F(1.0f, 0.0f);
	vertices[2].diffuse = (const float *)diffuse;
	vertices[2].specular = (const float *)specular; 

	vertices[3].position = D3DXVECTOR4_16F(1.0f, -1.0f, -1.0f, 1.0f);
	vertices[3].normal = D3DXVECTOR4_16F(0.0f, 0.0f, -1.0f, 0.0f);
	vertices[3].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[3].UV = D3DXVECTOR2_16F(1.0f, 1.0f);
	vertices[3].diffuse = (const float *)diffuse;
	vertices[3].specular = (const float *)specular; 

	//Back faces
	vertices[4].position = D3DXVECTOR4_16F(-1.0f, -1.0f, 1.0f, 1.0f);
	vertices[4].normal = D3DXVECTOR4_16F(0.0f, 0.0f, 1.0f, 0.0f);
	vertices[4].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[4].UV = D3DXVECTOR2_16F(1.0f, 1.0f);
	vertices[4].diffuse = (const float *)diffuse;
	vertices[4].specular = (const float *)specular; 

	vertices[5].position = D3DXVECTOR4_16F(1.0f,  -1.0f, 1.0f, 1.0f);
	vertices[5].normal = D3DXVECTOR4_16F(0.0f, 0.0f, 1.0f, 0.0f);
	vertices[5].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[5].UV = D3DXVECTOR2_16F(0.0f, 1.0f);
	vertices[5].diffuse = (const float *)diffuse;
	vertices[5].specular = (const float *)specular; 

	vertices[6].position = D3DXVECTOR4_16F(1.0f,  1.0f, 1.0f, 1.0f);
	vertices[6].normal = D3DXVECTOR4_16F(0.0f, 0.0f, 1.0f, 0.0f);
	vertices[6].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[6].UV = D3DXVECTOR2_16F(0.0f, 0.0f);
	vertices[6].diffuse = (const float *)diffuse;
	vertices[6].specular = (const float *)specular; 

	vertices[7].position = D3DXVECTOR4_16F(-1.0f, 1.0f, 1.0f, 1.0f);
	vertices[7].normal = D3DXVECTOR4_16F(0.0f, 0.0f, 1.0f, 0.0f);
	vertices[7].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[7].UV = D3DXVECTOR2_16F(1.0f, 0.0f);
	vertices[7].diffuse = (const float *)diffuse;
	vertices[7].specular = (const float *)specular; 

	//Bottom faces.
	vertices[8].position = D3DXVECTOR4_16F(-1.0f, -1.0f, -1.0f, 1.0f);
	vertices[8].normal = D3DXVECTOR4_16F(0.0f, -1.0f, 0.0f, 0.0f);
	vertices[8].tangent = D3DXVECTOR4_16F(-1.0f, 0.0f, 0.0f, 0.0f);
	vertices[8].UV = D3DXVECTOR2_16F(1.0f, 1.0f);
	vertices[8].diffuse = (const float *)diffuse;
	vertices[8].specular = (const float *)specular; 

	vertices[9].position = D3DXVECTOR4_16F(1.0f,  -1.0f, -1.0f, 1.0f);
	vertices[9].normal = D3DXVECTOR4_16F(0.0f, -1.0f, 0.0f, 0.0f);
	vertices[9].tangent = D3DXVECTOR4_16F(-1.0f, 0.0f, 0.0f, 0.0f);
	vertices[9].UV = D3DXVECTOR2_16F(0.0f, 1.0f);
	vertices[9].diffuse = (const float *)diffuse;
	vertices[9].specular = (const float *)specular; 

	vertices[10].position = D3DXVECTOR4_16F(1.0f,  -1.0f, 1.0f, 1.0f);
	vertices[10].normal = D3DXVECTOR4_16F(0.0f, -1.0f, 0.0f, 0.0f);
	vertices[10].tangent = D3DXVECTOR4_16F(-1.0f, 0.0f, 0.0f, 0.0f);
	vertices[10].UV = D3DXVECTOR2_16F(0.0f, 0.0f);
	vertices[10].diffuse = (const float *)diffuse;
	vertices[10].specular = (const float *)specular; 

	vertices[11].position = D3DXVECTOR4_16F(-1.0f, -1.0f, 1.0f, 1.0f);
	vertices[11].normal = D3DXVECTOR4_16F(0.0f, -1.0f, 0.0f, 0.0f);
	vertices[11].tangent = D3DXVECTOR4_16F(-1.0f, 0.0f, 0.0f, 0.0f);
	vertices[11].UV = D3DXVECTOR2_16F(1.0f, 0.0f);
	vertices[11].diffuse = (const float *)diffuse;
	vertices[11].specular = (const float *)specular; 

	//Top faces.
	vertices[12].position = D3DXVECTOR4_16F(-1.0f, 1.0f, -1.0f, 1.0f);
	vertices[12].normal = D3DXVECTOR4_16F(0.0f, 1.0f, 0.0f, 0.0f);
	vertices[12].tangent = D3DXVECTOR4_16F(0.0f, 0.0f, 0.0f, 0.0f);
	vertices[12].UV = D3DXVECTOR2_16F(0.0f, 1.0f);
	vertices[12].diffuse = (const float *)diffuse;
	vertices[12].specular = (const float *)specular; 

	vertices[13].position = D3DXVECTOR4_16F(-1.0f,  1.0f, 1.0f, 1.0f);
	vertices[13].normal = D3DXVECTOR4_16F(0.0f, 1.0f, 0.0f, 0.0f);
	vertices[13].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[13].UV = D3DXVECTOR2_16F(0.0f, 0.0f);
	vertices[13].diffuse = (const float *)diffuse;
	vertices[13].specular = (const float *)specular; 

	vertices[14].position = D3DXVECTOR4_16F(1.0f,  1.0f, 1.0f, 1.0f);
	vertices[14].normal = D3DXVECTOR4_16F(0.0f, 1.0f, 0.0f, 0.0f);
	vertices[14].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[14].UV = D3DXVECTOR2_16F(1.0f, 0.0f);
	vertices[14].diffuse = (const float *)diffuse;
	vertices[14].specular = (const float *)specular; 

	vertices[15].position = D3DXVECTOR4_16F(1.0f, 1.0f, -1.0f, 1.0f);
	vertices[15].normal = D3DXVECTOR4_16F(0.0f, 1.0f, 0.0f, 0.0f);
	vertices[15].tangent = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[15].UV = D3DXVECTOR2_16F(1.0f, 1.0f);
	vertices[15].diffuse = (const float *)diffuse;
	vertices[15].specular = (const float *)specular; 

	//Left faces.
	vertices[16].position = D3DXVECTOR4_16F(-1.0f, -1.0f, 1.0f, 1.0f);
	vertices[16].normal = D3DXVECTOR4_16F(-1.0f, 0.0f, 0.0f, 0.0f);
	vertices[16].tangent = D3DXVECTOR4_16F(0.0f, -1.0f, 0.0f, 0.0f);
	vertices[16].UV = D3DXVECTOR2_16F(0.0f, 1.0f);
	vertices[16].diffuse = (const float *)diffuse;
	vertices[16].specular = (const float *)specular; 

	vertices[17].position = D3DXVECTOR4_16F(-1.0f,  1.0f, 1.0f, 1.0f);
	vertices[17].normal = D3DXVECTOR4_16F(-1.0f, 0.0f, 0.0f, 0.0f);
	vertices[17].tangent = D3DXVECTOR4_16F(0.0f, -1.0f, 0.0f, 0.0f);
	vertices[17].UV = D3DXVECTOR2_16F(0.0f, 0.0f);
	vertices[17].diffuse = (const float *)diffuse;
	vertices[17].specular = (const float *)specular; 

	vertices[18].position = D3DXVECTOR4_16F(-1.0f,  1.0f, -1.0f, 1.0f);
	vertices[18].normal = D3DXVECTOR4_16F(-1.0f, 0.0f, 0.0f, 0.0f);
	vertices[18].tangent = D3DXVECTOR4_16F(0.0f, -1.0f, 0.0f, 0.0f);
	vertices[18].UV = D3DXVECTOR2_16F(1.0f, 0.0f);
	vertices[18].diffuse = (const float *)diffuse;
	vertices[18].specular = (const float *)specular; 

	vertices[19].position = D3DXVECTOR4_16F(-1.0f, -1.0f, -1.0f, 1.0f);
	vertices[19].normal = D3DXVECTOR4_16F(-1.0f, 0.0f, 0.0f, 0.0f);
	vertices[19].tangent = D3DXVECTOR4_16F(0.0f, -1.0f, 0.0f, 0.0f);
	vertices[19].UV = D3DXVECTOR2_16F(1.0f, 1.0f);
	vertices[19].diffuse = (const float *)diffuse;
	vertices[19].specular = (const float *)specular; 

	//Right faces.
	vertices[20].position = D3DXVECTOR4_16F(1.0f, -1.0f, -1.0f, 1.0f);
	vertices[20].normal = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[20].tangent = D3DXVECTOR4_16F(0.0f, 1.0f, 0.0f, 0.0f);
	vertices[20].UV = D3DXVECTOR2_16F(0.0f, 1.0f);
	vertices[20].diffuse = (const float *)diffuse;
	vertices[20].specular = (const float *)specular; 

	vertices[21].position = D3DXVECTOR4_16F(1.0f,  1.0f, -1.0f, 1.0f);
	vertices[21].normal = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[21].tangent = D3DXVECTOR4_16F(0.0f, 1.0f, 0.0f, 0.0f);
	vertices[21].UV = D3DXVECTOR2_16F(0.0f, 0.0f);
	vertices[21].diffuse = (const float *)diffuse;
	vertices[21].specular = (const float *)specular; 

	vertices[22].position = D3DXVECTOR4_16F(1.0f,  1.0f, 1.0f, 1.0f);
	vertices[22].normal = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[22].tangent = D3DXVECTOR4_16F(0.0f, 1.0f, 0.0f, 0.0f);
	vertices[22].UV = D3DXVECTOR2_16F(1.0f, 0.0f);
	vertices[22].diffuse = (const float *)diffuse;
	vertices[22].specular = (const float *)specular; 

	vertices[23].position = D3DXVECTOR4_16F(1.0f, -1.0f, 1.0f, 1.0f);
	vertices[23].normal = D3DXVECTOR4_16F(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[23].tangent = D3DXVECTOR4_16F(0.0f, 1.0f, 0.0f, 0.0f);
	vertices[23].UV = D3DXVECTOR2_16F(1.0f, 1.0f);
	vertices[23].diffuse = (const float *)diffuse;
	vertices[23].specular = (const float *)specular;

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSizeStatic() * getVertexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = vertices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, &bufferData, getVertexBufferStaticPointer())))
	{
		ERROR_MESSAGE("Error creating vertex buffer.")
	}
}

void InstanceShapeCube::createVertexBufferDynamic()
{

}

void InstanceShapeCube::createIndexBuffer()
{
	setIndexCount(36);

	unsigned int indices[36];

	//Front faces.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	//Back faces.
	indices[6] = 4;
	indices[7] = 5;
	indices[8] = 6;

	indices[9] = 4;
	indices[10] = 6;
	indices[11] = 7;

	//Bottom faces.
	indices[12] = 8;
	indices[13] = 9;
	indices[14] = 10;

	indices[15] = 8;
	indices[16] = 10;
	indices[17] = 11;

	//Top faces.
	indices[18] = 12;
	indices[19] = 13;
	indices[20] = 14;

	indices[21] = 12;
	indices[22] = 14;
	indices[23] = 15;

	//Left faces.
	indices[24] = 16;
	indices[25] = 17;
	indices[26] = 18;

	indices[27] = 16;
	indices[28] = 18;
	indices[29] = 19;

	//Right faces.
	indices[30] = 20;
	indices[31] = 21;
	indices[32] = 22;

	indices[33] = 20;
	indices[34] = 22;
	indices[35] = 23;

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(unsigned int) * getIndexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = indices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, &bufferData, getIndexBufferPointer())))
	{
		ERROR_MESSAGE("Error creating index buffer.")
	}
}

void InstanceShapeCube::createIndexBufferAdjacent()
{
	setIndexCount(72);

	unsigned int indices[72];

	//Front faces.
	indices[0] = 0;
	indices[1] = 16;
	indices[2] = 1;
	indices[3] = 14;
	indices[4] = 2;
	indices[5] = 3;

	indices[6] = 0;
	indices[7] = 1;
	indices[8] = 2;
	indices[9] = 22;
	indices[10] = 3;
	indices[11] = 10;

	//Back faces.
	indices[12] = 4;
	indices[13] = 8;
	indices[14] = 5;
	indices[15] = 20;
	indices[16] = 6;
	indices[17] = 7;

	indices[18] = 4;
	indices[19] = 5;
	indices[20] = 6;
	indices[21] = 12;
	indices[22] = 7;
	indices[23] = 18;

	//Bottom faces.
	indices[36] = 8;
	indices[37] = 2;
	indices[38] = 9;
	indices[39] = 22;
	indices[40] = 10;
	indices[41] = 11;

	indices[42] = 8;
	indices[43] = 9;
	indices[44] = 10;
	indices[45] = 6;
	indices[46] = 11;
	indices[47] = 18;

	//Top faces.
	indices[24] = 12;
	indices[25] = 16;
	indices[26] = 13;
	indices[27] = 4;
	indices[28] = 14;
	indices[29] = 15;

	indices[30] = 12;
	indices[31] = 13;
	indices[32] = 14;
	indices[33] = 20;
	indices[34] = 15;
	indices[35] = 0;

	//Left faces.
	indices[48] = 16;
	indices[49] = 6;
	indices[50] = 17;
	indices[51] = 14;
	indices[52] = 18;
	indices[53] = 19;

	indices[54] = 16;
	indices[55] = 17;
	indices[56] = 18;
	indices[57] = 2;
	indices[58] = 19;
	indices[59] = 10;

	//Right faces.
	indices[60] = 20;
	indices[61] = 0;
	indices[62] = 21;
	indices[63] = 12;
	indices[64] = 22;
	indices[65] = 23;

	indices[66] = 20;
	indices[67] = 21;
	indices[68] = 22;
	indices[69] = 4;
	indices[70] = 23;
	indices[71] = 8;

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(unsigned int) * getIndexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = indices;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, &bufferData, getIndexBufferPointer())))
	{
		ERROR_MESSAGE("Error creating index buffer.")
	}
}