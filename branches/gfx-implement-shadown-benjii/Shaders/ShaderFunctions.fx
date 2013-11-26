
float4x4 GetBillboardMatrix(float3 cameraPos, float3 posWS)
{
	float3 VCamera = normalize(cameraPos - posWS);
	float3 VRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), VCamera));
	float3 VUp = normalize(cross(VCamera, VRight));

	float4x4 BoardMW = {float4(VRight, 0.0f), float4(VUp, 0.0f), 
						float4(VCamera, 0.0f), float4(posWS, 1.0f)};

	return BoardMW;
}