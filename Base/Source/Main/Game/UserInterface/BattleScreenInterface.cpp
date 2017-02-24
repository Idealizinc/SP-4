#include "BattleScreenInterface.h"

BattleScreenInterface::BattleScreenInterface()
{
	Init();
}

BattleScreenInterface::~BattleScreenInterface()
{
	Exit();
}

void BattleScreenInterface::Init()
{
	TurnPopupTimer = 0;
	TurnPopup = CreateNewInterfaceLayer("StartLayer", 0, 0);

	BattleWordPopup = TurnPopup->CreateNewInterfaceElement("BattleWordPopUp", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	BattleWordPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.5, 0));
	BattleWordPopup->SetText("Battle");
	BattleWordPopup->SetTextColor(0);
	BattleWordPopup->SetVelocity(Vector3(0, -10, 0));

	StartWordPopup = TurnPopup->CreateNewInterfaceElement("StartWordPopUp", "quad2", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	StartWordPopup->SetTargetPosition(StartWordPopup->GetOriginalPosition());
	StartWordPopup->SetText("Start!");
	StartWordPopup->SetTextColor(0);
	Initiation = true;
	Followup = false;
	Finished = false;

	//TerrainInfoLayer = CreateNewInterfaceLayer("Bottom", 0, 0);
	//TerrainInfoLayer->SetOriginalPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.8f, 0));
	//
	//TerrainInfoBackElement = TerrainInfoLayer->CreateNewInterfaceElement("TerrainInfoBackground", "Background", Vector3(TerrainInfoLayer->GetPosition()), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.8f, 1));
	//TerrainInfoFrame = TerrainInfoLayer->CreateNewInterfaceElement("TerrainInfoBackground", "Background", Vector3(TerrainInfoBackElement->GetPosition()), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.8f, 1));
	//TerrainHealthBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainHealthBoostValue", "quad1", Vector3(TerrainInfoBackElement->GetPosition().x * 0.2f, TerrainInfoBackElement->GetPosition().y, 1), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.8f, 1));
	//TerrainMeleeBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMeleeBoostValue", "quad1", Vector3(TerrainInfoBackElement->GetPosition().x * 0.2f, TerrainInfoBackElement->GetPosition().y, 1), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.8f, 1));
	//TerrainMagicBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMagicBoostValue", "quad1", Vector3(TerrainInfoBackElement->GetPosition().x * 0.2f, TerrainInfoBackElement->GetPosition().y, 1), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.8f, 1));
	//TerrainRangeBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainRangeBoostValue", "quad1", Vector3(TerrainInfoBackElement->GetPosition().x * 0.2f, TerrainInfoBackElement->GetPosition().y, 1), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.8f, 1));
}

void BattleScreenInterface::SetTerrain(Terrain* T)
{
	currentTerrain = T;
}

void BattleScreenInterface::Update(const float& dt)
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active)
			it->Update(dt * 10);
	}
	PopUpDelay(dt);

	//TerrainHealthBoostValue->SetText("Health: x" + std::to_string(currentTerrain->UnitAdvantage.find(Terrain::T_HEALTH)->second));
	//TerrainMeleeBoostValue->SetText("Melee: x" + std::to_string(currentTerrain->UnitAdvantage.find(Terrain::T_MELEE)->second));
	//TerrainMagicBoostValue->SetText("Magic: x" + std::to_string(currentTerrain->UnitAdvantage.find(Terrain::T_MAGIC)->second));
	//TerrainRangeBoostValue->SetText("Range: x" + std::to_string(currentTerrain->UnitAdvantage.find(Terrain::T_RANGE)->second));
   
	if (Initiation && Followup)
	{
		PopUpReset();
	}
}

void BattleScreenInterface::Render()
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active && it->Visible)
			it->Render();
	}
}

void BattleScreenInterface::Exit()
{
	if (Finished)
	{
		ResetAll();
	}

	for (auto it : InternalLayerContainer)
	{
		it->Exit();
		delete it;
	}
	InternalLayerContainer.clear();
}

void BattleScreenInterface::PopUpDelay(const float& dt)
{
	if (BattleWordPopup->GetPosition().y <= BattleWordPopup->GetTargetPosition().y + 3 && PopUpDone == false && Initiation == true)
	{
		if (PoppedUp == false)
		{
			TurnPopupTimer = 1;
			PoppedUp = true;
		}
		else
		{
			if (TurnPopupTimer > 0)
			{
				TurnPopupTimer -= dt;
			}
			else
			{
				PopUpDone = true;
				BattleWordPopup->SetTargetPosition(BattleWordPopup->GetOriginalPosition());
				if (BattleWordPopup->GetPosition().y <= BattleWordPopup->GetTargetPosition().y + 3)
				{
					Followup = true;
				}
			}
		}
	}

	else if (StartWordPopup->GetPosition().y <= StartWordPopup->GetTargetPosition().y + 3 && PopUpDone == false && Followup == true)
	{
		if (PoppedUp == false)
		{
			TurnPopupTimer = 1;
			PoppedUp = true;
		}
		else
		{
			if (TurnPopupTimer > 0)
			{
				TurnPopupTimer -= dt;
			}
			else
			{
				PopUpDone = true;
				StartWordPopup->SetTargetPosition(StartWordPopup->GetOriginalPosition());
				if (StartWordPopup->GetPosition().y <= StartWordPopup->GetTargetPosition().y + 3)
				{
					Finished = true;
				}
			}
		}
	}

	if (Followup && BattleWordPopup->GetPosition() != BattleWordPopup->GetOriginalPosition())
		BattleWordPopup->ResetToOriginal();
	if (Finished && StartWordPopup->GetPosition() != StartWordPopup->GetOriginalPosition())
		StartWordPopup->ResetToOriginal();
}

void BattleScreenInterface::PopUpReset()
{
	PopUpDone = false;
	PoppedUp = false;
	if (Initiation == true)
	{
		StartWordPopup->SetTargetPosition(CenterTarget);
		Initiation = false;
	}
}

void BattleScreenInterface::ResetAll()
{
	PopUpDone = false;
	PoppedUp = false;
	Initiation = false;
	Finished = false;
	Followup = false;

}