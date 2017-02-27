#include "GameScreenInterface.h"
#include "../Systems/GameLogicSystem.h"


GameScreenInterface::GameScreenInterface()
{
	Init();
}

GameScreenInterface::~GameScreenInterface()
{
	Exit();
}

void GameScreenInterface::Init()
{
	TurnPopupTimer = 0;
	CashDisplayTimer = 0.f;
	CashEditAmt = 0;
	TurnPopup = CreateNewInterfaceLayer("TurnLayer", 0, 0);
	PlayerTurnPopup = TurnPopup->CreateNewInterfaceElement("PlayerPopUp", "quad2", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	PlayerTurnPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.9f, 0));
	PlayerTurnPopup->SetText("Player Turn");
	PlayerTurnPopup->SetTextColor(0);

	EnemyTurnPopup = TurnPopup->CreateNewInterfaceElement("EnemyPopUp", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	EnemyTurnPopup->SetTargetPosition(EnemyTurnPopup->GetOriginalPosition());
	EnemyTurnPopup->SetText("Enemy Turn");
	EnemyTurnPopup->SetTextColor(0);

	CashDisplayLayer = CreateNewInterfaceLayer("CashLayer", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.05f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0), 0);
	CashDisplayLayer->SetTargetPosition(CashDisplayLayer->GetOriginalPosition());
	CashTotalDisplay = CashDisplayLayer->CreateNewInterfaceElement("CashTotal", "quad", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	CashTotalDisplay->SetTextColor(0);

	CashSubstractDisplay = CashDisplayLayer->CreateNewInterfaceElement("CashReduce", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.0f, -(SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.09f)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	CashSubstractDisplay->SetTargetPosition(0);
	CashSubstractDisplay->SetTextColor(0);

	SurrenderButton = CashDisplayLayer->CreateNewInterfaceElement("WhiteFlag", "quad", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.88f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.02f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	SurrenderButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.88f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.02f, 0));
	SurrenderButton->SetText("Surrender");
	SurrenderButton->SetTextColor(0);

	PlayerTurn = true;

	InitSurrender();
}

void GameScreenInterface::InitSurrender()
{
	SurrenderLayer = CreateNewInterfaceLayer("SurrenderLayer", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(1), 0), 0);
	SurrenderLayer->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));
	
	SurrenderBackground = SurrenderLayer->CreateNewInterfaceElement("SurrenderBackground", "quad", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	SurrenderBackground->SetTargetPosition(0);
	SurrenderBackground->SetText("Are You Sure?");
	SurrenderBackground->SetTextColor(0);

	SurrenderYesButton = SurrenderLayer->CreateNewInterfaceElement("SurrenderYes", "quad", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.05f), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	SurrenderYesButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.05f), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0));
	SurrenderYesButton->SetText("Yeh");
	SurrenderYesButton->SetTextColor(0);

	SurrenderNoButton = SurrenderLayer->CreateNewInterfaceElement("SurrenderNo", "quad", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * (0.05f), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	SurrenderNoButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * (0.05f), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0));
	SurrenderNoButton->SetText("Nah");
	SurrenderNoButton->SetTextColor(0);

	SurrenderLayer->SwapOriginalWithTarget();
}

void GameScreenInterface::Update(const float& dt)
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active)
		{
			if (it->GetEntityID() == "CashLayer" && CashDisplayTimer > 0)
			{
				continue;
			}
			else
			{
				it->Update(dt * 10);
				if (it->GetEntityID() == "CashLayer" && CashEditAmt != 0)
				{
					GameLogicSystem::Instance().InternalPlayerSystem->SetCash(GameLogicSystem::Instance().InternalPlayerSystem->GetCash() + CashEditAmt);
					CashEditAmt = 0;
				}
			}
		}
	}
	PopUpDelay(dt);
	CashTotalDisplay->SetText("$" + std::to_string(GameLogicSystem::Instance().InternalPlayerSystem->GetCash()));
	if (PlayerTurn == 1 && GameLogicSystem::Instance().GetCurrentState()->GetStateName() == "Enemy's Turn" || PlayerTurn == 0 && GameLogicSystem::Instance().GetCurrentState()->GetStateName() == "Player's Turn")
	{
		PopUpReset();
	}
	if (CashDisplayTimer > 0)
	{
		CashDisplayTimer -= dt;
	}

	if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_DOWN)
	{
		Vector3 MousePos = SceneSystem::Instance().cSS_InputManager->GetMousePosition();
		if (SurrenderButton->DetectUserInput(MousePos, CashDisplayLayer->GetPosition()) || SurrenderNoButton->DetectUserInput(MousePos, SurrenderLayer->GetPosition()))
		{
			SurrenderLayer->SwapOriginalWithTarget();
		}
		if (SurrenderYesButton->DetectUserInput(MousePos, SurrenderLayer->GetPosition()))
		{

			GameLogicSystem::Instance().UnitInterface->InterfaceExit();
			SceneSystem::Instance().SwitchScene("MainMenuScene");
			SurrenderLayer->SwapOriginalWithTarget();
			//Exit();
		}
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

void GameScreenInterface::ShowCashReduction(int amt)
{
	CashSubstractDisplay->SetText("-$" + std::to_string(amt));
	CashSubstractDisplay->ResetToOriginal();
	CashDisplayTimer = 1;
	CashEditAmt -= amt;
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