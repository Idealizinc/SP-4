#pragma once

/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 18 February 2017 12:32 AM
File Name     : LuaInterface.h
Purpose       : Defines a LuaInterface that handles script based utilities
*/

// includes the lua headers
#include "../Internal/SingletonTemplate.h"
#include "../../Engine/System/RenderSystem.h"
#include <lua.hpp>
#include <string>
#include <map>
#include "Vector3.h"

class LuaInterface : public SingletonTemplate<LuaInterface>
{
public:
	LuaInterface();
	virtual ~LuaInterface();
	// Initialisation of the Lua Interface Class
	bool Init();
	// Run the Lua Interface Class
	void Run();
	// Drop the Lua Interface Class
	void Drop();
	// Getter for the LuaState
	lua_State* GetLuaState();

	// Lua based getters
	int GetIntegerValue(const std::string& VariableName);
	float GetFloatValue(const std::string& VariableName);
	std::string GetStringValue(const std::string& VariableName);
	bool GetBoolValue(const std::string& VariableName);
	Vector3 GetVector3Value(const std::string& VariableName);
	
	// Vector Utility
	// Get distance squared between two vectors
	float GetVector3DistanceSquared(Vector3 Direction);
	float GetVector3DistanceBetweenSquared(Vector3 Start, Vector3 End);
	float GetVector3Dot(Vector3 A, Vector3 B);

	void SaveIntegerValue(const char* VariableName, const int Value, const bool Overwrite);

	// Check the container for existing script
	lua_State* FindLuaState(const std::string& ScriptName);

	// Store the pointer in the container
	bool AddStatePointer(const std::string& ScriptName, lua_State* State);

	// Create a lua state
	lua_State* CreateLuaState(const std::string& ScriptName);

	// Remove a stored pointer
	bool RemoveStatePointer(const std::string& ScriptName);

	// Add a lua state	
	lua_State* AddLuaState(const std::string& ScriptName);

	int Score, HighScore;

	void InitiallizeLuaBasedMeshes();
private:
	// Lua State Container
	std::map<std::string, lua_State*> LuaStateContainer;

	// Pointer to the Lua State
	lua_State *LuaState;

	bool MeshesInitiallized = false;
};

static int LuaSphereMeshGeneration(lua_State* State);