#ifndef MAIN_SHADER_GUI_FX
#define MAIN_SHADER_GUI_FX
#include "ShaderFunctions.fx"
#include "ShaderStates.fx"

struct VS_GUI_INPUT
{
	half4 PosLS : POSITION;
	half2 UV : UV;
	half4 Color : COLOR;
	int ArrayID : ARRAYID;
	int TexID : TEXID;
	row_major half4x4 MTransform : TRANSFORM;
};

struct PS_GUI_INPUT
{
	half4 PosH : SV_POSITION;
	half2 UV : UV_PS;
	half4 Color : COLOR_PS;
	int ArrayID : ARRAYID_PS;
	int TexID : TEXID_PS;
};

PS_GUI_INPUT VS_GUI(VS_GUI_INPUT input)
{
	//Transform to screen space.
	half4 position = mul(input.PosLS, input.MTransform);
	position.x = (2.0h * position.x) / (half)ViewportSize.x - 1.0h;
	position.y = (2.0h * position.y) / (half)ViewportSize.y + 1.0h;
	position.z = 0.0h;

	PS_GUI_INPUT output;
	output.PosH = position;
	output.ArrayID = input.ArrayID;
	output.TexID = input.TexID;
	output.UV = input.UV;
	output.Color = input.Color;

	return output;
}

half4 PS_GUI(PS_GUI_INPUT input) : SV_Target
{
	half4 color = input.Color;
	if(input.TexID >= 0)
	{
		half4 texColor = SampleTexture(input.ArrayID, input.TexID, input.UV);
		texColor *= input.Color;
		color = texColor;
	}
	return color;
}

#endif