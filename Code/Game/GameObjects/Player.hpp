#pragma once
#include "Game\GameObjects\Entity.hpp"

class GameCamera;

class Player : Entity
{
public:
	Player();
	~Player();

	virtual void Update(float deltaSeconds) override;
    void PreRender();

	float UpdateFromInput(float deltaSeconds);

	void SetCamera(GameCamera* camera);
};