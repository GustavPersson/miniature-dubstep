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

#include "BufferObject.h"
#include "Effect.h"
#include "CollisionObject.h"

using namespace gfx;

BufferObject::BufferObject(ID3D10Device *pDevice, Effect *pEffect) : TransformObject(pDevice, pEffect)
{
	setType(RenderTypes::BUFFER_OBJECT);
	setEffectTechIndex(0);
	setEffectPassIndex(ShaderPasses::CEL);
	setEffectLayoutIndex(ShaderPasses::CEL);
	mpVertexBufferStatic = NULL;
	mpVertexBufferDynamic = NULL;
	mpIndexBuffer = NULL;
	mVertexCount = 0;
	mIndexCount = 0;
	mFlagCopyBuffers = true;
	mFlagReleaseBuffers = true;
}

BufferObject::BufferObject(const BufferObject &object) : TransformObject(object)
{
	setType(RenderTypes::BUFFER_OBJECT);
	mpVertexBufferStatic = NULL;
	mpVertexBufferDynamic = NULL;
	mpIndexBuffer = NULL;
	mVertexCount = object.mVertexCount;
	mIndexCount = object.mIndexCount;
	mFlagCopyBuffers = object.mFlagCopyBuffers;
	mFlagReleaseBuffers = object.mFlagReleaseBuffers;
	copyBuffers(object);
}

BufferObject::~BufferObject()
{
	clearVertexBuffers();
	clearIndexBuffer();
}

const BufferObject &BufferObject::operator=(const BufferObject &object)
{
	if(this != &object)
	{
		TransformObject::operator=(object);
		mpVertexBufferStatic = NULL;
		mpVertexBufferDynamic = NULL;
		mpIndexBuffer = NULL;
		mVertexCount = object.mVertexCount;
		mIndexCount = object.mIndexCount;
		mFlagCopyBuffers = object.mFlagCopyBuffers;
		mFlagReleaseBuffers = object.mFlagReleaseBuffers;
		copyBuffers(object);
	}
	return *this;
}

RenderObject *BufferObject::clone()
{
	return NULL;
}

void BufferObject::update(float deltaTime)
{
	TransformObject::update(deltaTime);
	if(getFlagMoved())
	{
		updateVertexBufferDynamic();
	}
}

bool BufferObject::render()
{
	if(TransformObject::render())
	{
		ID3D10Buffer *buffers[2];
		buffers[0] = mpVertexBufferStatic;
		buffers[1] = mpVertexBufferDynamic;
		unsigned int stride[2];
		stride[0] = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSizeStatic();
		stride[1] = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSizeDynamic();
		unsigned int offset[2];
		offset[0] = 0;
		offset[1] = 0;
		getDevice()->IASetVertexBuffers(0, 2, buffers, stride, offset);
		getDevice()->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		getDevice()->IASetPrimitiveTopology(TOPOLOGY);
		getDevice()->IASetInputLayout(getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getDXInputLayout());

		ID3D10EffectTechnique *pEffectTechnique = getEffect()->getEffect()->GetTechniqueByIndex(getEffectTechIndex());
		
		if(getEffectPassIndex() == -1)
		{
			D3D10_TECHNIQUE_DESC techDesc;
			pEffectTechnique->GetDesc(&techDesc);
			for(unsigned int n = 0; n < techDesc.Passes; ++n)
			{
				pEffectTechnique->GetPassByIndex(n)->Apply(0);
				getDevice()->DrawIndexed(getIndexCount(), 0, 0);
			}
			return true;
		}
		else
		{
			pEffectTechnique->GetPassByIndex(getEffectPassIndex())->Apply(0);
			getDevice()->DrawIndexed(getIndexCount(), 0, 0);

			return true;
		}
	}
	return false;
}

void BufferObject::recreateBuffers()
{
	clearVertexBuffers();
	clearIndexBuffer();
	createVertexBufferStatic();
	createVertexBufferDynamic();
	createIndexBuffer();
}

bool BufferObject::updateVertexBufferDynamic()
{
	return true;
}

void BufferObject::createVertexBufferStatic()
{

}

void BufferObject::createVertexBufferDynamic()
{

}

void BufferObject::createEmptyVertexBufferStatic()
{
	clearVertexBufferStatic();
	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSizeStatic() * getVertexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, NULL, &mpVertexBufferStatic)))
	{
		ERROR_MESSAGE("Error creating vertex buffer.")
	}
}

void BufferObject::createEmptyVertexBufferDynamic()
{
	clearVertexBufferDynamic();
	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSizeDynamic() * getVertexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, NULL, &mpVertexBufferDynamic)))
	{
		ERROR_MESSAGE("Error creating vertex buffer.")
	}
}

void BufferObject::createIndexBuffer()
{

}

void BufferObject::createIndexBufferAdjacent()
{

}

void BufferObject::createEmptyIndexBuffer()
{
	clearIndexBuffer();
	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(unsigned int) * getIndexCount();
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	if(FAILED(getDevice()->CreateBuffer(&bufferDesc, NULL, &mpIndexBuffer)))
	{
		ERROR_MESSAGE("Error creating index buffer.")
	}
}

void BufferObject::clearVertexBuffers()
{
	clearVertexBufferStatic();
	clearVertexBufferDynamic();
}

void BufferObject::clearVertexBufferStatic()
{
	if(mFlagReleaseBuffers)
	{
		if(mpVertexBufferStatic)
		{
			mpVertexBufferStatic->Release();
			mpVertexBufferStatic = NULL;
		}
	}
}

void BufferObject::clearVertexBufferDynamic()
{
	if(mFlagReleaseBuffers)
	{
		if(mpVertexBufferDynamic)
		{
			mpVertexBufferDynamic->Release();
			mpVertexBufferDynamic = NULL;
		}
	}
}

void BufferObject::clearIndexBuffer()
{
	if(mFlagReleaseBuffers)
	{
		if(mpIndexBuffer)
		{
			mpIndexBuffer->Release();
			mpIndexBuffer = NULL;
		}
	}
}

void BufferObject::setVertexBufferStatic(ID3D10Buffer *pVertexBuffer)
{
	clearVertexBufferStatic();
	mpVertexBufferStatic = pVertexBuffer;
}

void BufferObject::setVertexBufferDynamic(ID3D10Buffer *pVertexBuffer)
{
	clearVertexBufferDynamic();
	mpVertexBufferDynamic = pVertexBuffer;
}

void BufferObject::setIndexBuffer(ID3D10Buffer *pIndexBuffer)
{
	clearIndexBuffer();
	mpIndexBuffer = pIndexBuffer;
}

void BufferObject::copyBuffers(const BufferObject &object)
{
	if(object.mFlagCopyBuffers)
	{
		createEmptyVertexBufferStatic();
		createEmptyVertexBufferDynamic();
		createEmptyIndexBuffer();
		getDevice()->CopyResource(mpVertexBufferStatic, object.mpVertexBufferStatic);
		getDevice()->CopyResource(mpVertexBufferDynamic, object.mpVertexBufferDynamic);
		getDevice()->CopyResource(mpIndexBuffer, object.mpIndexBuffer);
	}
}