#include "Game\GameObjects\Entity.hpp"
#include "Game\GameObjects\GameCamera.hpp"

//  =========================================================================================
Entity::Entity()
{
}

//  =========================================================================================
Entity::~Entity()
{
	m_attachedCamera = nullptr;
}

//  =========================================================================================
void Entity::Update(float deltaSeconds)
{
	//does nothing. Intended for derivation
}

//  =========================================================================================
float Entity::UpdateFromInput(float deltaSeconds)
{
	return deltaSeconds;
}

//  =========================================================================================
void Entity::UpdatePhysics(float deltaSeconds)
{
	UNUSED(deltaSeconds);
}

//  =========================================================================================
void Entity::Render()
{
	//does nothing. Intended for derivation
}

//  =========================================================================================
void Entity::SetCamera(GameCamera* camera)
{
	m_attachedCamera = camera;
}

//  =========================================================================================
void Entity::DetachCamera()
{
	m_attachedCamera = nullptr;
}
