#include "Game\GameObjects\BlockLocator.hpp"
#include "Game\GameObjects\Chunk.hpp"
#include "Game\GameObjects\Block.hpp"


//  =========================================================================================
BlockLocator::BlockLocator()
{

}

//  =========================================================================================
BlockLocator::BlockLocator(Chunk* chunk, int blockIndex)
{
	m_chunk = chunk;
	m_blockIndex = blockIndex;
}

//  =========================================================================================
BlockLocator::~BlockLocator()
{

}

//  =========================================================================================
bool BlockLocator::IsValid()
{
	if (m_chunk == nullptr || m_blockIndex == -1)
	{
		return false;
	}

	return true;
}

//  =========================================================================================
Block* BlockLocator::GetBlock()
{
	if(m_chunk == nullptr)
		return nullptr;

	return &m_chunk->m_blocks[m_blockIndex];
}

//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorToNorth()
{
	int yIndex = m_blockIndex & (CHUNK_Y_MASK);
	if (yIndex != CHUNK_Y_MASK)
	{
		return BlockLocator(m_chunk, (m_blockIndex + BLOCKS_WIDE_X));
	}
	else
	{
		return BlockLocator(m_chunk->m_northNeighbor, m_blockIndex & (~CHUNK_Y_MASK));

	}	
}

//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorToSouth()
{
	int yIndex = m_blockIndex & (CHUNK_Y_MASK);
	if (yIndex != 0)
	{
		return BlockLocator(m_chunk, (m_blockIndex - BLOCKS_WIDE_X));
	}
	else
	{
		return BlockLocator(m_chunk->m_southNeighbor,  m_blockIndex | CHUNK_Y_MASK);
	}
}

//  ========================================================================================= 
BlockLocator BlockLocator::GetBlockLocatorToWest()
{
	int xIndex = m_blockIndex & (CHUNK_X_MASK);
	if (xIndex != 0)
	{
		return BlockLocator(m_chunk, (m_blockIndex - 1));
	}
	else
	{
		return BlockLocator(m_chunk->m_westNeighbor, m_blockIndex | CHUNK_X_MASK);
	}
}


//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorToEast()
{
	int xIndex = m_blockIndex & (CHUNK_X_MASK);
	if (xIndex != CHUNK_X_MASK)
	{
		return BlockLocator(m_chunk, (m_blockIndex + 1));
	}
	else
	{
		return BlockLocator(m_chunk->m_eastNeighbor, m_blockIndex & (~ CHUNK_X_MASK));
	}
}

//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorAbove()
{
	int zIndex = m_blockIndex & (CHUNK_Z_MASK);
	if (zIndex != CHUNK_Z_MASK)
	{
		return BlockLocator(m_chunk, (m_blockIndex + (BLOCKS_WIDE_X * BLOCKS_WIDE_Y)));
	}

	return BlockLocator(nullptr, -1);
}

//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorBelow()
{
	int zIndex = m_blockIndex & (CHUNK_Z_MASK);
	if (zIndex != 0)
	{
		return BlockLocator(m_chunk, (m_blockIndex - (BLOCKS_WIDE_X * BLOCKS_WIDE_Y)));
	}

	return BlockLocator(nullptr, -1);
}
