#include "LuaInterface.h"
#include "../../Engine/System/SceneSystem.h"
#include <iostream>

// Constructor
LuaInterface::LuaInterface() : LuaState(nullptr)
{

}

LuaInterface::~LuaInterface()
{

}

bool LuaInterface::Init()
{
	std::string Script = "WindowInitiallizer.lua";
	// Initiallize the LuaState
	AddLuaState("WindowInitiallizer.lua");
	return true;
}

void LuaInterface::Run()
{
	if (LuaState == nullptr)
		return;

	// Utillize the read variables
	// lua_getglobal(lua_State*, const char*);
}

void LuaInterface::Drop()
{
	LuaState = nullptr;
	for (auto it : LuaStateContainer)
	{
		lua_close(it.second);
	}
	LuaStateContainer.clear();
}

int LuaInterface::GetIntegerValue(const std::string& VariableName)
{
	lua_getglobal(LuaState, VariableName.c_str());
	return lua_tointeger(LuaState, -1);
}

float LuaInterface::GetFloatValue(const std::string& VariableName)
{
	lua_getglobal(LuaState, VariableName.c_str());
	return (float)lua_tonumber(LuaState, -1);
}

std::string LuaInterface::GetStringValue(const std::string& VariableName)
{
	lua_getglobal(LuaState, VariableName.c_str());
	return lua_tostring(LuaState, -1);
}

bool LuaInterface::GetBoolValue(const std::string& VariableName)
{
	lua_getglobal(LuaState, VariableName.c_str());
	return lua_toboolean(LuaState, -1);
}

Vector3 LuaInterface::GetVector3Value(const std::string& VariableName)
{
	lua_getglobal(LuaState, VariableName.c_str());
	lua_rawgeti(LuaState, -1, 1);
	int x = lua_tonumber(LuaState, -1);
	lua_pop(LuaState, 1);
	lua_rawgeti(LuaState, -1, 2);
	int y = lua_tonumber(LuaState, -1);
	lua_pop(LuaState, 1);
	lua_rawgeti(LuaState, -1, 3);
	int z = lua_tonumber(LuaState, -1);
	lua_pop(LuaState, 1);
	return Vector3(x, y, z);
}

float LuaInterface::GetVector3DistanceSquared(Vector3 Direction)
{
	AddLuaState("Vector3Utility.lua");
	lua_getglobal(LuaState, "CalculateDistanceSquared");
	lua_pushnumber(LuaState, Direction.x);
	lua_pushnumber(LuaState, Direction.y);
	lua_pushnumber(LuaState, Direction.z);
	lua_call(LuaState, 3, 1);
	float DS = (float)lua_tonumber(LuaState, -1);
	lua_pop(LuaState, 1);
	return DS;
}

float LuaInterface::GetVector3DistanceBetweenSquared(Vector3 Start, Vector3 End)
{
	AddLuaState("Vector3Utility.lua");
	lua_getglobal(LuaState, "CalculateBetweenDistanceSquared");
	lua_pushnumber(LuaState, Start.x);
	lua_pushnumber(LuaState, Start.y);
	lua_pushnumber(LuaState, Start.z);
	lua_pushnumber(LuaState, End.x);
	lua_pushnumber(LuaState, End.y);
	lua_pushnumber(LuaState, End.z);
	lua_call(LuaState, 6, 1);
	float DS = (float)lua_tonumber(LuaState, -1);
	lua_pop(LuaState, 1);
	return DS;
}

float LuaInterface::GetVector3Dot(Vector3 A, Vector3 B)
{
	AddLuaState("Vector3Utility.lua");
	lua_getglobal(LuaState, "CalculateDotProduct");
	lua_pushnumber(LuaState, A.x);
	lua_pushnumber(LuaState, A.y);
	lua_pushnumber(LuaState, A.z);
	lua_pushnumber(LuaState, B.x);
	lua_pushnumber(LuaState, B.y);
	lua_pushnumber(LuaState, B.z);
	lua_call(LuaState, 6, 1);
	float D = (float)lua_tonumber(LuaState, -1);
	lua_pop(LuaState, 1);
	return D;
}

