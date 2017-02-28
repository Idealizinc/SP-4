/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 21 November 2016 12:12 PM
File Name     : MeleeWeapon.h
Purpose       : Defines a MeleeWeapon
*/

#ifndef _MELEE_WEAPON_H
#define _MELEE_WEAPON_H

#include "../Weapon.h"

class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon(const int& Damage = 0, const float & AttackRate = 0, const float& EquipTime = 0, const float& MeleeRange = 0){ SetParameters(Damage, AttackRate, EquipTime, MeleeRange); };
	virtual ~MeleeWeapon(){};

	void SetParameters(const int& Damage, const float & AttackRate, const float& EquipTime, const float& MeleeRange)
	{
		this->Damage = Damage;
		this->AttackRate = AttackRate;
		this->EquipTime = EquipTime;
		this->MeleeRange = MeleeRange;
		ResetEquipped();
	};

	float GetMeleeRange()
	{
		return MeleeRange;
	}

	virtual void Attack(CharacterEntity* CE)
	{
		CE->HealthPoints -= Damage;
		int NumParticles = Math::RandIntMinMax(2, 4);
		/*for (int i = 0; i < NumParticles; ++i)
		{
			float ParticleSpeed = Math::RandFloatMinMax(3.f, 6.f);
			float ParticleLifeTime = Math::RandFloatMinMax(0.75f, 1.5f);
			ObjectManager::Instance().AddNewParticle(new Particle(CE->GetEntityID(), 1, CE->GetPosition(), CE->GetDimensions() * Math::RandFloatMinMax(0.3f, 0.75f), Vector3(Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(0, ParticleSpeed)), Vector3(0,0,100), ParticleLifeTime));
		}*/
		Attackability = false;
	}

private:
	float MeleeRange;
protected:

};

#endif //_MELEE_WEAPON_H