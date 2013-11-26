#ifndef MAIN_SHADER_FX
#define MAIN_SHADER_FX
#include "MainShaderCel.fx"
#include "MainShaderMultiTex.fx"
#include "MainShaderShadow.fx"
#include "MainShaderGUI.fx"
#include "MainShaderParticle.fx"
#include "MainShaderAnimation.fx"

technique10 Main
{
	pass GUI
	{
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthDisable, 0);
		SetBlendState(BlendAlphaAdd, half4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_GUI()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_GUI()));
	}

    pass Cel
    {
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, half4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_CEL()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_CEL()));
    }

	pass Animation
	{
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, half4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_ANIMATION()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_ANIMATION()));
	}

	pass MultiTex
    {
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, half4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        SetVertexShader(CompileShader(vs_4_0, VS_MULTITEX()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_MULTITEX()));
    }

	pass Shadow
	{
		SetRasterizerState(noWireFrame);
		SetDepthStencilState(DepthEnable, 0);
		SetBlendState(BlendDisable, half4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		SetVertexShader(CompileShader(vs_4_0, VS_SHADOW()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_SHADOW()));
	}

	/**
	 * Particles - Small explosion
	 */
	pass ParticleStreamOut0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_PARTICLE_SO()));

		SetGeometryShader(
			ConstructGSWithSO(
								CompileShader(gs_4_0, GS_PARTICLE_SMALL_EXPLOSION_SO()), 
								"POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x; ID.x"
							 )
						 );

		SetPixelShader(NULL);

		SetDepthStencilState(DepthDisable, 0);
	}

	pass ParticleDraw0
	{
		SetVertexShader(	CompileShader(vs_4_0, VS_PARTICLE_SMALL_EXPLOSION_DRAW()));
		SetGeometryShader(	CompileShader(gs_4_0, GS_PARTICLE_SMALL_EXPLOSION_DRAW()));
        SetPixelShader(		CompileShader(ps_4_0, PS_PARTICLE_SMALL_EXPLOSION_DRAW()));

		SetDepthStencilState(DepthDisable, 0);
	}

	/**
	 * Particles - Gun sparks
	 */
	pass ParticleStreamOut1
	{
		SetVertexShader(CompileShader(vs_4_0, VS_PARTICLE_SO()));

		SetGeometryShader(
			ConstructGSWithSO(
								CompileShader(gs_4_0, GS_PARTICLE_SPARKS_SO()), 
								"POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x; ID.x"
							 )
						 );

		SetPixelShader(NULL);

		SetDepthStencilState(DepthDisable, 0);
	}

	pass ParticleDraw1
	{
		SetVertexShader(	CompileShader(vs_4_0, VS_PARTICLE_SPARKS_DRAW()));
		SetGeometryShader(	CompileShader(gs_4_0, GS_PARTICLE_SPARKS_DRAW()));
        SetPixelShader(		CompileShader(ps_4_0, PS_PARTICLE_SPARKS_DRAW()));

		SetDepthStencilState(DepthDisable, 0);
	}

	/**
	 * Particles - Something else?
	 */
	pass ParticleStreamOut2
	{
		SetVertexShader(CompileShader(vs_4_0, VS_PARTICLE_SO()));

		SetGeometryShader(
			ConstructGSWithSO(
								CompileShader(gs_4_0, GS_PARTICLE_SMALL_EXPLOSION_SO()), 
								"POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x; ID.x"
							 )
						 );

		SetPixelShader(NULL);

		SetDepthStencilState(DepthDisable, 0);
	}

	pass ParticleDraw2
	{
		SetVertexShader(	CompileShader(vs_4_0, VS_PARTICLE_SMALL_EXPLOSION_DRAW()));
		SetGeometryShader(	CompileShader(gs_4_0, GS_PARTICLE_SMALL_EXPLOSION_DRAW()));
        SetPixelShader(		CompileShader(ps_4_0, PS_PARTICLE_SMALL_EXPLOSION_DRAW()));

		SetDepthStencilState(DepthDisable, 0);
	}
}

#endif
