#include "Magic.h"
#include "../../Engine/State/StateList.h"
#include "../Objects/Characters/MagicCharacter.h"
#include "../Systems/EventSystem.h"

MagicStateManager::MagicStateManager()
{
	SetMaxHistory(10);
	SetCurrentState("");
}

MagicStateManager::~MagicStateManager()
{
}

void MagicStateManager::Init()
{
	StateList::Instance().CreateState("Idle");
	StateList::Instance().CreateState("Attack");
	StateList::Instance().CreateState("Scout");
	StateList::Instance().CreateState("Dead");

	AttachState("Idle");
	AttachState("Attack");
	AttachState("Scout");
	AttachState("Dead");

	SetCurrentState("Idle");
}

void MagicStateManager::Update(const float &dt)
{
	State* temp = GetCurrentState();
	MagicCharacter* RC = dynamic_cast<MagicCharacter*>(InternalCharacter);
	RC->InternalTimer += dt;

	if (temp->GetStateName() == "Idle")
	{
		if (DeathCheck(RC))
		{
			RC->InternalTimer = 0;
			SetCurrentState("Dead");
		}
		else if (RC->InternalTimer > RC->WaitTime)
			SetCurrentState("Scout");
		ReactToMessage();
	}
	else if (temp->GetStateName() == "Attack")
	{
		if (DeathCheck(RC))
		{
			RC->InternalTimer = 0;
			SetCurrentState("Dead");
		}
		else
		{
			if (RC->TargetEnemy != nullptr)
			{
				if (WithinMagic(RC, RC->TargetEnemy) && RC->TargetEnemy->HealthPoints > 0)
				{
					MWeapon->Update((float)dt);
					RC->CastTime -= dt;
					RC->Static = true;
					if (RC->CastTime <= 0)
					{
						RC->CastTime = RC->MaxCastTime;
						FireAtTarget(RC);
						RC->Static = false;
					}
				}
				else if (!WithinMagic(RC, RC->TargetEnemy) || RC->TargetEnemy->HealthPoints <= 0)
				{
					RC->CastTime = RC->MaxCastTime;
					RC->Static = false;
					RC->TargetEnemy = nullptr;
					SetCurrentState("Idle");
					RC->WaitTime = Math::RandFloatMinMax(1.f, 3.f);
				}
			}
			else
			{
				CharacterEntity* NewTarget = FindNearestEnemy(RC);
				if (NewTarget == nullptr)
				{
					RC->Static = false;
					RC->TargetEnemy = nullptr;
					SetCurrentState("Idle");
					RC->WaitTime = Math::RandFloatMinMax(1.f, 3.f);
				}
				else if (NewTarget != nullptr && RC->Active)
				{
					//RC->ResetMessageAlert();
					//RC->SendingMessage = true;
					//MessageSystem::Instance().SendMessage(RC, "Enemy Spotted", RC->GetEntityID());
					RC->TargetEnemy = NewTarget;
				}
			}
		}

	}
	else if (temp->GetStateName() == "Scout")
	{
		RC->InternalTimer += dt;
		if (RC->InternalTimer > RC->WaitTime)
		{
			RC->WaitTime = Math::RandFloatMinMax(1.f, 5.f);
			RandomizeMovement(RC);
		}
		CharacterEntity* NewTarget = FindNearestEnemy(RC);
		if (NewTarget != nullptr && RC->Active && (NewTarget->RaceType != RC->RaceType))
		{
			//RC->ResetMessageAlert();
			//RC->SendingMessage = true;
			//MessageSystem::Instance().SendMessage(RC, "Enemy Spotted", RC->GetEntityID());
			RC->TargetEnemy = NewTarget;
			SetCurrentState("Attack");
			RC->InternalTimer = 0;
		}
		//ReactToMessage();
	}
	else if (temp->GetStateName() == "Dead")
	{
		RC->InternalTimer += dt;
		RC->Static = true;
		RC->HealthPoints = Math::Clamp(RC->HealthPoints, 0, RC->MaxHealthPoints);
		if (RC->InternalTimer > 5.f && RC->Active)
		{
			//RC->ResetMessageAlert();
			//RC->SendingMessage = true;
			//MessageSystem::Instance().SendMessage(RC, "Mage Lost", RC->GetEntityID());
			RC->Active = false;
		}
	}
}

void MagicStateManager::Exit()
{
	// Delete variables you create
	// Don't delete states
	if (MWeapon != nullptr)
	{
		delete MWeapon;
		MWeapon = nullptr;
	}
}

bool MagicStateManager::DeathCheck(CharacterEntity* RC)
{
	// Death Check
	if (RC->HealthPoints <= 0)
	{
		return true;
	}
	else return false;
}

bool MagicStateManager::WithinMagic(CharacterEntity* C1, CharacterEntity* C2)
{
	float DistanceBetween = (C2->GetPosition() - C1->GetPosition()).LengthSquared();
	if (DistanceBetween < C1->DetectionRadius * C1->DetectionRadius)
	{
		return true;
	}
	return false;
}

