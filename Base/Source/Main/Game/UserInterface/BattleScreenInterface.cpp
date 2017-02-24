#include "BattleScreenInterface.h"
#include <sstream>
#include <iomanip>

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2)
{
	std::ostringstream out;
	out << std::setprecision(n) << a_value;
	return out.str();
}

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

	BattleWordPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0));
	BattleWordPopup->SetText("Battle");
	BattleWordPopup->SetTextColor(0);

	StartWordPopup = TurnPopup->CreateNewInterfaceElement("StartWordPopUp", "quad2", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	StartWordPopup->SetTargetPosition(StartWordPopup->GetOriginalPosition());
	StartWordPopup->SetText("Start!");
	StartWordPopup->SetTextColor(0);
	Initiation = true;
	Followup = false;
	Finished = false;
	
	TerrainInfoLayer = CreateNewInterfaceLayer("BottomLayer", 0, 0);

	TerrainInfoFrame = TerrainInfoLayer->CreateNewInterfaceElement("TerrainInfoBackground", "Frame", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 2.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.9f, 0));
	TerrainInfoFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.2f, 0));

	TerrainHealthBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainHealthBoostValue", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.25f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainHealthBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.25f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.2f, 0));

	TerrainMeleeBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMeleeBoostValue", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.75f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainMeleeBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.75f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.2f, 0));

	TerrainMagicBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMagicBoostValue", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.25f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainMagicBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.25f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.2f, 0));

	TerrainRangeBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainRangeBoostValue", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.75f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainRangeBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.75f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.2f, 0));

	TerrainInfoBackElement = TerrainInfoLayer->CreateNewInterfaceElement("TerrainInfoBackground", "Background", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 2, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, 0));
	TerrainInfoBackElement->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.2f, 0));

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
	TerrainHealthBoostValue->SetText("Health: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_HEALTH)->second));
	TerrainMeleeBoostValue->SetText("Melee: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_MELEE)->second));
	TerrainMagicBoostValue->SetText("Magic: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_MAGIC)->second));
	TerrainRangeBoostValue->SetText("Range: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_RANGE)->second));
   
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

	if (StartWordPopup->GetPosition().y <= StartWordPopup->GetTargetPosition().y + 3 && PopUpDone2 == false && Followup == true)
	{
		if (PoppedUp2 == false)
		{
			TurnPopupTimer = 1;
			PoppedUp2 = true;
		}
		else
		{
			if (TurnPopupTimer > 0)
			{
				TurnPopupTimer -= dt;
			}
			else
			{
				PopUpDone2 = true;
				StartWordPopup->SetTargetPosition(StartWordPopup->GetOriginalPosition());
				if (StartWordPopup->GetPosition().y <= StartWordPopup->GetTargetPosition().y + 3)
				{
					Finished = true;
				}
			}
		}
	}
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
