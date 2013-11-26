#ifndef PARTICLE_STRUCTS_FX
#define PARTICLE_STRUCTS_FX

struct PARTICLE_INPUT
{
	float3	PosH		: POSITION;
	float3	Velocity	: VELOCITY;
	float2	Size		: SIZE;
	float	Age			: AGE;
	int		Type		: TYPE;
	int		ID			: ID;
};

/**
 * Gun sparks
 */
struct GS_PARTICLE_INPUT_SPARKS
{
	float4 PosH		: POSITION;
	float2 Size		: SIZE;
	float4 Color	: COLOR;
	int	   Type		: TYPE;
};

struct PS_PARTICLE_INPUT_SPARKS
{
	float4	PosH	: SV_Position;
	float4	Color	: COLOR;
	float2	TexC	: TEXCOORD;
	int		Type	: TYPE;
};

/**
 * Small explosion
 */
struct GS_PARTICLE_INPUT_SEXPLO
{
	float4 PosH		: POSITION;
	float2 Size		: SIZE;
	float4 Color	: COLOR;
	int	   Type		: TYPE;
};

struct PS_PARTICLE_INPUT_SEXPLO
{
	float4	PosH	: SV_Position;
	float4	Color	: COLOR;
	float2	TexC	: TEXCOORD;
	int		Type	: TYPE;
};

#endif