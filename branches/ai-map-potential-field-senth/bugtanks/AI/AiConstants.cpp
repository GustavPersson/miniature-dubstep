/**
* @file
* @author Matteus Magnusson <senth.wallace@gmail.com>
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

#include "AiConstants.h"
#include "../Utilities/Constants.h"
#include "../Utilities/CustomGetPrivateProfile.h"
#include <windows.h>
#include <shlobj.h>

using namespace ai;

const char* FILE_PATH = "../AiConstants.ini";

// Potential field
int AiConstants::POTENTIAL_BUILDING_RANGE = 7;
float AiConstants::POTENTIAL_BUILDING_START_VALUE = 5.0f;
int AiConstants::POTENTIAL_WIDTH_AGENT = 19;
int AiConstants::POTENTIAL_WIDTH_PLAYER = 201;

// AgentAi
float AiConstants::GOAL_CLOSE_RANGE = 3.0f;
int AiConstants::PRIORITIZE_LESS_THAN_VALID_GOALS = 6;
float AiConstants::AGENT_SEARCH_TARGET_TIME = 3.0f;

// RangedAi
float AiConstants::RANGED_AI_TOO_CLOSE_OFFSET = 20.0f;

// PotentialUnit
// 2.5 degrees from the original value, i.e. 5.0 difference between each state
float AiConstants::ROTATED_THRESHOLD = utilities::math::degToRad(2.5f); 

// Update
float AiConstants::UPDATE_TIME_AGENT = 0.25f;
float AiConstants::UPDATE_TIME_PLAYER = 0.25f;
float AiConstants::UPDATE_TIME_BUILDING = 5.0f;

// Map potential values
float AiConstants::MAP_POTENTIAL_MIN_DRAW_VALUE = -15.0f;
float AiConstants::MAP_POTENTIAL_MAX_DRAW_VALUE = 15.0f;

// Draw height
const float AiConstants::DRAW_HEIGHT_AGENT = 0.6f;
const float AiConstants::DRAW_HEIGHT_MAP = 0.4f;
const float AiConstants::DRAW_HEIGHT_ORIGIN = 1.0f;
const float AiConstants::DRAW_HEIGHT_PLAYER = 0.5f;
const float AiConstants::DRAW_HEIGHT_UNIT_SIZE = 0.7f;

void AiConstants::loadConstants()
{
	//---------------------------
	//	POTENTIAL FIELDS
	//---------------------------
	POTENTIAL_BUILDING_RANGE = GetPrivateProfileInt(
		"POTENTIAL_FIELD",
		"Potential_Building_Range",
		POTENTIAL_BUILDING_RANGE,
		FILE_PATH
	);

	POTENTIAL_BUILDING_START_VALUE = GetPrivateProfileFloat(
		"POTENTIAL_FIELD",
		"Potential_Building_Start_Value",
		POTENTIAL_BUILDING_START_VALUE,
		FILE_PATH
	);

	POTENTIAL_WIDTH_AGENT = GetPrivateProfileInt(
		"POTENTIAL_FIELD",
		"Potential_Width_Agent",
		POTENTIAL_WIDTH_AGENT,
		FILE_PATH
	);

	POTENTIAL_WIDTH_PLAYER = GetPrivateProfileInt(
		"POTENTIAL_FIELD",
		"Potential_Width_Player",
		POTENTIAL_WIDTH_PLAYER,
		FILE_PATH
	);

	//---------------------------
	//	POTENTIAL_UNIT
	//---------------------------
	GOAL_CLOSE_RANGE = utilities::math::degToRad(GetPrivateProfileFloat(
		"POTENTIAL_UNIT",
		"Rotated_Threshold",
		 utilities::math::radToDeg(ROTATED_THRESHOLD),
		FILE_PATH
	));

	//---------------------------
	//	AGENT
	//---------------------------
	GOAL_CLOSE_RANGE = GetPrivateProfileFloat(
		"AGENT",
		"Goal_Close_Range",
		GOAL_CLOSE_RANGE,
		FILE_PATH
	);

	PRIORITIZE_LESS_THAN_VALID_GOALS = GetPrivateProfileInt(
		"AGENT",
		"Prioritize_Less_Than_Valid_Goals",
		PRIORITIZE_LESS_THAN_VALID_GOALS,
		FILE_PATH
	);

	AGENT_SEARCH_TARGET_TIME = GetPrivateProfileFloat(
		"AGENT",
		"Search_Target_Time",
		AGENT_SEARCH_TARGET_TIME,
		FILE_PATH
	);

	//---------------------------
	//	RANGED AI
	//---------------------------
	RANGED_AI_TOO_CLOSE_OFFSET = GetPrivateProfileFloat(
		"RANGED_AI",
		"Too_Close_Offset",
		RANGED_AI_TOO_CLOSE_OFFSET,
		FILE_PATH
	);

	//---------------------------
	//	UPDATE TIME
	//---------------------------
	UPDATE_TIME_AGENT = GetPrivateProfileFloat(
		"UPDATE",
		"Update_Time_Agent",
		UPDATE_TIME_AGENT,
		FILE_PATH
	);

	UPDATE_TIME_PLAYER = GetPrivateProfileFloat(
		"UPDATE",
		"Update_Time_Player",
		UPDATE_TIME_PLAYER,
		FILE_PATH
	);

	UPDATE_TIME_BUILDING = GetPrivateProfileFloat(
		"UPDATE",
		"Update_Time_Building",
		UPDATE_TIME_BUILDING,
		FILE_PATH
	);

	//---------------------------
	//	MAP POTENTIAL VALUES
	//---------------------------
	MAP_POTENTIAL_MIN_DRAW_VALUE = GetPrivateProfileFloat(
		"MAP_POTENTIAL_VALUE",
		"Map_Potential_Min_Draw_Value",
		MAP_POTENTIAL_MIN_DRAW_VALUE,
		FILE_PATH
	);

	MAP_POTENTIAL_MAX_DRAW_VALUE = GetPrivateProfileFloat(
		"MAP_POTENTIAL_VALUE",
		"Map_Potential_Max_Draw_Value",
		MAP_POTENTIAL_MAX_DRAW_VALUE,
		FILE_PATH
	);
}

void AiConstants::saveConstants()
{
	//---------------------------
	//	POTENTIAL FIELDS
	//---------------------------
	WritePrivateProfileInt(
		"POTENTIAL_FIELD",
		"Potential_Building_Range",
		POTENTIAL_BUILDING_RANGE,
		FILE_PATH
	);

	WritePrivateProfileFloat(
		"POTENTIAL_FIELD",
		"Potential_Building_Start_Value",
		POTENTIAL_BUILDING_START_VALUE,
		FILE_PATH
	);

	WritePrivateProfileInt(
		"POTENTIAL_FIELD",
		"Potential_Width_Agent",
		POTENTIAL_WIDTH_AGENT,
		FILE_PATH
	);

	WritePrivateProfileInt(
		"POTENTIAL_FIELD",
		"Potential_Width_Player",
		POTENTIAL_WIDTH_PLAYER,
		FILE_PATH
	);

	//---------------------------
	//	POTENTIAL_UNIT
	//---------------------------
	WritePrivateProfileFloat(
		"POTENTIAL_UNIT",
		"Rotated_Threshold",
		 utilities::math::radToDeg(ROTATED_THRESHOLD),
		FILE_PATH
	);

	//---------------------------
	//	AGENT
	//---------------------------
	WritePrivateProfileFloat(
		"AGENT",
		"Goal_Close_Range",
		GOAL_CLOSE_RANGE,
		FILE_PATH
	);

	WritePrivateProfileInt(
		"AGENT",
		"Prioritize_Less_Than_Valid_Goals",
		PRIORITIZE_LESS_THAN_VALID_GOALS,
		FILE_PATH
	);
	
	WritePrivateProfileFloat(
		"AGENT",
		"Search_Target_Time",
		AGENT_SEARCH_TARGET_TIME,
		FILE_PATH
	);

	//---------------------------
	//	RANGED AI
	//---------------------------
	WritePrivateProfileFloat(
		"RANGED_AI",
		"Too_Close_Offset",
		RANGED_AI_TOO_CLOSE_OFFSET,
		FILE_PATH
	);

	//---------------------------
	//	UPDATE TIME
	//---------------------------
	WritePrivateProfileFloat(
		"UPDATE",
		"Update_Time_Building",
		UPDATE_TIME_BUILDING,
		FILE_PATH
	);

	WritePrivateProfileFloat(
		"UPDATE",
		"Update_Time_Agent",
		UPDATE_TIME_AGENT,
		FILE_PATH
	);

	WritePrivateProfileFloat(
		"UPDATE",
		"Update_Time_Player",
		UPDATE_TIME_PLAYER,
		FILE_PATH
	);
}

