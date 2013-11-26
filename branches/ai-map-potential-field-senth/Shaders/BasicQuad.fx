// Input från Vertexbuffern
struct VSQuadIn
{
	uint vertexID : SV_VertexID;
};

struct PSQuadIn
{
	float4 pos	: SV_Position;
	float2 texC	: TEX;
};

Texture2D mTexture;

SamplerState sampLinear
{
	Filter	 = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

// State structures
DepthStencilState DisableDepth
{
    DepthEnable		= FALSE;
    DepthWriteMask	= ZERO;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

// Vertex Shader
PSQuadIn VS_Quad(VSQuadIn input)
{
	PSQuadIn output = (PSQuadIn)0;
	
	float scale = 0.5f;

	if(input.vertexID == 3)
		output.pos = float4(1.0, 1.0, 0.5, 1.0f);
	if(input.vertexID == 2)
		output.pos = float4(1.0, -1.0, 0.5, 1.0f);
	if(input.vertexID == 1)
		output.pos = float4(-1.0, 1.0, 0.5, 1.0f);
	if(input.vertexID == 0)
		output.pos = float4(-1.0, -1.0, 0.5, 1.0f);

	output.pos.xy *= scale;
	output.pos.x += 0.5f;
	output.pos.y -= 0.5f;

	output.texC = float2(0.5f, -0.5f) * output.pos.xy + 0.5f.xx;

	return output;
}

//Pixel Shader
float4 PS_Quad(PSQuadIn input) : SV_Target
{
	float r = mTexture.Sample(sampLinear, input.texC).r;
	return float4(r, r, r, 1);
}

//Techniques Render
technique10 RenderQuad
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Quad()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, PS_Quad()));

		SetRasterizerState( NoCulling );
	    SetDepthStencilState( DisableDepth, 0 );
	}
}
