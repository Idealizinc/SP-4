#include "RobotStateManager.h"
#include "../../Engine/State/StateList.h"
#include "../Objects/Characters/RobotCharacter.h"
#include "../Systems/ObjectManager.h"
#include "../Systems/EventSystem.h"
#include "../../Game/Player/GlobalPlayer.h"
#include "../Systems/EventSystem.h"

RobotStateManager::RobotStateManager()
{
	SetMaxHistory(10);
	SetCurrentState("");
}

RobotStateManager::~RobotStateManager()
{

}

void RobotStateManager::Init()
{
	// Create The States
	StateList::Instance().CreateState("Idle");
	StateList::Instance().CreateState("Attack");
	StateList::Instance().CreateState("Scout");
	StateList::Instance().CreateState("Dead");

	// Set used states
	AttachState("Idle");
	AttachState("Attack");
	AttachState("Scout");
	AttachState("Dead");

	// Remember to set current state or nothing will run
	SetCurrentState("Idle");
}

void RobotStateManager::Update(const float& dt)
{
	// Do state machine stuff
	State* temp = GetCurrentState();
	RobotCharacter* RC = dynamic_cast<RobotCharacter*>(InternalCharacter);
	RC->InternalTimer += dt;

	if (temp->GetStateName() == "Idle")
	{
		// Death Check
		if (DeathCheck(RC))
		{
			RC->InternalTimer = 0;
			SetCurrentState("Dead");
		}
		// Scout Timer
		else if (RC->InternalTimer > RC->WaitTime)
			SetCurrentState("Scout");
	}
	else if (temp->GetStateName() == "Attack")
	{
		// Death Check
		if (DeathCheck(RC))
		{
			RC->InternalTimer = 0;
			SetCurrentState("Dead");
		}
		else
		{
			// Check if target is not too far away
			Vector3 PlayerWithoutY = GlobalPlayer::Instance().GetCurrentPosition();
			PlayerWithoutY.y = 0;
			Vector3 DirVec = PlayerWithoutY - (RC->GetPosition() - Vector3(0, RC->GetPosition().y, 0));
			float Distance = (DirVec).LengthSquared();
			if (Distance < RC->DetectionRadius * RC->DetectionRadius && GlobalPlayer::Instance().GetCurrentHealth() > 0)
			{
				if (!DirVec.IsZero())
					DirVec.Normalize() *= RC->WalkSpeed;
				DirVec.y = 0;
				RC->SetVelocity(DirVec);
				RC->WaitTime = Math::RandFloatMinMax(1.f, 3.f); 
				if (Distance <= RC->GetDimensions().x * RC->GetDimensions().x)
				{
					GlobalPlayer::Instance().SetCurrentHealth(GlobalPlayer::Instance().GetCurrentHealth() - 1);
				}
			}
			
			else
				SetCurrentState("Idle");
		}
		
	}
	else if (temp->GetStateName() == "Scout")
	{
		if (DeathCheck(RC))
		{
			RC->InternalTimer = 0;
			SetCurrentState("Dead");
		}
		RC->InternalTimer += dt;
		// Search for enemies
		if (RC->InternalTimer > RC->WaitTime)
		{
			RC->WaitTime = Math::RandFloatMinMax(1.f, 2.f);
			RandomizeMovement(RC);
		}

		if ((GlobalPlayer::Instance().GetCurrentPosition() - RC->GetPosition()).LengthSquared() < RC->DetectionRadius * RC->DetectionRadius)
		{
			SetCurrentState("Attack");
			RC->InternalTimer = 0;
		}
	}
	else if (temp->GetStateName() == "Dead")
	{
		RC->InternalTimer += dt;
		//RC->Static = true;
		RC->HealthPoints = Math::Clamp(RC->HealthPoints, 0, RC->MaxHealthPoints);
		if (RC->InternalTimer > 5.f)
		{
			if (RC->Active)
				EventSystem::Instance().CurrentCharacterCount.find("Robot")->second -= 1;
			RC->Active = false;
		}
	}
}

void RobotStateManager::Exit()
{
}

bool RobotStateManager::DeathCheck(CharacterEntity* RC)
{
	// Death Check
	if (RC->HealthPoints <= 0)
	{
		return true;
	}
	else return false;
}

bool RobotStateManager::WithinRange(CharacterEntity* C1, CharacterEntity* C2)
{
	float DistanceBetween = (C2->GetPosition() - C1->GetPosition()).LengthSquared();
	if (DistanceBetween < C1->DetectionRadius * C1->DetectionRadius)
	{
		return true;
	}
	return false;
}


void RobotStateManager::RandomizeMovement(CharacterEntity* C)
{
	RobotCharacter* RC = (RobotCharacter*)C;
	int Degree = Math::RandIntMinMax(-360, 360);
	float SpeedMultiplier = Math::RandFloatMinMax(1.f, 1.5f);
	RC->SetVelocity(RC->GetVelocity() + Vector3((float)sin(Degree),0, (float)cos(Degree)).Normalize() * Math::RandFloatMinMax((float)-RC->WalkSpeed * SpeedMultiplier, (float)RC->WalkSpeed)* SpeedMultiplier);
	RC->InternalTimer = 0;
}