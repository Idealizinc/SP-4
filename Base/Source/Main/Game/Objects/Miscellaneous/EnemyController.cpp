#include "EnemyController.h"
#include "../../../Engine/State/StateList.h"

EnemyController::EnemyController()
{
	SetMaxHistory(10);
	SetCurrentState("");
}

EnemyController::~EnemyController()
{

}

void EnemyController::Init()
{
	StateList::Instance().CreateState("Standby");
	StateList::Instance().CreateState("Spawning");
	StateList::Instance().CreateState("Moving");
	StateList::Instance().CreateState("End");

	AttachState("Standby");
	AttachState("Spawning");
	AttachState("Moving");
	AttachState("End");

	SetCurrentState("Idle");
}

void EnemyController::Update(const float& dt)
{
	State* temp = GetCurrentState();

	if (temp->GetStateName() == "Standby")
	{

	}
	else if (temp->GetStateName() == "Spawning")
	{
		//create a new enemy and spawn it at spawn
	}
	else if (temp->GetStateName() == "Moving")
	{
		//rand a enemy and pop from enemymoveVector
	}
	else if (temp->GetStateName() == "End")
	{

	}
}

void EnemyController::Exit()
{

}