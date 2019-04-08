#include "Game\GameObjects\GameCamera.hpp"
#include "Game\GameObjects\Entity.hpp"
#include "Game\GameCommon.hpp"
#include "Game\Game.hpp"
#include "Engine\Input\InputSystem.hpp"


//  =========================================================================================
GameCamera::GameCamera()
{
	m_currentCameraMode = DETACHED_CAMERA_MODE;
}

//  =========================================================================================
GameCamera::~GameCamera()
{
}

//  =========================================================================================
void GameCamera::UpdateFromInput(float deltaSeconds)
{
	if (m_currentCameraMode != DETACHED_CAMERA_MODE)
		return;

	InputSystem* theInput = InputSystem::GetInstance();
	Game* theGame = Game::GetInstance();

	Vector2 mouseDelta = Vector2::ZERO;
	mouseDelta = InputSystem::GetInstance()->GetMouse()->GetMouseDelta();

	//calculate rotation for camera and use same rotation for player
	//m_camera->m_transform->AddRotation(Vector3(mouseDelta.y, mouseDelta.x, 0.f) * deltaSeconds * 10.f);

	m_yawDegreesZ += -mouseDelta.x * 0.05f;
	m_pitchDegreesY += mouseDelta.y * 0.05f;

	m_yawDegreesZ = Modulus(m_yawDegreesZ, 360.f);
	m_pitchDegreesY = ClampFloat(m_pitchDegreesY, -90.f, 90.f);

	/*Vector3 rotation = Vector3(clampedX, clampedY, 0.f);
	m_camera->m_transform->SetLocalRotation(Vector3(rotation));*/

	Vector3 cameraCardinalForward = Vector3(CosDegrees(m_yawDegreesZ), SinDegrees(m_yawDegreesZ), 0);
	cameraCardinalForward.Normalize();
	Vector3 cameraRight = Vector3(SinDegrees(m_yawDegreesZ), -CosDegrees(m_yawDegreesZ), 0);

	Vector3 positionToAdd = Vector3::ZERO;
	Vector3 positionAtStartOfFrame = positionToAdd;

	//update movement
	//forward (x)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_W))
	{
		//calculate movement for camera and use same movement for ship and light
		positionToAdd = cameraCardinalForward * deltaSeconds * CAMERA_FLY_SPEED;
	}

	//backward (-x)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_S))
	{
		positionToAdd = -cameraCardinalForward * deltaSeconds * CAMERA_FLY_SPEED;
	}

	//left is north (y)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_A))
	{
		positionToAdd = -cameraRight * deltaSeconds * CAMERA_FLY_SPEED;
	}

	//right is south (-y)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_D))
	{
		positionToAdd = cameraRight * deltaSeconds * CAMERA_FLY_SPEED;
	}

	//up is (+z)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_SPACE) || theInput->IsKeyPressed(theInput->KEYBOARD_E))
	{
		positionToAdd = g_worldUp * deltaSeconds * CAMERA_FLY_SPEED;
	}

	//up is (-z)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_CONTROL) || theInput->IsKeyPressed(theInput->KEYBOARD_Q))
	{
		positionToAdd = -g_worldUp * deltaSeconds * CAMERA_FLY_SPEED;
	}

	//sprint move speed (twice as fast)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_SHIFT))
	{
		positionToAdd *= 3.f;
	}

	//walk move speed
	if (theInput->IsKeyPressed(theInput->KEYBOARD_C))
	{
		positionToAdd *= 0.15f;
	}

	Translate(positionToAdd);
}

//  =========================================================================================
void GameCamera::CreateFliippedViewMatrix(Matrix44& outMatrix)
{
	outMatrix.SetIdentity();
	outMatrix.Append(g_flipMatrix);
	outMatrix.RotateDegreesAroundX3D(-1.f * m_rollDegreesX);
	outMatrix.RotateDegreesAroundY3D(-1.f * m_pitchDegreesY);
	outMatrix.RotateDegreesAroundZ3D(-1.f * m_yawDegreesZ);
	outMatrix.Translate3D(-1.f * m_position);
}

//  =========================================================================================
void GameCamera::Translate(Vector3 translation)
{
	m_position += translation;
}

//  =========================================================================================
void GameCamera::AttachToEntity(Entity* entity)
{
	m_attachedEntity = entity;
	entity->SetCamera(this);
}
