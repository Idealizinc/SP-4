#include "BattleScreenCharacter.h"
// Mains
#include "../../../../Engine/System/RenderSystem.h"
#include "../../../../Engine/System/SceneSystem.h"
#include "../../../../Engine/Objects/GameObject.h"
#include "../../../Systems/GameLogicSystem.h"
#include "LoadHmap.h"
// Weapons
#include "../../Weapons/Melee/MeleeWeapon.h"
#include "../../Weapons/Magic/MagicWeapon.h"
#include "../../Weapons/Ranged/RangeWeapon.h"
// State Managers
#include "../StateManagers/MeleeStateManager.h"
#include "../StateManagers/MagicStateManager.h"
#include "../StateManagers/RangeStateManager.h"

BattleScreenCharacter::BattleScreenCharacter()
{
	Init();
}

BattleScreenCharacter::~BattleScreenCharacter()
{
	Exit();
}

void BattleScreenCharacter::Init()
{
	SetEntityID("UnassignedEntity");
	anim_Time = Math::RandFloatMinMax(0.5f, 1);
}

void BattleScreenCharacter::Update(const float& dt)
{
	if (Active) // Still can update if invisible
	{
		if (InternalStateManager->GetCurrentState()->GetStateName() != "Dead")
		{
			Vector3 StoredVelocity = GetVelocity();
			if (!Static)
			{
				Vector3 LegitimatePos = (GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt);// + Vector3(0, GetDimensions().y) * 0.2f;
				float TargetPositionY = SceneSystem::Instance().GetCurrentScene().TerrainScale.y * ReadHeightMap(SceneSystem::Instance().GetCurrentScene().m_heightMap, LegitimatePos.x / SceneSystem::Instance().GetCurrentScene().TerrainScale.x, LegitimatePos.z / SceneSystem::Instance().GetCurrentScene().TerrainScale.z);
				if (!(LegitimatePos.y - TargetPositionY <= Math::EPSILON && TargetPositionY - LegitimatePos.y <= Math::EPSILON))
				{
					float RateofChangeY = (LegitimatePos.y - TargetPositionY) * (float)dt * Dimensions.y * 0.5f;
					if (LegitimatePos.y - RateofChangeY >= TargetPositionY || LegitimatePos.y - RateofChangeY <= TargetPositionY)
					{
						LegitimatePos.y -= RateofChangeY;
					}
				}
				SetPosition(LegitimatePos);
			}
			if (!GetVelocity().IsZero())
			{
				LookVector = GetVelocity().Normalized();
				SetVelocity(GetVelocity() - GetVelocity() * dt);
				HandlePartitionCollision();
				HandleCharacterCollision();
				//HandleProjectileCollision();
			}
			
			// Update the angles and look vector
			float TargetRotationAngle = Math::RadianToDegree(atan2(-LookVector.x, -LookVector.z));
			if (TargetRotationAngle < 0.f)
				TargetRotationAngle += 360.f;

			// Interpolate the rotation angle
			if (abs(RotationAngle - TargetRotationAngle) > Math::EPSILON)
			{
				float Interval = (float)((((int)(TargetRotationAngle - RotationAngle) % 360) + 540) % 360) - 180;
				RotationAngle += Interval * InterpolationSpeed * (float)dt;
			}

			Timer += (float)dt;
			CurrentAnimationName = GetCurrentState()->GetStateName();
			auto it = AnimMap.find(CurrentAnimationName);
			if (it != AnimMap.end())
				CurrentAnimation = it->second;
			if (Timer > anim_Time)
				Timer = 0;

			CurrentFrameMultiplier = Timer / anim_Time;
			CurrentFrame = (int)(CurrentAnimation.size() * CurrentFrameMultiplier);
			Math::Clamp(CurrentFrame, 0, (int)CurrentAnimation.size() - 1);
			// This will set the texture to be rendered with the mesh to the one of the correct animation frame
			StoredMesh->textureArray[0] = CurrentAnimation[CurrentFrame];
		}
	}
	if (InternalStateManager != nullptr)
		InternalStateManager->Update((float)dt);
}

void BattleScreenCharacter::Render()
{
	if (Active && Visible && StoredMesh)
	{
		RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
		SceneSystem::Instance().GetCurrentScene().modelStack->PushMatrix();
		SceneSystem::Instance().GetCurrentScene().modelStack->Translate(GetPosition().x, GetPosition().y + GetDimensions().y * 0.25f, GetPosition().z);
		SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(GetRotationAngle(), GetRotationAxis().x, GetRotationAxis().y, GetRotationAxis().z);
		SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(90, -1, 0, 0);
		SceneSystem::Instance().GetCurrentScene().modelStack->Scale(GetDimensions().x, GetDimensions().y, GetDimensions().z);
		if (Dimensions.LengthSquared() > 0.05f)
			Renderer->RenderMesh(*GetMesh(), true);
		SceneSystem::Instance().GetRenderSystem().modelStack->PopMatrix();
	}
}

