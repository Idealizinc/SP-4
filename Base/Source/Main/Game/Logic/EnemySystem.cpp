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
	for (std::vector<UnitPiece*>::iterator it = InternalEnemyContainer.begin(); it != InternalEnemyContainer.end();)
	{
		if (!(*it)->Active)
		{
			delete *it;
			it = InternalEnemyContainer.erase(it);
		}
		else ++it;
	}

	switch (CurrentTurnState)
	{
	case (S_TURNSTART) : // It's my turn, Randomize Selection
		if (InternalEnemyContainer.size() <= 0)
			CurrentTurnState = S_SPAWN;
		else{
			if (Math::RandIntMinMax(1, 10) > 2)
				CurrentTurnState = S_MOVE;
			else CurrentTurnState = S_SPAWN;
		}
		break;
	case (S_SPAWN) : // I Spawn a Unit
		SelectedUnit = GenerateNewEnemy();
		CurrentTurnState = S_TURNEND;
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
				if (TargetedNode->TerrainTile->PlayerUnitList.size() > 0 && TargetedNode->TerrainTile->EnemyUnitList.size() > 0)
				{
					SceneSystem::Instance().SwitchScene("BattleScene");
					GameLogicSystem::Instance().SetCurrentState(GameLogicSystem::Instance().BattlePhase);
					GameLogicSystem::Instance().InternalBattleSystem->SetUpUnits(TargetedNode->TerrainTile);
				}
				TargetedNode = nullptr;
			}
		}
		break;
	}
}

void EnemySystem::Render(void)
{
	if (InternalEnemyContainer.size())
	for (auto it : InternalEnemyContainer)
	if (it->Active)
		it->Render();
}

EnemyPiece* EnemySystem::GenerateNewEnemy()
{
	// Create a new unit and assign him to the start
	EnemyPiece* EP = new EnemyPiece();
	EP->InternalDefinedPath = InternalPathContainer[Math::RandIntMinMax(0, InternalPathContainer.size() - 1)];
	EP->TargetPosition = EP->InternalDefinedPath.front()->GetEntity()->GetPosition() + Vector3(0, EP->InternalDefinedPath.front()->GetEntity()->GetDimensions().y + EP->GetDimensions().y);
	EP->SetPosition(EP->TargetPosition + Vector3(0, 10, 0));
	InternalEnemyContainer.push_back(EP);
	// Add to the tile he is on
	EP->InternalDefinedPath.front()->TerrainTile->EnemyUnitList.push_back(EP);
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
	EP->TargetPosition = EP->InternalDefinedPath.front()->GetEntity()->GetPosition() + Vector3(0, EP->InternalDefinedPath.front()->GetEntity()->GetDimensions().y + EP->GetDimensions().y);
	return EP;
}

EnemyPiece* EnemySystem::RandomizePieceSelection()
{
	// Might want to add some selection conditions here
	EnemyPiece* EP = (EnemyPiece*)InternalEnemyContainer[Math::RandIntMinMax(0, InternalEnemyContainer.size() - 1)];
	return EP;
}