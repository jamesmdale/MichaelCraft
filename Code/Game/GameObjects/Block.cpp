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
bool Block::IsAir()
{
	if(m_type == 0)
		return true;

	return false;
}

//  =========================================================================================
bool Block::IsOpaque()
{
	return m_bits && IS_BLOCK_FULL_OPAQUE_MASK == IS_BLOCK_FULL_OPAQUE_MASK;
}

//  =========================================================================================
bool Block::IsVisible()
{
	return m_bits && IS_BLOCK_VISIBLE_MASK == IS_BLOCK_VISIBLE_MASK;
}

//  =========================================================================================
bool Block::IsSolid()
{
	return m_bits && IS_BLOCK_SOLID_MASK == IS_BLOCK_SOLID_MASK;
}

//  =========================================================================================
bool Block::IsSky()
{
	return m_bits && IS_BLOCK_SKY_MASK == IS_BLOCK_SKY_MASK;
}

//  =========================================================================================
bool Block::IsLightingDirty()
{
	return m_bits && IS_BLOCK_LIGHT_DIRTY_MASK == IS_BLOCK_LIGHT_DIRTY_MASK;
}

//  =========================================================================================
void Block::SetSkyFlag(bool isSky)
{
	if (isSky == true)
	{
		m_bits |= IS_BLOCK_SKY_MASK;
	}
	else
	{
		m_bits &= (~IS_BLOCK_SKY_MASK);
	}
}

//  =========================================================================================
void Block::SetLightingFlag(bool isLightingDirty)
{
	if (isLightingDirty == true)
	{
		m_bits |= IS_BLOCK_LIGHT_DIRTY_MASK;
	}
	else
	{
		m_bits &= (~IS_BLOCK_LIGHT_DIRTY_MASK);
	}
}
