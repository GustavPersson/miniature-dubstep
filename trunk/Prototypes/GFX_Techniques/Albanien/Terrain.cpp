#include "Terrain.h"

Terrain::Terrain(ID3D10Device* device, const UINT cols, const UINT rows, const UINT cellSize, const float heightScale)
{
	gDevice = device;

	HRESULT hr;

	D3DXMatrixIdentity(&mWorldM);
	D3DXMatrixIdentity(&mTranslationM);
	D3DXMatrixIdentity(&mRotXM);
	D3DXMatrixIdentity(&mRotYM);
	D3DXMatrixIdentity(&mRotZM);
	D3DXMatrixIdentity(&mScaleM);

	mRebuildWorldMatrix = true;

	CreateFX();

	//Floor material
	mMtrl.ambient	= D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	mMtrl.diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	mMtrl.specular	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	mMtrl.power		= 1.0f;

	// Sätt storleken på terrängen
	numVertRow		= rows;
	numVertCol		= cols;
	mCellSize		= cellSize;
	mHeightScale	= heightScale;

	VertexPosTexNorm* vertices = new VertexPosTexNorm[numVertCol * numVertRow];

	for(UINT z = 0; z < numVertRow; ++z)
	{
		for(UINT x = 0; x < numVertCol; ++x)
		{
			vertices[x + z * numVertCol].pos.x = (float)x * mCellSize;			// x för vertexpunkten
			vertices[x + z * numVertCol].pos.z = (float)z * mCellSize;			// z för vertexpunkten
			vertices[x + z * numVertCol].pos.y = 0.0f;							// y för vertexpunkten

			vertices[x + z * numVertCol].norm  = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // Normalen, riktad upp

			vertices[x + z * numVertCol].tex.x = x / (float)numVertCol; // u
			vertices[x + z * numVertCol].tex.y = z / (float)numVertRow; // v
		}
	}

	UINT numFaces		= (numVertCol-1) * (numVertRow-1) * 2;	// Antal trianglar på ytan
	UINT numIndices		= numFaces * 3;							//  Antalindex-punkter för ytan (3 per triangel)

	DWORD* indices	= new DWORD[numIndices];
	int current		= 0;
	for(UINT z = 0; z < numVertRow-1; ++z)
	{
		for(UINT x = 0; x < numVertCol-1; ++x)
		{
			int curVertex = x + (z * numVertCol); // Aktuell position att skapa trianglar från (?)

			//  0	 2,3
			//	 +----+
			//	 |   /|
			// 	 |  / |
			// 	 | /  |
			//	 |/   |
			//	 +----+
			//  1,4	   5

			indices[current + 0] = curVertex;
			indices[current + 1] = curVertex + numVertCol;
			indices[current + 2] = curVertex + 1;

			indices[current + 3] = curVertex + 1;
			indices[current + 4] = curVertex + numVertCol;
			indices[current + 5] = curVertex + numVertCol + 1;

			current += 6;
		}
	}

	hr = D3DX10CreateMesh(
			gDevice,
			VertexPosTexNorm::inputDesc,
			VertexPosTexNorm::NUM_ELEMENTS,
			"POSITION",
			(numVertCol * numVertRow),
			numFaces,
			D3DX10_MESH_32_BIT,
			&mMesh
		);

	if(FAILED(hr))
	{
		//MessageBoxW(0, L"Failed to create mesh (ShadowMap)", 0, 0)
		DEBUG_WINDOW("Failed to create mesh (ShadowMap)");
		PostQuitMessage(0);
	}

	// Bind vertices/indices
	mMesh->SetVertexData(0, vertices);
	mMesh->SetIndexData(indices, numIndices);
	mMesh->CommitToDevice();

	delete [] vertices;
	delete [] indices;

	D3DX10CreateShaderResourceViewFromFile(gDevice, "blend.dds", 0, 0, &mBlendRV, &hr);
	if(FAILED(hr))
	{
		MessageBox(0, "Failed to load blend.dds from file..", 0, 0);
		PostQuitMessage(0);
	}

	D3DX10CreateShaderResourceViewFromFile(gDevice, "dirt.dds", 0, 0, &mDirtRV, &hr);
	if(FAILED(hr))
	{
		MessageBox(0, "Failed to load dirt.dds from file..", 0, 0);
		PostQuitMessage(0);
	}
		
	D3DX10CreateShaderResourceViewFromFile(gDevice, "grass.dds", 0, 0, &mGrassRV, &hr);
	if(FAILED(hr))
	{
		MessageBox(0, "Failed to load grass.dds from file..", 0, 0);
		PostQuitMessage(0);
	}

	D3DX10CreateShaderResourceViewFromFile(gDevice, "sand.dds", 0, 0, &mSandRV, &hr);
	if(FAILED(hr))
	{
		MessageBox(0, "Failed to load grass.dds from file..", 0, 0);
		PostQuitMessage(0);
	}
}

Terrain::~Terrain()
{
	SAFE_RELEASE(mfxRenderEffect);
	SAFE_RELEASE(mMesh);

	SAFE_RELEASE(mDirtRV);
	SAFE_RELEASE(mSandRV);
	SAFE_RELEASE(mGrassRV);
}

