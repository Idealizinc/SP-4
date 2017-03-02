#include "CharacterStateManager.h"
#include "BattleScreenCharacter.h"
#include "../../../../Engine/State/StateList.h"
#include "../../../../Engine/System/SceneSystem.h"
#include "../../../Systems/GameLogicSystem.h"
//#include "../../Engine/System/LuaInterface.h"

float DistanceBetweenSquared(const Vector3& A, const Vector3& B)
{
	return (A - B).LengthSquared();
}

CharacterStateManager::CharacterStateManager()
{
	SetMaxHistory(10);
	SetCurrentState("");
}

CharacterStateManager::~CharacterStateManager()
{
}

void CharacterStateManager::Init()
{
	// Create The States
	StateList::Instance().CreateState("Idle");
	StateList::Instance().CreateState("Attack");
	StateList::Instance().CreateState("Scout");
	StateList::Instance().CreateState("Dead");

	TargetNode = nullptr;

	// Set used states
	AttachState("Idle");
	AttachState("Attack");
	AttachState("Scout");
	AttachState("Dead");

	// Remember to set current state or nothing will run
	SetCurrentState("Idle");

	// Setup the internal wait timer
	BattleScreenCharacter* Character = dynamic_cast<BattleScreenCharacter*>(InternalCharacter);
	// Push back for the number of timers that you need.
	Character->InternalTimerVector.push_back(0.f);
	Character->InternalTimerVector.push_back(0.f);
}

void CharacterStateManager::Update(const float& dt)
{
	if (GetCurrentState()->GetStateName() == "Idle")
		Idle(dt);
	else if (GetCurrentState()->GetStateName() == "Scout")
		Scout(dt);
	else if (GetCurrentState()->GetStateName() == "Attack")
		Attack(dt);
	else if (GetCurrentState()->GetStateName() == "Dead")
		Dead(dt);
	//std::cout << InternalCharacter->GetEntityID() << GetCurrentState()->GetStateName() << std::endl;
}

void CharacterStateManager::Exit()
{
	TargetNode = nullptr;
}

void CharacterStateManager::Idle(const float& dt)
{
	BattleScreenCharacter* Character = dynamic_cast<BattleScreenCharacter*>(InternalCharacter);
	Character->InternalTimerVector[0] += dt;

	// Death Check
	if (DeathCheck(Character))
	{
		Character->InternalTimerVector[0] = 0;
		SetCurrentState("Dead");
	}
	// Scout Timer
	else if (Character->InternalTimerVector[0] > Character->WaitTime)
		SetCurrentState("Scout");
}

void CharacterStateManager::Scout(const float& dt)
{
	BattleScreenCharacter* Character = dynamic_cast<BattleScreenCharacter*>(InternalCharacter);
	Character->InternalTimerVector[0] += dt;
	Character->InternalTimerVector[1] += dt;

	// Self Deathcheck
	if (DeathCheck(Character))
	{
		Character->InternalTimerVector[0] = 0;
		SetCurrentState("Dead");
	}
	else if (Character->TargetEnemy)
	{
		// Target DeathCheck
		if (DeathCheck(Character->TargetEnemy))
		{
			Character->TargetEnemy = nullptr;
			TargetNode = nullptr;
		}
		else {
			// If the enemy is within sights I will forget about nodes and attack him directly
			Vector3 DirectionToTargetEnemy = (Character->TargetEnemy->GetPosition() - Character->GetPosition());
			DirectionToTargetEnemy.y = 0;
			float D = DirectionToTargetEnemy.LengthSquared();
			if (DirectionToTargetEnemy.LengthSquared() < Character->DetectionRadius * Character->DetectionRadius * 2.f)
			{
				SetCurrentState("Attack");
				Character->InternalTimerVector[0] = 0;
			}
			// I don't have a node to go to or I'm already at my target, so I need to find one
			else if (TargetNode == nullptr || (DistanceBetweenSquared(TargetNode->GetPosition(), Character->GetPosition()) < Character->DetectionRadius * Character->DetectionRadius * 0.05f))
			{
				FindTargetNode();
			}
			else if (Character->InternalTimerVector[1] > 5.f)
			{
				Character->InternalTimerVector[1] = 0.f;
				FindTargetNode();
			}
			
			// Move the character accordingly towards his target node
			if (Character->InternalTimerVector[0] > Character->WaitTime)
			{
				Character->WaitTime = Math::RandFloatMinMax(0.25f, 0.5f);
				Character->InternalTimerVector[0] = 0;
				if (TargetNode)
				{
					float SpeedMultiplier = Math::RandFloatMinMax(1.f, 2.f);
					Vector3 Velocity = Vector3(Math::RandFloatMinMax(-5.f, 5.f), 0, Math::RandFloatMinMax(-5.f, 5.f)) + Character->GetVelocity() + (TargetNode->GetPosition() - Character->GetPosition()).Normalize() * Character->WalkSpeed * SpeedMultiplier;
					Velocity.y = 0;
					Character->SetVelocity(Velocity);
				}
				else RandomizeMovement(Character);
			}
			else if (Character->InternalTimerVector[1] > 3.f)
			{
				Character->InternalTimerVector[1] = 0.f;
				Character->TargetEnemy = FindClosestEnemy();
			}
		}
	}
	else {
		// I will need to try to find another target
		Character->TargetEnemy = FindClosestEnemy();
	}
}

