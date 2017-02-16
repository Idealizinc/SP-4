#include "GridNode.h"
#include "../ScenePartitionGraph.h"

GridNode::GridNode()
{
	Object = nullptr;
	SetEntityID("Partition");
	// ParentNode = SPG->GetRootNode();
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
	// Update Child Nodes
	for (std::vector<Node*>::iterator it = ChildNodeList.begin(), end = ChildNodeList.end(); it != end; ++it)
	{
		if (!(*it)->GetEntity()->Static)
		{
			GridNode* GN = ScenePartitionGraph::Instance().FindGridForPosition((*it)->GetEntity()->GetPosition());
			if (GN != nullptr)
			{
				Vector3 SupposedGridIndex = GN->GetGridIndex();
				if (GridIndex != SupposedGridIndex)
				{
					ScenePartitionGraph::Instance().ReallocationList.push_back(*it);
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
	if (Object != nullptr)
		if (ScenePartitionGraph::Instance().ShowPartitions && Object->Visible)
			Object->Render();
	// Render Child Nodes
	for (std::vector<Node*>::iterator it = ChildNodeList.begin(), end = ChildNodeList.end(); it != end; ++it)
	{
		(*it)->Render();
	}
}
