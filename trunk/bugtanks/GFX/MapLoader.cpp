/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
 * @version 1.0
 * Copyright (©) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "MapLoader.h"

using namespace gfx;

MapLoader::MapLoader()
{

}

MapLoader::~MapLoader()
{

}

void MapLoader::loadMap(const char *filePath)
{
	int tileX = 0;
	int tileY = 0;
	int tileSizeX = 0;
	int tileSizeY = 0;
	DX10Module *pModule = DX10Module::getInstance();
	pModule->loadObjectShapeInstance(RenderObject::ShapeTypes_Cube, 1, "Tile", 50000);

	std::string input;
	std::fstream file(filePath, std::ios_base::in);
	if(file.is_open())
	{
		file >> input;
		if(input == "Tiles:")
		{
			file >> tileX;
			file >> tileY;
		}
		file >> input;
		if(input == "TileSize:")
		{
			file >> tileSizeX;
			file >> tileSizeY;
		}

		//Create tiles.
		for(int x = 0; x < tileX; ++x)
		{
			for(int y = 0; y < tileY; ++y)
			{
				TransformObject *tile = pModule->createObjectInstanced(1);
				tile->setPosition(D3DXVECTOR3(x * tileSizeX + tileSizeX * 0.5f, 0.0f, y * tileSizeY + tileSizeY * 0.5f));
				tile->setScale(D3DXVECTOR3(tileSizeX * 0.5f, 0.5f, tileSizeY * 0.5f));
				tile->setTextureID(-1);
				tile->setColor(D3DXCOLOR(0.2f, 0.2f, 0.4f, 1.0f));
			}
		}

		file >> input;
		if(input == "Models:")
		{
			int ID = 0;
			file >> input;
			while(input != "Map:")
			{
				if(input == "ModelID:")
				{
					file >> ID;
				}
				file >> input;
				if(input == "GeometryFile:")
				{
					file >> input;
					pModule->loadMeshInstance(input.c_str(), "", ID, "Unknown", 5000, true);
				}
				file >> input;
			}
		}
		if(input == "Map:")
		{
			int ID = 0;
			int AID = 0;
			D3DXVECTOR3 position;
			D3DXVECTOR3 scale;
			D3DXVECTOR3 rotation;
			while(!file.eof())
			{
				file >> input;
				if(input == "ID:")
				{
					file >> ID;
				}
				file >> input;
				if(input == "ActorID:")
				{
					file >> AID;
				}
				file >> input;
				if(input == "Position:")
				{
					file >> position.x;
					file >> position.z;
					position.x = position.x * tileSizeX + tileSizeX * 0.5f;
					position.z = position.z * tileSizeX + tileSizeX * 0.5f;
					position.y = 0.0f;
				}
				file >> input;
				if(input == "Scale:")
				{
					file >> scale.x;
					file >> scale.y;
					file >> scale.z;
				}
				file >> input;
				if(input == "Rotation:")
				{
					file >> rotation.x;
					file >> rotation.y;
					file >> rotation.z;
				}
				InstanceMesh *mesh = pModule->createMeshInstanced(ID);
				mesh->setID(AID);
				mesh->setPosition(position);
				mesh->setScale(scale);
				mesh->setRotation(rotation);
				mesh->setTextureID(-1);
				mesh->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		file.close();
	}
}