void BattleScreenCharacter::Exit()
{
	if (InternalStateManager)
	{
		CharacterStateManager* CMS = (CharacterStateManager*)InternalStateManager;
		CMS->Exit();
		delete CMS;
		InternalStateManager = nullptr;
	}
	if (StoredMesh)
	{
		StoredMesh->textureArray[0] = NULL;
		delete StoredMesh;
		StoredMesh = nullptr;
	}
	if (InternalWeapon)
	{
		InternalWeapon->Exit();
		delete InternalWeapon;
		InternalWeapon = nullptr;
	}
}

void BattleScreenCharacter::HandlePartitionCollision()
{
	// Find Current Grid
	GridNode* GN = SceneSystem::Instance().GetCurrentScene().ScenePartition->FindGridForPosition(Position);//dynamic_cast<GridNode*>(Self->GetParentNode());
	// Check Collision Here
	if (GN != nullptr)
	{
		std::vector<Node*> NodeList = GN->GetNodeList();
		for (std::vector<Node*>::iterator it = NodeList.begin(); it != NodeList.end(); ++it)
		{
			if ((*it)->GetEntity() != this)
			{
				GameObject* SO = dynamic_cast<GameObject*>((*it)->GetEntity());
				if (SO != nullptr)
				{
					Boundary* SelfBounds = GetBoundary();
					SO->SetBounds();
					if (SO->GetBoundary()->CheckCollision(GetPosition())) // Offset based on overlapping distance
					if (SO->Static)
						SetPosition((GetPosition() - (SO->GetBoundary()->GetOverlappingDistance() * SO->GetBoundary()->GetOverlappingAxis())));
					else SO->SetPosition((GetPosition() + (SO->GetBoundary()->GetOverlappingDistance() * SO->GetBoundary()->GetOverlappingAxis())));
				}
			}
		}
	}
}

void BattleScreenCharacter::HandleCharacterCollision()
{
	// Find Current Grid
	GridNode* GN = SceneSystem::Instance().GetCurrentScene().ScenePartition->FindGridForPosition(Position);//dynamic_cast<GridNode*>(Self->GetParentNode());
	// Check Collision Here
	if (GN != nullptr)
	{
		for (auto it : GameLogicSystem::Instance().InternalBattleSystem->GetPlayerCharacterList())
		{
			if (it != this)
			{
				GridNode* GN2 = SceneSystem::Instance().GetCurrentScene().ScenePartition->FindGridForPosition(it->GetPosition());
				if (GN == GN2)
				{
					// Same Grid
					GameObject* SO = dynamic_cast<GameObject*>(it);
					if (SO != nullptr)
					{
						Boundary* SelfBounds = GetBoundary();
						SO->SetBounds();
						if (SO->GetBoundary()->CheckCollision(GetPosition())) // Offset based on overlapping distance
							SetPosition((GetPosition() - (SO->GetBoundary()->GetOverlappingDistance() * SO->GetBoundary()->GetOverlappingAxis())));
					}
				}
			}
		}
		for (auto it : GameLogicSystem::Instance().InternalBattleSystem->GetEnemyCharacterList())
		{
			if (it != this)
			{
				GridNode* GN2 = SceneSystem::Instance().GetCurrentScene().ScenePartition->FindGridForPosition(it->GetPosition());
				if (GN == GN2)
				{
					// Same Grid
					GameObject* SO = dynamic_cast<GameObject*>(it);
					if (SO != nullptr)
					{
						Boundary* SelfBounds = GetBoundary();
						SO->SetBounds();
						if (SO->GetBoundary()->CheckCollision(GetPosition())) // Offset based on overlapping distance
							SetPosition((GetPosition() - (SO->GetBoundary()->GetOverlappingDistance() * SO->GetBoundary()->GetOverlappingAxis())));
					}
				}
			}
		}
	}
}

