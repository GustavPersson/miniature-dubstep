#include "ShaderBuffers.fx"
#include "ShaderStates.fx"
#include "ShaderFunctions.fx"

Texture2DArray	TextureArray;
//
Texture2D ShadowMap;
static const float SHADOW_EPSILON = 0.01f;
static const float SMAP_SIZE = 1024.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;
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
	float s0 = ShadowMap.Sample(SampLinearWrap, projTexC.xy).r;
	float s1 = ShadowMap.Sample(SampLinearWrap, projTexC.xy + float2(SMAP_DX, 0)).r;
	float s2 = ShadowMap.Sample(SampLinearWrap, projTexC.xy + float2(0, SMAP_DX)).r;
	float s3 = ShadowMap.Sample(SampLinearWrap, projTexC.xy + float2(SMAP_DX, SMAP_DX)).r;
	
	// Is the pixel depth <= shadow map value?
	float result0 = depth <= s0 + SHADOW_EPSILON;
	float result1 = depth <= s1 + SHADOW_EPSILON;
	float result2 = depth <= s2 + SHADOW_EPSILON;
	float result3 = depth <= s3 + SHADOW_EPSILON;	
	
	return result0;

	// Transform to texel space.
	float2 texelPos = SMAP_SIZE*projTexC.xy;
	
	// Determine the interpolation amounts.
	float2 t = frac( texelPos );
	
	// Interpolate results.
	return lerp( lerp(result0, result1, t.x), 
	             lerp(result2, result3, t.x), t.y);
}
//

struct VS_CEL_INPUT
{
	float3 PosLS : POSITION;
	float3 NormLS : NORMAL;
	float3 TangLS : TANGENT;
	float2 UV : UV;
	int TexID : TEXID;
	float4 Color : COLOR;
	row_major float4x4 MTransform : TRANSFORM;
};

struct GS_CEL_INPUT
{
	float4 PosH : SV_POSITION;
	float4 PosWS : POSITION_WS;
	float4 NormWS : NORMAL_WS;
	float4 ProjTex : TEXCOORD;
	float2 UV : UV_GS;
	int TexID : TEXID_GS;
	float4 Color : COLOR_GS;
	float Scale : SCALE_GS;
};

struct PS_CEL_INPUT
{
	float4 PosH : SV_POSITION;
	float4 ProjTex : TEXCOORD;
	float2 UV : UV_PS;
	int TexID : TEXID_PS;
	float4 Color : COLOR_PS;
	float Scale : SCALE_PS;
	unsigned int Type : TYPE;
};

struct PS_CEL_SHADOW_INPUT
{
	float4 PosH : SV_POSITION;
};

struct VS_GUI_INPUT
{
	float3 PosLS : POSITION;
	float3 NormLS : NORMAL;
	float3 TangLS : TANGENT;
	float2 UV : UV;
	int TexID : TEXID;
	float4 Color : COLOR;
	row_major float4x4 MTransform : TRANSFORM;
};

struct PS_GUI_INPUT
{
	float4 PosH : SV_POSITION;
	float2 UV : UV_PS;
	int TexID : TEXID_PS;
	float4 Color : COLOR_PS;
	float Scale : SCALE_PS;
	unsigned int Type : TYPE;
};

GS_CEL_INPUT VS_CEL(VS_CEL_INPUT input)
{
	float4x4 MTVP = mul(input.MTransform, MVP);
	float4x4 LTVP = mul(input.MTransform, LVP);

	GS_CEL_INPUT output;
	output.PosH = mul(float4(input.PosLS, 1.0f), MTVP);
	output.PosWS = mul(float4(input.PosLS, 1.0f), input.MTransform);
	output.ProjTex = mul(float4(input.PosLS, 1.0f), LTVP);
	output.NormWS = float4(normalize(mul(float4(input.NormLS, 0.0f), input.MTransform)));
	output.TexID = input.TexID;
	output.UV = input.UV;
	output.Color = input.Color;

	float3 lightVector = float3(0.5f, 0.5f, 0.0f);
	output.Scale = 1.0f - dot(output.NormWS.xyz, lightVector);

	return output;
}

