#include "BattleScreenInterface.h"
#include <sstream>
#include <iomanip>
#include "../Systems/GameLogicSystem.h"

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
	//Start Battle Interface
	TurnPopup = CreateNewInterfaceLayer("StartLayer", 0, 0);

	negativeHeight = -(SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight);
	CenterTarget = SceneSystem::Instance().cSS_InputManager->ScreenCenter;

	BattleScreenFrame = TurnPopup->CreateNewInterfaceElement("BattleScreenFrame", "BattleScreenFrame", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0));
	BattleScreenFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));


	BattleWordPopup = TurnPopup->CreateNewInterfaceElement("BattleWordPopUp", "BattleFont", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	BattleWordPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0));

	StartWordPopup = TurnPopup->CreateNewInterfaceElement("StartWordPopUp", "StartFont", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	StartWordPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0));

	//Show terrain info Interface
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

	TerrainInfoBackElement = TerrainInfoLayer->CreateNewInterfaceElement("TerrainInfoBackground", "Background", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.5f, 0));
	TerrainInfoBackElement->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.2f, 0));

	//Show Player unit data interface
	UnitCountInfoLayer = CreateNewInterfaceLayer("UnitCountLayer", 0, 0);

	CNOPU = CNOEU = 0;
	defaultbarSize = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0);

	PlayerUnitCountText = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCountText", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth*-0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), defaultbarSize);
	PlayerUnitCountText->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.8f, 0));
	PlayerUnitCountText->SetTextColor(0);

	PlayerUnitCount = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCount", "quad2", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.95f, 0), defaultbarSize);
	PlayerUnitCount->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.95f, 0));

	PlayerUnitCountBackground = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCountMAX", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0), defaultbarSize);
	PlayerUnitCountBackground->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0));

	EnemyUnitCountText = UnitCountInfoLayer->CreateNewInterfaceElement("EnemyUnitCountText", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth*1.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), defaultbarSize);
	EnemyUnitCountText->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.8f, 0));
	EnemyUnitCountText->SetTextColor(0);

	EnemyUnitCount = UnitCountInfoLayer->CreateNewInterfaceElement("EnemyUnitCount", "quad2", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0), defaultbarSize);
	EnemyUnitCount->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0));

	EnemyUnitCountBackground = UnitCountInfoLayer->CreateNewInterfaceElement("EnemyUnitCountMAX", "quad1", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0), defaultbarSize);
	EnemyUnitCountBackground->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0));

	PlayerRateofChange = EnemyRateofChange = 0;

	//Show Result Interface
	ResultInfoLayer = CreateNewInterfaceLayer("ResultInfoLayer", 0, 0);

	ResultInfoFrame = ResultInfoLayer->CreateNewInterfaceElement("ResultInfoFrame", "WoodFrameRect", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.54f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.44f, 0));
	CloseResultButton = ResultInfoLayer->CreateNewInterfaceElement("CloseInfo", "WoodButton", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.8f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	ResultInfo = ResultInfoLayer->CreateNewInterfaceElement("ResultInfo", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.65f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	ResultInfoBackElement = ResultInfoLayer->CreateNewInterfaceElement("ResultInfoBackground", "Background", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.4f, 0));

	ResultInfoFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, negativeHeight * 0.2f, 0));
	CloseResultButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, negativeHeight * 0.2f, 0));
	ResultInfo->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, negativeHeight * 0.15f, 0));
	ResultInfoBackElement->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, negativeHeight * 0.2f, 0));

	CloseResultButton->SetText("Continue");
	CloseResultButton->SetTextColor(Vector3(1, 1, 0));
	ResultInfo->SetTextColor(0);

	ResetAll();
}

void BattleScreenInterface::SetTerrain(Terrain* T)
{
	currentTerrain = T;
}

void BattleScreenInterface::Update(const float& dt)
{
	if (GetMaxData)
	{
		CNOMPU = GameLogicSystem::Instance().InternalBattleSystem->GetPlayerCharacterList().size();
		CNOMEU = GameLogicSystem::Instance().InternalBattleSystem->GetEnemyCharacterList().size();
		currentPlayerBarPos = PlayerUnitCountBackground->GetPosition();
		currentEnemyBarPos = EnemyUnitCountBackground->GetPosition();
		GetMaxData = false;
	}

	if (StartBattle)
	{
		CNOPU = GameLogicSystem::Instance().InternalBattleSystem->GetPlayerCharacterList().size();
		CNOEU = GameLogicSystem::Instance().InternalBattleSystem->GetEnemyCharacterList().size();


		PlayerRateofChange = defaultbarSize.x - PlayerUnitCount->GetDimensions().x;
		currentPlayerBarPos = PlayerUnitCountBackground->GetPosition() - Vector3(PlayerRateofChange * 0.5f, 0, 0);
		PlayerUnitCount->SetTargetPosition(currentPlayerBarPos);


		EnemyRateofChange = defaultbarSize.x - EnemyUnitCount->GetDimensions().x;
		currentEnemyBarPos = EnemyUnitCountBackground->GetPosition() + Vector3(EnemyRateofChange * 0.5f, 0, 0);
		EnemyUnitCount->SetTargetPosition(currentEnemyBarPos);


		PlayerUnitCount->SetDimensions(Vector3(defaultbarSize.x * ((float)CNOPU / (float)CNOMPU), PlayerUnitCount->GetDimensions().y, 0));
		EnemyUnitCount->SetDimensions(Vector3(defaultbarSize.x * ((float)CNOEU / (float)CNOMEU), EnemyUnitCount->GetDimensions().y, 0));
	}

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

	PlayerUnitCountText->SetText("Player Unit Count: " + std::to_string(CNOPU));
	EnemyUnitCountText->SetText("Enemy Unit Count: " + std::to_string(CNOEU));

	if (ShowResult)
	{
		ResultInfoLayer->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 1.3f, 0));

		if (CNOPU != 0)
		{
			ResultInfo->SetText("Player Won");
		}
		else if (CNOEU != 0)
		{
			ResultInfo->SetText("Enemy Won");
		}

		HandleUserInput();
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

void BattleScreenInterface::HandleUserInput()
{
	Vector3 MousePos = SceneSystem::Instance().cSS_InputManager->GetMousePosition();

	if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_DOWN)
	{
		if (CloseResultButton->DetectUserInput(MousePos, ResultInfoLayer->GetPosition()))
		{
			EndResult = true;
		}
	}
	else if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_UP)
	{


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
	GetMaxData = true;
	ShowResult = false;
	EndResult = false;
	TerrainInfoLayer->SetTargetPosition(Vector3(0, 0, 0));
	ResultInfoLayer->SetTargetPosition(Vector3(0, 0, 0));
	PlayerUnitCount->SetTargetPosition(PlayerUnitCountBackground->GetTargetPosition());
	EnemyUnitCount->SetTargetPosition(EnemyUnitCountBackground->GetTargetPosition());
	ResultInfoFrame->SetTargetPosition(ResultInfoFrame->GetOriginalPosition());
	ResultInfo->SetTargetPosition(ResultInfo->GetOriginalPosition());
	ResultInfoBackElement->SetTargetPosition(ResultInfoBackElement->GetOriginalPosition());
	CloseResultButton->SetTargetPosition(CloseResultButton->GetOriginalPosition());
	PlayerUnitCount->SetDimensions(defaultbarSize);
	EnemyUnitCount->SetDimensions(defaultbarSize);
}
