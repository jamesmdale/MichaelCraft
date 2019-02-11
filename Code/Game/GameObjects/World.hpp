#pragma once
#include <map>
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Camera\Camera.hpp"
#include "Game\GameObjects\Chunk.hpp"
#include "Game\GameObjects\GameCamera.hpp"


typedef IntVector2 ChunkCoords;

class World
{
public:
	World();
	~World();

	void Initialize();
	void Update(float deltaSeconds);
	void Render();
	void UpdateFromInput(float deltaSeconds);

	void UpdateChunks();
	void RenderChunks();

	void ActivateChunk(Chunk* chunk);

	Mesh* CreateBasis(const Vector3& center, float width, float scale = 1.f);

public:
	std::map<IntVector2, Chunk*> m_activeChunks;

	Camera* m_uiCamera = nullptr;
	Camera* m_engineCamera = nullptr;
	GameCamera* m_gameCamera = nullptr;
};

