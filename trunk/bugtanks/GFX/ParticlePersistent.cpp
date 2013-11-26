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
* Handles persistent particles
*/

#include "ParticlePersistent.h"
#include "Effect.h"

using namespace gfx;

ParticlePersistent::ParticlePersistent(ID3D10Device *pDevice, Effect *pEffect, char* name, int passIndex, int layoutIndex, int initParticles, int maxChildParticles, int offset) 
:Particle(pDevice, pEffect, name, passIndex, layoutIndex, initParticles, maxChildParticles, offset)
{
	for(int i = 0; i < initParticles; ++i)
	{
		mpEmitterData[i].size = D3DXVECTOR2(2.0f, 2.0f);
	}
}

ParticlePersistent::~ParticlePersistent()
{
}

void ParticlePersistent::updateParticleStorage(ID3D10Resource* pParticleStorage)
{
	// Check if an update is required for particles and if the system has been initialized
	if(mFlagUpdate)
	{
		for(unsigned int n = 0; n < mEmitParticles; ++n)
		{
			mpTransferData[(n * 3) + 0] = mpEmitterData[n].position;
			mpTransferData[(n * 3) + 1] = mpEmitterData[n].velocity;
			mpTransferData[(n * 3) + 2] = D3DXVECTOR3((float)mpEmitterData[n].type, mpEmitterData[n].size.x, mpEmitterData[n].size.y);
		}

		getDevice()->UpdateSubresource(pParticleStorage, D3D10CalcSubresource(0, getParticleType(), 1), NULL, (void *)mpTransferData, sizeof(D3DXVECTOR3) * 3, sizeof(D3DXVECTOR3) * 3 * mEmitParticles);

		mFlagUpdate = false;
	}
}