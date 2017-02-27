#include "MainMenuScene.h"
#include <sstream>

#include "SimpleCommand.h"
#include "../../Engine/System/MusicSystem.h"
#include "../Mains/Application.h"
#include "../SceneManagement/ScenePartitionGraph.h"

std::string MainMenuScene::id_ = "MainMenuScene";

MainMenuScene::MainMenuScene()
	: SceneEntity()
{
	framerates = 0;
	SetEntityID(id_);
	InteractiveMap = nullptr;
	Player = nullptr;
	SceneSystem::Instance().AddScene(*this);
	Init();

	MenuInterface = new MainMenuInterface();
}

MainMenuScene::~MainMenuScene()
{

}

void MainMenuScene::QuickInit()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	// Set Terrain Size
	TerrainScale.Set(400.f, 25.f, 400.f);
	ScenePartition = new ScenePartitionGraph();
	ScenePartition->AssignGridParameters(Vector3(), Vector3(TerrainScale.x, TerrainScale.z), 4);

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth / SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0.1f, 10000.0f);
	projectionStack->LoadMatrix(perspective);

	CameraAerial* CA = new CameraAerial();
	camera = CA;
	CA->AltInit(/*Player Character Position*/Vector3(0, 0, 0), Vector3(0, 100, 1), Vector3(0, 1, 0));
	CenterPosition.Set(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);

	// Initiallise Model Specific Meshes Here
	Mesh* newMesh = MeshBuilder::GenerateTerrain("MainMenu", "HeightMapFiles//heightmap_Town1.raw", m_heightMap);
	newMesh->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	newMesh->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	newMesh->material.kSpecular.Set(0.0f, 0.0f, 0.0f);
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	newMesh->textureArray[1] = LoadTGA("Image//GrassStoneTex.tga");
	Renderer->MeshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	InteractiveMap = new GameMap();
	GameMap *theMap = dynamic_cast<GameMap*>(InteractiveMap);
	theMap->ScenePartition = ScenePartition;
	theMap->SetEntityID("MainMenu");
	theMap->LoadMap("CSVFiles//BattlefieldLayout1.csv", false, m_heightMap, TerrainScale, EntityList, BManager);

	SceneSystem::Instance().cSS_InputManager->cIM_inMouseMode = true;
}

void MainMenuScene::QuickExit()
{
	if (ScenePartition)
	{
		ScenePartition->Exit();
		delete ScenePartition;
	}
	if (InteractiveMap)
		delete InteractiveMap;
	for (auto it : EntityList)
	{
		delete it;
	}
	EntityList.clear();
	if (Player)
		delete Player;
	if (camera)
		delete camera;

	if (MenuInterface)
	{
		MenuInterface->Exit();
		delete MenuInterface;
		MenuInterface = nullptr;
	}
}
void MainMenuScene::Init()
{
	QuickInit();
}

void MainMenuScene::Update(const float& dt)
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	Renderer->Update(dt);

	float Speed = 50.f;
	CameraAerial* CA = (CameraAerial*)camera;

	if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::FORWARD_COMMAND]))
	{
		CA->CameraMoveTargetPosition.z -= Speed*dt;
	}
	if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND]))
	{
		CA->CameraMoveTargetPosition.z += Speed*dt;
	}
	if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::RIGHT_COMMAND]))
	{
		CA->CameraMoveTargetPosition.x += Speed*dt;
	}
	if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::LEFT_COMMAND]))
	{
		CA->CameraMoveTargetPosition.x -= Speed*dt;
	}

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		ScenePartition->ShowPartitions = false;
	}

	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		ScenePartition->ShowPartitions = true;
	}

	CA->Update(dt);
	//MusicSystem::Instance().playBackgroundMusic("battle");
	BManager.UpdateContainer(dt, CA->position);

	ScenePartition->Update(dt);

	MenuInterface->Update(dt);

	framerates = 1 / dt;
}



void MainMenuScene::RenderTerrain()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	modelStack->PushMatrix();
	modelStack->Scale(TerrainScale.x, TerrainScale.y, TerrainScale.z);
	Renderer->RenderMesh("MainMenu", true);
	modelStack->PopMatrix();
}

void MainMenuScene::RenderShadowCasters()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//RenderTerrain();
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	ScenePartition->Render();


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

void MainMenuScene::RenderSkybox()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	//left
	modelStack->PushMatrix();
	modelStack->Rotate(90, 0, 1, 0);
	modelStack->PushMatrix();
	modelStack->Rotate(90, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Left", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(-90, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Right", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Front", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(180, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Back", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(90, 1, 0, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Rotate(90, 0, 0, 1);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Top", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(-90, 1, 0, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	//modelStack->Rotate(-90, 0, 0, 1);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Bottom", false);
	modelStack->PopMatrix();
	modelStack->PopMatrix();
}

void MainMenuScene::RenderPassGPass()
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

void MainMenuScene::RenderPassMain()
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
	perspective.SetToPerspective(60.0f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth / SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0.1f, 1000.0f);
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

	Renderer->RenderMesh("reference", false);

	MenuInterface->Render();

	RenderTerrain();
	RenderSkybox();
	RenderShadowCasters();

	Renderer->SetHUD(true);
	std::stringstream ss;
	ss.str("");
	ss << "FPS: " << framerates;
	Renderer->RenderTextOnScreen(ss.str(), Color(), SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth* 0.02f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth* 0.01f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.1f);

	Renderer->SetHUD(false);
}

void MainMenuScene::Render()
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

void MainMenuScene::Exit()
{
	QuickExit();
}