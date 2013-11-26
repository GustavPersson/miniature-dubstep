struct Material
{
	Material()
	{
		ZeroMemory(this, sizeof(Material));
	}

	D3DXVECTOR4 ambient;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;

	float power; // Specular power
};