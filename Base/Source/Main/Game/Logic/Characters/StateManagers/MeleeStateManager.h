#ifndef _MELEE_STATE_MANAGER_H
#define _MELEE_STATE_MANAGER_H

/*
Copyright (c) Lim Rui An, Ryan, Kenichi Matsuda 2017

Creator(s)    : Lim Rui An, Ryan, Kenichi Matsuda
Creation Date : 26 February 2017 16:13 PM
File Name     : MeleeStateManager.h
Purpose       : Defines the state logic for a Melee Type character within the battle screen
*/

#include "../BaseClasses/CharacterStateManager.h"
#include "../../Weapons/Melee/MeleeWeapon.h"


class MeleeStateManager : public CharacterStateManager
{
public:
	virtual ~MeleeStateManager(){};

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
			MeleeWeapon* MWeapon = (MeleeWeapon*)Character->InternalWeapon;
			float Distance = (DirVec).LengthSquared();
			if (Distance < MWeapon->GetMeleeRange() * MWeapon->GetMeleeRange() * 3.f)
			{
				MWeapon->Update((float)dt);
				if (MWeapon->CanAttack() && ((Character->GetPosition() - Character->TargetEnemy->GetPosition()).LengthSquared() <  MWeapon->GetMeleeRange()  *  MWeapon->GetMeleeRange() * 1.5f))
				{
					MWeapon->Attack(Character->TargetEnemy);
					Character->TargetEnemy->TargetEnemy = Character;
				}
			}
			if (DirVec.LengthSquared() < Character->DetectionRadius * Character->DetectionRadius * 3.f)
			{
				if (!DirVec.IsZero())
				{
					DirVec = Vector3(Math::RandFloatMinMax(-10.f, 10.f), 0, Math::RandFloatMinMax(-10.f, 10.f)) + Character->TargetEnemy->GetPosition() - Character->GetPosition();
					DirVec.Normalize();
					Character->SetVelocity(DirVec * 0.5f * (float)Character->WalkSpeed);
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

#endif // _MELEE_STATE_MANAGER_H

