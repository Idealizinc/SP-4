#include "PlayerSystem.h"
#include "../Systems/GameLogicSystem.h"
#include "../SceneManagement/ScenePartitionGraph.h"
#include "../../Engine/System/PerspectiveRaycaster.h"

PlayerSystem::~PlayerSystem(void)
{
	Exit();
}

void PlayerSystem::Init(void)
{
	CurrentTurnState = S_TURNSTART;
	SelectedUnit = nullptr;
	MouseDownSelection = MouseUpSelection = nullptr;
}

void PlayerSystem::Exit(void)
{
	for (auto it : InternalUnitContainer)
	{
		delete it;
	}
	InternalUnitContainer.clear();
}

void PlayerSystem::Update(const float& dt)
{
	CameraAerial* CA = (CameraAerial*)SceneSystem::Instance().GetCurrentScene().camera;
	
	switch (CurrentTurnState)
	{
	case (S_TURNSTART) : // It's my turn, do something
		CA->CameraMoveTargetPosition = ScenePartitionGraph::Instance().PlayerBase->GetEntity()->GetPosition();
		CurrentTurnState = S_ACTION;
		break;
	case (S_ACTION) : // I Spawn/Move a Unit
		// Input Detection
		HandleUserInput();
		
		if (SelectedUnit != nullptr)
			CurrentTurnState = S_TURNEND;
		break;
	case (S_TURNEND) : // I end my turn
		Vector3 Direction = SelectedUnit->TargetPosition - SelectedUnit->GetPosition();
		if (Direction.LengthSquared() > GameLogicSystem::Instance().PieceMinimumDistance)
		{
			SelectedUnit->SetPosition(SelectedUnit->GetPosition() + Direction * GameLogicSystem::Instance().PieceAnimationSpeed * dt);
			CA->CameraMoveTargetPosition = SelectedUnit->GetPosition();
		}
		else {
			// The animation is over, my turn is up
			GameLogicSystem::Instance().SetCurrentState(GameLogicSystem::Instance().EnemyTurn);
			// Reseting for next turn
			CurrentTurnState = S_TURNSTART;
			SelectedUnit = nullptr;
		}
		break;
	}
}

void PlayerSystem::Render(void)
{
	for (auto it : InternalUnitContainer)
		it->Render();
}

UnitPiece* PlayerSystem::GenerateNewUnit()
{
	// Create a new unit and assign him to the start
	UnitPiece* UP = new UnitPiece();
	BaseObject* Spawn = ScenePartitionGraph::Instance().PlayerBase->GetEntity();
	UP->TargetPosition = Spawn->GetPosition() + Vector3(0, Spawn->GetDimensions().y + UP->GetDimensions().y);
	UP->SetPosition(UP->TargetPosition + Vector3(0, 10, 0));
	InternalUnitContainer.push_back(UP);
	// Add to the tile he is on
	ScenePartitionGraph::Instance().PlayerBase->TerrainTile->PlayerUnitList.push_back(UP);
	UP->TargetNode = ScenePartitionGraph::Instance().PlayerBase;
	return UP;
}

UnitPiece* PlayerSystem::AdvanceSingleUnit(UnitPiece* Selection, TerrainNode* Target)
{
	// Remove from the old tile
	for (std::vector<UnitPiece*>::iterator it = Selection->TargetNode->TerrainTile->PlayerUnitList.begin(); it != Selection->TargetNode->TerrainTile->PlayerUnitList.end(); ++it)
	{
		if (*it == Selection)
		{
			Selection->TargetNode->TerrainTile->PlayerUnitList.erase(it);
			break;
		}
	}
	Target->TerrainTile->PlayerUnitList.push_back(Selection);
	Selection->TargetNode = Target;
	Selection->TargetPosition = Target->GetEntity()->GetPosition() + Vector3(0, Target->GetEntity()->GetDimensions().y + Selection->GetDimensions().y);
	return Selection;
}

void PlayerSystem::HandleUserInput()
{
	CameraAerial* CA = (CameraAerial*)SceneSystem::Instance().GetCurrentScene().camera;
	if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_DOWN)
	{
		TerrainNode* TN = GameLogicSystem::Instance().GetTerrainNodeForPosition(PerspectiveRaycaster::Instance().CalculateIntersectionPointInPlane(Vector3(), Vector3(0, 1), PerspectiveRaycaster::Instance().CalculateCursorPositionInWorldSpace(CA, CA->FieldOfView)));
		if (TN)
		{
			MouseDownSelection = TN;
			MouseUpSelection = nullptr;
		}
	}
	else if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_UP)
	{
		TerrainNode* TN = GameLogicSystem::Instance().GetTerrainNodeForPosition(PerspectiveRaycaster::Instance().CalculateIntersectionPointInPlane(Vector3(), Vector3(0, 1), PerspectiveRaycaster::Instance().CalculateCursorPositionInWorldSpace(CA, CA->FieldOfView)));
		if (TN)
			MouseUpSelection = TN;
	}

	if (MouseUpSelection != nullptr && MouseDownSelection != nullptr)
	{
		// The player has clicked
		if (MouseDownSelection == MouseUpSelection)
		{
			// Clicked the same tile
			CA->CameraMoveTargetPosition = MouseDownSelection->GetEntity()->GetPosition();
			if (MouseDownSelection == ScenePartitionGraph::Instance().PlayerBase)
			{
				SelectedUnit = GenerateNewUnit();
			}
		}
		else if (MouseDownSelection != MouseUpSelection)
		{
			// Clicked different tiles
			// Check if they are linked
			if (MouseDownSelection->TerrainTile->PlayerUnitList.size() > 0)
			for (auto it : MouseDownSelection->LinkedTerrainNodes)
			{
				if (it == MouseUpSelection)
				{
					SelectedUnit = AdvanceSingleUnit(MouseDownSelection->TerrainTile->PlayerUnitList.front(), MouseUpSelection);
					CA->CameraMoveTargetPosition = MouseUpSelection->GetEntity()->GetPosition();
					break;
				}
			}
		}
		MouseDownSelection = MouseUpSelection = nullptr;
	}
}