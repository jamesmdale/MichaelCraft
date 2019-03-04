#pragma once
#include <map>
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Camera\Camera.hpp"
#include "Game\GameObjects\Chunk.hpp"
#include "Game\GameObjects\GameCamera.hpp"
#include "Game\Helpers\RaycastResult.hpp"
#include <deque>

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
	void UpdateDirtyLighting();
	void UpdateChunks();

	void RenderUI();
	void RenderChunks();
	void RenderDebug();

	//Chunk Logic
	void ActivateChunks();
	void GenerateDirtyChunks();
	void DeactivateChunks();
	void DeactivateAllChunks();

	void ActivateChunk(const IntVector2& chunkCoordinates);
	void InitializeChunkLighting(Chunk* chunk);
	void DeactivateChunk(const IntVector2& keyVal);

	void GenerateChunkBuildOrderCheatSheet();
	void LoadSavedChunkReferences();

	//Lighting
	void ProcessLightingForBlock(BlockLocator blockLocator);
	void AddBlockLocatorToDirtyLightingQueue(BlockLocator blockLocator);

	//Raycasting
	RaycastResult Raycast(const Vector3& start, const Vector3& forward, float maxDistance);

	//Player Actions
	void InitializeSelectableBlockList();
	void DigBlock();
	void PlaceBlock();
	void ToggleCameraViewLocked();
	bool IsCameraViewLocked(){return m_isCameraViewLocked;}
	void CopyCameraDataToPlayerView(const Vector3& cameraPosition, const Vector3& cameraForward);

	void RemoveSkyFlagFromBelowBlocks(BlockLocator& blockLocator);
	void AddSkyFlagToBelowBlocks(BlockLocator& blockLocator);

	//UI
	Mesh* CreateTexturedUIMesh();
	Mesh* CreateUITextMesh();

public:
	std::map<IntVector2, Chunk*> m_activeChunks;
	std::vector<IntVector2> m_chunksOnDisk;
	std::vector<IntVector2> m_neighborHoodBuildOrder;

	std::deque<BlockLocator> m_blocksWithDirtyLighting;

	Camera* m_uiCamera = nullptr;
	Camera* m_engineCamera = nullptr;
	GameCamera* m_gameCamera = nullptr;

	Vector3 m_playerViewPosition;
	Vector3 m_playerViewForwardNormalized;

	RaycastResult m_raycastResult;

	int m_selectedBlockIndex = 0;
	std::vector<uint8> m_selectableBlockTypes;

private:
	bool m_isCameraViewLocked = false;
};

bool CompareDistanceFromZeroLessThan(const IntVector2& first, const IntVector2& second);
bool CompareDistanceFromLocationToLocationLessThanRadius(float& outDistanceSquared, const Vector2& startLocation, const Vector2& endLocation, const float radius);
bool CompareDistanceFromLocationToLocationGreaterThanRadius(float& outDistanceSquared, const Vector2 & startLocation, const Vector2 & endLocation, const float radius);
