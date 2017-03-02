#include "EndOfGameInterface.h"
#include "../Systems/GameLogicSystem.h"

EndOfGameInterface::EndOfGameInterface()
{
	Init();
}

EndOfGameInterface::~EndOfGameInterface()
{
	Exit();
}

void EndOfGameInterface::Init()
{
	CenterTarget = SceneSystem::Instance().cSS_InputManager->ScreenCenter;

	EndScreenLayer = CreateNewInterfaceLayer("EndLayer", 0, 0);

	EndScreenWinOrLose = EndScreenLayer->CreateNewInterfaceElement("EndScreenWinOrLose", "transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x , SceneSystem::Instance().cSS_InputManager->ScreenCenter.y , 0));
	EndScreenWinOrLose->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.5f, 0));
	//EndScreenWinOrLose->SetText("Victorious!");

	EndScreenReturnButton = EndScreenLayer->CreateNewInterfaceElement("ReturnButton", "WoodButton", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	EndScreenReturnButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	//EndScreenReturnButton->SetText("Main Menu");
	//EndScreenReturnButton->SetTextColor(Vector3(1, 1, 0));

	EndScreenFrame = EndScreenLayer->CreateNewInterfaceElement("EndGameFrame", "WoodFrameRect", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 1.05f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 1.05f, 0));
	EndScreenFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));


	EndScreenBackground = EndScreenLayer->CreateNewInterfaceElement("EndGameBackground", "LivingFaction", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0));
	EndScreenBackground->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));

	ResetAll();
}

void EndOfGameInterface::Update(const float& dt)
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active)
			it->Update(dt * 10);
	}
	PopUpDelay(dt);
	HandleUserInput();

	if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
	{
		if (GameLogicSystem::Instance().PlayerWon)
		{
			EndScreenWinOrLose->SetText("Victorious!");
			EndScreenReturnButton->SetText("Main Menu");
			EndScreenReturnButton->SetTextColor(Vector3(1, 1, 0));
			EndScreenFrame->SetMesh("WoodFrameRect");
			EndScreenBackground->SetMesh("LivingFaction");
		}
		else
		{
			EndScreenWinOrLose->SetText("Defeated!");
			EndScreenReturnButton->SetMesh("BlueButton");
			EndScreenReturnButton->SetText("Main Menu");
			EndScreenReturnButton->SetTextColor(Vector3(0,0, 1));
			EndScreenFrame->SetMesh("BlueFrameRect");
			EndScreenBackground->SetMesh("UndeadFaction");
		}
	}
	else if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_UNDEAD)
	{
		if (GameLogicSystem::Instance().PlayerWon)
		{
			EndScreenWinOrLose->SetText("Victorious!");
			EndScreenReturnButton->SetMesh("BlueButton");
			EndScreenReturnButton->SetText("Main Menu");
			EndScreenReturnButton->SetTextColor(Vector3(0, 0, 1));
			EndScreenFrame->SetMesh("BlueFrameRect");
			EndScreenBackground->SetMesh("UndeadFaction");
		}
		else
		{
			EndScreenWinOrLose->SetText("Defeated!");
			EndScreenReturnButton->SetText("Main Menu");
			EndScreenReturnButton->SetTextColor(Vector3(1, 1, 0));
			EndScreenFrame->SetMesh("WoodFrameRect");
			EndScreenBackground->SetMesh("LivingFaction");
		}
	}

}

void EndOfGameInterface::Render()
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active && it->Visible)
			it->Render();
	}
}

void EndOfGameInterface::HandleUserInput()
{
	Vector3 MousePos = SceneSystem::Instance().cSS_InputManager->GetMousePosition();

	if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_DOWN)
	{
		if (EndScreenReturnButton->DetectUserInput(MousePos, EndScreenLayer->GetPosition()))
		{
			GameLogicSystem::Instance().PlayerWon = false;
			GameLogicSystem::Instance().GameOver = false;

			GameLogicSystem::Instance().UnitInterface->InterfaceExit();
			SceneSystem::Instance().SwitchScene("MainMenuScene");
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
	}
	else if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_UP)
	{
	}
}

void EndOfGameInterface::PopUpDelay(const float& dt)
{
	
}

void EndOfGameInterface::ResetAll()
{
	ResetAllToOriginal();
}