void CharacterStateManager::Attack(const float& dt)
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
		float Distance = (DirVec).LengthSquared();
		if (Distance < Character->DetectionRadius * Character->DetectionRadius)
		{
			// Rush the target
			//if (!DirVec.IsZero())
			//	DirVec.Normalize() *= Character->WalkSpeed;
			//DirVec.y = 0;
			//Character->SetVelocity(DirVec * Character->WalkSpeed);
			//Character->WaitTime = Math::RandFloatMinMax(1.f, 3.f);
			//if (Distance <= Character->GetDimensions().x * Character->GetDimensions().x)
			//{
			//	
			//}
		}
		else
		{
			SetCurrentState("Idle");
			Character->TargetEnemy = nullptr;
			TargetNode = nullptr;
		}
	}
}

void CharacterStateManager::Dead(const float& dt)
{
	BattleScreenCharacter* Character = dynamic_cast<BattleScreenCharacter*>(InternalCharacter);
	Character->InternalTimerVector[0] += dt;
	//Character->Static = true;
	Character->HealthPoints = Math::Clamp(Character->HealthPoints, 0, Character->MaxHealthPoints);
	if (Character->InternalTimerVector[0] > 2.f)
	{
		Character->Active = false;
	}
}

bool CharacterStateManager::DeathCheck(CharacterEntity* Character)
{
	// Death Check
	if (Character->HealthPoints <= 0)
	{
		return true;
	}
	else return false;
}

bool CharacterStateManager::WithinRange(CharacterEntity* C1, CharacterEntity* C2)
{
	float DistanceBetween = DistanceBetweenSquared(C2->GetPosition(), C1->GetPosition()); //= LuaInterface::Instance().GetVector3DistanceBetweenSquared(C2->GetPosition(), C1->GetPosition());
	
	if (DistanceBetween < C1->DetectionRadius * C1->DetectionRadius)
	{
		return true;
	}
	return false;
}


void CharacterStateManager::RandomizeMovement(CharacterEntity* C)
{
	BattleScreenCharacter* Character = (BattleScreenCharacter*)C;
	int Degree = Math::RandIntMinMax(-360, 360);
	float SpeedMultiplier = Math::RandFloatMinMax(1.f, 1.5f);
	Character->SetVelocity(Character->GetVelocity() + Vector3((float)sin(Degree), 0, (float)cos(Degree)).Normalize() * Math::RandFloatMinMax((float)-Character->WalkSpeed * SpeedMultiplier, (float)Character->WalkSpeed)* SpeedMultiplier);
	Character->InternalTimerVector[0] = 0;
}

