#include "MainMenuInterface.h"
#include "../Mains/Application.h"
#include "../Logic/Level/LevelDataLoader.h"
#include "../Scene/GameScene.h"
#include "../../Engine/System/SceneSystem.h"

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
	InitInstructLayer();
	InitLevelSelectLayer();
	instructionsPage = 1;
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

	StartButton = MainLayer->CreateNewInterfaceElement("StartButtonF", "WoodFrameRect", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	StartButton->SetTargetPosition(0);
	StartButton = MainLayer->CreateNewInterfaceElement("StartButton", "Background", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	StartButton->SetTargetPosition(0);
	StartButton->SetText("StartGame");
	StartButton->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));


	InstructionsButton = MainLayer->CreateNewInterfaceElement("InstructionsButtonF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	InstructionsButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f)));
	InstructionsButton = MainLayer->CreateNewInterfaceElement("InstructionsButton", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	InstructionsButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0));
	InstructionsButton->SetText("Instructions");
	InstructionsButton->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	SettingsButton = MainLayer->CreateNewInterfaceElement("SettingsButtonF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	SettingsButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0));
	SettingsButton = MainLayer->CreateNewInterfaceElement("SettingsButton", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	SettingsButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0));
	SettingsButton->SetText("Settings");
	SettingsButton->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	ExitButton = MainLayer->CreateNewInterfaceElement("ExitButtonF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	ExitButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));
	ExitButton = MainLayer->CreateNewInterfaceElement("ExitButton", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	ExitButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));
	ExitButton->SetText("Exit");
	ExitButton->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));
}

void MainMenuInterface::InitFactionLayer()
{
	FactionLayer = CreateNewInterfaceLayer(std::to_string(L_FACTION), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(2.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));

	Faction1Name = FactionLayer->CreateNewInterfaceElement("Faction1Button", "LivingFactionLogo", 0.f, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, 0));
	Faction1Name->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * -(0.5f), 0, 0));

	Faction1Button = FactionLayer->CreateNewInterfaceElement("Faction1Name", "BlueButton", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y* 0.2f, 0));
	Faction1Button->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * -(0.5f), SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(0.5f), 0));
	Faction1Button->SetText("Living");
	Faction1Button->SetTextColor(Vector3(0, 0.2f, 0.9f));

	Faction2Name = FactionLayer ->CreateNewInterfaceElement("Faction2Button", "UndeadFactionLogo", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.y* 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, 0));
	Faction2Name->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * (0.5f)));

	Faction2Button = FactionLayer->CreateNewInterfaceElement("Faction2Name", "WoodButton", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	Faction2Button->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * (0.5f), SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(0.5f), 0));
	Faction2Button->SetText("Undead");
	Faction2Button->SetTextColor(Vector3(0.9f, 0.7f, 0));

	BackButtonF = FactionLayer->CreateNewInterfaceElement("BackButtonFF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonF->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0));
	BackButtonF = FactionLayer->CreateNewInterfaceElement("BackButtonF", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonF->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0));
	BackButtonF->SetText("Back");
	BackButtonF->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	FactionTitle = FactionLayer->CreateNewInterfaceElement("BackButtonFF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	FactionTitle->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (0.3f), 0));
	
	FactionTitle = FactionLayer->CreateNewInterfaceElement("BackButtonF", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	FactionTitle->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (0.3f), 0));
	FactionTitle->SetText("Select Your Faction!");
	FactionTitle->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));
}

