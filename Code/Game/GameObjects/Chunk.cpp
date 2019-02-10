#include "Game\GameObjects\Chunk.hpp"
#include "Game\Definitions\BlockDefinition.hpp"

//  =========================================================================================
Chunk::Chunk()
{
	BlockDefinition* airBlockDef = BlockDefinition::GetDefinitionById(0);
	for (int blockIndex = 0; blockIndex < BLOCKS_PER_CHUNK; ++blockIndex)
	{
		m_blocks[blockIndex].m_type = airBlockDef->m_type;
	}
}

//  =========================================================================================
Chunk::~Chunk()
{

}

//  =========================================================================================
void Chunk::Update()
{

}

//  =========================================================================================
void Chunk::Render()
{

}

//  =========================================================================================
void Chunk::GenerateChunkMesh()
{
}

//  =========================================================================================
int Chunk::GetBlockIndexForBlockCoords(const IntVector3& blockCoords)
{
	// FAST VERSION return blockCoords.x | (blockCoords.y << CHUNK_X_MASK) + (blockCoords.z << (CHUNK_X_MASK + CHUNK_Y_MASK));
	return blockCoords.x + (blockCoords.y * BLOCKS_WIDE_X) + (blockCoords.z * (BLOCKS_WIDE_X * BLOCKS_WIDE_Y));
}

//  =========================================================================================
IntVector3 Chunk::GetBlockCoordsForBlockIndex(int blockIndex)
{
	int x = blockIndex & (CHUNK_X_MASK);
	int y = (blockIndex & CHUNK_Y_MASK) >> CHUNK_X_MASK;
	int z = blockIndex >> (BITS_WIDE_X + BITS_WIDE_Y);	

	return IntVector3(x, y, z);
}
