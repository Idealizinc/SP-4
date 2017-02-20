/*
Copyright (c) Matsuda Kenichi 2016

Creator(s)    : Matsuda Kenichi
Creation Date : 15 February 2017 9:15 AM
File Name     : UnitRace.h
Purpose       : Defines a base unit race class
*/

#ifndef _UNIT_RACE_H_
#define _UNIT_RACE_H_

#include "../../../Engine/Internal/Entity.h"

class UnitRace : public Entity
{

private:
	float HealthModifier;
	float MeleeDamageModifier;
	float MagicDamageModifier;
	float RangeDamageModifier;
	float WalkspeedModifier;

public:

	UnitRace(){};
	virtual ~UnitRace(){};

	virtual void Init(){};
	virtual void Update(const float&){};
	virtual void Render(){};
	virtual void Exit(){};

	enum Race
	{
		R_HUMAN = 0,
		R_ELVEN,
		R_DEMON,
		R_BEAST,
		R_MAX
	};

	std::string RaceName;
	int Race;

	void SetName(std::string name) { this->RaceName = name; }
	void SetRace(int race) { this->Race = race; }
	void SetHealthModifier(float health) { this->HealthModifier = health; }
	void SetMeleeDamageModifier(float melee) { this->MeleeDamageModifier = melee; }
	void SetMagicDamageModifier(float magic) { this->MagicDamageModifier = magic; }
	void SetRangeDamageModifier(float range) { this->RangeDamageModifier = range; }
	void SetWalkspeedModifier(float walkspeed) { this->WalkspeedModifier = walkspeed; }

	std::string GetName() { return RaceName; }
	float GetHealthModifier(){ return HealthModifier; }
	int GetRace() { return Race; }
	float GetMeleeDamageModifier(){ return MeleeDamageModifier; }
	float GetMagicDamageModifier(){ return MagicDamageModifier; }
	float GetRangeDamageModifier(){ return RangeDamageModifier; }
	float GetWalkspeedModifier(){ return WalkspeedModifier; }
};

#endif