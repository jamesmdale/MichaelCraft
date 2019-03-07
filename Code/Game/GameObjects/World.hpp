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
	void UpdateTime(float deltaSeconds);
	void UpdateGlobalLightingColors();
	void UpdateLightingFromTimeOfDay();

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
	void GetTimeOfDay(float inSeconds, int& outHours, int& outMinutes, int& outSeconds, std::string& outAmPm);
	
	void PerlinLightningStrike();
	void IndoorLightingFlicker();

	//UI
	Mesh* CreateTexturedUIMesh();
	Mesh* CreateUITextMesh();

	//debug
	void GenerateDebugSkyMesh();
	Chunk* GetActiveChunkFromPlayerPosition(const Vector3& playerPosition);

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

	Mesh* m_debugSkyMesh = nullptr;

	Rgba m_globalIndoorLightColor;
	Rgba m_globalOutdoorLightColor;
	Rgba m_skyColor;
	Vector2 m_fogNearFarRange;

	float m_currentTimeOfDay = TIME_PER_DAY_IN_SECONDS * 0.5f;
	uint m_days = 0;

	float m_worldTimeScale = 500.f;

private:
	bool m_isDebugRGB = false;
	bool m_isCameraViewLocked = false;
	bool m_isDebugDirtyLighting = false;
	bool m_shouldStepDirtyLightingDebug = false;
	std::vector<Vector3> m_dirtyDebugLightingPoints;
};

bool CompareDistanceFromZeroLessThan(const IntVector2& first, const IntVector2& second);
bool CompareDistanceFromLocationToLocationLessThanRadius(float& outDistanceSquared, const Vector2& startLocation, const Vector2& endLocation, const float radius);
bool CompareDistanceFromLocationToLocationGreaterThanRadius(float& outDistanceSquared, const Vector2 & startLocation, const Vector2 & endLocation, const float radius);