CharacterEntity* MagicStateManager::FindNearestEnemy(CharacterEntity* RC)
{
	//std::vector<CharacterEntity*> Container = ObjectManager::Instance().GetCharacterList();
	//if (Container.size() > 1 && RC->TargetEnemy == nullptr)
	//{
	//	// Radius Check
	//	// Find a nearby TargetEnemy, Iterate char list, set TargetEnemy found, select closest one
	//	float DistanceBetween = FLT_MAX;
	//	for (std::vector<CharacterEntity*>::iterator it = Container.begin(); it != Container.end(); ++it)
	//	{
	//		if (*it != RC && (*it)->Active && !DeathCheck(*it) && (*it)->GetEntityID() != "Magic")
	//		{
	//			ObjectManager::Instance().WorldHeight;
	//			Vector3 Position = (*it)->GetPosition();
	//			// I can only target stuff inside the map, no spawn killing
	//			if (Position.x > 0 && Position.x < ObjectManager::Instance().WorldWidth && Position.y > 0 && Position.y < ObjectManager::Instance().WorldHeight)
	//			{
	//				if (WithinMagic(RC, *it))
	//				{
	//					// Check if target is within FOV by comparing the current angle against the angle between the two characters
	//					Vector3 DirVec = RC->GetPosition() - (*it)->GetPosition();
	//					int AngleBetween = ((int)Math::RadianToDegree(atan2(-DirVec.x, DirVec.y))) % 360;
	//					int CurrentAngle = (int)RC->GetRotationAngle() % 360;
	//					int FOVAddition = 90 - (int)(RC->FieldOfView * 0.5f);
	//
	//					if (!(AngleBetween < CurrentAngle + 90 + FOVAddition && AngleBetween > CurrentAngle - 90 - FOVAddition))
	//					{
	//						return *it;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	return nullptr;
}

void MagicStateManager::RandomizeMovement(CharacterEntity* C)
{
	MagicCharacter* RC = (MagicCharacter*)C;
	int Degree = Math::RandIntMinMax(-360, 360);
	float SpeedMultiplier = Math::RandFloatMinMax(1.f, 1.5f);
	RC->SetVelocity(RC->GetVelocity() + Vector3((float)sin(Degree), (float)cos(Degree)).Normalize() * Math::RandFloatMinMax((float)-RC->WalkSpeed * SpeedMultiplier, (float)RC->WalkSpeed)* SpeedMultiplier);
	RC->InternalTimer = 0;
}

void MagicStateManager::FixedMovment(CharacterEntity* C)
{
	MagicCharacter* RC = (MagicCharacter*)C;
	float SpeedMultiplier = Math::RandFloatMinMax(1.f, 1.5f);
	RC->SetVelocity(RC->GetVelocity() + Vector3(0, Math::RandFloatMinMax((float)-RC->WalkSpeed * SpeedMultiplier, 0) * SpeedMultiplier));
	RC->InternalTimer = 0;
}

void MagicStateManager::FireAtTarget(CharacterEntity* DC)
{
	// Aim at enemy
	Vector3 DirVec = (DC->TargetEnemy->GetPosition() - DC->GetPosition());
	if (!DirVec.IsZero())
		DC->SetVelocity(DirVec.Normalized() * (float)DC->WalkSpeed * 0.1f);

	// Shoot at enemy
	if (MWeapon->CanAttack())
	{
		MWeapon->Attack(DC);
	}
}

void MagicStateManager::ReactToMessage()
{
	MagicCharacter* DC = dynamic_cast<MagicCharacter*>(InternalCharacter);
	//if (!DC->SendingMessage)
	//{
	//	Message* Reactable = nullptr;
	//	// What's my priority
	//	if (Reactable = DC->FindReactableMessage("Enemy Spotted"))
	//	{
	//		if (Reactable->GetMessageSender() != nullptr && WithinMagic(DC, Reactable->GetMessageSender()))
	//		{
	//			DC->ResetMessageAlert();
	//			DC->FoundMessage = true;
	//			DC->TargetEnemy = Reactable->GetMessageSender()->TargetEnemy;
	//			SetCurrentState("Attack");
	//		}
	//	}
	//}
}	//

bool MagicStateManager::MoveToTargetFriend()
{
	MagicCharacter* DC = dynamic_cast<MagicCharacter*>(InternalCharacter);
	if (DC->TargetFriend == nullptr)
		return true;
	if (!((DC->GetPosition() - DC->TargetFriend->GetPosition()).LengthSquared() < 3 * DC->GetDimensions().x *  DC->GetDimensions().y))
	{
		Vector3 DVec = (DC->TargetFriend->GetPosition() - DC->GetPosition());
		// move near to friend
		if (DVec.IsZero())
			DVec.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1));
		DVec.Normalize();
		DC->SetVelocity(DVec * (float)DC->WalkSpeed);
		return true;
	}
	else
	{
		DC->TargetFriend->HealthPoints += (int)(DC->MaxHealthPoints * DC->HealRate);
		DC->TargetFriend = nullptr;
		return false;
	}
}