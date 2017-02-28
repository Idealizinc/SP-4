#include "SpriteObject.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/System/SceneSystem.h"

void SpriteObject::Init()
{
	Bounds = nullptr;
}

void SpriteObject::Update(double dt)
{
	if (Active)
	{

		Timer += (float)dt;
		Vector3 StoredVelocity = Velocity;

		if (!Static)
			Position += (StoredVelocity + Velocity) * 0.5f * (float)dt;
		RotationAngle = Math::RadianToDegree(atan2(-Velocity.x, Velocity.y));

		CurrentFrameMultiplier = Timer / anim_Time;
		CurrentFrame = (int)(AnimMap.size() * CurrentFrameMultiplier);

	}
}

void SpriteObject::Render()
{
	if (Active && Visible)
	{
		RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
		SceneSystem::Instance().GetCurrentScene().modelStack->PushMatrix();
		SceneSystem::Instance().GetCurrentScene().modelStack->Translate(Position.x, Position.y, Position.z);
		SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(RotationAngle, RotationAxis.x, RotationAxis.y, RotationAxis.z);
		if (Dimensions.LengthSquared() > 0.05f)
			SceneSystem::Instance().GetCurrentScene().modelStack->Scale(Dimensions.x, Dimensions.y, Dimensions.z);
		Renderer->RenderMesh(*StoredMesh, true);
		SceneSystem::Instance().GetCurrentScene().modelStack->PopMatrix();
	}
}

void SpriteObject::Exit()
{
	if (Bounds)
	{
		delete Bounds;
		Bounds = nullptr;
	}
}

void SpriteObject::SetSprite(const std::string& SpriteName)
{
	RenderSystem* Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	std::map<std::string, std::map<std::string, std::vector<GLuint>>>::iterator it = Renderer->SpriteList.find(SpriteName);
	if (it != Renderer->SpriteList.end())
	{
		this->MeshName = SpriteName;
		AnimMap = it->second;
	}
}