#ifndef _MAGIC_WEAPON_H
#define _MAGIC_WEAPON_H

#include "../Weapon.h"
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
		Vector3 DirVec = CE->TargetEnemy->GetPosition() - CE->GetPosition();
		BattleScreenCharacter* BSC = (BattleScreenCharacter*)CE;
		BSC->LookVector = DirVec.Normalized();

		// Bullet Fire
		if (this->Type == WeaponType::T_MAGIC_PROJECTILE)
		{
			// Bullet Fire
			Vector3 Inaccuracy = BulletSpread * Vector3(Math::RandFloatMinMax(-DirVec.LengthSquared(), DirVec.LengthSquared()), Math::RandFloatMinMax(DirVec.LengthSquared() * 0.5f, DirVec.LengthSquared()), Math::RandFloatMinMax(-DirVec.LengthSquared(), DirVec.LengthSquared())) - CE->GetPosition();
			Vector3 AltDirVec = CE->TargetEnemy->GetPosition() + Inaccuracy;
			Projectile* Bullet = new Projectile("", Damage, 10);
			Bullet->OwnerID = CE->GetEntityID();
			Bullet->OwnerFaction = BSC->CharacterFaction;
			Bullet->GravityAffected = false;
			Vector3 Velocity = AltDirVec.Normalized() * (float)CE->WalkSpeed * 3.f;
			Bullet->SetParameters("Explosion", 1, CE->GetPosition() + Vector3(0, CE->GetDimensions().y) + Velocity * 0.01f, Vector3(CE->GetDimensions().x * 0.5f, CE->GetDimensions().x * 0.5f, CE->GetDimensions().x * 0.5f), Velocity, 0, Vector3(0, 1, 0));
			GameLogicSystem::Instance().InternalBattleSystem->AddNewProjectile(Bullet);
		}
		else if (this->Type == WeaponType::T_MAGIC_AOE)
		{
			Vector3 Inaccuracy = BulletSpread * Vector3(Math::RandFloatMinMax(-DirVec.LengthSquared(), DirVec.LengthSquared()), Math::RandFloatMinMax(DirVec.LengthSquared() * 0.5f, DirVec.LengthSquared()), Math::RandFloatMinMax(-DirVec.LengthSquared(), DirVec.LengthSquared())) - CE->GetPosition();
			Vector3 AltDirVec = CE->TargetEnemy->GetPosition() + Inaccuracy;
			Projectile* Bullet = new Projectile("", Damage, 10);
			Bullet->OwnerID = CE->GetEntityID();
			Bullet->OwnerFaction = BSC->CharacterFaction;
			Bullet->GravityAffected = false;
			Bullet->DespawnOnHit = false;
			Bullet->SetLifeTime(1.f);
			Bullet->SetParameters("Explosion", 1, CE->TargetEnemy->GetPosition(), Vector3(CE->GetDimensions().x * 2.f, CE->GetDimensions().x * 2.f, CE->GetDimensions().x * 2.f), Velocity, 0, Vector3(0, 1, 0));
			GameLogicSystem::Instance().InternalBattleSystem->AddNewProjectile(Bullet);
		}

		//// Magic Particle
		//float ParticleSpeed = Math::RandFloatMinMax(2.f, 3.f);
		//float ParticleLifeTime = Math::RandFloatMinMax(0.25f, 0.5f);
		//ObjectManager::Instance().AddNewParticle(new Particle("Magic Particle", 1, Magic->GetPosition() + Magic->GetVelocity() * 0.015f, CE->GetDimensions() * Math::RandFloatMinMax(0.3f, 0.5f), Vector3(Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(0, ParticleSpeed)), Vector3(0, 0, 100), ParticleLifeTime));

		Attackability = false;
		Timer = 0.f;
	}

private:
protected:
	const float BulletSpread = 0.0025f;
};

#endif //_MAGIC_WEAPON_H