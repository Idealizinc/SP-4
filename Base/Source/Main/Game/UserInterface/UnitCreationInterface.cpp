#include "UnitCreationInterface.h"
#include "../Systems/GameLogicSystem.h"

UnitCreationInterface::UnitCreationInterface()
{
	Init();
}

UnitCreationInterface::~UnitCreationInterface()
{
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
	CountLayer->SetOriginalPosition(Vector3(ScreenHalfDimension.x * 3.f, 0, 0));
	CountLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 3.f, 0, 0));

	IconLayer = CreateNewInterfaceLayer("Icons", 0, 0);
	IconLayer->SetOriginalPosition(Vector3(0, 0, 0));
	IconLayer->SetTargetPosition(Vector3(0, 0, 0));

	for (auto it : GameLogicSystem::Instance().InternalBattleSystem->UnitData.UnitMap)
	{
		InterfaceElement* tempElement = nullptr;

		tempElement = IconLayer->CreateNewInterfaceElement("Icon" + it.second->GetName(), "weed2", Vector3(ScreenHalfDimension.x * 0.5f, ScreenHalfDimension.y * 3.f, 0), Vector3(ScreenHalfDimension.x *0.15f, ScreenHalfDimension.y*0.25f, 1));
		tempElement->SetTargetPosition(Vector3(ScreenHalfDimension.x * 0.5f, ScreenHalfDimension.y * 3.f, 0));

		IconMap.insert(std::pair<std::string, InterfaceElement*>(it.second->GetName(), tempElement));
	}
	

	UnitSpawnMap->CreateUnitUIElement(UnitSelectLayer);

	DeployButton = UnitSelectLayer->CreateNewInterfaceElement("Deploy", "quad1", Vector3(-ScreenHalfDimension.x, ScreenHalfDimension.y * 0.5f, 0), Vector3(ScreenHalfDimension.x *0.6f, ScreenHalfDimension.y*0.2f, 1.f));
	DeployButton->SetTargetPosition(Vector3(-ScreenHalfDimension.x, ScreenHalfDimension.y * 0.5f, 0));
	DeployButton->SetText("Deploy");

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
	//UnitSpawnMap->CreateUnitDisplayElement(IconLayer);
	
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
			bool displayed = false;
			for (auto it : currentUnitMap)
			{
				if (it2.first == it.first)
				{
					it2.second->SetTargetPosition(Vector3(lowestPt.x + IconSpaceWidth * count , highestPt.y));
					++count;
					displayed = true;
					break;
				}
			}
			if (displayed == false)
			{
				it2.second->SetTargetPosition(it2.second->GetOriginalPosition());
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

InterfaceLayer* UnitCreationInterface::CreateNewInterfaceLayer(const std::string& LayerID, const Vector3& OriginalPosition, const Vector3& TargetPosition)
{
	InterfaceLayer* NewL = new InterfaceLayer();
	NewL->SetEntityID(LayerID);
	NewL->SetOriginalPosition(OriginalPosition);
	NewL->SetTargetPosition(TargetPosition);
	InternalLayerContainer.push_back(NewL);
	return NewL;
}

void UnitCreationInterface::OpenInterface()
{
	if (firstTime == 0)
	{
		UnitSelectLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 1.5f,0, 0));
	
		firstTime = 1;
	}
	else
	{
		UnitSelectLayer->SwapOriginalWithTarget();
	
	}
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


//void UnitCreationInterface::CloseInterface()
//{
//	UnitSelectLayer->SwapOriginalWithTarget();
//	UnitInfoLayer->SwapOriginalWithTarget();
//	UnitSpawnMap->resetUnitMap();
//	//UIDisplayed = false;
//}

//std::map<int, int> UnitCreationInterface::returnUnitSpawnMap()
//{
//	return UnitSpawnMap->returnUnitMap();
//}

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