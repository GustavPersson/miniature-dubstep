#ifndef MAIN_SHADER_PARTICLE_FX
#define MAIN_SHADER_PARTICLE_FX

#include "ShaderFunctions.fx"
#include "ShaderStates.fx"
#include "ParticleStructs.fx"
#include "ParticleGunSparks.fx"
#include "ParticleSmallExplosion.fx"
#include "ParticleBigExplosion.fx"
#include "ParticleTankExplosion.fx"
#include "ParticleFlames.fx"
#include "ParticlePulse.fx"
#include "ParticleBeam.fx"

PARTICLE_INPUT VS_PARTICLE_SO(PARTICLE_INPUT input)
{
	return input;
}

#endif