#include "ShaderBuffers.fx"
#include "ShaderStates.fx"
#include "ShaderFunctions.fx"

Texture2DArray TextureArray;

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
	float2 UV : UV_GS;
	int TexID : TEXID_GS;
	float4 Color : COLOR_GS;
	float Scale : SCALE_GS;
};

struct PS_CEL_INPUT
{
	float4 PosH : SV_POSITION;
	float2 UV : UV_PS;
	int TexID : TEXID_PS;
	float4 Color : COLOR_PS;
	float Scale : SCALE_PS;
	unsigned int Type : TYPE;
};

struct PS_CEL_SHADOW_INPUT
{
	half4 PosH : SV_POSITION;
	half4 PosWS : POSITION_WS;
	half4 NormWS : NORMAL_WS;
	float2 UV : UV_GS;
	int TexID : TEXID_GS;
	half4 Color : COLOR_GS;
	float Scale : SCALE_GS;
};

GS_CEL_INPUT VS_CEL(VS_CEL_INPUT input)
{
	float4x4 MTVP = mul(input.MTransform, MVP);

	GS_CEL_INPUT output;
	output.PosH = mul(float4(input.PosLS, 1.0f), MTVP);
	output.PosWS = mul(float4(input.PosLS, 1.0f), input.MTransform);
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
	float shades = 10.0f;
	float excludedShades = 3.0f;
	float4 textureColor = TextureArray.Sample(SampLinearWrap, float3(input.UV, input.TexID));
	textureColor *= input.Color;
	textureColor.rgb *= input.Scale;
	float4 color = input.Color;
	color.rgb *= input.Scale;

	if(input.TexID >= 0)
	{
		color = textureColor;
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

technique10 Cel
{
    pass P0
    {
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_CEL()));
        SetGeometryShader(CompileShader(gs_4_0, GS_CEL()));
        SetPixelShader(CompileShader(ps_4_0, PS_CEL()));
    }

	pass P1
	{
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	}
}