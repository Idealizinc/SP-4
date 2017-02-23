/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 21 February 2017 11:17 AM
File Name     : InterfaceElement.h
Purpose       : Defines an InterfaceElement, a 2D user interface element that exists within the camera space.
*/

#ifndef _INTERFACE_ELEMENT_H
#define _INTERFACE_ELEMENT_H

#include "../Objects/BaseObject.h"
#include "../../Game/Objects/Miscellaneous/Boundary2D.h"
#include <vector>


class InterfaceElement : public BaseObject
{
public:
	InterfaceElement();
	virtual ~InterfaceElement();

	// Main
	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render(){};
	void Render(const Vector3& LayerPosition);
	virtual void Exit();

	// Getters
	Vector3 GetTargetPosition();
	Vector3 GetOriginalPosition();

	// Setters
	void SetTextWrapping(const bool& Enabled);
	void SetTextColor(const Vector3& Color);
	void SetText(const std::string& Text);
	void SetText(const std::string& Text, const float& Size, const bool& WrapEnabled, const bool& ScalingEnabled);
	void SetTargetPosition(const Vector3& Position);
	void SetOriginalPosition(const Vector3& Position);
	
	// Additional
	bool DetectUserInput(const Vector3& MousePosition, const Vector3& ParentLayerPosition);
	void ResetToOriginal();
	void SwapOriginalWithTarget();

	// Variables

private:
	// Internals
	// Bounds
	Boundary2D* Boundary = nullptr;
	// Text
	Vector3 TextColor;
	std::string ElementText; 
	float TextSize;
	bool DynamicScalingEnabled;

	// Text Wrapping
	bool TextWrappingEnabled;
	std::vector<std::string> WrappedTextContainer;

	// Animations
	Vector3 TargetPosition;
	Vector3 OriginalPosition;

	// Functions
	void WrapElementText();

protected:

};

#endif //_INTERFACE_ELEMENT_H