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
	PlayerTurnPopup = TurnPopup->CreateNewInterfaceElement("PlayerPopUp", "GreenBar", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	PlayerTurnPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.9f, 0));
	PlayerTurnPopup->SetText("Player Turn");
	PlayerTurnPopup->SetTextColor(0);

	EnemyTurnPopup = TurnPopup->CreateNewInterfaceElement("EnemyPopUp", "RedBar", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	EnemyTurnPopup->SetTargetPosition(EnemyTurnPopup->GetOriginalPosition());
	EnemyTurnPopup->SetText("Enemy Turn");
	EnemyTurnPopup->SetTextColor(0);\

	MenuLayer = CreateNewInterfaceLayer("MenuLayer", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.9f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0)));
	//MenuLayer->SwapOriginalWithTarget();
	MenuFrame = MenuLayer->CreateNewInterfaceElement("MenuFrame", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.03f, 0, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.4f, 0));
	MenuFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.03f, 0, 0));

	SurrenderButton = MenuLayer->CreateNewInterfaceElement("WhiteFlag", "WhiteFlag", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.05f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.08f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.16f, 0));
	SurrenderButton->SetTargetPosition(SurrenderButton->GetOriginalPosition());
	SurrenderButton->SetText("Surrender");
	SurrenderButton->SetTextColor(0);


	DeployButton = MenuLayer->CreateNewInterfaceElement("DeployUI", "M_Crusader", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.05f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.0f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.18f, 0));
	DeployButton->SetTargetPosition(DeployButton->GetOriginalPosition());
	DeployButton->SetText("Deploy");
	DeployButton->SetTextColor(0);

	SkipButton = MenuLayer->CreateNewInterfaceElement("SkipButton", "SkipImage", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.05f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.1f, 0.0f), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.08f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.16f, 0));
	SkipButton->SetTargetPosition(SkipButton->GetOriginalPosition());
	SkipButton->SetText("Skip");
	SkipButton->SetTextColor(0);

	MenuButton = MenuLayer->CreateNewInterfaceElement("MenuUI", "LoadTexture", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.04f),0, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.16f, 0));
	MenuButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.04f),0, 0));
	MenuButton->SetText("Menu");
	MenuButton->SetTextColor(0);

	//Ingame Frame
	IngameLayer = CreateNewInterfaceLayer("IngameLayer", 0 ,0);
	
	GameFrame = IngameLayer->CreateNewInterfaceElement("GameFrame", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0));
	GameFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));

	//Cash
	CashDisplayLayer = CreateNewInterfaceLayer("CashLayer", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.08f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.94f, 0), 0);
	CashDisplayLayer->SetTargetPosition(CashDisplayLayer->GetOriginalPosition());

	CashTotalDisplay = CashDisplayLayer->CreateNewInterfaceElement("CashTotal", "LoadTexture", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	CashTotalDisplay->SetTextColor(0);

	CashSubstractFrame = CashDisplayLayer->CreateNewInterfaceElement("CashReduceFrame", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.0f, -(SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.09f)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	CashSubstractFrame->SetTargetPosition(0);

	CashSubstractDisplay = CashDisplayLayer->CreateNewInterfaceElement("CashReduce", "RedBar", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.0f, -(SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.09f)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	CashSubstractDisplay->SetTargetPosition(0);
	CashSubstractDisplay->SetTextColor(0);


	MultipleUnitLayer = CreateNewInterfaceLayer("multipleLayer", 0,0);

	PlayerTurn = true;
	SurrenderOn = false;
	UnitSelected = nullptr;
	MenuOpen = false;
	MultipleUnitUI = false;
	InitSurrender();
}

void GameScreenInterface::InitSurrender()
{
	SurrenderLayer = CreateNewInterfaceLayer("SurrenderLayer", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(1), 0), 0);
	SurrenderLayer->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));
	
	SurrenderBackground = SurrenderLayer->CreateNewInterfaceElement("SurrenderBackground", "LoadTexture", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	SurrenderBackground->SetTargetPosition(0);
	SurrenderBackground->SetText("Are You Sure?");
	SurrenderBackground->SetTextColor(0);

	SurrenderYesButton = SurrenderLayer->CreateNewInterfaceElement("SurrenderYes", "LoadTexture", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.05f), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	SurrenderYesButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.05f), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0));
	SurrenderYesButton->SetText("Yeh");
	SurrenderYesButton->SetTextColor(0);

	SurrenderNoButton = SurrenderLayer->CreateNewInterfaceElement("SurrenderNo", "LoadTexture", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * (0.05f), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	SurrenderNoButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * (0.05f), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0));
	SurrenderNoButton->SetText("Nah");
	SurrenderNoButton->SetTextColor(0);

	SurrenderLayer->SwapOriginalWithTarget();
	SurrenderLayer->Active = false;
	SurrenderCheck = false;
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
		if (MenuOpen == 1)
		{
			MenuOpen = 0;
			MenuLayer->SwapOriginalWithTarget();
		}
		SurrenderLayer->Visible = 0;
	SurrenderLayer->Active = 0;
	/*SurrenderButton->Visible = 0;
	SurrenderButton->Active = 0;
	DeployButton->Visible = 0;
	DeployButton->Active = 0;*/
	MenuLayer->Visible = 0;
	MenuLayer->Active = 0;
	}
	else
	{
		SurrenderLayer->Visible = 1;
		SurrenderLayer->Active = 1;
		/*SurrenderButton->Visible = 1;
		SurrenderButton->Active = 1;
		DeployButton->Visible = 1;
		DeployButton->Active = 1;*/
		MenuLayer->Visible = 1;
		MenuLayer->Active = 1;
	}

}

