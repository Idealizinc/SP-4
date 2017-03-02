#include "GameScene.h"
#include <sstream>
#include <iomanip>

#include "SimpleCommand.h"
#include "../../Engine/System/MusicSystem.h"
#include "../Mains/Application.h"
#include "../SceneManagement/ScenePartitionGraph.h"
#include "../Systems/GameLogicSystem.h"

std::string GameScene::id_ = "1_Scene";

GameScene::GameScene()
	: SceneEntity()
{
	framerates = 0;
	SetEntityID(id_);
	InteractiveMap = nullptr;
	Player = nullptr;
	SceneSystem::Instance().AddScene(*this);
	Init();
}

GameScene::~GameScene()
{

}

void GameScene::QuickInit()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	// Set Terrain Size
	TerrainScale.Set(500.f, 20.f, 500.f);
	ScenePartition = new ScenePartitionGraph();
	ScenePartition->AssignGridParameters(0, Vector3(TerrainScale.x, TerrainScale.z), 8);

	CameraAerial* CA = new CameraAerial();
	
	camera = CA;
	CA->AltInit(/*Player Character Position*/Vector3(0, 0, 0), Vector3(0, 150, 0.01f), Vector3(0, 1, 0));
	CenterPosition.Set(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);

	// Initiallise Model Specific Meshes Here
	Mesh* newMesh = MeshBuilder::GenerateTerrain("Town 1", "HeightMapFiles//heightmap_test.raw", m_heightMap);
	newMesh->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	newMesh->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	newMesh->material.kSpecular.Set(0.0f, 0.0f, 0.0f);
	newMesh->textureArray[0] = LoadTGA("Image//Map//RockTex.tga");
	newMesh->textureArray[1] = LoadTGA("Image//Map//GrassStoneTex.tga");
	Renderer->MeshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));
		
	GameLogicSystem::Instance().TerrainLoader.LoadTerrainData("CSVFiles/TerrainDataLoader.csv");
	InteractiveMap = new GameMap();
	InteractiveMap->ScenePartition = ScenePartition;
	InteractiveMap->SetEntityID("SceneMap");
	//InteractiveMap->LoadMap("CSVFiles//Town1Layout.csv", true, m_heightMap, TerrainScale, EntityList, BManager);
	InteractiveMap->LoadMap("CSVFiles//LevelDesign3.csv", true, m_heightMap, TerrainScale, EntityList, BManager);
	GameLogicSystem::Instance().Init();

	SceneSystem::Instance().cSS_InputManager->cIM_inMouseMode = true;
}

void GameScene::QuickExit()
{
	if (ScenePartition)
	{
		ScenePartition->Exit();
		delete ScenePartition;
	}

	if (InteractiveMap)
		delete InteractiveMap;
	EntityList.clear();
	if (Player)
		delete Player;
	if (camera)
		delete camera;
}

void GameScene::Init()
{
	QuickInit();
}

void GameScene::Update(const float& dt)
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	Renderer->Update(dt);

	//GSI->Update(dt);

	float Speed = 80.f;
	CameraAerial* CA = (CameraAerial*)camera;
	float DetectionOffset = 0.1f;
	if (!GameLogicSystem::Instance().UnitInterface->UIDisplayed && !GameLogicSystem::Instance().GameInterface->MenuOpen)
	{
		if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::FORWARD_COMMAND]) ||
			SceneSystem::Instance().cSS_InputManager->GetMousePosition().y > SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (1.f - DetectionOffset))
		{
			CA->CameraMoveTargetPosition.z -= Speed*dt;
		}
		if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND]) ||
			SceneSystem::Instance().cSS_InputManager->GetMousePosition().y < SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * DetectionOffset)
		{
			CA->CameraMoveTargetPosition.z += Speed*dt;
		}
		if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::RIGHT_COMMAND]) ||
			SceneSystem::Instance().cSS_InputManager->GetMousePosition().x > SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * (1.f - DetectionOffset))
		{
			CA->CameraMoveTargetPosition.x += Speed*dt;
		}
		if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::LEFT_COMMAND]) ||
			SceneSystem::Instance().cSS_InputManager->GetMousePosition().x < SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * DetectionOffset)
		{
			CA->CameraMoveTargetPosition.x -= Speed*dt;
		}
		float XOffset = TerrainScale.x * 0.5f;
		if (CA->CameraMoveTargetPosition.x < -XOffset)
			CA->CameraMoveTargetPosition.x = -XOffset;
		else if (CA->CameraMoveTargetPosition.x > XOffset)
			CA->CameraMoveTargetPosition.x = XOffset;

		float ZOffset = TerrainScale.z * 0.5f;
		if (CA->CameraMoveTargetPosition.z < -ZOffset)
			CA->CameraMoveTargetPosition.z = -ZOffset;
		else if (CA->CameraMoveTargetPosition.z > ZOffset)
			CA->CameraMoveTargetPosition.z = ZOffset;
	}

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		ScenePartition->ShowPartitions = false;
	}

	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		ScenePartition->ShowPartitions = true;
	}

	if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_RMB) == InputManager::MOUSE_HOLD)
	{
		CA->CameraMoveTargetPosition.y = 100.f;
	}
	else CA->CameraMoveTargetPosition.y = 0;

	CA->Update(dt);
	//MusicSystem::Instance().playBackgroundMusic("battle");
	BManager.UpdateContainer(dt, CA->position);
	
	ScenePartition->Update(dt);
	GameLogicSystem::Instance().Update(dt);
	
	framerates = 1 / dt;
}

