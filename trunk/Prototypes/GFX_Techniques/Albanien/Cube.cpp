#include "Cube.h"

// Transformationsmatriser för [MAX_INST] lådor
D3DXMATRIX g_treeInstanceMatrices[MAX_INST];

Cube::Cube(int nrOfInstances, const float width, ID3D10Device* device, D3DXVECTOR3 pos)
{
	gDevice		= device;

	D3DXMatrixIdentity(&mWorldM);
	D3DXMatrixIdentity(&mTranslationM);
	D3DXMatrixIdentity(&mRotXM);
	D3DXMatrixIdentity(&mRotYM);
	D3DXMatrixIdentity(&mRotZM);
	D3DXMatrixIdentity(&mScaleM);

	mPosV = pos;
	D3DXMatrixTranslation(&mTranslationM,mPosV.x,mPosV.y,mPosV.z);

	mRotX   = 0.0f;
	mRotY   = 0.0f;
	mRotZ   = 0.0f;
	mScale  = 0.0f;

	mMtrl.ambient	= D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	mMtrl.diffuse	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	mMtrl.specular	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	mMtrl.power		= 1.0f;

	mRebuildWorldMatrix = true;

	VertexPosTexNorm vertices[24];
	D3DXMATRIX temp;
	D3DXMatrixIsIdentity(&temp);

	// Fill in the front face vertex data.
 	vertices[0] = VertexPosTexNorm(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[1] = VertexPosTexNorm(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[2] = VertexPosTexNorm( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[3] = VertexPosTexNorm( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	vertices[4] = VertexPosTexNorm(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	vertices[5] = VertexPosTexNorm( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	vertices[6] = VertexPosTexNorm( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices[7] = VertexPosTexNorm(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	vertices[8]  = VertexPosTexNorm(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	vertices[9]  = VertexPosTexNorm(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[10] = VertexPosTexNorm( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	vertices[11] = VertexPosTexNorm( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	vertices[12] = VertexPosTexNorm(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	vertices[13] = VertexPosTexNorm( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	vertices[14] = VertexPosTexNorm( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[15] = VertexPosTexNorm(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	vertices[16] = VertexPosTexNorm(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[17] = VertexPosTexNorm(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[18] = VertexPosTexNorm(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[19] = VertexPosTexNorm(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	vertices[20] = VertexPosTexNorm( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[21] = VertexPosTexNorm( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[22] = VertexPosTexNorm( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[23] = VertexPosTexNorm( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
 
	mNumVertices = 24;

	// Scale the box.
	for(DWORD i = 0; i < (DWORD)mNumVertices; ++i)
		vertices[i].pos *= width;

	// Create VB
	D3D10_BUFFER_DESC buffDesc;
	buffDesc.ByteWidth		= sizeof(VertexPosTexNorm) * mNumVertices;	// Bytes required from buffer
	buffDesc.Usage			= D3D10_USAGE_DEFAULT;						// How the buffer will be used
	buffDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;					// Type of buffer being created
	buffDesc.CPUAccessFlags = 0;										// If the CPU should have acces to the data
	buffDesc.MiscFlags		= 0;										// 0 if you have a single device

	// Vertex buffer för Normal, position och textur
	D3D10_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices;
	HRESULT hr = gDevice->CreateBuffer(&buffDesc, &initData, &mVB[0]);
	if(FAILED(hr))
	{
		MessageBox(0, "Failed to create Vertex Buffer..", 0, 0);
		PostQuitMessage(0);
	}

	// Create VB
	buffDesc.ByteWidth		= sizeof(D3DXMATRIX) * MAX_INST;	// Bytes required from buffer
	buffDesc.Usage			= D3D10_USAGE_DYNAMIC;					// How the buffer will be used
	buffDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;				// Type of buffer being created
	buffDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;				// If the CPU should have acces to the data
	buffDesc.MiscFlags		= 0;									// 0 if you have a single device
	
	// Skapa transformationsmatriser
	CreateRandomMatrices();

	// Spara dem i VB
	initData.pSysMem = g_treeInstanceMatrices;
	hr = gDevice->CreateBuffer(&buffDesc, &initData, &mVB[1]);
	if(FAILED(hr))
	{
		MessageBox(0, "Failed to create Vertex Buffer2..", 0, 0);
		PostQuitMessage(0);
	}

	// Create IB
	DWORD indices[36];

	// Fill in the front face index data
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// Fill in the back face index data
	indices[6] = 4; indices[7]  = 5; indices[8]  = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// Fill in the top face index data
	indices[12] = 8; indices[13] =  9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// Fill in the bottom face index data
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// Fill in the left face index data
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// Fill in the right face index data
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	mNumIndices = sizeof(indices) / sizeof(DWORD);

	buffDesc.Usage			= D3D10_USAGE_DEFAULT;
	buffDesc.ByteWidth		= sizeof(DWORD) * mNumIndices;
	buffDesc.BindFlags		= D3D10_BIND_INDEX_BUFFER;
	buffDesc.CPUAccessFlags	= 0;
	buffDesc.MiscFlags		= 0;

	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	hr = gDevice->CreateBuffer( &buffDesc, &iinitData, &mIB );
	if(FAILED(hr))
	{
		MessageBox(0, "Failed to create Index Buffer..", 0, 0);
		PostQuitMessage(0);
	}

	CreateFX();	
}


Cube::~Cube()
{
	mVB[0]->Release();
	mVB[1]->Release();
	mIB->Release();

	if( mTextureRV )
		mTextureRV->Release();

	if( mTexture )
		mTexture->Release();

	mRenderEffect->Release();
	mInputLayout->Release();

}

void Cube::CreateFX()
{
	HRESULT hr;
	ID3D10Blob *compilationErrors = 0;
	D3DX10CreateEffectFromFile( "Default.fx", 
								NULL, 
								NULL, 
								"fx_4_0",
								D3D10_SHADER_ENABLE_STRICTNESS, 
								0, 
								gDevice, 
								NULL, 
								NULL, 
								&mRenderEffect, 
								&compilationErrors,
								&hr);

	if(FAILED(hr))
	{
		if(compilationErrors)
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
		}

		MessageBox(0, "Failed to create effect (cube)", 0, 0);
		PostQuitMessage(0);
	}

	// Load texture
	ID3D10Resource* resource = 0;
	hr = D3DX10CreateTextureFromFile( gDevice,		// DX device
									 "wood.dds",	// Filename of texture to load
									 NULL,			// How to load texture (D3DX10_IMAGE_LOAD_INFO)
									 NULL,			// Pointer to a thread. To load resource in a background thread
									 &resource,		// Resource object to store the data in
									 NULL );		// HRESULT

	if( FAILED( hr ) )
	{
		MessageBox(0, "Failed to load ID3D10Texture2D from file..", 0, 0);
		PostQuitMessage(0);
	}

	// Convert resource to texture
	hr = resource->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&mTexture );
	resource->Release();
	if( FAILED( hr ) )
	{
		MessageBox(0, "Failed to convert ID3D10Resource to texture..", 0, 0);
		PostQuitMessage(0);
	}
	
	//Get resource view
	D3D10_TEXTURE2D_DESC desc;
	mTexture->GetDesc(&desc);

    // Create a shader resource view of the texture
	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof(srvDesc) );
	srvDesc.Format				= desc.Format;
	srvDesc.ViewDimension		= D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
		
	gDevice->CreateShaderResourceView(mTexture, &srvDesc, &mTextureRV);

	D3DX10CreateShaderResourceViewFromFile(gDevice, "wood_normal.dds", 0, 0, &mTextureNRV, &hr);
	if(FAILED(hr))
	{
		MessageBox(0, "Failed to load tree0.dds from file..", 0, 0);
		PostQuitMessage(0);
	}

	// Get techniques
	mRenderTechnique = mRenderEffect->GetTechniqueByName( "Render" );
	mShadowTechnique = mRenderEffect->GetTechniqueByName( "RenderShadowMap" );

	// Bind variables
	mFXWVPM				= mRenderEffect->GetVariableByName( "fxWVPM" )->AsMatrix();
	fxLWVP				= mRenderEffect->GetVariableByName( "fxLWVPM" )->AsMatrix();
	mFxWorldM			= mRenderEffect->GetVariableByName( "fxWorldM" )->AsMatrix();
	mFXTextureVariable	= mRenderEffect->GetVariableByName( "fxTexture" )->AsShaderResource();
	fxLight				= mRenderEffect->GetVariableByName( "fxLight" );
	fxMtrl				= mRenderEffect->GetVariableByName( "fxMtrl" );
	fxEyePos			= mRenderEffect->GetVariableByName( "fxEyePos" );
	mfxShadowT			= mRenderEffect->GetVariableByName( "fxShadowMap" )->AsShaderResource();

	// Create inputlayout
    D3D10_PASS_DESC passDesc;
    mRenderTechnique->GetPassByIndex( 0 )->GetDesc(&passDesc);

	hr = gDevice->CreateInputLayout(VertexPosTexNormMat::inputDesc, 
									VertexPosTexNormMat::NUM_ELEMENTS, 
									passDesc.pIAInputSignature, 
									passDesc.IAInputSignatureSize, 
									&mInputLayout);

	if(FAILED(hr))
	{
		MessageBox(0, "Failed to create Input layout..", 0, 0);
		PostQuitMessage(0);
	}
}

void Cube::Render()
{
	gDevice->IASetInputLayout(mInputLayout);
	gDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set vertex buffer
	UINT stride[2];
	stride[0] = sizeof(VertexPosTexNorm);
	stride[1] = sizeof(D3DXMATRIX);

	UINT offset[2] = {0,0};

	gDevice->IASetIndexBuffer( mIB, DXGI_FORMAT_R32_UINT, 0 );
	gDevice->IASetVertexBuffers(0,			// Starting slot
								2,			// Number of vertex buffers
								mVB,		// Pointer to the VB arrays
								stride,		// Number of bytes each vertex requires
								offset);	// Offset to the first vertex in buffer

	// Normal
	D3D10_TECHNIQUE_DESC techDesc;
	mRenderTechnique->GetDesc( &techDesc );
	mRenderTechnique->GetPassByIndex( 0 )->Apply( 0 );
	gDevice->DrawIndexedInstanced( mNumIndices, MAX_INST, 0, 0, 0 );
}

void Cube::RenderShadow()
{
	gDevice->IASetInputLayout(mInputLayout);
	gDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set vertex buffer
	UINT stride[2];
	stride[0] = sizeof(VertexPosTexNorm);
	stride[1] = sizeof(D3DXMATRIX);

	UINT offset[2] = {0,0};

	gDevice->IASetIndexBuffer( mIB, DXGI_FORMAT_R32_UINT, 0 );
	gDevice->IASetVertexBuffers(0,			// Starting slot
								2,			// Number of vertex buffers
								mVB,		// Pointer to the VB arrays
								stride,		// Number of bytes each vertex requires
								offset);	// Offset to the first vertex in buffer

	// Normal
	D3D10_TECHNIQUE_DESC techDesc;
	mRenderTechnique->GetDesc( &techDesc );
	mRenderTechnique->GetPassByIndex( 1 )->Apply( 0 );
	gDevice->DrawIndexedInstanced( mNumIndices, MAX_INST, 0, 0, 0 );
}

void Cube::bindShadow(ID3D10ShaderResourceView* texture)
{
	mfxShadowT->SetResource(texture);
}

void Cube::Update(const D3DXMATRIX& view, const D3DXMATRIX& proj, const D3DXMATRIX& lview, const D3DXMATRIX& lproj, D3DXVECTOR3& eyePos, Light light)
{
	if( mRebuildWorldMatrix )
	{
		//Set the world matrice
		mWorldM = mScaleM * (mRotXM * mRotYM * mRotZM) * mTranslationM;
		mRebuildWorldMatrix = false;
	}

	//Set the shader WVP matrix
	mFXWVPM				->SetMatrix((float*)&(mWorldM * view * proj));
	fxLWVP				->SetMatrix((float*)&(mWorldM * lview * lproj));
	mFxWorldM			->SetMatrix((float*)&mWorldM);
	mFXTextureVariable	->SetResource(mTextureRV);
	fxLight				->SetRawValue(&light, 0, sizeof(light));
	fxMtrl				->SetRawValue(&mMtrl, 0, sizeof(mMtrl));
	fxEyePos			->SetRawValue(&eyePos, 0, sizeof(D3DXVECTOR3));
}

void Cube::Move(const float x, const float y, const float z)
{
	mPosV += D3DXVECTOR3(x,y,z);
	D3DXMatrixTranslation(&mTranslationM, mPosV.x, mPosV.y, mPosV.z);

	mRebuildWorldMatrix = true;
}

void Cube::Scale(const float scale)
{
	mScale += scale;
	D3DXMatrixScaling(&mScaleM, mScale, mScale, mScale);
}

void Cube::RotateX(const float angle)
{
	mRotX += angle;
	D3DXMatrixRotationX(&mRotXM, mRotX);
}

void Cube::RotateY(const float angle)
{
	mRotY += angle;
	D3DXMatrixRotationY(&mRotYM, mRotY);
}

void Cube::RotateZ(const float angle)
{
	mRotZ += angle;
	D3DXMatrixRotationZ(&mRotZM, mRotZ);
}

void Cube::updateTexture(ID3D10ShaderResourceView *tex)
{
	mTextureRV = tex;
	mFXTextureVariable->SetResource(mTextureRV);
}

// Fyller transformationsmatriser med positioner
HRESULT CreateRandomMatrices()
{
    for( int i = 0; i < MAX_INST; i++ )
    {
        //find a random position
        D3DXVECTOR3 pos;
        pos.x = -128.0f;
        pos.y = 5.0f;
        pos.z = 50.0f - i * MAX_INT;

        pos.x *= -1;
        pos.z *= -1;


        D3DXMATRIX mTrans;
        D3DXMATRIX mRot;
		D3DXMatrixIdentity(&mRot);

        D3DXMatrixTranslation( &mTrans, pos.x, pos.y, pos.z );

        g_treeInstanceMatrices[i] = mRot * mTrans;
    }
    return S_OK;
}