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
		if (CE->TargetEnemy)
		{
			CE->TargetEnemy->HealthPoints -= Damage;
			int NumParticles = (int)Math::RandFloatMinMax(2, 4 * GameLogicSystem::Instance().ParticleMultiplier);
			for (int i = 0; i < NumParticles; ++i)
			{
				float ParticleSpeed = Math::RandFloatMinMax(1.f, 2.f);
				float ParticleLifeTime = Math::RandFloatMinMax(1.f, 1.5f);
				float Interval = CE->TargetEnemy->GetDimensions().x * 0.5f;
				Vector3 Dimensions = Vector3(Interval, Interval, Interval);
				Vector3 Velocity = ParticleSpeed * Vector3(Math::RandFloatMinMax(-Interval, Interval), Interval * Math::RandFloatMinMax(1.f, 1.5f), Math::RandFloatMinMax(-Interval, Interval));
				GameLogicSystem::Instance().InternalBattleSystem->ParticleSystem.AddWorldSpaceParticle("Blood", CE->TargetEnemy->GetPosition(), Dimensions, Velocity, SceneSystem::Instance().GetCurrentScene().camera->position, ParticleLifeTime);
			}
			Attackability = false;
		}
	}

private:
	float MeleeRange;
protected:

};

#endif //_MELEE_WEAPON_H