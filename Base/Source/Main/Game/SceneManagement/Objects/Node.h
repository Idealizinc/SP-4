/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 10 December 2016 14:57 PM
File Name     : Node.h
Purpose       : Defines a basic Node
*/

#ifndef _NODE_H
#define _NODE_H

#include "../../../Engine/Internal/Entity.h"
#include "../../../Engine/Objects/BaseObject.h"
#include <vector>

class Node : public Entity
{
public:
	Node(Node* Parent = nullptr, BaseObject* Internal = nullptr);
	virtual ~Node();

	Vector3 Offset;

	// Main
	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render();
	virtual void Exit();
	
	// Internal Entity Methods
	void SetEntity(BaseObject*);
	BaseObject* GetEntity() const;

	// Parent Node Methods
	void SetParentNode(Node*);
	Node* GetParentNode() const;

	// Child Node List Methods
	// Attach & Detach
	void AttachChildNode(Node*);
	void AttachChildEntity(BaseObject*);
	Node* DetachChildNode(const std::string& EntityID);
	Node* DetachChildNode(Node*);
	// Delete
	bool DeleteChildNode(const std::string& EntityID);
	bool DeleteChildNode(Node*);
	void ClearNodeList();
	// NodeList Related
	unsigned GetNodeListSize();
	std::vector<Node*>& GetNodeList();
	bool CheckForNode(Node*);
	bool CheckForNode(const std::string& EntityID);
	Node* FindNode(const std::string& EntityID);
	Vector3 GetGridIndex() const;
	void SetGridIndex(const Vector3&);
private:

protected:
	// Defines Entity that represents this node in the scene
	BaseObject* Object;
	// Defines the parent of this node
	Node* ParentNode;
	// Stored list of child nodes
	std::vector<Node*> ChildNodeList;

	Vector3 GridIndex;
};

#endif //_NODE_H