#include "Melee.h"
#include "../../Engine/State/StateList.h"
#include "../Objects/Characters/MeleeCharacter.h"
#include "../Systems/ObjectManager.h"
#include "../Systems/BattleSystem.h"
#include "../Systems/GameLogicSystem.h"

MeleeStateManager::MeleeStateManager()
{
	SetMaxHistory(10);
	SetCurrentState("");
}

MeleeStateManager::~MeleeStateManager()
{
}

void MeleeStateManager::Init()
{
	//Create state
	StateList::Instance().CreateState("Idle");
	StateList::Instance().CreateState("Scout");
	StateList::Instance().CreateState("Attack");
	StateList::Instance().CreateState("Dead");

	//Attach state
	AttachState("Idle");
	AttachState("Scout");
	AttachState("Attack");
	AttachState("Dead");

	//Current state first
	SetCurrentState("Idle");

	isEngaged = false;
	//if (Val > 75)
	//	MWeapon = new MeleeWeapon(4, Math::RandFloatMinMax(0.1f, 0.3f), )

}

void MeleeStateManager::Update(const float& dt)
{
	State* temp = GetCurrentState();
	MeleeCharacter* MC = dynamic_cast<MeleeCharacter*>(InternalCharacter);
	MC->InternalTimer += dt;
	//MC->HealthPoints -= 0.1f * dt;

	

	if (temp->GetStateName() == "Idle")
	{
		MC->InternalTimer += dt;
		if (DeathCheck(MC))
		{
			MC->InternalTimer = 0;
			SetCurrentState("Dead");
		}

		else if (MC->InternalTimer > 0.5f)
		{
			if (MC->HealthPoints != MC->MaxHealthPoints)
			{
				MC->HealthPoints = Math::Clamp(MC->HealthPoints, 0, MC->MaxHealthPoints);
			}

			if (MC->InternalTimer > MC->WaitTime)
			{
				MC->InternalTimer = 0;
				SetCurrentState("Scout");
			}
		}
		//ReactToMessage();
	}
	else if (temp->GetStateName() == "Attack")
	{
		if (DeathCheck(MC))
		{
			MC->InternalTimer = 0;
			SetCurrentState("Dead");
		}
		else
		{
			MC->InternalTimer += dt;
			if(MC->TargetEnemy != nullptr)
			{
				if (!WithinRange(MC, MC->TargetEnemy) || MC->TargetEnemy->HealthPoints <= 0)
				{
					MC->TargetEnemy = nullptr;

					// Find a nearby TargetEnemy, Iterate char list, set TargetEnemy found, select closest one
					CharacterEntity* CE = FindNearestEnemy(MC);
					if (CE != nullptr)
					{
						MC->TargetEnemy = CE;
					}
				}
				else
				{
					if (MC->TargetEnemy != nullptr && MC->TargetEnemy->HealthPoints > 0 && (WithinRange(MC, MC->TargetEnemy)))
					{
						MoveToTargetEnemy(MC);
						bool DistCheck = (MC->GetPosition() - MC->TargetEnemy->GetPosition()).LengthSquared() < MWeapon->GetMeleeRange() * MWeapon->GetMeleeRange() * 3.f;
						//if (((MC != MC->TargetEnemy->TargetEnemy) && MC->TargetEnemy->GetCurrentState()->GetStateName() != "Attack") || DistCheck)
						//{
						if (DistCheck)
						{
							MWeapon->Update((float)dt);
							MeleeTarget(MC);
						}
						//}
					}
				}
			}
			else
			{
				CharacterEntity* CE = FindNearestEnemy(MC);
				if (CE == nullptr)
				{
					MC->Static = false;
					MC->TargetEnemy = nullptr;
					SetCurrentState("Idle");
					MC->WaitTime = Math::RandFloatMinMax(1.f, 3.f);
				}
				else if (CE != nullptr && (CE->RaceType != MC->RaceType))
				{
					//MC->ResetMessageAlert();
					//MC->SendingMessage = true;
					//MessageSystem::Instance().SendMessage(MC, "Engaging Enemy", MC->GetEntityID());
					MC->TargetEnemy = CE;
				}
			}
		}
	}
	else if (temp->GetStateName() == "Scout")
	{
		MC->InternalTimer += dt;

		if (DeathCheck(MC))
		{
			MC->InternalTimer = 0;
			SetCurrentState("Dead");
		}
		//Search for enemy
		CharacterEntity* CE = FindNearestEnemy(MC);
		if (CE != nullptr)
		{
			//MC->ResetMessageAlert();
			//MC->SendingMessage = true;
			//MessageSystem::Instance().SendMessage(MC, "Engaging Enemy", MC->GetEntityID());
			isEngaged = true;
			if (isEngaged)
			{
				MC->TargetDir = Vector3(0, 0, 0);
			}
			MC->TargetEnemy = CE;
			SetCurrentState("Attack");
		}

		if (MC->InternalTimer > MC->WaitTime)
		{
			MC->WaitTime = Math::RandFloatMinMax(1.5f, 5.f);
			RandomizeMovement(MC);
		}
		//ReactToMessage();
		//CharacterEntity* NewTarget = FindNearestEnemy(MC);
	}
	else if (temp->GetStateName() == "Dead")
	{
		MC->InternalTimer += dt;
		MC->Static = true;
		MC->HealthPoints = Math::Clamp(MC->HealthPoints, 0, MC->MaxHealthPoints);
		if (MC->InternalTimer > 5.f && MC->Active)
		{
			//MC->ResetMessageAlert();
			//MC->SendingMessage = true;
			//MessageSystem::Instance().SendMessage(MC, "Unit Lost", MC->GetEntityID());
			MC->Active = false;
		}
	}
}

