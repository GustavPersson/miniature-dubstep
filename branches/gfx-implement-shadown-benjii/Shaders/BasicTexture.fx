#include "ShaderBuffers.fx"
#include "ShaderStates.fx"
#include "ShaderFunctions.fx"

Texture2DArray TextureArray;

struct VS_TEX_INPUT
{
	float3 PosLS : POSITION;
	float3 NormLS : NORMAL;
	float3 TangLS : TANGENT;
	float2 UV : UV;
	float4 Color : COLOR;
	row_major float4x4 MTransform : TRANSFORM;
};

struct PS_TEX_INPUT
{
	float4 PosH : SV_POSITION;
	float2 UV : UV;
	float4 Color : COLOR_PS;
};

PS_TEX_INPUT VS_TEX(VS_TEX_INPUT input)
{
	float4x4 MTVP = mul(input.MTransform, MVP);

	PS_TEX_INPUT output;
	output.PosH = mul(float4(input.PosLS, 1.0f), MTVP);
	output.UV = input.UV;
	output.Color = input.Color;

	return output;
}

float4 PS_TEX(PS_TEX_INPUT input) : SV_Target
{
	return TextureArray.Sample(SampLinearWrap, float3(input.UV, 0.0f));
}

technique10 Tex
{
    pass P0
    {
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_TEX()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_TEX()));
    }
}