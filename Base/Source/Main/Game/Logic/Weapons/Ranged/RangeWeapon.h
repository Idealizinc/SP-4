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
		Vector3 DirVec = CE->TargetEnemy->GetPosition() - CE->GetPosition();
		// Generate Bullets
 		for (int i = 0; i < NumberOfBullets; ++i)
		{
			BattleScreenCharacter* BSC = (BattleScreenCharacter*)CE;
			// Bullet Fire
			Vector3 Inaccuracy = BulletSpread * Vector3(Math::RandFloatMinMax(-DirVec.LengthSquared(), DirVec.LengthSquared()), Math::RandFloatMinMax(DirVec.LengthSquared() * 0.5f, DirVec.LengthSquared()), Math::RandFloatMinMax(-DirVec.LengthSquared(), DirVec.LengthSquared())) - CE->GetPosition();
			Vector3 AltDirVec = CE->TargetEnemy->GetPosition() + Inaccuracy;
			Projectile* Bullet = new Projectile("", Damage, 10);
			Bullet->OwnerID = CE->GetEntityID();
			Bullet->OwnerFaction = BSC->CharacterFaction;
			Vector3 Velocity = AltDirVec.Normalized() * (float)CE->WalkSpeed * 3.f;
			Bullet->SetParameters("quad", 1, CE->GetPosition() + Vector3(0, CE->GetDimensions().y) + Velocity * 0.01f, Vector3(CE->GetDimensions().x * 0.025f, 1, CE->GetDimensions().y * 0.1f), Velocity, 0, Vector3(0, 1, 0));
			GameLogicSystem::Instance().InternalBattleSystem->AddNewProjectile(Bullet);
			
			//// Smoke Particle
			//float ParticleSpeed = Math::RandFloatMinMax(2.f, 3.f);
			//float ParticleLifeTime = Math::RandFloatMinMax(0.25f, 0.5f);
			//ObjectManager::Instance().AddNewParticle(new Particle("Smoke", 1, Bullet->GetPosition() + Bullet->GetVelocity() * 0.015f, CE->GetDimensions() * Math::RandFloatMinMax(0.3f, 0.5f), Vector3(Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(0, ParticleSpeed)), Vector3(0, 0, 100), ParticleLifeTime));
			
		}
		Attackability = false;
		Timer = 0.f;
	}

private:
	int NumberOfBullets = 1;
	const float BulletSpread = 0.003f;
protected:

};

#endif //_RANGE_WEAPON_H