/*
Copyright (c) Matsuda Kenichi 2016

Creator(s)    : Matsuda Kenichi
Creation Date : 16 February 2017 2:00 PM
File Name     : TerrainType.h
Purpose       : Defines a base terrain type class
*/

#ifndef _TERRAIN_TYPE_H_
#define _TERRAIN_TYPE_H_

#include "../../../Engine/Internal/Entity.h"
#include <vector>

class TerrainType : public Entity
{

private:
	float T_HealthModifier;
	float T_MeleeDamageModifier;
	float T_MagicDamageModifier;
	float T_RangeDamageModifier;

public:

	TerrainType(){};
	virtual ~TerrainType(){};

	virtual void Init(){};
	virtual void Update(const float&){};
	virtual void Render(){};
	virtual void Exit(){};

	enum TerrainBoost
	{
		TB_HEALTH = 0,
		TB_MELEEDMG,
		TB_MAGICDMG,
		TB_RANGEDMG,
		TB_MAX
	};

	std::string TerrainName;
	int TerrainBooster;

	void SetName(std::string name) { this->TerrainName = name; }
	void SetTerrainBoost(int boost) { this->TerrainBooster = boost; }
	void SetT_HealthModifier(float health) { this->T_HealthModifier = health; }
	void SetT_MeleeDmgModifier(float melee) { this->T_MeleeDamageModifier = melee; }
	void SetT_MagicDmgModifier(float magic) { this->T_MagicDamageModifier = magic; }
	void SetT_RangeDmgModifier(float range) { this->T_RangeDamageModifier = range; }

	std::string GetName() { return TerrainName; }
	int GetT_Boost(){ return TerrainBooster; }
	float GetT_HealthModifier() { return T_HealthModifier; }
	float GetT_MeleeDmgModifier() { return T_MeleeDamageModifier; }
	float GetT_MagicDmgModifier() { return T_MagicDamageModifier; }
	float GetT_RangeDmgModifier() { return T_RangeDamageModifier; }


};

#endif