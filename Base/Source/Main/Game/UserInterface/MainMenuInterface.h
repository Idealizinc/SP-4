/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 20 February 2016 13:13 PM
File Name     : MainMenu.h
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
	
	

	void ResetLayer();

private:
	enum Layers
	{
		L_MAIN = 0,
		L_FACTION,
		L_LEVELSELECT1,
		L_LEVELSELECT2,
		L_INSTRUCTIONS,
		L_SETTINGS,
		L_TOTAL
	};

	int currentLayer;
	int nextLayer;
	//bool PositionsReset;
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

	InterfaceLayer* Faction1LevelSelectLayer;

	InterfaceLayer* Faction2LevelSelectLayer;

	InterfaceLayer* InstructionsLayer;

	InterfaceLayer* SettingsLayer;


	void CheckMenuUserInput(Vector3 mousePos);
	void CheckFactionUserInput(Vector3 mousePos);

	void InitMainLayer();

	void InitFactionLayer();

};

#endif