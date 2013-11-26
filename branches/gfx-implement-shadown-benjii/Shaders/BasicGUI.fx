#include "ShaderBuffers.fx"
#include "ShaderStates.fx"
#include "ShaderFunctions.fx"

Texture2DArray TextureArray;

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

technique10 GUI
{
    pass P0
    {
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthDisable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_GUI()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_GUI()));
    }
}