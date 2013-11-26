/**
 * @file
 * @author Gustav Persson <ninjamuff@gmail.com>
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
 * 
 * @section DESCRIPTION
 *
 * Events.cpp is where you put events!
 */
#include "Events.h"

using namespace logic;

//System Events
const EventType EvtData_System_StartGame::mEventType("start_game");
const EventType EvtData_System_EndGame::mEventType("end_game");
const EventType EvtData_System_GameOver::mEventType("game_over");
const EventType EvtData_System_EventSystemStarted::mEventType("event_system_started");
const EventType EvtData_System_GFXSystemStarted::mEventType("GFX_system_started");
const EventType EvtData_System_PreSave::mEventType("pre_save");
const EventType EvtData_System_PostSave::mEventType("post_save");
//From-Events
const EventType EvtData_From_ActorCreated::mEventType("Created_actor");
const EventType EvtData_From_ActorDestroyed::mEventType("destroyed_actor");
const EventType EvtData_From_ActorCollision::mEventType("actor_collided");
const EventType EvtData_From_DriverDeath::mEventType("driver_death");
const EventType EvtData_From_DriverHit::mEventType("driver_hit");
const EventType EvtData_From_EnemyHit::mEventType("enemy_hit");
const EventType EvtData_From_WeaponFired::mEventType("Weapon_Fired");
//To-Events
const EventType EvtData_To_InputChangeGameState::mEventType("change_game_state");
const EventType EvtData_To_EnemyUpdateHp::mEventType("update_enemy_hp");
const EventType EvtData_To_TankStartFire::mEventType("tankStartFire");
const EventType EvtData_To_TankStopFire::mEventType("tankStopFire");
const EventType EvtData_To_DriverStartFire::mEventType("driverStartFire");
const EventType EvtData_To_DriverStopFire::mEventType("driverStopFire");
const EventType EvtData_To_ActorStartMove::mEventType("actorStartMove");
const EventType EvtData_To_ActorStopMove::mEventType("actorStopMove");
const EventType EvtData_To_ActorStartRotate::mEventType("actorStartRotate");
const EventType EvtData_To_ActorStopRotate::mEventType("actorStopRotate");

