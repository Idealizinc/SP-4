#ifndef _RANGE_STATE_MANAGER_H
#define _RANGE_STATE_MANAGER_H

/*
Copyright (c) Lim Rui An, Ryan, Kenichi Matsuda 2017

Creator(s)    : Lim Rui An, Ryan, Kenichi Matsuda
Creation Date : 27 February 2017 9:13 AM
File Name     : RangeStateManager.h
Purpose       : Defines the state logic for a Range Type character within the battle screen
*/

#include "../BaseClasses/CharacterStateManager.h"
#include "../../Weapons/Ranged/RangeWeapon.h"


class RangeStateManager : public CharacterStateManager
{
public:
	virtual ~RangeStateManager(){};

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
			RangeWeapon* RWeapon = (RangeWeapon*)Character->InternalWeapon;
			float Distance = (DirVec).LengthSquared();
			if (DirVec.LengthSquared() < Character->DetectionRadius * Character->DetectionRadius * 3.f)
			{
				RWeapon->Update((float)dt);
				if (!DirVec.IsZero())
				{
					DirVec = Vector3(Math::RandFloatMinMax(-10.f, 10.f), 0, Math::RandFloatMinMax(-10.f, 10.f)) + Character->TargetEnemy->GetPosition() - Character->GetPosition();
					DirVec.Normalize();
					Character->SetVelocity(DirVec * 0.5f * (float)Character->WalkSpeed);
				}
				// Shoot at enemy
				if (RWeapon->CanAttack())
				{
					RWeapon->Attack(Character);
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

#endif // _RANGE_STATE_MANAGER_H