void MainMenuInterface::InitInstructLayer()
{
	InstructionsLayer = CreateNewInterfaceLayer(std::to_string(L_INSTRUCTIONS), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(2.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));

	/*InstructionsText = InstructionsLayer->CreateNewInterfaceElement("Instructions", "WoodFrameRect", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.0f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
	InstructionsText->SetTargetPosition(0);*/
	InstructionsText = InstructionsLayer->CreateNewInterfaceElement("Instructions", "InstructionsBoard", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	InstructionsText->SetTargetPosition(0);

	/*InstructionsTextP2 = InstructionsLayer->CreateNewInterfaceElement("Instructions", "WoodFrameRect", RightSide, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.0f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
	InstructionsTextP2->SetTargetPosition(RightSide);*/
	InstructionsTextP2 = InstructionsLayer->CreateNewInterfaceElement("Instructions", "InstructionsBoard", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	InstructionsTextP2->SetTargetPosition(RightSide);

	/*InstructionsTextP3 = InstructionsLayer->CreateNewInterfaceElement("Instructions", "WoodFrameRect", RightSide, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.0f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
	InstructionsTextP3->SetTargetPosition(RightSide);*/
	InstructionsTextP3 = InstructionsLayer->CreateNewInterfaceElement("Instructions", "InstructionsBoardWeed", RightSide, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	InstructionsTextP3->SetTargetPosition(RightSide);

	//NextButtonI = InstructionsLayer->CreateNewInterfaceElement("NextButtonIF", "WoodFrameRect", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.1, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	//NextButtonI->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.1, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));

	NextButtonI = InstructionsLayer->CreateNewInterfaceElement("NextButtonI", "Background", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.2f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0.f), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	NextButtonI->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.2f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0));
	NextButtonI->SetText("Next");
	NextButtonI->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	//PrevButtonI = InstructionsLayer->CreateNewInterfaceElement("PrevButtonIF", "WoodFrameRect", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.1), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	//PrevButtonI->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.1), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));

	PrevButtonI = InstructionsLayer->CreateNewInterfaceElement("PrevButtonI", "Background", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -0.2f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0.f), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	PrevButtonI->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.2f), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0));
	PrevButtonI->SetText("Prev");
	PrevButtonI->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	PageDisplayI = InstructionsLayer->CreateNewInterfaceElement("PrevButtonI", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	PageDisplayI->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0));

	PageDisplayI->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	BackButtonI = InstructionsLayer->CreateNewInterfaceElement("BackButtonIF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonI->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0));
	BackButtonI = InstructionsLayer->CreateNewInterfaceElement("BackButtonI", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonI->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0));
	BackButtonI->SetText("Back");
	BackButtonI->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	if (PrevButtonI->Active == 1)
	{
		PrevButtonI->Active = 0;
		PrevButtonI->Visible = 0;
	}
}

void MainMenuInterface::InitLevelSelectLayer()
{
	LevelSelectLayer = CreateNewInterfaceLayer(std::to_string(L_LEVELSELECT), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(2.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));

	///*InstructionsText = InstructionsLayer->CreateNewInterfaceElement("Instructions", "WoodFrameRect", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.0f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
	//InstructionsText->SetTargetPosition(0);*/
	//InstructionsText = LevelSelectLayer->CreateNewInterfaceElement("Instructions", "InstructionsBoard", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	//InstructionsText->SetTargetPosition(0);

	///*InstructionsTextP2 = InstructionsLayer->CreateNewInterfaceElement("Instructions", "WoodFrameRect", RightSide, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.0f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
	//InstructionsTextP2->SetTargetPosition(RightSide);*/
	//InstructionsTextP2 = LevelSelectLayer->CreateNewInterfaceElement("Instructions", "InstructionsBoard", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	//InstructionsTextP2->SetTargetPosition(RightSide);

	///*InstructionsTextP3 = InstructionsLayer->CreateNewInterfaceElement("Instructions", "WoodFrameRect", RightSide, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.0f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
	//InstructionsTextP3->SetTargetPosition(RightSide);*/
	//InstructionsTextP3 = LevelSelectLayer->CreateNewInterfaceElement("Instructions", "InstructionsBoardWeed", RightSide, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	//InstructionsTextP3->SetTargetPosition(RightSide);

	//NextButtonI = InstructionsLayer->CreateNewInterfaceElement("NextButtonIF", "WoodFrameRect", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.1, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	//NextButtonI->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.1, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));

	lastLSPage = LevelDataLoader::Instance().LevelMap.size();

	for (int i = 0; i < lastLSPage; ++i)
	{
		InterfaceElement* tempElement = nullptr;

		if (i == 0)
		{
			tempElement = LevelSelectLayer->CreateNewInterfaceElement("Level" + std::to_string(i + 1), "Background", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
			tempElement->SetTargetPosition(0);
		}
		else
		{
			tempElement = LevelSelectLayer->CreateNewInterfaceElement("Level" + std::to_string(i + 1), "Background", RightSide, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
			tempElement->SetTargetPosition(RightSide);
		}

		/*tempElement->SetText("Level" + std::to_string(i + 1));
		tempElement->SetTextColor(0);*/

		LevelSelectData.push_back(tempElement);
	}

	NextButtonLS = LevelSelectLayer->CreateNewInterfaceElement("NextButtonLS", "Background", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.2f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0.f), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	NextButtonLS->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.2f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0));
	NextButtonLS->SetText("Next");
	NextButtonLS->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	//PrevButtonI = InstructionsLayer->CreateNewInterfaceElement("PrevButtonIF", "WoodFrameRect", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.1), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	//PrevButtonI->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.1), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));

	PrevButtonLS =LevelSelectLayer->CreateNewInterfaceElement("PrevButtonLS", "Background", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -0.2f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0.f), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	PrevButtonLS->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(0.2f), SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0));
	PrevButtonLS->SetText("Prev");
	PrevButtonLS->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	PageDisplayLS = LevelSelectLayer->CreateNewInterfaceElement("PrevButtonLS", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	PageDisplayLS->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0));

	PageDisplayLS->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	BackButtonLS = LevelSelectLayer->CreateNewInterfaceElement("BackButtonLSF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonLS->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0));
	BackButtonLS = LevelSelectLayer->CreateNewInterfaceElement("BackButtonLS", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonLS->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0));
	BackButtonLS->SetText("Back");
	BackButtonLS->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	PlayButton = LevelSelectLayer->CreateNewInterfaceElement("PlayButtonF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	PlayButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));
	PlayButton = LevelSelectLayer->CreateNewInterfaceElement("PlayButton", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	PlayButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));
	PlayButton->SetText("BATTLE");
	PlayButton->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	LevelTitle = LevelSelectLayer->CreateNewInterfaceElement("LevelTitleF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	LevelTitle->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.35f, 0));
	LevelTitle = LevelSelectLayer->CreateNewInterfaceElement("LevelTitle", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	LevelTitle->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.35f, 0));

	LevelTitle->SetTextColor(Vector3(0.5f, 0.3f, 0.3f));

	if (PrevButtonLS->Active == 1)
	{
		PrevButtonLS->Active = 0;
		PrevButtonLS->Visible = 0;
	}
	LevelSelectPage = 1;
}

