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

#ifndef GUI_FRAME_H
#define GUI_FRAME_H
#include "../Logic/IEventManager.h"
#include <vector>

using namespace logic;

namespace gfx
{
	/**
	 * Base class for all GUI components.
	 */
	class TempSprite
	{
		public:
			TempSprite(){}
			TempSprite(const TempSprite &sprite){}
			virtual ~TempSprite(){}
			const TempSprite &operator=(const TempSprite &sprite){if(this != &sprite){} return *this;}
	};

	namespace FrameTypes
	{
		enum Types
		{
			FRAME = 1,
			ANIMATED_FRAME = 2,
			BUTTON = 3,
			ANIMATED_BUTTON = 4,
			LIST = 5,
			SCROLL_LIST = 6
		};
	}
	
	class GUIFrame : public TempSprite
	{
		public:
			/**
			 * Default constructor.
			 */
			GUIFrame();

			/**
			 * Default copy constructor.
			 */
			GUIFrame(const GUIFrame &frame);

			/**
			 * Default destructor.
			 */
			virtual ~GUIFrame();

			/**
			 * Default assignment operator.
			 */
			const GUIFrame &operator=(const GUIFrame &frame);

			/**
			 * Method that is called when the mouse pointer enters
			 * the frame.
			 */
			virtual void onMouseEnter();

			/**
			 * Method that is called when the mouse pointer exits
			 * the frame.
			 */
			virtual void onMouseExit();

			/**
			 * Method that is called when the mouse left clicks
			 * within the frame.
			 */
			virtual void onMouseLeftClick();

			/**
			 * Method that is called when the mouse right clicks
			 * within the frame.
			 */
			virtual void onMouseRightClick();

			/**
			 * Adds all sub frames to another vector.
			 * @param frameList Vector to add the sub frames to.
			 */
			virtual void addSubFrames(std::vector<GUIFrame *> frameList) const;

			/**
			 * Adds all visible sub frames to another vector.
			 * @param frameList Vector to add the sub frames to.
			 */
			virtual void addVisibleSubFrames(std::vector<GUIFrame *> frameList) const;

			/**
			 * Resizes the frame and all sub frames to fit the supplied size.
			 * @param sizeX New size along the X axis.
			 * @param sizeY New size along the Y axis.
			 */
			virtual void resizeFrameAbsolute(float sizeX, float sizeY);

			/**
			 * Resizes the frame and all sub frames according to a supplied factor.
			 * @param factorX Factor for the scaling along the X axis.
			 * @param factorY Factor for the scaling along the Y axis.
			 */
			virtual void resizeFrameFactor(float factorX, float factorY);

			/**
			 * Retrieves the sub frame list.
			 */
			inline const std::vector<GUIFrame *> &getSubFrameList() const {return mSubFrameList;}

			/**
			 * Retrieves the frame type.
			 */
			inline const int getFrameType() const {return mFrameType;}

			/**
			 * Retrieves the frame ID.
			 */
			inline const int getFrameID() const {return mFrameID;}

			/**
			 * Retrieves the frame name.
			 */
			inline const char *getFrameName() const {return mFrameName;}

			/**
			 * Retrieves the visibility flag.
			 */
			inline const bool isVisible() const {return mFlagVisible;}

			/**
			 * Sets the frame ID.
			 */
			inline void setFrameID(int frameID){mFrameID = frameID;}

			/**
			 * Sets the frame name.
			 */
			inline void setFrameName(const char *frameName){mFrameName = frameName;}

			/**
			 * Sets the visibility flag.
			 */
			inline void setFlagVisible(bool flagVisible){mFlagVisible = flagVisible;}

		protected:
			std::vector<GUIFrame *> mSubFrameList;
			int mFrameType;
			int mFrameID;
			const char *mFrameName;
			bool mFlagVisible;

			virtual void removeAllSubFrames();

		private:
	};
}

#endif