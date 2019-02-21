#pragma once
#include "Engine\Math\AABB3.hpp"
#include "Engine\Math\IntVector3.hpp"
#include "Game\GameObjects\Block.hpp"
#include "Game\GameCommon.hpp"

class Meshbuilder;
class Mesh;

enum eNeighborType
{
	NORTH_NEIGHBOR_TYPE,
	WEST_NEIGHBOR_TYPE,
	SOUTH_NEIGHBOR_TYPE,
	EAST_NEIGHBOR_TYPE,
	NUM_NEIGHBOR_TYPES
};

class Chunk
{
public:
	Chunk(const IntVector2& coordinates);
	~Chunk();

	void Update();
	void Render();

	// helper functions ----------------------------------------------
	void GenerateBlockDataWithPerlin();
	void GenerateChunkMesh();

	//block helpers
	int GetBlockIndexForBlockCoords(const IntVector3& blockCoords);
	IntVector3 GetBlockCoordsForBlockIndex(int blockIndex);
	Vector3 GetBlockWorldCoordsForBlockIndex(int blockIndex);
	Vector3 GetBlockWorldCenterForBlockIndex(int blockIndex);
	void AddBlockToMesh(const int blockIndex, const Vector3& center, Block* block);

	void AddNeighbor(Chunk* neighbor, eNeighborType neighborDirection);
	bool DoesHaveAllNeighbors();

public:
	Block m_blocks[BLOCKS_PER_CHUNK];
	IntVector2 m_chunkCoords;
	AABB3 m_worldBounds;
	MeshBuilder* m_meshBuilder = nullptr;
	Mesh* m_gpuMesh = nullptr;

	Chunk* m_northNeighbor = nullptr;
	Chunk* m_westNeighbor = nullptr;
	Chunk* m_southNeighbor = nullptr;
	Chunk* m_eastNeighbor = nullptr;
	
	bool m_isMeshDirty = false;
	bool m_doesRequireSave = false;
};
