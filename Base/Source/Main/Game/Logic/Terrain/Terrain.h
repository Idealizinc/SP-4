/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan, Lim Rui An, Ryan
Creation Date : 20 February 2016 13:13 PM
File Name     : Terrain.h
Purpose       : Defines the statistics within a Terrain Tile
*/

#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "../../../Engine/Internal/Entity.h"
#include <string>
#include <map>
#include "../Pieces/UnitPiece.h"

//class TerrainNode;
class UnitPiece;

class Terrain : public Entity
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

	virtual ~Terrain();

	virtual void Init(){};
	virtual void Update(const float&){};
	virtual void Render(){};
	virtual void Exit();
	
	// Getters
	std::string GetTerrainName();
	int GetTerrainType();
	float GetUnitAdvantage(int stat);
	float GetUnitDisadvantage(int stat);

	// Setters
	void SetTerrainName(std::string TerrainName);
	void SetTerrainType(int TerrainType);
	void SetUnitAdvantage(int stat, float value);
	void SetUnitDisadvantage(int stat, float value);

	std::vector<UnitPiece*> PlayerUnitList;
	std::vector<UnitPiece*> EnemyUnitList;

	std::map<int, float> UnitAdvantage;
	std::map<int, float> UnitDisadvantage;

private:
	std::string EntityID;
	std::string TerrainName;
	int TerrainType;

};

#endif // _TERRAIN_H