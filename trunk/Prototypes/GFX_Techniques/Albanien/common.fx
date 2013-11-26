Texture2D fxShadowMap;

static const float SHADOW_EPSILON = 0.01f;
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

DepthStencilState DepthWrite
{
	DepthEnable		= TRUE;
	DepthWriteMask	= ALL;
	DepthFunc		= LESS;
	StencilEnable	= FALSE;
};

SamplerState sampLinear
{
	Filter	 = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState gShadowSam
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};	

BlendState blend
{
	BlendEnable[0]	= TRUE;
	SrcBlend		= SRC_ALPHA;
	DestBlend		= INV_SRC_ALPHA;
};

RasterizerState noBackCull
{
	CullMode = Front;
};
RasterizerState noFrontCull
{
	CullMode = Back;
};

DepthStencilState NoDepthWrite
{
	DepthEnable		= TRUE;
	DepthWriteMask	= ZERO;

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
	float s0 = fxShadowMap.Sample(gShadowSam, projTexC.xy).r;
	float s1 = fxShadowMap.Sample(gShadowSam, projTexC.xy + float2(SMAP_DX, 0)).r;
	float s2 = fxShadowMap.Sample(gShadowSam, projTexC.xy + float2(0, SMAP_DX)).r;
	float s3 = fxShadowMap.Sample(gShadowSam, projTexC.xy + float2(SMAP_DX, SMAP_DX)).r;
	
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
