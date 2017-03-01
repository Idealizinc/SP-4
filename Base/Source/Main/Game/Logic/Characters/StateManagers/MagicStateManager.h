#ifndef _MAGIC_STATE_MANAGER_H
#define _MAGIC_STATE_MANAGER_H

/*
Copyright (c) Lim Rui An, Ryan, Kenichi Matsuda 2017

Creator(s)    : Lim Rui An, Ryan, Kenichi Matsuda
Creation Date : 27 February 2017 9:13 AM
File Name     : MagicStateManager.h
Purpose       : Defines the state logic for a Magic Type character within the battle screen
*/

#include "../BaseClasses/CharacterStateManager.h"
#include "../../Weapons/Magic/MagicWeapon.h"


class MagicStateManager : public CharacterStateManager
{
public:
	virtual ~MagicStateManager(){};

	// Overriding the Attack logic for this character
	virtual void Attack(const float& dt)
	{
		BattleScreenCharacter* Character = dynamic_cast<BattleScreenCharacter*>(InternalCharacter);
		Character->InternalTimerVector[0] += dt;

		// Death Check
		if (DeathCheck(Character))
		{
			Character->InternalTimerVector[0] = 0;
			SetCurrentState("Dead");
		}
		// Target DeathCheck
		else if (DeathCheck(Character->TargetEnemy))
		{
			Character->TargetEnemy = nullptr;
			TargetNode = nullptr;
			SetCurrentState("Idle");
		}
		else
		{
			// Check if target is not too far away
			Vector3 TargetWithoutY = Character->TargetEnemy->GetPosition();
			TargetWithoutY.y = 0;
			Vector3 DirVec = TargetWithoutY - (Character->GetPosition() - Vector3(0, Character->GetPosition().y, 0));
			MagicWeapon* MWeapon = (MagicWeapon*)Character->InternalWeapon;
			float Distance = (DirVec).LengthSquared();
			if (DirVec.LengthSquared() < Character->DetectionRadius * Character->DetectionRadius * 2.f)
			{
				MWeapon->Update((float)dt);
				if (!DirVec.IsZero())
				{
					DirVec = Vector3(Math::RandFloatMinMax(-10.f, 10.f), 0, Math::RandFloatMinMax(-10.f, 10.f)) + Character->TargetEnemy->GetPosition() - Character->GetPosition();
					DirVec.Normalize();
					Character->SetVelocity(DirVec);
				}
				// Shoot at enemy
				if (MWeapon->CanAttack())
				{
					MWeapon->Attack(Character);
				}
				else {
					int NumParticles = Math::RandIntMinMax(0, 1);
					for (int i = 0; i < NumParticles; ++i)
					{
						float ParticleSpeed = Math::RandFloatMinMax(1.f, 2.f);
						float ParticleLifeTime = Math::RandFloatMinMax(1.f, 1.5f);
						float Interval = Character->GetDimensions().x * 0.5f;
						Vector3 Dimensions = Vector3(Interval, Interval, Interval);
						Vector3 Velocity = ParticleSpeed * Vector3(Math::RandFloatMinMax(-Interval, Interval), Interval * Math::RandFloatMinMax(1.f, 1.5f), Math::RandFloatMinMax(-Interval, Interval));
						GameLogicSystem::Instance().InternalBattleSystem->ParticleSystem.AddWorldSpaceParticle("Explosion", Character->GetPosition() - Vector3(0, Character->GetDimensions().y * 0.25f), Dimensions, Velocity, SceneSystem::Instance().GetCurrentScene().camera->position, ParticleLifeTime);
					}
				}
			}
			else
			{
				SetCurrentState("Idle");
				Character->TargetEnemy = nullptr;
				TargetNode = nullptr;
			}
		}
	}
};

#endif // _MAGIC_STATE_MANAGER_H

