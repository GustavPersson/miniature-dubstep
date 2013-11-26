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

#ifndef PARTICLE_HANDLER_H
#define PARTICLE_HANDLER_H

#include "../Utilities/Macros.h"
#include <d3dx10.h>
#include <vector>

namespace gfx
{
	class Particle;
	class Effect;

	class ParticleHandler
	{
	public:
		ParticleHandler();
		~ParticleHandler();

		/**
		 * Initializes system
		 */
		void init(ID3D10Device *pDevice, Effect *pEffect);

		/**
		 * Loads all particle systems
		 */
		void loadParticleSystems();

		/**
		 * Returns pointer to particle system
		 * @param index Index of particle system
		 */
		Particle* getParticleSystem(int index);

		/**
		 * Returns pointer to particle system
		 * @param name Name of particle system
		 */
		Particle* getParticleSystem(char* name);

		/**
		 * Updates all particle systems
		 */
		void updateParticleSystems();

		/**
		 * Renders all particle systems
		 */
		void renderParticleSystems();

	private:
		/**
		 * Creates particle emitter texture to hold particle values
		 */
		void createParticleStorageTexture(int numParticles, int numParticleSystems);

		std::vector<Particle *>		mParticleSystemList;

		ID3D10Device				*mpDevice;
		Effect						*mpEffect;

		ID3D10Texture2D				*mpParticleStorageTexture;
		ID3D10ShaderResourceView	*mpParticleStorageSRV;

		bool						mFlagSRVUpdateRequired;
	};
}

#endif