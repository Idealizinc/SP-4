#include "GridNode.h"
#include "../ScenePartitionGraph.h"
#include "../../../Engine/System/SceneSystem.h"

GridNode::GridNode()
{
	Object = nullptr;
	SetEntityID("Partition");
	// ParentNode = SPG->GetRootNode();
}

GridNode::~GridNode()
{
	if (Object)
		delete Object;
	Object = nullptr;
}

void GridNode::SetDimensions(const Vector3& V)
{
	Dimensions = V;
}

void GridNode::SetPosition(const Vector3& V)
{
	Position = V;
}

Vector3 GridNode::GetDimensions() const
{
	return Dimensions;
}

Vector3 GridNode::GetPosition() const
{
	return Position;
}


bool GridNode::CheckWithin(const Vector3& V)
{
	// Point collision
	if (abs(this->GetPosition().x - V.x) > (this->GetDimensions().x) * 0.5f) 
		return false;
	if (abs(this->GetPosition().y - V.y) > (this->GetDimensions().y) * 0.5f) 
		return false;
	return true;
}

bool GridNode::CheckWithin(BaseObject* B)
{
	return CheckWithin(B->GetPosition());
}

bool GridNode::CheckWithin(Node* N)
{
	return CheckWithin(N->GetEntity()->GetPosition());
}

void GridNode::Update(const float& dt)
{
	if (Object != nullptr)
		if (!Object->Static)
			Object->Update(dt);

	// Cleanup
	for (std::vector<Node*>::iterator it = ChildNodeList.begin(); it != ChildNodeList.end(); ++it)
	{
		if (!(*it)->GetEntity()->Active)
		{
			delete *it;
			it = ChildNodeList.erase(it);
			if (ChildNodeList.size() <= 0)
				break;
		}
	}

	// Update Child Nodes
	for (std::vector<Node*>::iterator it = ChildNodeList.begin(); it != ChildNodeList.end(); ++it)
	{
		if (!(*it)->GetEntity()->Static)
		{
			GridNode* GN = SceneSystem::Instance().GetCurrentScene().ScenePartition->FindGridForPosition((*it)->GetEntity()->GetPosition());
			if (GN != nullptr)
			{
				Vector3 SupposedGridIndex = GN->GetGridIndex();
				if (GridIndex != SupposedGridIndex)
				{
					SceneSystem::Instance().GetCurrentScene().ScenePartition->ReallocationList.push_back(*it);
				}
				else {
					(*it)->Update(dt);
				}
			}
		}
	}
}

void GridNode::Render()
{
	//if (Object != nullptr)
	//if (SceneSystem::Instance().GetCurrentScene().ScenePartition->ShowPartitions && Object->Visible)
	//		Object->Render();
	// Render Child Nodes
	for (std::vector<Node*>::iterator it = ChildNodeList.begin(), end = ChildNodeList.end(); it != end; ++it)
	{
		(*it)->Render();
	}
}
