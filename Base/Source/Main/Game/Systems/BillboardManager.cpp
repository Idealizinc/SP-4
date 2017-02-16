#include "BillboardManager.h"
#include <algorithm>

BillboardManager::BillboardManager()
{
	Init();
}

BillboardManager::~BillboardManager()
{
	Exit();
}

void BillboardManager::Init()
{

}

void BillboardManager::Update(const float& dt)
{
	for (std::vector<Particle*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); ++it)
	{
		if ((*it)->Active)
			(*it)->Update((float)dt);
	}
}

void BillboardManager::Render()
{
	for (std::vector<Particle*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); ++it)
	{
		if ((*it)->Active)
			(*it)->Render();
	}
}

void BillboardManager::Exit()
{
    for (std::vector<Particle*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); ++it)
    {
        (*it)->Exit();
        delete *it;
    }
    BillboardContainer.clear();
}

void BillboardManager::UpdateContainer(float dt, const Vector3 &CameraPosition){
	for (std::vector<Particle*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); it++)
	{
		Particle* B = *it;
		B->PlayerPosition = CameraPosition;
	}
	Update(dt);
	if (BillboardContainer.size() > 0)
		std::sort(&BillboardContainer[0], &BillboardContainer[BillboardContainer.size() - 1]);

}

Particle* BillboardManager::FetchB()
{
	for (std::vector<Particle*>::iterator it = BillboardContainer.begin(); it != BillboardContainer.end(); ++it)
	{
		Particle* B = *it;
		if (B->Active == false)
		{
			B->Reset();
			return B;
		}
	}
	// No inactive object found
	for (int i = 0; i < 10; i++)
	{
		BillboardContainer.push_back(new Particle());
	}
	Particle* B = BillboardContainer.back();
	B->Reset();
	return B;
}

void BillboardManager::AddHMapBillboard(const std::string& MeshName, std::vector<unsigned char>& heightMap, const Vector3& TerrainScale, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition)
{
	Particle* B = FetchB();
	Vector3 Pos = Position;
	Pos.y = Dimensions.y * 0.5f + TerrainScale.y * ReadHeightMap(heightMap, Position.x / TerrainScale.x, Position.z / TerrainScale.z);
	B->SetEntityID(MeshName);
	B->SetParameters(MeshName, 1, Pos, Dimensions, 0, 0, Vector3(0, 1, 0));
	B->PlayerPosition = PlayerPosition;
	B->SetMeshBasedOnID();
}

void BillboardManager::AddBillboard(const std::string& MeshName, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition)
{
	Particle* B = FetchB();
	B->SetParameters(MeshName, 1, Position, Dimensions, 0, 0, Vector3(0, 1, 0));
	B->PlayerPosition = PlayerPosition;
	B->SetMeshBasedOnID();
}
