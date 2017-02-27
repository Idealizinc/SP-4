#include "EndOfGameInterface.h"

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

	EndScreenNextButton = EndScreenLayer->CreateNewInterfaceElement("CloseButton", "WoodButton", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	EndScreenNextButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	//EndScreenNextButton->SetText("Next Game");
	//EndScreenNextButton->SetTextColor(Vector3(1, 1, 0));

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

	//if (Faction = LIVING)
	//{
	//	if (Win)
	//	{
	//		EndScreenWinOrLose->SetText("Victorious!");
	//		EndScreenNextButton->SetText("Next Game");
	//		EndScreenNextButton->SetTextColor(Vector3(1, 1, 0));
	//		EndScreenReturnButton->SetText("Main Menu");
	//		EndScreenReturnButton->SetTextColor(Vector3(1, 1, 0));
	//		EndScreenFrame->SetMesh("WoodFrameRect");
	//		EndScreenBackground->SetMesh("LivingFaction");
	//	}
	//	else
	//	{
	//		EndScreenWinOrLose->SetText("Defeated!");
	//		EndScreenNextButton->SetText("Try Again");
	//		EndScreenNextButton->SetTextColor(Vector3(0, 0, 1));
	//		EndScreenReturnButton->SetText("Main Menu");
	//		EndScreenReturnButton->SetTextColor(Vector3(0,0, 1));
	//		EndScreenFrame->SetMesh("BlueFrameRect");
	//		EndScreenBackground->SetMesh("UndeadFaction");
	//	}
	//}
	//else if (Faction = UNDEAD)
	//{
	//	if (Win)
	//	{
	//		EndScreenWinOrLose->SetText("Victorious!");
	//		EndScreenNextButton->SetText("Next Game");
	//		EndScreenNextButton->SetTextColor(Vector3(0, 0, 1));
	//		EndScreenReturnButton->SetText("Main Menu");
	//		EndScreenReturnButton->SetTextColor(Vector3(0, 0, 1));
	//		EndScreenFrame->SetMesh("BlueFrameRect");
	//		EndScreenBackground->SetMesh("UndeadFaction");
	//	}
	//	else
	//	{
	//		EndScreenWinOrLose->SetText("Defeated!");
	//		EndScreenNextButton->SetText("Try Again");
	//		EndScreenNextButton->SetTextColor(Vector3(1, 1, 0));
	//		EndScreenReturnButton->SetText("Main Menu");
	//		EndScreenReturnButton->SetTextColor(Vector3(1, 1, 0));
	//		EndScreenFrame->SetMesh("WoodFrameRect");
	//		EndScreenBackground->SetMesh("LivingFaction");
	//	}
	//}

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
		if (EndScreenNextButton->DetectUserInput(MousePos, EndScreenLayer->GetPosition()))
		{
		}

		else if (EndScreenReturnButton->DetectUserInput(MousePos, EndScreenLayer->GetPosition()))
		{
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
