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

#ifndef GUI_TEXT_H
#define GUI_TEXT_H
#include "GUIFrame.h"

namespace gfx
{
	/**
	 * Basic GUI component for displaying text strings.
	 * Contains an array of InstanceSprites that render the
	 * letters.
	 * The font should be stored in a Texture2DArray since
	 * manipulation of texture coordinates isn't available yet.
	 */
	class GUIText : public GUIFrame
	{
		public:
			enum TextFormat
			{
				TextFormat_ScaledLine = 1,
				TextFormat_ScaledRows,
				TextFormat_FixedLine,
				TextFormat_FixedRows
			};

			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to the instance base of a sprite.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 * @param parentDepth Depth level of the parent.
			 */
			GUIText(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

			/**
			 * Subframe constructor.
			 * @param pParent Pointer to the parent.
			 */
			GUIText(GUIFrame *pParent);

			/**
			 * Default copy constructor.
			 */
			GUIText(const GUIText &frame);

			/**
			 * Default destructor.
			 */
			virtual ~GUIText();

			/**
			 * Default assignment operator.
			 */
			const GUIText &operator=(const GUIText &frame);

			/**
			 * Updates the GUI frame and all sub frames.
			 * @param deltaTime Time difference since the last frame.
			 */
			virtual void update(float deltaTime);

			/**
			 * Method that is called when the mouse pointer enters
			 * the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseEnter(long mX, long mY);

			/**
			 * Method that is called when the mouse pointer exits
			 * the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseExit(long mX, long mY);

			/**
			 * Method that is called when the mouse pointer moves within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMove(long mX, long mY);

			/**
			 * Method that is called when the left mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseLeftDown(long mX, long mY);

			/**
			 * Method that is called when the middle mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMiddleDown(long mX, long mY);

			/**
			 * Method that is called when the right mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseRightDown(long mX, long mY);

			/**
			 * Method that is called when the left mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseLeftUp(long mX, long mY);

			/**
			 * Method that is called when the middle mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMiddleUp(long mX, long mY);

			/**
			 * Method that is called when the right mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseRightUp(long mX, long mY);

			/**
			 * Method that is called when a keyboard key is pressed.
			 * @param letterKeys Integer containing letter key status information.
			 * @param keys Integer containing key status information.
			 */
			virtual void onKeyDown(int letterKeys, int keys);

			/**
			 * Adds characters to the text string based on
			 *  the pressed keys found in the parameters.
			 * @param letterKeys Integer containing the status of letter keys.
			 * @param keys Integer containing the status of number and special keys.
			 */
			virtual void addCharFromKey(int letterKeys, int keys);

			/**
			 * Sets the render flag for the frame and all sub frames.
			 * @param render New value for the render flag.
			 */
			virtual void setFlagRender(bool render);

			/**
			 * Sets the current text string.
			 * @param text Const reference to a string.
			 */
			virtual void setText(const std::string &text);

			/**
			 * Sets the current text string.
			 * @param text Const char array to a string.
			 */
			virtual void setText(const char *text);

			/**
			 * Sets the current text format.
			 * @param format New text format.
			 */
			virtual void setTextFormat(TextFormat format);

			/**
			 * Sets the current text size.
			 * @param size New text size.
			 */
			virtual void setTextSize(const D3DXVECTOR2 &size);

			/**
			 * Sets the current space between characters in scaled text formats.
			 * @param space New text space.
			 */
			virtual void setTextSpaceRelative(const D3DXVECTOR2 &space);

			/**
			 * Sets the current space between characters in fixed text formats.
			 * @param space New text space.
			 */
			virtual void setTextSpaceAbsolute(const D3DXVECTOR2 &space);

			/**
			* Set the row that that will represent the first line in the textfield.
			* OBS! This is only relevant for the TextFormat_FixedRows.
			* @param rowOffset is the offset number.
			*/
			virtual void setRowOffset(int rowOffset);

			/**
			* Set the number of rows to show.
			* OBS! This is only relevant for the TextFormat_FixedRows.
			* @param nRows is the number of rows to show.
			*/
			virtual void showNumberOfRows(unsigned int nRows);

			/**
			* Get the nuber of rows shown.
			* OBS! This is only relevant for the TextFormat_FixedRows.
			* @return the number of rows.
			*/
			unsigned int getNumberOfRowsShown()const;

			/**
			* Get the total number of rows.
			* OBS! This is only relevant for the TextFormat_FixedRows.
			* @return the number of rows.
			*/
			unsigned int getTotalNumberOfRows()const;

			/**
			 * Retrieves the current text string as a const char array.
			 * @return Returns mText as const char array.
			 */
			inline const char *getText() const {return mText.c_str();}

			/**
			 * Retrieves the current text as a string.
			 * @return Returns mText.
			 */
			inline const std::string &getTextString() const {return mText;}

			/**
			 * Retrieves the current text size.
			 * @return Returns mTextSize.
			 */
			inline const D3DXVECTOR2 &getTextSize() const {return mTextSize;}

			/**
			 * Retrieves the space between characters in scaled text formats.
			 * @return Returns mTextSpaceRelative.
			 */
			inline const D3DXVECTOR2 &getTextSpaceRelative() const {return mTextSpaceRelative;}

			/**
			 * Retrieves the space between characters in fixed text formats.
			 * @return Returns mTextSpaceAbsolute.
			 */
			inline const D3DXVECTOR2 &getTextSpaceAbsolute() const {return mTextSpaceAbsolute;} 

		protected:
			/**
			 * Initializes the frame.
			 */
			virtual void init();

			/**
			 * Sends/receives a notification.
			 * @param message Message to send/receive.
			 * @param pSender Pointer to the sender.
			 */
			virtual void notify(int message, GUIFrame *pSender);

			/**
			 * Updates position and size for the text sprites.
			 */
			virtual void updateText();

			/**
			 * Updates position and size for the text sprites.
			 * Uses scaled line text format.
			 */
			virtual void formatTextScaledLine();

			/**
			 * Updates position and size for the text sprites.
			 * Uses fixed line text format.
			 */
			virtual void formatTextFixedLine();

			/**
			 * Updates position and size for the text sprites.
			 * Uses scaled rows text format.
			 */
			virtual void formatTextScaledRows();

			/**
			 * Updates position and size for the text sprites.
			 * Uses fixed rows text format.
			 */
			virtual void formatTextFixedRows();

			/**
			 * Calculates the position and scale for a character render instance.
			 * Uses scaled line text format.
			 * @param charIndex Index of the character render instance.
			 */
			virtual void calculateCharDimensionsScaledLine(unsigned int charIndex);

			/**
			 * Calculates the position and scale for a character render instance.
			 * Uses fixed line text format.
			 * @param charIndex Index of the character render instance.
			 */
			virtual void calculateCharDimensionsFixedLine(unsigned int charIndex);

			/**
			 * Calculates the position and scale for a character render instance.
			 * Uses scaled rows text format.
			 * @param charIndex Index of the character render instance.
			 */
			virtual void calculateCharDimensionsScaledRows(unsigned int charIndex);

			/**
			 * Calculates the position and scale for a character render instance.
			 * Uses fixed rows text format.
			 * @param charIndex Index of the character render instance.
			 */
			virtual void calculateCharDimensionsFixedRows(unsigned int charIndex);

			/**
			 * Converts a character to alphabetical index.
			 * @param c Character to convert.
			 * @return Returns the alphabetical index of the character.
			 */
			virtual int charToIndex(char c);

			/**
			 * Retrieves the index of the last character in
			 *  the word that contains the provided character.
			 * @param charIndex Index of the character to check.
			 * @return Returns the index of the word's last character.
			 */
			virtual int findWordEnd(unsigned int charIndex);

		private:
			unsigned int mCharCount;
			unsigned int mCharCountMax;
			unsigned int mSkippedChars;
			unsigned int mMaxRows;
			unsigned int mRowOffset;
			unsigned int mTotalNumberOfRows;
			InstanceSprite **mpRenderInstanceArray;
			std::string mText;
			TextFormat mTextFormat;
			D3DXVECTOR2 mTextSize;
			D3DXVECTOR2 mTextSpaceRelative;
			D3DXVECTOR2 mTextSpaceAbsolute;
			bool mFlagRender;
	};
}

#endif