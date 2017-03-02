#include "GameLogicSystem.h"
#include "../../Engine/Objects/Boundary2D.h"
#include "../../Engine/State/StateList.h"
#include "../SceneManagement/ScenePartitionGraph.h"
#include "../../Engine/Objects/GameObject.h"
#include "../Logic/Level/LevelDataLoader.h"
#include "../../Engine/System/LuaInterface.h"

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

	// Don't forget to Init the second initiallization if required
	//QuickInit();

	LuaInterface::Instance().AddLuaState("CSVInitiallizer.lua");
	TerrainLoader.LoadTerrainData(LuaInterface::Instance().GetStringValue("CSVFilePath_TerrainDataLoader").c_str());
	LevelDataLoader::Instance().LoadLevelData(LuaInterface::Instance().GetStringValue("CSVFilePath_LevelDataLoader").c_str());

}

void GameLogicSystem::QuickInit()
{
	// Initiallize the Player & Enemy
	InternalPlayerSystem = new PlayerSystem();
	InternalPlayerSystem->Init();
	InternalEnemySystem = new EnemySystem();
	InternalEnemySystem->Init();
	// Init Battle System
	InternalBattleSystem = new BattleSystem();
	InternalBattleSystem->Init();

	// Misc
	//Init UI
	UnitInterface = new UnitCreationInterface();
	GameInterface = new GameScreenInterface();

	InternalEnemySystem->RemainingGold = UnitInterface->returnUnitSpawnSys()->maxUnitCost * maxStartingUnits;
	//InternalPlayerSystem->SetCash(/*UnitInterface->returnUnitSpawnSys()->maxUnitCost * maxStartingUnits*/1000);
	GameOver = false;
	PlayerWon = false;
}

void GameLogicSystem::Update(const float& dt)
{
	ParticleSystem.UpdateContainer(dt, SceneSystem::Instance().GetCurrentScene().camera->position);
	if (!GameOver)
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
			InternalBattleSystem->Update(dt);
		}

		UnitInterface->Update(dt);
		GameInterface->Update(dt);
	}
	else SceneSystem::Instance().SwitchScene("EndOfGameScene");
}

bool GameLogicSystem::DetectWinner()
{
	// Checking the enemy
	TerrainNode* EnemyBase = SceneSystem::Instance().GetCurrentScene().ScenePartition->EnemyBase;
	if (EnemyBase)
	{
		// Theres a player here and no defending units
		if (EnemyBase->TerrainTile->PlayerUnitList.size() > 0 &&
			EnemyBase->TerrainTile->EnemyUnitList.size() <= 0)
		{
			PlayerWon = true;
			GameOver = true;
			return true;
		}
	}
	// Checking the Player
	TerrainNode* PlayerBase = SceneSystem::Instance().GetCurrentScene().ScenePartition->PlayerBase;
	if (PlayerBase)
	{
		// Theres a player here and no defending units
		if (PlayerBase->TerrainTile->PlayerUnitList.size() <= 0 &&
			PlayerBase->TerrainTile->EnemyUnitList.size() > 0)
		{
			PlayerWon = false;
			GameOver = true;
			return true;
		}
	}
	return false;
}

void GameLogicSystem::DetectSurrender()
{
	PlayerWon = false;
	GameOver = true;
}

void GameLogicSystem::Render()
{
	if (GetCurrentState()->GetStateName() != BattlePhase)
	{
		ParticleSystem.Render();
		if (InternalPlayerSystem)
		{
			InternalPlayerSystem->Render();
			UnitInterface->Render();
			GameInterface->Render();
		}
		if (InternalEnemySystem)
		{
			InternalEnemySystem->Render();
		}
	}
	else
	{
		if (!SceneSystem::Instance().AnimationDirectionInwards)
			InternalBattleSystem->Render();
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
	if (InternalBattleSystem)
	{
		InternalBattleSystem->Exit();
		delete InternalBattleSystem;
		InternalBattleSystem = nullptr;
	}
	if (UnitInterface)
	{
		UnitInterface->Exit();
		delete UnitInterface;
		UnitInterface = nullptr;
	}
	if (GameInterface)
	{
		GameInterface->Exit();
		delete GameInterface;
		GameInterface = nullptr;
	}

}

TerrainNode* GameLogicSystem::GetTerrainNodeForPosition(const Vector3& Position)
{
	GridNode* GN = SceneSystem::Instance().GetCurrentScene().ScenePartition->FindGridForPosition(Position);
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