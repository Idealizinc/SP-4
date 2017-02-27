/*
Copyright (c) Matsuda Kenichi 2016

Creator(s)    : Matsuda Kenichi
Creation Date : 15 February 2017 9:15 AM
File Name     : WeaponType.h
Purpose       : Defines a base weapon type class
*/

#ifndef _WEAPON_TYPE_H_
#define _WEAPON_TYPE_H_

#include "../../../Engine/Internal/Entity.h"

class WeaponType : public Entity
{

private:
	int DamageStat;
	float RateStat;

public:

	WeaponType(){};
	virtual ~WeaponType(){};

	virtual void Init(){};
	virtual void Update(const float&){};
	virtual void Render(){};
	virtual void Exit(){};

	enum Type
	{
		//Melee
		T_1HSWORD = 0,
		T_2HSWORD,
		T_DSWORD,

		T_MAGIC_PROJECTILE,
		T_MAGIC_AOE,

		T_FASTSHOOT,
		T_SLOWSHOOT,
		T_RAPIDSHOOT,

		T_MAX
	};

	std::string WeaponName;
	int Type;

	void SetName(std::string name) { this->WeaponName = name; }
	void SetType(int type) { this->Type = type; }
	void SetDamage(int damage) { this->DamageStat = damage; }
	void SetRate(float rate) { this->RateStat = rate; }

	std::string GetName() { return WeaponName; }
	int GetType() { return Type; }
	int GetDamage(){ return DamageStat; }
	float GetRate(){ return RateStat; }

};

#endif