#include "SceneSystem.h"
#include "RenderSystem.h"
#ifdef _DEBUG
#include <assert.h>
#endif

void SceneSystem::Init()
{
	Renderer = nullptr;
	PreviousScene = nullptr;
	TransitionLayer = new InterfaceLayer();
	cSS_InputManager = new InputManager();
	AnimationActivated = AnimationDirectionInwards = false;
}

void SceneSystem::Update(const float& dt)
{
//#ifdef _DEBUG
//	assert(dt > 0 || dt < 0 || dt == 0);
//#endif
	if (AnimationActivated)
		TransitionLayer->Update(dt * TransitionSpeed);

	AnimationActivated = false;
	for (auto it : TransitionLayer->GetContainer())
	{
		if ((it->GetPosition() - it->GetTargetPosition()).LengthSquared() > MinimumAcceptableDistanceSquared)
		{
			AnimationActivated = true;
			break;
		}
	}
	if (!AnimationActivated && AnimationDirectionInwards)
	{
		AnimateTransitionLayer(AnimationDirectionInwards = false);
		//TransitToTargetedScene();
		cSS_InputManager->CenterCursor();
		Renderer->modelStack = SceneHistory.back()->modelStack;
		Renderer->viewStack = SceneHistory.back()->viewStack;
		Renderer->projectionStack = SceneHistory.back()->projectionStack;
	}
}

void SceneSystem::RenderTransitionEffects()
{
	if (SceneSystem::Instance().AnimationActivated || SceneSystem::Instance().AnimationDirectionInwards)
		SceneSystem::Instance().TransitionLayer->Render();
}

void SceneSystem::RenderMouseCursor(const Vector3& Dimensions, const std::string MeshName)
{
	RenderSystem *AlternativeRenderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	AlternativeRenderer->SetHUD(true);
	AlternativeRenderer->RenderMeshIn2D(MeshName, false, Dimensions.x, Dimensions.y, cSS_InputManager->GetMousePosition().x, cSS_InputManager->GetMousePosition().y);
	AlternativeRenderer->SetHUD(false);
}

void SceneSystem::AddScene(SceneEntity &SceneObject)
{
#ifdef _DEBUG
	assert(Renderer != nullptr);  //Ensuring that a renderer has already been attached
#endif
	if (SceneHistory.empty() == true)
	{
		SceneHistory.push(&SceneObject);
		Renderer->modelStack = SceneObject.modelStack;
		Renderer->viewStack = SceneObject.viewStack;
		Renderer->projectionStack = SceneObject.projectionStack;
	}
	StoredSceneList.insert(std::pair<std::string, SceneEntity*>(SceneObject.GetEntityID(), &SceneObject));
}

void SceneSystem::SwitchScene(const std::string &id_)
{
#ifdef _DEBUG
	assert(Renderer != nullptr);
#endif
	TargetScene = id_;
	AnimateTransitionLayer(AnimationDirectionInwards = true);
	TransitToTargetedScene();
}

void SceneSystem::TransitToTargetedScene()
{
#ifdef _DEBUG
	assert(Renderer != nullptr);
#endif
	std::map<std::string, SceneEntity*>::iterator it = StoredSceneList.find(TargetScene);
	if (it != StoredSceneList.end())
	{
		PreviousScene = SceneHistory.back();
		SceneHistory.push(it->second);
	}
}

bool SceneSystem::SwitchToPreviousScene()
{
	if (SceneHistory.size() > 1)
	{
		SceneHistory.pop();
		Renderer->modelStack = SceneHistory.back()->modelStack;
		Renderer->viewStack = SceneHistory.back()->viewStack;
		Renderer->projectionStack = SceneHistory.back()->projectionStack;
		return true;
	}
	else return false;
}

SceneEntity &SceneSystem::GetCurrentScene()
{
	return *SceneHistory.back();
}

SceneEntity &SceneSystem::GetPreviousScene()
{
	return *PreviousScene;
}

SceneEntity &SceneSystem::GetRenderSystem()
{
	return *Renderer;
}

void SceneSystem::SetRenderSystem(SceneEntity &SceneObject)
{
#ifdef _DEBUG
	assert(Renderer == nullptr);
#endif
	Renderer = &SceneObject;
}

void SceneSystem::ClearMemoryUsage()
{
	if (TransitionLayer)
	{
		TransitionLayer->Exit();
		delete TransitionLayer;
		TransitionLayer = nullptr;
	}
	for (auto it : StoredSceneList)
	{
		it.second->Exit();
		delete it.second;
	}
	StoredSceneList.clear();

	if (Renderer != nullptr)
	{
		Renderer->Exit();
		delete Renderer;
		Renderer = nullptr;
	}

	while (!SceneHistory.empty())
	{
		SceneHistory.pop();
	}

	if (cSS_InputManager)
	{
		delete cSS_InputManager;
	}
}

void SceneSystem::GenerateTransitionLayer(const int& Divisions, const float& RandomizerScale, const std::string& MeshName)
{
	Vector3 PieceDimensions = Vector3(cSS_InputManager->cIM_ScreenWidth / Divisions, cSS_InputManager->cIM_ScreenHeight / Divisions, 1);
	PieceDimensions *= 1.2f;
	for (unsigned short x = 1; x <= Divisions; ++x)
	{
		for (unsigned short y = 1; y <= Divisions; ++y)
		{
			Vector3 Position = Vector3((x - 1) * PieceDimensions.x, (y - 1) * PieceDimensions.y);
			InterfaceElement* Temp = TransitionLayer->CreateNewInterfaceElement("TransitionPiece", MeshName, Position, PieceDimensions);
			Temp->SetTargetPosition(Position);
		}
	}
	AnimateTransitionLayer(false);
}

bool SceneSystem::AnimateTransitionLayer(const bool& MoveInwards, const float& RandomizerScale)
{
	AnimationDirectionInwards = MoveInwards;
	AnimationActivated = true;
	float MinimumOffset = 0.5f;
	Math::InitRNG();
	for (auto it : TransitionLayer->GetContainer())
	{
		Vector3 Target;
		if (!AnimationDirectionInwards)
		{
			if (Math::RandIntMinMax(0, 1))
				Target.x = Math::RandFloatMinMax(-cSS_InputManager->cIM_ScreenWidth * RandomizerScale, -cSS_InputManager->cIM_ScreenWidth * MinimumOffset);
			else Target.x = Math::RandFloatMinMax(cSS_InputManager->cIM_ScreenWidth * RandomizerScale + cSS_InputManager->cIM_ScreenWidth * MinimumOffset, 2.f * cSS_InputManager->cIM_ScreenWidth * RandomizerScale);

			if (Math::RandIntMinMax(0, 1))
				Target.y = Math::RandFloatMinMax(-cSS_InputManager->cIM_ScreenHeight * RandomizerScale, -cSS_InputManager->cIM_ScreenHeight * MinimumOffset);
			else
				Target.y = Math::RandFloatMinMax(cSS_InputManager->cIM_ScreenHeight * RandomizerScale + cSS_InputManager->cIM_ScreenHeight * MinimumOffset, 2.f * cSS_InputManager->cIM_ScreenHeight * RandomizerScale);
		}
		else Target = it->GetOriginalPosition();
		it->SetTargetPosition(Target);
	}
	return true;
}
