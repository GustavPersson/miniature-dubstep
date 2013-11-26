//Texture2D fxDepthMap;
//
////Texture samplers
//SamplerState sampLinear
//{
//	Filter	 = MIN_MAG_MIP_LINEAR;
//    AddressU = Wrap;
//    AddressV = Wrap;
//};
//
//// Filled by vertex shader
//struct VS_IN
//{
//    float3 pos : POSITION;
//    float3 norm : NORMAL;
//    float2 tex : TEXTURE;
//    row_major float4x4 mTransform : mTransform;
//};
//
//struct VS_OUT
//{
//	float4 pos		: SV_POSITION;
//	float2 tex		: TEXTURE;
//	float4 color	: COLOR;
//};
//
//// Vertex Shader
//VS_OUT VS(VS_IN input)
//{	
//	VS_OUT vOut;
//
//	float4 InstancePosition = mul(float4(input.pos, 1), input.mTransform);
//    float4 ViewPos			= mul(InstancePosition, fxWorldM );
//
//	float4 v4Position	= mul(InstancePosition, fxLightWVPM);
//    psInput.pos			= v4Position;
//
//	vOut.tex = vIn.tex;
//
//	return vOut;
//}
//
//void PS(VS_OUT pIn)
//{
//	//float4 diffuse = fxDepthMap.Sample(sampLinear, pIn.texC);
//	//clip(diffuse.a - 0.15f);
//}
//
//
//technique10 RenderShadowMap
//{
//	pass P0
//	{
//		SetVertexShader(CompileShader(vs_4_0, VS()));
//		SetGeometryShader(NULL);
//		SetPixelShader(CompileShader(ps_4_0, PS()));		
//	}
//}
