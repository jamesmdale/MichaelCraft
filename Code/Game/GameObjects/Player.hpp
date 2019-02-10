#pragma once
#include "Engine\Core\GameObject.hpp"
#include "Game\GameObjects\GameCamera.hpp"

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
	GameCamera* m_camera = nullptr;
};