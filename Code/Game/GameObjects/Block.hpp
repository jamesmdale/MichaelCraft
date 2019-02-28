#pragma once
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Core\EngineCommon.hpp"
#include "Game\GameCommon.hpp"

class BlockDefinition;

class Block
{
public:
	Block();
	Block(BlockDefinition* definition);
	~Block();

public:
	//helpers for bitfields
	inline bool IsAir();
	inline bool IsOpaque();
	inline bool IsVisible();
	inline bool IsSolid();
	inline bool IsSky();
	inline bool IsLightingDirty();

	//setters for bit fields
	inline void SetSkyFlag(bool isSky);
	inline void SetLightingFlag(bool isLightingDirty); 

public:
	uchar8 m_type = 0; //max 255 types
	uchar8 m_bits = 0; //state
	//uchar8 m_lighting; //lighting state
};


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
	return (m_bits & IS_BLOCK_FULL_OPAQUE_MASK) == IS_BLOCK_FULL_OPAQUE_MASK;
}

//  =========================================================================================
bool Block::IsVisible()
{
	return (m_bits & IS_BLOCK_VISIBLE_MASK) == IS_BLOCK_VISIBLE_MASK;
}

//  =========================================================================================
bool Block::IsSolid()
{
	return (m_bits & IS_BLOCK_SOLID_MASK) == IS_BLOCK_SOLID_MASK;
}

//  =========================================================================================
bool Block::IsSky()
{
	return (m_bits & IS_BLOCK_SKY_MASK) == IS_BLOCK_SKY_MASK;
}

//  =========================================================================================
bool Block::IsLightingDirty()
{
	return (m_bits & IS_BLOCK_LIGHT_DIRTY_MASK) == IS_BLOCK_LIGHT_DIRTY_MASK;
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