void Terrain::CreateFX()
{
	HRESULT hr;
	ID3D10Blob *compilationErrors = 0;

	D3DX10CreateEffectFromFile(
		"Terrain.fx",	// Effect file
		NULL,			// 
		NULL,			//
		"fx_4_0",		// Shader version (4.0 for dx10)
		D3D10_SHADER_ENABLE_STRICTNESS, // Flags specifying how the shader code is compiled
		0,
		gDevice,		// Direct3D device
		NULL,
		NULL,
		&mfxRenderEffect,
		&compilationErrors,	// Pointer to errors (ID3D10Blob*), must be initiated first
		&hr
	);

	if(FAILED(hr))
	{
		if(compilationErrors)
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
		}

		MessageBox(0, "Failed to create effect (ShadowMap)", 0, 0);
		PostQuitMessage(0);
	}

	// Bind variables
	fxWVP		= mfxRenderEffect->GetVariableByName("fxWVPM")->AsMatrix();
	fxLWVP		= mfxRenderEffect->GetVariableByName( "fxLWVPM" )->AsMatrix();
	fxWorldM	= mfxRenderEffect->GetVariableByName("fxWorldM")->AsMatrix();
	fxShadowWVP = mfxRenderEffect->GetVariableByName("fxLightWVPM")->AsMatrix();
	fxLight		= mfxRenderEffect->GetVariableByName("fxLight");
	fxMtrl		= mfxRenderEffect->GetVariableByName("fxMtrl");
	fxEyePos	= mfxRenderEffect->GetVariableByName("fxEyePos");
	fxShadowBool = mfxRenderEffect->GetVariableByName("fxShadowEnable")->AsScalar();
	mfxBlendT	= mfxRenderEffect->GetVariableByName("fxBlendT")->AsShaderResource();
	mfxDirtT	= mfxRenderEffect->GetVariableByName("fxDirtT")->AsShaderResource();
	mfxGrassT	= mfxRenderEffect->GetVariableByName("fxGrassT")->AsShaderResource();
	mfxSandT	= mfxRenderEffect->GetVariableByName("fxSandT")->AsShaderResource();
	mfxShadowT	= mfxRenderEffect->GetVariableByName( "fxShadowMap" )->AsShaderResource();

	// Get render technique
	mfxRenderTechnique = mfxRenderEffect->GetTechniqueByName("Render");

	D3D10_PASS_DESC passDesc;
	mfxRenderTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	hr = gDevice->CreateInputLayout(
			VertexPosTexNorm::inputDesc,
			VertexPosTexNorm::NUM_ELEMENTS,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&mInputLayout
		);

	if(FAILED(hr))
	{
		MessageBox(0, "Failed to create input layout (ShadowMap).", 0, 0);
		PostQuitMessage(0);
	}
}


void Terrain::Update(D3DXMATRIX &mViewM, const D3DXMATRIX &mProjM, const D3DXMATRIX& lview, const D3DXMATRIX& lproj, D3DXVECTOR3 &eyePos, Light light, bool shadows)
{
	if(mRebuildWorldMatrix)
	{
		mWorldM				= mScaleM * (mRotXM * mRotYM * mRotZM) * mTranslationM;
		mRebuildWorldMatrix = false;
	}

	fxWVP		->SetMatrix((float*)&(mWorldM * mViewM * mProjM));
	fxLWVP		->SetMatrix((float*)&(mWorldM * lview * lproj));
	fxWorldM	->SetMatrix((float*)&mWorldM);
	fxLight		->SetRawValue(&light, 0, sizeof(light));
	fxMtrl		->SetRawValue(&mMtrl, 0, sizeof(mMtrl));
	fxEyePos	->SetRawValue(&eyePos, 0, sizeof(D3DXVECTOR3));
	fxShadowBool->SetBool(shadows);

	mfxBlendT	->SetResource(mBlendRV);
	mfxDirtT	->SetResource(mDirtRV);
	mfxGrassT	->SetResource(mGrassRV);
	mfxSandT	->SetResource(mSandRV);
}

void Terrain::Render()
{
	gDevice->IASetInputLayout(mInputLayout);

	D3D10_TECHNIQUE_DESC techDesc;
	mfxRenderTechnique->GetDesc(&techDesc);

	mfxRenderTechnique->GetPassByIndex(0)->Apply(0);
	mMesh->DrawSubset(0);
}

void Terrain::RenderShadow()
{
	gDevice->IASetInputLayout(mInputLayout);

	D3D10_TECHNIQUE_DESC techDesc;
	mfxRenderTechnique->GetDesc(&techDesc);

	mfxRenderTechnique->GetPassByIndex(1)->Apply(0);
	mMesh->DrawSubset(0);
}

void Terrain::bindShadow(ID3D10ShaderResourceView* texture)
{
	mfxShadowT->SetResource(texture);
}

float Terrain::getHeight(UINT x, UINT z)
{	
	float avg = 0.0f;
	float num = 0.0f;

	if(inBounds(x, z))
	{
		for(UINT m = z-1; m <= z+1; ++m)
		{
			for(UINT n = x-1; n <= x+1; ++n)
			{
				if(inBounds(x,z))
				{
					avg += mHeightMap[m*numVertCol + n];
					num += 1.0f;
				}
			}
		}
	}

	return (avg / num);
}

bool Terrain::inBounds(UINT z, UINT x)
{
	return x >= 0 && x < numVertRow && z >= 0 && z < numVertCol;
}