[maxvertexcount(15)]
void GS_CEL(triangleadj GS_CEL_INPUT input[6], inout TriangleStream<PS_CEL_INPUT> outstream)
{
	PS_CEL_INPUT output;
	//The first three vertices are from the triangle.
	[unroll]
	for(int nVertex = 0; nVertex < 6; nVertex += 2)
	{
		output.PosH = input[nVertex].PosH;
		output.ProjTex = input[nVertex].ProjTex; // temtalsmd
		output.UV = input[nVertex].UV;
		output.TexID = input[nVertex].TexID;
		output.Color = input[nVertex].Color;
		output.Scale = input[nVertex].Scale;
		output.Type = 0;
		outstream.Append(output);
	}
	outstream.RestartStrip();

	//float3 viewDirection = -input[0].PosWS.xyz;
	float3 viewDirection = float3(VCameraDirection);
	float3 normal = float3(0.0f, 0.0f, 0.0f);
	float3 normalAdjacent = float3(0.0f, 0.0f, 0.0f);
	float4 sideOffset = float4(0.0f, 0.0f, 0.0f, 0.0f);

	normal = normalize(cross(normalize(input[2].PosWS.xyz - input[0].PosWS.xyz),
					   normalize(input[4].PosWS.xyz - input[0].PosWS.xyz)));

	float edgeLength = 1.0f;
	float offsetLength = 0.4f;
	int index1 = 0;
	int index2 = 0;
	int indexAdjacent = 0;
	//Front facing.
	if(dot(normal, viewDirection) < 0)
	{
		//Edge 1. Adjacency vertex index = 1.
		index1 = 2;
		index2 = 0;
		indexAdjacent = 1;
		normalAdjacent = normalize(cross(normalize(input[index1].PosWS.xyz - input[indexAdjacent].PosWS.xyz),
								   normalize(input[index2].PosWS.xyz - input[indexAdjacent].PosWS.xyz)));
		sideOffset = float4(normalize(input[index2].PosWS.xyz - input[index1].PosWS.xyz), 0.0f) * offsetLength;

		if(dot(normalAdjacent, viewDirection) >= 0)
		{
			for(unsigned int n = 0; n < 2; ++n)
			{
				float4 newPositionWS = input[index2].PosWS + n * float4(normal, 0.0f) * edgeLength + sideOffset;
				output.PosH = mul(newPositionWS, MVP);
				output.UV = input[index2].UV;
				output.TexID = -1;
				output.Color = input[index2].Color;
				output.Scale = input[index2].Scale;
				output.Type = 1;
				outstream.Append(output);
			}
			for(n = 0; n < 2; ++n)
			{
				float4 newPositionWS = input[index1].PosWS + n * float4(normal, 0.0f) * edgeLength - sideOffset;
				output.PosH = mul(newPositionWS, MVP);
				output.UV = input[index1].UV;
				output.TexID = -1;
				output.Color = input[index1].Color;
				output.Scale = input[index1].Scale;
				output.Type = 1;
				outstream.Append(output);
			}
			outstream.RestartStrip();
		}

		//Edge 2. Adjacency vertex index = 3.
		index1 = 4;
		index2 = 2;
		indexAdjacent = 3;
		normalAdjacent = normalize(cross(normalize(input[index1].PosWS.xyz - input[indexAdjacent].PosWS.xyz),
								   normalize(input[index2].PosWS.xyz - input[indexAdjacent].PosWS.xyz)));
		sideOffset = float4(normalize(input[index2].PosWS.xyz - input[index1].PosWS.xyz), 0.0f) * offsetLength;

		if(dot(normalAdjacent, viewDirection) >= 0)
		{
			for(unsigned int n = 0; n < 2; ++n)
			{
				float4 newPositionWS = input[index2].PosWS + n * float4(normal, 0.0f) * edgeLength + sideOffset;
				output.PosH = mul(newPositionWS, MVP);
				output.UV = input[index2].UV;
				output.TexID = -1;
				output.Color = input[index2].Color;
				output.Scale = input[index2].Scale;
				output.Type = 1;
				outstream.Append(output);
			}
			for(n = 0; n < 2; ++n)
			{
				float4 newPositionWS = input[index1].PosWS + n * float4(normal, 0.0f) * edgeLength - sideOffset;
				output.PosH = mul(newPositionWS, MVP);
				output.UV = input[index1].UV;
				output.TexID = -1;
				output.Color = input[index1].Color;
				output.Scale = input[index1].Scale;
				output.Type = 1;
				outstream.Append(output);
			}
			outstream.RestartStrip();
		}

		//Edge 3. Adjacency vertex index = 5.
		index1 = 0;
		index2 = 4;
		indexAdjacent = 5;
		normalAdjacent = normalize(cross(normalize(input[index1].PosWS.xyz - input[indexAdjacent].PosWS.xyz),
								   normalize(input[index2].PosWS.xyz - input[indexAdjacent].PosWS.xyz)));
		sideOffset = float4(normalize(input[index2].PosWS.xyz - input[index1].PosWS.xyz), 0.0f) * offsetLength;

		if(dot(normalAdjacent, viewDirection) >= 0)
		{
			for(unsigned int n = 0; n < 2; ++n)
			{
				float4 newPositionWS = input[index2].PosWS + n * float4(normal, 0.0f) * edgeLength + sideOffset;
				output.PosH = mul(newPositionWS, MVP);
				output.UV = input[index2].UV;
				output.TexID = -1;
				output.Color = input[index2].Color;
				output.Scale = input[index2].Scale;
				output.Type = 1;
				outstream.Append(output);
			}
			for(n = 0; n < 2; ++n)
			{
				float4 newPositionWS = input[index1].PosWS + n * float4(normal, 0.0f) * edgeLength - sideOffset;
				output.PosH = mul(newPositionWS, MVP);
				output.UV = input[index1].UV;
				output.TexID = -1;
				output.Color = input[index1].Color;
				output.Scale = input[index1].Scale;
				output.Type = 1;
				outstream.Append(output);
			}
			outstream.RestartStrip();
		}
	}
}

