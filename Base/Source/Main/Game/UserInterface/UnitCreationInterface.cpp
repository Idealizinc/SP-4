#include "UnitCreationInterface.h"
#include "../Systems/GameLogicSystem.h"

Vector3 UnitBoxDimensions;

UnitCreationInterface::UnitCreationInterface()
{
	Init();
}

UnitCreationInterface::~UnitCreationInterface()
{
	if (UnitSpawner)
		delete UnitSpawner;
	UnitSpawner = nullptr;
	Exit();
}

void UnitCreationInterface::Init()
{
	ScreenHalfDimension = SceneSystem::Instance().cSS_InputManager->ScreenCenter;
	UnitBoxDimensions = Vector3(ScreenHalfDimension.x *0.7f, ScreenHalfDimension.y*0.6f, 1.f);
	
	UnitSpawner = new UnitSpawnSystem();

	//popup stuff
	warningDisplayed = 0;
	warningDisplayed2 = 0;
	WarningLayer = CreateNewInterfaceLayer("Warning", 0, 0);
	WarningLayer->SetOriginalPosition(Vector3(0, 0, 0));
	WarningLayer->SetTargetPosition(Vector3(0, 0, 0));

	NoUnitPopup = WarningLayer->CreateNewInterfaceElement("NoUnit", "RedBar", Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y * 2.5f, 0), Vector3(ScreenHalfDimension.x * 1.5f, ScreenHalfDimension.y*0.2f, 1.f));
	NoUnitPopup->SetTargetPosition(Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y, 0));
	NoUnitPopup->SetText("Please select some units!");
	//NoUnitPopup->SetTextColor(0);

	NoMoneyPopup = WarningLayer->CreateNewInterfaceElement("NoMoney", "RedBar", Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y * 2.5f, 0), Vector3(ScreenHalfDimension.x * 1.5f, ScreenHalfDimension.y*0.2f, 1.f));
	NoMoneyPopup->SetTargetPosition(Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y, 0));
	NoMoneyPopup->SetText("You do not have enough money...");
	//NoMoneyPopup->SetTextColor(0);

	NoSlotPopup = WarningLayer->CreateNewInterfaceElement("NoSlot", "RedBar", Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y * 2.5f, 0), Vector3(ScreenHalfDimension.x * 1.5f, ScreenHalfDimension.y*0.2f, 1.f));
	NoSlotPopup->SetTargetPosition(Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y, 0));
	NoSlotPopup->SetText("The Tile is full...");
	//NoSlotPopup->SetTextColor(0);

	NoUnitPopup->SwapOriginalWithTarget();
	NoMoneyPopup->SwapOriginalWithTarget();
	NoSlotPopup->SwapOriginalWithTarget();

	/*UnitDisplayLayer = CreateNewInterfaceLayer("Left", 0, 0);
	UnitDisplayLayer->SetOriginalPosition(Vector3(ScreenHalfDimension.x * 0.25f, 0, 0));
	UnitDisplayLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 0.25f, 0, 0));

	DeployButton = UnitDisplayLayer->CreateNewInterfaceElement("Deploy", "GreenBar", Vector3(-ScreenHalfDimension.x  * 0.9f, ScreenHalfDimension.y * 0.5f, 0), Vector3(ScreenHalfDimension.x *0.6f, ScreenHalfDimension.y*0.2f, 1.f));
	DeployButton->SetTargetPosition(Vector3(-ScreenHalfDimension.x  * 0.9f, ScreenHalfDimension.y * 0.5f, 0));
	DeployButton->SetText("Deploy");
	DeployButton->SetTextColor(0);

	BackButton = UnitDisplayLayer->CreateNewInterfaceElement("Back", "RedBar", Vector3(-ScreenHalfDimension.x  * 0.9f, ScreenHalfDimension.y * 0.3f, 0), Vector3(ScreenHalfDimension.x *0.3f, ScreenHalfDimension.y*0.1f, 1.f));
	BackButton->SetTargetPosition(Vector3(-ScreenHalfDimension.x  * 0.9f, ScreenHalfDimension.y * 0.3f, 0));
	BackButton->SetText("Back");
	BackButton->SetTextColor(0);

	CostBox = UnitDisplayLayer->CreateNewInterfaceElement("CostBox", "quad", Vector3(-ScreenHalfDimension.x  *0.9f, ScreenHalfDimension.y * 0.9f, 0), Vector3(ScreenHalfDimension.x *0.25f, ScreenHalfDimension.y*0.15f, 1.f));
	CostBox->SetTargetPosition(Vector3(-ScreenHalfDimension.x * 0.9f, ScreenHalfDimension.y * 0.9f, 0));
	CostBox->SetText("Unit Cost");
	CostBox->SetTextColor(0);

	UnitDisplayBackground = UnitDisplayLayer->CreateNewInterfaceElement("background", "RedBar", Vector3(-ScreenHalfDimension.x, ScreenHalfDimension.y, 0), Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y * 2, 1.f));
	UnitDisplayBackground->SetTargetPosition(Vector3(-ScreenHalfDimension.x, ScreenHalfDimension.y, 0));*/

	UIDisplayed = 0;
	deploy = 0;

	warningTime2 = 0;
	warningDisplayed3 = 0;
}

