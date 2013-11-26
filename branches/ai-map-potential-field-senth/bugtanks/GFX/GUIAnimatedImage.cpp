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

#include "GUIAnimatedImage.h"

using namespace gfx;

GUIAnimatedImage::GUIAnimatedImage(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
		: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth), mAnimationFrameList(), mAnimationIter()
{
	mTimer = 0.0f;
	mAnimationLength = 0.0f;
	init();
}

GUIAnimatedImage::GUIAnimatedImage(GUIFrame *pParent) : GUIImage(pParent), mAnimationFrameList(), mAnimationIter()
{
	mTimer = 0.0f;
	mAnimationLength = 0.0f;
	init();
}

GUIAnimatedImage::GUIAnimatedImage(const GUIAnimatedImage &frame) : GUIImage(frame), mAnimationFrameList(), mAnimationIter()
{
	mTimer = frame.mTimer;
	mAnimationLength = frame.mAnimationLength;
	init();
	//TO DO: copy animation frame list.
}

GUIAnimatedImage::~GUIAnimatedImage()
{

}

const GUIAnimatedImage &GUIAnimatedImage::operator=(const GUIAnimatedImage &frame)
{
	if(this != &frame)
	{
		GUIImage::operator=(frame);
		mTimer = frame.mTimer;
		mAnimationLength = frame.mAnimationLength;
		//TO DO: copy animation frame list.
	}
	return *this;
}

void GUIAnimatedImage::update(float deltaTime)
{
	GUIImage::update(deltaTime);
	updateAnimation(deltaTime);
}

void GUIAnimatedImage::init()
{
	//Add some test frames.
	AnimationFrame frame;
	frame.mTime = 0.25f;
	frame.mTextureID = 0;
	frame.mColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	addAnimation(frame);

	frame.mTime = 0.25f;
	frame.mTextureID = 1;
	frame.mColor = D3DXCOLOR(0.25f, 0.0f, 0.0f, 1.0f);
	addAnimation(frame);

	frame.mTime = 0.25f;
	frame.mTextureID = 2;
	frame.mColor = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
	addAnimation(frame);

	frame.mTime = 0.25f;
	frame.mTextureID = 3;
	frame.mColor = D3DXCOLOR(0.75f, 0.0f, 0.0f, 1.0f);
	addAnimation(frame);

	frame.mTime = 0.25f;
	frame.mTextureID = 4;
	frame.mColor = D3DXCOLOR(0.9f, 0.0f, 0.0f, 1.0f);
	addAnimation(frame);
}

void GUIAnimatedImage::notify(int message, GUIFrame *pSender)
{
	//Unref parameters.
	message;
	pSender;
}

void GUIAnimatedImage::addAnimation(const AnimationFrame &frame)
{
	//Add the frame to the vector.
	mAnimationFrameList.push_back(frame);

	//Reset the iterator.
	mAnimationIter = mAnimationFrameList.begin();

	//Increase the animation length.
	mAnimationLength += frame.mTime;
}

void GUIAnimatedImage::updateAnimation(float deltaTime)
{
	//Update timer.
	mTimer += deltaTime;

	//Increase frame.
	if(mTimer > (*mAnimationIter).mTime)
	{
		++mAnimationIter;
		if(mAnimationIter == mAnimationFrameList.end())
		{
			mAnimationIter = mAnimationFrameList.begin();
		}
		mTimer = 0.0f;

		//Set render instance variables.
		getRenderInstance()->setTextureID((*mAnimationIter).mTextureID);
		getRenderInstance()->setColor((*mAnimationIter).mColor);
	}
}