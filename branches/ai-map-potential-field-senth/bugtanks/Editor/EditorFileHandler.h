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

#ifndef EDITOR_FILE_HANDLER_H
#define EDITOR_FILE_HANDLER_H
#include <fstream>
#include <string>
#include "EditorMapHandler.h"
#include "../Logic/ActorHandler.h"

namespace editor
{
	class EditorFileHandler
	{
		public:
			/**
			 * Default constructor.
			 */
			EditorFileHandler();

			/**
			 * Default destructor.
			 */
			virtual ~EditorFileHandler();

			/**
			 * Saves the current map.
			 * @param mapName Name of the current map.
			 * @param overwrite True if any current map with the same name should be overwritten.
			 * @return Returns true if successful.
			 */
			bool saveMap(const char *mapName, bool overwrite);

			/**
			 * Loads a map.
			 * @param mapName Name of the map to load.
			 * @return Returns true if successful.
			 */
			bool loadMap(const char *mapName);

		protected:
			/**
			 * Default copy constructor.
			 */
			EditorFileHandler(const EditorFileHandler &handler);

			/**
			 * Default assignment operator.
			 */
			const EditorFileHandler &operator=(const EditorFileHandler &handler);

			/**
			 * Attempts to read a block of data.
			 * @param pData Pointer to the allocated memory to read to.
			 * @param dataSize Size of the data block to read.
			 * @param location Reference to the data location. Will increase
			 *  the location to be after the read data block.
			 */
			bool read(char *pData, unsigned int dataSize, unsigned int &location);

			/**
			 * Attempts to write a block of data.
			 * @param pData Pointer to the data that should be written.
			 * @param dataSize Size of the data block to write.
			 * @param location Reference to the data location. Will increase
			 *  the location to be after the written data block.
			 */
			bool write(const char *pData, unsigned int dataSize, unsigned int &location);

			/**
			 * Attemps to open/create a map file for saving with the supplied name.
			 * @param mapName Name of the map.
			 * @param mode Open modes for this file.
			 * @param overwrite True if any current map with the same name should be overwritten.
			 * @return Returns true if successful.
			 */
			bool openMapFileSave(const char *mapName, std::ios_base::open_mode mode, bool overwrite);

			/**
			 * Attemps to open/create a map file for loading with the supplied name.
			 * @param mapName Name of the map.
			 * @param mode Open modes for this file.
			 * @return Returns true if successful.
			 */
			bool openMapFileLoad(const char *mapName, std::ios_base::open_mode mode);

			/**
			 * Closes the map file.
			 */
			void closeMapFile();

		private:
			std::fstream mMapFile;
	};
}

#endif