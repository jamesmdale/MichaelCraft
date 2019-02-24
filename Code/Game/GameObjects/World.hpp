#pragma once
#include <map>
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Camera\Camera.hpp"
#include "Game\GameObjects\Chunk.hpp"
#include "Game\GameObjects\GameCamera.hpp"
#include "Game\Helpers\RaycastResult.hpp"

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

	void UpdatePlayerViewPosition();
	void UpdateChunks();

	void RenderChunks();
	void RenderDebug();

	void ActivateChunks();
	void GenerateDirtyChunks();
	void DeactivateChunks();
	void DeactivateAllChunks();

	void ActivateChunk(const IntVector2& chunkCoordinates);
	void DeactivateChunk(const IntVector2& keyVal);

	void GenerateChunkBuildOrderCheatSheet();
	void LoadSavedChunkReferences();

	//Raycasting
	RaycastResult Raycast(const Vector3& start, const Vector3& forward, float maxDistance);

	//player actions
	void DigBlock();
	void PlaceBlock();
	void ToggleCameraViewLocked();
	void LockCamera();
	bool IsCameraViewLocked(){return m_isCameraViewLocked;}
	void UnlockCamera();
	void CopyCameraDataToPlayerView(const Vector3& cameraPosition, const Vector3& cameraForward);

public:
	std::map<IntVector2, Chunk*> m_activeChunks;
	std::vector<IntVector2> m_chunksOnDisk;
	std::vector<IntVector2> m_neighborHoodBuildOrder;

	Camera* m_uiCamera = nullptr;
	Camera* m_engineCamera = nullptr;
	GameCamera* m_gameCamera = nullptr;

	Vector3 m_playerViewPosition;
	Vector3 m_playerViewForwardNormalized;

	RaycastResult m_raycastResult;

private:
	bool m_isCameraViewLocked = false;
};

bool CompareDistanceFromZeroLessThan(const IntVector2& first, const IntVector2& second);
bool CompareDistanceFromLocationToLocationLessThanRadius(float& outDistanceSquared, const Vector2& startLocation, const Vector2& endLocation, const float radius);
bool CompareDistanceFromLocationToLocationGreaterThanRadius(float& outDistanceSquared, const Vector2 & startLocation, const Vector2 & endLocation, const float radius);
