#include "Game\Game.hpp"
#include "Game\GameStates\PlayingState.hpp"
#include "Game\GameObjects\GameMeshes.hpp"
#include "Engine\Window\Window.hpp"
#include "Engine\Debug\DebugRender.hpp"
#include "Engine\Core\LightObject.hpp"
#include "Engine\Debug\DebugRender.hpp"
#include "Engine\Renderer\RenderScene.hpp"
#include "Engine\Renderer\RenderScene2D.hpp"
#include "Engine\Core\StringUtils.hpp"
#include <map>
#include <string>

//  =========================================================================================
World::World()
{

}

//  =========================================================================================
World::~World()
{
	//these will get deleted from game later becuase it's used in other gamestates
	m_engineCamera = nullptr;
	m_uiCamera = nullptr;

	delete(m_gameCamera);
	m_gameCamera = nullptr;
}

//  =========================================================================================
void World::Initialize()
{
	//initialize game camera
	Window* theWindow = Window::GetInstance();
	Renderer* theRenderer = Renderer::GetInstance();

	m_engineCamera = Game::GetInstance()->m_engineCamera;
	m_uiCamera = Game::GetInstance()->m_uiCamera;

	m_gameCamera = new GameCamera();
	m_gameCamera->Translate(-g_worldForward * 10.f);

	//m_camera->m_skybox = new Skybox("Data/Images/galaxy2.png");
	theRenderer->SetAmbientLightIntensity(0.15f);
}

//  =========================================================================================
void World::Update(float deltaSeconds)
{
	UpdateFromInput(deltaSeconds);
}

//  =========================================================================================
void World::Render()
{
	Renderer* theRenderer = Renderer::GetInstance();
	Game* theGame = Game::GetInstance();

	//always do this first at the beginning of the frame's render
	theRenderer->SetCamera(m_engineCamera);	
	theRenderer->ClearDepth(1.f);
	theRenderer->ClearColor(Rgba::BLACK);

	//hammer over camera view matrix
	m_gameCamera->CreateFliippedViewMatrix(m_engineCamera->m_viewMatrix);

	MeshBuilder builder;

	builder.CreateBasis(Matrix44::IDENTITY, Vector3::ZERO, 1.f);
	Mesh* axisMesh = builder.CreateMesh<VertexPCU>();

	//draw axis for debugging
	theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));
	theRenderer->DrawMesh(axisMesh);

	//make a blockmesh
	//Mesh* blockMesh = MakeBlockMesh(builder, Vector3(2.5f, 0.f, 0.f));
	//theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("Data/Images/Cards/Chillwind Yeti.png"));

	//theRenderer->DrawMesh(blockMesh);

	////cleanup..for now
	//delete(blockMesh);
	//blockMesh = nullptr;
}

//  =========================================================================================
void World::UpdateFromInput(float deltaSeconds)
{
	InputSystem* theInput = InputSystem::GetInstance();

	Vector2 mouseDelta = Vector2::ZERO;
	mouseDelta = InputSystem::GetInstance()->GetMouse()->GetMouseDelta();

	//calculate rotation for camera and use same rotation for player
	//m_camera->m_transform->AddRotation(Vector3(mouseDelta.y, mouseDelta.x, 0.f) * deltaSeconds * 10.f);

	m_gameCamera->m_yawDegreesZ += -mouseDelta.x * 0.05f;
	m_gameCamera->m_pitchDegreesY += mouseDelta.y * 0.05f;

	m_gameCamera->m_yawDegreesZ = Modulus(m_gameCamera->m_yawDegreesZ, 360.f);
	m_gameCamera->m_pitchDegreesY = ClampFloat(m_gameCamera->m_pitchDegreesY,-90.f, 90.f);

	/*Vector3 rotation = Vector3(clampedX, clampedY, 0.f);
	m_camera->m_transform->SetLocalRotation(Vector3(rotation));*/

	Vector3 cameraForward = Vector3(CosDegrees(m_gameCamera->m_yawDegreesZ), SinDegrees(m_gameCamera->m_yawDegreesZ), 0);
	Vector3 cameraRight = Vector3(SinDegrees(m_gameCamera->m_yawDegreesZ), -CosDegrees(m_gameCamera->m_yawDegreesZ), 0);

	Vector3 positionToAdd = Vector3::ZERO;
	Vector3 positionAtStartOfFrame = positionToAdd;

	//update movement
	//forward (x)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_W))
	{
		//calculate movement for camera and use same movement for ship and light
		positionToAdd = cameraForward * deltaSeconds * 10.f;
		m_gameCamera->Translate(positionToAdd);
	}

	//backward (-x)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_S))
	{
		positionToAdd = -cameraForward * deltaSeconds * 10.f;
		m_gameCamera->Translate(positionToAdd);
	}

	//left is north (y)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_A))
	{
		positionToAdd = -cameraRight * deltaSeconds * 10.f;
		m_gameCamera->Translate(positionToAdd);
	}

	//right is south (-y)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_D))
	{
		positionToAdd = cameraRight * deltaSeconds * 10.f;
		m_gameCamera->Translate(positionToAdd);
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_SPACE))
	{
		positionToAdd = g_worldUp * deltaSeconds * 10.f;
		m_gameCamera->Translate(positionToAdd);
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_CONTROL))
	{
		positionToAdd = -g_worldUp * deltaSeconds * 10.f;
		m_gameCamera->Translate(positionToAdd);
	}


	//deltaSeconds = m_player->UpdateFromInput(deltaSeconds);
	//deltaSeconds = UpdateFromInputDebug(deltaSeconds);

	// return 
	//return deltaSeconds; //new deltaSeconds
}

//  =========================================================================================
void World::UpdateChunks()
{
	std::map<IntVector2, Chunk*>::iterator chunkIterator;

	for (chunkIterator = m_activeChunks.begin(); chunkIterator != m_activeChunks.end(); ++chunkIterator)
	{
		chunkIterator->second->Update();
	}
}