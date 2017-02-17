/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 20 November 2016 13:37 PM
File Name     : Weapon.h
Purpose       : Defines a Weapon
*/

#ifndef _WEAPON_H
#define _WEAPON_H

#include "../../../Engine/Objects/BaseObject.h"
#include "../../../Engine/Objects/CharacterEntity.h"

class Weapon : public BaseObject
{
public:
	Weapon(){
		Damage = 0;
		AttackRate = 0;
		EquipTime = -1;
		Equipped = false;
		Timer = 0;
		Type = 0;
		Attackability = false;
	};
	virtual ~Weapon(){};

	//Getters
	int GetDamage(){ return Damage; }
	float GetAttackRate(){ return AttackRate; }
	float GetEquipTime(){ return EquipTime; }
	bool IsEquipped(){ return Equipped; }
	bool CanAttack(){ return Attackability; }

	// Setter
	void ResetTimer()
	{
		Timer = 0;
	};

	void ResetEquipped()
	{
		Equipped = false;
		ResetTimer();
	};

	void SetParameters(const int& Damage, const float & AttackRate, const float& EquipTime)
	{
		this->Damage = Damage;
		this->AttackRate = AttackRate;
		this->EquipTime = EquipTime;
		ResetEquipped();
	};

	virtual void Init(){};

	virtual void Update(const float& dt)
	{
		Timer += (float)dt;
		if (!Equipped && Timer >= EquipTime)
		{
			Equipped = true;
			ResetTimer();
		}
		else if (Equipped && !Attackability && Timer >= AttackRate)
		{
			Attackability = true;
			ResetTimer();
		}
		//if (Active) // Still can update if invisible
		//{
		//	Vector3 StoredVelocity = GetVelocity();
		//	if (!Static)
		//		SetPosition(GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt);
		//	SetRotationAngle(Math::RadianToDegree(atan2(-GetVelocity().x, GetVelocity().y)));
		//}
	}

	virtual void Render(){}; 
	virtual void Exit(){};

	virtual void Attack(CharacterEntity*) = 0;

	float WeaponMass = 1.f;

protected:
	// Damage Dealt to Enemies
	int Damage;
	// Interval at which the weapon can be used
	float AttackRate;
	// Time to wait before weapon is usable
	float EquipTime;
	// When true weapon is usable
	bool Equipped;
	// Internal timer
	float Timer;
	// Attackability?
	bool Attackability;
	// Type
	int Type;
	
};

#endif //_WEAPON_H