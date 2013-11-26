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

#ifndef EDITOR_MESSAGES_H
#define EDITOR_MESSAGES_H
#include "../Utilities/Macros.h"
#include "../Logic/KeyType.h"
#include <string>
#include "EditorMapHandler.h"

namespace editor
{
	/**
	 * Base message class.
	 */
	class EM
	{
		public:
			enum MessageType
			{
				MessageType_Unknown = -1,
				MessageType_Activate = 1,
				MessageType_Deactivate,
				MessageType_ListSelectModel,
				MessageType_ListSelectEnemy,
				MessageType_CreateModel,
				MessageType_CreateEnemy,
				MessageType_RemoveMarker,
				MessageType_SaveMap,
				MessageType_LoadMap,
				MessageType_UpdateWorldPlane
			};

			EM()
			{
				mType = EM::MessageType_Unknown;
			}

			EM(const EM &message)
			{
				mType = message.mType;
			}

			virtual ~EM(){}

			const EM &operator=(const EM &message)
			{
				if(this != &message)
				{
					mType = message.mType;
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EM(*this);
			}

			inline int getType() const {return mType;}

		protected:
			inline void setType(int type){mType = type;}

		private:
			int mType;
	};

	/**
	 * Message that is sent when the editor should activate.
	 */
	class EMActivate : public EM
	{
		public:
			EMActivate() : EM()
			{
				setType(EM::MessageType_Activate);
			}

			EMActivate(const EMActivate &message) : EM(message)
			{

			}

			virtual ~EMActivate(){}

			const EMActivate &operator=(const EMActivate &message)
			{
				if(this != &message)
				{
					EM::operator=(message);
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EMActivate(*this);
			}

		protected:

		private:
	};

	/**
	 * Message that is sent when the editor should deactivate.
	 */
	class EMDeactivate : public EM
	{
		public:
			EMDeactivate() : EM()
			{
				setType(EM::MessageType_Deactivate);
			}

			EMDeactivate(const EMDeactivate &message) : EM(message)
			{

			}

			virtual ~EMDeactivate(){}

			const EMDeactivate &operator=(const EMDeactivate &message)
			{
				if(this != &message)
				{
					EM::operator=(message);
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EMDeactivate(*this);
			}

		protected:

		private:
	};

	/**
	 * Message that is sent when the user selects a model from the
	 *  universal list.
	 */
	class EMListSelectModel : public EM
	{
		public:
			EMListSelectModel(int ID, const char *name) : EM()
			{
				setType(EM::MessageType_ListSelectModel);
				mID = ID;
				mName = name;
			}

			EMListSelectModel(const EMListSelectModel &message) : EM(message)
			{
				setType(EM::MessageType_ListSelectModel);
				mID = message.mID;
				mName = message.mName;
			}

			virtual ~EMListSelectModel(){}

			const EMListSelectModel &operator=(const EMListSelectModel &message)
			{
				if(this != &message)
				{
					EM::operator=(message);
					mID = message.mID;
					mName = message.mName;
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EMListSelectModel(*this);
			}

			inline int getID() const {return mID;}
			inline const char *getName() const {return mName.c_str();}

		protected:

		private:
			int mID;
			std::string mName;
	};

	/**
	 * Message that is sent when the user selects an enemy actor from the
	 *  universal list.
	 */
	class EMListSelectEnemy : public EM
	{
		public:
			EMListSelectEnemy(int ID, const char *name) : EM()
			{
				setType(EM::MessageType_ListSelectEnemy);
				mID = ID;
				mName = name;
			}

			EMListSelectEnemy(const EMListSelectEnemy &message) : EM(message)
			{
				setType(EM::MessageType_ListSelectEnemy);
				mID = message.mID;
				mName = message.mName;
			}

			virtual ~EMListSelectEnemy(){}

			const EMListSelectEnemy &operator=(const EMListSelectEnemy &message)
			{
				if(this != &message)
				{
					EM::operator=(message);
					mID = message.mID;
					mName = message.mName;
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EMListSelectEnemy(*this);
			}

			inline int getID() const {return mID;}
			inline const char *getName() const {return mName.c_str();}

		protected:

		private:
			int mID;
			std::string mName;
	};

	/**
	 * Message that is sent when a model should be created.
	 */
	class EMCreateModel : public EM
	{
		public:
			EMCreateModel(int ID, const char *name) : EM()
			{
				setType(EM::MessageType_CreateModel);
				mID = ID;
				mName = name;
			}

