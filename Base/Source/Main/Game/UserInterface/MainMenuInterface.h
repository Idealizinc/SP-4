/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 20 February 2016 13:13 PM
File Name     : MainMenuInterface.h
Purpose       : InterfaceSystem class that handles UI for MainMenu
*/

#ifndef MAINMENUINTERFACE_H
#define MAINMENUINTERFACE_H

#include "../../Engine/Interface/InterfaceSystem.h"
#include "../../Engine/System/SceneSystem.h"
#include "../Systems/GameLogicSystem.h"


class MainMenuInterface : public InterfaceSystem
{
public:
	MainMenuInterface();
	virtual ~MainMenuInterface();

	virtual void Init(void);
	virtual void Update(const float& dt);
	virtual void Render(void);
	virtual void Exit(void);
	
	void InitiateLevelLoading(const std::string& LevelID);

	void ResetLayer();

private:
	enum Layers
	{
		L_MAIN = 0,
		L_FACTION,
		L_LEVELSELECT,
		L_INSTRUCTIONS,
		L_SETTINGS,
		L_TOTAL
	};

	int currentLayer;
	int nextLayer;
	int setCounter;
	bool initedLayers[L_TOTAL];
	InterfaceLayer* MainLayer;
	//InterfaceElement* GameLogo;
	InterfaceElement* StartButton;
	InterfaceElement* InstructionsButton;
	InterfaceElement* SettingsButton;
	InterfaceElement* ExitButton;

	InterfaceLayer* FactionLayer;
	InterfaceElement* Faction1Button;
	InterfaceElement* Faction1Name;
	InterfaceElement* Faction2Button;
	InterfaceElement* Faction2Name;
	InterfaceElement* BackButtonF;
	InterfaceElement* FactionTitle;

	InterfaceLayer* LevelSelectLayer;
	std::vector<InterfaceElement*> LevelSelectData; // or map?
	int LevelSelectPage;
	int lastLSPage;
	InterfaceElement* NextButtonLS;
	InterfaceElement* PrevButtonLS;
	InterfaceElement* PageDisplayLS;
	InterfaceElement* BackButtonLS;
	InterfaceElement* PlayButton;
	InterfaceElement* LevelTitle;


	InterfaceLayer* InstructionsLayer;
	InterfaceElement* InstructionsText;
	InterfaceElement* InstructionsTextP2;
	InterfaceElement* InstructionsTextP3;
	int instructionsPage;
	const int lastIPage = 3;
	InterfaceElement* NextButtonI;
	InterfaceElement* PrevButtonI;
	InterfaceElement* PageDisplayI;
	InterfaceElement* BackButtonI;

	InterfaceLayer* SettingsLayer;


	void CheckMenuUserInput(Vector3 mousePos);
	void CheckFactionUserInput(Vector3 mousePos);
	void CheckInstructionsUserInput(Vector3 mousePos);
	void CheckLevelUserInput(Vector3 mousePos);
	
	void InitMainLayer();
	void InitFactionLayer();
	void InitInstructLayer();
	void InitLevelSelectLayer();

	Vector3 LeftSide = Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * -(1.0f), SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0);
	Vector3 RightSide = Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 2.0f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.0f, 0);

	std::string SelectedLevelName;
};

#endif