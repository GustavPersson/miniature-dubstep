#include "common.fx"

// Input från Vertexbuffern
struct VS_INST_INPUT
{
    float3 pos		: POSITION;
    float3 norm		: NORMAL;
    float2 tex		: TEXTURE;
    row_major float4x4 mTransform : mTransform;
};

struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXTURE;
	float4 color	: COLOR;
	float4 projTex	: TEXCOORD1;
};

cbuffer cbPerFrame
{
	Light		fxLight;
	Material	fxMtrl;
	float3		fxEyePos;
};

cbuffer cbPerObject
{
	matrix    fxWVPM;
	matrix    fxWorldM;
	matrix	  fxLWVPM;
};

Texture2D fxTexture;

/* ----------------------------------------------------------- Vertex shaders */
PS_INPUT VS_INSTANCE(VS_INST_INPUT input)
{
	PS_INPUT psInput; 
    
    // Transform by our Sceneance matrix ananas
    float4 InstancePosition = mul(float4(input.pos, 1), input.mTransform);
    float4 ViewPos			= mul(InstancePosition, fxWorldM );
    
    // Transform the vert to view-space
    float4 v4Position	= mul(InstancePosition, fxWVPM);
    psInput.pos			= v4Position;

	psInput.projTex		= mul(InstancePosition, fxLWVPM);
    
    // Transfer the rest
    psInput.tex		= input.tex;
    
    // dot the norm with the light dir
    float3 norm		= mul(input.norm, (float3x3)input.mTransform);

	return psInput;
};

PS_INPUT VS_SHADOW(VS_INST_INPUT input)
{	
	PS_INPUT psInput;

	float4 InstancePosition = mul(float4(input.pos, 1), input.mTransform);
    float4 ViewPos			= mul(InstancePosition, fxWorldM );

	float4 v4Position		= mul(InstancePosition, fxLWVPM);
    psInput.pos				= v4Position;

	psInput.projTex			= mul(InstancePosition, fxLWVPM);

	psInput.tex				= input.tex;

	return psInput;
}

/* ----------------------------------------------------------- Pixel shaders */
float4 PS_INSTANCE(PS_INPUT pIn) : SV_Target
{
	float	sfactor		= CalcShadowFactor(pIn.projTex);
	float4	cTexture	= fxTexture.Sample(sampLinear, pIn.tex) * sfactor;
	cTexture.a = 1.0f;
	return cTexture;
}

float4 PS_SHADOW() : SV_Target
{
	float depth = 1.0f; 
	
	return float4(depth, depth, depth, 1.0f);
}

/* ----------------------------------------------------------- Techniques */
technique10 Render
{
	pass P0
	{
		SetRasterizerState(noFrontCull);
		SetDepthStencilState(DepthWrite, 0);

		SetVertexShader(CompileShader(vs_4_0, VS_INSTANCE()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, PS_INSTANCE()));
	}

	pass P1
	{
	//	SetRasterizerState(noBackCull);
		SetDepthStencilState(DepthWrite, 0);

		SetVertexShader(CompileShader(vs_4_0, VS_SHADOW()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, PS_SHADOW()));
	}
}