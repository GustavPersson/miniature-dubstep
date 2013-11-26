#ifndef MAIN_SHADER_SHADOW_FX
#define MAIN_SHADER_SHADOW_FX
#include "MainShaderCel.fx"

struct PS_SHADOW_INPUT
{
	half4 PosH : SV_POSITION;
};

PS_SHADOW_INPUT VS_SHADOW(VS_CEL_INPUT input)
{
	half4x4 LTVP = mul(input.MTransform, (half4x4)LVP);

	PS_SHADOW_INPUT output;
	output.PosH = mul(input.PosLS, LTVP);

	return output;
}

half4 PS_SHADOW() : SV_Target
{
	return half4(1.0h, 1.0h, 1.0h, 1.0h);
}

#endif