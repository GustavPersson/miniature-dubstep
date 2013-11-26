cbuffer cbPerObject
{
	matrix world;
};

float4 VS( float4 pos : POSITION ) : SV_POSITION
{
    return mul(pos, world);
};

float4 PS() : SV_Target
{
	return float4(1.0f,0.0f,0.0f,1.0f);
}

technique10 Color
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}