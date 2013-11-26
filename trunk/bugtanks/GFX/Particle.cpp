/**
* @file
* @author Benjamin Golba <benjii.g@gmail.com>
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
* Handles particles
*/

#include "Particle.h"
#include "Effect.h"

using namespace gfx;

Particle::Particle(ID3D10Device *pDevice, Effect *pEffect, char* name, int passIndex, int layoutIndex, int initParticles, int maxChildParticles, int offset)
{
	mName				= name;
	mpDevice			= pDevice;
	mpEffect			= pEffect;

	mEmitParticles		= initParticles;
	mChildParticles		= maxChildParticles;
	mNumEmitters		= 0;

	mFlagInit			= true;
	mFlagUpdate			= true;
	mFlagCleared		= false;

	mpEmitterData		= myNew VertexParticles[mEmitParticles];
	mpTransferData		= myNew D3DXVECTOR3[mEmitParticles * 3];
	mpEmitterUpdate		= myNew bool[mEmitParticles];
	mpEmitterState		= myNew bool[mEmitParticles];

	mpFXParticleStorage = getEffect()->getEffect()->GetVariableByName("ParticleStorage")->AsShaderResource();

	mpInitVB			= NULL;
	mpDrawVB			= NULL;
	mpStreamOutVB		= NULL;

	mParticleType		= -1;
	mEffectTechIndex	= 0;
	mEffectPassIndex	= passIndex;
	mEffectLayoutIndex	= layoutIndex;

	clear();
	createVertexBuffers();
}

Particle::~Particle()
{
	if(mpEmitterData)
	{
		delete [] mpEmitterData;
		mpEmitterData = NULL;
	}
	if(mpTransferData)
	{
		delete [] mpTransferData;
		mpTransferData = NULL;
	}
	if(mpEmitterUpdate)
	{
		delete [] mpEmitterUpdate;
		mpEmitterUpdate = NULL;
	}
	if(mpEmitterState)
	{
		delete [] mpEmitterState;
		mpEmitterState = NULL;
	}
}

void Particle::updateParticleStorage(ID3D10Resource* pParticleStorage)
{
	// Check if an update is required for particles and if the system has been initialized
	if(mFlagUpdate && !mFlagInit)
	{
		D3DXVECTOR3 *pData = myNew D3DXVECTOR3[mEmitParticles * 3];

		for(unsigned int n = 0; n < mEmitParticles; ++n)
		{
			pData[(n * 3) + 0] = mpEmitterData[n].position;
			pData[(n * 3) + 1] = mpEmitterData[n].velocity;
			pData[(n * 3) + 2] = D3DXVECTOR3((float)mpEmitterData[n].type, mpEmitterData[n].size.x, mpEmitterData[n].size.y);
		}

		getDevice()->UpdateSubresource(pParticleStorage, D3D10CalcSubresource(0, getParticleType(), 1), NULL, (void *)pData, sizeof(D3DXVECTOR3) * 3, sizeof(D3DXVECTOR3) * 3 * mEmitParticles);

		delete [] pData;
	}
}

void Particle::render()
{
	// Set the input layout
	getDevice()->IASetInputLayout(getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getDXInputLayout());
	getDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = getEffect()->getInputLayoutHandler().getInputLayout(getEffectLayoutIndex())->getLayoutSize();
    UINT offset = 0;

	// Init vertex buffers
	if(mFlagInit)
		getDevice()->IASetVertexBuffers(0, 1, &mpInitVB, &stride, &offset);
	else
		getDevice()->IASetVertexBuffers(0, 1, &mpDrawVB, &stride, &offset);

	getDevice()->SOSetTargets(1, &mpStreamOutVB, &offset);

	ID3D10EffectTechnique *pEffectTechnique = getEffect()->getEffect()->GetTechniqueByIndex(getEffectTechIndex());

	// Render pass for streaming out particles
	pEffectTechnique->GetPassByIndex(getEffectPassIndex())->Apply(0);

	/** 
	 * Initialize system by streaming 1 emitter,
	 * else stream all particles that are alive
	 */
	if(mFlagInit)
	{
		getDevice()->Draw(mEmitParticles, 0);
		mFlagInit = false;
	}
	else
	{
		getDevice()->DrawAuto();
	}

	// Unbind the vertex buffer when done streaming out
	ID3D10Buffer *bufferArray[1] = {0};
	getDevice()->SOSetTargets(1, bufferArray, &offset);

	/** 
	 * Swap buffers so that they switch their tasks (streaming - drawing)
	 * and repeat this with each render-call
	 */
	std::swap(mpDrawVB, mpStreamOutVB);

	// Draw particles using getEffectPassIndex() + 1 to have the right pass for drawing
	getDevice()->IASetVertexBuffers(0, 1, &mpDrawVB, &stride, &offset);
	pEffectTechnique->GetPassByIndex(getEffectPassIndex() + 1)->Apply(0);
	getDevice()->DrawAuto();
}

void Particle::clear()
{
	for(unsigned int i = 0; i < mNumEmitters; ++i)
	{
		mpEmitterData[i].type		= -1;
		mpEmitterData[i].age		= 0.0f;

		mpEmitterState[i]		= false;
		mpEmitterUpdate[i]		= true;
		mFlagUpdate				= true;
		mFlagCleared			= true;
	}
}

