#include "Game\GameObjects\Player.hpp"
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
	UNUSED(deltaSeconds);
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

	theInput = nullptr;

	return deltaSeconds;
}

//  =========================================================================================
void Player::SetCamera(GameCamera* camera)
{

}

