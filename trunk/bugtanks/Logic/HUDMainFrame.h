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

#ifndef HUD_MAIN_FRAME_H
#define HUD_MAIN_FRAME_H
#include "../GFX/GUIImage.h"
#include "Tank.h"

#define HUD_HP_POS_X 0.072f
#define HUD_HP_POS_Y 0.254f
#define HUD_HP_SIZE_X 0.192f
#define HUD_HP_SIZE_Y 0.684f

#define HUD_COOLDOWN_POS_X 0.301f
#define HUD_COOLDOWN_POS_Y 0.661f
#define HUD_COOLDOWN_SIZE_X 0.391f
#define HUD_COOLDOWN_SIZE_Y 0.285f

#define HUD_SELECTION_COOLDOWN_POS_X 0.713f
#define HUD_SELECTION_COOLDOWN_POS_Y 0.758f
#define HUD_SELECTION_COOLDOWN_SIZE_X 0.1f
#define HUD_SELECTION_COOLDOWN_SIZE_Y 0.185f

#define HUD_WEAPON_SIZE_X 0.16f
#define HUD_WEAPON_SIZE_Y 0.105f
#define HUD_WEAPON_POS_Y 0.531f
#define HUD_WEAPON_POS_X_1 0.307f
#define HUD_WEAPON_POS_X_2 0.49f
#define HUD_WEAPON_POS_X_3 0.668f

namespace logic
{
	class HUDMainFrame : public gfx::GUIImage
	{
		public:
			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to the instance base of a sprite.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 * @param parentDepth Depth level of the parent.
			 */
			HUDMainFrame(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

			/**
			 * Subframe constructor.
			 * @param pParent Pointer to the parent.
			 */
			HUDMainFrame(gfx::GUIFrame *pParent);

			/**
			 * Default copy constructor.
			 */
			HUDMainFrame(const HUDMainFrame &frame);

			/**
			 * Default destructor.
			 */
			virtual ~HUDMainFrame();

			/**
			 * Default assignment operator.
			 */
			const HUDMainFrame &operator=(const HUDMainFrame &frame);

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

			virtual void updateHPBar(float HPPercentage);
			virtual void updateCooldownBar(float cooldownPercentage);
			virtual void updateSelectionCooldownBar(float cooldownPercentage);
			virtual void updateWeapons(Tank::TankTypes tankType, Weapon::WeaponTypes weaponType);

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
			virtual void notify(int message, gfx::GUIFrame *pSender);

		private:
			gfx::GUIImage *mpHPBar;
			gfx::GUIImage *mpCooldownBar;
			gfx::GUIImage *mpSelectionCooldownBar;
			gfx::GUIImage *mpMainWeapon;
			gfx::GUIImage *mpWeapon1;
			gfx::GUIImage *mpWeapon2;
			gfx::GUIImage *mpWeapon3;
	};
}

#endif