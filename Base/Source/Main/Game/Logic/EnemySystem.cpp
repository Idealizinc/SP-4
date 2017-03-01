#include "EnemySystem.h"
#include "../Miscellaneous/RouteGeneration.h"
#include "../Systems/GameLogicSystem.h"

EnemySystem::~EnemySystem(void)
{
	Exit();
}

void EnemySystem::Init(void)
{
	Math::InitRNG();
	RouteGeneration RG;
	for (int i = 0; i < NumberOfPaths; ++i)
	{
		RG.GeneratePath();
		InternalPathContainer.push_back(RG.DefinedPath);
	}
	CurrentTurnState = S_TURNSTART;
	TargetedNode = nullptr;
}

void EnemySystem::Exit(void)
{
	InternalPathContainer.clear();
	for (auto it : InternalEnemyContainer)
	{
		delete it;
	}
	InternalEnemyContainer.clear();
}

void EnemySystem::Update(const float& dt)
{
	std::map<std::string, unsigned short> Battalion;
	switch (CurrentTurnState)
	{
	case (S_TURNSTART) : // It's my turn, Randomize Selection
		if (InternalEnemyContainer.size() <= 0)
			CurrentTurnState = S_SPAWN;
		else{
			if (Math::RandIntMinMax(1, 10) < 3 && SceneSystem::Instance().GetCurrentScene().ScenePartition->EnemyBase->TerrainTile->EnemyUnitList.size() <= 0)
				CurrentTurnState = S_SPAWN;
			else 
				CurrentTurnState = S_MOVE;
		}
		break;
	case (S_SPAWN) : // I Spawn a Unit
		Battalion = GenerateBattalion();
		if (Battalion.size() > 0)
		{
			SelectedUnit = GenerateNewEnemy(Battalion);
			CurrentTurnState = S_TURNEND;
		}
		else CurrentTurnState = S_MOVE;
		break;
	case (S_MOVE) : // I Move a Unit
		if (InternalEnemyContainer.size() > 0)
		{
			SelectedUnit = AdvanceSingleUnit();
			CurrentTurnState = S_TURNEND;
		}
		else CurrentTurnState = S_SPAWN;
		break;
	case (S_TURNEND) : // I end my turn
		Vector3 Direction = SelectedUnit->TargetPosition - SelectedUnit->GetPosition();
		if (Direction.LengthSquared() > GameLogicSystem::Instance().PieceMinimumDistance)
		{
			SelectedUnit->SetPosition(SelectedUnit->GetPosition() + Direction * GameLogicSystem::Instance().PieceAnimationSpeed * dt);
			CameraAerial* CA = (CameraAerial*)SceneSystem::Instance().GetCurrentScene().camera;
			CA->CameraMoveTargetPosition = SelectedUnit->GetPosition();
		}
		else {
			// The animation is over, my turn is up
			GameLogicSystem::Instance().SetCurrentState(GameLogicSystem::Instance().PlayerTurn);
			// Reseting for next turn
			CurrentTurnState = S_TURNSTART;
			SelectedUnit = nullptr;
			if (TargetedNode != nullptr)
			{
				if (!GameLogicSystem::Instance().DetectWinner())
				{
					if (TargetedNode->TerrainTile->PlayerUnitList.size() > 0 && TargetedNode->TerrainTile->EnemyUnitList.size() > 0)
					{
						SceneSystem::Instance().SwitchScene("BattleScene");
						GameLogicSystem::Instance().SetCurrentState(GameLogicSystem::Instance().BattlePhase);
						GameLogicSystem::Instance().InternalBattleSystem->SetUpUnits(TargetedNode->TerrainTile);
					}
				}
				TargetedNode = nullptr;
			}
		}
		break;
	}
	for (std::vector<UnitPiece*>::iterator it = InternalEnemyContainer.begin(); it != InternalEnemyContainer.end();)
	{
		if (!(*it)->Active)
		{
			delete *it;
			it = InternalEnemyContainer.erase(it);
		}
		else ++it;
	}
}

void EnemySystem::Render(void)
{
	if (InternalEnemyContainer.size())
	for (auto it : InternalEnemyContainer)
	if (it->Active)
		it->Render();
}

