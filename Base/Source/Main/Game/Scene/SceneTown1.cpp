#include "SceneTown1.h"
#include <sstream>

#include "SimpleCommand.h"
#include "../../Engine/System/MusicSystem.h"
#include "../Mains/Application.h"
#include "../SceneManagement/ScenePartitionGraph.h"
#include "../Systems/ObjectManager.h"

//#include "../Miscellaneous/TerrainDataLoader.h"

std::string SceneTown1::id_ = "1_Scene";

SceneTown1::SceneTown1()
	: SceneEntity()
{
	framerates = 0;
	SetEntityID(id_);
	InteractiveMap = nullptr;
	Player = nullptr;
	SceneSystem::Instance().AddScene(*this);
	Init();
}

SceneTown1::~SceneTown1()
{

}

void SceneTown1::QuickInit()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	// Set Terrain Size
	TerrainScale.Set(400.f, 25.f, 400.f);
	ScenePartitionGraph::Instance().AssignGridParameters(Vector3(), Vector3(TerrainScale.x, TerrainScale.z), 4);

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth / SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0.1f, 10000.0f);
	projectionStack->LoadMatrix(perspective);

	CameraAerial* CA = new CameraAerial();
	camera = CA;
	CA->AltInit(/*Player Character Position*/Vector3(0, 0, 0), Vector3(0, 300, 1), Vector3(0, 1, 0));
	CenterPosition.Set(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);

	// Initiallise Model Specific Meshes Here
	Mesh* newMesh = MeshBuilder::GenerateTerrain("Town 1", "HeightMapFiles//heightmap_Town1.raw", m_heightMap);
	newMesh->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	newMesh->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	newMesh->material.kSpecular.Set(0.0f, 0.0f, 0.0f);
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	newMesh->textureArray[1] = LoadTGA("Image//GrassStoneTex.tga");
	Renderer->MeshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	InteractiveMap = new GameMap();
	GameMap *theMap = dynamic_cast<GameMap*>(InteractiveMap);
	theMap->SetEntityID("scene town 1 logic map");
	theMap->LoadMap("CSVFiles//Town1Layout.csv", m_heightMap, TerrainScale, EntityList, BManager);
}

void SceneTown1::QuickExit()
{
	ScenePartitionGraph::Instance().Exit();
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
}
void SceneTown1::Init()
{
	RouteGeneration RG;
	QuickInit();
	for (int i = 0; i < 100; ++i)
	{
		RG.GeneratePath();
		PathList.push_back(RG.DefinedPath);
		//std::cout << i << ": " << RG.DefinedPath.size() << std::endl;
	}


	//terrain test
		/*TerrainDataLoader* test = new TerrainDataLoader();
		test->LoadTerrainData("CSVFiles/TerrainDataLoader.csv");
		Terrain* test2 = test->GetTerrain("MeleePanel");
		test2->GetTerrainName();
		float hpboost = test2->GetUnitAdvantage(Terrain::T_HEALTH);
		float meleeboost = test2->GetUnitAdvantage(Terrain::T_MELEE);
		float magicboost = test2->GetUnitAdvantage(Terrain::T_MAGIC);
		float rangeboost = test2->GetUnitAdvantage(Terrain::T_RANGE);
		float hpnerf = test2->GetUnitDisadvantage(Terrain::T_HEALTH);
		float meleenerf = test2->GetUnitDisadvantage(Terrain::T_MELEE);
		float magicnerf = test2->GetUnitDisadvantage(Terrain::T_MAGIC);
		float rangenerf = test2->GetUnitDisadvantage(Terrain::T_RANGE);*/


}

void SceneTown1::Update(const float& dt)
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
		ScenePartitionGraph::Instance().ShowPartitions = false;
	}

	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		ScenePartitionGraph::Instance().ShowPartitions = true;
	}

	for (auto it : ObjectManager::Instance().GetParticleList())
		it->Update(dt);

	CA->Update(dt);
	//MusicSystem::Instance().playBackgroundMusic("battle");
	BManager.UpdateContainer(dt, CA->position);
	timer += dt;
	if (timer > .5f)
	{
		timer = 0;

		/*enemyMoveQueue.push_back(test->getQueue());
		testEnemyPos.push_back(ScenePartitionGraph::Instance().EnemyBase->GetEntity()->GetPosition());
		currEnemyTarget.push_back(enemyMoveQueue.front().front());*/
		Enemy* E = new Enemy();
		int Selection = Math::RandIntMinMax(0, PathList.size()-1);
		E->targetNode = PathList[Selection];
		E->currPos = E->targetNode.front()->GetEntity()->GetPosition();
		enemyList.push_back(E);

	}
	ScenePartitionGraph::Instance().Update(dt);
	for (int i = 0; i < enemyList.size(); ++i)
	{
		Vector3 enemyDir = enemyList[i]->currPos - (enemyList[i]->targetNode.front()->GetEntity()->GetPosition());

		if ((enemyList[i]->currPos - (enemyList[i]->targetNode.front()->GetEntity()->GetPosition())).LengthSquared() < 10)
		{
			if (enemyList[i]->targetNode.size() > 1)
			{
				enemyList[i]->targetNode.erase(enemyList[i]->targetNode.begin());
			}
			else enemyList[i]->targetNode.erase(enemyList[i]->targetNode.begin());
		}
		else
		{
			
			enemyList[i]->currPos -= enemyDir.Normalized() *2 * Speed * dt;
		}
	}

	for (std::vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); )
	{
		if ((*it)->targetNode.size() == 0)
		{
			delete *it;
			it = enemyList.erase(it);
		}
		else ++it;
	}
	
	framerates = 1 / dt;
}

void SceneTown1::RenderTerrain()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	modelStack->PushMatrix();
	modelStack->Scale(TerrainScale.x, TerrainScale.y, TerrainScale.z);
	Renderer->RenderMesh("Town 1", true);
	modelStack->PopMatrix();
}

void SceneTown1::RenderShadowCasters()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//RenderTerrain();
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	ScenePartitionGraph::Instance().Render();

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

void SceneTown1::RenderSkybox()
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

void SceneTown1::RenderPassGPass()
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

void SceneTown1::RenderPassMain()
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

	RenderTerrain();
	RenderSkybox();
	RenderShadowCasters();
	for (int i = 0; i < enemyList.size(); ++i)
	{
		modelStack->PushMatrix();
		modelStack->Translate(enemyList[i]->currPos.x, 5, enemyList[i]->currPos.z);
		Renderer->RenderMesh("cube", false);
		modelStack->PopMatrix();
	}

	for (auto it : ObjectManager::Instance().GetParticleList())
		it->Render();
	
	Renderer->SetHUD(true);
	std::stringstream ss;
	ss.str("");
	ss << "FPS: " << framerates;
	Renderer->RenderTextOnScreen(ss.str(), Color(), SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth* 0.02f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth* 0.01f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.1f);

	Renderer->SetHUD(false);
}

void SceneTown1::Render()
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

void SceneTown1::Exit()
{
	for (std::vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end();)
	{	
		delete *it;
		it = enemyList.erase(it);
	}
	QuickExit();
}