void BattleScreenCharacter::HandleProjectileCollision()
{
	// In need of rework
	//std::vector<Bullet*> bulletList = GlobalPlayer::Instance().WeaponSystem.BulletList;
	//// Create a boundary for testing against
	//Boundary* temp = new Boundary();
	//for (auto it : bulletList)
	//{
	//	if (it->Active)
	//	if (temp->BoundaryCheckBullet(it->GetPosition(), this->GetPosition(), GetDimensions().x * 0.5f) ||
	//		temp->BoundaryCheckBullet(it->PrevPos, this->GetPosition(), GetDimensions().x * 0.5f))
	//	{
	//		if (temp->CheckCollisionLaser(this->GetPosition(), (*it), GetDimensions().x * 0.5f))
	//		{
	//			float MinY = SceneSystem::Instance().GetCurrentScene().TerrainScale.y * ReadHeightMap(SceneSystem::Instance().GetCurrentScene().m_heightMap, GetPosition().x / SceneSystem::Instance().GetCurrentScene().TerrainScale.x, GetPosition().z / SceneSystem::Instance().GetCurrentScene().TerrainScale.z);
	//			float MaxY = GetPosition().y + GetDimensions().y * 0.5f;
	//
	//			if (it->GetPosition().y > MinY && it->GetPosition().y < MaxY)
	//			{
	//				if (it->GetPosition().y < (MaxY - MinY) * 0.8f + MinY)
	//				{
	//					this->HealthPoints -= it->Damage;
	//				}
	//				else
	//				{
	//					this->HealthPoints = 0;
	//				}
	//				it->Active = false;
	//			}
	//		}
	//	}
	//}
	//delete temp;
}

void BattleScreenCharacter::InitiallizeCharacter(UnitType* Type, UnitRace* Race, Terrain* Terrain, bool GetsAdvantage)
{
	SetCharacter(Type, Race, Terrain, GetsAdvantage);
}

template <class Type>
Type CalculateTerrainStatistic(Type Value, Terrain::TerrainStatEnum Statistic, Terrain* Terrain, bool GetsAdvantage)
{
	Type Result;
	if (GetsAdvantage)
		Result = (Type)(Value * Terrain->GetUnitAdvantage(Statistic));
	else Result = (Type)(Value * Terrain->GetUnitAdvantage(Statistic));
	return Result;
}

void BattleScreenCharacter::SetCharacter(UnitType* Type, UnitRace* Race, Terrain* Terrain, bool GetsAdvantage)
{
	// Initiallize his statistics
	SetEntityID(Type->GetName());
	HealthPoints = (int)(Type->GetHealth() * Race->GetHealthModifier());
	WalkSpeed = Type->GetWalkspeed();
	DetectionRadius = Type->GetRange();
	RaceType = Race->GetRace();
	SetSprite(Type->GetMeshName());
	SetRotationAxis(Vector3(0, 1, 0));
	SetDimensions(Vector3(GameLogicSystem::Instance().InternalBattleSystem->UnitSize, GameLogicSystem::Instance().InternalBattleSystem->UnitSize, GameLogicSystem::Instance().InternalBattleSystem->UnitSize));

	// Set up for Terrain Stats
	HealthPoints = CalculateTerrainStatistic(HealthPoints, Terrain::T_HEALTH, Terrain, GetsAdvantage);
	MaxHealthPoints = HealthPoints;

	// Grant this character a newly created quad that is unique to him only, such that the sprite animations will not be in sync
	SetMesh(MeshBuilder::GenerateQuad("CharacterQuad", Color(1, 1, 1), 1));
	//SetMesh("cube");
	// This mesh will have to be deleted by this class later on.

	// Set up a weapon for use
	WeaponType* InternalWeaponType = Type->PossibleWeapon[Math::RandIntMinMax(0, Type->PossibleWeapon.size() - 1)];
	int WeaponDamage = (int)(InternalWeaponType->GetDamage() * Race->GetMeleeDamageModifier());

	// Set up weapon & statemanager
	switch (Type->GetType())
	{
	case (UnitType::T_MELEE) :
		InternalWeapon = new MeleeWeapon(CalculateTerrainStatistic(WeaponDamage, Terrain::T_MELEE, Terrain, GetsAdvantage), InternalWeaponType->GetRate(), 0, Dimensions.x * 0.5f);
		if (InternalStateManager != nullptr)
			delete InternalStateManager;
		InternalStateManager = new MeleeStateManager();
		InternalStateManager->SetInternalCharacter(this);
		InternalStateManager->Init();
		break;
	case (UnitType::T_MAGIC) :
		InternalWeapon = new MagicWeapon(CalculateTerrainStatistic(WeaponDamage, Terrain::T_MAGIC, Terrain, GetsAdvantage), InternalWeaponType->GetRate(), InternalWeaponType->GetType());
		if (InternalStateManager != nullptr)
			delete InternalStateManager;
		InternalStateManager = new MagicStateManager();
		InternalStateManager->SetInternalCharacter(this);
		InternalStateManager->Init();
		break;
	case (UnitType::T_RANGE) :
		InternalWeapon = new RangeWeapon(CalculateTerrainStatistic(WeaponDamage, Terrain::T_RANGE, Terrain, GetsAdvantage), InternalWeaponType->GetRate(), 0, 1);
		if (InternalStateManager != nullptr)
			delete InternalStateManager;
		InternalStateManager = new RangeStateManager();
		InternalStateManager->SetInternalCharacter(this);
		InternalStateManager->Init();
		break;
	}
	// We cannot Assign this character to the partition as we will not be able to tell how many are left alive
	//AssignToSPG();
}