BaseObject* CharacterStateManager::FindTargetNode()
{
	BattleScreenCharacter* Character = (BattleScreenCharacter*)InternalCharacter;
	BaseObject* LastSelectedNode = nullptr;
	bool NodeFound = false;
	if (Character->TargetEnemy != nullptr)
	{
		Vector3 DirectionToTarget = (Character->TargetEnemy->GetPosition() - Character->GetPosition());
		float SelectionDistance = Character->DetectionRadius * Character->DetectionRadius * 0.5;
		std::vector<GameObject*> NodeList = SceneSystem::Instance().GetCurrentScene().ScenePartition->WaypointList;
		for (std::vector<GameObject*>::iterator it = NodeList.begin(); it != NodeList.end(); ++it)
		{
			GameObject* GO = *it;
			if (GO != nullptr)
			// Make sure it's not my current node
			if (GO != TargetNode)
			{
				Vector3 DirectionNormalized = DirectionToTarget.Normalized();
				float DistanceApart = DistanceBetweenSquared((*it)->GetPosition(), Character->GetPosition());
				if (DistanceApart < SelectionDistance)
				if (DirectionNormalized.Dot(((*it)->GetPosition() - Character->GetPosition()).Normalize()) > 0) // Check for acute angle
				{
					// Give him some probability in selecting the node
					if (Math::RandIntMinMax(0, 100) > 70)
					{
						NodeFound = true;
						TargetNode = *it;
						break;
					}
					else LastSelectedNode = *it;
				}
			}
		}
	}
	if (NodeFound == false)
	{
		if (LastSelectedNode)
			TargetNode = LastSelectedNode;
		else TargetNode = nullptr;
	}
	return TargetNode;
}

CharacterEntity* CharacterStateManager::FindClosestEnemy()
{
	BattleScreenCharacter* Character = dynamic_cast<BattleScreenCharacter*>(InternalCharacter);
	std::vector<CharacterEntity*> Container;
	if (Character->IsPlayerCharacter)
		Container = GameLogicSystem::Instance().InternalBattleSystem->GetEnemyCharacterList();
	else
		Container = GameLogicSystem::Instance().InternalBattleSystem->GetPlayerCharacterList();

	if (Container.size() > 0/* && Character->TargetEnemy == nullptr*/)
	{
		// Radius Check
		// Find a nearby TargetEnemy, Iterate char list, set TargetEnemy found, select closest one
		float DistanceBetweenSquared = FLT_MAX;
		std::vector<CharacterEntity*> ViableTargets;
		for (std::vector<CharacterEntity*>::iterator it = Container.begin(); it != Container.end(); ++it)
		{
			if ((*it)->Active && !DeathCheck(*it))
			{
				//if (WithinRange(Character, *it)) Let us grant characters infinite detection radius
				{
					// Check if target is within FOV by comparing the current angle against the angle between the two characters
					Vector3 DirVec = Character->GetPosition() - (*it)->GetPosition();
					//int AngleBetween = ((int)Math::RadianToDegree(atan2(-DirVec.x, DirVec.z))) % 360;
					//int CurrentAngle = (int)Character->GetRotationAngle() % 360;
					//int FOVAddition = 90 - (int)(Character->FieldOfView * 0.5f);
					//if (!(AngleBetween < CurrentAngle + 90 + FOVAddition && AngleBetween > CurrentAngle - 90 - FOVAddition))
					{
						//return *it;
						float DistanceCheck = DirVec.LengthSquared();
						// Decrement the DistanceBetween if a lower value has been found
						if (DistanceBetweenSquared > DistanceCheck)
						{
							DistanceBetweenSquared = DistanceCheck;
							ViableTargets.push_back(*it);
						}
					}
				}
			}
		}
		if (ViableTargets.size() > 0)
		{
			while (ViableTargets.size() > MaxViableTargets)
			{
				ViableTargets.erase(ViableTargets.begin());
			}
			return ViableTargets[Math::RandIntMinMax(0, ViableTargets.size() - 1)];
		}
	}
	return nullptr;
}