void UnitCreationInterface::InterfaceReset()
{
	IconLayer = CreateNewInterfaceLayer("Icons", 0, 0);
	IconLayer->SetOriginalPosition(Vector3(0, 0, 0));
	IconLayer->SetTargetPosition(Vector3(0, 0, 0));

	std::map<std::string, UnitType*>UnitMap;
	if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
		UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap;
	else
		UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap;

	for (auto it : UnitMap)
	{
		InterfaceElement* tempElement = nullptr;

		tempElement = IconLayer->CreateNewInterfaceElement("Icon" + it.second->GetName(), it.second->GetMeshName(), Vector3(ScreenHalfDimension.x * 0.3f, ScreenHalfDimension.y * 3.f, 0), Vector3(ScreenHalfDimension.x *0.12f, ScreenHalfDimension.y*0.20f, 1));
		tempElement->SetTargetPosition(Vector3(ScreenHalfDimension.x * 0.3f, ScreenHalfDimension.y * 3.f, 0));
		IconMap.insert(std::pair<std::string, InterfaceElement*>(it.second->GetName(), tempElement));

		tempElement = IconLayer->CreateNewInterfaceElement("IconCount" + it.second->GetName(), "Transparent", Vector3(ScreenHalfDimension.x * 0.5f, ScreenHalfDimension.y * 3.f, 0), Vector3(ScreenHalfDimension.x *0.15f, ScreenHalfDimension.y*0.25f, 1));
		tempElement->SetTargetPosition(Vector3(ScreenHalfDimension.x * 0.5f, ScreenHalfDimension.y * 2.f, 0));
		IconCounterMap.insert(std::pair<std::string, InterfaceElement*>(it.second->GetName(), tempElement));
	}

	UnitSelectLayer = CreateNewInterfaceLayer("Right", 0, 0);

	UnitSelectLayer->SetOriginalPosition(Vector3(ScreenHalfDimension.x * 4.f, 0, 0));
	UnitSelectLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 4.f, 0, 0));

	UnitSpawner->CreateUnitUIElement(UnitSelectLayer);

	Vector3 UnitBoxPosition = Vector3(-ScreenHalfDimension.x  *0.9f, ScreenHalfDimension.y * 1.2f, 0);
	TotalCost = UnitSelectLayer->CreateNewInterfaceElement("Header", "GreenBar", Vector3(-ScreenHalfDimension.x  *0.9f, ScreenHalfDimension.y * 1.f, 0), Vector3(UnitBoxDimensions.x * 1.1f, ScreenHalfDimension.y*0.15f, 1.f));
	TotalCost->SetTargetPosition(UnitBoxPosition + Vector3(0, UnitBoxDimensions.y * 0.5f));
	TotalCost->SetTextColor(0);
	TotalCost->SetText("< Selected Units >");

	TotalCost = UnitSelectLayer->CreateNewInterfaceElement("Cost", "BlueBar", Vector3(-ScreenHalfDimension.x  *0.9f, ScreenHalfDimension.y * 1.f, 0), Vector3(UnitBoxDimensions.x * 1.1f, ScreenHalfDimension.y*0.15f, 1.f));
	TotalCost->SetTargetPosition(UnitBoxPosition - Vector3(0, UnitBoxDimensions.y * 0.5f));
	TotalCost->SetTextColor(0);

	DisplayQuad = UnitSelectLayer->CreateNewInterfaceElement("Display", "LoadTexture", UnitBoxDimensions, UnitBoxDimensions);
	DisplayQuad->SetTargetPosition(UnitBoxPosition);

	DeployButton = UnitSelectLayer->CreateNewInterfaceElement("Deploy", "GreenBar", Vector3(-ScreenHalfDimension.x  * 0.9f, ScreenHalfDimension.y * 0.5f, 0), Vector3(ScreenHalfDimension.x *0.6f, ScreenHalfDimension.y*0.2f, 1.f));
	DeployButton->SetTargetPosition(Vector3(-ScreenHalfDimension.x  * 0.9f, ScreenHalfDimension.y * 0.5f, 0));
	DeployButton->SetText("Deploy");
	DeployButton->SetTextColor(0);

	BackButton = UnitSelectLayer->CreateNewInterfaceElement("Back", "RedBar", Vector3(-ScreenHalfDimension.x  * 0.9f, ScreenHalfDimension.y * 0.3f, 0), Vector3(ScreenHalfDimension.x *0.3f, ScreenHalfDimension.y*0.1f, 1.f));
	BackButton->SetTargetPosition(Vector3(-ScreenHalfDimension.x  * 0.9f, ScreenHalfDimension.y * 0.3f, 0));
	BackButton->SetText("Back");
	BackButton->SetTextColor(0);


	firstTime = 0;
}


