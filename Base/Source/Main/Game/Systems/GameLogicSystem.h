#ifndef _GAME_LOGIC_SYSTEM_H
#define _GAME_LOGIC_SYSTEM_H

/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 18 February 2017 1:42 AM
File Name     : GameLogicSystem.h
Purpose       : Defines the GameLogicSystem that handles the both the player and enemy's FSM AI's turn based logic
*/

#include "../../Engine/Internal/SingletonTemplate.h"
#include "../../Engine/State/StateManager.h"
#include "../SceneManagement/Objects/TerrainNode.h"
#include "../Logic/PlayerSystem.h"
#include "../Logic/EnemySystem.h"
#include "../../Engine/System/SceneSystem.h"
#include "../Miscellaneous/CameraAerial.h"
#include "../Logic/Terrain/TerrainDataLoader.h"
#include "../Systems/BattleSystem.h"

#include "../UserInterface/UnitCreationInterface.h"
#include "../UserInterface/GameScreenInterface.h"
#include "../Systems/BillboardManager.h"



/*
Description
The GLS is the acting state manager of the game.
Based on what is the current active state, it will update it's selected internal systems accordingly.
It is to hold both the Player and Enemy Systems and also make use of the SceneSystem for BattleScene Transitions

Player System
- A system that holds the player's statistics [Resources, Units]
- Links to the Input manager and contains Input Related Game Logic Codes.
- Makes use of the GLS to carry out game specific tasks such as unit placement/movement

Enemy System
- A system that defines the Enemy.
- Uses an Internal State Manager to decide on it's course of action
- Like the PS, it makes use of the GLS for the same reasons.
*/

class GameLogicSystem : public SingletonTemplate<GameLogicSystem>, public StateManager
{
public:
	enum Faction
	{
		F_LIVING = 0,
		F_UNDEAD = 1,
		F_MAX,
	};

	virtual ~GameLogicSystem();

	// Initiallization of objects that can only be initiallized once
	virtual void Init();
	// Initiallization of objects that can be reinited.
	void QuickInit();
	// Finite state machine logic
	virtual void Update(const float& dt);
	// Rendering of Units
	virtual void Render();
	// Deletion of objects that should only be deleted once
	virtual void Exit();
	// Deletion of objects that exist only for the current instance of the GLS
	void QuickExit();

	TerrainNode* GetTerrainNodeForPosition(const Vector3& Position);

	const std::string PlayerTurn = "Player's Turn";
	const std::string EnemyTurn = "Enemy's Turn";
	const std::string BattlePhase = "Battle Phase";
	TerrainDataLoader TerrainLoader;

	BillboardManager ParticleSystem;
	Faction PlayerFaction;
	bool PlayerWon = false;
	bool GameOver = false;


//private:
	PlayerSystem* InternalPlayerSystem;
	EnemySystem* InternalEnemySystem;
	BattleSystem* InternalBattleSystem;
	float PieceAnimationSpeed = 5.f;
	float PieceMinimumDistance = 0.01f;
	UnitCreationInterface* UnitInterface;
	GameScreenInterface* GameInterface;

	const int maxStartingUnits = 10;
	bool DetectWinner();
	void DetectSurrender();
	const int MaxUnitInNode = 5;
	float ParticleMultiplier = 1.f;

protected:

};

#endif //_GAME_LOGIC_SYSTEM_H