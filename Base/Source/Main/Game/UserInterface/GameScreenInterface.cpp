#include "GameScreenInterface.h"

GameScreenInterface::GameScreenInterface()
{
	Init();
}

GameScreenInterface::~GameScreenInterface()
{

}

void GameScreenInterface::Init()
{
	TurnPopupTimer = 0;
	TurnPopup = CreateNewInterfaceLayer("TurnLayer", 0, 0);
	PlayerTurnPopup = TurnPopup->CreateNewInterfaceElement("PlayerPopUp", "quad2", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	PlayerTurnPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.9f, 0));
	PlayerTurnPopup->SetText("Player Turn");
	PlayerTurnPopup->SetTextColor(0);

	EnemyTurnPopup = TurnPopup->CreateNewInterfaceElement("EnemyPopUp", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	EnemyTurnPopup->SetTargetPosition(EnemyTurnPopup->GetOriginalPosition());
	EnemyTurnPopup->SetText("Enemy Turn");
	EnemyTurnPopup->SetTextColor(0);
	PlayerTurn = true;
}

void GameScreenInterface::Update(const float& dt)
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active)
			it->Update(dt * 10);
	}
	PopUpDelay(dt);

	if (PlayerTurn == 1 && GameLogicSystem::Instance().GetCurrentState()->GetStateName() == "Enemy's Turn" || PlayerTurn == 0 && GameLogicSystem::Instance().GetCurrentState()->GetStateName() == "Player's Turn")
	{
		PopUpReset();
	}
}

void GameScreenInterface::Render()
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active && it->Visible)
			it->Render();
	}
}

void GameScreenInterface::Exit()
{
	for (auto it : InternalLayerContainer)
	{
		it->Exit();
		delete it;
	}
	InternalLayerContainer.clear();
}

InterfaceLayer* GameScreenInterface::CreateNewInterfaceLayer(const std::string& LayerID, const Vector3& OriginalPosition, const Vector3& TargetPosition)
{
	InterfaceLayer* NewL = new InterfaceLayer();
	NewL->SetEntityID(LayerID);
	NewL->SetOriginalPosition(OriginalPosition);
	NewL->SetTargetPosition(TargetPosition);
	InternalLayerContainer.push_back(NewL);
	return NewL;
}

void GameScreenInterface::PopUpDelay(const float& dt)
{
	if (PlayerTurnPopup->GetPosition().y <= PlayerTurnPopup->GetTargetPosition().y + 3 && PopUpDone == false && PlayerTurn == 1)
	{
		if (PoppedUp == false)
		{
			TurnPopupTimer = 1;
			PoppedUp = true;

		}
		else
		{
			if (TurnPopupTimer > 0)
			{
				TurnPopupTimer -= dt;
			}
			else
			{
				PopUpDone = true;
				PlayerTurnPopup->SetTargetPosition(PlayerTurnPopup->GetOriginalPosition());
			}
		}
	}
	else if (EnemyTurnPopup->GetPosition().y <= EnemyTurnPopup->GetTargetPosition().y + 3 && PopUpDone == false && PlayerTurn == 0)
	{
		if (PoppedUp == false)
		{
			TurnPopupTimer = 1;
			PoppedUp = true;

		}
		else
		{
			if (TurnPopupTimer > 0)
			{
				TurnPopupTimer -= dt;
			}
			else
			{
				PopUpDone = true;
				EnemyTurnPopup->SetTargetPosition(EnemyTurnPopup->GetOriginalPosition());
			}
		}
	}

	if (PlayerTurn == 1 && EnemyTurnPopup->GetPosition() != EnemyTurnPopup->GetOriginalPosition())
	{
		EnemyTurnPopup->ResetToOriginal();
	}
	else if (PlayerTurn == 0 && PlayerTurnPopup->GetPosition() != PlayerTurnPopup->GetOriginalPosition())
	{
		PlayerTurnPopup->ResetToOriginal();
	}
}

void GameScreenInterface::PopUpReset()
{
	PopUpDone = false;
	PoppedUp = false;
	if (PlayerTurn == 1)
	{
		PlayerTurn = 0;
		EnemyTurnPopup->SetTargetPosition(CenterTarget);
	}
	else
	{
		PlayerTurn = 1;
		PlayerTurnPopup->SetTargetPosition(CenterTarget);
	}
}