			EMCreateModel(const EMCreateModel &message) : EM(message)
			{
				setType(EM::MessageType_CreateModel);
				mID = message.mID;
				mName = message.mName;
			}

			virtual ~EMCreateModel(){}

			const EMCreateModel &operator=(const EMCreateModel &message)
			{
				if(this != &message)
				{
					EM::operator=(message);
					mID = message.mID;
					mName = message.mName;
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EMCreateModel(*this);
			}

			inline int getID() const {return mID;}
			inline const char *getName() const {return mName.c_str();}

		protected:

		private:
			int mID;
			std::string mName;
	};

	/**
	 * Message that is sent when an enemy actor should be created.
	 */
	class EMCreateEnemy : public EM
	{
		public:
			EMCreateEnemy(int ID, const char *name) : EM()
			{
				setType(EM::MessageType_CreateEnemy);
				mID = ID;
				mName = name;
			}

			EMCreateEnemy(const EMCreateEnemy &message) : EM(message)
			{
				mID = message.mID;
				mName = message.mName;
			}

			virtual ~EMCreateEnemy(){}

			const EMCreateEnemy &operator=(const EMCreateEnemy &message)
			{
				if(this != &message)
				{
					EM::operator=(message);
					mID = message.mID;
					mName = message.mName;
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EMCreateEnemy(*this);
			}

			inline int getID() const {return mID;}
			inline const char *getName() const {return mName.c_str();}

		protected:

		private:
			int mID;
			std::string mName;
	};

	/**
	 * Message that is sent when the mouse marker should be
	 * cleared and any temporary cursor objects should be deleted.
	 */
	class EMRemoveMarker : public EM
	{
		public:
			EMRemoveMarker() : EM()
			{
				setType(EM::MessageType_RemoveMarker);
			}

			EMRemoveMarker(const EMRemoveMarker &message) : EM(message)
			{

			}

			virtual ~EMRemoveMarker(){}

			const EMRemoveMarker &operator=(const EMRemoveMarker &message)
			{
				if(this != &message)
				{
					EM::operator=(message);
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EMRemoveMarker(*this);
			}

		protected:

		private:
	};

	/**
	 * Message that is sent when the current map should be saved.
	 */
	class EMSaveMap : public EM
	{
		public:
			EMSaveMap(const char *mapName, bool overwrite) : EM()
			{
				setType(EM::MessageType_SaveMap);
				mMapName = mapName;
				mOverwrite = overwrite;
			}

			EMSaveMap(const EMSaveMap &message) : EM(message)
			{
				mMapName = message.mMapName;
				mOverwrite = message.mOverwrite;
			}

			virtual ~EMSaveMap(){}

			const EMSaveMap &operator=(const EMSaveMap &message)
			{
				if(this != &message)
				{
					EM::operator=(message);
					mMapName = message.mMapName;
					mOverwrite = message.mOverwrite;
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EMSaveMap(*this);
			}

			inline const std::string &getMapName() const {return mMapName;}
			inline bool getOverwrite() const {return mOverwrite;}

		protected:

		private:
			std::string mMapName;
			bool mOverwrite;
	};

	/**
	 * Message that is sent when a map should be loaded.
	 */
	class EMLoadMap : public EM
	{
		public:
			EMLoadMap(const char *mapName) : EM()
			{
				setType(EM::MessageType_LoadMap);
				mMapName = mapName;
			}

			EMLoadMap(const EMLoadMap &message) : EM(message)
			{
				mMapName = message.mMapName;
			}

			virtual ~EMLoadMap(){}

			const EMLoadMap &operator=(const EMLoadMap &message)
			{
				if(this != &message)
				{
					EM::operator=(message);
					mMapName = message.mMapName;
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EMLoadMap(*this);
			}

			inline const std::string &getMapName() const {return mMapName;}

		protected:

		private:
			std::string mMapName;
	};

	/**
	 * Message that is sent when a map has been loaded over the network which causes the world plane to be updated.
	 */
	class EMUpdateWorldPlane : public EM
	{
		public:
			EMUpdateWorldPlane() : EM()
			{
				setType(EM::MessageType_UpdateWorldPlane);
			}

			EMUpdateWorldPlane(const EMUpdateWorldPlane &message) : EM(message)
			{

			}

			virtual ~EMUpdateWorldPlane(){}

			const EMUpdateWorldPlane &operator=(const EMUpdateWorldPlane &message)
			{
				if(this != &message)
				{
					EM::operator=(message);
				}
				return *this;
			}

			virtual EM *clone()
			{
				return myNew EMUpdateWorldPlane(*this);
			}

		protected:

		private:
	};
}

#endif