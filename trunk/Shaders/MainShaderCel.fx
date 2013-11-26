#ifndef MAIN_SHADER_CEL_FX
#define MAIN_SHADER_CEL_FX
#include "ShaderFunctions.fx"
#include "ShaderStates.fx"

struct VS_CEL_INPUT
{
	half4 PosLS : POSITION;
	half4 NormLS : NORMAL;
	half4 TangLS : TANGENT;
	half2 UV : UV;
	half4 Diffuse : DIFFUSE;
	half4 Specular : SPECULAR;
	int ArrayID : ARRAYID;
	int TexID : TEXID;
	half4 Color : COLOR;
	row_major half4x4 MTransform : TRANSFORM;
};

struct GS_CEL_INPUT
{
	half4 PosH : SV_POSITION;
	half4 PosWS : POSITION_GS;
	half4 NormWS : NORMAL_GS;
	half2 UV : UV_GS;
	half4 Diffuse : DIFFUSE_GS;
	half4 Specular : SPECULAR_GS;
	int ArrayID : ARRAYID_GS;
	int TexID : TEXID_GS;
	half4 Color : COLOR_GS;
	half Scale : SCALE_GS;
	half4 ProjTex : TEXCOORD_GS;
};

struct PS_CEL_INPUT
{
	half4 PosH : SV_POSITION;
	half4 PosWS : POSITION_PS;
	half4 NormWS : NORMAL_PS;
	half2 UV : UV_PS;
	half4 Diffuse : DIFFUSE_PS;
	half4 Specular : SPECULAR_PS;
	int ArrayID : ARRAYID_PS;
	int TexID : TEXID_PS;
	half4 Color : COLOR_PS;
	half Scale : SCALE_PS;
	half4 ProjTex : TEXCOORD_PS;
	unsigned int Type : TYPE;
};

PS_CEL_INPUT VS_CEL(VS_CEL_INPUT input)
{
	half4x4 MTVP = mul(input.MTransform, (half4x4)MVP);
	half4x4 LTVP = mul(input.MTransform, (half4x4)LVP);

	PS_CEL_INPUT output;
	output.PosH = mul(input.PosLS, MTVP);
	output.PosWS = mul(input.PosLS, input.MTransform);
	output.NormWS = (half4)normalize(mul(input.NormLS, input.MTransform));
	output.ArrayID = input.ArrayID;
	output.TexID = input.TexID;
	output.UV = input.UV;
	output.Color = input.Color;
	output.Diffuse = input.Diffuse;
	output.Specular = input.Specular;
	output.ProjTex = mul(input.PosLS, LTVP);
	output.Type = 0;

	half3 lightVector = half3(0.5f, 0.5f, 0.0f);
	output.Scale = 1.0h - dot(output.NormWS.xyz, (half3)VLight.dir);

	return output;
}

