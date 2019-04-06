#pragma once
#include "Engine\Math\Vector3.hpp"
#include "Engine\Math\Matrix44.hpp"

class Entity;

enum CameraModes
{
	FIRST_PERSON_CAMERA_MODE,
	THIRD_PERSON_CAMERA_MODE,
	FIXED_ANGLE_CAMERA_MODE,
	STATIONARY_CAMERA_MODE,
	DETACHED_CAMERA_MODE,
	NUM_CAMERA_MODES
};

class GameCamera
{
public:
	GameCamera();
	~GameCamera();

	void CreateFliippedViewMatrix(Matrix44& outMatrix);
	void Translate(Vector3 translation);
	void AttachToEntity(Entity* entity);

public:
	float m_rollDegreesX = 0.0f; //roll
	float m_pitchDegreesY = 0.0f; //pitch
	float m_yawDegreesZ = 0.0f; //yaw

	CameraModes m_currentCameraMode = NUM_CAMERA_MODES;
	Vector3 m_position = Vector3::ZERO;
	Entity* m_attachedEntity = nullptr;
};

