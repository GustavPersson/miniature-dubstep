
SamplerState SampLinearWrap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

RasterizerState wireFrame
{
	FillMode = WIREFRAME;
	DepthBias = 800;
	AntialiasedLineEnable = true;
};

RasterizerState noWireFrame
{
	FillMode = SOLID;
	DepthBias = 0;
	AntialiasedLineEnable = false;
};

DepthStencilState DepthEnable
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS;
	StencilEnable = FALSE;
};

DepthStencilState DepthDisable
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
	DepthFunc = LESS;
	StencilEnable = FALSE;
};

BlendState BlendAlphaAdd
{
	BlendEnable[0] = TRUE;
	AlphaToCoverageEnable = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

BlendState BlendDisable
{
	BlendEnable[0] = FALSE;
	AlphaToCoverageEnable = TRUE;
};