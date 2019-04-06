#pragma once
#include "Engine\Math\AABB3.hpp"
#include "Engine\Math\Sphere.hpp"

class GameCamera;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void Update(float deltaSeconds);
	virtual float UpdateFromInput(float deltaSeconds);
	virtual void Render();
	
	void SetCamera(GameCamera* camera);
	void DetachCamera();

	inline bool IsCameraAttached() { return m_attachedCamera != nullptr; }
	inline void EnableInput() { m_doesReceiveUserInput = true; }
	inline void DisableInput() { m_doesReceiveUserInput = false; }
	inline bool DoesReceiveInput() { return m_doesReceiveUserInput; }
	inline Sphere GetPhysicsBounds() { return m_physicsSphere; }

	inline void Translate(const Vector3& translation) { m_position += translation; }

public:
	Vector3 m_firstPersonCameraPositionOffset;
	Vector3 m_thirdPersonCameraPositionOffset;
	
	Vector3 m_position = Vector3::ZERO;	
	Vector3 m_velocity = Vector3::ZERO;

	float m_rollDegreesX = 0.0f; //roll
	float m_pitchDegreesY = 0.0f; //pitch
	float m_yawDegreesZ = 0.0f; //yaw

protected:

	Sphere m_physicsSphere;
	bool m_doesReceiveUserInput = false;
	GameCamera* m_attachedCamera = nullptr;
};