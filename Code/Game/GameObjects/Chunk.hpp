#pragma once
#include "Engine\Renderer\MeshBuilder.hpp"
#include "Engine\Math\AABB3.hpp"
#include "Engine\Math\IntVector3.hpp"
#include "Game\GameObjects\Block.hpp"
#include "Game\GameCommon.hpp"

class Chunk
{
public:
	Chunk();
	~Chunk();

	void Update();
	void Render();

	//helper functions
	void GenerateChunkMesh();
	int GetBlockIndexForBlockCoords(const IntVector3& blockCoords);
	IntVector3 GetBlockCoordsForBlockIndex(int blockIndex);

public:
	Block m_blocks[BLOCKS_PER_CHUNK];
	IntVector2 m_chunkCoords;
	AABB3 m_worldBounds;
	MeshBuilder* m_meshBuilder = new MeshBuilder();
	Mesh* m_gpyMesh = nullptr;
	bool m_isVBOPriority =- true;
	
};