[maxvertexcount(15)]
void GS_CEL(triangleadj GS_CEL_INPUT input[6], inout TriangleStream<PS_CEL_INPUT> outstream)
{
	PS_CEL_INPUT output;
	//Add the vertices from the triangle.
	[unroll]
	for(int nVertex = 0; nVertex < 6; nVertex += 2)
	{
		output.PosH = input[nVertex].PosH;
		output.PosWS = input[nVertex].PosWS;
		output.NormWS = input[nVertex].NormWS;
		output.UV = input[nVertex].UV;
		output.Diffuse = input[nVertex].Diffuse;
		output.Specular = input[nVertex].Specular;
		output.ArrayID = input[nVertex].ArrayID;
		output.TexID = input[nVertex].TexID;
		output.Color = input[nVertex].Color;
		output.Scale = input[nVertex].Scale;
		output.ProjTex = input[nVertex].ProjTex;
		output.Type = 0;
		outstream.Append(output);
	}
	outstream.RestartStrip();

	if (input[0].Color.a > 0.5h)
	{
	half3 viewDirection = half3(VCameraDirection);
	half3 normal = half3(0.0f, 0.0f, 0.0f);
	half3 normalAdjacent = half3(0.0f, 0.0f, 0.0f);
	half4 sideOffset = half4(0.0f, 0.0f, 0.0f, 0.0f);

	normal = (half3)normalize(cross(normalize(input[2].PosWS.xyz - input[0].PosWS.xyz),
					   normalize(input[4].PosWS.xyz - input[0].PosWS.xyz)));

	half edgeLength = 0.3h;
	half offsetLength = 0.1h;
	if(input[0].TexID >= 0)
	{
		edgeLength = 0.1h;
		offsetLength = 0.01h;
	}
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
		normalAdjacent = (half3)normalize(cross(normalize(input[index1].PosWS.xyz - input[indexAdjacent].PosWS.xyz),
								   normalize(input[index2].PosWS.xyz - input[indexAdjacent].PosWS.xyz)));
		sideOffset = half4(normalize(input[index2].PosWS.xyz - input[index1].PosWS.xyz), 0.0f) * offsetLength;

		if(dot(normalAdjacent, viewDirection) >= 0)
		{
			for(unsigned int n = 0; n < 2; ++n)
			{
				half4 newPositionWS = input[index2].PosWS + (half)n * half4(normal, 0.0f) * edgeLength + sideOffset;
				output.PosH = mul(newPositionWS, (half4x4)MVP);
				output.PosWS = input[index2].PosWS;
				output.NormWS = input[index2].NormWS;
				output.UV = input[index2].UV;
				output.Diffuse = input[index2].Diffuse;
				output.Specular = input[index2].Specular;
				output.ArrayID = 0;
				output.TexID = -1;
				output.Color = input[index2].Color;
				output.Scale = input[index2].Scale;
				output.Type = 1;
				outstream.Append(output);
			}
			for(n = 0; n < 2; ++n)
			{
				half4 newPositionWS = input[index1].PosWS + (half)n * half4(normal, 0.0f) * edgeLength - sideOffset;
				output.PosH = mul(newPositionWS, (half4x4)MVP);
				output.PosWS = input[index1].PosWS;
				output.NormWS = input[index1].NormWS;
				output.UV = input[index1].UV;
				output.Diffuse = input[index1].Diffuse;
				output.Specular = input[index1].Specular;
				output.ArrayID = 0;
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
		normalAdjacent = (half3)normalize(cross(normalize(input[index1].PosWS.xyz - input[indexAdjacent].PosWS.xyz),
								   normalize(input[index2].PosWS.xyz - input[indexAdjacent].PosWS.xyz)));
		sideOffset = half4(normalize(input[index2].PosWS.xyz - input[index1].PosWS.xyz), 0.0f) * offsetLength;

		if(dot(normalAdjacent, viewDirection) >= 0)
		{
			for(unsigned int n = 0; n < 2; ++n)
			{
				half4 newPositionWS = input[index2].PosWS + (half)n * half4(normal, 0.0f) * edgeLength + sideOffset;
				output.PosH = mul(newPositionWS, (half4x4)MVP);
				output.PosWS = input[index2].PosWS;
				output.NormWS = input[index2].NormWS;
				output.UV = input[index2].UV;
				output.Diffuse = input[index2].Diffuse;
				output.Specular = input[index2].Specular;
				output.ArrayID = 0;
				output.TexID = -1;
				output.Color = input[index2].Color;
				output.Scale = input[index2].Scale;
				output.Type = 1;
				outstream.Append(output);
			}
			for(n = 0; n < 2; ++n)
			{
				half4 newPositionWS = input[index1].PosWS + (half)n * half4(normal, 0.0f) * edgeLength - sideOffset;
				output.PosH = mul(newPositionWS, (half4x4)MVP);
				output.PosWS = input[index1].PosWS;
				output.NormWS = input[index1].NormWS;
				output.UV = input[index1].UV;
				output.Diffuse = input[index1].Diffuse;
				output.Specular = input[index1].Specular;
				output.ArrayID = 0;
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
		normalAdjacent = (half3)normalize(cross(normalize(input[index1].PosWS.xyz - input[indexAdjacent].PosWS.xyz),
								   normalize(input[index2].PosWS.xyz - input[indexAdjacent].PosWS.xyz)));
		sideOffset = half4(normalize(input[index2].PosWS.xyz - input[index1].PosWS.xyz), 0.0f) * offsetLength;

		if(dot(normalAdjacent, viewDirection) >= 0)
		{
			for(unsigned int n = 0; n < 2; ++n)
			{
				half4 newPositionWS = input[index2].PosWS + (half)n * half4(normal, 0.0f) * edgeLength + sideOffset;
				output.PosH = mul(newPositionWS, (half4x4)MVP);
				output.PosWS = input[index2].PosWS;
				output.NormWS = input[index2].NormWS;
				output.UV = input[index2].UV;
				output.Diffuse = input[index2].Diffuse;
				output.Specular = input[index2].Specular;
				output.ArrayID = 0;
				output.TexID = -1;
				output.Color = input[index2].Color;
				output.Scale = input[index2].Scale;
				output.Type = 1;
				outstream.Append(output);
			}
			for(n = 0; n < 2; ++n)
			{
				half4 newPositionWS = input[index1].PosWS + (half)n * half4(normal, 0.0f) * edgeLength - sideOffset;
				output.PosH = mul(newPositionWS, (half4x4)MVP);
				output.PosWS = input[index1].PosWS;
				output.NormWS = input[index1].NormWS;
				output.UV = input[index1].UV;
				output.Diffuse = input[index1].Diffuse;
				output.Specular = input[index1].Specular;
				output.ArrayID = 0;
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
}

half4 PS_CEL(PS_CEL_INPUT input) : SV_Target
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
			sample = (half)ShadowMap.SampleLevel(SampLinearWrap, input.ProjTex.xy + half2(x, y) * smTexelSize, 0).r;
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

	if(input.TexID >= 0)
	{
		//Sample texture.
		color *= SampleTexture(input.ArrayID, input.TexID, input.UV);
		color = half4(ParallelLight(color, input.Specular, shadow, input.PosWS.xyz, normalize(input.NormWS.xyz)), input.Color.a);
		if(input.Scale < (excludedShades / shades))
		{
			color *= half4(0.7h, 0.7h, 0.7h, 1.0h);
		}
	}
	else if(input.Type == 0)
	{
		color = half4(ParallelLight(color, input.Specular, shadow, input.PosWS.xyz, normalize(input.NormWS.xyz)), input.Color.a);
		if(input.Scale < (excludedShades / shades))
		{
			color *= half4(0.7h, 0.7h, 0.7h, 1.0h);
		}
	}
	else if(input.Type == 1)
	{
		color = half4(0.0h, 0.0h, 0.0h, 1.0h);
	}

	color = AddFade(color, input.PosWS);

	return color;
}

#endif