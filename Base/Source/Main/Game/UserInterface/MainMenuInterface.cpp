#include "MainMenuInterface.h"
#include "../Mains/Application.h"
#include "../Logic/Level/LevelDataLoader.h"
#include "../Scene/GameScene.h"
#include "../../Engine/System/SceneSystem.h"

Vector3 WoodTextColor = Vector3(0.3f, 0.1f, 0.1f);


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
	InitSettingsLayer();
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
	StartButton->SetText("Start");
	StartButton->SetTextColor(WoodTextColor);


	InstructionsButton = MainLayer->CreateNewInterfaceElement("InstructionsButtonF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f)), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	InstructionsButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f)));
	InstructionsButton = MainLayer->CreateNewInterfaceElement("InstructionsButton", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	InstructionsButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.1f), 0));
	InstructionsButton->SetText("Instructions");
	InstructionsButton->SetTextColor(WoodTextColor);

	SettingsButton = MainLayer->CreateNewInterfaceElement("SettingsButtonF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	SettingsButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0));
	SettingsButton = MainLayer->CreateNewInterfaceElement("SettingsButton", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	SettingsButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.2f), 0));
	SettingsButton->SetText("Settings");
	SettingsButton->SetTextColor(WoodTextColor);

	ExitButton = MainLayer->CreateNewInterfaceElement("ExitButtonF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	ExitButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));
	ExitButton = MainLayer->CreateNewInterfaceElement("ExitButton", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	ExitButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.3f), 0));
	ExitButton->SetText("Exit");
	ExitButton->SetTextColor(WoodTextColor);
}

void MainMenuInterface::InitFactionLayer()
{
	FactionLayer = CreateNewInterfaceLayer(std::to_string(L_FACTION), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(2.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));

	Faction1Name = FactionLayer->CreateNewInterfaceElement("Faction1Button", "LivingFactionLogo", 0.f, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, 0));
	Faction1Name->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * -(0.35f), 0, 0));

	Faction1Button = FactionLayer->CreateNewInterfaceElement("Faction1Name", "BlueButton", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y* 0.2f, 0));
	Faction1Button->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * -(0.35f), SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(0.5f), 0));
	Faction1Button->SetText("Living");
	Faction1Button->SetTextColor(Vector3(0, 0.2f, 0.9f));

	Faction2Name = FactionLayer ->CreateNewInterfaceElement("Faction2Button", "UndeadFactionLogo", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.y* 0.8f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, 0));
	Faction2Name->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * (0.35f)));

	Faction2Button = FactionLayer->CreateNewInterfaceElement("Faction2Name", "WoodButton", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	Faction2Button->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * (0.35f), SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(0.5f), 0));
	Faction2Button->SetText("Undead");
	Faction2Button->SetTextColor(Vector3(0.9f, 0.7f, 0));

	BackButtonF = FactionLayer->CreateNewInterfaceElement("BackButtonFF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonF->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0));
	BackButtonF = FactionLayer->CreateNewInterfaceElement("BackButtonF", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonF->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0));
	BackButtonF->SetText("Back");
	BackButtonF->SetTextColor(WoodTextColor);

	FactionTitle = FactionLayer->CreateNewInterfaceElement("BackButtonFF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	FactionTitle->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (0.3f), 0));
	
	FactionTitle = FactionLayer->CreateNewInterfaceElement("BackButtonF", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	FactionTitle->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (0.3f), 0));
	FactionTitle->SetText("Select Your Faction!");
	FactionTitle->SetTextColor(WoodTextColor);
}

