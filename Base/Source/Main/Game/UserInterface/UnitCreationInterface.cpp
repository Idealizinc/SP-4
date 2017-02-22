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
	ScreenHalfDimension = SceneSystem::Instance().cSS_InputManager->ScreenCenter;

	warningDisplayed = 0;
	WarningLayer = CreateNewInterfaceLayer("Warning", 0, 0);
	WarningLayer->SetOriginalPosition(Vector3(0, 0, 0));
	WarningLayer->SetTargetPosition(Vector3(0, 0, 0));

	NoUnitPopup = WarningLayer->CreateNewInterfaceElement("NoUnit", "quad1", Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y * 2.5, 0), Vector3(ScreenHalfDimension.x *0.6f, ScreenHalfDimension.y*0.2f, 1));
	NoUnitPopup->SetTargetPosition(Vector3(ScreenHalfDimension.x, ScreenHalfDimension.y * 1.3, 0));
	NoUnitPopup->SetText("No Unit Selected");
	NoUnitPopup->SetTextColor(0);

	NoUnitPopup->SwapOriginalWithTarget();



	UnitSelectLayer = CreateNewInterfaceLayer("Right", 0, 0);
	UnitSelectLayer->SetOriginalPosition(Vector3(ScreenHalfDimension.x * 3.f,0, 0));
	UnitSelectLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 3.f,0, 0));

	UnitSelectFrame = UnitSelectLayer->CreateNewInterfaceElement("UnitSelectFrame", "Frame", Vector3(0, ScreenHalfDimension.y * 1.f, 0), Vector3(ScreenHalfDimension.x * 0.9f, ScreenHalfDimension.y * 2.1f, 1));
	UnitSelectFrame->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.f, 0));
	UnitSelectFrame->SetTextColor(0);

	//NoUnitPopup = UnitSelectLayer->CreateNewInterfaceElement("MeleeSlot", "quad2", Vector3(0, ScreenHalfDimension.y * 1.35f, 0), Vector3(ScreenHalfDimension.x *0.7f, ScreenHalfDimension.y*0.3f, 1));
	//NoUnitPopup->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.35f, 0));
	//NoUnitPopup->SetText("No Unit Selected");
	//NoUnitPopup->SetTextColor(0);



	MeleeSlotElement = UnitSelectLayer->CreateNewInterfaceElement("MeleeSlot", "quad2", Vector3(0, ScreenHalfDimension.y * 1.35f, 0), Vector3(ScreenHalfDimension.x *0.3f, ScreenHalfDimension.y*0.2f, 1));
	MeleeSlotElement->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.35f, 0));
	MeleeSlotElement->SetText("WARRIOR");
	MeleeSlotElement->SetTextColor(0);

	MagicSlotElement = UnitSelectLayer->CreateNewInterfaceElement("MagicSlot", "quad2", Vector3(0, ScreenHalfDimension.y * 1.f, 0), Vector3(ScreenHalfDimension.x *0.3f, ScreenHalfDimension.y*0.2f, 1));
	MagicSlotElement->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.f, 0));
	MagicSlotElement->SetText("MAGE");
	MagicSlotElement->SetTextColor(0);

	RangeSlotElement = UnitSelectLayer->CreateNewInterfaceElement("RangeSlot", "quad2", Vector3(0, ScreenHalfDimension.y * 0.65f, 0), Vector3(ScreenHalfDimension.x *0.3f, ScreenHalfDimension.y*0.2f, 1));
	RangeSlotElement->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 0.65f, 0));
	RangeSlotElement->SetText("RANGER");
	RangeSlotElement->SetTextColor(0);

	DeployButton = UnitSelectLayer->CreateNewInterfaceElement("DeployButton", "quad1", Vector3(0, ScreenHalfDimension.y * 0.3f, 0), Vector3(ScreenHalfDimension.x *0.3f, ScreenHalfDimension.y*0.2f, 1));
	DeployButton->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 0.3f, 0));
	DeployButton->SetText("DEPLOY");
	DeployButton->SetTextColor(0);

	Title = UnitSelectLayer->CreateNewInterfaceElement("Title", "quad", Vector3(0, ScreenHalfDimension.y * 1.7f, 0), Vector3(ScreenHalfDimension.x *0.7f, ScreenHalfDimension.y*0.3f, 1));
	Title->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.7f, 0));
	Title->SetText("DEPLOYMENT");
	Title->SetTextColor(0);



	UnitSelectBackElement = UnitSelectLayer->CreateNewInterfaceElement("UnitSelectBackElement", "Background", Vector3(0, ScreenHalfDimension.y * 1.f, 0), Vector3(ScreenHalfDimension.x * 0.8f, ScreenHalfDimension.y * 2.f, 1));
	UnitSelectBackElement->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.f, 0));
	UnitSelectBackElement->SetTextColor(0);


	UnitInfoLayer = CreateNewInterfaceLayer("Left", 0, 0);
	UnitInfoLayer->SetOriginalPosition(Vector3(-(ScreenHalfDimension.x ), 0, 0));
	UnitInfoLayer->SetTargetPosition(Vector3(-(ScreenHalfDimension.x ), 0, 0));
	
	UnitInfoFrame = UnitInfoLayer->CreateNewInterfaceElement("UnitInfoFrame", "Frame", Vector3(0, ScreenHalfDimension.y * 1.f, 0), Vector3(ScreenHalfDimension.x * 0.9f, ScreenHalfDimension.y * 2.1f, 1));
	UnitInfoFrame->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.f, 0));
	UnitInfoFrame->SetTextColor(0);

	UnitCountElementW = UnitInfoLayer->CreateNewInterfaceElement("UnitCountW", "quad1", Vector3(0, ScreenHalfDimension.y * 1.7f, 0), Vector3(ScreenHalfDimension.x *0.7f, ScreenHalfDimension.y*0.3f, 1));
	UnitCountElementW->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.7f, 0));
	UnitCountElementW->SetTextColor(0);
	

	UnitCountElementM = UnitInfoLayer->CreateNewInterfaceElement("UnitCountM", "quad1", Vector3(0, ScreenHalfDimension.y * 1.4f, 0), Vector3(ScreenHalfDimension.x *0.7f, ScreenHalfDimension.y*0.3f, 1));
	UnitCountElementM->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.5f, 0));
	UnitCountElementM->SetTextColor(0);
	


	UnitCountElementR = UnitInfoLayer->CreateNewInterfaceElement("UnitCountR", "quad1", Vector3(0, ScreenHalfDimension.y * 1.1f, 0), Vector3(ScreenHalfDimension.x *0.7f, ScreenHalfDimension.y*0.3f, 1));
	UnitCountElementR->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.3f, 0));
	UnitCountElementR->SetTextColor(0);

	UnitInfoBackElement = UnitInfoLayer->CreateNewInterfaceElement("UnitInfoBackElement", "quad3", Vector3(0, ScreenHalfDimension.y * 0.55f, 0), Vector3(ScreenHalfDimension.x * 0.7f, ScreenHalfDimension.y * 0.4f, 1));
	UnitInfoBackElement->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 0.55f, 0));
	UnitInfoBackElement->SetText("Drag Units Here");
	UnitInfoBackElement->SetTextColor(0);
	//UnitInfoBackElement->SetTextWrapping(true);



	UnitInfoDisplayElement = UnitInfoLayer->CreateNewInterfaceElement("UnitInfoDisplayElement", "Background", Vector3(0, ScreenHalfDimension.y * 1.f, 0), Vector3(ScreenHalfDimension.x * 0.8f, ScreenHalfDimension.y * 2.f, 1));
	UnitInfoDisplayElement->SetTargetPosition(Vector3(0, ScreenHalfDimension.y * 1.f, 0));
	UnitInfoDisplayElement->SetTextColor(0);

	firstTime = 0;
	DraggedElement = nullptr;
	UnitSpawnMap = new UnitSpawnSystem();
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
	UnitCountElementW->SetText("Warriors: " + std::to_string(UnitSpawnMap->returnUnitMap().find(UnitSpawnSystem::U_MELEE)->second));
	UnitCountElementM->SetText("Mages: " + std::to_string(UnitSpawnMap->returnUnitMap().find(UnitSpawnSystem::U_MAGIC)->second));
	UnitCountElementR->SetText("Rangers: " + std::to_string(UnitSpawnMap->returnUnitMap().find(UnitSpawnSystem::U_RANGE)->second));

	if (deploy == true && UnitSpawnMap->getCurrentUnitCount() == 0 && UIDisplayed == true)
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
		if (DraggedElement == nullptr)
		{
			if (MeleeSlotElement->DetectUserInput(MousePos, UnitSelectLayer->GetPosition()))
			{
				DraggedElement = MeleeSlotElement;
			}

			if (MagicSlotElement->DetectUserInput(MousePos, UnitSelectLayer->GetPosition()))
			{
				DraggedElement = MagicSlotElement;
			}

			if (RangeSlotElement->DetectUserInput(MousePos, UnitSelectLayer->GetPosition()))
			{
				DraggedElement = RangeSlotElement;
			}
			if (DeployButton->DetectUserInput(MousePos, UnitSelectLayer->GetPosition()))
			{
				deploy = true;
			}

		}
	}
	else if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_UP)
	{
		if (DraggedElement != nullptr)
		{
			if (UnitInfoBackElement->DetectUserInput(MousePos, UnitInfoLayer->GetPosition()))
			{
				if (DraggedElement == MeleeSlotElement)
				{
					UnitSpawnMap->editUnit(UnitSpawnSystem::U_MELEE, 1);
					MeleeSlotElement->ResetToOriginal();
				}
				if (DraggedElement == MagicSlotElement)
				{
					UnitSpawnMap->editUnit(UnitSpawnSystem::U_MAGIC, 1);
					MagicSlotElement->ResetToOriginal();
				}
				if (DraggedElement == RangeSlotElement)
				{
					UnitSpawnMap->editUnit(UnitSpawnSystem::U_RANGE, 1);
					RangeSlotElement->ResetToOriginal();
				}
			}

			DraggedElement = nullptr;
			//MeleeSlotElement->/*SetTargetPosition(Vector3(ScreenHalfDimension.x * 1.5, ScreenHalfDimension.y * 1.2f, 1))*/ResetToOriginal();
			//MagicSlotElement->ResetToOriginal();
		}	

	}
	if (DraggedElement != nullptr)
	{
		DraggedElement->/*SetTargetPosition(MousePos)*/SetPosition(MousePos - UnitSelectLayer->GetPosition());
	}
	if (UIDisplayed == false)
	{
		MeleeSlotElement->ResetToOriginal();
		MagicSlotElement->ResetToOriginal();
		RangeSlotElement->ResetToOriginal();
		DraggedElement = nullptr;
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
		UnitSelectLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 1.5,0, 0));
		UnitInfoLayer->SetTargetPosition(Vector3(ScreenHalfDimension.x * 0.5, 0, 0));
		firstTime = 1;
	}
	else
	{
		UnitSelectLayer->SwapOriginalWithTarget();
		UnitInfoLayer->SwapOriginalWithTarget();
	}
	UnitSpawnMap->resetUnitMap();
	if (UIDisplayed == 0)
	{
		UIDisplayed = true;
	}
	else
	{
		UIDisplayed = false;
	}
	deploy = false;
	if (warningDisplayed == true)
	{
		warningDisplayed = false;
		NoUnitPopup->SwapOriginalWithTarget();
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