void GameScene::RenderTerrain()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	modelStack->PushMatrix();
	modelStack->Scale(TerrainScale.x, TerrainScale.y, TerrainScale.z);
	Renderer->RenderMesh("Town 1", true);
	modelStack->PopMatrix();
}

void GameScene::RenderShadowCasters()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//RenderTerrain();
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	RenderTerrain();
	ScenePartition->Render();

	GameLogicSystem::Instance().Render();

	for (std::vector<Particle*>::iterator it = BManager.BillboardContainer.begin(); it != BManager.BillboardContainer.end(); ++it)
	{
		if ((*it)->Active)
		{
			float TimeRatio = 1;
			modelStack->PushMatrix();
			modelStack->Translate((*it)->GetPosition().x, (*it)->GetPosition().y, (*it)->GetPosition().z);
			modelStack->Rotate(Math::RadianToDegree(atan2(camera->position.x - (*it)->GetPosition().x, camera->position.z - (*it)->GetPosition().z)), 0, 1, 0);
			modelStack->Scale(TimeRatio * (*it)->GetDimensions().x, TimeRatio *(*it)->GetDimensions().y, TimeRatio *(*it)->GetDimensions().z);
			Renderer->RenderMesh((*it)->GetMeshName(), false);
			modelStack->PopMatrix();
		}
	}
}

void GameScene::RenderSkybox()
{
	float Offset = SkyboxSize *0.001f * 2.f;
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	//left
	modelStack->PushMatrix();
	modelStack->Translate(camera->position.x, camera->position.y, camera->position.z);
	modelStack->Rotate(90, 0, 1, 0);
	modelStack->PushMatrix();
	modelStack->Rotate(90, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize *0.5f + Offset);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Left", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(-90, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize *0.5f + Offset);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Right", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0, 0, -SkyboxSize *0.5f + Offset);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Front", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(180, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize*0.5f + Offset);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Back", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(90, 1, 0, 0);
	modelStack->Translate(0, 0, -SkyboxSize *0.5f + Offset);
	modelStack->Rotate(90, 0, 0, 1);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Top", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(-90, 1, 0, 0);
	modelStack->Translate(0, 0, -SkyboxSize *0.5f + Offset);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Bottom", false);
	modelStack->PopMatrix();
	modelStack->PopMatrix();
}

void GameScene::RenderPassGPass()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	Renderer->m_renderPass = RenderSystem::RENDER_PASS::RENDER_PASS_PRE;
	Renderer->m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(Renderer->m_gPassShaderID);
	//These matrices should change when light position or direction changes
	if (Renderer->lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		// based on scene size [below]
		Renderer->m_lightDepthProj.SetToOrtho(-1200, 1200, -1200, 1200, 0, 4000);
	}
	else
	{
		Renderer->m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
	}
	Renderer->m_lightDepthView.SetToLookAt(Renderer->lights[0].position.x, Renderer->lights[0].position.y, Renderer->lights[0].position.z, 0, 0, 0, 0, 1, 0);
	// Things that the light sees and creates shadows from.
	RenderShadowCasters();
}

void GameScene::RenderPassMain()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	Renderer->m_renderPass = RenderSystem::RENDER_PASS::RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, (GLsizei)SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth, (GLsizei)SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(Renderer->m_programID);
	//pass light depth texture
	Renderer->m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(Renderer->m_parameters[Renderer->U_SHADOW_MAP], 8);

	if (Renderer->lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(Renderer->lights[0].position.x, Renderer->lights[0].position.y, Renderer->lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack->Top() * lightDir;
		glUniform3fv(Renderer->m_parameters[Renderer->U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (Renderer->lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack->Top() * Renderer->lights[0].position;
		glUniform3fv(Renderer->m_parameters[Renderer->U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack->Top() * Renderer->lights[0].spotDirection;
		glUniform3fv(Renderer->m_parameters[Renderer->U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack->Top() * Renderer->lights[0].position;
		glUniform3fv(Renderer->m_parameters[Renderer->U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	Renderer->SetHUD(false);

	Mtx44 perspective;
	CameraAerial* CA = (CameraAerial*)SceneSystem::Instance().GetCurrentScene().camera;
	perspective.SetToPerspective(CA->FieldOfView, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth / SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0.1f, 2000.0f);
	projectionStack->LoadMatrix(perspective);

	// Camera matrix
	viewStack->LoadIdentity();
	viewStack->LookAt(
		camera->position.x, camera->position.y, camera->position.z,
		camera->target.x, camera->target.y, camera->target.z,
		camera->up.x, camera->up.y, camera->up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack->LoadIdentity();

	RenderSkybox();
	RenderShadowCasters();

	Renderer->SetHUD(true);
	std::stringstream ss;
	ss.str("");
	ss << "FPS: " << framerates;
	Renderer->RenderTextOnScreen(ss.str(), Color(), SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth* 0.02f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth* 0.01f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.1f);
	Renderer->SetHUD(false);
}

void GameScene::Render()
{
	//*********************************
	//		PRE RENDER PASS
	//*********************************
	RenderPassGPass();
	//*********************************
	//		MAIN RENDER PASS
	//*********************************
	RenderPassMain();
}

void GameScene::Exit()
{
	QuickExit();
}