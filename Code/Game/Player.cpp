#include "Game\Player.hpp"
#include "Engine\Input\InputSystem.hpp"

//  =========================================================================================
Player::Player()
{
}

//  =========================================================================================
Player::~Player()
{
	m_camera = nullptr;
}

//  =========================================================================================
void Player::Update(float deltaSeconds)
{

}

//  =========================================================================================
void Player::PreRender()
{

}

//  =========================================================================================
float Player::UpdateFromInput(float deltaSeconds)
{
	InputSystem* theInput = InputSystem::GetInstance();

	Vector2 mouseDelta = Vector2::ZERO;
	mouseDelta = InputSystem::GetInstance()->GetMouse()->GetMouseDelta();

	//calculate rotation for camera and use same rotation for player
	m_transform->AddRotation(Vector3(mouseDelta.y, mouseDelta.x, 0.f) * deltaSeconds * 10.f);

	float clampedX = ClampFloat(m_transform->GetLocalRotationAroundX(), -90.f, 90.f);
	float clampedY = Modulus(m_transform->GetLocalRotationAroundY(), 360.f);

	Vector3 rotation = Vector3(clampedX, clampedY, 0.f);

	m_transform->SetLocalRotation(Vector3(rotation));

	Vector3 positionToAdd = Vector3::ZERO;
	Vector3 positionAtStartOfFrame = positionToAdd;
	//Vector3 positionAtStartOfFrame = m_transform->GetWorldPosition();

	//update movement
	if (theInput->IsKeyPressed(theInput->KEYBOARD_W))
	{
		//calculate movement for camera and use same movement for ship and light
		positionToAdd = m_transform->GetWorldForward() * deltaSeconds * 10.f;
		m_transform->TranslatePosition(positionToAdd);
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_S))
	{
		positionToAdd = (-1.f * m_transform->GetWorldForward()) * deltaSeconds * 10.f;
		m_transform->TranslatePosition(positionToAdd);
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_A))
	{
		positionToAdd = (-1.f * m_transform->GetWorldRight()) * deltaSeconds * 10.f;
		m_transform->TranslatePosition(positionToAdd);
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_D))
	{
		positionToAdd = m_transform->GetWorldRight() * deltaSeconds * 10.f;
		m_transform->TranslatePosition(positionToAdd);
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_SPACE))
	{
		positionToAdd = m_transform->GetWorldUp() * deltaSeconds * 10.f;
		m_transform->TranslatePosition(positionToAdd);
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_CONTROL))
	{
		positionToAdd = (-1.f * m_transform->GetWorldUp()) * deltaSeconds * 10.f;
		m_transform->TranslatePosition(positionToAdd);
	}

	theInput = nullptr;

	return deltaSeconds;
}

//  =========================================================================================
void Player::SetCamera(Camera * camera)
{
	if (camera != nullptr)
	{
		if (m_camera != nullptr)
		{
			m_transform->RemoveChildTransform(m_camera->m_transform);
			m_camera = nullptr;
		}

		m_camera = camera;
		m_transform->AddChildTransform(m_camera->m_transform);
	}
}

