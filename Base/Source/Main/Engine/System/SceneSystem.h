/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 30 September 2016 10:42 AM
File Name     : SceneSystem.h
Purpose       : Defines a SceneSystem
*/

#ifndef SCENE_SYSTEM_H
#define SCENE_SYSTEM_H

#include "../Internal/System.h"
#include "../Internal/SingletonTemplate.h"
#include "../Objects/SceneEntity.h"
#include <map>
#include <queue>
#include "Utility.h"
#include "InputManager.h"
#include "../Interface/InterfaceLayer.h"

class SceneSystem : public System, public SingletonTemplate<SceneSystem>
{
public:
    virtual void Init();    
	virtual void Update(const float& dt);
	virtual void Render(void){};
	void RenderTransitionEffects();
	void RenderMouseCursor(const Vector3& Dimensions, const std::string MeshName = "quad");
	//virtual void SendMessage(const string&){};
	virtual void Exit(void){};

    virtual void AddScene(SceneEntity&);

	virtual void SwitchScene(const std::string&);
	virtual bool SwitchToPreviousScene();

	SceneEntity &GetCurrentScene();
	SceneEntity &GetPreviousScene();
    SceneEntity &GetRenderSystem();

	virtual void SetRenderSystem(SceneEntity&);
    virtual void ClearMemoryUsage();

	InputManager* cSS_InputManager;
	void GenerateTransitionLayer(const int& Divisions, const float& RandomizerScale = 1.f, const std::string& MeshName = "quad");

	InterfaceLayer* TransitionLayer;
	bool AnimationActivated;
	bool AnimationDirectionInwards;
	SceneEntity* PreviousScene;

protected:
	std::string TargetScene;
	int MaxStoredHistory;
	std::queue<SceneEntity*> SceneHistory;
    std::map<std::string, SceneEntity*> StoredSceneList;
    SceneEntity *Renderer;
	const float TransitionSpeed = 2.f;
	const float MinimumAcceptableDistanceSquared = 1.f;

	bool AnimateTransitionLayer(const bool& MoveInwards = false, const float& RandomizerScale = 1.f);
	void TransitToTargetedScene();
};

#endif