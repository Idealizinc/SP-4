/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan, Lim Rui An, Ryan
Creation Date : 20 February 2016 13:13 PM
File Name     : Terrain.h
Purpose       : Defines the statistics within a Terrain Tile
*/

#ifndef _TERRAIN_H
#define _TERRAIN_H

#include <string>
#include <map>
#include <vector>
#include "../Pieces/UnitPiece.h"

class TerrainNode;
class UnitPiece;

class Terrain
{
public:
	enum TerrainStatEnum
	{
		T_HEALTH = 0,
		T_MELEE,
		T_MAGIC,
		T_RANGE,
		T_NORMAL,
		T_MAX,
	};
	Terrain()
	{

	}
	~Terrain()
	{

	}

	// Getters
	std::string GetEntityID()
	{
		return EntityID;
	}
	std::string GetTerrainName()
	{
		return TerrainName;
	}
	int GetTerrainType()
	{
		return TerrainType;
	}
	float GetUnitAdvantage(int stat)
	{
		return UnitAdvantage.find(stat)->second;
	}
	float GetUnitDisadvantage(int stat)
	{
		return UnitDisadvantage.find(stat)->second;
	}

	// Setters
	void SetEntityID(std::string EntityID)
	{
		this->EntityID = EntityID;
	}
	void SetTerrainName(std::string TerrainName)
	{
		this->TerrainName = TerrainName;
	}
	void SetTerrainType(int TerrainType)
	{
		this->TerrainType = TerrainType;
	}
	void SetUnitAdvantage(int stat, float value)
	{
		UnitAdvantage.insert(std::pair<int, float>(stat,value));
	}
	void SetUnitDisadvantage(int stat, float value)
	{
		UnitDisadvantage.insert(std::pair<int, float>(stat, value));
	}

	std::vector<UnitPiece*> PlayerUnitList;
	std::vector<UnitPiece*> EnemyUnitList;

private:
	std::string EntityID;
	std::string TerrainName;
	int TerrainType;

	std::map<int, float> UnitAdvantage;
	std::map<int, float> UnitDisadvantage;

};

#endif // _TERRAIN_H