float4 PS_CEL(PS_CEL_INPUT input) : SV_Target
{
	// +Shadows
	float smSize		= 1024.0f;
	float smTexelSize	= 1.0f / smSize;

	input.ProjTex.xyz	/= input.ProjTex.w;
	input.ProjTex.x		= +0.5f * input.ProjTex.x + 0.5f;
	input.ProjTex.y		= -0.5f * input.ProjTex.y + 0.5f;
	float depth			= input.ProjTex.z;

	float r[9];
	r[0] = ShadowMap.Sample(SampLinearWrap, input.ProjTex.xy).r;
	r[1] = ShadowMap.Sample(SampLinearWrap, input.ProjTex.xy + float2(0, smTexelSize)).r;
	r[2] = ShadowMap.Sample(SampLinearWrap, input.ProjTex.xy + float2(smTexelSize, 0)).r;
	r[3] = ShadowMap.Sample(SampLinearWrap, input.ProjTex.xy + float2(-smTexelSize, 0)).r;
	r[4] = ShadowMap.Sample(SampLinearWrap, input.ProjTex.xy + float2(0, -smTexelSize)).r;
	r[5] = ShadowMap.Sample(SampLinearWrap, input.ProjTex.xy + float2(-smTexelSize, smTexelSize)).r;
	r[6] = ShadowMap.Sample(SampLinearWrap, input.ProjTex.xy + float2(smTexelSize, -smTexelSize)).r;
	r[7] = ShadowMap.Sample(SampLinearWrap, input.ProjTex.xy + float2(-smTexelSize, -smTexelSize)).r;
	r[8] = ShadowMap.Sample(SampLinearWrap, input.ProjTex.xy + float2(smTexelSize, smTexelSize)).r;

	float shadow = 0.0f;
	for(int i = 0; i < 9; i++)
	{
		if(r[i] >= (depth - 0.01f))
			shadow += (1.0f / 9.0f);
	}

	//float a = ShadowMap.Sample(SampLinearWrap, input.ProjTex.xy).r;
	//float shadow = depth <= a + 0.001f;
	// -Shadows

	float shades			= 10.0f;
	float excludedShades	= 3.0f;
	float4 textureColor		= TextureArray.Sample(SampLinearWrap, float3(input.UV, input.TexID));
	textureColor			*= input.Color;
	textureColor.rgb		*= input.Scale;
	float4 color			= input.Color;
	color.rgb				*= input.Scale;
	color.rgb				*= shadow;

	if(input.TexID >= 0)
	{
		color = textureColor;
		color.rgb *= shadow;
	}
	else if(input.Type == 0)
	{
		if(input.Scale < (excludedShades / shades))
		{
			color = float4(0.2f, 0.2f, 0.2f, 1.0f);
		}
	}
	else if(input.Type == 1)
	{
		color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	return color;
}

PS_GUI_INPUT VS_GUI(VS_GUI_INPUT input)
{
	//Retrieve scale information.
	float2 scale = float2(input.MTransform._11, input.MTransform._22);

	//Transform to screen space.
	float4 position = float4(input.PosLS, 1.0f);
	position = mul(position, input.MTransform);
	position.x -= ViewportSize.x * 0.5f;
	position.y += ViewportSize.y * 0.5f;

	PS_GUI_INPUT output;
	//Project to render plane.
	output.PosH = mul(position, MOrtP);
	output.TexID = input.TexID;
	output.UV = input.UV;
	output.Color = input.Color;

	return output;
}

float4 PS_GUI(PS_GUI_INPUT input) : SV_Target
{
	float4 texColor = TextureArray.Sample(SampLinearWrap, float3(input.UV, input.TexID));
	texColor *= input.Color;
	float4 color = input.Color;
	if(input.TexID >= 0)
	{
		color = texColor;
	}
	return color;
}

PS_CEL_SHADOW_INPUT VS_CEL_SHADOW(VS_CEL_INPUT input)
{
	float4x4 LTVP = mul(input.MTransform, LVP);

	PS_CEL_SHADOW_INPUT output;
	output.PosH = mul(float4(input.PosLS, 1.0f), LTVP);

	return output;
}

float4 PS_CEL_SHADOW() : SV_Target
{
	return float4(1,1,1,1);
}

technique10 Main
{
	pass GUI
	{
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthDisable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_GUI()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_GUI()));
	}

    pass Cel
    {
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_CEL()));
        SetGeometryShader(CompileShader(gs_4_0, GS_CEL()));
        SetPixelShader(CompileShader(ps_4_0, PS_CEL()));
    }

	pass Shadow
	{
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		SetVertexShader(CompileShader(vs_4_0, VS_CEL_SHADOW()));
        SetPixelShader(CompileShader(ps_4_0, PS_CEL_SHADOW()));
	}
}