void UnitCreationInterface::InterfaceExit()
{
	if (IconLayer)
	{
		for (std::vector<InterfaceLayer*>::iterator it = InternalLayerContainer.begin(); it != InternalLayerContainer.end(); ++it)
			if ((*it)->GetEntityID() == IconLayer->GetEntityID())
			{
				InternalLayerContainer.erase(it);
				break;
			}
		IconLayer->Exit();
		delete IconLayer;
		IconLayer = nullptr;
	}
	if (UnitSelectLayer)
	{
		for (std::vector<InterfaceLayer*>::iterator it = InternalLayerContainer.begin(); it != InternalLayerContainer.end(); ++it)
			if ((*it)->GetEntityID() == UnitSelectLayer->GetEntityID())
			{
				InternalLayerContainer.erase(it);
				break;
			}
		UnitSelectLayer->Exit();
		delete UnitSelectLayer;
		UnitSelectLayer = nullptr;
	}
	IconMap.clear();
	//delete IconMap;
	IconCounterMap.clear();
	returnUnitSpawnSys()->MapReset();
	//UnitDisplayLayer->SetOriginalPosition(Vector3(ScreenHalfDimension.x * 0.25f, 0, 0));
	//UnitDisplayLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 0.25f, 0, 0));
}

void UnitCreationInterface::Update(const float& dt)
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active)
			it->Update(dt * 10);
	}

	HandleUserInput();
	ShowDisplay();
	TotalCost->SetText("Unit Cost: $" + std::to_string(UnitSpawner->CalculateCost()) + " / $" + std::to_string(UnitSpawner->maxUnitCost));
	//UnitSpawner->CreateUnitDisplayElement(IconLayer);
	if (deploy == true && UnitSpawner->getCurrentUnitCount() == 0)
	{
		if (warningDisplayed == 0)
		{
			NoUnitPopup->SwapOriginalWithTarget();
			warningDisplayed = 1;
			warningTime = 2;
			deploy = false;
		}
	}
	if (warningDisplayed == 1)
	{
		if (warningTime > 0)
		{
			warningTime -= dt;
		}
		else
		{
			NoUnitPopup->SwapOriginalWithTarget();
			warningDisplayed = 0;
		}
	}

	if (warningDisplayed2 == 1)
	{
		if (warningTime > 0)
		{
			warningTime -= dt;
		}
		else
		{
			NoMoneyPopup->SwapOriginalWithTarget();
			warningDisplayed2 = 0;
		}
	}

	if (warningDisplayed3 == 1)
	{
		if (warningTime2 > 0)
		{
			warningTime2 -= dt;
		}
		else
		{
			NoSlotPopup->SwapOriginalWithTarget();
			warningDisplayed3 = 0;
		}
	}

}

