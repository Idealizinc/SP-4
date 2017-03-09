#include "PlayerSystem.h"
#include "../Systems/GameLogicSystem.h"
#include "../../Engine/System/PerspectiveRaycaster.h"
#include "../../Engine/System/MusicSystem.h"

PlayerSystem::~PlayerSystem(void)
{
	Exit();
}

void PlayerSystem::Init(void)
{
	CurrentTurnState = S_TURNSTART;
	SelectedUnit = nullptr;
	MouseDownSelection = MouseUpSelection = TargetedNode = nullptr;
	selectingUnit = false;

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
		CA->CameraMoveTargetPosition = SceneSystem::Instance().GetCurrentScene().ScenePartition->PlayerBase->GetEntity()->GetPosition();
		CurrentTurnState = S_ACTION;
		GameLogicSystem::Instance().GameInterface->toggleSurrender();
		break;
	case (S_ACTION) : // I Spawn/Move a Unit
		// Input Detection
		HandleUserInput(dt);

		if (SelectedUnit != nullptr)
			CurrentTurnState = S_TURNEND;
		break;
	case (S_TURNEND) : // I end my turn
		if (!AnimationEnded)
		{
			CA->CameraMoveTargetPosition = SelectedUnit->GetPosition();
			if (AnimateUpwards)
			{
				Vector3 AltDirection = SelectedUnit->TargetPosition + Vector3(0, SelectedUnit->GetDimensions().y * 8.f) - SelectedUnit->GetPosition();
				SelectedUnit->SetPosition(SelectedUnit->GetPosition() + AltDirection * 2.f * GameLogicSystem::Instance().PieceAnimationSpeed * dt);
				if (AltDirection.LengthSquared() <= GameLogicSystem::Instance().PieceMinimumDistance)
				{
					AnimateUpwards = false;
				}
			}
			else
			{
				Vector3 Direction = SelectedUnit->TargetPosition - SelectedUnit->GetPosition();
				SelectedUnit->SetPosition(SelectedUnit->GetPosition() + Direction * 4.f * GameLogicSystem::Instance().PieceAnimationSpeed * dt);
				if (Direction.LengthSquared() <= GameLogicSystem::Instance().PieceMinimumDistance)
				{
					CA->CameraMoveTargetPosition = SelectedUnit->GetPosition();
					AnimationEnded = true;

					int ParticleCount = (int)Math::RandFloatMinMax(10.f, 20.f * GameLogicSystem::Instance().ParticleMultiplier);
					for (unsigned int i = 0; i < (unsigned)ParticleCount; ++i)
					{
						float ParticleSpeed = Math::RandFloatMinMax(1.f, 2.f);
						float ParticleLifeTime = Math::RandFloatMinMax(1.f, 1.5f);
						float Interval = SelectedUnit->GetDimensions().x * 5.f;
						Vector3 Dimensions = Vector3(Interval, Interval, Interval) * 2.f;
						Vector3 Velocity = ParticleSpeed * Vector3(Math::RandFloatMinMax(-Interval, Interval) * 0.5f, 0.25f * Interval, Math::RandFloatMinMax(-Interval, Interval)* 0.5f);
						GameLogicSystem::Instance().ParticleSystem.AddWorldSpaceParticle("Smoke", SelectedUnit->GetPosition() + Vector3(0, SelectedUnit->GetDimensions().y * 0.5f), 0.5f *Dimensions, Velocity, SceneSystem::Instance().GetCurrentScene().camera->position, ParticleLifeTime);
					}
					MusicSystem::Instance().PlaySong("impact");
				}
			}
		}
		else {
			if (WaitTimer < 1.f)
				WaitTimer += dt;
			else
			{
				WaitTimer = 0.f;
				// The animation is over, my turn is up
				GameLogicSystem::Instance().SetCurrentState(GameLogicSystem::Instance().EnemyTurn);
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
						else TargetedNode->TerrainTile->PlayerHasAdvantage = true;
					}
					TargetedNode = nullptr;
				}
			}
		}
		break;
	}
	for (std::vector<UnitPiece*>::iterator it = InternalUnitContainer.begin(); it != InternalUnitContainer.end();)
	{
		if (!(*it)->Active)
		{
			delete *it;
			it = InternalUnitContainer.erase(it);
		}
		else ++it;
	}
}

void PlayerSystem::Render(void)
{
	if (InternalUnitContainer.size())
	for (auto it : InternalUnitContainer)
	if (it->Active)
		it->Render();
}

