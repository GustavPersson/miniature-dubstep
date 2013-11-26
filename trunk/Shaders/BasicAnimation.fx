#include "ShaderFunctions.fx"
#include "ShaderStates.fx"

Texture2DArray TextureArray;
Texture2DArray animations;

struct VS_CEL_INPUT
{
	float3 PosLS : POSITION;
	float3 NormLS : NORMAL;
	float3 TangLS : TANGENT;
	float2 UV : UV;
	float4 Diffuse : DIFFUSE;
	float4 Specular : SPECULAR;
	int TexID : TEXID;
	float4 Color : COLOR;
	row_major float4x4 MTransform : TRANSFORM;
	unsigned int VertexID : SV_VertexID;
};

struct GS_CEL_INPUT
{
	float4 PosH : SV_POSITION;
	float4 PosWS : POSITION_WS;
	float4 NormWS : NORMAL_WS;
	float2 UV : UV_GS;
	float4 Diffuse : DIFFUSE_GS;
	float4 Specular : SPECULAR_GS;
	int TexID : TEXID_GS;
	float4 Color : COLOR_GS;
	float Scale : SCALE_GS;
	uint id	: VertexID;
};

struct PS_CEL_INPUT
{
	float4 PosH : SV_POSITION;
	float2 UV : UV_PS;
	float4 Diffuse : DIFFUSE_GS;
	float4 Specular : SPECULAR_GS;
	int TexID : TEXID_PS;
	float4 Color : COLOR_PS;
	float Scale : SCALE_PS;
	unsigned int Type : TYPE;
};

GS_CEL_INPUT VS_CEL(VS_CEL_INPUT input)
{
	float4x4 MTVP = mul(input.MTransform, MVP);

	GS_CEL_INPUT output;
	output.PosH = mul(float4(input.PosLS, 1.0f), MTVP);
	output.PosWS = mul(float4(input.PosLS, 1.0f), input.MTransform);
	output.NormWS = normalize(mul(float4(input.NormLS, 0.0f), input.MTransform));
	output.TexID = input.TexID;
	output.UV = input.UV;
	output.Diffuse = input.Diffuse;
	output.Specular = input.Specular;
	output.Color = input.Color;

	float3 lightVector = float3(0.5f, 0.5f, 0.0f);
	output.Scale = 1.0f - dot(output.NormWS.xyz, lightVector);

	return output;
}

