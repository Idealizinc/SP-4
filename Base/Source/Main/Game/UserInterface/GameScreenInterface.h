/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 20 February 2016 13:13 PM
File Name     : GameScreenInterface.h
Purpose       : InterfaceSystem class that handles UI for game screen
*/

#ifndef GAMESCREENINTERFACE_H
#define GAMESCREENINTERFACE_H

#include "../../Engine/Interface/InterfaceSystem.h"
#include "../../Engine/System/SceneSystem.h"
//#include "../Systems/GameLogicSystem.h"

class GameLogicSystem;

class GameScreenInterface : public InterfaceSystem
{
public:
	GameScreenInterface();
	virtual ~GameScreenInterface();

	virtual void Init(void);
	virtual void Update(const float& dt);
	virtual void Render(void);
	virtual void Exit(void);

	void ShowCashReduction(int amount);
	void toggleSurrender();

	void MultipleUnitSelect(std::vector<UnitPiece*> Selections, bool interactive);
	void MultipleUnitSelectE(std::vector<UnitPiece*> Selections);

	UnitPiece* UnitSelected;
	InterfaceLayer* MultipleUnitLayer;
	std::map<UnitPiece*, InterfaceElement*> MultipleUnitElements;
	bool MultipleUnitUI;
	bool SurrenderOn;
	bool MenuOpen;
	bool SurrenderCheck;
	InterfaceLayer* MenuLayer;
	InterfaceElement* MenuButton;

private:
	void PopUpDelay(const float& dt);
	void PopUpReset();
	void InitSurrender();
	//void checkUserInput();

	InterfaceLayer* TurnPopup;
	InterfaceElement* PlayerTurnPopup;
	InterfaceElement* EnemyTurnPopup;

	InterfaceLayer* CashDisplayLayer;
	InterfaceElement* CashTotalDisplay;
	InterfaceElement* CashAddDisplay;
	InterfaceElement* CashSubstractDisplay;
	InterfaceElement* CashSubstractFrame;
	InterfaceElement* GoldImage;
	InterfaceElement* CashBG;

	InterfaceElement* SurrenderButton;
	InterfaceElement* DeployButton;
	InterfaceElement* SkipButton;
	InterfaceElement* MenuFrame;

	InterfaceLayer* SurrenderLayer;
	InterfaceElement* SurrenderBackground;
	InterfaceElement* SurrenderYesButton;
	InterfaceElement* SurrenderNoButton;

	InterfaceLayer* IngameLayer;
	InterfaceElement* GameFrame;

	float CashDisplayTimer;
	int CashEditAmt;

	float TurnPopupTimer = 0;
	bool PoppedUp = 0;
	bool PopUpDone = 0;
	Vector3 CenterTarget = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.9f, 0);
	bool PlayerTurn;
};

#endif