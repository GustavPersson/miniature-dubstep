#include "ShaderBuffers.fx"
#include "ShaderStates.fx"
#include "ShaderFunctions.fx"

Texture2DArray TextureArray;
Texture2DArray animations;

struct VS_CEL_INPUT
{
	float3 PosLS : POSITION;
	float3 NormLS : NORMAL;
	float3 TangLS : TANGENT;
	float2 UV : UV;
	//unsigned int VertexID : SV_VertexID;
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
	int TexID : TEXID_GS;
	float4 Color : COLOR_GS;
	float Scale : SCALE_GS;
};

struct PS_CEL_INPUT
{
	float4 PosH : SV_POSITION;
	float2 UV : UV_PS;
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
	output.Color = input.Color;

	float3 lightVector = float3(0.5f, 0.5f, 0.0f);
	output.Scale = 1.0f - dot(output.NormWS.xyz, lightVector);

	return output;
}

GS_CEL_INPUT VS_ANIMATION(VS_CEL_INPUT input)
{
	float2x4 dual = (float2x4)0;
	float2x4 m = (float2x4)0;

	float duration = 20000;

	if(input.Color.r>duration)
	{
		input.Color.r -= duration;
	}

	int frame = (input.Color.r / duration) * 100;

	int vertexId = input.VertexID % 6;
	int animationId = 0;

	float index0, index1, index2, index3, index4, index5, index6, index7;

	float m11, m12, m13, m14, m21, m22, m23, m24;

	index0 = g_joints[vertexId].x * 8 + 0;
	index1 = g_joints[vertexId].x * 8 + 1;
	index2 = g_joints[vertexId].x * 8 + 2;
	index3 = g_joints[vertexId].x * 8 + 3;
	index4 = g_joints[vertexId].x * 8 + 4;
	index5 = g_joints[vertexId].x * 8 + 5;
	index6 = g_joints[vertexId].x * 8 + 6;
	index7 = g_joints[vertexId].x * 8 + 7;	
	
	m11 = (float)(animations.Load(float4(index0,frame,animationId,0)))/256;
	m12 = (float)(animations.Load(float4(index1,frame,animationId,0)))/256;
	m13 = (float)(animations.Load(float4(index2,frame,animationId,0)))/256;
	m14 = (float)(animations.Load(float4(index3,frame,animationId,0)))/256;
	m21 = (float)(animations.Load(float4(index4,frame,animationId,0)))/256;
	m22 = (float)(animations.Load(float4(index5,frame,animationId,0)))/256;
	m23 = (float)(animations.Load(float4(index6,frame,animationId,0)))/256;
	m24 = (float)(animations.Load(float4(index7,frame,animationId,0)))/256;

	m = float2x4(m11,m12,m13,m14,m21,m22,m23,m24);

    float4 dq0 = (float1x4)m ;

	dual = g_weights[vertexId].x * m ;

	index0 = g_joints[vertexId].y * 8 + 0;
	index1 = g_joints[vertexId].y * 8 + 1;
	index2 = g_joints[vertexId].y * 8 + 2;
	index3 = g_joints[vertexId].y * 8 + 3;
	index4 = g_joints[vertexId].y * 8 + 4;
	index5 = g_joints[vertexId].y * 8 + 5;
	index6 = g_joints[vertexId].y * 8 + 6;
	index7 = g_joints[vertexId].y * 8 + 7;	
	
	m11 = (float)(animations.Load(float4(index0,frame,animationId,0)))/256;
	m12 = (float)(animations.Load(float4(index1,frame,animationId,0)))/256;
	m13 = (float)(animations.Load(float4(index2,frame,animationId,0)))/256;
	m14 = (float)(animations.Load(float4(index3,frame,animationId,0)))/256;
	m21 = (float)(animations.Load(float4(index4,frame,animationId,0)))/256;
	m22 = (float)(animations.Load(float4(index5,frame,animationId,0)))/256;
	m23 = (float)(animations.Load(float4(index6,frame,animationId,0)))/256;
	m24 = (float)(animations.Load(float4(index7,frame,animationId,0)))/256;

	m = float2x4(m11,m12,m13,m14,m21,m22,m23,m24);

    float4 dq = (float1x4)m ;   
    if (dot( dq0, dq ) < 0)        
      dual -= g_weights[vertexId].y * m;
    else 
    dual += g_weights[vertexId].y * m;        


	index0 = g_joints[vertexId].z * 8 + 0;
	index1 = g_joints[vertexId].z * 8 + 1;
	index2 = g_joints[vertexId].z * 8 + 2;
	index3 = g_joints[vertexId].z * 8 + 3;
	index4 = g_joints[vertexId].z * 8 + 4;
	index5 = g_joints[vertexId].z * 8 + 5;
	index6 = g_joints[vertexId].z * 8 + 6;
	index7 = g_joints[vertexId].z * 8 + 7;	
	
	m11 = (float)(animations.Load(float4(index0,frame,animationId,0)))/256;
	m12 = (float)(animations.Load(float4(index1,frame,animationId,0)))/256;
	m13 = (float)(animations.Load(float4(index2,frame,animationId,0)))/256;
	m14 = (float)(animations.Load(float4(index3,frame,animationId,0)))/256;
	m21 = (float)(animations.Load(float4(index4,frame,animationId,0)))/256;
	m22 = (float)(animations.Load(float4(index5,frame,animationId,0)))/256;
	m23 = (float)(animations.Load(float4(index6,frame,animationId,0)))/256;
	m24 = (float)(animations.Load(float4(index7,frame,animationId,0)))/256;

	m = float2x4(m11,m12,m13,m14,m21,m22,m23,m24);

    dq = (float1x4)m ;          
    if (dot( dq0, dq ) < 0)        
      dual -= g_weights[vertexId].z * m;
    else 
    dual += g_weights[vertexId].z * m ;
            
	index0 = g_joints[vertexId].w * 8 + 0;
	index1 = g_joints[vertexId].w * 8 + 1;
	index2 = g_joints[vertexId].w * 8 + 2;
	index3 = g_joints[vertexId].w * 8 + 3;
	index4 = g_joints[vertexId].w * 8 + 4;
	index5 = g_joints[vertexId].w * 8 + 5;
	index6 = g_joints[vertexId].w * 8 + 6;
	index7 = g_joints[vertexId].w * 8 + 7;	
	
	m11 = (float)(animations.Load(float4(index0,frame,animationId,0)))/256;
	m12 = (float)(animations.Load(float4(index1,frame,animationId,0)))/256;
	m13 = (float)(animations.Load(float4(index2,frame,animationId,0)))/256;
	m14 = (float)(animations.Load(float4(index3,frame,animationId,0)))/256;
	m21 = (float)(animations.Load(float4(index4,frame,animationId,0)))/256;
	m22 = (float)(animations.Load(float4(index5,frame,animationId,0)))/256;
	m23 = (float)(animations.Load(float4(index6,frame,animationId,0)))/256;
	m24 = (float)(animations.Load(float4(index7,frame,animationId,0)))/256;

	m = float2x4(m11,m12,m13,m14,m21,m22,m23,m24);

    dq = (float1x4)m ;              
    if (dot( dq0, dq ) < 0)        
      dual -= g_weights[vertexId].w * m;
    else                
    dual += g_weights[vertexId].w * m;
	
	float4 Pos;
    float3 Norm, Tan, position, translation ; 

    // fast dqs
	
    float length = sqrt(dual[0].w * dual[0].w + dual[0].x * dual[0].x + dual[0].y * dual[0].y + dual[0].z * dual[0].z) ;
    dual = dual / length ; 
    position = input.PosLS + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, input.PosLS) + dual[0].w * input.PosLS) ;
    translation = 2.0 * (dual[0].w * dual[1].xyz - dual[1].w * dual[0].xyz + cross(dual[0].xyz, dual[1].xyz)) ; 
    position += translation ;
	
	Pos = float4(position,1.0f);
    Norm = input.NormLS.xyz + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz,input.NormLS) + dual[0].w * input.NormLS) ; 
    Tan = input.TangLS.xyz + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz,input.TangLS) + dual[0].w * input.TangLS) ;
    float4 vAnimatedPos = Pos ; 
    float4 vAnimatedNormal = float4(Norm, 0) ; 
    float4 vAnimatedTangent = float4(Tan, 0) ;

	float4x4 MTVP = mul(input.MTransform, MVP);
	
	GS_CEL_INPUT output;

    output.PosH = mul(vAnimatedPos, MTVP); 
    output.NormWS = normalize(mul(vAnimatedNormal, input.MTransform)); // normal to world space
	output.PosWS = mul(vAnimatedPos, input.MTransform);
	output.UV = input.UV;
	output.Color = input.Color;

	float3 lightVector = float3(0.5f, 0.5f, 0.0f);
	output.Scale = 1.0f - dot(output.NormWS.xyz, lightVector);

	output.TexID = input.TexID;

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

	float edgeLength = 1.0f;
	float offsetLength = 0.4f;
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

	if(input.TexID >= 0)
	{
		color = TextureArray.Sample(SampLinearWrap, float3(input.UV, input.TexID)) * input.Scale;
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
	return color;
}

technique10 Color
{
    pass P0
    {
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_ANIMATION()));
        SetGeometryShader(CompileShader(gs_4_0, GS_CEL()));
        SetPixelShader(CompileShader(ps_4_0, PS_CEL()));
    }
}