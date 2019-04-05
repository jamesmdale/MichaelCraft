#pragma once
#include "Engine\Math\AABB3.hpp"

class GameCamera;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void Update(float deltaSeconds);
	virtual float UpdateFromInput(float deltaSeconds);
	void SetCamera(GameCamera* camera);

	inline bool IsCameraAttached() { return m_attachedCamera != nullptr; }

public:
	//AABB3 m_physicsBounds;

	Vector3 m_firstPersonCameraPositionOffset;
	Vector3 m_thirdPersonCameraPositionOffset;
	
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_velocity;

	GameCamera* m_attachedCamera = nullptr;
};