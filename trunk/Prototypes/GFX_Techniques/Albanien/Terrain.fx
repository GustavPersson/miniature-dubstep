#include "common.fx"

cbuffer cbPerFrame
{
	Light		fxLight;
	Material	fxMtrl;
	float3		fxEyePos;
	bool		fxShadowEnable;
};

cbuffer cbPerObject   
{
	matrix fxWVPM;
	matrix fxWorldM;
	matrix fxLWVPM;
};

//Blend textures
Texture2D fxBlendT;
Texture2D fxDirtT;
Texture2D fxGrassT;
Texture2D fxSandT;

// Filled by vertex shader
struct PS_INPUT
{
	float4 posH      : SV_POSITION;
	float4 posWS     : POSITION;
	float3 normalWS  : NORMAL;
	float2 tex0      : TEXCOORD0;
	float4 projTex	 : TEXCOORD1;
};

/* ----------------------------------------------------------- Vertex shaders */
PS_INPUT VS(float3 posLS  : POSITION, 
			float3 normLS : NORMAL, 
			float2 tex    : TEXCOORD0)
{
	PS_INPUT psInput;
	
	psInput.posH	 = mul( float4(posLS, 1.0f), fxWVPM );
	psInput.posWS    = mul( float4(posLS, 1.0f), fxWorldM );
	psInput.normalWS = normalize( mul( float4(normLS, 0.0f), fxWorldM) );
	psInput.tex0     = tex;
	psInput.projTex	 = mul(float4(posLS, 1.0f), fxLWVPM);

	return psInput;
};

PS_INPUT VS_SHADOW(float3 posLS  : POSITION, 
			float3 normLS : NORMAL, 
			float2 tex    : TEXCOORD0)
{	
	PS_INPUT psInput;
	
	psInput.posH	 = mul( float4(posLS, 1.0f), fxLWVPM );
	psInput.posWS    = mul( float4(posLS, 1.0f), fxWorldM);
	psInput.normalWS = normalize( mul( float4(normLS, 0.0f), fxWorldM) );
	psInput.tex0     = tex;
	psInput.projTex	 = mul(float4(posLS, 1.0f), fxLWVPM);

	return psInput;
}

/* ----------------------------------------------------------- Pixel Shaders */
float4 PS_Normal(PS_INPUT psInput)
{	
	float4 cBlend = fxBlendT.Sample(sampLinear, psInput.tex0);
	float3 bColor = cBlend.r + cBlend.g + cBlend.b;

	float4 cDirt  = fxDirtT.Sample(sampLinear, psInput.tex0);
	float4 cGrass = fxGrassT.Sample(sampLinear, psInput.tex0);
	float4 cSand  = fxSandT.Sample(sampLinear, psInput.tex0);

	float w1 = cBlend.r / bColor;
	float w2 = cBlend.g / bColor;
	float w3 = cBlend.b / bColor;

	float3 fColor = (w2 * cGrass) + (w1 * cSand) + (w3 * cDirt);

	float4 ldir         = float4(normalize((fxLight.pos - psInput.posWS)),0.0f);
    float4 amb          = fxMtrl.ambient * fxLight.ambient;
    float4 dif          = max(dot(psInput.normalWS, ldir),0) * fxMtrl.diffuse * fxLight.diffuse;
    float4 r            = float4(2*(dot(psInput.normalWS, ldir)) * psInput.normalWS - ldir,0.0f);
    float4 spec         = pow(max((dot(r,normalize(fxEyePos - psInput.posWS))),0),fxMtrl.specPower) * fxMtrl.specular*fxLight.specular;
	float shadowFactor	= CalcShadowFactor(psInput.projTex);
    float4 light        = (amb + dif + spec) * shadowFactor;

	return float4(fColor, (w1 + w2 + w3)) * light;
}

float4 PS_Shadow(PS_INPUT psInput)
{	
	float4 fColor = float4(1, 1, 1, 1.0f);
	return fColor;
}

float4 PS(PS_INPUT psInput) : SV_Target
{
	if(fxShadowEnable)
		return PS_Shadow(psInput);
	else
		return PS_Normal(psInput);
}

/* ----------------------------------------------------------- Techniques */
technique10 Render
{
	pass P0
	{
		SetRasterizerState(noFrontCull);
		SetDepthStencilState(DepthWrite, 0);
		SetBlendState(blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );		
	}

	pass P1
	{
		//SetRasterizerState(noBackCull);
		SetDepthStencilState(DepthWrite, 0);

		SetVertexShader( CompileShader( vs_4_0, VS_SHADOW() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}
