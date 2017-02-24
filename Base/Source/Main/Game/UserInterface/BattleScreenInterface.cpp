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
	TurnPopup = CreateNewInterfaceLayer("StartLayer", 0, 0);

	negativeHeight = -((int)SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight);
	CenterTarget = SceneSystem::Instance().cSS_InputManager->ScreenCenter;

	BattleWordPopup = TurnPopup->CreateNewInterfaceElement("BattleWordPopUp", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	BattleWordPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0));
	BattleWordPopup->SetText("Battle");
	BattleWordPopup->SetTextColor(0);

	StartWordPopup = TurnPopup->CreateNewInterfaceElement("StartWordPopUp", "quad2", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	StartWordPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0));
	StartWordPopup->SetText("Start!");
	StartWordPopup->SetTextColor(0);

	TerrainInfoLayer = CreateNewInterfaceLayer("BottomLayer", 0, 0);

	TerrainInfoFrame = TerrainInfoLayer->CreateNewInterfaceElement("TerrainInfoBackground", "WoodFrameRect", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.5f, 0));
	TerrainInfoFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.2f, 0));

	TerrainAdvantage = TerrainInfoLayer->CreateNewInterfaceElement("TerrainAdvantage", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainAdvantage->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.27f, 0));
	TerrainAdvantage->SetTextColor(0);

	TerrainHealthBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainHealthBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.75f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainHealthBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.71f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.22f, 0));
	TerrainHealthBoostValue->SetTextColor(0);

	TerrainMeleeBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMeleeBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.95f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMeleeBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.91f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.22f, 0));
	TerrainMeleeBoostValue->SetTextColor(0);

	TerrainMagicBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMagicBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMagicBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.11f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.22f, 0));
	TerrainMagicBoostValue->SetTextColor(0);

	TerrainRangeBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainRangeBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.35f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainRangeBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.31f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.22f, 0));
	TerrainRangeBoostValue->SetTextColor(0);

	TerrainDisadvantage = TerrainInfoLayer->CreateNewInterfaceElement("TerrainDisAdvantage", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainDisadvantage->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.17f, 0));
	TerrainDisadvantage->SetTextColor(0);

	TerrainHealthMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainHealthMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.75f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainHealthMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.71f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.12f, 0));
	TerrainHealthMinusValue->SetTextColor(0);

	TerrainMeleeMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMeleeMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.95f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMeleeMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.91f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.12f, 0));
	TerrainMeleeMinusValue->SetTextColor(0);

	TerrainMagicMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMagicMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMagicMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.11f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.12f, 0));
	TerrainMagicMinusValue->SetTextColor(0);

	TerrainRangeMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainRangeMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.35f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainRangeMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.31f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.12f, 0));
	TerrainRangeMinusValue->SetTextColor(0);

	TerrainInfoBackElement = TerrainInfoLayer->CreateNewInterfaceElement("TerrainInfoBackground", "Background", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.4f, 0));
	TerrainInfoBackElement->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.2f, 0));

	UnitCountInfoLayer = CreateNewInterfaceLayer("UnitCountLayer", 0, 0);

	PlayerUnitCount = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCount", "quad2", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth*-0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	PlayerUnitCount->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.71f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.22f, 0));

	EnemyUnitCount = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCount", "quad2", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth*1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	EnemyUnitCount->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.71f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.22f, 0));


	ResetAll();
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
	TerrainAdvantage->SetText("Advantage");	
	TerrainHealthBoostValue->SetText("Health: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_HEALTH)->second));
	TerrainMeleeBoostValue->SetText("Melee: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_MELEE)->second));
	TerrainMagicBoostValue->SetText("Magic: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_MAGIC)->second));
	TerrainRangeBoostValue->SetText("Range: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_RANGE)->second));
	TerrainDisadvantage->SetText("Disdvantage");
	TerrainHealthMinusValue->SetText("Health: x" + to_string_with_precision(currentTerrain->UnitDisadvantage.find(Terrain::T_HEALTH)->second));
	TerrainMeleeMinusValue->SetText("Melee: x" + to_string_with_precision(currentTerrain->UnitDisadvantage.find(Terrain::T_MELEE)->second));
	TerrainMagicMinusValue->SetText("Magic: x" + to_string_with_precision(currentTerrain->UnitDisadvantage.find(Terrain::T_MAGIC)->second));
	TerrainRangeMinusValue->SetText("Range: x" + to_string_with_precision(currentTerrain->UnitDisadvantage.find(Terrain::T_RANGE)->second));
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
	if (TerrainInfoFrame->GetPosition().y <= TerrainInfoFrame->GetTargetPosition().y + 3)
	{
		if (ShowTerrainInfo == false)
		{
			TerrainInfoTimer -= dt;
			if (TerrainInfoTimer <= 0)
			{
				TerrainInfoLayer->SetTargetPosition(Vector3(0, (float)negativeHeight, 0));
				if (TerrainInfoLayer->GetPosition().y >= TerrainInfoLayer->GetTargetPosition().y + 3)
				{
					Initiation = true;
					ShowTerrainInfo = true;
				}
			}
		}
	}

	if (Initiation)
	{
		TurnPopupTimer -= dt;
		if (TurnPopupTimer <= 0)
		{
			TurnPopoutTimer -= dt;
			BattleWordPopup->SetTargetPosition(CenterTarget);
			if (TurnPopoutTimer <= 0)
			{
				BattleWordPopup->SetTargetPosition(BattleWordPopup->GetOriginalPosition());

				if (BattleWordPopup->GetPosition().y <= BattleWordPopup->GetTargetPosition().y + 3)
				{
					Followup = true;
					Initiation = false;
				}
			}
		}
	}

	else if (Followup)
	{
		TurnPopupTimer2 -= dt;
		if (TurnPopupTimer2 <= 0)
		{
			TurnPopoutTimer2 -= dt;
			StartWordPopup->SetTargetPosition(CenterTarget);
			if (TurnPopoutTimer2  <= 0)
			{
				StartWordPopup->SetTargetPosition(StartWordPopup->GetOriginalPosition());
				if (StartWordPopup->GetPosition().y <= StartWordPopup->GetTargetPosition().y + 3)
				{
					Finished = true;
					StartBattle = true;
				}
			}
		}
	}
}


void BattleScreenInterface::ResetAll()
{
	ResetAllToOriginal();
	TurnPopupTimer = 1;
	TurnPopoutTimer = 1;
	TurnPopupTimer2 = 0.5f;
	TurnPopoutTimer2 = 1;
	TerrainInfoTimer = 2;
	ShowTerrainInfo = false;
	Initiation = false;
	Finished = false;
	Followup = false;
	StartBattle = false;
	TerrainInfoLayer->SetTargetPosition(Vector3(0, 0, 0));
}