void GameScreenInterface::Update(const float& dt)
{
	if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
	{
		CashSubstractFrame->SetMesh("LivingFrame");
		GameFrame->SetMesh("IngameLivingFrame");
		MenuFrame->SetMesh("LivingMenuFrame");
	}
	else
	{
		CashSubstractFrame->SetMesh("UndeadFrame");
		GameFrame->SetMesh("IngameUndeadFrame");
		MenuFrame->SetMesh("UndeadMenuFrame");
	}

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

		if ((DeployButton->DetectUserInput(MousePos, MenuLayer->GetPosition())))
		{
			if (SceneSystem::Instance().GetCurrentScene().ScenePartition->PlayerBase->TerrainTile->PlayerUnitList.size() == 0)
			{
				GameLogicSystem::Instance().UnitInterface->OpenInterface();
				GameLogicSystem::Instance().GameInterface->toggleSurrender();
			}
			else
			{
				if (GameLogicSystem::Instance().UnitInterface->warningDisplayed3 == 0)
				{
					GameLogicSystem::Instance().UnitInterface->NoSlotError();
				}
			}
		}
		if (SurrenderButton->DetectUserInput(MousePos, MenuLayer->GetPosition()))
		{
			SurrenderLayer->SwapOriginalWithTarget();
			MenuLayer->SwapOriginalWithTarget();
			if (SurrenderOn == 1)
			{
				SurrenderOn = 0;
			}
			else
			{
				SurrenderOn = 1;
			}
		}
		if (SurrenderNoButton->DetectUserInput(MousePos, SurrenderLayer->GetPosition()) && SurrenderLayer->Active == 1)
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
			MenuOpen = false;
		}
			if (SurrenderYesButton->DetectUserInput(MousePos, SurrenderLayer->GetPosition()) && SurrenderLayer->Active == 1)
			{

				GameLogicSystem::Instance().UnitInterface->InterfaceExit();
				//SceneSystem::Instance().SwitchScene("MainMenuScene");
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
				//toggleSurrender();
				MenuOpen = false;
				GameLogicSystem::Instance().DetectSurrender();
			}

		if (MenuButton->DetectUserInput(MousePos, MenuLayer->GetPosition())&& MenuLayer->Active == 1)
		{
			MenuLayer->SwapOriginalWithTarget();
			if (MenuOpen == 1)
			{
				MenuOpen = 0;
			}
			else
			{
				MenuOpen = 1;
			}
		}
			if (MultipleUnitElements.size() > 0)
			{
				for (auto it : MultipleUnitElements)
				{
					if (MultipleUnitUI == 1)
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
					else
					{
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
			if (SkipButton->DetectUserInput(MousePos, MenuLayer->GetPosition()))
			{
				GameLogicSystem::Instance().InternalPlayerSystem->SkipTurn();
				toggleSurrender();
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
	CashSubstractFrame->ResetToOriginal();
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

void GameScreenInterface::MultipleUnitSelect(std::vector<UnitPiece*> Selections, bool interactive)
{
	MultipleUnitUI = interactive;
	Vector3 HalfDimension = SceneSystem::Instance().cSS_InputManager->ScreenCenter;
	//std::map<std::string, unsigned short> currentUnitMap = UnitSpawner->returnRecordedUnitMap();
	int IconCount = Selections.size();

	Vector3 lowestPt(HalfDimension.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f));
	Vector3 highestPt(HalfDimension.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (0.7f));

	float lowestPtX = SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.18f);
	float highestPtX = SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * (0.7f);

	float DisplayWidth = (highestPtX - lowestPtX);
	float DisplayHeight = (highestPt.y - lowestPt.y);

	float IconSpaceWidth = (DisplayWidth / 15) / 2;
	float IconSpaceHeight = (DisplayHeight / IconCount) / 2;

	InterfaceElement* tempElement = nullptr;
	if (interactive == 1)
	{
		tempElement = MultipleUnitLayer->CreateNewInterfaceElement("MULTitleUI", "LoadTexture", Vector3(lowestPt.x, highestPt.y * 1.1f), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
		tempElement->SetTargetPosition(Vector3(lowestPt.x, highestPt.y * 1.1f));
		tempElement->SetText("Pick Squad To Move");
		tempElement->SetTextColor(0);

		tempElement = MultipleUnitLayer->CreateNewInterfaceElement("MULTitleUI2", "LoadTexture", Vector3(lowestPt.x, highestPt.y * 1.1f), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
		tempElement->SetTargetPosition(Vector3(lowestPt.x, highestPt.y));
		tempElement->SetText("Click on desired squad");
		tempElement->SetTextColor(0);
	}
	else
	{
		tempElement = MultipleUnitLayer->CreateNewInterfaceElement("MULTitle", "LoadTexture", Vector3(lowestPt.x, highestPt.y), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
		tempElement->SetTargetPosition(Vector3(lowestPt.x, highestPt.y * 1.1f));
		tempElement->SetText("Squads in this Node");
		tempElement->SetTextColor(0);

		tempElement = MultipleUnitLayer->CreateNewInterfaceElement("MULTitle2", "LoadTexture", Vector3(lowestPt.x, highestPt.y), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
		tempElement->SetTargetPosition(Vector3(lowestPt.x, highestPt.y));
		tempElement->SetText("Click to dismiss");
		tempElement->SetTextColor(0);

	}

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
	/*int j = 0;
	float boxSize = 0;
	for (auto it : Selections)
	{
		if (boxSize == NULL || boxSize < Selections[j]->InternalBattalionList.size())
			boxSize = 0.25f * Selections[j]->InternalBattalionList.size();
		++j;
	}*/

	std::map<std::string, UnitType*>UnitMap;
	if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
		UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap;
	else
		UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap;
	
	for (auto it : Selections)
	{
		
		/*string UnitDisplay;
		for (auto it : Selections[i]->InternalBattalionList)
		{
			UnitDisplay += (it.first + " x" + std::to_string(it.second) + " ");
		}

		tempElement->SetText(UnitDisplay);
		tempElement->SetTextColor(0);*/
		float count2 = 1.f;
		for (auto it2 : Selections[i]->InternalBattalionList)
		{
			std::string MeshName;
			for (auto it3 : UnitMap)
			{
				if (it3.first == it2.first)
				{
					MeshName = it3.second->GetMeshName();
				}
			}
			InterfaceElement* tempElement2 = nullptr;
			tempElement2 = MultipleUnitLayer->CreateNewInterfaceElement(it2.first + std::to_string(i) + "Image", MeshName, Vector3((lowestPtX + IconSpaceWidth * count2) + HalfDimension.x, highestPt.y - IconSpaceHeight * (count)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
			tempElement2->SetTargetPosition(Vector3((lowestPtX + IconSpaceWidth * count2)+ HalfDimension.x, highestPt.y - IconSpaceHeight * (count)));

			tempElement2 = MultipleUnitLayer->CreateNewInterfaceElement(it2.first + std::to_string(i) + "Amount", "Transparent", Vector3((lowestPtX + IconSpaceWidth * (count2 + 1) + HalfDimension.x), highestPt.y - IconSpaceHeight * (count)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
			tempElement2->SetTargetPosition(Vector3((lowestPtX + IconSpaceWidth * (count2 + 1)+HalfDimension.x), highestPt.y - IconSpaceHeight * (count)));
			tempElement2->SetText("x" + std::to_string(it2.second));
			tempElement2->SetTextColor(0); 

			count2 += 2.5;

		}
		
		tempElement = MultipleUnitLayer->CreateNewInterfaceElement(std::to_string(i), "LoadTexture", Vector3(lowestPt.x, highestPt.y - IconSpaceHeight * (count)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
		tempElement->SetTargetPosition(Vector3(lowestPt.x, highestPt.y - IconSpaceHeight * (count)));

		MultipleUnitElements.insert(std::pair<UnitPiece*, InterfaceElement*>(Selections[i], tempElement));
		++i;
		++count;
	}
}

void GameScreenInterface::MultipleUnitSelectE(std::vector<UnitPiece*> Selections)
{
	MultipleUnitUI = false;
	Vector3 HalfDimension = SceneSystem::Instance().cSS_InputManager->ScreenCenter;
	//std::map<std::string, unsigned short> currentUnitMap = UnitSpawner->returnRecordedUnitMap();
	int IconCount = Selections.size();

	Vector3 lowestPt(HalfDimension.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f));
	Vector3 highestPt(HalfDimension.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (0.7f));

	float lowestPtX = SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.18f);
	float highestPtX = SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * (0.7f);

	float DisplayWidth = (highestPtX - lowestPtX);
	float DisplayHeight = (highestPt.y - lowestPt.y);

	float IconSpaceWidth = (DisplayWidth / 15) / 2;
	float IconSpaceHeight = (DisplayHeight / IconCount) / 2;

	InterfaceElement* tempElement = nullptr;
		tempElement = MultipleUnitLayer->CreateNewInterfaceElement("MULTitle", "LoadTexture", Vector3(lowestPt.x, highestPt.y), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
		tempElement->SetTargetPosition(Vector3(lowestPt.x, highestPt.y * 1.1f));
		tempElement->SetText("Squads in this Node");
		tempElement->SetTextColor(0);

		tempElement = MultipleUnitLayer->CreateNewInterfaceElement("MULTitle2", "LoadTexture", Vector3(lowestPt.x, highestPt.y), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
		tempElement->SetTargetPosition(Vector3(lowestPt.x, highestPt.y));
		tempElement->SetText("Click to dismiss");
		tempElement->SetTextColor(0);

	int count = 1;

	int i = 0;


	//std::map<std::string, UnitType*>UnitMap;
	//if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
	//	UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap;
	//else
	//	UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap;

	for (auto it : Selections)
	{

		/*string UnitDisplay;
		for (auto it : Selections[i]->InternalBattalionList)
		{
		UnitDisplay += (it.first + " x" + std::to_string(it.second) + " ");
		}

		tempElement->SetText(UnitDisplay);
		tempElement->SetTextColor(0);*/
		float count2 = 1.f;
		for (auto it2 : Selections[i]->InternalBattalionList)
		{
			std::string MeshName;
			/*for (auto it3 : UnitMap)
			{
				if (it3.first == it2.first)
				{
					MeshName = it3.second->GetMeshName();
				}
			}*/
			InterfaceElement* tempElement2 = nullptr;
			tempElement2 = MultipleUnitLayer->CreateNewInterfaceElement(it2.first + std::to_string(i) + "Transparent", MeshName, Vector3((lowestPtX + IconSpaceWidth * count2) + HalfDimension.x, highestPt.y - IconSpaceHeight * (count)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
			tempElement2->SetTargetPosition(Vector3((lowestPtX + IconSpaceWidth * count2) + HalfDimension.x, highestPt.y - IconSpaceHeight * (count)));
			tempElement2->SetText("?");
			tempElement2->SetTextColor(0);

			tempElement2 = MultipleUnitLayer->CreateNewInterfaceElement(it2.first + std::to_string(i) + "Amount", "Transparent", Vector3((lowestPtX + IconSpaceWidth * (count2 + 1) + HalfDimension.x), highestPt.y - IconSpaceHeight * (count)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
			tempElement2->SetTargetPosition(Vector3((lowestPtX + IconSpaceWidth * (count2 + 1) + HalfDimension.x), highestPt.y - IconSpaceHeight * (count)));
			tempElement2->SetText("x" + std::to_string(it2.second));
			tempElement2->SetTextColor(0);

			count2 += 2.5;

		}

		tempElement = MultipleUnitLayer->CreateNewInterfaceElement(std::to_string(i), "LoadTexture", Vector3(lowestPt.x, highestPt.y - IconSpaceHeight * (count)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
		tempElement->SetTargetPosition(Vector3(lowestPt.x, highestPt.y - IconSpaceHeight * (count)));

		MultipleUnitElements.insert(std::pair<UnitPiece*, InterfaceElement*>(Selections[i], tempElement));
		++i;
		++count;
	}
}