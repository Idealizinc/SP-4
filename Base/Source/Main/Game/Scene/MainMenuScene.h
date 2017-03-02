/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 20 February 2016 13:13 PM
File Name     : MainMenuScene.h
Purpose       : MainMenu Scene
*/

#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "../../Engine/Objects/SceneEntity.h"
#include "../../Engine/System/SceneSystem.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/Objects/GameObject.h"
#include "../Systems/BillboardManager.h"
#include "../Objects/GameMap.h"
#include "../Miscellaneous/CameraAerial.h"
#include "../UserInterface/MainMenuInterface.h"

class MainMenuScene : public SceneEntity
{
public:
	static std::string id_;
	MainMenuScene();
	virtual ~MainMenuScene();

	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render();
	virtual void Exit();

	MainMenuInterface* MenuInterface;
private:
	// Shadow GPass
	void RenderPassGPass();
	void RenderPassMain();

	void QuickInit();
	void QuickExit();

	// Additional Calls
	void RenderShadowCasters();
	void RenderTerrain();
	void RenderSkybox();

	const float SkyboxSize = 1500;

	BillboardManager BManager;
	BillboardManager ParticleManager;
	GameObject* Player;

	GameMap *InteractiveMap;
	std::vector<GameObject*> EntityList;
	Vector3 CenterPosition;

	




};

#endif