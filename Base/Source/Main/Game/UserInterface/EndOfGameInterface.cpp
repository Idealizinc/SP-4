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

	EndScreenFrame = EndScreenLayer->CreateNewInterfaceElement("EndGameFrame", "WoodFrameRect", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 1.05f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 1.05f, 0));
	EndScreenFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y , 0));

	EndScreenWinOrLose = EndScreenLayer->CreateNewInterfaceElement("EndScreenWinOrLose", "transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x , SceneSystem::Instance().cSS_InputManager->ScreenCenter.y , 0));
	EndScreenWinOrLose->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.5f, 0));
	EndScreenWinOrLose->SetText("Victorious!");

	EndScreenNextButton = EndScreenLayer->CreateNewInterfaceElement("CloseButton", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	EndScreenNextButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	EndScreenNextButton->SetText("Next Game");

	EndScreenReturnButton = EndScreenLayer->CreateNewInterfaceElement("ReturnButton", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	EndScreenReturnButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	EndScreenReturnButton->SetText("Return to Main Menu");

	EndScreenBackground = EndScreenLayer->CreateNewInterfaceElement("EndGameBackground", "Background", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0));
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
