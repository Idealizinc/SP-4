#include "UnitCreationInterface.h"
#include "../Systems/GameLogicSystem.h"

UnitCreationInterface::UnitCreationInterface()
{
	Init();
}

UnitCreationInterface::~UnitCreationInterface()
{
	if (UnitSpawnMap)
		delete UnitSpawnMap;
	UnitSpawnMap = nullptr;
	Exit();
}

void UnitCreationInterface::Init()
{

	UnitSpawnMap = new UnitSpawnSystem();

	ScreenHalfDimension = SceneSystem::Instance().cSS_InputManager->ScreenCenter;
	//popup stuff
	warningDisplayed = 0;
	WarningLayer = CreateNewInterfaceLayer("Warning", 0, 0);
	WarningLayer->SetOriginalPosition(Vector3(0, 0, 0));
	WarningLayer->SetTargetPosition(Vector3(0, 0, 0));

	NoUnitPopup = WarningLayer->CreateNewInterfaceElement("NoUnit", "quad1", Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y * 2.5f, 0), Vector3(ScreenHalfDimension.x *0.6f, ScreenHalfDimension.y*0.2f, 1.f));
	NoUnitPopup->SetTargetPosition(Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y * 1.3f, 0));
	NoUnitPopup->SetText("No Unit Selected");
	NoUnitPopup->SetTextColor(0);

	NoUnitPopup->SwapOriginalWithTarget();


	//drag side
	UnitSelectLayer = CreateNewInterfaceLayer("Right", 0, 0);
	UnitSelectLayer->SetOriginalPosition(Vector3(ScreenHalfDimension.x * 4.f,0, 0));
	UnitSelectLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 4.f,0, 0));

	CountLayer = CreateNewInterfaceLayer("Count", 0, 0);
	CountLayer->SetOriginalPosition(Vector3(0, 0, 0));
	CountLayer->SetTargetPosition(Vector3(0, 0, 0));

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
	

	UnitSpawnMap->CreateUnitUIElement(UnitSelectLayer);

	UnitDisplayLayer = CreateNewInterfaceLayer("Left", 0, 0);
	UnitDisplayLayer->SetOriginalPosition(Vector3(ScreenHalfDimension.x * 0.25f, 0, 0));
	UnitDisplayLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 0.25f, 0, 0));

	DeployButton = UnitDisplayLayer->CreateNewInterfaceElement("Deploy", "quad1", Vector3(-ScreenHalfDimension.x  * 0.9f, ScreenHalfDimension.y * 0.5f, 0), Vector3(ScreenHalfDimension.x *0.6f, ScreenHalfDimension.y*0.2f, 1.f));
	DeployButton->SetTargetPosition(Vector3(-ScreenHalfDimension.x  * 0.9f, ScreenHalfDimension.y * 0.5f, 0));
	DeployButton->SetText("Deploy");

	DisplayQuad = UnitDisplayLayer->CreateNewInterfaceElement("Display", "quad", Vector3(-ScreenHalfDimension.x  *0.9f, ScreenHalfDimension.y * 1.6f, 0), Vector3(ScreenHalfDimension.x *0.7f, ScreenHalfDimension.y*0.6f, 1.f));
	DisplayQuad->SetTargetPosition(Vector3(-ScreenHalfDimension.x * 0.9f, ScreenHalfDimension.y * 1.6f, 0));

	TotalCost = UnitDisplayLayer->CreateNewInterfaceElement("Cost", "quad", Vector3(-ScreenHalfDimension.x  *0.9f, ScreenHalfDimension.y * 1.f, 0), Vector3(ScreenHalfDimension.x *0.2f, ScreenHalfDimension.y*0.15f, 1.f));
	TotalCost->SetTargetPosition(Vector3(-ScreenHalfDimension.x * 0.9f, ScreenHalfDimension.y * 1.f, 0));
	TotalCost->SetTextColor(0);

	firstTime = 0;
	UIDisplayed = 0;
	deploy = 0;

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
	TotalCost->SetText("$" + std::to_string(UnitSpawnMap->CalculateCost()) + "/$30");
	//UnitSpawnMap->CreateUnitDisplayElement(IconLayer);
	if (deploy == true && UnitSpawnMap->getCurrentUnitCount() == 0)
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

}

void UnitCreationInterface::ShowDisplay()
{
	Vector3 HalfDimension = SceneSystem::Instance().cSS_InputManager->ScreenCenter;
	std::map<std::string, unsigned short> currentUnitMap = UnitSpawnMap->returnRecordedUnitMap();
	int IconCount = currentUnitMap.size();
	//if (IconCount > 1)
	//{
	//	++IconCount;
	//}
	////if (IconCount > 0)
	//{
	//	Vector3 TrueDisplayQuadPos = DisplayQuad->GetPosition() - (UnitDisplayLayer->GetOriginalPosition().x -  UnitDisplayLayer->GetPosition().x);
	//	float DisplayQuadMinX = TrueDisplayQuadPos.x - (DisplayQuad->GetDimensions().x * 0.5f);
	//	int IntervalCounter = 0;
	//	float XInterval = DisplayQuad->GetDimensions().x / IconCount;
	//	for (auto it2 : IconMap)
	//	{
	//		auto it = currentUnitMap.find(it2.first);
	//		auto it3 = IconCounterMap.find(it2.first);

	//			
	//			if (it != currentUnitMap.end() && it3 != IconCounterMap.end())
	//			{
	//				++IntervalCounter;
	//				it2.second->SetTargetPosition(Vector3(DisplayQuadMinX + IntervalCounter * XInterval, DisplayQuad->GetPosition().y + it2.second->GetDimensions().y * 0.5f));
	//				it3->second->SetTargetPosition(Vector3(DisplayQuadMinX + IntervalCounter * XInterval, DisplayQuad->GetPosition().y - it3->second->GetDimensions().y * 0.5f));
	//				if (IntervalCounter >= IconCount)
	//					break;
	//			}
	//			else
	//			{
	//				it2.second->ResetToOriginal();
	//				it3->second->ResetToOriginal();
	//			}
	//	}
	//		
	//}

		Vector3 lowestPt(HalfDimension.x * 0.2f, HalfDimension.y);
		Vector3 highestPt(HalfDimension.x * 1.5f, HalfDimension.y * 1.7f);

		float DisplayWidth = (highestPt.x - lowestPt.x);
		float DisplayHeight = (highestPt.y - lowestPt.y);

		float IconSpaceWidth = (DisplayWidth / IconCount) / 2;
		float IconSpaceHeight = (DisplayHeight / IconCount) / 2;

		InterfaceElement* tempElement = nullptr;

		int count = 1;
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
		UnitSpawnMap->HandleUserInput(MousePos, UnitSelectLayer->GetPosition());

		if (DeployButton->DetectUserInput(MousePos, UnitSelectLayer->GetPosition()))
		{
			deploy = 1;
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
		UnitDisplayLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 1.5f, 0, 0));
		firstTime = 1;
	}
	else
	{
		UnitSelectLayer->SwapOriginalWithTarget();
		UnitDisplayLayer->SwapOriginalWithTarget();
	
	}
	UnitSpawnMap->resetUnitMap();
	deploy = false;
	if (warningDisplayed == true)
	{
		warningDisplayed = false;
		NoUnitPopup->SwapOriginalWithTarget();
	}
	
}

void UnitCreationInterface::CheckDeployed()
{
	if (deploy == 1)
	{
		deploy = 0;
		UnitSpawnMap->resetUnitMap();
	}
}

UnitSpawnSystem* UnitCreationInterface::returnUnitSpawnSys()
{
	return UnitSpawnMap;
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