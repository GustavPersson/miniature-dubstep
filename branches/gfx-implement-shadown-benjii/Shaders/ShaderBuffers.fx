
cbuffer cbPerFrame
{
	float2 ViewportSize;
	float4x4 LVP;
	float4x4 LV;
	float4x4 LP;
	float4x4 MVP;
	float4x4 MV;
	float4x4 MP;
	float4x4 MOrtP;
	float3 VCameraPosition;
	float3 VCameraDirection;
};

cbuffer animation
{
	float4	  g_joints[1600];
	float4    g_weights[1600];
};