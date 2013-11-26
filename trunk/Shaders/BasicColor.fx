#include "ShaderFunctions.fx"
#include "ShaderStates.fx"

Texture2DArray TextureArray;

struct VS_COLOR_INPUT
{
	float3 PosLS : POSITION;
	float3 NormLS : NORMAL;
	float3 TangLS : TANGENT;
	float2 UV : UV;
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
	float4 Color : COLOR;
	row_major float4x4 MTransform : TRANSFORM;
};

struct PS_COLOR_INPUT
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR_PS;
};

PS_COLOR_INPUT VS_COLOR(VS_COLOR_INPUT input)
{
	float4x4 MTVP = mul(input.MTransform, MVP);

	PS_COLOR_INPUT output;
	output.PosH = mul(float4(input.PosLS, 1.0f), MTVP);
	output.Color = input.Color;

	return output;
}

float4 PS_COLOR(PS_COLOR_INPUT input) : SV_Target
{
	return input.Color;
}

technique10 Color
{
    pass P0
    {
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_COLOR()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_COLOR()));
    }
}