void MainMenuInterface::InitInstructLayer()
{
	InstructionsLayer = CreateNewInterfaceLayer(std::to_string(L_INSTRUCTIONS), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(2.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));

	
	InstructionsText = InstructionsLayer->CreateNewInterfaceElement("Instructions", "InstructionsBoard", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	InstructionsText->SetTargetPosition(0);

	InstructionsTextP2 = InstructionsLayer->CreateNewInterfaceElement("Instructions", "InstructionsBoard", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	InstructionsTextP2->SetTargetPosition(RightSide);

	InstructionsTextP3 = InstructionsLayer->CreateNewInterfaceElement("Instructions", "InstructionsBoardWeed", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));
	InstructionsTextP3->SetTargetPosition(RightSide);

	Vector3 ButtonDimensions = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.25f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0);

	NextButtonI = InstructionsLayer->CreateNewInterfaceElement("NextButtonI", "BlueBar", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.2f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0.f), ButtonDimensions);
	NextButtonI->SetText("Next");
	NextButtonI->SetTextColor(WoodTextColor);

	PrevButtonI = InstructionsLayer->CreateNewInterfaceElement("PrevButtonI", "BlueBar", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -0.2f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0.f), ButtonDimensions);
	PrevButtonI->SetText("Previous");
	PrevButtonI->SetTextColor(WoodTextColor);

	PageDisplayI = InstructionsLayer->CreateNewInterfaceElement("IDisplayFrame", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0), ButtonDimensions + ButtonDimensions.x);
	PageDisplayI = InstructionsLayer->CreateNewInterfaceElement("PrevButtonI", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0), ButtonDimensions + ButtonDimensions.x);
	PageDisplayI->SetTextColor(WoodTextColor);

	PageDisplayI->SetTextColor(WoodTextColor);

	BackButtonI = InstructionsLayer->CreateNewInterfaceElement("BackButtonI", "RedBar", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonI->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0));
	BackButtonI->SetText("Back");
	BackButtonI->SetTextColor(WoodTextColor);


	if (PrevButtonI->Active == 1)
	{
		PrevButtonI->Active = 0;
		PrevButtonI->Visible = 0;
	}
}

void MainMenuInterface::InitLevelSelectLayer()
{
	LevelSelectLayer = CreateNewInterfaceLayer(std::to_string(L_LEVELSELECT), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(2.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));

	lastLSPage = LevelDataLoader::Instance().LevelMap.size();

	for (int i = 0; i < lastLSPage; ++i)
	{
		InterfaceElement* tempElement = nullptr;
		std::string ID = "Level " + std::to_string(i + 1);

		if (i == 0)
		{
			tempElement = LevelSelectLayer->CreateNewInterfaceElement(ID, "Background", 0, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
			tempElement->SetTargetPosition(0);
		}
		else
		{
			tempElement = LevelSelectLayer->CreateNewInterfaceElement(ID, "Background", RightSide, Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
			tempElement->SetTargetPosition(RightSide);
		}

		LevelSelectData.push_back(tempElement);
	}

	Vector3 ButtonDimensions = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.25f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0);

	NextButtonLS = LevelSelectLayer->CreateNewInterfaceElement("NextButtonLS", "BlueBar", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.2f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.325f), 0.f), ButtonDimensions);
	NextButtonLS->SetText("Next");
	NextButtonLS->SetTextColor(WoodTextColor);

	PrevButtonLS = LevelSelectLayer->CreateNewInterfaceElement("PrevButtonLS", "BlueBar", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -0.2f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.325f), 0.f), ButtonDimensions);
	PrevButtonLS->SetText("Previous");
	PrevButtonLS->SetTextColor(WoodTextColor);

	PageDisplayLS = LevelSelectLayer->CreateNewInterfaceElement("LevelSDisplayFrame", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.325f), 0), ButtonDimensions + ButtonDimensions.x);
	PageDisplayLS = LevelSelectLayer->CreateNewInterfaceElement("PrevButtonLS", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.325f), 0), ButtonDimensions + ButtonDimensions.x);
	PageDisplayLS->SetTextColor(WoodTextColor);

	BackButtonLS = LevelSelectLayer->CreateNewInterfaceElement("BackButtonLS", "RedBar", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.5f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonLS->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.55f), 0));
	BackButtonLS->SetText("Back");
	BackButtonLS->SetTextColor(WoodTextColor);

	PlayButton = LevelSelectLayer->CreateNewInterfaceElement("PlayButton", "GreenBar", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.475f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	PlayButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.45f), 0));
	PlayButton->SetText("Start Level");
	PlayButton->SetTextColor(WoodTextColor);

	LockedButton = LevelSelectLayer->CreateNewInterfaceElement("LockButton", "Lock", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.475f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	LockedButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.45f), 0));
	LockedButton->SetText("Level Locked");
	LockedButton->SetTextColor(Vector3(1, 1, 1));
	LockedButton = LevelSelectLayer->CreateNewInterfaceElement("LockButton", "RedBar", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.475f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	LockedButton->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.45f), 0));

	LevelTitle = LevelSelectLayer->CreateNewInterfaceElement("LevelTitleF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	LevelTitle->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.325f, 0));
	LevelTitle = LevelSelectLayer->CreateNewInterfaceElement("LevelTitle", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	LevelTitle->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.325f, 0));
	LevelTitle->SetText(LevelSelectData.front()->GetEntityID());
	LevelTitle->SetTextColor(WoodTextColor);

	if (PrevButtonLS->Active == 1)
	{
		PrevButtonLS->Active = 0;
		PrevButtonLS->Visible = 0;
	}
	LevelSelectPage = 1;
}

