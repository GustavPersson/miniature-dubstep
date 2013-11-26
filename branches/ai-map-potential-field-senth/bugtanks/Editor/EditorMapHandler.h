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

#ifndef EDITOR_MAP_HANDLER_H
#define EDITOR_MAP_HANDLER_H
#include "../Logic/ActorHandler.h"

namespace editor
{
	struct MapDataWorldPlane
	{
		D3DXVECTOR3 mPosition;
		int mTileX;
		int mTileZ;
		float mTileSizeX;
		float mTileSizeZ;
		int mTileWidth;
		int mTileHeight;

		MapDataWorldPlane()
		{
			mTileX = 0;
			mTileZ = 0;
			mTileSizeX = 0.0f;
			mTileSizeZ = 0.0f;
			mTileWidth = 0;
			mTileHeight = 0;
		}

		~MapDataWorldPlane() {}
	};

	struct MapDataActor
	{
		int mType;
		int mSubType;
		D3DXVECTOR3 mPosition;
		D3DXVECTOR3 mDirection;
		D3DXVECTOR3 mScale;
		D3DXCOLOR mColor;

		MapDataActor() : mPosition(0.0f, 0.0f, 0.0f), mDirection(0.0f, 0.0f, 0.0f),
			mScale(0.0f, 0.0f, 0.0f), mColor(0.0f, 0.0f, 0.0f, 0.0f)
		{
			mType = 0;
			mSubType = 0;
		}

		~MapDataActor() {}
	};

	class EditorMapHandler
	{
		public:
			static void updateWorldPlane(const MapDataWorldPlane &data, const std::vector<D3DXVECTOR4> &weights);

			/**
			 * Sends a creation event based on the supplied actor data.
			 * @param data Data struct for the actor.
			 */
			static void createActorFromData(const MapDataActor &data);

			/**
			 * Packs the world plane into a data struct.
			 * @param data Outgoing reference to the data struct.
			 * @param weights Outgoing reference to the texture weight vector.
			 * @return Returns true if successful.
			 */
			static bool packWorldPlane(MapDataWorldPlane &data, std::vector<D3DXVECTOR4> &weights);

			/**
			 * Packs an actor into a data struct.
			 * @param pActor Pointer to the actor to pack.
			 * @param data Outgoing reference to the data struct.
			 * @return Returns true if successful.
			 */
			static bool packActor(logic::Actor *pActor, MapDataActor &data);

		protected:
			/**
			 * Default constructor.
			 */
			EditorMapHandler();

			/**
			 * Default destructor.
			 */
			virtual ~EditorMapHandler();

			/**
			 * Default copy constructor.
			 */
			EditorMapHandler(const EditorMapHandler &handler);

			/**
			 * Default assignment operator.
			 */
			const EditorMapHandler &operator=(const EditorMapHandler &handler);

		private:
	};
}

#endif