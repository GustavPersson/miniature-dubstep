static const float SHADOW_EPSILON = 0.001f;
static const float SMAP_SIZE = 1024.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;

struct Light
{
	float3 pos;
	float3 dir;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float  power;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float  specPower;
};

cbuffer cbPerFrame
{
	Light	 fxLight;
	Material fxMtrl;
	float3   fxEyePos;
};

cbuffer cbPerObject
{
	float4x4 fxLightWVP;
	float4x4 fxWorldM;
	float4x4 fxWVPM;
};

BlendState blend
{
	BlendEnable[0]	= TRUE;
	SrcBlend		= SRC_ALPHA;
	DestBlend		= INV_SRC_ALPHA;
};

DepthStencilState NoDepthWrite
{
	DepthEnable		= TRUE;
	DepthWriteMask	= ZERO;
};

Texture2D fxShadowMap;
Texture2D fxBlendT;
Texture2D fxDirtT;
Texture2D fxGrassT;
Texture2D fxSandT;

SamplerState sampLinear
{
	Filter		= MIN_MAG_MIP_LINEAR;
	AddressU	= Wrap;
	AddressV	= Wrap;
};

SamplerState sampShadow
{
	Filter		= MIN_MAG_MIP_POINT;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

float CalcShadowFactor(float4 projTexC)
{
	// Complete projection by doing division by w.
	projTexC.xyz /= projTexC.w;
	
	// Points outside the light volume are in shadow.
	if( projTexC.x < -1.0f || projTexC.x > 1.0f || 
	    projTexC.y < -1.0f || projTexC.y > 1.0f ||
	    projTexC.z < 0.0f )
	    return 0.0f;
	    
	// Transform from NDC space to texture space.
	projTexC.x = +0.5f*projTexC.x + 0.5f;
	projTexC.y = -0.5f*projTexC.y + 0.5f;

	// Depth in NDC space.
	float depth = projTexC.z;

	// Sample shadow map to get nearest depth to light.
	float s0 = fxShadowMap.Sample(sampShadow, projTexC.xy).r;
	float s1 = fxShadowMap.Sample(sampShadow, projTexC.xy + float2(SMAP_DX, 0)).r;
	float s2 = fxShadowMap.Sample(sampShadow, projTexC.xy + float2(0, SMAP_DX)).r;
	float s3 = fxShadowMap.Sample(sampShadow, projTexC.xy + float2(SMAP_DX, SMAP_DX)).r;

	// Is the pixel depth <= shadow map value?
	float result0 = depth <= s0 + SHADOW_EPSILON;
	float result1 = depth <= s1 + SHADOW_EPSILON;
	float result2 = depth <= s2 + SHADOW_EPSILON;
	float result3 = depth <= s3 + SHADOW_EPSILON;	
	
	// Transform to texel space.
	float2 texelPos = SMAP_SIZE*projTexC.xy;
	
	// Determine the interpolation amounts.
	float2 t = frac( texelPos );
	
	// Interpolate results.
	return lerp( lerp(result0, result1, t.x), 
	             lerp(result2, result3, t.x), t.y);
}

struct VS_IN
{
	float3 pos	: POSITION;
	float3 tan	: TANGENT;
	float3 norm : NORMAL;
	float2 texC : TEXCOORD;
};

struct VS_OUT
{
	float4 posH     : SV_POSITION;
    float3 posW     : POSITION;
    float3 tangentW : TANGENT;
    float3 normalW  : NORMAL;
    float2 tex0     : TEXCOORD0;
    float4 projTexC : TEXCOORD1;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;

	// Transform to world space
	vOut.posH		= mul(float4(vIn.pos, 1.0f), fxWVPM);
	vOut.posW		= mul(float4(vIn.pos, 1.0f), fxWorldM);
	vOut.tangentW	= mul(float4(vIn.tan, 0.0f), fxWorldM);
	vOut.normalW	= mul(float4(vIn.norm, 0.0f), fxWorldM);
	vOut.tex0		= vIn.texC;

	// Generate projective tex-coords to projekt shadowmap onto scene
	vOut.projTexC = mul(float4(vIn.pos, 1.0f), fxLightWVP);

	return vOut;
}

float4 PS(VS_OUT pIn) : SV_TARGET
{
	float4 cBlend = fxBlendT.Sample(sampLinear, pIn.tex0);
	float3 bColor = cBlend.r + cBlend.g + cBlend.b;

	float4 cDirt  = fxDirtT.Sample(sampLinear, pIn.tex0);
	float4 cGrass = fxGrassT.Sample(sampLinear, pIn.tex0);
	float4 cSand  = fxSandT.Sample(sampLinear, pIn.tex0);

	float w1 = cBlend.r / bColor;
	float w2 = cBlend.g / bColor;
	float w3 = cBlend.b / bColor;

	float3 fColor = (w2 * cGrass) + (w1 * cSand) + (w3 * cDirt);

	float  shadowFactor = 1.0f;//CalcShadowFactor(pIn.projTexC);
	float4 ldir         = float4(normalize((fxLight.pos - pIn.posW)),0.0f);
    float4 amb          = fxMtrl.ambient * fxLight.ambient;
    float4 dif          = max(dot(pIn.normalW, ldir),0) * fxMtrl.diffuse * fxLight.diffuse;
    float4 r            = float4(2*(dot(pIn.normalW, ldir)) * pIn.normalW - ldir,0.0f);
    float4 spec         = pow(max((dot(r,normalize(fxEyePos - pIn.posW))),0),fxMtrl.specPower) * fxMtrl.specular*fxLight.specular;
	
	float4 light		= shadowFactor * (dif + spec);
	light				+= amb;
   
	return float4(fColor, (w1 + w2 + w3)) * light;
}

technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
