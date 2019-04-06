#pragma once
#include "Game\GameObjects\Entity.hpp"

class Mesh;

class Player : Entity
{
public:
	Player();
	~Player();

	virtual void Update(float deltaSeconds) override;
	virtual void Render() override;
    void PreRender();

	float UpdateFromInput(float deltaSeconds);

	void SetCamera(GameCamera* camera);
	void GenerateDebugVisualsMesh();
	void GenerateDebugPhysicsMesh();
	
	Vector3 GetBottomCenterPivot();
	void UpdateBoundsToCurrentPosition();

	inline AABB3 GetVisualBounds() { return m_visualBounds; }

protected:
	AABB3 m_visualBounds;
	Mesh* m_visualsMesh = nullptr;
	Mesh* m_physicsMesh = nullptr;
};