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
	switch (CurrentTurnState)
	{
	case (S_TURNSTART) : // It's my turn, Randomize Selection
		if (InternalEnemyContainer.size() > 2 || (InternalEnemyContainer.size() > 0 && (Math::RandIntMinMax(1, 10) < 8)))
			CurrentTurnState = S_MOVE;
		else CurrentTurnState = S_SPAWN;
		break;
	case (S_SPAWN) : // I Spawn a Unit
		SelectedUnit = GenerateNewEnemy();
		CurrentTurnState = S_TURNEND;
		break;
	case (S_MOVE) : // I Move a Unit
		SelectedUnit = AdvanceSingleUnit();
		CurrentTurnState = S_TURNEND;
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
		}
		break;
	}
}

void EnemySystem::Render(void)
{
	for (auto it : InternalEnemyContainer)
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
	return EP;
}

EnemyPiece* EnemySystem::AdvanceSingleUnit()
{
	// Select a piece that can move
	EnemyPiece* EP = RandomizePieceSelection();
	while (EP->InternalDefinedPath.size() <= 1)
		EP = RandomizePieceSelection();
	// Pop the front of his current path
	EP->InternalDefinedPath.erase(EP->InternalDefinedPath.begin());
	EP->TargetPosition = EP->InternalDefinedPath.front()->GetEntity()->GetPosition() + Vector3(0, EP->InternalDefinedPath.front()->GetEntity()->GetDimensions().y + EP->GetDimensions().y);
	return EP;
}

EnemyPiece* EnemySystem::RandomizePieceSelection()
{
	// Might want to add some selection conditions here
	EnemyPiece* EP = InternalEnemyContainer[Math::RandIntMinMax(0, InternalEnemyContainer.size() - 1)];
	return EP;
}