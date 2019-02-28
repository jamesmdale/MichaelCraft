#include "Game\GameObjects\Block.hpp"
#include "Game\Definitions\BlockDefinition.hpp"
#include "Game\GameCommon.hpp"

//  =========================================================================================
Block::Block()
{

}

//  =========================================================================================
Block::Block(BlockDefinition* definition)
{
	m_type = definition->m_type;
	m_bits = definition->m_defaultBits;
}

//  =========================================================================================
Block::~Block()
{

}


//  =========================================================================================
void SetBlockToType(Block* block, const uchar8 id)
{
	BlockDefinition* newDefinition = BlockDefinition::GetDefinitionById(id);

	block->m_type = newDefinition->m_type;
	block->m_bits = newDefinition->m_defaultBits;
}
