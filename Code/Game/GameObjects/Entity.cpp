#include "Game\GameObjects\Entity.hpp"
#include "Game\GameObjects\GameCamera.hpp"

//  =========================================================================================
Entity::Entity()
{
}

//  =========================================================================================
Entity::~Entity()
{
}

//  =========================================================================================
void Entity::Update(float deltaSeconds)
{

}

//  =========================================================================================
float Entity::UpdateFromInput(float deltaSeconds)
{
	return 0.0f;
}

//  =========================================================================================
void Entity::SetCamera(GameCamera* camera)
{
	m_attachedCamera = camera;
}
