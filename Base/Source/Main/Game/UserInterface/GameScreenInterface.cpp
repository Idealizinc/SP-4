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

	MultipleUnitLayer = CreateNewInterfaceLayer("multipleLayer", 0,0);

	PlayerTurn = true;
	SurrenderOn = false;
	UnitSelected = nullptr;
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

void GameScreenInterface::toggleSurrender()
{
	if (SurrenderLayer->Active == 1)
	{
		if (SurrenderOn == 1)
		{
			SurrenderLayer->SwapOriginalWithTarget();
			SurrenderOn = 0;
		}
		SurrenderLayer->Visible = 0;
		SurrenderLayer->Active = 0;
		SurrenderButton->Visible = 0;
		SurrenderButton->Active = 0;
	}
	else
	{
		SurrenderLayer->Visible = 1;
		SurrenderLayer->Active = 1;
		SurrenderButton->Visible = 1;
		SurrenderButton->Active = 1;
	}
	
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
		
		if ((SurrenderButton->DetectUserInput(MousePos, CashDisplayLayer->GetPosition()) || SurrenderNoButton->DetectUserInput(MousePos, SurrenderLayer->GetPosition())) && SurrenderLayer->Active == 1)
		{
			SurrenderLayer->SwapOriginalWithTarget();
			if (SurrenderOn == 1)
			{
				SurrenderOn = 0;
			}
			else
			{
				SurrenderOn = 1;
			}
		}
		if (SurrenderYesButton->DetectUserInput(MousePos, SurrenderLayer->GetPosition()) && SurrenderLayer->Active == 1)
		{

			GameLogicSystem::Instance().UnitInterface->InterfaceExit();
			SceneSystem::Instance().SwitchScene("MainMenuScene");
			SurrenderLayer->SwapOriginalWithTarget();
			SurrenderOn = 0;
			if (GameLogicSystem::Instance().InternalPlayerSystem->InternalUnitContainer.size() > 0)
			for (auto it : GameLogicSystem::Instance().InternalPlayerSystem->InternalUnitContainer)
			{
				for (std::vector<UnitPiece*>::iterator it2 = it->TargetNode->TerrainTile->PlayerUnitList.begin(); it2 != it->TargetNode->TerrainTile->PlayerUnitList.end();)
				{
					if (it == *it2)
					{
						it->TargetNode->TerrainTile->PlayerUnitList.erase(it2);
						break;
					}
					++it2;
				}
				it->Exit();
				delete it;
			}
			GameLogicSystem::Instance().InternalPlayerSystem->InternalUnitContainer.clear();

			if (GameLogicSystem::Instance().InternalEnemySystem->InternalEnemyContainer.size() > 0)
			for (auto it : GameLogicSystem::Instance().InternalEnemySystem->InternalEnemyContainer)
			{
				for (std::vector<UnitPiece*>::iterator it2 = it->TargetNode->TerrainTile->EnemyUnitList.begin(); it2 != it->TargetNode->TerrainTile->EnemyUnitList.end();)
				{
					if (it == *it2)
					{
						it->TargetNode->TerrainTile->EnemyUnitList.erase(it2);
						break;
					}
					++it2;
				}
				it->Exit();
				delete it;
			}
			GameLogicSystem::Instance().InternalEnemySystem->InternalEnemyContainer.clear();
		}
		if (MultipleUnitElements.size() > 0)
		{
			for (auto it : MultipleUnitElements)
			{
				if (it.second->DetectUserInput(MousePos, MultipleUnitLayer->GetPosition()))
				{
					UnitSelected = it.first;
					MultipleUnitElements.clear();
					if (MultipleUnitLayer)
					{
						for (std::vector<InterfaceLayer*>::iterator it = InternalLayerContainer.begin(); it != InternalLayerContainer.end(); ++it)
							if ((*it)->GetEntityID() == MultipleUnitLayer->GetEntityID())
							{
								InternalLayerContainer.erase(it);
								break;
							}
						MultipleUnitLayer->Exit();
						delete MultipleUnitLayer;
						MultipleUnitLayer = nullptr;
					}
					MultipleUnitLayer = CreateNewInterfaceLayer("multipleLayer", 0, 0);
					break;
				}
			}
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

void GameScreenInterface::MultipleUnitSelect(std::vector<UnitPiece*> Selections)
{
	Vector3 HalfDimension = SceneSystem::Instance().cSS_InputManager->ScreenCenter;
	//std::map<std::string, unsigned short> currentUnitMap = UnitSpawner->returnRecordedUnitMap();
	int IconCount = Selections.size();

	Vector3 lowestPt(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.2f, HalfDimension.y);
	Vector3 highestPt(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 1.1f, HalfDimension.y);

	float DisplayWidth = (highestPt.x - lowestPt.x);
	//float DisplayHeight = (highestPt.y - lowestPt.y);

	float IconSpaceWidth = (DisplayWidth / IconCount) / 2;
	//float IconSpaceHeight = (DisplayHeight / IconCount) / 2;

	InterfaceElement* tempElement = nullptr;

	int count = 1;
	/*for (auto it2 : Selections)
	{
		auto it = currentUnitMap.find(it2.first);
		auto it3 = IconCounterMap.find(it2.first);
		if (it == currentUnitMap.end())
		{
			it2.second->SetTargetPosition(it2.second->GetOriginalPosition());
			it3->second->SetTargetPosition(it3->second->GetOriginalPosition());
		}
		else
		{
			it2.second->SetTargetPosition(Vector3(lowestPt.x + IconSpaceWidth * (count), highestPt.y));
			it3->second->SetTargetPosition(Vector3(lowestPt.x + IconSpaceWidth * (count), highestPt.y - it2.second->GetDimensions().y * 0.75f));
			it3->second->SetText("x" + std::to_string(it->second));
			it3->second->SetTextColor(0);
			++count;
		}
	}*/
	int i = 0;
	for (auto it : Selections)
	{
		tempElement = MultipleUnitLayer->CreateNewInterfaceElement(std::to_string(i), "quad", Vector3(lowestPt.x + IconSpaceWidth * (count), highestPt.y), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
		tempElement->SetTargetPosition(Vector3(lowestPt.x + IconSpaceWidth * (count), highestPt.y));
		tempElement->SetText(Selections[i]->InternalBattalionList.begin()->first);
		tempElement->SetTextColor(0);
		MultipleUnitElements.insert(std::pair<UnitPiece*, InterfaceElement*>(Selections[i], tempElement));
		++i;
		++count;
	}
}