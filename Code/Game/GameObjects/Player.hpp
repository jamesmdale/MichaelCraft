#pragma once
#include "Game\GameObjects\Entity.hpp"
#include "Game\GameObjects\BlockLocator.hpp"
#include <vector>

class Mesh;
class BlockLocator;

class Player : public Entity
{
public:
	Player(World* world);
	~Player();

	virtual void Update(float deltaSeconds) override;
	virtual void UpdatePhysics(float deltaSeconds) override;
	virtual void Render() override;
    void PreRender();

	float UpdateFromInput(float deltaSeconds);
	void UpdateIsOnGround();

	void SetCamera(GameCamera* camera);
	void GenerateDebugVisualsMesh();
	void GenerateDebugPhysicsMesh();
	
	Vector3 GetBottomCenterPivot();
	void UpdateBoundsToCurrentPosition();

	//physics helpers
	void ApplyFriction(float deltaSeconds);
	void GetBlockNeighborhood(std::vector<BlockLocator>& outBlockLocators);
	bool PushOutOfBlock(BlockLocator locator);

	//inline functions
	inline AABB3 GetVisualBounds() { return m_visualBounds; }
	inline float GetSpeed() { return m_velocity.GetLength(); }

protected:
	AABB3 m_visualBounds;
	Mesh* m_visualsMesh = nullptr;
	Mesh* m_physicsMesh = nullptr;

	bool m_isOnGround = false;
};