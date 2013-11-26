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

#include "Effect.h"
#include "ParticleHandler.h"
#include "Particle.h"
#include "ParticleGunSparks.h"
#include "ParticleSmallExplosion.h"

using namespace gfx;

ParticleHandler::ParticleHandler()
{
	mpDevice = NULL;
	mpEffect = NULL;

	mpParticleStorageTexture	= NULL;
	mpParticleStorageSRV		= NULL;
}

ParticleHandler::~ParticleHandler()
{
	for(unsigned int i = 0; i < mParticleSystemList.size(); ++i)
		SAFE_DELETE(mParticleSystemList.at(i));

	mParticleSystemList.clear();
}

void ParticleHandler::init(ID3D10Device *pDevice, Effect *pEffect)
{
	mpDevice = pDevice;
	mpEffect = pEffect;

	loadParticleSystems();
}

void ParticleHandler::loadParticleSystems()
{
	Particle *p				= NULL;
	int numEmitParticles	= 0;
	int numSystems			= 0;

	// Explosion
	p = myNew ParticleSmallExplosion(mpDevice, mpEffect, "SmallExplosion", 0, 5, 10, 100);
	mParticleSystemList.push_back(p);
	numEmitParticles += 10;
	++numSystems;
/*
	p = myNew ParticleGunSparks(mpDevice, mpEffect, "GunSparks", 0, 7, 10, 100);
	mParticleSystemList.push_back(p);
	numEmitParticles += 10;
	++numSystems;
*/

	createParticleStorageTexture(numEmitParticles, numSystems);
	mpEffect->updateParticleStorage(mpParticleStorageSRV);
}

Particle* ParticleHandler::getParticleSystem(int index)
{
	Particle *p = mParticleSystemList.at(index);

	if(p != NULL)
		return p;

	return NULL;
}

Particle* ParticleHandler::getParticleSystem(char* name)
{
	for(std::vector<Particle *>::iterator n = mParticleSystemList.begin();
	n != mParticleSystemList.end(); ++n)
	{
		if(strcmp((*n)->getName(),name)==0)
		{
			return (*n);
		}
	}

	return NULL;
}

void ParticleHandler::updateParticleSystems()
{
	for(std::vector<Particle *>::iterator n = mParticleSystemList.begin();
	n != mParticleSystemList.end(); ++n)
	{
		//(*n)->update();
		(*n)->updateParticleStorage(mpParticleStorageTexture);
	}
}

void ParticleHandler::renderParticleSystems()
{
	for(std::vector<Particle *>::iterator n = mParticleSystemList.begin();
	n != mParticleSystemList.end(); ++n)
	{
		(*n)->render();
	}
}

void ParticleHandler::createParticleStorageTexture(int numEmitParticles, int numParticleSystems)
{
	//Release any current weight maps.
	SAFE_RELEASE(mpParticleStorageTexture);
	SAFE_RELEASE(mpParticleStorageSRV);

	D3D10_TEXTURE2D_DESC arrayDesc;
	arrayDesc.Format				= DXGI_FORMAT_R32G32B32_FLOAT;
	arrayDesc.BindFlags				= D3D10_BIND_SHADER_RESOURCE;
	arrayDesc.Usage					= D3D10_USAGE_DEFAULT;
	arrayDesc.CPUAccessFlags		= 0;
	arrayDesc.ArraySize				= numParticleSystems;
	arrayDesc.Width					= 3;
	arrayDesc.Height				= numEmitParticles;
	arrayDesc.MipLevels				= 1;
	arrayDesc.MiscFlags				= 0;
	arrayDesc.SampleDesc.Count		= 1;
	arrayDesc.SampleDesc.Quality	= 0;

	if(FAILED(mpDevice->CreateTexture2D(&arrayDesc, NULL, &mpParticleStorageTexture)))
	{
		ERROR_MESSAGE("Failed to create particle storage texture.")
	}

	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.ViewDimension						= D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	SRVDesc.Format								= DXGI_FORMAT_R32G32B32_FLOAT;
	SRVDesc.Texture2DArray.ArraySize			= numParticleSystems;
	SRVDesc.Texture2DArray.FirstArraySlice		= 0;
	SRVDesc.Texture2DArray.MipLevels			= 1;
	SRVDesc.Texture2DArray.MostDetailedMip		= 0;

	if(FAILED(mpDevice->CreateShaderResourceView(mpParticleStorageTexture, &SRVDesc, &mpParticleStorageSRV)))
	{
		ERROR_MESSAGE("Failed to create particle storage shader resource.")
	}

	mFlagSRVUpdateRequired = true;
}