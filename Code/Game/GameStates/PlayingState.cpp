#include "Game\Game.hpp"
#include "Game\GameStates\PlayingState.hpp"
#include "Game\GameMeshes.hpp"
#include "Engine\Window\Window.hpp"
#include "Engine\Debug\DebugRender.hpp"
#include "Engine\Core\LightObject.hpp"
#include "Engine\Renderer\MeshBuilder.hpp"
#include "Engine\Debug\DebugRender.hpp"
#include "Engine\Renderer\RenderScene.hpp"
#include "Engine\Renderer\RenderScene2D.hpp"
#include "Engine\Core\StringUtils.hpp"
#include <map>
#include <string>

//  =============================================================================
PlayingState::~PlayingState()
{
	//will be deleted in game destructor
	m_uiCamera = nullptr;

	delete(m_renderScene);
	m_renderScene = nullptr;

	delete(m_renderScene2D);
	m_renderScene2D = nullptr;
}

//  =============================================================================
void PlayingState::Initialize()
{
	//initialize game camera
	Window* theWindow = Window::GetInstance();
	Renderer* theRenderer = Renderer::GetInstance();

	m_camera = Game::GetInstance()->m_gameCamera;
	m_uiCamera = Game::GetInstance()->m_uiCamera;

	m_michaelCraftCamera = new GameCamera();
	m_michaelCraftCamera->Translate(-g_worldForward * 10.f);

	//m_camera->m_skybox = new Skybox("Data/Images/galaxy2.png");
	theRenderer->SetAmbientLightIntensity(0.15f);
	
	//update building
	MeshBuilder builder;

	builder.CreateBasis(Matrix44::IDENTITY, Vector3::ZERO, 1.f);
	m_axisMesh = builder.CreateMesh<VertexPCU>();

	m_isInitialized = true;
}

//  =============================================================================
void PlayingState::Update(float deltaSeconds)
{ 
	deltaSeconds = UpdateFromInput(deltaSeconds);
}

//  =============================================================================
void PlayingState::PreRender()
{

}

//  =============================================================================
void PlayingState::Render()
{
	Renderer* theRenderer = Renderer::GetInstance();
	Game* theGame = Game::GetInstance();

	//always do this first at the beginning of the frame's render
	theRenderer->SetCamera(m_camera);	
	theRenderer->ClearDepth(1.f);
	theRenderer->ClearColor(Rgba::BLACK);

	//hammer over camera view matrix
	m_michaelCraftCamera->CreateFliippedViewMatrix(m_camera->m_viewMatrix);

	//draw axis for debugging
	theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));
	theRenderer->DrawMesh(m_axisMesh);

	Mesh* blockMesh = MakeBlock(Vector3(2.5f, 0.f, 0.f));
	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("Data/Images/Cards/Chillwind Yeti.png"));

	theRenderer->DrawMesh(blockMesh);
}

//  =============================================================================
void PlayingState::PostRender()
{
	//post render steps here.
}

//  =============================================================================
float PlayingState::UpdateFromInput(float deltaSeconds)
{
	InputSystem* theInput = InputSystem::GetInstance();

	Vector2 mouseDelta = Vector2::ZERO;
	mouseDelta = InputSystem::GetInstance()->GetMouse()->GetMouseDelta();

	//calculate rotation for camera and use same rotation for player
	//m_camera->m_transform->AddRotation(Vector3(mouseDelta.y, mouseDelta.x, 0.f) * deltaSeconds * 10.f);

	m_michaelCraftCamera->m_yawDegreesZ += -mouseDelta.x * 0.05f;
	m_michaelCraftCamera->m_pitchDegreesY += mouseDelta.y * 0.05f;

	m_michaelCraftCamera->m_yawDegreesZ = Modulus(m_michaelCraftCamera->m_yawDegreesZ, 360.f);
	m_michaelCraftCamera->m_pitchDegreesY = ClampFloat(m_michaelCraftCamera->m_pitchDegreesY,-90.f, 90.f);

	/*Vector3 rotation = Vector3(clampedX, clampedY, 0.f);
	m_camera->m_transform->SetLocalRotation(Vector3(rotation));*/

	Vector3 cameraForward = Vector3(CosDegrees(m_michaelCraftCamera->m_yawDegreesZ), SinDegrees(m_michaelCraftCamera->m_yawDegreesZ), 0);
	Vector3 cameraRight = Vector3(SinDegrees(m_michaelCraftCamera->m_yawDegreesZ), -CosDegrees(m_michaelCraftCamera->m_yawDegreesZ), 0);

	Vector3 positionToAdd = Vector3::ZERO;
	Vector3 positionAtStartOfFrame = positionToAdd;

	//update movement
	//forward (x)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_W))
	{
		//calculate movement for camera and use same movement for ship and light
		positionToAdd = cameraForward * deltaSeconds * 10.f;
		m_michaelCraftCamera->Translate(positionToAdd);
	}

	//backward (-x)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_S))
	{
		positionToAdd = -cameraForward * deltaSeconds * 10.f;
		m_michaelCraftCamera->Translate(positionToAdd);
	}

	//left is north (y)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_A))
	{
		positionToAdd = -cameraRight * deltaSeconds * 10.f;
		m_michaelCraftCamera->Translate(positionToAdd);
	}

	//right is south (-y)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_D))
	{
		positionToAdd = cameraRight * deltaSeconds * 10.f;
		m_michaelCraftCamera->Translate(positionToAdd);
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_SPACE))
	{
		positionToAdd = g_worldUp * deltaSeconds * 10.f;
		m_michaelCraftCamera->Translate(positionToAdd);
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_CONTROL))
	{
		positionToAdd = -g_worldUp * deltaSeconds * 10.f;
		m_michaelCraftCamera->Translate(positionToAdd);
	}


	//deltaSeconds = m_player->UpdateFromInput(deltaSeconds);
	deltaSeconds = UpdateFromInputDebug(deltaSeconds);

	// return 
	return deltaSeconds; //new deltaSeconds
}

