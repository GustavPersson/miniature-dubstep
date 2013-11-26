#ifndef PARTICLE_FLAMES_FX
#define PARTICLE_FLAMES_FX

#include "ShaderFunctions.fx"
#include "ShaderStates.fx"
#include "ParticleStructs.fx"

DepthStencilState NoDepthWrites
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

/**
 * Particle Stream out
 */
[maxvertexcount(9)]
void GS_PARTICLE_FLAMES_SO(point PARTICLE_INPUT gIn[1],
					inout PointStream<PARTICLE_INPUT> particleStream)
{
	gIn[0].Age += tDeltaTime;
	gIn[0].PosH += gIn[0].Velocity * tDeltaTime;

	if(gIn[0].ID >= 0 && gIn[0].Type == INACTIVE_EMITTER)
	{
		float3 pos		= ParticleStorage.Load(int4(0, gIn[0].ID, FLAMES, 0)).rgb;
		float3 vel		= ParticleStorage.Load(int4(1, gIn[0].ID, FLAMES, 0)).rgb;
		float3 misc		= ParticleStorage.Load(int4(2, gIn[0].ID, FLAMES, 0)).rgb;
		
		vel.y = 0.0f;

		gIn[0].PosH		= pos;
		gIn[0].Velocity = vel;
		gIn[0].Size		= misc.yz;
		gIn[0].Type		= misc.x;
	}
	
	if(gIn[0].Type == INACTIVE_EMITTER)
	{
		particleStream.Append(gIn[0]);
	}
	else if(gIn[0].Type == ACTIVE_EMITTER)
	{
		if(gIn[0].Age > 0.1f)
		{
			PARTICLE_INPUT p;
			p.PosH		= gIn[0].PosH;
			p.Velocity	= gIn[0].Velocity;
			p.Size		= gIn[0].Size;
			p.Age		= 0.0f;
			p.Type		= CHILD;
			p.ID		= -1;
			particleStream.Append(p);
			
			p.PosH		= gIn[0].PosH;
			p.Velocity	= gIn[0].Velocity + float3(5.0f, 0.0f, 0.0f);
			p.Size		= gIn[0].Size;
			p.Age		= 0.0f;
			p.Type		= CHILD;
			p.ID		= -1;
			particleStream.Append(p);
			
			p.PosH		= gIn[0].PosH;
			p.Velocity	= gIn[0].Velocity + float3(0.0f, 0.0f, 5.0f);
			p.Size		= gIn[0].Size;
			p.Age		= 0.0f;
			p.Type		= CHILD;
			p.ID		= -1;
			particleStream.Append(p);
			
			p.PosH		= gIn[0].PosH;
			p.Velocity	= gIn[0].Velocity + float3(10.0f, 0.0f, 0.0f);
			p.Size		= gIn[0].Size;
			p.Age		= 0.0f;
			p.Type		= CHILD;
			p.ID		= -1;
			particleStream.Append(p);
			
			p.PosH		= gIn[0].PosH;
			p.Velocity	= gIn[0].Velocity + float3(0.0f, 0.0f, 10.0f);
			p.Size		= gIn[0].Size;
			p.Age		= 0.0f;
			p.Type		= CHILD;
			p.ID		= -1;
			particleStream.Append(p);
			
			gIn[0].Age	= 0.0f;
			gIn[0].Type = INACTIVE_EMITTER;
		}
					
		particleStream.Append(gIn[0]);
	}
	else if(gIn[0].Type == CHILD)
	{
		if(gIn[0].Age <= 1.0f)
			particleStream.Append(gIn[0]);
	}
}

/**
 * Particle drawingd
 */
GS_PARTICLE_INPUT VS_PARTICLE_FLAMES_DRAW(PARTICLE_INPUT input)
{
	GS_PARTICLE_INPUT output = (PS_PARTICLE_INPUT)0;
	
	output.PosH		= float4(input.PosH, 1.0f);
	output.Size		= input.Size;
	float opacity	= 1.0f - smoothstep(0.0f, 1.0f, tDeltaTime/1.0f);
	output.Color	= float4(0.05f, 0.05f, 0.05f, opacity);
	output.Type		= input.Type;

	return output;
}

[maxvertexcount(4)]					  
void GS_PARTICLE_FLAMES_DRAW(point GS_PARTICLE_INPUT gIn[1],
					  inout TriangleStream<PS_PARTICLE_INPUT> triStream)
{
	if(gIn[0].Type == CHILD)
	{
		float3 look  = normalize(VCameraPosition - gIn[0].PosH.xyz);
		float3 right = normalize(cross(float3(0,1,0), look));
		float3 up    = cross(look, right);
		float4x4 W;
		W[0] = float4(right,	0.0f);
		W[1] = float4(up,		0.0f);		
		W[2] = float4(look,		0.0f);
		W[3] = gIn[0].PosH;

		float4x4 WVP = mul(W, MVP);

		float halfWidth  = 0.5f*gIn[0].Size.x;
		float halfHeight = 0.5f*gIn[0].Size.y;
	
		float4 v[4];
		v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
		v[1] = float4(+halfWidth, -halfHeight, 0.0f, 1.0f);
		v[2] = float4(-halfWidth, +halfHeight, 0.0f, 1.0f);
		v[3] = float4(+halfWidth, +halfHeight, 0.0f, 1.0f);
		 
		PS_PARTICLE_INPUT output;
		[unroll]
		for(int i = 0; i < 4; ++i)
		{
			output.PosH  = mul(v[i], WVP);
			output.TexC  = mParticleQuad[i];
			output.Color = gIn[0].Color;
			output.Type  = gIn[0].Type;

			triStream.Append(output);
		}	
	}
}

float4 PS_PARTICLE_FLAMES_DRAW(PS_PARTICLE_INPUT input) : SV_Target
{
	float4 particleColor;
	if(input.TexC.y < 0.1f || input.TexC.x < 0.1f)
	{
		particleColor = 0.0f;
	}
	else
	{
		particleColor = SampleParticleTexture(PARTICLE_FLAME_ID, (half2)input.TexC) * input.Color;
	}
	
	return particleColor;
}

#endif