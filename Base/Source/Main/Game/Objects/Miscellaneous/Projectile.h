/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 20 November 2016 13:37 PM
File Name     : Projectile.h
Purpose       : Defines a projectile object
*/

#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "../../../Engine/Objects/BaseObject.h"

class Projectile : public BaseObject
{
public:
	Projectile(const std::string& Name = "", const int& DamageDealt = 0, const float& LifeTime = 0, const float& ExistenceTime = 0) : DamageDealt(DamageDealt), LifeTime(LifeTime), ExistenceTime(ExistenceTime){ SetEntityID(Name); };
	virtual ~Projectile(){};

	void SetName(const std::string& Name = ""){	SetEntityID(Name); }

	void SetDamageDealt(const int& DamageDealt = 0){ this->DamageDealt = DamageDealt; }

	void SetLifeTime(const float& LifeTime = 0) { this->LifeTime = LifeTime; }

	int GetDamageDealt(){ return DamageDealt; }

	float GetLifeTime(){ return LifeTime; }

	float GetExistenceTime(){ return ExistenceTime; }

	void ResetParameters()
	{
		Active = true;
		Static = false;
		Visible = true;
		DamageDealt = 0;
		LifeTime = 0;
		ExistenceTime = 0;
	}

	std::string OwnerID = "";

protected:
	// Damage Dealt by this bullet
	int DamageDealt;
	// Life Time of bullet
	float LifeTime;
	// Current time of bullet
	float ExistenceTime;

};

#endif //_PROJECTILE_H