UnitPiece* PlayerSystem::GenerateNewUnit(const std::map<std::string, unsigned short>& Battalion)
{
	// Create a new unit and assign him to the start
	UnitPiece* UP = new UnitPiece();
	UP->InternalBattalionList = Battalion;
	BaseObject* Spawn = SceneSystem::Instance().GetCurrentScene().ScenePartition->PlayerBase->GetEntity();
	UP->TargetPosition = Spawn->GetPosition() + Vector3(0, Spawn->GetDimensions().y + UP->GetDimensions().y);
	UP->SetPosition(UP->TargetPosition + Vector3(0, 10, 0));
	InternalUnitContainer.push_back(UP);
	// Add to the tile he is on
	SceneSystem::Instance().GetCurrentScene().ScenePartition->PlayerBase->TerrainTile->PlayerUnitList.push_back(UP);
	UP->TargetNode = SceneSystem::Instance().GetCurrentScene().ScenePartition->PlayerBase;
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



void PlayerSystem::HandleUserInput(const float& dt)
{
	UnitSwapWaitTimer += dt;
	CameraAerial* CA = (CameraAerial*)SceneSystem::Instance().GetCurrentScene().camera;
	if (InternalUnitContainer.size())
	{
		if (UnitSwapWaitTimer > UnitWaitTime && SceneSystem::Instance().cSS_InputManager->GetKeyValue('Q'))
		{
			UnitSwapWaitTimer = 0.f;
			++UnitIndex;
			if ((unsigned int)UnitIndex > InternalUnitContainer.size() - 1)
				UnitIndex = 0;
			CA->CameraMoveTargetPosition = InternalUnitContainer[UnitIndex]->GetPosition();
		}
		else if (UnitSwapWaitTimer > UnitWaitTime && SceneSystem::Instance().cSS_InputManager->GetKeyValue('E'))
		{
			UnitSwapWaitTimer = 0.f;
			--UnitIndex;
			if (UnitIndex < 0)
				UnitIndex = InternalUnitContainer.size() - 1;
			CA->CameraMoveTargetPosition = InternalUnitContainer[UnitIndex]->GetPosition();
		}
	}

	AnimateUpwards = true;
	AnimationEnded = false;
	if (selectingUnit == 0 && GameLogicSystem::Instance().GameInterface->SurrenderOn == false && GameLogicSystem::Instance().UnitInterface->UIDisplayed == 0)
	{
		if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_DOWN)
		{
			TerrainNode* TN = GameLogicSystem::Instance().GetTerrainNodeForPosition(PerspectiveRaycaster::Instance().CalculateIntersectionPointInPlane(Vector3(0, 5), Vector3(0, 1), PerspectiveRaycaster::Instance().CalculateCursorPositionInWorldSpace(CA, CA->FieldOfView)));
			if (TN)
			{
				MouseDownSelection = TN;
				MouseUpSelection = nullptr;
				InfoWaitTimer = 0.f;
			}
		}
		else if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_HOLD)
		{
			if (MouseDownSelection)
			{
				InfoWaitTimer += dt;
				if (InfoWaitTimer > 0.25f)
				if (MouseDownSelection->TerrainTile->PlayerUnitList.size())
				{
					for (auto it : MouseDownSelection->LinkedTerrainNodes)
					{
						if (MouseDownSelection->TerrainTile->PlayerUnitList.size() < (unsigned)GameLogicSystem::Instance().MaxUnitInNode)
						{
							if (Math::RandIntMinMax(0, 1 + (int)(10 * GameLogicSystem::Instance().ParticleMultiplier) / 10))
							{
								float ParticleSpeed = Math::RandFloatMinMax(1.f, 2.f);
								float ParticleLifeTime = Math::RandFloatMinMax(1.f, 1.5f);
								float Interval = it->GetEntity()->GetDimensions().x * 1.f;
								Vector3 Dimensions = Vector3(Interval, Interval, Interval);
								Vector3 Velocity;
								if (Math::RandIntMinMax(0, 1))
								{
									Velocity = ParticleSpeed * Vector3(Math::RandFloatMinMax(-Interval, Interval) * 0.5f, Interval * 0.5f, Math::RandFloatMinMax(-Interval, Interval)* 0.5f);
									GameLogicSystem::Instance().ParticleSystem.AddWorldSpaceParticle("Light", it->GetEntity()->GetPosition(), 0.5f *Dimensions, Velocity, SceneSystem::Instance().GetCurrentScene().camera->position, ParticleLifeTime);
								}
								else{
									Velocity = 0.5f * ParticleSpeed * Interval * (it->GetEntity()->GetPosition() - MouseDownSelection->GetEntity()->GetPosition()).Normalize();
									GameLogicSystem::Instance().ParticleSystem.AddWorldSpaceParticle("Light", MouseDownSelection->GetEntity()->GetPosition() + Vector3(0, Dimensions.y), 0.5f * Dimensions, Velocity, SceneSystem::Instance().GetCurrentScene().camera->position, 2.f * ParticleLifeTime);

								}
							}
						}
					}
				}
				if (InfoWaitTimer > 1.f)
				{
					if (MouseDownSelection->TerrainTile->PlayerUnitList.size() != 0 && GameLogicSystem::Instance().GameInterface->MultipleUnitElements.size() == 0)
					{
						GameLogicSystem::Instance().GameInterface->MultipleUnitSelect(MouseDownSelection->TerrainTile->PlayerUnitList, false);
					}
					else if (MouseDownSelection->TerrainTile->EnemyUnitList.size() != 0 && GameLogicSystem::Instance().GameInterface->MultipleUnitElements.size() == 0)
					{
						GameLogicSystem::Instance().GameInterface->MultipleUnitSelectE(MouseDownSelection->TerrainTile->EnemyUnitList);
					}
				}
			}
		}
		else if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_UP)
		{
			TerrainNode* TN = GameLogicSystem::Instance().GetTerrainNodeForPosition(PerspectiveRaycaster::Instance().CalculateIntersectionPointInPlane(Vector3(0, 5), Vector3(0, 1), PerspectiveRaycaster::Instance().CalculateCursorPositionInWorldSpace(CA, CA->FieldOfView)));
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
			}
			else if (MouseDownSelection != MouseUpSelection)
			{
				// Clicked different tiles
				// Check if they are linked
				if (MouseDownSelection->TerrainTile->PlayerUnitList.size() > 0 && GameLogicSystem::Instance().UnitInterface->UIDisplayed == 0)
				for (auto it : MouseDownSelection->LinkedTerrainNodes)
				{
					if (it == MouseUpSelection)
					{
						int test = GameLogicSystem::Instance().MaxUnitInNode;
						int t2 = MouseUpSelection->TerrainTile->PlayerUnitList.size();
						if ((unsigned)GameLogicSystem::Instance().MaxUnitInNode > MouseUpSelection->TerrainTile->PlayerUnitList.size() && MouseUpSelection != SceneSystem::Instance().GetCurrentScene().ScenePartition->PlayerBase || MouseUpSelection == SceneSystem::Instance().GetCurrentScene().ScenePartition->PlayerBase && !SceneSystem::Instance().GetCurrentScene().ScenePartition->PlayerBase->TerrainTile->PlayerUnitList.size())
						{
							if (MouseDownSelection->TerrainTile->PlayerUnitList.size() == 1)
							{
								SelectedUnit = AdvanceSingleUnit(MouseDownSelection->TerrainTile->PlayerUnitList.front(), MouseUpSelection);
								TargetedNode = MouseUpSelection;
								CA->CameraMoveTargetPosition = MouseUpSelection->GetEntity()->GetPosition();
								GameLogicSystem::Instance().GameInterface->toggleSurrender();
								break;
							}
							else
							{
								GameLogicSystem::Instance().GameInterface->MultipleUnitSelect(MouseDownSelection->TerrainTile->PlayerUnitList, true);
								TargetedNode = MouseUpSelection;
								selectingUnit = true;
								GameLogicSystem::Instance().GameInterface->toggleSurrender();
								break;
							}
						}
						else
						{
							if (GameLogicSystem::Instance().UnitInterface->warningDisplayed3 == 0)
							{
								GameLogicSystem::Instance().UnitInterface->NoSlotError();
							}
						}
					}
				}
			}
			MouseDownSelection = MouseUpSelection = nullptr;
		}

	}
	else if (GameLogicSystem::Instance().UnitInterface->UIDisplayed == 1)
	{
		if (GameLogicSystem::Instance().UnitInterface->deploy == true && GameLogicSystem::Instance().UnitInterface->returnUnitSpawnSys()->getCurrentUnitCount() != 0)
		{
			if (Cash >= GameLogicSystem::Instance().UnitInterface->returnUnitSpawnSys()->CalculateCost())
			{
				SelectedUnit = GenerateNewUnit(GameLogicSystem::Instance().UnitInterface->returnUnitSpawnSys()->returnRecordedUnitMap());
				GameLogicSystem::Instance().GameInterface->ShowCashReduction(GameLogicSystem::Instance().UnitInterface->returnUnitSpawnSys()->CalculateCost());
				GameLogicSystem::Instance().UnitInterface->OpenInterface();
				GameLogicSystem::Instance().UnitInterface->CheckDeployed();
				//GameLogicSystem::Instance().GameInterface->toggleSurrender();
			}
			else
			{
				GameLogicSystem::Instance().UnitInterface->NoMoneyError();
			}

		}
	}
	else if (selectingUnit == 1)
	{
		if (GameLogicSystem::Instance().GameInterface->UnitSelected != nullptr)
		{
			SelectedUnit = AdvanceSingleUnit(GameLogicSystem::Instance().GameInterface->UnitSelected, TargetedNode);
			GameLogicSystem::Instance().GameInterface->UnitSelected = nullptr;
			selectingUnit = false;
		}
	}
}

int PlayerSystem::GetCash()
{
	return Cash;
}

void PlayerSystem::SetCash(int amt)
{
	Cash = amt;
}

void PlayerSystem::SkipTurn()
{
	GameLogicSystem::Instance().SetCurrentState(GameLogicSystem::Instance().EnemyTurn);
	CurrentTurnState = S_TURNSTART;
}