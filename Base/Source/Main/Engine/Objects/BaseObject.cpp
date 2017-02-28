#include "BaseObject.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/System/SceneSystem.h"

// Constructors
BaseObject::BaseObject()
{
	LightEnabled = true;
	Static = false;
	SetParameters(nullptr, 0, Vector3(), Vector3(1, 1, 1), Vector3(), 0, Vector3(0, 0, 1));
}

BaseObject::BaseObject(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	Init(MeshName, Mass, Position, Dimensions, Velocity, RotationAngle, RotationAxis);
}

// Destructor
BaseObject::~BaseObject()
{
	Exit();
}

// Virtual
void BaseObject::Init(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	Active = true;
	Visible = true;
	SetParameters(MeshName, Mass, Position, Dimensions, Velocity, RotationAngle, RotationAxis);
}

void BaseObject::Render()
{
	if (Active && Visible && StoredMesh)
	{
		RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
		SceneSystem::Instance().GetCurrentScene().modelStack->PushMatrix();
		SceneSystem::Instance().GetCurrentScene().modelStack->Translate(Position.x, Position.y, Position.z);
		SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(RotationAngle, RotationAxis.x, RotationAxis.y, RotationAxis.z);
		SceneSystem::Instance().GetCurrentScene().modelStack->Scale(Dimensions.x, Dimensions.y, Dimensions.z);
		if (Dimensions.LengthSquared() > 0.05f)
		Renderer->RenderMesh(*StoredMesh, LightEnabled);
		SceneSystem::Instance().GetCurrentScene().modelStack->PopMatrix();
	}
}

void BaseObject::Update(const float& dt)
{
	if (Active) // Still can update if invisible
	{
		Vector3 StoredVelocity = Velocity;
		//Velocity += m_gravity * dt; // For Gravity
		if (!Static)
			Position += (StoredVelocity + Velocity) * 0.5f * (float)dt;
		RotationAngle = Math::RadianToDegree(atan2(-Velocity.x, Velocity.y));
	}
}

void BaseObject::Exit()
{
	// Clean up if necessary
	Active = false;
}

// Setters
void BaseObject::SetMesh(Mesh* Mesh)
{
	this->StoredMesh = Mesh;
}

void BaseObject::SetMesh(const std::string& MeshName)
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	std::map<std::string, Mesh*>::iterator it = Renderer->MeshList.find(MeshName);
	if (it != Renderer->MeshList.end())
	{
		this->MeshName = MeshName;
		this->StoredMesh = it->second;
	}
}

void BaseObject::SetPosition(const Vector3& Position)
{
	this->Position = Position;
}

void BaseObject::SetDimensions(const Vector3& Dimensions)
{
	this->Dimensions = Dimensions;
}

void BaseObject::SetRotationAngle(const float& RotationAngle)
{
	this->RotationAngle = RotationAngle;
}

void BaseObject::SetRotationAxis(const Vector3& RotationAxis)
{
	this->RotationAxis = RotationAxis;
}

void BaseObject::SetMass(const float& Mass)
{
	this->Mass = Mass;
}

void BaseObject::SetParameters(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	SetMesh(MeshName);
	this->Position = Position;
	this->Dimensions = Dimensions;
	this->Velocity = Velocity;
	this->RotationAngle = RotationAngle;
	this->RotationAxis = RotationAxis;
	this->Mass = Mass;
}

void BaseObject::SetParameters(Mesh* Mesh, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	SetMesh(Mesh);
	this->Position = Position;
	this->Dimensions = Dimensions;
	this->Velocity = Velocity;
	this->RotationAngle = RotationAngle;
	this->RotationAxis = RotationAxis;
	this->Mass = Mass;
}

void  BaseObject::SetVelocity(const Vector3& Velocity)
{
	this->Velocity = Velocity;
}

// Getters
Mesh* BaseObject::GetMesh() const
{
	return StoredMesh;
}

std::string BaseObject::GetMeshName() const
{
	return MeshName;
}

Vector3 BaseObject::GetPosition() const
{
	return Position;
}

Vector3 BaseObject::GetDimensions() const
{
	return Dimensions;
}

float BaseObject::GetRotationAngle() const
{
	return RotationAngle;
}

Vector3 BaseObject::GetRotationAxis() const
{
	return RotationAxis;
}

float BaseObject::GetMass() const
{
	return Mass;
}

Vector3 BaseObject::GetVelocity() const
{
	return Velocity;
}