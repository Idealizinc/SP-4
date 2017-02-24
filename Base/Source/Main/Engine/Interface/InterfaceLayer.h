/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 21 February 2017 14:34 PM
File Name     : InterfaceLayer.h
Purpose       : Defines an InterfaceLayer, a storage element that holds child InterfaceElements.
*/

#ifndef _INTERFACE_LAYER_H
#define _INTERFACE_LAYER_H

#include "../Internal/System.h"
#include "InterfaceElement.h"

class InterfaceLayer : public System, public BaseObject
{
public:
	InterfaceLayer(){};
	virtual ~InterfaceLayer(void){};

	virtual void Init(void);
	virtual void Update(const float& dt);
	virtual void Render(void);
	virtual void Exit(void);

	Vector3 GetTargetPosition();

	void SetTargetPosition(const Vector3& Position);
	void SetOriginalPosition(const Vector3& Position);

	void ResetToOriginal();
	void SwapOriginalWithTarget();
	InterfaceElement* CreateNewInterfaceElement(const std::string& ElementID, const std::string& MeshID, const Vector3& OriginalPosition, const Vector3& Dimensions);
	std::vector<InterfaceElement*>& GetContainer();

private:
	std::vector<InterfaceElement*> InternalElementContainer;
	Vector3 TargetPosition;
	Vector3 OriginalPosition;

protected:

};


#endif //_INTERFACE_LAYER_H