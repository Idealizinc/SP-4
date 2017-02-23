#include "UnitCreationInterface.h"

UnitCreationInterface::UnitCreationInterface()
{
	Init();
}

UnitCreationInterface::~UnitCreationInterface()
{

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

	IconLayer = CreateNewInterfaceLayer("Icons", 0, 0);
	IconLayer->SetOriginalPosition(Vector3(0, 0, 0));
	IconLayer->SetTargetPosition(Vector3(0, 0, 0));


	CountLayer = CreateNewInterfaceLayer("Count", 0, 0);
	CountLayer->SetOriginalPosition(Vector3(ScreenHalfDimension.x * 3.f, 0, 0));
	CountLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 3.f, 0, 0));

	

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

	UnitSpawnMap->CreateUnitDisplayElement(IconLayer);
	
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