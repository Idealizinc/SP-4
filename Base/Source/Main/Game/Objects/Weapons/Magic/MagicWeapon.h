#ifndef _MAGIC_WEAPON_H
#define _MAGIC_WEAPON_H

#include "../Weapon.h"
#include "../../../Systems/ObjectManager.h"
#include "../../UnitData/WeaponType.h"

class MagicWeapon : public Weapon
{
public:
	MagicWeapon(const int& Damage = 0, const float & AttackRate = 0, const int& Type = 0){ SetParameters(Damage, AttackRate, Type); };
	virtual ~MagicWeapon(){};

	void SetParameters(const int& Damage, const float & AttackRate, const int& Type)
	{
		this->Damage = Damage;
		this->AttackRate = AttackRate;
		this->Type = Type;
		ResetEquipped();
	};

	virtual void Attack(CharacterEntity* CE)
	{
		Vector3 DirVec = (CE->TargetEnemy->GetPosition() - CE->GetPosition());
		if (!DirVec.IsZero())
			CE->SetVelocity(DirVec.Normalized());

		// Bullet Fire
		if (this->Type == WeaponType::T_MAGIC_PROJECTILE)
		{
			//Vector3 AltDirVec = (CE->TargetEnemy->GetPosition() + 0.001f * Vector3(Math::RandFloatMinMax(-DirVec.LengthSquared(), DirVec.LengthSquared()), Math::RandFloatMinMax(-DirVec.LengthSquared(), DirVec.LengthSquared())) - CE->GetPosition());
			//Projectile* Magic = new Projectile("Magical", Damage, 0, 10);
			//Magic->OwnerID = CE->GetEntityID();
			//Vector3 Velocity = AltDirVec.Normalized() * (float)CE->WalkSpeed * 5;
			//Magic->SetParameters("Magical", 1, CE->GetPosition(), Vector3(CE->GetDimensions().x * 0.3f, CE->GetDimensions().y * 0.3f, 1), Velocity, 0, Vector3(0, 0, 1));
			//ObjectManager::Instance().AddNewProjectile(Magic);
		}
		else if (this->Type == WeaponType::T_MAGIC_AOE)
		{
			//Projectile* Magic = new Projectile("Magical", Damage, 0, 10);
			//Magic->OwnerID = CE->GetEntityID();
			//Magic->SetParameters("Magical", 1, CE->TargetEnemy->GetPosition(), Vector3(CE->GetDimensions().x * 3.f, CE->GetDimensions().y * 3.f, 1), 0, 0, Vector3(0, 0, 1));
			//ObjectManager::Instance().AddNewProjectile(Magic);
			//
		}

		//// Magic Particle
		//float ParticleSpeed = Math::RandFloatMinMax(2.f, 3.f);
		//float ParticleLifeTime = Math::RandFloatMinMax(0.25f, 0.5f);
		//ObjectManager::Instance().AddNewParticle(new Particle("Magic Particle", 1, Magic->GetPosition() + Magic->GetVelocity() * 0.015f, CE->GetDimensions() * Math::RandFloatMinMax(0.3f, 0.5f), Vector3(Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(0, ParticleSpeed)), Vector3(0, 0, 100), ParticleLifeTime));

		Attackability = false;
	}

private:
protected:

};

#endif //_MAGIC_WEAPON_H