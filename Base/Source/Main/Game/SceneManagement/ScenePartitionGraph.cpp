#include "ScenePartitionGraph.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/System/SceneSystem.h"

Mesh* FindMesh(const string& name)
{
	Mesh* mesh = nullptr;
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	std::map<std::string, Mesh*>::iterator it = Renderer->MeshList.find(name);
	if (it != Renderer->MeshList.end())
	{
		mesh = it->second;
	}
	return mesh;
}

void ScenePartitionGraph::Init()
{
	AssignGridParameters(Vector3(), Vector3(1000, 1000), 2);
}

void ScenePartitionGraph::Update(const float& dt)
{
	while (ReallocationList.size() > 0)
	{
		Node* ToBeRealloc = ReallocationList.back();
		if (ToBeRealloc != nullptr)
		{
			GridNode* OldGrid = nullptr;
			for (std::vector<GridNode*>::iterator it = PartitionGraphContainer.begin(); it != PartitionGraphContainer.end(); ++it)
			{
				if ((*it)->GetGridIndex() == ToBeRealloc->GetGridIndex())
				{
					OldGrid = (*it);
					for (std::vector<Node*>::iterator it = OldGrid->GetNodeList().begin(); it != OldGrid->GetNodeList().end(); ++it)
					{
						if (*it == ToBeRealloc)
						{
							OldGrid->GetNodeList().erase(it);
							break;
						}
					}
					break;
				}
			}
		}
		bool Check = AssignObject(ToBeRealloc);
		if (!Check)
		{
			// Reallocation failed
			// Delete the node
			delete ToBeRealloc;
			ToBeRealloc = nullptr;
		}
		ReallocationList.pop_back();
	}
	for (std::vector<GridNode*>::iterator it = PartitionGraphContainer.begin(); it != PartitionGraphContainer.end(); ++it)
	{
		(*it)->Update(dt);
	}
}

void ScenePartitionGraph::Render()
{
	float GridDivisionScale = GridDimensions.x / GridDivisions;
	for (std::vector<GridNode*>::iterator it = PartitionGraphContainer.begin(); it != PartitionGraphContainer.end(); ++it)
	{
		(*it)->Render();
	}
	
}

void ScenePartitionGraph::Exit()
{
	for (std::vector<Node*>::iterator it = ReallocationList.begin(); it != ReallocationList.end(); ++it)
	{
		delete *it;
		it = ReallocationList.erase(it);
	}
	for (std::vector<GridNode*>::iterator it = PartitionGraphContainer.begin(); it != PartitionGraphContainer.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			(*it)->ClearNodeList();
			delete *it;
		}
	}
	PartitionGraphContainer.clear();
	for (auto it : WaypointList)
	{
		it->Exit();
		delete it;
	}
	WaypointList.clear();
}


void ScenePartitionGraph::GenerateGrid()
{
	float XInterval = GridDimensions.x / GridDivisions;
	float YInterval = GridDimensions.y / GridDivisions;
	float Interval = (GridDivisions - 1) * 0.5f;

	for (unsigned i = 0; i < GridDivisions; ++i)
	{
		for (unsigned j = 0; j < GridDivisions; ++j)
		{
			Vector3 Index(Vector3((float)j, (float)i));
			GridNode* GN = new GridNode();
			GN->SetPosition(Vector3(XInterval * j - XInterval * Interval, YInterval * i - YInterval * Interval) + GridCenter);
			GN->SetDimensions(Vector3(XInterval, YInterval));
			GN->SetGridIndex(Index);
			BaseObject* Cube = new BaseObject();
			Cube->SetMesh("Partition");
			Cube->GetMesh()->mode = Cube->GetMesh()->DRAW_LINES;
			Cube->SetPosition(Vector3(GN->GetPosition().x, 0, GN->GetPosition().y));
			Cube->SetDimensions(Vector3(GN->GetDimensions().x, 1, GN->GetDimensions().y));
			Cube->LightEnabled = false;
			GN->SetEntity(Cube);
			PartitionGraphContainer.push_back(GN);
		}
	}
}

GridNode* ScenePartitionGraph::FindGridForPosition(const Vector3& Position)
{
	Vector3 Index = FindGridIndexForPosition(Position);
	GridNode* N = nullptr;
	for (std::vector<GridNode*>::iterator it = PartitionGraphContainer.begin(); it != PartitionGraphContainer.end(); ++it)
	{
		if (Index == (*it)->GetGridIndex())
		{
			N = *it;
			break;
		}
	}
	return N;
}

Vector3 ScenePartitionGraph::FindGridIndexForPosition(const Vector3& Position)
{
	return Vector3((float)(int)((Position.x + GridCenter.x + GridDimensions.x * 0.5f) / GridDimensions.x * GridDivisions), (float)(int)((Position.z + GridCenter.y + GridDimensions.y * 0.5f) / GridDimensions.y * GridDivisions));
}

Node* ScenePartitionGraph::CreateNewNode(BaseObject* B)
{
	Node* N = nullptr;
	GridNode* PositionalNode = FindGridForPosition(B->GetPosition());
	if (PositionalNode != nullptr)
	{
		N = new Node(PositionalNode, B);
		PositionalNode->AttachChildNode(N);
		return N;
	}
	else return N;
}

bool ScenePartitionGraph::AssignObject(Node* N)
{
	GridNode* PositionalNode = FindGridForPosition(N->GetEntity()->GetPosition());
	if (PositionalNode != nullptr)
	{
		N->SetGridIndex(PositionalNode->GetGridIndex());
		N->SetParentNode(PositionalNode);
		PositionalNode->AttachChildNode(N);
		return true;
	}
	else return false;
}


std::vector<GridNode*>& ScenePartitionGraph::GetContainer()
{
	return PartitionGraphContainer;
}

void ScenePartitionGraph::AssignGridParameters(const Vector3& GridCenter, const Vector3& GridDimensions, const unsigned& GridDivisions)
{
	this->GridCenter = GridCenter;
	this->GridDimensions = GridDimensions;
	this->GridDivisions = GridDivisions;
}
