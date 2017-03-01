#include "MainMenuInterface.h"
#include "../Mains/Application.h"

MainMenuInterface::MainMenuInterface()
{
	Init();
}

MainMenuInterface::~MainMenuInterface()
{

}

void MainMenuInterface::Init()
{
	currentLayer = L_MAIN;
	nextLayer = -1;
	setCounter = 0;
	InitMainLayer();
	InitFactionLayer();
	for (int i = 0; i < L_TOTAL; ++i)
	{
		if (i == L_MAIN)
		{
			initedLayers[i] = true;
		}
		else
		{
			initedLayers[i] = false;
		}
	}
}

void MainMenuInterface::InitMainLayer()
{
	MainLayer = CreateNewInterfaceLayer(std::to_string(L_MAIN), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(2.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	StartButton = MainLayer->CreateNewInterfaceElement("StartButton", "quad", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	StartButton->SetTargetPosition(0);
	StartButton->SetText("StartGame");
	StartButton->SetTextColor(0);

	InstructionsButton = MainLayer->CreateNewInterfaceElement("InstructionsButton", "quad", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	InstructionsButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0));
	InstructionsButton->SetText("Instructions");
	InstructionsButton->SetTextColor(0);

	SettingsButton = MainLayer->CreateNewInterfaceElement("SettingsButton", "quad", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	SettingsButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0));
	SettingsButton->SetText("Settings");
	SettingsButton->SetTextColor(0);

	ExitButton = MainLayer->CreateNewInterfaceElement("ExitButton", "quad", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	ExitButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));
	ExitButton->SetText("Exit");
	ExitButton->SetTextColor(0);
}

void MainMenuInterface::InitFactionLayer()
{
	FactionLayer = CreateNewInterfaceLayer(std::to_string(L_FACTION), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(2.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	Faction1Button = FactionLayer->CreateNewInterfaceElement("Faction1Button", "LivingFactionLogo", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	Faction1Button->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * -(0.5f), SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(0.25f), 0));

	Faction1Name = FactionLayer->CreateNewInterfaceElement("Faction1Name", "WoodButton", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y* 0.3f, 0));
	Faction1Name->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * -(0.5f), SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.5f, 0));
	Faction1Name->SetText("Humans");
	Faction1Name->SetTextColor(Vector3(0.9f,0.7f,0));

	Faction2Button = FactionLayer ->CreateNewInterfaceElement("Faction2Button", "UndeadFactionLogo", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	Faction2Button->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * (0.5f), SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(0.25f), 0));

	Faction2Name = FactionLayer->CreateNewInterfaceElement("Faction2Name", "BlueButton", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	Faction2Name->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * (0.5f), SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.5f, 0));
	Faction2Name->SetText("Demons");
	Faction2Name->SetTextColor(Vector3(0, 0.2f, 0.9f));

	BackButtonF = FactionLayer->CreateNewInterfaceElement("BackButtonF", "quad", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonF->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0));
	BackButtonF->SetText("Back");
	BackButtonF->SetTextColor(0);
}


void MainMenuInterface::Update(const float& dt)
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active)
		{
			if (nextLayer != -1)
			{
				if (setCounter < 2)
				{
					if ((it->GetEntityID() == std::to_string(currentLayer) || it->GetEntityID() == std::to_string(nextLayer)))
					{
						if (initedLayers[stoi(it->GetEntityID())] == false)
						{
							initedLayers[stoi(it->GetEntityID())] = true;
						}
						else
						{
							it->SwapOriginalWithTarget();
						}
						++setCounter;
					}
				}
				if (it->GetEntityID() == std::to_string(currentLayer) && (it->GetPosition() - it->GetTargetPosition()).LengthSquared() < 5)
				{
					currentLayer = nextLayer;
					nextLayer = -1;
					setCounter = 0;
				}
			}

			if ((it->GetEntityID() == std::to_string(currentLayer) || it->GetEntityID() == std::to_string(nextLayer)))
				it->Update(dt * 10);

		}
	}
	


	if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_DOWN)
	{
		Vector3 MousePos = SceneSystem::Instance().cSS_InputManager->GetMousePosition();
		if (currentLayer == L_MAIN && nextLayer == -1)
		{
			CheckMenuUserInput(MousePos);
		}

		if (currentLayer == L_FACTION && nextLayer == -1)
		{
			CheckFactionUserInput(MousePos);
		}
	}
}

void MainMenuInterface::Render()
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active && it->Visible)
			it->Render();
	}
}

void MainMenuInterface::Exit()
{
	for (auto it : InternalLayerContainer)
	{
		it->Exit();
		delete it;
	}
	InternalLayerContainer.clear();
}

void MainMenuInterface::CheckMenuUserInput(Vector3 mousePos)
{
	if (StartButton->DetectUserInput(mousePos, MainLayer->GetPosition()))
	{
		//SceneSystem::Instance().SwitchScene("1_Scene");
		nextLayer = L_FACTION;
	}
	if (ExitButton->DetectUserInput(mousePos, MainLayer->GetPosition()))
	{
		Application::ExitGame = true;
	}
}

void MainMenuInterface::CheckFactionUserInput(Vector3 mousePos)
{
	if (BackButtonF->DetectUserInput(mousePos, FactionLayer->GetPosition()))
	{
		//SceneSystem::Instance().SwitchScene("1_Scene");
		nextLayer = L_MAIN;
	}
	else if (Faction1Button->DetectUserInput(mousePos, FactionLayer->GetPosition()))
	{
		GameLogicSystem::Instance().PlayerFaction = GameLogicSystem::Instance().F_LIVING;
		//GameLogicSystem::Instance().UnitInterface->Init();
		GameLogicSystem::Instance().UnitInterface->InterfaceReset();
		GameLogicSystem::Instance().InternalPlayerSystem->SetCash(GameLogicSystem::Instance().UnitInterface->returnUnitSpawnSys()->maxUnitCost * GameLogicSystem::Instance().maxStartingUnits);
		SceneSystem::Instance().SwitchScene("1_Scene");	
		nextLayer = L_MAIN;
		
	}
	else if (Faction2Button->DetectUserInput(mousePos, FactionLayer->GetPosition()))
	{
		GameLogicSystem::Instance().PlayerFaction = GameLogicSystem::Instance().F_UNDEAD;
		//GameLogicSystem::Instance().UnitInterface->Init();
		GameLogicSystem::Instance().UnitInterface->InterfaceReset();
		GameLogicSystem::Instance().InternalPlayerSystem->SetCash(GameLogicSystem::Instance().UnitInterface->returnUnitSpawnSys()->maxUnitCost * GameLogicSystem::Instance().maxStartingUnits);
		SceneSystem::Instance().SwitchScene("1_Scene");
		nextLayer = L_MAIN;
		
	}
}
