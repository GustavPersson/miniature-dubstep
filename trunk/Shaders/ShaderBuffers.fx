#ifndef SHADER_BUFFERS_FX
#define SHADER_BUFFERS_FX

struct Light
{
	float3 pos;
	float3 dir;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float shadowFactor;
};

cbuffer cbPerFrame
{
	// Camera
	float2 ViewportSize;
	float4x4 MVP;
	float4x4 MV;
	float4x4 MP;
	float4x4 MOrtP;
	float3 VCameraPosition;
	float3 VCameraDirection;

	// Light
	float4x4 LVP;
	Light VLight;

	// Timer
	float tGameTime;
	float tDeltaTime;
};

cbuffer cbPerFrameParticlesLocal
{
	// Net constant acceleration used to accerlate the particles.
	float3 mParticleAccel = {1.0f, -9.8f, 1.0f};
	
	// Texture coordinates used to stretch texture over quad 
	// when we expand point particle into a quad.
	float2 mParticleQuad[4] = 
	{
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
	};
};

#define SMALLEXPLO	0
#define BIGEXPLO	1
#define TANKEXPLO	2
#define GUNSPARKS	3
#define PULSE		4
#define BEAM		5
#define FLAMES		6

#define ARRAY_128 0
#define ARRAY_256 1
#define ARRAY_512 2
#define ARRAY_1024 3
#define ARRAY_FONT 4

#define FADE_START 1500.0h
#define FADE_MAX 1800.h


Texture2DArray TextureArray128;
Texture2DArray TextureArray256;
Texture2DArray TextureArray512;
Texture2DArray TextureArray1024;
Texture2DArray FontArray;
Texture2DArray WorldPlaneArray;
Texture2DArray ParticleStorage;
Texture2DArray Animations;
Texture2D ShadowMap;
Texture2DArray ParticleTextureArray;

#endif