/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 20 February 2016 13:13 PM
File Name     : UnitCreationInterface.h
Purpose       : InterfaceSystem class that handles UI for unit spawning menu
*/

#ifndef UNITCREATIONINTERFACE_H
#define UNITCREATIONINTERFACE_H

#include "../../Engine/Interface/InterfaceSystem.h"
#include "../../Engine/System/SceneSystem.h"
#include "../Systems/UnitSpawnSystem.h"
#include <map>


class UnitCreationInterface: public InterfaceSystem
{
public:
	bool deploy;
	bool UIDisplayed;

	UnitCreationInterface();
	virtual ~UnitCreationInterface();

	virtual void Init(void);
	virtual void Update(const float& dt);
	virtual void Render(void);
	virtual void Exit(void);

	void OpenInterface();

	UnitSpawnSystem* returnUnitSpawnSys();

	void CheckDeployed();

	void NoMoneyError();
	
	void InterfaceReset();

	void InterfaceExit();

private:
	void HandleUserInput();
	void ShowDisplay();
	bool firstTime;

	InterfaceLayer* UnitSelectLayer;
	

	//LInterfaceLayer* ScrollingLayer;
	InterfaceLayer* IconLayer;



	InterfaceLayer* WarningLayer;
	InterfaceElement* NoUnitPopup;
	InterfaceElement* NoMoneyPopup;

	UnitSpawnSystem* UnitSpawner;

	Vector3 ScreenHalfDimension;

	InterfaceLayer* UnitDisplayLayer;
	InterfaceElement* DeployButton;
	InterfaceElement* TotalCost;
	InterfaceElement* CostBox;
	InterfaceElement* DisplayQuad = nullptr;

	bool warningDisplayed;
	bool warningDisplayed2;
	float warningTime;

	std::map<std::string, InterfaceElement*> IconMap;
	std::map<std::string, InterfaceElement*> IconCounterMap;

};

#endif