EnemyPiece* EnemySystem::GenerateNewEnemy(std::map<std::string, unsigned short>& Battalion)
{
	// Create a new unit and assign him to the start
	EnemyPiece* EP = new EnemyPiece();
	EP->InternalDefinedPath = InternalPathContainer[Math::RandIntMinMax(0, InternalPathContainer.size() - 1)];
	EP->TargetPosition = EP->InternalDefinedPath.front()->GetEntity()->GetPosition() + Vector3(0, EP->InternalDefinedPath.front()->GetEntity()->GetDimensions().y + EP->GetDimensions().y);
	EP->SetPosition(EP->TargetPosition + Vector3(0, 10, 0));
	EP->InternalBattalionList = Battalion;
	InternalEnemyContainer.push_back(EP);
	// Add to the tile he is on
	EP->InternalDefinedPath.front()->TerrainTile->EnemyUnitList.push_back(EP);
	EP->TargetNode = SceneSystem::Instance().GetCurrentScene().ScenePartition->EnemyBase;
	return EP;
}

EnemyPiece* EnemySystem::AdvanceSingleUnit()
{
	// Select a piece that can move
	EnemyPiece* EP = RandomizePieceSelection();
	while (EP->InternalDefinedPath.size() <= 1)
		EP = RandomizePieceSelection();
	// Remove from the old tile
	for (std::vector<UnitPiece*>::iterator it = EP->InternalDefinedPath.front()->TerrainTile->EnemyUnitList.begin(); it != EP->InternalDefinedPath.front()->TerrainTile->EnemyUnitList.end(); ++it)
	{
		if (*it == EP)
		{
			EP->InternalDefinedPath.front()->TerrainTile->EnemyUnitList.erase(it);
			break;
		}
	}
	// Pop the front of his current path
	EP->InternalDefinedPath.erase(EP->InternalDefinedPath.begin());
	// Add to the tile he is on
	EP->InternalDefinedPath.front()->TerrainTile->EnemyUnitList.push_back(EP);
	TargetedNode = EP->InternalDefinedPath.front();
	EP->TargetNode = TargetedNode;
	EP->TargetPosition = EP->InternalDefinedPath.front()->GetEntity()->GetPosition() + Vector3(0, EP->InternalDefinedPath.front()->GetEntity()->GetDimensions().y + EP->GetDimensions().y);
	return EP;
}

EnemyPiece* EnemySystem::RandomizePieceSelection()
{
	// Might want to add some selection conditions here
	EnemyPiece* EP = (EnemyPiece*)InternalEnemyContainer[Math::RandIntMinMax(0, InternalEnemyContainer.size() - 1)];
	return EP;
}

std::map<std::string, unsigned short> EnemySystem::GenerateBattalion()
{
	std::map<std::string, unsigned short> Battalion;
	// Parameters
	int CurrentCost = 0;
	int MaxCost = GameLogicSystem::Instance().UnitInterface->returnUnitSpawnSys()->maxUnitCost;
	if (MaxCost > RemainingGold)
		MaxCost = RemainingGold;
	std::vector<UnitType*> ViableUnitList;
	std::map<std::string, UnitType*>UnitMap;
	if (GameLogicSystem::Instance().PlayerFaction != GameLogicSystem::F_LIVING)
		UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap;
	else
		UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap;

	while (true)
	{
		// Set Up Viable List
		ViableUnitList.clear();
		int MaximumViableCost = MaxCost - CurrentCost;
		for (auto it : UnitMap)
		{
			if (it.second->GetCost() < MaximumViableCost)
			{
				// I can afford this unit!
				ViableUnitList.push_back(it.second);
			}
		}
		if (ViableUnitList.size() <= 0)
			break;
		// Now to think of whether I'd like to buy anything
		int Randomizer = Math::RandIntMinMax(0, 100);
		int SpawnChance = (int)(100.f * SpawningCoefficient * (((float)MaxCost - (float)CurrentCost) / (float)MaxCost));

		if (SpawnChance >= Randomizer) // The Greater my current cost, the lower the RHS value becomes [The spawn chance]
		{
			// I have decided on buying a unit.
			UnitType* Choice = ViableUnitList[Math::RandIntMinMax(0, ViableUnitList.size() - 1)];
			auto it = Battalion.find(Choice->GetName());
			if (it == Battalion.end())
			{
				// This is an absolutely new unit that I've decided to purchase!
				Battalion.insert(std::pair<std::string, unsigned short>(Choice->GetName(), 1));
			}
			else it->second += 1;
			CurrentCost += Choice->GetCost();
		}
		else break; // Time to make payment
	}
	RemainingGold -= CurrentCost;
	return Battalion;
}