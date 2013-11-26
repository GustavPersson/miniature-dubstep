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

#include "ParticleGunSparks.h"
#include "Effect.h"

using namespace gfx;

ParticleGunSparks::ParticleGunSparks(ID3D10Device *pDevice, Effect *pEffect, char* name, int techIndex, int passIndex, int initParticles, int maxChildParticles) 
:Particle(pDevice, pEffect, name, techIndex, passIndex, initParticles, maxChildParticles)
{
	setParticleType(0);
	
	for(int i = 0; i < initParticles; ++i)
	{
		mpEmitterData[i].size = D3DXVECTOR2(2.0f, 2.0f);
	}
}

ParticleGunSparks::~ParticleGunSparks()
{
}

void ParticleGunSparks::updateParticleStorage(ID3D10Resource* pParticleStorage)
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

		getDevice()->UpdateSubresource(pParticleStorage, D3D10CalcSubresource(0, 0, 1), NULL, (void *)pData, sizeof(D3DXVECTOR3) * 3, sizeof(D3DXVECTOR3) * 3 * mEmitParticles);

		if(!mFlagCleared)
			clear();

		delete [] pData;
	}
}

void ParticleGunSparks::render()
{
	Particle::render();
}