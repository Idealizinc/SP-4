#include "Node.h"
#include "../../../Engine/System/SceneSystem.h"
#include "../../../Engine/System/RenderSystem.h"
#include "../../SceneManagement/ScenePartitionGraph.h"

Node::Node(Node* Parent, BaseObject* Internal)
{
	if (Parent != nullptr)
		this->GridIndex = Parent->GridIndex;
	this->ParentNode = Parent;
	this->Object = Internal;
	if (this->Object != nullptr)
		this->SetEntityID(this->Object->GetEntityID());
}

Node::~Node()
{
	// The exit will be called upon when the Node is deleted.
	Exit();
}


// Main
void Node::Init()
{

}

void Node::Update(const float& dt)
{
	if (Object != nullptr)
		if (!Object->Static)
			Object->Update(dt);
	// Update Child Nodes
	for (std::vector<Node*>::iterator it = ChildNodeList.begin(), end = ChildNodeList.end(); it != end; ++it)
	{
		if (!(*it)->GetEntity()->Static)
			(*it)->Update(dt);
	}
}

void Node::Render()
{
	if (Object != nullptr)
	{
		if (Object->Visible)
		{
			if (ParentNode->GetEntityID() == "Partition")
			{
				GridNode* Parent = dynamic_cast<GridNode*>(ParentNode);
				//if (ParentNode != ScenePartitionGraph::Instance().FindGridForPosition(GlobalPlayer::Instance().GetCurrentPosition()))
				//{
				//	float LODDistance = (ScenePartitionGraph::Instance().GridDimensions.x / ScenePartitionGraph::Instance().GridDivisions) * 0.25f;
				//	float PlayerDistance = (GlobalPlayer::Instance().GetCurrentPosition() - Object->GetPosition()).LengthSquared();
				//	Parent->LODLevel = Math::Clamp((int)(PlayerDistance / (LODDistance * LODDistance)), 0, 2);
				//}
				//else Parent->LODLevel = 0;

				RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
				SceneSystem::Instance().GetRenderSystem().modelStack->PushMatrix();
				SceneSystem::Instance().GetRenderSystem().modelStack->Translate(GetEntity()->GetPosition().x, GetEntity()->GetPosition().y, GetEntity()->GetPosition().z);
				SceneSystem::Instance().GetRenderSystem().modelStack->Rotate(GetEntity()->GetRotationAngle(), GetEntity()->GetRotationAxis().x, GetEntity()->GetRotationAxis().y, GetEntity()->GetRotationAxis().z);
				SceneSystem::Instance().GetRenderSystem().modelStack->Scale(GetEntity()->GetDimensions().x, GetEntity()->GetDimensions().y, GetEntity()->GetDimensions().z);

				/*std::map<std::string, std::vector<Mesh*>>::iterator iter = ScenePartitionGraph::Instance().LODList.find(Object->GetEntityID());
				if (iter != ScenePartitionGraph::Instance().LODList.end())
					Renderer->RenderMesh(*iter->second.at(Parent->LODLevel), true);
				else */Renderer->RenderMesh(*Object->GetMesh(), true);

				SceneSystem::Instance().GetRenderSystem().modelStack->PopMatrix();
				Parent->LODLevel = 0;
			}

		}
	}
}

void Node::Exit()
{
	//ClearNodeList();
	if (Object != nullptr)
	{
		delete Object;
		Object = nullptr;
	}
}


// Internal Entity Methods
void Node::SetEntity(BaseObject* Entity)
{
	this->Object = Entity;
}

BaseObject* Node::GetEntity() const
{
	return this->Object;
}

Vector3 Node::GetGridIndex() const
{
	return GridIndex;
}

void Node::SetGridIndex(const Vector3& V)
{
	GridIndex = V;
}

// Parent Node Methods
void Node::SetParentNode(Node* Node)
{
	this->ParentNode = Node;
}

Node* Node::GetParentNode() const
{
	return this->ParentNode;
}


// Child Node List Methods
// Attach & Detach
void Node::AttachChildNode(Node* Node)
{
	this->ChildNodeList.push_back(Node);
}

void Node::AttachChildEntity(BaseObject* Entity)
{
	Node* newNode = new Node(this, Entity);
	this->AttachChildNode(newNode);
}

Node* Node::DetachChildNode(const std::string& EntityID)
{
	// Detach from list and return
	Node* Detach = nullptr;
	for (std::vector<Node*>::iterator it = ChildNodeList.begin(), end = ChildNodeList.end(); it != end; ++it)
	{
		if ((*it)->GetEntityID() == EntityID)
		{
			// Assign
			Detach = *it;
			// Erase from list
			this->ChildNodeList.erase(it);
			break;
		}
	}
	return Detach;
}

Node* Node::DetachChildNode(Node* Node)
{
	return DetachChildNode(Node->GetEntityID());
}

// Delete
bool Node::DeleteChildNode(const std::string& EntityID)
{
	// Detach from list and delete
	Node* Delete = nullptr;
	if ((Delete = DetachChildNode(EntityID)) != nullptr)
	{
		delete Delete;
		return true;
	}
	return false;
}

bool Node::DeleteChildNode(Node* Node)
{
	return DeleteChildNode(Node->GetEntityID());
}

void Node::ClearNodeList()
{
	if (ChildNodeList.size() > 0)
	{
		for (std::vector<Node*>::iterator it = ChildNodeList.begin(); it != ChildNodeList.end();)
		{
			if ((*it) != nullptr)
			{
				delete * it;
				it = ChildNodeList.erase(it);
			}
			else {
				++it;
			}
		}
		ChildNodeList.clear();
	}
	// Reset the pointers for this node
	Object = nullptr;
	ParentNode = nullptr;
}

// NodeList Related
unsigned Node::GetNodeListSize()
{
	return ChildNodeList.size();
}

std::vector<Node*>& Node::GetNodeList()
{
	return ChildNodeList;
}

bool Node::CheckForNode(Node* Node)
{
	return CheckForNode(Node->GetEntityID());
}

bool Node::CheckForNode(const std::string& EntityID)
{
	if (FindNode(EntityID) != nullptr)
		return true;
	else return false;
}

Node* Node::FindNode(const std::string& EntityID)
{
	for (std::vector<Node*>::iterator it = ChildNodeList.begin(), end = ChildNodeList.end(); it != end; ++it)
	{
		if ((*it)->GetEntityID() == EntityID)
		{
			return *it;
		}
	}
	return nullptr;

}