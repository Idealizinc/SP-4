#include "CharacterEntity.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/System/SceneSystem.h"

CharacterEntity::CharacterEntity() : InternalStateManager(nullptr), TargetEnemy(nullptr)
{
	Init();
}

CharacterEntity::~CharacterEntity()
{
	Exit();
}

void CharacterEntity::Init()
{
	WalkSpeed = 5.0;
	InternalTimer = 0;
	TargetEnemy = nullptr;
	HealthPoints = 100;
	TargetItem = nullptr;
	FieldOfView = 60;
	HealRate = 0.01f;
}

void CharacterEntity::Render()
{
	if (Active && Visible)
	{
		RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
		SceneSystem::Instance().GetCurrentScene().modelStack->PushMatrix();
		SceneSystem::Instance().GetCurrentScene().modelStack->Translate(GetPosition().x, GetPosition().y, GetPosition().z);
		SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(GetRotationAngle(), GetRotationAxis().x, GetRotationAxis().y, GetRotationAxis().z);
		SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(90, -1, 0, 0);
		SceneSystem::Instance().GetCurrentScene().modelStack->Scale(GetDimensions().x, GetDimensions().y, GetDimensions().z);
		Renderer->RenderMesh(*GetMesh(), true);
		SceneSystem::Instance().GetCurrentScene().modelStack->PopMatrix();
	}
}

void CharacterEntity::Update(const float& dt)
{
	if (InternalStateManager != nullptr)
		InternalStateManager->Update((float)dt);
	if (Active) // Still can update if invisible
	{
		Vector3 StoredVelocity = GetVelocity();
		if (!Static)
			SetPosition(GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt); 
		if (!GetVelocity().IsZero())
			LookVector = GetVelocity().Normalized();
		if (GetVelocity().LengthSquared() > 0)
			SetRotationAngle(Math::RadianToDegree(atan2(-GetVelocity().x, -GetVelocity().z)));
	}
}

void CharacterEntity::Exit()
{
	if (InternalStateManager)
	{
		InternalStateManager->Exit();
		delete InternalStateManager;
		InternalStateManager = nullptr;
	}
}

void CharacterEntity::AddToObjectList(BaseObject* Obj)
{
	//CharacterObjectList.insert(Obj);
}

bool CharacterEntity::RemoveFromObjectList(string ID)
{
	//set<BaseObject*>::iterator Iter = CharacterObjectList.begin();
	//while (Iter != CharacterObjectList.end())
	//{
	//	if ((*Iter)->GetEntityID() == ID)
	//	{
	//		CharacterObjectList.erase(Iter);
	//		return true;
	//	}
	//	++Iter;
	//}
	return false;
}

State* CharacterEntity::GetCurrentState()
{
	return InternalStateManager->GetCurrentState();
}
