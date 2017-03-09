#include "BattleScreenInterface.h"
#include <sstream>
#include <iomanip>
#include "../Systems/GameLogicSystem.h"
#include "../Miscellaneous/CameraAerial.h"
#include "../Logic/Characters/BaseClasses/BattleScreenCharacter.h"

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


	negativeHeight = -((float)SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight);

	CenterTarget = SceneSystem::Instance().cSS_InputManager->ScreenCenter;

	BattleScreenFrame = TurnPopup->CreateNewInterfaceElement("BattleScreenFrame", "BattleScreenFrame", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0));
	BattleScreenFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));


	BattleWordPopup = TurnPopup->CreateNewInterfaceElement("BattleWordPopUp", "BattleFont", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	BattleWordPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0));

	StartWordPopup = TurnPopup->CreateNewInterfaceElement("StartWordPopUp", "StartFont", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	StartWordPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0));

	//Show terrain info Interface
	TerrainInfoLayer = CreateNewInterfaceLayer("LeftLayer", 0, 0);

	TerrainName = TerrainInfoLayer->CreateNewInterfaceElement("TerrainName", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.35f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.25f, 0));
	TerrainName->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.5f, 0));
	TerrainName->SetTextColor(Vector3(1, 1, 1));

	TerrainAdvantage = TerrainInfoLayer->CreateNewInterfaceElement("TerrainAdvantage", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.25f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainAdvantage->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.25f, 0));
	TerrainAdvantage->SetTextColor(Vector3(0,1,0));

	TerrainHealthBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainHealthBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.16f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.15f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainHealthBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.16f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y* 1.15f, 0));
	TerrainHealthBoostValue->SetTextColor(Vector3(1, 1, 1));

	TerrainMeleeBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMeleeBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.36f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.15f, 00), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMeleeBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.36f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.15f, 0));
	TerrainMeleeBoostValue->SetTextColor(Vector3(1, 1, 1));

	TerrainMagicBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMagicBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.56f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.15f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMagicBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.56f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y* 1.15f, 0));
	TerrainMagicBoostValue->SetTextColor(Vector3(1, 1, 1));

	TerrainRangeBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainRangeBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.76f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.15f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainRangeBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.76f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y* 1.15f, 0));
	TerrainRangeBoostValue->SetTextColor(Vector3(1, 1, 1));

	TerrainDisadvantage = TerrainInfoLayer->CreateNewInterfaceElement("TerrainDisAdvantage", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainDisadvantage->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));
	TerrainDisadvantage->SetTextColor(Vector3(1, 0, 0));

	TerrainHealthMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainHealthMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.16f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.9f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainHealthMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.16f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 0.9f, 0));
	TerrainHealthMinusValue->SetTextColor(Vector3(1, 1, 1));

	TerrainMeleeMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMeleeMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.36f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.9f, 00), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMeleeMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.36f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 0.9f, 0));
	TerrainMeleeMinusValue->SetTextColor(Vector3(1, 1, 1));

	TerrainMagicMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMagicMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.65f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.9f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMagicMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.56f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 0.9f, 0));
	TerrainMagicMinusValue->SetTextColor(Vector3(1, 1, 1));

	TerrainRangeMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainRangeMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.76f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.9f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainRangeMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.76f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 0.9f, 0));
	TerrainRangeMinusValue->SetTextColor(Vector3(1, 1, 1));

	TerrainAdvantageBoost = TerrainInfoLayer->CreateNewInterfaceElement("TerrainAdvantageBoost", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.6f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainAdvantageBoost->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.6f, 0));

	TerrainFrameButton = TerrainInfoLayer->CreateNewInterfaceElement("TerrainFrameButton", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.98f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.025f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.075f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	TerrainFrameButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.98f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y* 1.025f, 0));

	TerrainInfoFrame = TerrainInfoLayer->CreateNewInterfaceElement("TerrainInfoBackground", "InfoFrame", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.3f, 0));
	TerrainInfoFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));

	//Show player unit data info interface


	//Show Player unit data interface
	UnitCountInfoLayer = CreateNewInterfaceLayer("UnitCountLayer", 0, 0);

	CNOPU = CNOEU = 0;
	defaultbarSize = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0);

	PlayerFactionNameText = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCountText", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.54f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.88f, 0), defaultbarSize * 0.8f);
	PlayerFactionNameText->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.54f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.88f, 0));
	PlayerFactionNameBack = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCountText", "BlackBack", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.53f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.88f, 0), Vector3(defaultbarSize.x * 0.6f, defaultbarSize.y * 0.5f, 0));
	PlayerFactionNameBack->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.53f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.88f, 0));

	PlayerUnitCount = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCount", "GreenBar", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.95f, 0), defaultbarSize);
	PlayerUnitCount->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.95f, 0));

	PlayerUnitCountBackground = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCountMAX", "RedBar", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0), defaultbarSize);
	PlayerUnitCountBackground->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0));

	EnemyFactionNameText = UnitCountInfoLayer->CreateNewInterfaceElement("EnemyUnitCountText", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x*1.47f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.88f, 0), defaultbarSize * 0.8f);
	EnemyFactionNameText->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.47f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.88f, 0));
	EnemyFactionNameBack = UnitCountInfoLayer->CreateNewInterfaceElement("EnemyUnitCountText", "BlackBack", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x*1.47f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.88f, 0), Vector3(defaultbarSize.x * 0.6f, defaultbarSize.y * 0.5f, 0));
	EnemyFactionNameBack->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.47f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.88f, 0));

	EnemyUnitCount = UnitCountInfoLayer->CreateNewInterfaceElement("EnemyUnitCount", "GreenBar", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0), defaultbarSize);
	EnemyUnitCount->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0));

	EnemyUnitCountBackground = UnitCountInfoLayer->CreateNewInterfaceElement("EnemyUnitCountMAX", "RedBar", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0), defaultbarSize);
	EnemyUnitCountBackground->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0));

	PlayerRateofChange = EnemyRateofChange = 0;

	//Show Result Interface
	ResultInfoLayer = CreateNewInterfaceLayer("ResultInfoLayer", 0, 0);

	ResultInfoFrame = ResultInfoLayer->CreateNewInterfaceElement("ResultInfoFrame", "WoodFrameRect", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.54f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.44f, 0));
	CloseResultButton = ResultInfoLayer->CreateNewInterfaceElement("CloseInfo", "WoodButton", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.8f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
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

		RecreateDataLayer();

		GetMaxData = false;

		CameraAerial* CA = (CameraAerial*)SceneSystem::Instance().GetCurrentScene().camera;
		CA->CameraMoveTargetPosition = SceneSystem::Instance().GetCurrentScene().ScenePartition->PlayerSpawn->GetEntity()->GetPosition();
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
	TerrainName->SetText(currentTerrain->GetTerrainName());

	if (GameLogicSystem::Instance().InternalBattleSystem->CurrentBattleTile->PlayerHasAdvantage)
	{
		TerrainAdvantageBoost->SetText("I have advantage.");
		TerrainAdvantageBoost->SetTextColor(Vector3(0, 1, 0));
	}
	else
	{
		TerrainAdvantageBoost->SetText("I have no advantage.");
		TerrainAdvantageBoost->SetTextColor(Vector3(1, 0, 0));
	}

	TerrainAdvantage->SetText("Advantage");
	if ((int)(currentTerrain->UnitAdvantage.find(Terrain::T_HEALTH)->second * 100 - 100) > 0)
	{
		TerrainHealthBoostValue->SetText("Health+" + std::to_string((int)(currentTerrain->UnitAdvantage.find(Terrain::T_HEALTH)->second * 100 - 100)) + "%");
		TerrainHealthBoostValue->SetTextColor(Vector3(0, 1, 0));
	}
	else
		TerrainHealthBoostValue->SetText("Health+" + std::to_string((int)(currentTerrain->UnitAdvantage.find(Terrain::T_HEALTH)->second * 100 - 100)) + "%");

	if ((int)(currentTerrain->UnitAdvantage.find(Terrain::T_MELEE)->second * 100 - 100) > 0)
	{
		TerrainMeleeBoostValue->SetText("Melee+" + std::to_string((int)(currentTerrain->UnitAdvantage.find(Terrain::T_MELEE)->second * 100 - 100)) + "%");
		TerrainMeleeBoostValue->SetTextColor(Vector3(0, 1, 0));
	}
	else
		TerrainMeleeBoostValue->SetText("Melee+" + std::to_string((int)(currentTerrain->UnitAdvantage.find(Terrain::T_MELEE)->second * 100 - 100)) + "%");

	if ((int)(currentTerrain->UnitAdvantage.find(Terrain::T_MAGIC)->second * 100 - 100) > 0)
	{
		TerrainMagicBoostValue->SetText("Magic+" + std::to_string((int)(currentTerrain->UnitAdvantage.find(Terrain::T_MAGIC)->second * 100 - 100)) + "%");
		TerrainMagicBoostValue->SetTextColor(Vector3(0, 1, 0));
	}
	else
		TerrainMagicBoostValue->SetText("Magic+" + std::to_string((int)(currentTerrain->UnitAdvantage.find(Terrain::T_MAGIC)->second * 100 - 100)) + "%");

	if ((int)(currentTerrain->UnitAdvantage.find(Terrain::T_RANGE)->second * 100 - 100) > 0)
	{
		TerrainRangeBoostValue->SetText("Range+" + std::to_string((int)(currentTerrain->UnitAdvantage.find(Terrain::T_RANGE)->second * 100 - 100)) + "%");
		TerrainRangeBoostValue->SetTextColor(Vector3(0, 1, 0));
	}
	else
		TerrainRangeBoostValue->SetText("Range+" + std::to_string((int)(currentTerrain->UnitAdvantage.find(Terrain::T_RANGE)->second * 100 - 100)) + "%");

	TerrainDisadvantage->SetText("Disdvantage");
	if ((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_HEALTH)->second * 100 - 100) < 0)
	{
		TerrainHealthMinusValue->SetText("Health" + std::to_string((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_HEALTH)->second * 100 - 100)) + "%");
		TerrainHealthMinusValue->SetTextColor(Vector3(1, 0, 0));
	}
	else
		TerrainHealthMinusValue->SetText("Health-" + std::to_string((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_HEALTH)->second * 100 - 100)) + "%");

	if ((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_MELEE)->second * 100 - 100) < 0)
	{
		TerrainMeleeMinusValue->SetText("Melee" + std::to_string((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_MELEE)->second * 100 - 100)) + "%");
		TerrainMeleeMinusValue->SetTextColor(Vector3(1, 0, 0));
	}
	else
		TerrainMeleeMinusValue->SetText("Melee-" + std::to_string((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_MELEE)->second * 100 - 100)) + "%");

	if ((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_MAGIC)->second * 100 - 100) < 0)
	{
		TerrainMagicMinusValue->SetText("Magic" + std::to_string((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_MAGIC)->second * 100 - 100)) + "%");
		TerrainMagicMinusValue->SetTextColor(Vector3(1, 0, 0));
	}
	else
		TerrainMagicMinusValue->SetText("Magic-" + std::to_string((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_MAGIC)->second * 100 - 100)) + "%");

	if ((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_RANGE)->second * 100 - 100) < 0)
	{
		TerrainRangeMinusValue->SetText("Range" + std::to_string((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_RANGE)->second * 100 - 100)) + "%");
		TerrainRangeMinusValue->SetTextColor(Vector3(1, 0, 0));
	}
	else
		TerrainRangeMinusValue->SetText("Range-" + std::to_string((int)(currentTerrain->UnitDisadvantage.find(Terrain::T_RANGE)->second * 100 - 100)) + "%");

	if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
	{
		PlayerFactionNameText->SetText("Player Unit Count:" + std::to_string(CNOPU));
		PlayerFactionNameText->SetTextColor(Vector3(1, 1, 0));
		EnemyFactionNameText->SetText("Enemy Unit Count:" + std::to_string(CNOEU));
		EnemyFactionNameText->SetTextColor(Vector3(1, 0, 0));
	}
	else
	{
		PlayerFactionNameText->SetText("Player Unit Count:" + std::to_string(CNOPU));
		PlayerFactionNameText->SetTextColor(Vector3(1, 0, 0));
		EnemyFactionNameText->SetText("Enemy Unit Count:" + std::to_string(CNOEU));
		EnemyFactionNameText->SetTextColor(Vector3(1, 1, 0));
	}

	if (ShowResult)
	{
		ResultInfoLayer->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 1.3f, 0));
		TerrainInfoLayer->SetTargetPosition(Vector3(-(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x) * 0.925f, 0, 0));
		UnitDataLayer->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.925f, 0, 0));
		if (CNOPU != 0)
		{
			PlayerWon = true;
			ResultInfo->SetText("Player Won");
		}
		else if (CNOEU != 0)
		{
			EnemyWon = true;
			ResultInfo->SetText("Enemy Won");
		}
	}
	HandleUserInput();
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
			UnitMap.clear();
			if (UnitDataLayer)
			{
				for (std::vector<InterfaceLayer*>::iterator it = InternalLayerContainer.begin(); it != InternalLayerContainer.end(); ++it)
					if ((*it)->GetEntityID() == UnitDataLayer->GetEntityID())
					{
						InternalLayerContainer.erase(it);
						break;
					}
				UnitDataLayer->Exit();
				delete UnitDataLayer;
				UnitDataLayer = nullptr;
				UnitDataFrameButton = nullptr;
			}

		}

		if (TerrainFrameButton->DetectUserInput(MousePos, TerrainInfoLayer->GetPosition()))
		{
			if ((TerrainInfoLayer->GetPosition() - TerrainInfoLayer->GetTargetPosition()).LengthSquared() < 1)
				TerrainInfoLayer->SwapOriginalWithTarget();
			else
				TerrainInfoLayer->SwapOriginalWithTarget();
		}
		else if (UnitDataFrameButton)
		if (UnitDataFrameButton->DetectUserInput(MousePos, UnitDataLayer->GetPosition()))
		{
			if ((UnitDataLayer->GetPosition() - UnitDataLayer->GetTargetPosition()).LengthSquared() < 1)
				UnitDataLayer->SwapOriginalWithTarget();
			else
				UnitDataLayer->SwapOriginalWithTarget();
		}
	}
	else if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_UP)
	{


	}
}

