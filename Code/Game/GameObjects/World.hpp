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

	void ActivateChunks();
	void GenerateDirtyChunks();
	void DeactivateChunks();

	void ActivateChunk(const IntVector2& chunkCoordinates);
	void DeactivateChunk(const IntVector2& keyVal);

	void GenerateChunkBuildOrderCheatSheet();
	Mesh* CreateBasis(const Vector3& center, float width, float scale = 1.f);

public:
	std::map<IntVector2, Chunk*> m_activeChunks;

	std::vector<IntVector2> m_neighborHoodBuildOrder;

	Camera* m_uiCamera = nullptr;
	Camera* m_engineCamera = nullptr;
	GameCamera* m_gameCamera = nullptr;
};

bool CompareDistanceFromZeroLessThan(const IntVector2& first, const IntVector2& second);
bool CompareDistanceFromLocationToLocationLessThanRadius(float& outDistanceSquared, const Vector2& startLocation, const Vector2& endLocation, const float radius);
bool CompareDistanceFromLocationToLocationGreaterThanRadius(float& outDistanceSquared, const Vector2 & startLocation, const Vector2 & endLocation, const float radius);