void MeleeStateManager::Exit()
{
	if (MWeapon != nullptr)
	{
		delete MWeapon;
		MWeapon = nullptr;
	}
}

bool MeleeStateManager::DeathCheck(CharacterEntity* MC)
{
	if (MC->HealthPoints <= 0)
	{
		return true;
	}
	else return false;
}

bool MeleeStateManager::WithinRange(CharacterEntity* C1, CharacterEntity* C2)
{
	float DistanceBetween = (C2->GetPosition() - C1->GetPosition()).LengthSquared();
	if (DistanceBetween < C1->DetectionRadius * C1->DetectionRadius)
	{
		return true;
	}
	return false;
}

CharacterEntity* MeleeStateManager::FindNearestEnemy(CharacterEntity* MC)
{
	MeleeCharacter* Chara = dynamic_cast<MeleeCharacter*>(MC);
	if (Chara->isPlayer)
		Container = GameLogicSystem::Instance().InternalBattleSystem->GetEnemyCharacterList();
	else
		Container = GameLogicSystem::Instance().InternalBattleSystem->GetPlayerCharacterList();

	if (Container.size() > 1 && MC->TargetEnemy == nullptr)
	{
		// Radius Check
		// Find a nearby TargetEnemy, Iterate char list, set TargetEnemy found, select closest one
		float DistanceBetween = FLT_MAX;
		for (std::vector<CharacterEntity*>::iterator it = Container.begin(); it != Container.end(); ++it)
		{
			if (*it != MC && (*it)->Active && !DeathCheck(*it))
			{
				ObjectManager::Instance().WorldHeight;
				Vector3 Position = (*it)->GetPosition();
				// I can only target stuff inside the map, no spawn killing
				//if (Position.x > 0 && Position.x < ObjectManager::Instance().WorldWidth && Position.z > 0 && Position.z < ObjectManager::Instance().WorldHeight)
				//{
					if (WithinRange(MC, *it))
					{
						// Check if target is within FOV by comparing the current angle against the angle between the two characters
						Vector3 DirVec = MC->GetPosition() - (*it)->GetPosition();
						int AngleBetween = ((int)Math::RadianToDegree(atan2(-DirVec.x, DirVec.z))) % 360;
						int CurrentAngle = (int)MC->GetRotationAngle() % 360;
						int FOVAddition = 90 - (int)(MC->FieldOfView * 0.5f);

						if (!(AngleBetween < CurrentAngle + 90 + FOVAddition && AngleBetween > CurrentAngle - 90 - FOVAddition))
						{
							return *it;
						}
					}
				//}
			}
		}
	}
	return nullptr;
}

