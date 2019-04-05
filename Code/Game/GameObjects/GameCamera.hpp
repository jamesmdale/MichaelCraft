#pragma once
#include "Engine\Math\Vector3.hpp"
#include "Engine\Math\Matrix44.hpp"

class Entity;

class GameCamera
{
public:
	GameCamera();
	~GameCamera();

	void CreateFliippedViewMatrix(Matrix44& outMatrix);
	void Translate(Vector3 translation);

public:
	float m_rollDegreesX = 0.0f; //roll
	float m_pitchDegreesY = 0.0f; //pitch
	float m_yawDegreesZ = 0.0f; //yaw

	Vector3 m_position = Vector3::ZERO;

	Entity* m_attachedEntity = nullptr;
};

