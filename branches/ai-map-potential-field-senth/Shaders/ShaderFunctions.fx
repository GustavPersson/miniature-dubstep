#ifndef SHADER_FUNCTIONS_FX
#define SHADER_FUNCTIONS_FX
#include "ShaderBuffers.fx"
#include "ShaderStates.fx"

half4 SampleTexture(int arrayIndex, int textureID, half2 UV)
{
	half4 textureColor = half4(0.0h, 0.0h, 0.0h, 1.0h);

	switch(arrayIndex)
	{
		case ARRAY_128:
		{
			half4 temp = (half4)TextureArray128.Load(int4(0, 0, textureID, 0));
			UV.x = UV.x * temp.x;
			UV.y = UV.y * temp.y;
			textureColor = (half4)TextureArray128.SampleLevel(SampLinearWrap, half3(UV, textureID), 0);
			break;
		}

		case ARRAY_256:
		{
			half4 temp = (half4)TextureArray256.Load(int4(0, 0, textureID, 0));
			UV.x = UV.x * temp.x;
			UV.y = UV.y * temp.y;
			textureColor = (half4)TextureArray256.SampleLevel(SampLinearWrap, half3(UV, textureID), 0);
			break;
		}

		case ARRAY_512:
		{
			half4 temp = (half4)TextureArray512.Load(int4(0, 0, textureID, 0));
			UV.x = UV.x * temp.x;
			UV.y = UV.y * temp.y;
			textureColor = (half4)TextureArray512.SampleLevel(SampLinearWrap, half3(UV, textureID), 0);
			break;
		}

		case ARRAY_1024:
		{
			half4 temp = (half4)TextureArray1024.Load(int4(0, 0, textureID, 0));
			UV.x = UV.x * temp.x;
			UV.y = UV.y * temp.y;
			textureColor = (half4)TextureArray1024.SampleLevel(SampLinearWrap, half3(UV, textureID), 0);
			break;
		}

		case ARRAY_FONT:
		{
			textureColor = (half4)FontArray.SampleLevel(SampLinearWrap, half3(UV, textureID), 0);
			break;
		}

		default:
			break;
	}

	return textureColor;
}

half4 SampleMultiTexture(half4 arrayID, half4 textureID, unsigned int instanceID, half2 UV)
{
	half4 textureColor = half4(0.0f, 0.0f, 0.0f, 1.0f);
	half4 weights = (half4)WorldPlaneArray.SampleLevel(SampLinearClamp, half3(UV, instanceID), 0);
	textureColor.rgb += SampleTexture((int)(arrayID.x + 0.01f), (int)(textureID.x + 0.01f), UV).rgb * weights.x;
	textureColor.rgb += SampleTexture((int)(arrayID.y + 0.01f), (int)(textureID.y + 0.01f), UV).rgb * weights.y;
	textureColor.rgb += SampleTexture((int)(arrayID.z + 0.01f), (int)(textureID.z + 0.01f), UV).rgb * weights.z;
	textureColor.rgb += SampleTexture((int)(arrayID.w + 0.01f), (int)(textureID.w + 0.01f), UV).rgb * weights.w;

	return textureColor;
}

half4 AddFade(half4 color, half4 posWS)
{
	half distance = (half)abs(length(posWS));
	if(distance > FADE_START)
	{
		half CFade = max(1.0h - (distance - FADE_START) / (FADE_MAX - FADE_START), 0.0h);
		color *= CFade;
	}
	return color;
}

float4x4 GetBillboardMatrix(float3 cameraPos, float3 posWS)
{
	float3 VCamera = normalize(cameraPos - posWS);
	float3 VRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), VCamera));
	float3 VUp = normalize(cross(VCamera, VRight));

	float4x4 BoardMW = {float4(VRight, 0.0f), float4(VUp, 0.0f), 
						float4(VCamera, 0.0f), float4(posWS, 1.0f)};

	return BoardMW;
}

/**
 * Calculates parallel light
 * @param mDiffuse Material color
 * @param mSpecular Materials specular color
 * @param shadowFactor Result of shadow calculation
 * @param inPos Pixels position in world space
 * @param inNorm Normal in world space
 * @param L Light settings
 * @param eyePos Camera position
 * @return Final color of lighting combined with shadowing
 */
float3 ParallelLight(float4 mDiffuse, float4 mSpecular, float shadowFactor, float3 inPos, float3 inNorm)
{
	// Initial light color
	float3 color			= VLight.ambient.rgb + lerp(VLight.ambient.rgb, shadowFactor, 0.5f);

	float diffuseFactor		= dot(-VLight.dir, inNorm);
	[branch]
	if(diffuseFactor > 0.0f)
	{
		// Specular calculations
		float3 r				= reflect(VLight.dir, inNorm);
		float4 specularFactor	= pow(max(dot(r, normalize(VCameraPosition - inPos)), 0.0f), VLight.specular.a);

		color					+= shadowFactor * diffuseFactor * VLight.diffuse.rgb;		// Add diffuse
		color					+= specularFactor.rgb * mSpecular.rgb * VLight.specular.rgb * shadowFactor;	// Add specular
		color *= 0.6f;
	}
	else
	{
		color = lerp(VLight.ambient.rgb, shadowFactor, 0.5f);
	}

	return mDiffuse.rgb * color;
}

#endif
