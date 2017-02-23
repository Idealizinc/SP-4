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


class UnitCreationInterface: public InterfaceSystem
{
public:
	bool UIDisplayed;
	bool deploy;

	UnitCreationInterface();
	~UnitCreationInterface();

	virtual void Init(void);
	virtual void Update(const float& dt);
	virtual void Render(void);
	//virtual void Exit(void);

	void OpenInterface();
	//void CloseInterface();

	//std::map<int, int> returnUnitSpawnMap();

	UnitSpawnSystem* returnUnitSpawnSys();

private:
	void HandleUserInput();
	bool firstTime;
	InterfaceLayer* CreateNewInterfaceLayer(const std::string& LayerID, const Vector3& OriginalPosition, const Vector3& TargetPosition);
	std::vector<InterfaceLayer*> InternalLayerContainer;

	InterfaceLayer* UnitInfoLayer;
	InterfaceElement* UnitInfoBackElement;
	InterfaceElement* UnitInfoFrame;
	InterfaceElement* UnitInfoDisplayElement;
	InterfaceElement* UnitCountElementW;
	InterfaceElement* UnitCountElementM;
	InterfaceElement* UnitCountElementR;


	InterfaceLayer* UnitSelectLayer;
	InterfaceElement* UnitSelectBackElement;
	InterfaceElement* UnitSelectFrame;
	InterfaceElement* Title;
	InterfaceElement* MeleeSlotElement;
	InterfaceElement* MagicSlotElement;
	InterfaceElement* RangeSlotElement;
	InterfaceElement* DeployButton;

	InterfaceLayer* WarningLayer;
	InterfaceElement* NoUnitPopup;

	InterfaceElement* DraggedElement;// for element being dragged

	UnitSpawnSystem* UnitSpawnMap;

	Vector3 ScreenHalfDimension;

	bool warningDisplayed;
	float warningTime;

};

#endif