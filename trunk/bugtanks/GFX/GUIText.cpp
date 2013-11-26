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

#include "GUIText.h"

using namespace gfx;

GUIText::GUIText(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIFrame(pBaseInstance, clientWidth, clientHeight, parentDepth), mText(""), mTextSize(5.0f, 10.0f),
	mTextSpaceRelative(0.001f, 0.001f), mTextSpaceAbsolute(1.0f, 1.0f)
{
	setFrameType(GUIFrame::FrameTypes_Text);
	mCharCount = 0;
	mCharCountMax = 50;
	mSkippedChars = 0;
	mMaxRows = 10;
	mRowOffset = 0;
	mTextFormat = GUIText::TextFormat_ScaledLine;
	mpRenderInstanceArray = myNew InstanceSprite *[mCharCountMax];
}

GUIText::GUIText(GUIFrame *pParent) : GUIFrame(pParent), mText(""), mTextSize(5.0f, 10.0f),
	mTextSpaceRelative(0.001f, 0.001f), mTextSpaceAbsolute(1.0f, 1.0f)
{
	setFrameType(GUIFrame::FrameTypes_SubText);
	mCharCount = 0;
	mCharCountMax = 50;
	mSkippedChars = 0;
	mMaxRows = 10;
	mRowOffset = 0;
	mTextFormat = GUIText::TextFormat_ScaledLine;
	mpRenderInstanceArray = myNew InstanceSprite *[mCharCountMax];
}

GUIText::GUIText(const GUIText &frame) : GUIFrame(frame), mText(frame.mText), mTextSize(frame.mTextSize),
	mTextSpaceRelative(frame.mTextSpaceRelative), mTextSpaceAbsolute(frame.mTextSpaceAbsolute)
{
	mCharCount = frame.mCharCount;
	mCharCountMax = frame.mCharCountMax;
	mSkippedChars = 0;
	mMaxRows = 10;
	mRowOffset = 0;
	mTextFormat = frame.mTextFormat;
	mpRenderInstanceArray = myNew InstanceSprite *[mCharCountMax];
}

GUIText::~GUIText()
{
	if(mpRenderInstanceArray)
	{
		delete [] mpRenderInstanceArray;
		mpRenderInstanceArray = NULL;
	}
}

const GUIText &GUIText::operator=(const GUIText &frame)
{
	if(this != &frame)
	{
		GUIFrame::operator=(frame);
		mText = frame.mText;
		mTextFormat = frame.mTextFormat;
		mTextSize = frame.mTextSize;
		mTextSpaceRelative = frame.mTextSpaceRelative;
		mTextSpaceAbsolute = frame.mTextSpaceAbsolute;
		mCharCount = frame.mCharCount;
		mCharCountMax = frame.mCharCountMax;
		mSkippedChars = 0;
	}
	return *this;
}

void GUIText::update(float deltaTime)
{
	GUIFrame::update(deltaTime);
	if(getFlagUpdateRequired())
	{
		mSkippedChars = 0;
		updateText();
	}
}

void GUIText::onMouseEnter(long mX, long mY)
{
	GUIFrame::onMouseEnter(mX, mY);
}

void GUIText::onMouseExit(long mX, long mY)
{
	GUIFrame::onMouseExit(mX, mY);
}

void GUIText::onMouseMove(long mX, long mY)
{
	GUIFrame::onMouseMove(mX, mY);
}

void GUIText::onMouseLeftDown(long mX, long mY)
{
	GUIFrame::onMouseLeftDown(mX, mY);
}

void GUIText::onMouseMiddleDown(long mX, long mY)
{
	GUIFrame::onMouseMiddleDown(mX, mY);
}

void GUIText::onMouseRightDown(long mX, long mY)
{
	GUIFrame::onMouseRightDown(mX, mY);
}

void GUIText::onMouseLeftUp(long mX, long mY)
{
	GUIFrame::onMouseLeftUp(mX, mY);
}

void GUIText::onMouseMiddleUp(long mX, long mY)
{
	GUIFrame::onMouseMiddleUp(mX, mY);
}

void GUIText::onMouseRightUp(long mX, long mY)
{
	GUIFrame::onMouseRightUp(mX, mY);
}

void GUIText::onKeyDown(int letterKeys, int keys)
{
	GUIFrame::onKeyDown(letterKeys, keys);
}

void GUIText::addCharFromKey(int letterKeys, int keys)
{
	if(letterKeys & logic::InputStates::LetterKey_A)
	{
		mText += "A";
	}
	if(letterKeys & logic::InputStates::LetterKey_B)
	{
		mText += "B";
	}
	if(letterKeys & logic::InputStates::LetterKey_C)
	{
		mText += "C";
	}
	if(letterKeys & logic::InputStates::LetterKey_D)
	{
		mText += "D";
	}
	if(letterKeys & logic::InputStates::LetterKey_E)
	{
		mText += "E";
	}
	if(letterKeys & logic::InputStates::LetterKey_F)
	{
		mText += "F";
	}
	if(letterKeys & logic::InputStates::LetterKey_G)
	{
		mText += "G";
	}
	if(letterKeys & logic::InputStates::LetterKey_H)
	{
		mText += "H";
	}
	if(letterKeys & logic::InputStates::LetterKey_I)
	{
		mText += "I";
	}
	if(letterKeys & logic::InputStates::LetterKey_J)
	{
		mText += "J";
	}
	if(letterKeys & logic::InputStates::LetterKey_K)
	{
		mText += "K";
	}
	if(letterKeys & logic::InputStates::LetterKey_L)
	{
		mText += "L";
	}
	if(letterKeys & logic::InputStates::LetterKey_M)
	{
		mText += "M";
	}
	if(letterKeys & logic::InputStates::LetterKey_N)
	{
		mText += "N";
	}
	if(letterKeys & logic::InputStates::LetterKey_O)
	{
		mText += "O";
	}
	if(letterKeys & logic::InputStates::LetterKey_P)
	{
		mText += "P";
	}
	if(letterKeys & logic::InputStates::LetterKey_Q)
	{
		mText += "Q";
	}
	if(letterKeys & logic::InputStates::LetterKey_R)
	{
		mText += "R";
	}
	if(letterKeys & logic::InputStates::LetterKey_S)
	{
		mText += "S";
	}
	if(letterKeys & logic::InputStates::LetterKey_T)
	{
		mText += "T";
	}
	if(letterKeys & logic::InputStates::LetterKey_U)
	{
		mText += "U";
	}
	if(letterKeys & logic::InputStates::LetterKey_V)
	{
		mText += "V";
	}
	if(letterKeys & logic::InputStates::LetterKey_W)
	{
		mText += "W";
	}
	if(letterKeys & logic::InputStates::LetterKey_X)
	{
		mText += "X";
	}
	if(letterKeys & logic::InputStates::LetterKey_Y)
	{
		mText += "Y";
	}
	if(letterKeys & logic::InputStates::LetterKey_Z)
	{
		mText += "Z";
	}
	if(keys & logic::InputStates::Key_0)
	{
		mText += "0";
	}
	if(keys & logic::InputStates::Key_1)
	{
		mText += "1";
	}
	if(keys & logic::InputStates::Key_2)
	{
		mText += "2";
	}
	if(keys & logic::InputStates::Key_3)
	{
		mText += "3";
	}
	if(keys & logic::InputStates::Key_4)
	{
		mText += "4";
	}
	if(keys & logic::InputStates::Key_5)
	{
		mText += "5";
	}
	if(keys & logic::InputStates::Key_6)
	{
		mText += "6";
	}
	if(keys & logic::InputStates::Key_7)
	{
		mText += "7";
	}
	if(keys & logic::InputStates::Key_8)
	{
		mText += "8";
	}
	if(keys & logic::InputStates::Key_9)
	{
		mText += "9";
	}
	if(keys & logic::InputStates::Key_Space)
	{
		mText += " ";
	}
	if(keys & logic::InputStates::Key_Return)
	{
		mText += "\n";
	}
	if(keys & logic::InputStates::Key_Backspace)
	{
		if(!mText.empty())
		{
			mText.erase(mText.end() - 1);
		}
	}
	updateRequired();
}

void GUIText::setFlagRender(bool render)
{
	GUIFrame::setFlagRender(render);
	mFlagRender = render;
	for(unsigned int n = 0; n < mCharCount; ++n)
	{
		mpRenderInstanceArray[n]->setFlagRender(render);
	}
	updateRequired();
}

void GUIText::setText(const std::string &text)
{
	mText = text;
	updateRequired();
}

void GUIText::setText(const char *text)
{
	mText = text;
	updateRequired();
}

void GUIText::setTextFormat(TextFormat format)
{
	mTextFormat = format;
	updateRequired();
}

void GUIText::setTextSize(const D3DXVECTOR2 &size)
{
	mTextSize = size;
	updateRequired();
}

void GUIText::setTextSpaceRelative(const D3DXVECTOR2 &space)
{
	mTextSpaceRelative = space;
	updateRequired();
}

void GUIText::setTextSpaceAbsolute(const D3DXVECTOR2 &space)
{
	mTextSpaceAbsolute = space;
	updateRequired();
}
void GUIText::setRowOffset(int rowOffset)
{
	if(rowOffset<0)
		this->mRowOffset=0;

	else if((unsigned int)rowOffset>this->getTotalNumberOfRows())
		this->mRowOffset=this->getTotalNumberOfRows();

	else
		this->mRowOffset=rowOffset;
	this->updateRequired();
}
void GUIText::showNumberOfRows(unsigned int nRows)
{
	this->mMaxRows = nRows;
	this->updateRequired();
}
unsigned int GUIText::getNumberOfRowsShown()const
{
	return this->mMaxRows;
}
unsigned int GUIText::getTotalNumberOfRows()const
{
	return this->mTotalNumberOfRows;
}
void GUIText::init()
{

}

void GUIText::notify(int message, GUIFrame *pSender)
{
	//Unrefer parameters.
	message;
	pSender;
}

void GUIText::updateText()
{
	if(mFlagRender)
	{

	//Increase array size if necessary.
	if(mText.size() > mCharCountMax)
	{
		mCharCountMax = mText.size();
		InstanceSprite **pNewRenderInstanceArray = myNew InstanceSprite *[mCharCountMax];
		for(unsigned int n = 0; n < mCharCount; ++n)
		{
			pNewRenderInstanceArray[n] = mpRenderInstanceArray[n];
		}
		if(mpRenderInstanceArray)
		{
			delete [] mpRenderInstanceArray;
			mpRenderInstanceArray = NULL;
		}
		mpRenderInstanceArray = pNewRenderInstanceArray;
	}

	//Create new render instances.
	if(mText.size() > mCharCount)
	{
		for(unsigned int n = mCharCount; n < mText.size(); ++n)
		{
			mpRenderInstanceArray[n] = (InstanceSprite *)getBaseInstance()->createInstanceSorted((float)getDepthLevel());
		}
	}
	//Hide the extra render instances.
	else if(mText.size() < mCharCount)
	{
		for(unsigned int n = mText.size(); n < mCharCount; ++n)
		{
			mpRenderInstanceArray[n]->setFlagRender(false);
		}
	}

	//Update character count.
	mCharCount = mText.size();

	switch(mTextFormat)
	{
		case GUIText::TextFormat_ScaledLine:
			formatTextScaledLine();
			break;

		case GUIText::TextFormat_FixedLine:
			formatTextFixedLine();
			break;

		case GUIText::TextFormat_ScaledRows:
			formatTextScaledRows();
			break;

		case GUIText::TextFormat_FixedRows:
			formatTextFixedRows();
			break;

		default:
			break;
	}
	}
}

void GUIText::formatTextScaledLine()
{
	//Update render instances.
	for(unsigned int n = 0; n < mCharCount; ++n)
	{
		mpRenderInstanceArray[n]->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		mpRenderInstanceArray[n]->setTextureArrayIndex(4);
		mpRenderInstanceArray[n]->setTextureID(charToIndex(mText[n]));
		calculateCharDimensionsScaledLine(n);
	}
}

void GUIText::formatTextFixedLine()
{
	//Update render instances.
	for(unsigned int n = 0; n < mCharCount; ++n)
	{
		mpRenderInstanceArray[n]->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		mpRenderInstanceArray[n]->setTextureArrayIndex(4);
		mpRenderInstanceArray[n]->setTextureID(charToIndex(mText[n]));
		calculateCharDimensionsFixedLine(n);
	}
}

void GUIText::formatTextScaledRows()
{
	//Update render instances.
	for(unsigned int n = 0; n < mCharCount; ++n)
	{
		mpRenderInstanceArray[n]->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		mpRenderInstanceArray[n]->setTextureArrayIndex(4);
		mpRenderInstanceArray[n]->setTextureID(charToIndex(mText[n]));
		calculateCharDimensionsScaledRows(n);
	}
}

void GUIText::formatTextFixedRows()
{
	//Update render instances.
	for(unsigned int n = 0; n < mCharCount; ++n)
	{
		mpRenderInstanceArray[n]->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		mpRenderInstanceArray[n]->setTextureArrayIndex(4);
		mpRenderInstanceArray[n]->setTextureID(charToIndex(mText[n]));
		calculateCharDimensionsFixedRows(n);
	}
	unsigned int charsPerRow = max(1, getAbsoluteSizeX() / (long)(mTextSize.x + mTextSpaceAbsolute.x));
	this->mTotalNumberOfRows = ((mCharCount + mSkippedChars) / charsPerRow);
}

void GUIText::calculateCharDimensionsScaledLine(unsigned int charIndex)
{
	InstanceSprite *pRenderInstance = mpRenderInstanceArray[charIndex];

	//Calculate relative size of characters.
	D3DXVECTOR2 relativeCharSize;
	relativeCharSize.x = 1.0f / ((float)(mCharCount + mTextSpaceRelative.x));
	relativeCharSize.y = 1.0f;
	
	//Calculate relative position of characters.
	D3DXVECTOR2 relativeCharPosition;
	relativeCharPosition.x = relativeCharSize.x * charIndex;
	relativeCharPosition.y = 0.0f;
	relativeCharSize.x -= mTextSpaceRelative.x;

	//Calculate absolute position and size of characters.
	long absoluteCharX = getAbsoluteX() + (long)(relativeCharPosition.x * getAbsoluteSizeX());
	long absoluteCharY = getAbsoluteY() + (long)(relativeCharPosition.y * getAbsoluteSizeY());
	long absoluteCharSizeX = (long)(relativeCharSize.x * getAbsoluteSizeX());
	long absoluteCharSizeY = (long)(relativeCharSize.y * getAbsoluteSizeY());

	D3DXVECTOR3 position;
	position.x = (float)absoluteCharX;
	position.y = (float)-absoluteCharY;
	position.z = pRenderInstance->getPositionZ();

	D3DXVECTOR3 scale;
	scale.x = (float)absoluteCharSizeX;
	scale.y = (float)absoluteCharSizeY;
	scale.z = 1.0f;

	pRenderInstance->setPosition(position);
	pRenderInstance->setScale(scale);
}

void GUIText::calculateCharDimensionsFixedLine(unsigned int charIndex)
{
	InstanceSprite *pRenderInstance = mpRenderInstanceArray[charIndex];

	//Calculate absolute position and size of characters.
	long absoluteCharX = getAbsoluteX() + (long)((mTextSize.x + mTextSpaceAbsolute.x) * charIndex);
	long absoluteCharY = getAbsoluteY();
	long absoluteCharSizeX = (long)mTextSize.x;
	long absoluteCharSizeY = (long)mTextSize.y;

	D3DXVECTOR3 position;
	position.x = (float)absoluteCharX;
	position.y = (float)-absoluteCharY;
	position.z = pRenderInstance->getPositionZ();

	D3DXVECTOR3 scale;
	scale.x = (float)absoluteCharSizeX;
	scale.y = (float)absoluteCharSizeY;
	scale.z = 1.0f;

	pRenderInstance->setPosition(position);
	pRenderInstance->setScale(scale);
}

void GUIText::calculateCharDimensionsScaledRows(unsigned int charIndex)
{
	charIndex;
}

void GUIText::calculateCharDimensionsFixedRows(unsigned int charIndex)
{
	InstanceSprite *pRenderInstance = mpRenderInstanceArray[charIndex];

	//Calculate current row.
	unsigned int charsPerRow = getAbsoluteSizeX() / (long)(mTextSize.x + mTextSpaceAbsolute.x);
	long row = ((charIndex + mSkippedChars) / charsPerRow);

	unsigned int rowIndex = charIndex - charsPerRow * row + mSkippedChars;

	//Find if the current word fits on the row.
	int wordEnd = findWordEnd(charIndex);

	//Newline character.
	if(wordEnd == -2)
	{
		//Add the skipped length of the previous row and the newline character
		//to mSkippedChars.
		mSkippedChars += charsPerRow - rowIndex - 1;

		//Move to next row.
		++row;
		rowIndex = 0;
	}
	//Word end found.
	else if(wordEnd > (int)charIndex)
	{
		unsigned int wordLength = wordEnd - charIndex;
		if(rowIndex + wordLength >= charsPerRow)
		{
			//Add the skipped length of the previous row to mSkippedChars.
			mSkippedChars += charsPerRow - rowIndex;

			//The word does not fit. Move to next row.
			++row;
			rowIndex = 0;
		}
	}

	row -= mRowOffset;

	int maxrows = getAbsoluteSizeY() / (int)((mTextSize.y + mTextSpaceAbsolute.y));
	if(row >= maxrows)
	{
		mpRenderInstanceArray[charIndex]->setFlagRender(false);
		return;
	}
	else if(row < 0)
	{
		mpRenderInstanceArray[charIndex]->setFlagRender(false);
		return;
	}

	mpRenderInstanceArray[charIndex]->setFlagRender(true);

	//Calculate absolute position and size of characters.
	long absoluteCharX = getAbsoluteX() + (long)((mTextSize.x + mTextSpaceAbsolute.x) * rowIndex);
	long absoluteCharY = getAbsoluteY() + (long)((mTextSize.y + mTextSpaceAbsolute.y) * row);
	long absoluteCharSizeX = (long)mTextSize.x;
	long absoluteCharSizeY = (long)mTextSize.y;

	D3DXVECTOR3 position;
	position.x = (float)absoluteCharX;
	position.y = (float)-absoluteCharY;
	position.z = pRenderInstance->getPositionZ();

	D3DXVECTOR3 scale;
	scale.x = (float)absoluteCharSizeX;
	scale.y = (float)absoluteCharSizeY;
	scale.z = 1.0f;

	pRenderInstance->setPosition(position);
	pRenderInstance->setScale(scale);
}

int GUIText::charToIndex(char c)
{
	//Upper case. ASCII 65-90.
	if(c >= 65 && c <= 90)
	{
		return c - 65;
	}
	//Lower case. ASCII 97-122.
	if(c >= 97 && c <= 122)
	{
		return c - 97;
	}
	//Numbers. ASCII 48-57.
	if(c >= 48 && c <= 57)
	{
		return c - 22;
	}
	//Special characters.
	//Dot. ASCII 46.
	if(c == 46)
	{
		return 36;
	}
	//Comma. ASCII 44.
	if(c == 44)
	{
		return 37;
	}
	//Return space index.
	return 38;
}

int GUIText::findWordEnd(unsigned int charIndex)
{
	//Check for newline character.
	if(mText[charIndex] == '\n')
	{
		return -2;
	}
	//Check for end of the word.
	for(unsigned int n = charIndex; n < mText.size(); ++n)
	{
		if(mText[n] == ' ' || mText[n] == '\n')
		{
			return n;
		}
		else if(n == mText.size() - 1)
		{
			return n;
		}
	}
	return -1;
}