void MeleeStateManager::RandomizeMovement(CharacterEntity* C)
{
	MeleeCharacter* MC = (MeleeCharacter*)C;
	int Degree = Math::RandIntMinMax(-360, 360);
	//float SpeedMultiplier = Math::RandFloatMinMax(1.f, 1.5f);
	MC->SetVelocity(/*MC->GetVelocity() + (MC->TargetDir) +*/ Vector3((float)sin(Degree), 0, (float)cos(Degree)).Normalize() * Math::RandFloatMinMax((float)-MC->WalkSpeed /*SpeedMultiplier*/, (float)MC->WalkSpeed) /*SpeedMultiplier*/);
	MC->InternalTimer = 0;
}

void MeleeStateManager::MoveToTargetEnemy(CharacterEntity* SC)
{
	if (!((SC->GetPosition() - SC->TargetEnemy->GetPosition()).LengthSquared() < MWeapon->GetMeleeRange() *  MWeapon->GetMeleeRange()))
	{
		Vector3 DVec = (SC->TargetEnemy->GetPosition() - SC->GetPosition());
		// Creep up to enemy
		if (DVec.IsZero())
			DVec.Set(Math::RandFloatMinMax(-1, 1), 0, Math::RandFloatMinMax(-1, 1));
		DVec.Normalize();
		SC->SetVelocity(DVec * (float)SC->WalkSpeed);
	}
}

void MeleeStateManager::MeleeTarget(CharacterEntity* SC)
{
	Vector3 DVec = (SC->TargetEnemy->GetPosition() - SC->GetPosition());
	if (DVec.IsZero())
		DVec.Set(Math::RandFloatMinMax(-1.f, 1.f), 0, Math::RandFloatMinMax(-1.f, 1.f));
	// If too far
	if (MWeapon->CanAttack() && ((SC->GetPosition() - SC->TargetEnemy->GetPosition()).LengthSquared() <  MWeapon->GetMeleeRange()  *  MWeapon->GetMeleeRange()))
	{
		MWeapon->Attack(SC->TargetEnemy);
		DVec = Vector3(Math::RandFloatMinMax(-2.f, 2.f), 0, Math::RandFloatMinMax(-2.f, 2.f)) + SC->TargetEnemy->GetPosition() - SC->GetPosition();
		SC->SetVelocity(DVec * 0.5f * (float)SC->WalkSpeed);
	}
}

void MeleeStateManager::ReactToMessage()
{
	MeleeCharacter* SC = dynamic_cast<MeleeCharacter*>(InternalCharacter);
	//if (!SC->SendingMessage)
	//{
	//	Message* Reactable = nullptr;
	//	// What's my priority
	//	if (Reactable = SC->FindReactableMessage("Engaging Enemy"))
	//	{
	//		if (Reactable->GetMessageSender() != nullptr && WithinRange(SC, Reactable->GetMessageSender()))
	//		{
	//			SC->ResetMessageAlert();
	//			SC->FoundMessage = true;
	//			SC->TargetEnemy = Reactable->GetMessageSender()->TargetEnemy;
	//			SetCurrentState("Attack");
	//		}
	//	}
	//}
}

bool MeleeStateManager::MoveToTargetFriend()
{
	MeleeCharacter* SC = dynamic_cast<MeleeCharacter*>(InternalCharacter);
	MeleeCharacter* Friend = (MeleeCharacter*)SC->TargetFriend;
	if (Friend == nullptr)
		return true;

	//bool FriendNeedsHelp = (bool)(Friend->Ammunition < Friend->MaxAmmunition / 2);

	if ((!((SC->GetPosition() - Friend->GetPosition()).LengthSquared() < SC->GetDimensions().x *  SC->GetDimensions().z)))
	{
		Vector3 DVec = (SC->TargetFriend->GetPosition() - SC->GetPosition());
		// move near to friend
		if (DVec.IsZero())
			DVec.Set(Math::RandFloatMinMax(-1, 1), 0, Math::RandFloatMinMax(-1, 1));
		DVec.Normalize();
		SC->SetVelocity(DVec * (float)SC->WalkSpeed * 2);
		return true;
	}

	SC->TargetFriend = nullptr;
	return false;
}