void LuaInterface::SaveIntegerValue(const char* VariableName, const int Value, const bool Overwrite)
{
	// Function Name within the file
	AddLuaState("DataStoreUtility.lua");
	lua_getglobal(LuaState, "SaveToLuaFile");
	char Output[80];
	sprintf(Output, "%s = %d\n", VariableName, Value);
	lua_pushstring(LuaState, Output);
	lua_pushinteger(LuaState, Overwrite);
	lua_call(LuaState, 2, 0);
}

lua_State* LuaInterface::FindLuaState(const std::string& ScriptName)
{
	for (std::map<std::string, lua_State*>::iterator it = LuaStateContainer.begin(); it != LuaStateContainer.end(); ++it)
	{
		if ((*it).first == ScriptName)
		{
			// If a lua state of that name already exists, return it
			LuaState = (*it).second;
			return (*it).second;
		}
	}
	return nullptr;
}

bool LuaInterface::AddStatePointer(const std::string& ScriptName, lua_State* State)
{
	// If the state does not exist, create it
	if (FindLuaState(ScriptName) == nullptr)
	{
		LuaStateContainer.insert(std::pair<std::string, lua_State*>(ScriptName, State));
		return true;
	}
	else return false;
}

lua_State* LuaInterface::CreateLuaState(const std::string& ScriptName)
{
	lua_State* NewState;
	NewState = lua_open();

	if (NewState)
	{
		// Load Lua's Auxillary Libraries
		luaL_openlibs(NewState);
		// Load a Lua Script
		luaL_dofile(NewState, ("Scripts/" + ScriptName).c_str());
		return NewState;
	}
	else return nullptr;
}

bool LuaInterface::RemoveStatePointer(const std::string& ScriptName)
{
	for (std::map<std::string, lua_State*>::iterator it = LuaStateContainer.begin(); it != LuaStateContainer.end(); ++it)
	{
		if ((*it).first == ScriptName)
		{
			// found the deletion target
			lua_close((*it).second);
			LuaStateContainer.erase(it);
			return true;
		}
	}
	return false;
}

lua_State* LuaInterface::AddLuaState(const std::string& ScriptName)
{
	if ((LuaState = FindLuaState(ScriptName)) == nullptr)
	{
		LuaState = CreateLuaState(ScriptName);
		AddStatePointer(ScriptName, LuaState);
	}
	return LuaState;
}

void LuaInterface::InitiallizeLuaBasedMeshes()
{
	if (!MeshesInitiallized)
	{
		AddLuaState("MeshUtility.lua");
		lua_register(LuaState, "CreateSphereMesh", &LuaSphereMeshGeneration);
		lua_getglobal(LuaState, "InitiallizeMeshes");
		lua_call(LuaState, 0, 0);
		MeshesInitiallized = true;
	}
}

int LuaSphereMeshGeneration(lua_State* State)
{
	int NumberOfReturnedVariables = lua_gettop(State);
	int HardcodedNumberOfReturnedVariables = 7;
	if (NumberOfReturnedVariables != HardcodedNumberOfReturnedVariables)
		return 0;

	std::string MeshName = lua_tostring(State, 1);
	Color Color3 = Color((float)lua_tonumber(State, 2), (float)lua_tonumber(State, 3), (float)lua_tonumber(State, 4));
	int numStack = lua_tointeger(State, 5);
	int numSlice = lua_tointeger(State, 6);
	float radius = (float)lua_tonumber(State, 7);


	RenderSystem* RS = (RenderSystem*)(&(SceneSystem::Instance().GetRenderSystem()));
	Mesh* mesh = MeshBuilder::GenerateSphere(MeshName, Color3, numStack, numSlice, radius);
	RS->MeshList.insert(std::pair<std::string, Mesh*>(mesh->name, mesh));

	return 1;
};