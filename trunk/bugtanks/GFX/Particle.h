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
* Base class for particles
*/

#ifndef PARTICLE_H
#define PARTICLE_H

#include <d3d10.h>
#include <d3dx10.h>
#include <vector>
#include "../Utilities/Macros.h"
#include "Vertex.h"
#include "../GFX/TextureHandler.h"

namespace gfx
{
	class Effect;

	class Particle
	{
	public:
		/**
		 * Constructor
		 * @param pDevice Pointer to graphics device
		 * @param pEffect Pointer to Effect
		 * @param name Name of the particle system
		 * @param passIndex The pass index to be used for streaming out (will use index+1 for rendering)
		 * @param layoutIndex The layout index used for particles
		 * @param initParticles Number of emitters
		 * @param maxChildParticles Number of child particles per each emitter
		 * @param offset Offset to get emitters from in the storage texture
		 */
		Particle(ID3D10Device *pDevice, Effect *pEffect, char* name, int passIndex, int layoutIndex, int initParticles, int maxChildParticles, int offset);
		
		/**
		 * Destructor
		 */
		~Particle();

		/**
		 * Adds an emitting particle to draw 
		 * @param position Position to emit from
		// * @param emitterType Type of emitter
		 * @param index Emitter index in the particle system
		 * @return True if sucessfully added particle, false otherwise
		 */
		virtual bool addEmitter(const D3DXVECTOR3 &position, unsigned int &index);

		/**
		 * Adds an emitting particle to draw list
		 * @param position Position to emit from
		 * @return True if sucessfully added particle, false otherwise
		 */
		virtual bool addEmitter(const D3DXVECTOR3 &position);

		virtual bool addEmitter(const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction);
		
		/**
		 * Removes particle as a emitter and defaults all values
		 * @param emitterIndex The emitters index
		 */
		virtual bool removeEmitter(unsigned int emitterIndex);
		
		/**
		 * Updates emitter at the given index
 		 * @param emitterIndex Emitter index in the particle system
		 * @param position Position to emit from
		 * @param emitterType Type of emitter
		 */
		virtual bool updateEmitter(unsigned int emitterIndex, const D3DXVECTOR3 &position);

		/**
		 * Updates particle storage texture with information about emitters
		 * @param pParticleStorage Texture to save data in
		 */
		virtual void updateParticleStorage(ID3D10Resource* pParticleStorage);

		/**
		 * Renders particles
		 */
		virtual void render();

		/**
		 * Clears particle list
		 */
		virtual void clear();

		/**
		 * Sets size for particles
		 */
		void setParticleSize(float sizeX, float sizeY);

		/**
		 * Sets particle type
		 * @param Type of particle to emit
		 */
		inline void setParticleType(int type) { mParticleType = type; }

		/**
		 * Sets effect technique index to be used for rendering
		 * @param index Index of technique
		 */
		inline void setEffectTechIndex(int index) { mEffectTechIndex = index; }

		/**
		 * Sets effect pass to be used for rendering. Particles need two passes,
		 * one for streaming out and one for drawing. Because of this the pass specified 
		 * this function is the stream out pass and the (index + 1) is the draw pass.
		 * @param index Index of render pass
		 */
		inline void setEffectPassIndex(int index) { mEffectPassIndex = index; }
		
		/**
		 * Sets layout index used for rendering particles
		 */
		void setEffectLayoutIndex(int index) { mEffectLayoutIndex = index; }

		/**
		 * Returns particle type
		 * @return Particle system type
		 */
		inline int getParticleType() { return mParticleType; }
		/**
		 * Return effect technique index used for rendering
		 * @return Effect tech index
		 */
		inline int getEffectTechIndex() { return mEffectTechIndex; }

		/**
		 * Return effect pass index used for rendering
		 * @return Effect pass index
		 */
		inline int getEffectPassIndex() { return mEffectPassIndex; }

		/**
		 * Returns effect layout index used for rendering
		 * @return Effect layout index
		 */
		inline int getEffectLayoutIndex() { return mEffectLayoutIndex; }

		/**
		 * Returns pointer to effect class
		 * @return Pointer to Effect handle
		 */
		inline Effect* getEffect() { return mpEffect; }
		
		/**
		 * Returns particle system name
		 * @return Praticle system name
		 */
		inline char* getName() { return mName; }

		/**
		 * Returns pointer to device
		 * @return ID3D10Device pointer
		 */
		inline ID3D10Device* getDevice() { return mpDevice; }

		/**
		 * Returns max amount of particles
		 * @return Returns max amount of particles in current system
		 */
		inline int getMaxParticles() { return (mEmitParticles * mChildParticles); }

		virtual ID3D10EffectShaderResourceVariable* setTexture(std::string shaderName);

	protected:
		// Globals
		char*			mName;				/**< System name */
		ID3D10Device	*mpDevice;
		Effect			*mpEffect;

		unsigned int	mEmitParticles;		/**< Number of emitting particles */
		unsigned int	mChildParticles;	/**< Number of particles an emitter can spawn */
		unsigned int	mNumEmitters;		/**< Number of active emitters at once */	

		bool			mFlagInit;
		bool			mFlagUpdate;
		bool			mFlagCleared;

		VertexParticles	*mpEmitterData;
		D3DXVECTOR3		*mpTransferData;
		bool			*mpEmitterUpdate;
		bool		    *mpEmitterState;

		ID3D10EffectShaderResourceVariable	*mpFXParticleStorage;

		/**
		 * Creates vertex buffers for initiating, streaming and drawing particles
		 */
		void createVertexBuffers();

		/**
		 * Finds a free emitter spot
		 * @return Index value of place where there is no emitter
		 */
		bool findFreeEmitterIndex(unsigned int &index);

	private:
		// Buffers
		ID3D10Buffer	*mpInitVB;
		ID3D10Buffer	*mpDrawVB;
		ID3D10Buffer	*mpStreamOutVB;

		// Variables
		int				mParticleType;		/**< Particle type, ex: Explosion, spark etc. */
		int				mEffectTechIndex;
		int				mEffectPassIndex;
		int				mEffectLayoutIndex;
	};
};

#endif