void BattleScreenInterface::PopUpDelay(const float& dt)
{
	if (ShowTerrainInfo)
	{
		TerrainInfoTimer -= dt;
		if (TerrainInfoTimer <= 0)
		{
			TerrainInfoLayer->SetTargetPosition(Vector3(-(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x) * 0.925f, 0, 0));
			//TerrainInfoLayer->SetPosition(TerrainInfoLayer->GetTargetPosition());
			UnitDataLayer->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.925f, 0, 0));
			//UnitDataLayer->SetPosition(UnitDataLayer->GetTargetPosition());
			if (TerrainInfoLayer->GetPosition().x <= TerrainInfoLayer->GetTargetPosition().x + 3)
			{
				ShowTerrainInfo = false;
				Initiation = true;
			}
		}
	}

	else if (Initiation)
	{
		TurnPopupTimer -= dt;
		if (TurnPopupTimer <= 0)
		{
			TurnPopoutTimer -= dt;
			BattleWordPopup->SetTargetPosition(CenterTarget);
			if (TurnPopoutTimer <= 0)
			{
				BattleWordPopup->SetTargetPosition(BattleWordPopup->GetOriginalPosition());

				if ((BattleWordPopup->GetPosition() - BattleWordPopup->GetTargetPosition()).LengthSquared() < 1)
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

void BattleScreenInterface::RecreateDataLayer()
{
	UnitDataLayer = CreateNewInterfaceLayer("RightLayer", 0, 0);

	UnitDataFrameButton = UnitDataLayer->CreateNewInterfaceElement("UnitDtatFrameButton", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 1.02f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.025f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.075f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	UnitDataFrameButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.02f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.025f, 0));

	UnitDataInfoText = UnitDataLayer->CreateNewInterfaceElement("UnitDtatInfoText", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 1.53f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.25f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.125f, 0));
	UnitDataInfoText->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.53f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.5f, 0));
	UnitDataInfoText->SetText("Unit  Equipped Weapon  Quantity");

	int Counter = 0;
	float offSet = SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 0.08f;
	Vector3 OriginPos = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 1.18f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.35f, 0);
	Vector3 IconDimensions = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0);
	Vector3 WeaponBarDimensions = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0);

	for (auto it : GameLogicSystem::Instance().InternalBattleSystem->GetPlayerCharacterList())
	{
		BattleScreenCharacter * BSC = (BattleScreenCharacter *)it;
		UnitDataIcon = nullptr;

		auto it2 = UnitMap.find(it->GetMeshName());
		if (it2 == UnitMap.end())
		{
			std::map<std::string, int> Temp;
			Temp.insert(std::pair<std::string, int>(BSC->InternalWeapon->GetEntityID(), 1));
			UnitMap.insert(std::pair<std::string, std::map<std::string, int>>(it->GetMeshName(), Temp));
			it2 = UnitMap.find(it->GetMeshName());
		}
		else
		{
			auto it3 = it2->second.find(BSC->InternalWeapon->GetEntityID());
			if (it3 == it2->second.end())
			{
				it2->second.insert(std::pair<std::string, int>(BSC->InternalWeapon->GetEntityID(), 1));
			}
			else
			{
				it3->second++;
			}
		}
		//UnitWeaponDataName = UnitDataLayer->CreateNewInterfaceElement("Name" + BSC->InternalWeapon->GetEntityID(), "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 1.53f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.4f, 0), Vector3(50, 50, 0));
	}



	for (auto it : UnitMap)
	{
		// decide how to offset the bars
		float barOffset = SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.1f;
		Vector3 barOriginPos = Vector3(/*UnitDataIcon->GetPosition().x*/OriginPos.x  * 1.27f, OriginPos.y, 0);
		Vector3 textDimensions = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0);

		for (auto it2 : it.second)
		{
			Vector3 CurrentUIPos = OriginPos + Vector3(0, -offSet * Counter);
			// create the bars with the info
			UnitDataIcon = UnitDataLayer->CreateNewInterfaceElement("Icon" + it.first, it.first, CurrentUIPos, IconDimensions);
			UnitDataIcon->SetTargetPosition(CurrentUIPos);

			UnitWeaponDataName = UnitDataLayer->CreateNewInterfaceElement("Icon" + it.first, "Transparent", CurrentUIPos + Vector3(barOffset * 3, 0, 0), textDimensions);
			UnitWeaponDataName->SetTargetPosition(CurrentUIPos + Vector3(barOffset * 3, 0, 0));
			UnitWeaponDataName->SetText(it2.first);

			UnitWeaponDataNumber = UnitDataLayer->CreateNewInterfaceElement("Icon" + it.first, "Transparent", CurrentUIPos + Vector3(barOffset * 6.5f, 0, 0), textDimensions);
			UnitWeaponDataNumber->SetTargetPosition(CurrentUIPos + Vector3(barOffset * 6.5f, 0, 0));
			UnitWeaponDataNumber->SetText(std::to_string(it2.second));

			++Counter;
		}

	}

	UnitDataInfoFrame = UnitDataLayer->CreateNewInterfaceElement("UnitDataInfoBackground", "InfoFrame2", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.3f, 0));
	UnitDataInfoFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));
}

void BattleScreenInterface::ResetAll()
{
	ResetAllToOriginal();
	TurnPopupTimer = 1;
	TurnPopoutTimer = 1;
	TurnPopupTimer2 = 0.5f;
	TurnPopoutTimer2 = 1;
	TerrainInfoTimer = 2;
	ShowTerrainInfo = true;
	Initiation = false;
	Finished = false;
	Followup = false;
	StartBattle = false;
	GetMaxData = true;
	ShowResult = false;
	EndResult = false;
	PlayerWon = false;
	EnemyWon = false;
	TerrainInfoLayer->SetTargetPosition(Vector3(0));
	TerrainInfoLayer->SetPosition(TerrainInfoLayer->GetTargetPosition());
	//UnitDataLayer->SetTargetPosition(Vector3(0));
	//UnitDataLayer->SetPosition(UnitDataLayer->GetTargetPosition());
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
