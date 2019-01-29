#pragma once
#include "Engine\Core\GameObject.hpp"

class Player : GameObject
{
public:
	Player();
	~Player();

	virtual void Update(float deltaSeconds) override;
    void PreRender();

	float UpdateFromInput(float deltaSeconds);

	void SetCamera(Camera* camera);

public:
	Camera* m_camera = nullptr;
};