void Particle::setParticleSize(float sizeX, float sizeY)
{
	for(unsigned int i = 0; i < mEmitParticles; ++i) 
	{ 
		mpEmitterData[i].size = D3DXVECTOR2(sizeX, sizeY); 
	}
}

bool Particle::addEmitter(const D3DXVECTOR3 &position, unsigned int &index)
{
	if(findFreeEmitterIndex(index))
	{
		mpEmitterData[index].position	= position;
		mpEmitterData[index].type		= 0;
		mpEmitterState[index]			= true;
		mpEmitterUpdate[index]			= true;
		mFlagUpdate						= true;
		mFlagCleared					= false;

		return true;
	}

	return false;
}

bool Particle::addEmitter(const D3DXVECTOR3 &position)
{
	if(mNumEmitters < mEmitParticles)
	{
		mpEmitterData[mNumEmitters].position	= position;
		mpEmitterData[mNumEmitters].type		= 0;
		mpEmitterState[mNumEmitters]			= true;
		mpEmitterUpdate[mNumEmitters]			= true;
		mFlagUpdate								= true;
		mFlagCleared							= false;
		++mNumEmitters;

		return true;
	}
	else
	{
		mNumEmitters = 0; // Reset number of emitters
	}

	return false;
}

bool Particle::addEmitter(const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction)
{
	if(mNumEmitters < mEmitParticles)
	{
		mpEmitterData[mNumEmitters].position	= position;
		mpEmitterData[mNumEmitters].type		= 0;
		mpEmitterData[mNumEmitters].velocity    = direction;
		mpEmitterState[mNumEmitters]			= true;
		mpEmitterUpdate[mNumEmitters]			= true;
		mFlagUpdate								= true;
		mFlagCleared							= false;
		++mNumEmitters;

		return true;
	}
	else
	{
		mNumEmitters = 0; // Reset number of emitters
	}

	return false;
}

bool Particle::removeEmitter(unsigned int emitterIndex)
{
	if(mpEmitterState[emitterIndex])
	{
		mpEmitterData[emitterIndex].type	= -1;
		mpEmitterState[emitterIndex]		= false;
		mpEmitterUpdate[emitterIndex]		= true;
		mFlagUpdate							= true;
		mFlagCleared						= false;

		return true;
	}

	return false;
}

bool Particle::updateEmitter(unsigned int emitterIndex, const D3DXVECTOR3 &position)
{
	if(mpEmitterState[emitterIndex])
	{
		mpEmitterData[emitterIndex].position	= position;
		mpEmitterData[emitterIndex].type		= 0;
		mpEmitterUpdate[emitterIndex]			= true;
		mFlagUpdate								= true;
		mFlagCleared							= false;

		return true;
	}

	return false;
}

void Particle::createVertexBuffers()
{
	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth		= sizeof(VertexParticles) * mEmitParticles;
	bufferDesc.Usage			= D3D10_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags	= 0;
	bufferDesc.MiscFlags		= 0;

	VertexParticles *p = myNew VertexParticles[mEmitParticles];
	
	for(unsigned int i = 0; i < mEmitParticles; ++i)
	{
		p[i].id = i;
	}
	
	D3D10_SUBRESOURCE_DATA inData;
	inData.pSysMem = p;

	// Buffer to initiate the particle system
	if(FAILED(mpDevice->CreateBuffer(&bufferDesc, &inData, &mpInitVB)))
	{
		ERROR_MESSAGE("Error: GFX: Particle: createVertexBuffers(). Error creating vertex buffer (mInitVB).");
	}

	delete [] p;

	// Create buffers to swap between when drawing and streaming
	bufferDesc.CPUAccessFlags	= 0;
	bufferDesc.ByteWidth		= sizeof(VertexParticles) * (mEmitParticles * mChildParticles);
	bufferDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER | D3D10_BIND_STREAM_OUTPUT;

	// Draw buffer
	if(FAILED(mpDevice->CreateBuffer(&bufferDesc, NULL, &mpDrawVB)))
	{
		ERROR_MESSAGE("Error: GFX: Particle: createVertexBuffers(). Error creating vertex buffer (mDrawVB).");
	}
		
	// Stream out buffer
	if(FAILED(mpDevice->CreateBuffer(&bufferDesc, NULL, &mpStreamOutVB)))
	{
		ERROR_MESSAGE("Error: GFX: Particle: createVertexBuffers(). Error creating vertex buffer (mStreamOutVB).");
	}
}

bool Particle::findFreeEmitterIndex(unsigned int &index)
{
	for(unsigned int n = 0; n < mEmitParticles; ++n)
	{
		if(!mpEmitterState[n])
		{
			index = n;
			return true;
		}
	}

	return false;
}

ID3D10EffectShaderResourceVariable* Particle::setTexture(std::string shaderName)
{
	return mpEffect->getEffect()->GetVariableByName(shaderName.c_str())->AsShaderResource();
	//pShaderVariable = mpEffect->getEffect()->GetVariableByName("texture")->AsShaderResource();
}