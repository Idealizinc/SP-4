#include "InterfaceElement.h"
#include "../System/RenderSystem.h"
#include "../System/SceneSystem.h"
#include <sstream>
#include <algorithm>

InterfaceElement::InterfaceElement()
{
	Init();
}

InterfaceElement::~InterfaceElement()
{
	Exit();
}


// Main
void InterfaceElement::Init()
{
	TextColor = Vector3(1, 1, 1);
	ElementText = "";
	TextSize = 1.f;
	TextWrappingEnabled = DynamicScalingEnabled = false;

	if (Boundary == nullptr)
		Boundary = new Boundary2D();
}

void InterfaceElement::Update(const float& dt)
{
	Vector3 DirVec = TargetPosition - Position;
	if (Active && DirVec.LengthSquared() > 1.f)
	{
		Position += DirVec * dt;
	}
}

void InterfaceElement::Render(const Vector3& LayerPosition)
{
	if (Active && Visible)
	{
		float Divisor = 0.45f;
		glDisable(GL_CULL_FACE);
		RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
		Renderer->SetHUD(true);
		if (!TextWrappingEnabled)
			Renderer->RenderTextOnScreen(ElementText, Color(TextColor.x, TextColor.y, TextColor.z), Dimensions.y * Divisor, Position.x + LayerPosition.x - (ElementText.size() * 0.5f * 0.75f * Dimensions.y * Divisor), Position.y + LayerPosition.y - (0.5f * Dimensions.y * Divisor));
		else if (TextWrappingEnabled)
		for (unsigned int i = 0; i < WrappedTextContainer.size(); ++i)
		{
			Renderer->RenderTextOnScreen(WrappedTextContainer[i], Color(TextColor.x, TextColor.y, TextColor.z), Dimensions.y * 0.275f, Position.x - Dimensions.x * 0.5f + (Dimensions.y * 0.25f) + LayerPosition.x, Position.y + LayerPosition.y + (0.5f * Dimensions.y * 0.25f) - (i * Dimensions.y * 0.25f));
		}
		if (MeshName != "")
			Renderer->RenderMeshIn2D(*StoredMesh, false, Dimensions.x, Dimensions.y, Position.x + LayerPosition.x, Position.y + LayerPosition.y);
		Renderer->SetHUD(false);
		glEnable(GL_CULL_FACE);
	}
}

void InterfaceElement::Exit()
{
	if (Boundary)
	{
		delete Boundary;
		Boundary = nullptr;
	}
}


// Getters

// Setters
void InterfaceElement::SetTextWrapping(const bool& Enabled)
{
	TextWrappingEnabled = Enabled;
}

void InterfaceElement::SetTextColor(const Vector3& Color)
{
	TextColor = Color;
}

void InterfaceElement::SetText(const std::string& Text)
{
	ElementText = Text;
}

void InterfaceElement::SetText(const std::string& Text, const float& Size, const bool& WrapEnabled, const bool& ScalingEnabled)
{
	ElementText = Text;
	TextSize = Size;
	TextWrappingEnabled = WrapEnabled;
	DynamicScalingEnabled = ScalingEnabled;
}

void InterfaceElement::SetTargetPosition(const Vector3& Position)
{
	TargetPosition = Position;
}

void InterfaceElement::SetOriginalPosition(const Vector3& Position)
{
	OriginalPosition = Position;
	SetPosition(Position);
}

// Additional
bool InterfaceElement::DetectUserInput(const Vector3& MousePosition, const Vector3& ParentLayerPosition)
{
	if (Active && Visible)
	{
		Boundary->SetPosition(Position + ParentLayerPosition);
		Boundary->SetDimensions(Dimensions);
		Boundary->ResetValues();

		return Boundary->CheckCollision(MousePosition);
	}
	return false;
}

void InterfaceElement::ResetToOriginal()
{
	Position = OriginalPosition;
}

void InterfaceElement::SwapOriginalWithTarget()
{
	std::swap(TargetPosition, OriginalPosition);
}

void InterfaceElement::WrapElementText()
{
	WrappedTextContainer.clear();
	float SingleSpace = 0.75f * Dimensions.y * 0.3f;
	float RequiredSpace = ElementText.size() * SingleSpace;
	int MaxCharactersPerLine = (int)(Dimensions.x / SingleSpace);
	std::stringstream Tmp, MainString;
	int CPos = 0;
	int CCheck = 0;
	if (MaxCharactersPerLine < (int)ElementText.size())
	while (CPos < (int)ElementText.size())
	{
		Tmp << ElementText[CPos];
		++CPos;
		if (CPos == ElementText.size() || ElementText[CPos] == ' ')
		{
			MainString << Tmp.str();
			Tmp.str("");
		}
		if (CPos == ElementText.size() || CPos >= MaxCharactersPerLine + MaxCharactersPerLine * CCheck)
		{
			CCheck++;
			WrappedTextContainer.push_back(MainString.str());
			MainString.str("");
			if (Tmp)
			{
				MainString << Tmp.str();
			}
			Tmp.str("");
			TextWrappingEnabled = true;
		}
	}
	if (WrappedTextContainer.size() > 1)
	{
		for (unsigned i = 0; i < WrappedTextContainer.size(); ++i)
		{
			if (WrappedTextContainer[i].at(0) == ' ')
			{
				WrappedTextContainer[i].erase(0, 1);
			}
		}
	}
	else TextWrappingEnabled = false;
}

Vector3 InterfaceElement::GetTargetPosition()
{
	return TargetPosition;
}

Vector3 InterfaceElement::GetOriginalPosition()
{
	return OriginalPosition;
}