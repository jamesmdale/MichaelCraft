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
Block* BlockLocator::GetBlock()
{
	if(m_chunk != nullptr)
		return nullptr;

	return &m_chunk->m_blocks[m_blockIndex];
}

//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorToNorth()
{
	if (m_blockIndex & (BLOCKS_WIDE_X - 1))
	{
		BlockLocator(m_chunk, (m_blockIndex + 1) % BLOCKS_WIDE_X);
	}
	
	return BlockLocator(m_chunk->m_northNeighbor, m_blockIndex);
}

//  ========================================================================================= 
BlockLocator BlockLocator::GetBlockLocatorToWest()
{
	return BlockLocator();
}

//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorToSouth()
{
	return BlockLocator();
}

//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorToEast()
{
	return BlockLocator();
}

//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorAbove()
{
	return BlockLocator();
}

//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorBelow()
{
	return BlockLocator();
}