//  =========================================================================================
void PlayingState::InitializeAxisObject()
{
	MeshBuilder builder;

	//put matrix here
	builder.CreateBasis(Matrix44::IDENTITY, Vector3::ZERO, 1.f);
	m_axisMesh = builder.CreateMesh<VertexPCU>();
}

//  =========================================================================================
float PlayingState::UpdateFromInputDebug(float deltaSeconds)
{
	//update debug input
	/*if(theInput->WasKeyJustPressed((theInput->KEYBOARD_F9)))
	{
		theRenderer->ReloadShaders();
	}

	if(theInput->WasKeyJustPressed((theInput->KEYBOARD_V)))
	{
		isDebugText = !isDebugText;
	}*/

	////debug hotkeys
	//if(theInput->WasKeyJustPressed((theInput->KEYBOARD_1))) 
	//{
	//	DebugRender::GetInstance()->CreateDebugPoint(m_gameCamera->m_transform->GetWorldPosition() + m_gameCamera->m_transform->GetWorldForward(), .25f, Rgba::BLUE, Rgba::RED, 5.f, LESS_DEPTH_TYPE, m_gameCamera);
	//}

	//if(theInput->WasKeyJustPressed((theInput->KEYBOARD_2))) 
	//{
	//	DebugRender::GetInstance()->CreateDebugCube(m_gameCamera->m_transform->GetWorldPosition() + m_gameCamera->m_transform->GetWorldForward(), Vector3(2.f, 2.f, 2.f), Rgba::RED, Rgba::BLUE, 10.f, LESS_DEPTH_TYPE, m_gameCamera);
	//}

	//if(theInput->WasKeyJustPressed((theInput->KEYBOARD_3))) 
	//{
	//	DebugRender::GetInstance()->CreateDebugWireCube(m_gameCamera->m_transform->GetWorldPosition() + m_gameCamera->m_transform->GetWorldForward(), Vector3(1.5f, 1.5f, 1.5f), Rgba::RED, Rgba::BLUE, 10.f, LESS_DEPTH_TYPE, m_gameCamera);
	//}

	//if(theInput->WasKeyJustPressed((theInput->KEYBOARD_4))) 
	//{
	//	DebugRender::GetInstance()->CreateDebugQuad(m_gameCamera->m_transform->GetWorldPosition() + m_gameCamera->m_transform->GetWorldForward(), Vector2(.25f, .25f), Rgba::RED, Rgba::BLUE, 10.f, LESS_DEPTH_TYPE, m_gameCamera);
	//}

	//if(theInput->WasKeyJustPressed((theInput->KEYBOARD_5))) 
	//{
	//	DebugRender::GetInstance()->CreateDebugSphere(m_gameCamera->m_transform->GetWorldPosition() + m_gameCamera->m_transform->GetWorldForward(), 1.f, Rgba::RED, Rgba::BLUE, 10.f, LESS_DEPTH_TYPE, m_gameCamera);
	//}

	//if(theInput->WasKeyJustPressed((theInput->KEYBOARD_6))) 
	//{
	//	DebugRender::GetInstance()->CreateDebugWireSphere(m_gameCamera->m_transform->GetWorldPosition() + m_gameCamera->m_transform->GetWorldForward(), 1.f, Rgba::RED, Rgba::BLUE, 10.f, LESS_DEPTH_TYPE, m_gameCamera);
	//}

	//if(theInput->WasKeyJustPressed(theInput->KEYBOARD_7))
	//{
	//	DebugRender::GetInstance()->CreateDebugBasis(m_gameCamera->m_transform->GetWorldPosition() + m_gameCamera->m_transform->GetWorldForward(), 1.f, 10.f, LESS_DEPTH_TYPE, m_gameCamera);
	//}

	//2d debug render examples
	//if(theInput->WasKeyJustPressed((theInput->KEYBOARD_8))) 
	//{
	//	Window* theWindow = Window::GetInstance();
	//	DebugRender::GetInstance()->CreateDebugCrosshair2D(theWindow->GetCenterOfClientWindow(), Rgba::WHITE, Rgba::WHITE, -1.f, ALWAYS_DEPTH_TYPE);
	//	theWindow = nullptr;
	//}

	//if(theInput->WasKeyJustPressed((theInput->KEYBOARD_9))) 
	//{
	//	Window* theWindow = Window::GetInstance();
	//	DebugRender::GetInstance()->CreateDebugQuad2D(theWindow->GetCenterOfClientWindow(), Vector2(100.f, 100.f), Rgba::RED, Rgba::RED, 5.f, ALWAYS_DEPTH_TYPE);
	//	theWindow = nullptr;
	//}

	//if(theInput->WasKeyJustPressed((theInput->KEYBOARD_0)))
	//{
	//	Window* theWindow = Window::GetInstance();
	//	DebugRender::GetInstance()->CreateDebugLine2D(Vector2(theWindow->m_clientWidth, theWindow->m_clientWidth), Vector2(theWindow->m_clientWidth * 0.5f, theWindow->m_clientWidth * 0.5f), Rgba::RED, Rgba::BLUE, 10.f, ALWAYS_DEPTH_TYPE);
	//	theWindow = nullptr;
	//}

	//if(theInput->IsKeyPressed((theInput->KEYBOARD_C))) 
	//{
	//	DebugRender::GetInstance()->CreateDebugLine(m_gameCamera->m_transform->GetLocalPosition(), m_gameCamera->m_transform->GetLocalPosition() - m_gameCamera->m_transform->GetWorldForward() * 0.75f, Rgba::RED, Rgba::BLUE, 5.f, LESS_DEPTH_TYPE, m_gameCamera);
	//}

	////lighting hotkeys
	//if(theInput->IsKeyPressed((theInput->KEYBOARD_O))) 
	//{
	//	float ambientIntensity = theRenderer->GetAmbientLightIntensity();
	//	ambientIntensity -= 0.01f;
	//	ambientIntensity = ClampFloat(ambientIntensity, 0.f, 1.f);

	//	theRenderer->SetAmbientLightIntensity(ambientIntensity);
	//}
	//if(theInput->IsKeyPressed((theInput->KEYBOARD_P))) 
	//{
	//	float ambientIntensity = theRenderer->GetAmbientLightIntensity();

	//	ambientIntensity += 0.01f;
	//	ambientIntensity = ClampFloat(ambientIntensity, 0.f, 1.f);

	//	theRenderer->SetAmbientLightIntensity(ambientIntensity);
	//}

	//if(theInput->IsKeyPressed((theInput->KEYBOARD_K))) 
	//{
	//	float specularAmount = theRenderer->GetSpecularAmount();

	//	specularAmount -= 0.01f;
	//	specularAmount = ClampFloat(specularAmount, 0.f, 1.f);

	//	theRenderer->SetSpecularAmount(specularAmount);
	//}
	//if(theInput->IsKeyPressed((theInput->KEYBOARD_L))) 
	//{
	//	float specularAmount = theRenderer->GetSpecularAmount();

	//	specularAmount += 0.01f;
	//	specularAmount = ClampFloat(specularAmount, 0.f, 1.f);

	//	theRenderer->SetSpecularAmount(specularAmount);
	//}

	//if(theInput->IsKeyPressed((theInput->KEYBOARD_N))) 
	//{
	//	float specularPower = theRenderer->GetSpecularPower();

	//	specularPower -= 1.f;
	//	specularPower = ClampFloat(specularPower, 0.f, 9999999999.f);

	//	theRenderer->SetSpecularPower(specularPower);
	//}
	//if(theInput->IsKeyPressed((theInput->KEYBOARD_M))) 
	//{
	//	float specularPower = theRenderer->GetSpecularPower();

	//	specularPower += 1.f;
	//	specularPower = ClampFloat(specularPower, 0.f, 9999999999.f);

	//	theRenderer->SetSpecularPower(specularPower);
	//}		

	/*theInput = nullptr;
	theRenderer = nullptr;*/
	return deltaSeconds;
}





