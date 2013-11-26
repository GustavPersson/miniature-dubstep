#ifndef MAIN_SHADER_MULTI_TEX_FX
#define MAIN_SHADER_MULTI_TEX_FX
#include "ShaderFunctions.fx"
#include "ShaderStates.fx"

struct VS_MULTITEX_INPUT
{
	half4 PosLS : POSITION;
	half4 NormLS : NORMAL;
	half4 TangLS : TANGENT;
	half2 UV : UV;
	half4 Diffuse : DIFFUSE;
	half4 Specular : SPECULAR;
	unsigned int ArraySlice : ARRAYSLICE;
	half4 ArrayID : ARRAYID;
	half4 TexID : TEXID;
	half4 Color : COLOR;
	row_major half4x4 MTransform : TRANSFORM;
};

struct PS_MULTITEX_INPUT
{
	half4 PosH : SV_POSITION;
	half4 PosWS : POSITION_PS;
	half4 NormWS : NORMAL_PS;
	half2 UV : UV_PS;
	half4 Diffuse : DIFFUSE_PS;
	half4 Specular : SPECULAR_PS;
	unsigned int ArraySlice: ARRAYSLICE_PS;
	half4 ArrayID : ARRAYID_PS;
	half4 TexID : TEXID_PS;
	half4 Color : COLOR_PS;
	half Scale : SCALE_PS;
	half4 ProjTex : TEXCOORD_PS;
};

PS_MULTITEX_INPUT VS_MULTITEX(VS_MULTITEX_INPUT input)
{
	half4x4 MTVP = mul(input.MTransform, (half4x4)MVP);
	half4x4 LTVP = mul(input.MTransform, (half4x4)LVP);

	PS_MULTITEX_INPUT output;
	output.PosH = mul(input.PosLS, MTVP);
	output.PosWS = mul(input.PosLS, input.MTransform);
	output.NormWS = (half4)normalize(mul(input.NormLS, input.MTransform));
	output.UV = input.UV * 1.0h;
	output.Diffuse = input.Diffuse;
	output.Specular = input.Specular;
	output.ArraySlice = input.ArraySlice;
	output.ArrayID = input.ArrayID;
	output.TexID = input.TexID;
	output.Color = input.Color;
	output.ProjTex = mul(input.PosLS, LTVP);

	half3 lightVector = half3(0.5f, 0.5f, 0.0f);
	output.Scale = 1.0h - dot(output.NormWS.xyz, lightVector);

	return output;
}

half4 PS_MULTITEX(PS_MULTITEX_INPUT input) : SV_Target
{
	// Shadow map variables
	half smSize	= 2048.0h;				// Shadowmap size
	half smTexelSize = 1.0h / smSize;	// Texel size

	input.ProjTex.xy /= input.ProjTex.w;
	input.ProjTex.x	= +0.5h * input.ProjTex.x + 0.5h;
	input.ProjTex.y	= -0.5h * input.ProjTex.y + 0.5h;

	half shadow	= 0.0h;					// Shadow value
	half interval = 2.5h;				// -2.5 => 2.5 with 1.0 interval equals
	half increment = 1.0h;				// 6x6 PCF sampling
	half sample	= 0.0h;

	for(half y = -interval; y < interval; y += increment)
	{
		for(half x = -interval; x <= interval; x += increment)
		{
			sample = (half)ShadowMap.SampleLevel(SampLinearClamp, input.ProjTex.xy + half2(x, y) * smTexelSize, 0).r;
			if(sample >= input.ProjTex.z - 0.001h)
			{
				shadow += sample;
			}
		}
	}

	//Average value of all samples (6x6 PCF = 36 samples)
	shadow /= 36.0h;

	half shades	= 10.0h;
	half excludedShades	= 3.0h;

	//Material
	half4 color	= input.Color;
	color.rgb *= input.Diffuse.rgb;

	if(input.TexID.r >= 0.0h)
	{
		color *= SampleMultiTexture(input.ArrayID, input.TexID, input.ArraySlice, input.UV);
		color = half4(ParallelLight(color, input.Specular, shadow, input.PosWS.xyz, normalize(input.NormWS.xyz)), input.Color.a);
		if(input.Scale < (excludedShades / shades))
		{
			color *= half4(0.7h, 0.7h, 0.7h, 1.0h);
		}
	}
	else
	{
		color = half4(ParallelLight(color, input.Specular, shadow, input.PosWS.xyz, normalize(input.NormWS.xyz)), input.Color.a);
		if(input.Scale < (excludedShades / shades))
		{
			color *= half4(0.7h, 0.7h, 0.7h, 1.0h);
		}
	}

	color = AddFade(color, input.PosWS);

	return color;
}

#endif