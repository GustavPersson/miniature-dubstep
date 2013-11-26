#ifndef MAIN_SHADER_SHADOW_FX
#define MAIN_SHADER_SHADOW_FX
#include "MainShaderCel.fx"
#include "MainShaderAnimation.fx"

struct PS_SHADOW_INPUT
{
	half4 PosH : SV_POSITION;
};

/**************************************************************
 * Utilizing VertexTransform layout.
 **************************************************************/

PS_SHADOW_INPUT VS_SHADOW(VS_CEL_INPUT input)
{
	half4x4 LTVP = mul(input.MTransform, (half4x4)LVP);

	PS_SHADOW_INPUT output;
	output.PosH = mul(input.PosLS, LTVP);

	return output;
}

/**************************************************************
 * Utilizing VertexAnimation layout.
 **************************************************************/

PS_SHADOW_INPUT VS_SHADOW_ANIMATION(VS_ANIMATION_INPUT input)
{
	half4x4 LTVP = mul(input.MTransform, (half4x4)LVP);

	int animationIndex = (int)input.AnimationInfo.x;

	int numberOfFrames = (int)Animations.Load(float4( 0, 0, animationIndex, 0)).r;
	half timePerFrame = (half)Animations.Load(float4( 0, 0, animationIndex, 0)).g;

	half time = input.AnimationInfo.y;

	time = (half)(time % (timePerFrame * (half)(numberOfFrames - 1)));

	int frameIndex = time / timePerFrame;

	half w1 = (time - (half)frameIndex * timePerFrame) / timePerFrame;
	half w2 = 1 - w1;

	half4x4 finalMatrix = 0;
	finalMatrix[0][0] = 1;
	finalMatrix[1][1] = 1;
	finalMatrix[2][2] = 1;
	finalMatrix[3][3] = 1;
	half4x4 tempMatrix;

	frameIndex += 1;
		
	int jointIndex = 0;

	if(input.Weights.x > 0.1h)
	{
		jointIndex = input.Joints.x;

		if(frameIndex < numberOfFrames)
		{
			half4 texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex + 1, animationIndex, 0));

			tempMatrix[0][0] = texel.r;
			tempMatrix[0][1] = texel.g;
			tempMatrix[0][2] = texel.b;
			tempMatrix[0][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex + 1, animationIndex, 0));

			tempMatrix[1][0] = texel.r;
			tempMatrix[1][1] = texel.g;
			tempMatrix[1][2] = texel.b;
			tempMatrix[1][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex + 1, animationIndex, 0));

			tempMatrix[2][0] = texel.r;
			tempMatrix[2][1] = texel.g;
			tempMatrix[2][2] = texel.b;
			tempMatrix[2][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex + 1, animationIndex, 0));

			tempMatrix[3][0] = texel.r;
			tempMatrix[3][1] = texel.g;
			tempMatrix[3][2] = texel.b;
			tempMatrix[3][3] = texel.a;
		}
		else
		{
			half4 texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 0, 1, animationIndex, 0));

			tempMatrix[0][0] = texel.r;
			tempMatrix[0][1] = texel.g;
			tempMatrix[0][2] = texel.b;
			tempMatrix[0][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 1, 1, animationIndex, 0));

			tempMatrix[1][0] = texel.r;
			tempMatrix[1][1] = texel.g;
			tempMatrix[1][2] = texel.b;
			tempMatrix[1][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 2, 1, animationIndex, 0));

			tempMatrix[2][0] = texel.r;
			tempMatrix[2][1] = texel.g;
			tempMatrix[2][2] = texel.b;
			tempMatrix[2][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 3, 1, animationIndex, 0));

			tempMatrix[3][0] = texel.r;
			tempMatrix[3][1] = texel.g;
			tempMatrix[3][2] = texel.b;
			tempMatrix[3][3] = texel.a;
		}
		finalMatrix += input.Weights.x * tempMatrix;
	}
	
	if(input.Weights.y > 0.1h)
	{
		jointIndex = input.Joints.y;
		
		if(frameIndex < numberOfFrames)
		{
			half4 texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex + 1, animationIndex, 0));

			tempMatrix[0][0] = texel.r;
			tempMatrix[0][1] = texel.g;
			tempMatrix[0][2] = texel.b;
			tempMatrix[0][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex + 1, animationIndex, 0));

			tempMatrix[1][0] = texel.r;
			tempMatrix[1][1] = texel.g;
			tempMatrix[1][2] = texel.b;
			tempMatrix[1][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex + 1, animationIndex, 0));

			tempMatrix[2][0] = texel.r;
			tempMatrix[2][1] = texel.g;
			tempMatrix[2][2] = texel.b;
			tempMatrix[2][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex + 1, animationIndex, 0));

			tempMatrix[3][0] = texel.r;
			tempMatrix[3][1] = texel.g;
			tempMatrix[3][2] = texel.b;
			tempMatrix[3][3] = texel.a;
		}
		else
		{
			half4 texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 0, 1, animationIndex, 0));

			tempMatrix[0][0] = texel.r;
			tempMatrix[0][1] = texel.g;
			tempMatrix[0][2] = texel.b;
			tempMatrix[0][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 1, 1, animationIndex, 0));

			tempMatrix[1][0] = texel.r;
			tempMatrix[1][1] = texel.g;
			tempMatrix[1][2] = texel.b;
			tempMatrix[1][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 2, 1, animationIndex, 0));

			tempMatrix[2][0] = texel.r;
			tempMatrix[2][1] = texel.g;
			tempMatrix[2][2] = texel.b;
			tempMatrix[2][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 3, 1, animationIndex, 0));

			tempMatrix[3][0] = texel.r;
			tempMatrix[3][1] = texel.g;
			tempMatrix[3][2] = texel.b;
			tempMatrix[3][3] = texel.a;	
		}
		finalMatrix += input.Weights.y * tempMatrix;
	}
	
	if(input.Weights.z > 0.1h)
	{
		jointIndex = input.Joints.z;

		if(frameIndex < numberOfFrames)
		{
			half4 texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex + 1, animationIndex, 0));

			tempMatrix[0][0] = texel.r;
			tempMatrix[0][1] = texel.g;
			tempMatrix[0][2] = texel.b;
			tempMatrix[0][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex + 1, animationIndex, 0));

			tempMatrix[1][0] = texel.r;
			tempMatrix[1][1] = texel.g;
			tempMatrix[1][2] = texel.b;
			tempMatrix[1][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex + 1, animationIndex, 0));

			tempMatrix[2][0] = texel.r;
			tempMatrix[2][1] = texel.g;
			tempMatrix[2][2] = texel.b;
			tempMatrix[2][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex + 1, animationIndex, 0));

			tempMatrix[3][0] = texel.r;
			tempMatrix[3][1] = texel.g;
			tempMatrix[3][2] = texel.b;
			tempMatrix[3][3] = texel.a;
		}
		else
		{
			half4 texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 0, 1, animationIndex, 0));

			tempMatrix[0][0] = texel.r;
			tempMatrix[0][1] = texel.g;
			tempMatrix[0][2] = texel.b;
			tempMatrix[0][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 1, 1, animationIndex, 0));

			tempMatrix[1][0] = texel.r;
			tempMatrix[1][1] = texel.g;
			tempMatrix[1][2] = texel.b;
			tempMatrix[1][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 2, 1, animationIndex, 0));

			tempMatrix[2][0] = texel.r;
			tempMatrix[2][1] = texel.g;
			tempMatrix[2][2] = texel.b;
			tempMatrix[2][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 3, 1, animationIndex, 0));

			tempMatrix[3][0] = texel.r;
			tempMatrix[3][1] = texel.g;
			tempMatrix[3][2] = texel.b;
			tempMatrix[3][3] = texel.a;	
		}
		finalMatrix += input.Weights.z * tempMatrix;
	}
	
	if(input.Weights.w > 0.1h)
	{
		jointIndex = input.Joints.w;

		if(frameIndex < numberOfFrames)
		{
			half4 texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex + 1, animationIndex, 0));

			tempMatrix[0][0] = texel.r;
			tempMatrix[0][1] = texel.g;
			tempMatrix[0][2] = texel.b;
			tempMatrix[0][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex + 1, animationIndex, 0));

			tempMatrix[1][0] = texel.r;
			tempMatrix[1][1] = texel.g;
			tempMatrix[1][2] = texel.b;
			tempMatrix[1][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex + 1, animationIndex, 0));

			tempMatrix[2][0] = texel.r;
			tempMatrix[2][1] = texel.g;
			tempMatrix[2][2] = texel.b;
			tempMatrix[2][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex + 1, animationIndex, 0));

			tempMatrix[3][0] = texel.r;
			tempMatrix[3][1] = texel.g;
			tempMatrix[3][2] = texel.b;
			tempMatrix[3][3] = texel.a;
		}
		else
		{
			half4 texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 0, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 0, 1, animationIndex, 0));

			tempMatrix[0][0] = texel.r;
			tempMatrix[0][1] = texel.g;
			tempMatrix[0][2] = texel.b;
			tempMatrix[0][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 1, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 1, 1, animationIndex, 0));

			tempMatrix[1][0] = texel.r;
			tempMatrix[1][1] = texel.g;
			tempMatrix[1][2] = texel.b;
			tempMatrix[1][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 2, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 2, 1, animationIndex, 0));

			tempMatrix[2][0] = texel.r;
			tempMatrix[2][1] = texel.g;
			tempMatrix[2][2] = texel.b;
			tempMatrix[2][3] = texel.a;

			texel = w2 * (half4)Animations.Load(float4( jointIndex * 4 + 3, frameIndex, animationIndex, 0)) + w1 * (half4)Animations.Load(float4( jointIndex * 4 + 3, 1, animationIndex, 0));

			tempMatrix[3][0] = texel.r;
			tempMatrix[3][1] = texel.g;
			tempMatrix[3][2] = texel.b;
			tempMatrix[3][3] = texel.a;	
		}
		finalMatrix += input.Weights.w * tempMatrix;
	}

	half4 vAnimatedPos = mul(input.PosLS, finalMatrix);

	PS_SHADOW_INPUT output;
	if(animationIndex >= 0)
	{
		output.PosH = mul(vAnimatedPos, LTVP);
	}
	else
	{
		output.PosH = mul(input.PosLS, LTVP);
	}

	return output;
}

half4 PS_SHADOW() : SV_Target
{
	return half4(1.0h, 1.0h, 1.0h, 1.0h);
}

#endif