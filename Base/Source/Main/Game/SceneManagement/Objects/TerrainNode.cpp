#include "TerrainNode.h"

TerrainNode::~TerrainNode()
{
	//Exit();
	if (Object != nullptr)
		delete Object;
	Object = nullptr;
	if (TerrainTile != nullptr)
		delete TerrainTile;
	TerrainTile = nullptr;
}

void TerrainNode::Exit()
{
	for (std::vector<TerrainNode*>::iterator it = LinkedTerrainNodes.begin(); it != LinkedTerrainNodes.end(); ++it)
	{
		delete *it;
		it = LinkedTerrainNodes.erase(it);
	}
}

void TerrainNode::AssignTerrainNode(TerrainNode* TN)
{
	LinkedTerrainNodes.push_back(TN);
}

float TerrainNode::CalculateRemainderPathCost(TerrainNode* End)
{
	Vector3 AxialNodeDistance = End->GetEntity()->GetPosition() - GetEntity()->GetPosition();
	float CumulativeNodeDistanceForXZ = abs(AxialNodeDistance.x) + abs(AxialNodeDistance.z);
	return CumulativeNodeDistanceForXZ;

	//return (abs(End->GetEntity()->GetPosition().x - GetEntity()->GetPosition().x) + abs(End->GetEntity()->GetPosition().z - GetEntity()->GetPosition().z));
}