void MainMenuInterface::InitSettingsLayer()
{
	SettingsLayer = CreateNewInterfaceLayer(std::to_string(L_SETTINGS), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -(2.f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0));

	SettingsTitle = SettingsLayer->CreateNewInterfaceElement("SettingsTitleF", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.225f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	SettingsTitle = SettingsLayer->CreateNewInterfaceElement("SettingsTitle", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.225f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.6f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	SettingsTitle->SetText("Settings");
	SettingsTitle->SetTextColor(WoodTextColor);

	BackButtonS = SettingsLayer->CreateNewInterfaceElement("BackButtonS", "RedBar", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -(0.4f), 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.4f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	BackButtonS->SetText("Back");
	BackButtonS->SetTextColor(WoodTextColor);

	// Volume stuff
	float VolumeY = SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.025f;

	TitleVolume = SettingsLayer->CreateNewInterfaceElement("VolumeT", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -0.15f, VolumeY), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TitleVolume->SetText("Volume");
	TitleVolume->SetTextColor(WoodTextColor);

	ButtonVolume = SettingsLayer->CreateNewInterfaceElement("VolumeB", "BlueBar", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.15f, VolumeY, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	ButtonVolume->SetText("Clicker");
	ButtonVolume->SetTextColor(WoodTextColor);

	SettingsLayer->CreateNewInterfaceElement("Line", "BlueBar", Vector3(0, VolumeY), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.01f, 0));

	// Graphics stuff
	float GraphicsY = SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -0.1f;

	TitleGraphics = SettingsLayer->CreateNewInterfaceElement("GraphicsT", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -0.15f, GraphicsY), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TitleGraphics->SetText("Graphics");
	TitleGraphics->SetTextColor(WoodTextColor);

	ButtonGraphics = SettingsLayer->CreateNewInterfaceElement("GraphicsB", "BlueBar", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.15f, GraphicsY, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	ButtonGraphics->SetTextColor(WoodTextColor);

	SettingsLayer->CreateNewInterfaceElement("Line", "BlueBar", Vector3(0, GraphicsY), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.01f, 0));

	// Difficulty stuff
	float SpeedY = SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -0.225f;

	TitleSpeed = SettingsLayer->CreateNewInterfaceElement("SpeedT", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -0.175f, SpeedY), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TitleSpeed->SetText("Anim Speed");
	TitleSpeed->SetTextColor(WoodTextColor);

	ButtonSpeed = SettingsLayer->CreateNewInterfaceElement("SpeedB", "BlueBar", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.15f, SpeedY, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	ButtonSpeed->SetTextColor(WoodTextColor);

	SettingsLayer->CreateNewInterfaceElement("Line", "BlueBar", Vector3(0, SpeedY), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.01f, 0));

	SettingsLayer->CreateNewInterfaceElement("SettingsBackground", "WoodFrameRect", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -0.1f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));
	SettingsBackground = SettingsLayer->CreateNewInterfaceElement("SettingsBackground", "Background", Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * -0.1f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0));

	
	if (GameLogicSystem::Instance().VolumeMultiplier <= 0.6f)
	{
		GameLogicSystem::Instance().VolumeMultiplier = 1.f;
		ButtonVolume->SetText("Medium");
	}
	else if (GameLogicSystem::Instance().VolumeMultiplier <= 1.1f)
	{
		GameLogicSystem::Instance().VolumeMultiplier = 1.5f;
		ButtonVolume->SetText("High");
	}
	else
	{
		GameLogicSystem::Instance().VolumeMultiplier = 0.5f;
		ButtonVolume->SetText("Low");
	}
	
	if (GameLogicSystem::Instance().ParticleMultiplier <= 1.1f)
	{
		GameLogicSystem::Instance().ParticleMultiplier = 3.f;
		ButtonGraphics->SetText("Medium");
	}
	else if (GameLogicSystem::Instance().ParticleMultiplier <= 3.1f)
	{
		GameLogicSystem::Instance().ParticleMultiplier = 5.f;
		ButtonGraphics->SetText("High");
	}
	else
	{
		GameLogicSystem::Instance().ParticleMultiplier = 1.f;
		ButtonGraphics->SetText("Low");
	}
	
	if (GameLogicSystem::Instance().PieceAnimationSpeed <= 1.1f)
	{
		GameLogicSystem::Instance().PieceAnimationSpeed = 3.f;
		ButtonSpeed->SetText("Medium");
	}
	else if (GameLogicSystem::Instance().PieceAnimationSpeed <= 3.1f)
	{
		GameLogicSystem::Instance().PieceAnimationSpeed = 5.f;
		ButtonSpeed->SetText("High");
	}
	else
	{
		GameLogicSystem::Instance().PieceAnimationSpeed = 1.f;
		ButtonSpeed->SetText("Low");
	}
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
		PlayButton->Visible = true;

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
		if (LevelSelectPage > GameLogicSystem::Instance().MaxLevelUnlocked)
		{
			PlayButton->Visible = false;
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
		if (LevelSelectPage <= GameLogicSystem::Instance().MaxLevelUnlocked)
		{
			PlayButton->Visible = true;
		}
	}
	else if (PlayButton->DetectUserInput(mousePos, LevelSelectLayer->GetPosition()) && LevelSelectPage <= GameLogicSystem::Instance().MaxLevelUnlocked)
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
		if (GameLogicSystem::Instance().MaxLevelUnlocked == LevelSelectPage)
		{
			GameLogicSystem::Instance().currHighestLevel = true;
		}
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
		for (unsigned int i = 0; i < LevelSelectData.size(); ++i)
		{
			if (i + 1 < (unsigned)LevelSelectPage)
			{
				LevelSelectData[i]->SetTargetPosition(LeftSide);
			}
			else if (i + 1 > (unsigned)LevelSelectPage)
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
		else if (currentLayer == L_SETTINGS)
		{
			CheckSettingsUserInput(MousePos);
		}
	}
}

void MainMenuInterface::CheckSettingsUserInput(Vector3 mousePos)
{
	if (BackButtonS->DetectUserInput(mousePos, SettingsLayer->GetPosition()))
	{
		//SceneSystem::Instance().SwitchScene("1_Scene");
		nextLayer = L_MAIN;
	}
	else if (ButtonVolume->DetectUserInput(mousePos, SettingsLayer->GetPosition()))
	{
		if (GameLogicSystem::Instance().VolumeMultiplier <= 0.6f)
		{
			GameLogicSystem::Instance().VolumeMultiplier = 1.f;
			ButtonVolume->SetText("Medium");
		}
		else if (GameLogicSystem::Instance().VolumeMultiplier <= 1.1f)
		{
			GameLogicSystem::Instance().VolumeMultiplier = 1.5f;
			ButtonVolume->SetText("High");
		}
		else
		{
			GameLogicSystem::Instance().VolumeMultiplier = 0.5f;
			ButtonVolume->SetText("Low");
		}
	}
	else if (ButtonGraphics->DetectUserInput(mousePos, SettingsLayer->GetPosition()))
	{
		if (GameLogicSystem::Instance().ParticleMultiplier <= 1.1f)
		{
			GameLogicSystem::Instance().ParticleMultiplier = 3.f;
			ButtonGraphics->SetText("Medium");
		}
		else if (GameLogicSystem::Instance().ParticleMultiplier <= 3.1f)
		{
			GameLogicSystem::Instance().ParticleMultiplier = 5.f;
			ButtonGraphics->SetText("High");
		}
		else
		{
			GameLogicSystem::Instance().ParticleMultiplier = 1.f;
			ButtonGraphics->SetText("Low");
		}
	}
	else if (ButtonSpeed->DetectUserInput(mousePos, SettingsLayer->GetPosition()))
	{
		if (GameLogicSystem::Instance().PieceAnimationSpeed <= 1.1f)
		{
			GameLogicSystem::Instance().PieceAnimationSpeed = 3.f;
			ButtonSpeed->SetText("Medium");
		}
		else if (GameLogicSystem::Instance().PieceAnimationSpeed <= 3.1f)
		{
			GameLogicSystem::Instance().PieceAnimationSpeed = 5.f;
			ButtonSpeed->SetText("High");
		}
		else
		{
			GameLogicSystem::Instance().PieceAnimationSpeed = 1.f;
			ButtonSpeed->SetText("Low");
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
	else if (InstructionsButton->DetectUserInput(mousePos, MainLayer->GetPosition()))
	{
		nextLayer = L_INSTRUCTIONS;
	}
	else if (SettingsButton->DetectUserInput(mousePos, MainLayer->GetPosition()))
	{
		nextLayer = L_SETTINGS;
	}
	else if(ExitButton->DetectUserInput(mousePos, MainLayer->GetPosition()))
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