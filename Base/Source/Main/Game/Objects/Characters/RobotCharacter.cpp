#include "RobotCharacter.h"
#include "../../../Engine/System/RenderSystem.h"
#include "../../../Engine/System/SceneSystem.h"
#include "../../SceneManagement/ScenePartitionGraph.h"
#include "../../Objects/Miscellaneous/Boundary.h"
#include "LoadHmap.h"
#include "../../Objects/Miscellaneous/GameObject.h"
RobotCharacter::RobotCharacter()
{
	Init();
}

RobotCharacter::~RobotCharacter()
{
	Exit();
}

void RobotCharacter::Init()
{
	// Init Base Stats
	HealthPoints = MaxHealthPoints = 200;
	WalkSpeed = 3;
	DetectionRadius = 100.f;
	InternalTimer = 0;
	WaitTime = 0;

	if (InternalStateManager != nullptr)
		delete InternalStateManager;
	InternalStateManager = new RobotStateManager();
	InternalStateManager->Init();
	SetEntityID("Robot");

	Self = nullptr;
	LHand = nullptr;
	RHand = nullptr;
	LLeg = nullptr;
	RLeg = nullptr;
	HandAnimationTimer = 0.f;
	HandAnimation = 0.05f;
	DeathOffset = 0.f;
	InternalStateManager->SetInternalCharacter(this);
}

