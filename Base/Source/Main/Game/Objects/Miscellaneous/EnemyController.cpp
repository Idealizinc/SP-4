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
		if (turnCheck() == true)
		{
			SetCurrentState("Spawning");
		}
	}
	else if (temp->GetStateName() == "Spawning")
	{
		//insert unitspawn with enemybase position code here (can be implemented by function)
		SetCurrentState("Moving");
	}
	else if (temp->GetStateName() == "Moving")
	{
		//rand a enemy in the vector containing enemy units in the field and pop the front most slot from the Enemy targetNode vector of the selected enemy (can be implemented as function)
		SetCurrentState("End");
	}
	else if (temp->GetStateName() == "End")
	{
		SetCurrentState("Standby");
		turnSet();
	}
}

void EnemyController::Exit()
{

}

bool EnemyController::turnCheck(/*CharacterEntity* test*/)//this checks if its the enemy turn yet
{
	/*
	if (test->playerTurn == false)
	{
		return true;
	}
	else
	{
		return false;
	}
	*/
	return false;
}

void EnemyController::turnSet()//similar function should be in playercontroller, this functions gives the turn and thus control back to the player (or can just directly edit the variable i guess)
{
	//test->playerTurn = true;
}