void MainMenuInterface::CheckLevelUserInput(Vector3 mousePos)
{
	bool LevelInitiationRequired = false;

	if (BackButtonLS->DetectUserInput(mousePos, LevelSelectLayer->GetPosition()))
	{
		nextLayer = L_MAIN;
		LevelSelectPage = 1;
		if (NextButtonLS->Active == 0)
		{
			NextButtonLS->Active = 1;
			NextButtonLS->Visible = 1;
		}
		if (PrevButtonLS->Active == 1)
		{
			PrevButtonLS->Active = 0;
			PrevButtonLS->Visible = 0;
		}

	}
	else if (NextButtonLS->DetectUserInput(mousePos, LevelSelectLayer->GetPosition()))
	{
		++LevelSelectPage;
		if (LevelSelectPage == lastLSPage)
		{
			NextButtonLS->Active = 0;
			NextButtonLS->Visible = 0;
		}
		if (LevelSelectPage == 2)
		{
			PrevButtonLS->Active = 1;
			PrevButtonLS->Visible = 1;
		}
	}
	else if (PrevButtonLS->DetectUserInput(mousePos, LevelSelectLayer->GetPosition()))
	{
		--LevelSelectPage;
		if (LevelSelectPage == 1)
		{
			PrevButtonLS->Active = 0;
			PrevButtonLS->Visible = 0;
		}
		if (LevelSelectPage == lastLSPage - 1)
		{
			NextButtonLS->Active = 1;
			NextButtonLS->Visible = 1;
		}
	}
	else if (PlayButton->DetectUserInput(mousePos, LevelSelectLayer->GetPosition()))
	{
		nextLayer = L_MAIN;

		if (NextButtonLS->Active == 0)
		{
			NextButtonLS->Active = 1;
			NextButtonLS->Visible = 1;
		}
		if (PrevButtonLS->Active == 1)
		{
			PrevButtonLS->Active = 0;
			PrevButtonLS->Visible = 0;
		}
		LevelInitiationRequired = true;
	}
	if (LevelInitiationRequired)
	{
		LevelInitiationRequired = false;
		InitiateLevelLoading(LevelSelectData[LevelSelectPage - 1]->GetEntityID());
		LevelSelectPage = 1;
	}
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
				it->Update(dt * 8);
		}
	}

		PageDisplayI->SetText("Page " + std::to_string(instructionsPage) + " of " + std::to_string(lastIPage));
		PageDisplayLS->SetText("Page " + std::to_string(LevelSelectPage) + " of " + std::to_string(lastLSPage));

	if (instructionsPage == 1)
	{
		InstructionsText->SetTargetPosition(0);
		InstructionsTextP2->SetTargetPosition(RightSide);
		InstructionsTextP3->SetTargetPosition(RightSide);
	}
	else if (instructionsPage == 2)
	{
		InstructionsText->SetTargetPosition(LeftSide);
		InstructionsTextP2->SetTargetPosition(0);
		InstructionsTextP3->SetTargetPosition(RightSide);
	}
	else if (instructionsPage == 3)
	{
		InstructionsText->SetTargetPosition(LeftSide);
		InstructionsTextP2->SetTargetPosition(LeftSide);
		InstructionsTextP3->SetTargetPosition(0);
	}

	if (currentLayer == L_LEVELSELECT)
	{
		for (int i = 0; i < LevelSelectData.size(); ++i)
		{
			if (i + 1 < LevelSelectPage)
			{
				LevelSelectData[i]->SetTargetPosition(LeftSide);
			}
			else if (i + 1 > LevelSelectPage)
			{
				LevelSelectData[i]->SetTargetPosition(RightSide);
			}
			else
			{
				LevelSelectData[i]->SetTargetPosition(0);
				LevelTitle->SetText(LevelSelectData[i]->GetEntityID());
			}
			
		}
	}


	if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_DOWN)
	{
		Vector3 MousePos = SceneSystem::Instance().cSS_InputManager->GetMousePosition();
		if (currentLayer == L_MAIN)
		{
			CheckMenuUserInput(MousePos);
		}
		else if(currentLayer == L_FACTION)
		{
			CheckFactionUserInput(MousePos);
		}
		else if (currentLayer == L_INSTRUCTIONS)
		{
			CheckInstructionsUserInput(MousePos);
		}
		else if (currentLayer == L_LEVELSELECT)
		{
			CheckLevelUserInput(MousePos);
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

	if (InstructionsButton->DetectUserInput(mousePos, MainLayer->GetPosition()))
	{
		nextLayer = L_INSTRUCTIONS;
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
		nextLayer = L_LEVELSELECT;
	}
	else if (Faction2Button->DetectUserInput(mousePos, FactionLayer->GetPosition()))
	{
		GameLogicSystem::Instance().PlayerFaction = GameLogicSystem::Instance().F_UNDEAD;
		nextLayer = L_LEVELSELECT;
	}
	
}

void MainMenuInterface::CheckInstructionsUserInput(Vector3 mousePos)
{
	if (BackButtonI->DetectUserInput(mousePos, InstructionsLayer->GetPosition()))
	{
		nextLayer = L_MAIN;
		instructionsPage = 1;
		if (NextButtonI->Active == 0)
		{
			NextButtonI->Active = 1;
			NextButtonI->Visible = 1;
		}
		if (PrevButtonI->Active == 1)
		{
			PrevButtonI->Active = 0;
			PrevButtonI->Visible = 0;
		}

	}
	else if (NextButtonI->DetectUserInput(mousePos, InstructionsLayer->GetPosition()))
	{
		++instructionsPage;
		if (instructionsPage == 3)
		{
			NextButtonI->Active = 0;
			NextButtonI->Visible = 0;
		}
		if (instructionsPage == 2)
		{
			PrevButtonI->Active = 1;
			PrevButtonI->Visible = 1;
		}
	}
	else if (PrevButtonI->DetectUserInput(mousePos, InstructionsLayer->GetPosition()))
	{
		--instructionsPage;
		if (instructionsPage == 1)
		{
			PrevButtonI->Active = 0;
			PrevButtonI->Visible = 0;
		}
		if (instructionsPage == 2)
		{
			NextButtonI->Active = 1;
			NextButtonI->Visible = 1;
		}
	}
}

void MainMenuInterface::InitiateLevelLoading(const std::string& LevelID)
{
	// Switch the scene first! If not we will not be able to call the right function.
	SceneSystem::Instance().SwitchScene("1_Scene");
	//// Here we shall randomly assign a level to the player for testing purposes
	//auto it = LevelDataLoader::Instance().LevelMap.begin();
	//std::advance(it, Math::RandIntMinMax(0, LevelDataLoader::Instance().LevelMap.size() - 1));
	//std::string random_key = it->first;

	GameScene* GS = dynamic_cast<GameScene*>(&SceneSystem::Instance().GetCurrentScene());
	GS->SetUpForLevel(LevelDataLoader::Instance().LevelMap.find(LevelID)->second);

	//GameLogicSystem::Instance().UnitInterface->Init();
	GameLogicSystem::Instance().UnitInterface->InterfaceReset();
	GameLogicSystem::Instance().InternalPlayerSystem->SetCash(GameLogicSystem::Instance().UnitInterface->returnUnitSpawnSys()->maxUnitCost * GameLogicSystem::Instance().maxStartingUnits);

}