void RobotCharacter::Update(const float& dt)
{
	if (Active) // Still can update if invisible
	{
		if (InternalStateManager->GetCurrentState()->GetStateName() != "Dead")
		{
			Vector3 StoredVelocity = GetVelocity();
			if (!Static)
			{
				Vector3 LegitimatePos = (GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt) + Vector3(0,GetDimensions().y) * 0.2f;
				float TargetPositionY = SceneSystem::Instance().GetCurrentScene().TerrainScale.y * ReadHeightMap(SceneSystem::Instance().GetCurrentScene().m_heightMap, LegitimatePos.x / SceneSystem::Instance().GetCurrentScene().TerrainScale.x, LegitimatePos.z / SceneSystem::Instance().GetCurrentScene().TerrainScale.z);
				if (!(LegitimatePos.y - TargetPositionY <= Math::EPSILON && TargetPositionY - LegitimatePos.y <= Math::EPSILON))
				{
					//float RateofChangeY = (LegitimatePos.y - TargetPositionY) * (float)dt * (GlobalPlayer::Instance().MaximumMovementSpeed * 0.45f);
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
			}
			SetRotationAngle(Math::RadianToDegree(atan2(-GetVelocity().x, -GetVelocity().z)));

			CheckCollision();
			CheckCollision("Static");
		}
	}

	if (InternalStateManager != nullptr)
		InternalStateManager->Update((float)dt);

	if (HealthPoints <= 0)
	{
		// Despawn the robot
		DeathOffset += dt;
		SetPosition(GetPosition() - Vector3(0, DeathOffset));
	}
	HandAnimationTimer += dt;
	if (HandAnimationTimer >= 0.75f)
	{
		HandAnimation *= -1;
		HandAnimationTimer = 0;
	}
	LHand->Offset += Vector3(0, -DeathOffset, HandAnimation * dt);
	RHand->Offset -= Vector3(0, DeathOffset, HandAnimation * dt);
	LLeg->Offset -= Vector3(0, DeathOffset, HandAnimation * dt * 2);
	RLeg->Offset += Vector3(0, -DeathOffset, HandAnimation * dt * 2);
}

bool RobotCharacter::CheckCollision(const string& type)
{
	if (type == "Ray")
	{
		std::vector<Bullet*> bulletList = GlobalPlayer::Instance().WeaponSystem.BulletList;
		Boundary* temp = new Boundary();
		for (auto it : bulletList)
		{
			if (it->Active)
				if (temp->BoundaryCheckBullet(it->GetPosition(), this->GetPosition(), GetDimensions().x * 0.5f) ||
					temp->BoundaryCheckBullet(it->PrevPos, this->GetPosition(), GetDimensions().x * 0.5f))
				{
					if (temp->CheckCollisionLaser(this->GetPosition(), (*it), GetDimensions().x * 0.5f))
					{
						float MinY = SceneSystem::Instance().GetCurrentScene().TerrainScale.y * ReadHeightMap(SceneSystem::Instance().GetCurrentScene().m_heightMap, GetPosition().x / SceneSystem::Instance().GetCurrentScene().TerrainScale.x, GetPosition().z / SceneSystem::Instance().GetCurrentScene().TerrainScale.z);
						float MaxY = GetPosition().y + GetDimensions().y * 0.5f;

						if (it->GetPosition().y > MinY && it->GetPosition().y < MaxY)
						{
							if (it->GetPosition().y < (MaxY - MinY) * 0.8f + MinY)
							{
								this->HealthPoints -= it->Damage;
							}
							else
							{
								this->HealthPoints = 0;
							}
							it->Active = false;
						}
					}
				}
		}
		delete temp;
	}
	else if (type == "Static")
	{
		// Find Current Grid
		GridNode* GN = dynamic_cast<GridNode*>(Self->GetParentNode());//ScenePartitionGraph::Instance().FindGridForPosition(GetPosition());
		// Check Collision Here
		if (GN != nullptr)
		{
			std::vector<Node*> NodeList = GN->GetNodeList();
			for (std::vector<Node*>::iterator it = NodeList.begin(); it != NodeList.end(); ++it)
			{
				GameObject* GO = dynamic_cast<GameObject*>((*it)->GetEntity());
				if (GO != nullptr)
					if (GO->GetEntityID() != "Robot" && GO->Static)
					{
						if (GO->GetBoundary() != nullptr)
						{
							if (GO->Static)
							{
								Boundary* B = GO->GetBoundary();
								if (B->CheckCollision(GetPosition()))
								{
									SetPosition((GetPosition() - (GO->GetBoundary()->GetOverlappingDistance() * GO->GetBoundary()->GetOverlappingAxis())));
								}
							}
						}

					}
			}
		}
	}
	return true;
}

void RobotCharacter::GenerateRobot(const Vector3& bodyPosition)
{
	AssignToSPG();
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	SetRotationAxis(Vector3(0, 1, 0));

	Vector3 BotPosition = Vector3(bodyPosition.x, 5.f, bodyPosition.z);
	SetMesh("Robot_Body");
	SetPosition(BotPosition);
	SetDimensions(Vector3(3.5f, 3.5f, 3.5f));
	SetRotationAxis(Vector3(0, 1, 0));

	BaseObject* bodyPartBase;

	bodyPartBase = new BaseObject();
	LHand = new Node(Self, bodyPartBase);
	bodyPartBase->SetMesh("Robot_LHand");
	LHand->SetEntityID("Robot_LHand");
	LHand->Offset = Vector3(-0.7f, -0.2f, -0.2f);
	LHand->GetEntity()->SetDimensions(Vector3(0.5f, 1.f, 1.2f));

	Self->AttachChildNode(LHand);


	bodyPartBase = new BaseObject();
	RHand = new Node(Self, bodyPartBase);
	bodyPartBase->SetMesh("Robot_RHand");
	RHand->SetEntityID("Robot_RHand");
	RHand->Offset = Vector3(0.7f, -0.2f, -0.2f);
	RHand->GetEntity()->SetDimensions(Vector3(0.5f, 1.f, 1.2f));

	Self->AttachChildNode(RHand);


	bodyPartBase = new BaseObject();
	LLeg = new Node(Self, bodyPartBase);
	bodyPartBase->SetMesh("Robot_Leg");
	LLeg->SetEntityID("Robot_Leg");
	LLeg->Offset = Vector3(-0.25f, -0.8f);
	LLeg->GetEntity()->SetDimensions(Vector3(0.35f, 1.2f, 1.f));

	Self->AttachChildNode(LLeg);


	bodyPartBase = new BaseObject();
	RLeg = new Node(Self, bodyPartBase);
	bodyPartBase->SetMesh("Robot_Leg");
	RLeg->SetEntityID("Robot_Leg");
	RLeg->Offset = Vector3(0.25f, -0.8f);
	RLeg->GetEntity()->SetDimensions(Vector3(0.35f, 1.2f, 1.f));

	Self->AttachChildNode(RLeg);

	bodyPartBase = nullptr;
	delete bodyPartBase;
}

void RobotCharacter::AssignToSPG()
{
	Self = ScenePartitionGraph::Instance().CreateNewNode(this);
}

void RobotCharacter::Exit()
{
	if (InternalStateManager)
	{
		RobotStateManager* SM = dynamic_cast<RobotStateManager*>(InternalStateManager);
		SM->Exit();
		delete SM;
		InternalStateManager = nullptr;
	}
	if (Self)
		delete Self;
	if (LHand)
		delete LHand;
	if (RHand)
		delete RHand;
	if (LLeg)
		delete LLeg;
	if (RLeg)
		delete RLeg;
}