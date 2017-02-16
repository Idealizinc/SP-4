#include "SceneSystem.h"
#ifdef _DEBUG
#include <assert.h>
#endif

void SceneSystem::Init()
{
	Renderer = nullptr;
	cSS_InputManager = new InputManager();
}

void SceneSystem::Update(const float& dt)
{
#ifdef _DEBUG
	assert(dt > 0 || dt < 0 || dt == 0);
#endif
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
	std::map<std::string, SceneEntity*>::iterator it = StoredSceneList.find(id_);
	if (it != StoredSceneList.end())
	{
		SceneHistory.push(it->second);
		Renderer->modelStack = SceneHistory.back()->modelStack;
		Renderer->viewStack = SceneHistory.back()->viewStack;
		Renderer->projectionStack = SceneHistory.back()->projectionStack;
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
}