void UnitCreationInterface::ShowDisplay()
{
	Vector3 HalfDimension = SceneSystem::Instance().cSS_InputManager->ScreenCenter;
	std::map<std::string, unsigned short> currentUnitMap = UnitSpawner->returnRecordedUnitMap();
	int IconCount = currentUnitMap.size();

	Vector3 lowestPt(DisplayQuad->GetOriginalPosition().x - UnitBoxDimensions.x * 0.5f, DisplayQuad->GetOriginalPosition().y - UnitBoxDimensions.y * 0.5f);
	Vector3 highestPt(DisplayQuad->GetOriginalPosition().x + UnitBoxDimensions.x * 1.35f, DisplayQuad->GetOriginalPosition().y + UnitBoxDimensions.y);

	float DisplayWidth = (highestPt.x - lowestPt.x);
	float DisplayHeight = (highestPt.y - lowestPt.y);

	float IconSpaceWidth = (DisplayWidth / IconCount) * 0.5f;
	float IconSpaceHeight = (DisplayHeight / IconCount) * 0.5f;

	InterfaceElement* tempElement = nullptr;

	int count = 0;
	for (auto it2 : IconMap)
	{
		auto it = currentUnitMap.find(it2.first);
		auto it3 = IconCounterMap.find(it2.first);
		if (it == currentUnitMap.end())
		{
			it2.second->SetTargetPosition(it2.second->GetOriginalPosition());
			it3->second->SetTargetPosition(it3->second->GetOriginalPosition());
		}
		else
		{
			it2.second->SetTargetPosition(Vector3(lowestPt.x + IconSpaceWidth * (count), highestPt.y));
			it3->second->SetTargetPosition(Vector3(lowestPt.x + IconSpaceWidth * (count), highestPt.y - it2.second->GetDimensions().y * 0.75f));
			it3->second->SetText("x" + std::to_string(it->second));
			it3->second->SetTextColor(0);
			++count;
		}
	}

}

void UnitCreationInterface::Render()
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active && it->Visible)
		it->Render();
	}
}


void UnitCreationInterface::HandleUserInput()
{
	Vector3 MousePos = SceneSystem::Instance().cSS_InputManager->GetMousePosition();

	if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_DOWN)
	{
		UnitSpawner->HandleUserInput(MousePos, UnitSelectLayer->GetPosition());

		if (DeployButton->DetectUserInput(MousePos, UnitSelectLayer->GetPosition()) && warningDisplayed == false && warningDisplayed2 == false)
		{
			deploy = 1;
		}

		if (BackButton->DetectUserInput(MousePos, UnitSelectLayer->GetPosition()))
		{
			GameLogicSystem::Instance().UnitInterface->OpenInterface();
			GameLogicSystem::Instance().GameInterface->toggleSurrender();
		}
	}
	else if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_UP)
	{
		
		

	}
}

void UnitCreationInterface::OpenInterface()
{
	if (firstTime == 0)
	{
		UnitSelectLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 1.5f,0, 0));
		//UnitDisplayLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 1.5f, 0, 0));
		firstTime = 1;
	}
	else
	{
		UnitSelectLayer->SwapOriginalWithTarget();
		//UnitDisplayLayer->SwapOriginalWithTarget();
	}
	UnitSpawner->resetUnitMap();
	deploy = false;
	if (warningDisplayed == true)
	{
		warningDisplayed = false;
		NoUnitPopup->SwapOriginalWithTarget();
	}
	if (warningDisplayed2 == true)
	{
		warningDisplayed2 = false;
		NoMoneyPopup->SwapOriginalWithTarget();
	}
	/*if (warningDisplayed3 == true)
	{
		warningDisplayed3 = false;
		NoMoneyPopup->SwapOriginalWithTarget();
	}*/
	if (UIDisplayed == 1)
	{
		UIDisplayed = 0;
	}
	else
	{
		UIDisplayed = 1;
	}
}

void UnitCreationInterface::CheckDeployed()
{
	if (deploy == 1)
	{
		deploy = 0;
		UnitSpawner->resetUnitMap();
	}
}

void UnitCreationInterface::NoMoneyError()
{
	NoMoneyPopup->SwapOriginalWithTarget();
	warningDisplayed2 = 1;
	warningTime = 2;
	deploy = false;
}

void UnitCreationInterface::NoSlotError()
{
	NoSlotPopup->SwapOriginalWithTarget();
	warningDisplayed3 = 1;
	warningTime2 = 2;
}

UnitSpawnSystem* UnitCreationInterface::returnUnitSpawnSys()
{
	return UnitSpawner;
}

void UnitCreationInterface::Exit()
{
	for (auto it : InternalLayerContainer)
	{
		it->Exit();
		delete it;
	}
	InternalLayerContainer.clear();
}