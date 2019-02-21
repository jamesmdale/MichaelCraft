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
void BlockLocator::StepNorth()
{
	int yIndex = m_blockIndex & (CHUNK_Y_MASK);
	if (yIndex != CHUNK_Y_MASK)
	{
		m_chunk = m_chunk;
		m_blockIndex = (m_blockIndex + BLOCKS_WIDE_X);
		return;
	}

	//error case if north neighbor doesn't exist
	if (m_chunk->m_northNeighbor == nullptr)
	{
		m_chunk = nullptr;
		m_blockIndex = -1;
		return;
	}
	else
	{
		m_chunk = m_chunk->m_northNeighbor;
		m_blockIndex = m_blockIndex & (~CHUNK_Y_MASK);
		return;
	}	
}

//  =========================================================================================
void BlockLocator::StepWest()
{
	int xIndex = m_blockIndex & (CHUNK_X_MASK);
	if (xIndex != 0)
	{
		m_blockIndex = (m_blockIndex - 1);
		return;
	}

	//error case if west neighbor doesn't exist
	if (m_chunk->m_westNeighbor == nullptr)
	{
		m_chunk = nullptr;
		m_blockIndex = -1;
		return;
	}
	else
	{
		m_chunk = m_chunk->m_westNeighbor;
		m_blockIndex = m_blockIndex | CHUNK_X_MASK;
		return;
	}
}

//  =========================================================================================
void BlockLocator::StepSouth()
{
	int yIndex = m_blockIndex & (CHUNK_Y_MASK);
	if (yIndex != 0)
	{
		m_blockIndex = m_blockIndex - BLOCKS_WIDE_X;
		return;
	}

	//error case if south neighbor doesn't exist
	if (m_chunk->m_southNeighbor == nullptr)
	{
		m_chunk = nullptr;
		m_blockIndex = -1;
		return;
	}
	else
	{
		m_chunk = m_chunk->m_southNeighbor;
		m_blockIndex = m_blockIndex | CHUNK_Y_MASK;
	}
}

//  =========================================================================================
void BlockLocator::StepEast()
{
	int xIndex = m_blockIndex & (CHUNK_X_MASK);
	if (xIndex != CHUNK_X_MASK)
	{
		m_blockIndex = (m_blockIndex + 1);
		return;
	}

	//error case if east neighbor doesn't exist
	if (m_chunk->m_eastNeighbor == nullptr)
	{
		m_chunk = nullptr;
		m_blockIndex = -1;
		return;
	}
	else
	{
		m_chunk = m_chunk->m_eastNeighbor;
		m_blockIndex = m_blockIndex & (~ CHUNK_X_MASK);
		return;
	}
}

//  =========================================================================================
void BlockLocator::StepUp()
{
	int zIndex = m_blockIndex & (CHUNK_Z_MASK);
	if (zIndex != CHUNK_Z_MASK)
	{
		m_blockIndex = m_blockIndex + (BLOCKS_WIDE_X * BLOCKS_WIDE_Y);
		return;
	}

	m_chunk = nullptr;
	m_blockIndex = -1;
}

//  =========================================================================================
void BlockLocator::StepDown()
{
	int zIndex = m_blockIndex & (CHUNK_Z_MASK);
	if (zIndex != 0)
	{
		m_blockIndex = m_blockIndex - (BLOCKS_WIDE_X * BLOCKS_WIDE_Y);
	}

	m_chunk = nullptr;
	m_blockIndex = -1;
}

//  =========================================================================================
BlockLocator BlockLocator::GetBlockLocatorToNorth()
{
	int yIndex = m_blockIndex & (CHUNK_Y_MASK);
	if (yIndex != CHUNK_Y_MASK)
	{
		return BlockLocator(m_chunk, (m_blockIndex + BLOCKS_WIDE_X));
	}

	//error case if north neighbor doesn't exist
	if (m_chunk->m_northNeighbor == nullptr)
	{
		return BlockLocator(nullptr, -1);
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

	//error case if south neighbor doesn't exist
	if (m_chunk->m_southNeighbor == nullptr)
	{
		return BlockLocator(nullptr, -1);
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

	//error case if west neighbor doesn't exist
	if (m_chunk->m_westNeighbor == nullptr)
	{
		return BlockLocator(nullptr, -1);
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

	//error case if east neighbor doesn't exist
	if (m_chunk->m_eastNeighbor == nullptr)
	{
		return BlockLocator(nullptr, -1);
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
