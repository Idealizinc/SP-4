#include "GameLogicSystem.h"
#include "../../Engine/State/StateList.h"
#include "../SceneManagement/ScenePartitionGraph.h"
#include "../Objects/Miscellaneous/GameObject.h"
#include "../Objects/Miscellaneous/Boundary2D.h"

// Constructor and Destructor
GameLogicSystem::~GameLogicSystem()
{

}

// Main Calls
void GameLogicSystem::Init()
{
	// Create The States & Set them
	AttachState(StateList::Instance().CreateState(PlayerTurn));
	AttachState(StateList::Instance().CreateState(EnemyTurn));
	AttachState(StateList::Instance().CreateState(BattlePhase));

	// Remember to set current state or nothing will run
	SetCurrentState(PlayerTurn);

	// Don't forget to Init the second initiallization
	QuickInit();
}

void GameLogicSystem::QuickInit()
{
	// Initiallize the Player & Enemy
	InternalPlayerSystem = new PlayerSystem();
	InternalPlayerSystem->Init();
	InternalEnemySystem = new EnemySystem();
	InternalEnemySystem->Init();
	// Init Battle System
}

void GameLogicSystem::Update(const float& dt)
{
	State* CurrentState = GetCurrentState();

	if (CurrentState->GetStateName() == PlayerTurn)
	{
		InternalPlayerSystem->Update(dt);
	}
	else if (CurrentState->GetStateName() == EnemyTurn)
	{
		InternalEnemySystem->Update(dt);
	}
	else if (CurrentState->GetStateName() == BattlePhase)
	{
		
	}
}

void GameLogicSystem::Render()
{
	if (InternalPlayerSystem)
	{
		InternalPlayerSystem->Render();
	}
	if (InternalEnemySystem)
	{
		InternalEnemySystem->Render();
	}
}

void GameLogicSystem::Exit()
{
	QuickExit();
}

void GameLogicSystem::QuickExit()
{
	if (InternalPlayerSystem)
	{
		InternalPlayerSystem->Exit();
		delete InternalPlayerSystem;
		InternalPlayerSystem = nullptr;
	}
	if (InternalEnemySystem)
	{
		InternalEnemySystem->Exit();
		delete InternalEnemySystem;
		InternalEnemySystem = nullptr;
	}
}

TerrainNode* GameLogicSystem::GetTerrainNodeForPosition(const Vector3& Position)
{
	GridNode* GN = ScenePartitionGraph::Instance().FindGridForPosition(Position);
	if (GN != nullptr)
	{
		for (std::vector<Node*>::reverse_iterator it = GN->GetNodeList().rbegin(); it != GN->GetNodeList().rend(); ++it)
		{
			TerrainNode* TN = nullptr;
			// Validity check for whether the pointer is a TerrainNode pointer
			if (TN = dynamic_cast<TerrainNode*>(*it))
			{
				GameObject* GO = dynamic_cast<GameObject*>((*it)->GetEntity());
				// Is the object within the node valid
				if (GO != nullptr)
				{
					Boundary2D Bounds;
					Bounds.SetPosition(Vector3(GO->GetPosition().x, GO->GetPosition().z));
					Bounds.SetDimensions(Vector3(GO->GetDimensions().x, GO->GetDimensions().z));
					if (Bounds.CheckCollision(Vector3(Position.x, Position.z)))
					{
						// A hit has been identified
						GO->GetBoundary();
						return TN;
					}
				}	
			}
		}
	}
	return nullptr;
}

// Internal Calls