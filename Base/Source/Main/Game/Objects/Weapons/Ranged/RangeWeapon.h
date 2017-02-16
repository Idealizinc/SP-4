/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 21 November 2016 12:12 PM
File Name     : RangeWeapon.h
Purpose       : Defines a RangeWeapon
*/

#ifndef _RANGE_WEAPON_H
#define _RANGE_WEAPON_H

#include "../Weapon.h"
#include "../../../Systems/ObjectManager.h"

class RangeWeapon : public Weapon
{
public:
	RangeWeapon(const int& Damage = 0, const float & AttackRate = 0, const float& EquipTime = 0, const int& NumberOfBullets = 0){ SetParameters(Damage, AttackRate, EquipTime, NumberOfBullets); };
	virtual ~RangeWeapon(){};

	void SetParameters(const int& Damage, const float & AttackRate, const float& EquipTime, const int& NumberOfBullets)
	{
		this->Damage = Damage;
		this->AttackRate = AttackRate;
		this->EquipTime = EquipTime;
		this->NumberOfBullets = NumberOfBullets;
		ResetEquipped();
	};

	int GetNumberOfBullets()
	{
		return NumberOfBullets;
	}

	virtual void Attack(CharacterEntity* CE)
	{
		Vector3 DirVec = (CE->TargetEnemy->GetPosition() - CE->GetPosition());
		if (!DirVec.IsZero())
			CE->SetVelocity(DirVec.Normalized());

		// Generate Bullets
		for (int i = 0; i < NumberOfBullets; ++i)
		{
				
			// Bullet Fire
			Vector3 AltDirVec = (CE->TargetEnemy->GetPosition() + 0.005f * Vector3(Math::RandFloatMinMax(-DirVec.LengthSquared(), DirVec.LengthSquared()), Math::RandFloatMinMax(-DirVec.LengthSquared(), DirVec.LengthSquared())) - CE->GetPosition());
			Projectile* Bullet = new Projectile("Bullet", Damage, 10);
			Bullet->OwnerID = CE->GetEntityID();
			Vector3 Velocity = AltDirVec.Normalized() * (float)CE->WalkSpeed * (float)CE->WalkSpeed;
			Bullet->SetParameters("Bullet", 1, CE->GetPosition() + Velocity * 0.01f, Vector3(CE->GetDimensions().x * 0.1f, CE->GetDimensions().y * 0.3f, 1), Velocity, 0, Vector3(0, 0, 1));
			ObjectManager::Instance().AddNewProjectile(Bullet);

			// Smoke Particle
			float ParticleSpeed = Math::RandFloatMinMax(2.f, 3.f);
			float ParticleLifeTime = Math::RandFloatMinMax(0.25f, 0.5f);
			ObjectManager::Instance().AddNewParticle(new Particle("Smoke", 1, Bullet->GetPosition() + Bullet->GetVelocity() * 0.015f, CE->GetDimensions() * Math::RandFloatMinMax(0.3f, 0.5f), Vector3(Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(0, ParticleSpeed)), Vector3(0, 0, 100), ParticleLifeTime));
			
		}
		Attackability = false;
	}

private:
	int NumberOfBullets;
protected:

};

#endif //_RANGE_WEAPON_H