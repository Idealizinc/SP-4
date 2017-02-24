/*
Copyright (c) Matsuda Kenichi 2016

Creator(s)    : Matsuda Kenichi
Creation Date : 15 February 2017 9:15 AM
File Name     : UnitType.h
Purpose       : Defines a base unit type class
*/

#ifndef _UNIT_TYPE_H_
#define _UNIT_TYPE_H_

#include "../../../Engine/Internal/Entity.h"
#include "WeaponType.h"
#include <vector>

class UnitType : public Entity
{

private:
	int Health, MaxHealth;
	int Damage;
	double Walkspeed;
	float Range;
	int Cost;

public:

	UnitType(){};
	virtual ~UnitType(){};

	virtual void Init(){};
	virtual void Update(const float&){};
	virtual void Render(){};
	virtual void Exit(){};

	enum Type
	{
		T_MELEE = 0,
		T_MAGIC,
		T_RANGE,
		T_MAX
	};

	std::string UnitName;
	std::string MeshName;
	std::vector<WeaponType*> PossibleWeapon;
	int Type;

	void SetName(const std::string& name) { this->UnitName = name; }
	void SetType(const int& type) { this->Type = type; }
	void SetMeshName(const std::string& Mname) { this->MeshName = Mname; }
	void SetHealth(int health) { this->Health = health; }
	void SetMaxHealth(int maxhealth) { this->MaxHealth = maxhealth; }
	void SetDamage(int damage) { this->Damage = damage; }
	void SetWalkspeed(double walkspeed) { this->Walkspeed = walkspeed; }
	void SetRange(float range) { this->Range = range; }
	void SetCost(int cost) { this->Cost = cost; }

	std::string GetName() { return UnitName; }
	int GetType() { return Type; }
	std::string GetMeshName() { return MeshName; }
	int GetHealth(){ return Health; }
	int GetMaxHealth(){ return MaxHealth; }
	int GetDamage(){ return Damage; }
	double GetWalkspeed(){ return Walkspeed; }
	float GetRange(){ return Range; }
	int GetCost(){ return Cost; }

};

#endif