GS_CEL_INPUT VS_ANIMATION(VS_CEL_INPUT input)
{	
	int animationIndex = 1;
	int numberOfFrames = (int)animations.Load(float4( 0, 0, animationIndex, 0));
	float timePerFrame = animations.Load(float4( 1, 0, animationIndex, 0));

	float time = input.Color.r;

	time = time % (timePerFrame * numberOfFrames);

	int frameIndex = time / timePerFrame;

	float w1 = (time - frameIndex * timePerFrame) / timePerFrame;
	float w2 = 1 - w1;

	matrix finalMatrix;
	matrix tempMatrix;

	frameIndex += 1;

	/*frameIndex = 3;
	w1 = 0;
	w2 = 1;*/

	int jointIndex = 0; //joints[input.VertexID].x;

	if(frameIndex<numberOfFrames)
	{
		tempMatrix[0][0] = w2 * animations.Load(float4( jointIndex * 16 + 0, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 0, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][1] = w2 * animations.Load(float4( jointIndex * 16 + 1, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 1, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][2] = w2 * animations.Load(float4( jointIndex * 16 + 2, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 2, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][3] = w2 * animations.Load(float4( jointIndex * 16 + 3, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 3, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][0] = w2 * animations.Load(float4( jointIndex * 16 + 4, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 4, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][1] = w2 * animations.Load(float4( jointIndex * 16 + 5, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 5, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][2] = w2 * animations.Load(float4( jointIndex * 16 + 6, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 6, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][3] = w2 * animations.Load(float4( jointIndex * 16 + 7, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 7, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][0] = w2 * animations.Load(float4( jointIndex * 16 + 8, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 8, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][1] = w2 * animations.Load(float4( jointIndex * 16 + 9, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 9, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][2] = w2 * animations.Load(float4( jointIndex * 16 + 10, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 10, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][3] = w2 * animations.Load(float4( jointIndex * 16 + 11, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 11, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][0] = w2 * animations.Load(float4( jointIndex * 16 + 12, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 12, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][1] = w2 * animations.Load(float4( jointIndex * 16 + 13, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 13, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][2] = w2 * animations.Load(float4( jointIndex * 16 + 14, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 14, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][3] = w2 * animations.Load(float4( jointIndex * 16 + 15, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 15, frameIndex + 1, animationIndex, 0));
	}
	else
	{
		tempMatrix[0][0] = w2 * animations.Load(float4( jointIndex * 16 + 0, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 0, 1, animationIndex, 0));
		tempMatrix[0][1] = w2 * animations.Load(float4( jointIndex * 16 + 1, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 1, 1, animationIndex, 0));
		tempMatrix[0][2] = w2 * animations.Load(float4( jointIndex * 16 + 2, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 2, 1, animationIndex, 0));
		tempMatrix[0][3] = w2 * animations.Load(float4( jointIndex * 16 + 3, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 3, 1, animationIndex, 0));
		tempMatrix[1][0] = w2 * animations.Load(float4( jointIndex * 16 + 4, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 4, 1, animationIndex, 0));
		tempMatrix[1][1] = w2 * animations.Load(float4( jointIndex * 16 + 5, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 5, 1, animationIndex, 0));
		tempMatrix[1][2] = w2 * animations.Load(float4( jointIndex * 16 + 6, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 6, 1, animationIndex, 0));
		tempMatrix[1][3] = w2 * animations.Load(float4( jointIndex * 16 + 7, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 7, 1, animationIndex, 0));
		tempMatrix[2][0] = w2 * animations.Load(float4( jointIndex * 16 + 8, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 8, 1, animationIndex, 0));
		tempMatrix[2][1] = w2 * animations.Load(float4( jointIndex * 16 + 9, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 9, 1, animationIndex, 0));
		tempMatrix[2][2] = w2 * animations.Load(float4( jointIndex * 16 + 10, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 10, 1, animationIndex, 0));
		tempMatrix[2][3] = w2 * animations.Load(float4( jointIndex * 16 + 11, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 11, 1, animationIndex, 0));
		tempMatrix[3][0] = w2 * animations.Load(float4( jointIndex * 16 + 12, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 12, 1, animationIndex, 0));
		tempMatrix[3][1] = w2 * animations.Load(float4( jointIndex * 16 + 13, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 13, 1, animationIndex, 0));
		tempMatrix[3][2] = w2 * animations.Load(float4( jointIndex * 16 + 14, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 14, 1, animationIndex, 0));
		tempMatrix[3][3] = w2 * animations.Load(float4( jointIndex * 16 + 15, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 15, 1, animationIndex, 0));	
	}
	
    //finalMatrix = weights[input.VertexID].x * tempMatrix;
	
	//jointIndex = joints[input.VertexID].y;
	
	if(frameIndex<numberOfFrames)
	{
		tempMatrix[0][0] = w2 * animations.Load(float4( jointIndex * 16 + 0, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 0, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][1] = w2 * animations.Load(float4( jointIndex * 16 + 1, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 1, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][2] = w2 * animations.Load(float4( jointIndex * 16 + 2, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 2, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][3] = w2 * animations.Load(float4( jointIndex * 16 + 3, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 3, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][0] = w2 * animations.Load(float4( jointIndex * 16 + 4, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 4, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][1] = w2 * animations.Load(float4( jointIndex * 16 + 5, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 5, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][2] = w2 * animations.Load(float4( jointIndex * 16 + 6, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 6, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][3] = w2 * animations.Load(float4( jointIndex * 16 + 7, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 7, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][0] = w2 * animations.Load(float4( jointIndex * 16 + 8, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 8, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][1] = w2 * animations.Load(float4( jointIndex * 16 + 9, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 9, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][2] = w2 * animations.Load(float4( jointIndex * 16 + 10, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 10, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][3] = w2 * animations.Load(float4( jointIndex * 16 + 11, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 11, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][0] = w2 * animations.Load(float4( jointIndex * 16 + 12, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 12, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][1] = w2 * animations.Load(float4( jointIndex * 16 + 13, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 13, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][2] = w2 * animations.Load(float4( jointIndex * 16 + 14, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 14, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][3] = w2 * animations.Load(float4( jointIndex * 16 + 15, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 15, frameIndex + 1, animationIndex, 0));
	}
	else
	{
		tempMatrix[0][0] = w2 * animations.Load(float4( jointIndex * 16 + 0, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 0, 1, animationIndex, 0));
		tempMatrix[0][1] = w2 * animations.Load(float4( jointIndex * 16 + 1, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 1, 1, animationIndex, 0));
		tempMatrix[0][2] = w2 * animations.Load(float4( jointIndex * 16 + 2, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 2, 1, animationIndex, 0));
		tempMatrix[0][3] = w2 * animations.Load(float4( jointIndex * 16 + 3, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 3, 1, animationIndex, 0));
		tempMatrix[1][0] = w2 * animations.Load(float4( jointIndex * 16 + 4, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 4, 1, animationIndex, 0));
		tempMatrix[1][1] = w2 * animations.Load(float4( jointIndex * 16 + 5, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 5, 1, animationIndex, 0));
		tempMatrix[1][2] = w2 * animations.Load(float4( jointIndex * 16 + 6, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 6, 1, animationIndex, 0));
		tempMatrix[1][3] = w2 * animations.Load(float4( jointIndex * 16 + 7, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 7, 1, animationIndex, 0));
		tempMatrix[2][0] = w2 * animations.Load(float4( jointIndex * 16 + 8, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 8, 1, animationIndex, 0));
		tempMatrix[2][1] = w2 * animations.Load(float4( jointIndex * 16 + 9, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 9, 1, animationIndex, 0));
		tempMatrix[2][2] = w2 * animations.Load(float4( jointIndex * 16 + 10, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 10, 1, animationIndex, 0));
		tempMatrix[2][3] = w2 * animations.Load(float4( jointIndex * 16 + 11, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 11, 1, animationIndex, 0));
		tempMatrix[3][0] = w2 * animations.Load(float4( jointIndex * 16 + 12, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 12, 1, animationIndex, 0));
		tempMatrix[3][1] = w2 * animations.Load(float4( jointIndex * 16 + 13, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 13, 1, animationIndex, 0));
		tempMatrix[3][2] = w2 * animations.Load(float4( jointIndex * 16 + 14, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 14, 1, animationIndex, 0));
		tempMatrix[3][3] = w2 * animations.Load(float4( jointIndex * 16 + 15, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 15, 1, animationIndex, 0));	
	}
	
    //finalMatrix += weights[input.VertexID].y * tempMatrix;
	
	//jointIndex = joints[input.VertexID].z;

	if(frameIndex<numberOfFrames)
	{
		tempMatrix[0][0] = w2 * animations.Load(float4( jointIndex * 16 + 0, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 0, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][1] = w2 * animations.Load(float4( jointIndex * 16 + 1, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 1, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][2] = w2 * animations.Load(float4( jointIndex * 16 + 2, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 2, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][3] = w2 * animations.Load(float4( jointIndex * 16 + 3, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 3, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][0] = w2 * animations.Load(float4( jointIndex * 16 + 4, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 4, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][1] = w2 * animations.Load(float4( jointIndex * 16 + 5, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 5, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][2] = w2 * animations.Load(float4( jointIndex * 16 + 6, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 6, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][3] = w2 * animations.Load(float4( jointIndex * 16 + 7, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 7, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][0] = w2 * animations.Load(float4( jointIndex * 16 + 8, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 8, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][1] = w2 * animations.Load(float4( jointIndex * 16 + 9, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 9, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][2] = w2 * animations.Load(float4( jointIndex * 16 + 10, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 10, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][3] = w2 * animations.Load(float4( jointIndex * 16 + 11, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 11, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][0] = w2 * animations.Load(float4( jointIndex * 16 + 12, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 12, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][1] = w2 * animations.Load(float4( jointIndex * 16 + 13, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 13, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][2] = w2 * animations.Load(float4( jointIndex * 16 + 14, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 14, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][3] = w2 * animations.Load(float4( jointIndex * 16 + 15, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 15, frameIndex + 1, animationIndex, 0));
	}
	else
	{
		tempMatrix[0][0] = w2 * animations.Load(float4( jointIndex * 16 + 0, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 0, 1, animationIndex, 0));
		tempMatrix[0][1] = w2 * animations.Load(float4( jointIndex * 16 + 1, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 1, 1, animationIndex, 0));
		tempMatrix[0][2] = w2 * animations.Load(float4( jointIndex * 16 + 2, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 2, 1, animationIndex, 0));
		tempMatrix[0][3] = w2 * animations.Load(float4( jointIndex * 16 + 3, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 3, 1, animationIndex, 0));
		tempMatrix[1][0] = w2 * animations.Load(float4( jointIndex * 16 + 4, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 4, 1, animationIndex, 0));
		tempMatrix[1][1] = w2 * animations.Load(float4( jointIndex * 16 + 5, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 5, 1, animationIndex, 0));
		tempMatrix[1][2] = w2 * animations.Load(float4( jointIndex * 16 + 6, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 6, 1, animationIndex, 0));
		tempMatrix[1][3] = w2 * animations.Load(float4( jointIndex * 16 + 7, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 7, 1, animationIndex, 0));
		tempMatrix[2][0] = w2 * animations.Load(float4( jointIndex * 16 + 8, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 8, 1, animationIndex, 0));
		tempMatrix[2][1] = w2 * animations.Load(float4( jointIndex * 16 + 9, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 9, 1, animationIndex, 0));
		tempMatrix[2][2] = w2 * animations.Load(float4( jointIndex * 16 + 10, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 10, 1, animationIndex, 0));
		tempMatrix[2][3] = w2 * animations.Load(float4( jointIndex * 16 + 11, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 11, 1, animationIndex, 0));
		tempMatrix[3][0] = w2 * animations.Load(float4( jointIndex * 16 + 12, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 12, 1, animationIndex, 0));
		tempMatrix[3][1] = w2 * animations.Load(float4( jointIndex * 16 + 13, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 13, 1, animationIndex, 0));
		tempMatrix[3][2] = w2 * animations.Load(float4( jointIndex * 16 + 14, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 14, 1, animationIndex, 0));
		tempMatrix[3][3] = w2 * animations.Load(float4( jointIndex * 16 + 15, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 15, 1, animationIndex, 0));	
	}
	
    //finalMatrix += weights[input.VertexID].z * tempMatrix;
	
	//jointIndex = joints[input.VertexID].w;

	if(frameIndex<numberOfFrames)
	{
		tempMatrix[0][0] = w2 * animations.Load(float4( jointIndex * 16 + 0, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 0, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][1] = w2 * animations.Load(float4( jointIndex * 16 + 1, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 1, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][2] = w2 * animations.Load(float4( jointIndex * 16 + 2, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 2, frameIndex + 1, animationIndex, 0));
		tempMatrix[0][3] = w2 * animations.Load(float4( jointIndex * 16 + 3, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 3, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][0] = w2 * animations.Load(float4( jointIndex * 16 + 4, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 4, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][1] = w2 * animations.Load(float4( jointIndex * 16 + 5, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 5, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][2] = w2 * animations.Load(float4( jointIndex * 16 + 6, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 6, frameIndex + 1, animationIndex, 0));
		tempMatrix[1][3] = w2 * animations.Load(float4( jointIndex * 16 + 7, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 7, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][0] = w2 * animations.Load(float4( jointIndex * 16 + 8, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 8, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][1] = w2 * animations.Load(float4( jointIndex * 16 + 9, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 9, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][2] = w2 * animations.Load(float4( jointIndex * 16 + 10, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 10, frameIndex + 1, animationIndex, 0));
		tempMatrix[2][3] = w2 * animations.Load(float4( jointIndex * 16 + 11, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 11, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][0] = w2 * animations.Load(float4( jointIndex * 16 + 12, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 12, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][1] = w2 * animations.Load(float4( jointIndex * 16 + 13, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 13, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][2] = w2 * animations.Load(float4( jointIndex * 16 + 14, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 14, frameIndex + 1, animationIndex, 0));
		tempMatrix[3][3] = w2 * animations.Load(float4( jointIndex * 16 + 15, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 15, frameIndex + 1, animationIndex, 0));
	}
	else
	{
		tempMatrix[0][0] = w2 * animations.Load(float4( jointIndex * 16 + 0, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 0, 1, animationIndex, 0));
		tempMatrix[0][1] = w2 * animations.Load(float4( jointIndex * 16 + 1, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 1, 1, animationIndex, 0));
		tempMatrix[0][2] = w2 * animations.Load(float4( jointIndex * 16 + 2, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 2, 1, animationIndex, 0));
		tempMatrix[0][3] = w2 * animations.Load(float4( jointIndex * 16 + 3, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 3, 1, animationIndex, 0));
		tempMatrix[1][0] = w2 * animations.Load(float4( jointIndex * 16 + 4, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 4, 1, animationIndex, 0));
		tempMatrix[1][1] = w2 * animations.Load(float4( jointIndex * 16 + 5, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 5, 1, animationIndex, 0));
		tempMatrix[1][2] = w2 * animations.Load(float4( jointIndex * 16 + 6, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 6, 1, animationIndex, 0));
		tempMatrix[1][3] = w2 * animations.Load(float4( jointIndex * 16 + 7, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 7, 1, animationIndex, 0));
		tempMatrix[2][0] = w2 * animations.Load(float4( jointIndex * 16 + 8, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 8, 1, animationIndex, 0));
		tempMatrix[2][1] = w2 * animations.Load(float4( jointIndex * 16 + 9, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 9, 1, animationIndex, 0));
		tempMatrix[2][2] = w2 * animations.Load(float4( jointIndex * 16 + 10, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 10, 1, animationIndex, 0));
		tempMatrix[2][3] = w2 * animations.Load(float4( jointIndex * 16 + 11, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 11, 1, animationIndex, 0));
		tempMatrix[3][0] = w2 * animations.Load(float4( jointIndex * 16 + 12, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 12, 1, animationIndex, 0));
		tempMatrix[3][1] = w2 * animations.Load(float4( jointIndex * 16 + 13, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 13, 1, animationIndex, 0));
		tempMatrix[3][2] = w2 * animations.Load(float4( jointIndex * 16 + 14, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 14, 1, animationIndex, 0));
		tempMatrix[3][3] = w2 * animations.Load(float4( jointIndex * 16 + 15, frameIndex, animationIndex, 0)) + w1 * animations.Load(float4( jointIndex * 16 + 15, 1, animationIndex, 0));	
	}
	
    //finalMatrix += weights[input.VertexID].w * tempMatrix;
	
	finalMatrix = tempMatrix;

	float4 vAnimatedPos = mul(float4(input.PosLS, 1.0f), finalMatrix);
    float4 vAnimatedNormal = mul(float4(input.NormLS, 0.0f), finalMatrix);

	float4x4 MTVP = mul(input.MTransform, MVP);

	GS_CEL_INPUT output;
	output.PosH = mul(vAnimatedPos, MTVP);
	output.PosWS = mul(vAnimatedPos, input.MTransform);
	output.NormWS = normalize(mul(vAnimatedNormal, input.MTransform)); // normal to world space
	output.TexID = input.TexID;
	output.UV = input.UV;
	output.Diffuse = input.Diffuse;
	output.Specular = input.Specular;
	//output.Color = weights[input.VertexID];
	output.Color = input.Color;
	output.Color.a = 1.0f;

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
		output.Diffuse = input[nVertex].Diffuse;
		output.Specular = input[nVertex].Specular;
		output.TexID = input[nVertex].TexID;
		output.Color = input[nVertex].Color;
		output.Scale = input[nVertex].Scale;
		output.Type = 0;
		outstream.Append(output);
	}
	outstream.RestartStrip();

	//float3 viewDirection = -input[0].PosWS.xyz;
	float3 viewDirection = VCameraDirection;
	float3 normal = float3(0.0f, 0.0f, 0.0f);
	float3 normalAdjacent = float3(0.0f, 0.0f, 0.0f);
	float4 sideOffset = float4(0.0f, 0.0f, 0.0f, 0.0f);

	normal = normalize(cross(normalize(input[2].PosWS.xyz - input[0].PosWS.xyz),
					   normalize(input[4].PosWS.xyz - input[0].PosWS.xyz)));

	float edgeLength = 0.2f;
	float offsetLength = 0.1f;
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
				output.Diffuse = input[0].Diffuse;
				output.Specular = input[0].Specular;
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
				output.Diffuse = input[0].Diffuse;
				output.Specular = input[0].Specular;
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
				output.Diffuse = input[0].Diffuse;
				output.Specular = input[0].Specular;
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
				output.Diffuse = input[0].Diffuse;
				output.Specular = input[0].Specular;
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
				output.Diffuse = input[0].Diffuse;
				output.Specular = input[0].Specular;
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
				output.Diffuse = input[0].Diffuse;
				output.Specular = input[0].Specular;
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
	float shades = 10;
	float excludedShades = 3;
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);

	float4 textureColor = TextureArray.Sample(SampLinearWrap, float3(input.UV, input.TexID));

	if(input.TexID >= 0)
	{
		color = textureColor * input.Scale;
	}
	else
	{
		color = input.Color * input.Scale;
	}

	if(input.Type == 0)
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
	color.r = 0.8f;
	color.a = 1.0f;
	return color;
}

technique10 Color
{
    pass P0
    {
		//SetRasterizerState(noWireFrame);
		SetRasterizerState(wireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_CEL()));
        SetGeometryShader(CompileShader(gs_4_0, GS_CEL()));
        SetPixelShader(CompileShader(ps_4_0, PS_CEL()));
    }

	pass P1
    {
		SetRasterizerState(noWireFrame);
		//SetRasterizerState(wireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_ANIMATION()));
        SetGeometryShader(CompileShader(gs_4_0, GS_CEL()));
        SetPixelShader(CompileShader(ps_4_0, PS_CEL()));
    }

	pass P2
    {
		//SetRasterizerState(noWireFrame);
		SetRasterizerState(wireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_CEL()));
        SetGeometryShader(CompileShader(gs_4_0, GS_CEL()));
        SetPixelShader(CompileShader(ps_4_0, PS_CEL()));
    }
}