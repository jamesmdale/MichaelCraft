#pragma once
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Core\EngineCommon.hpp"
#include "Game\Definitions\BlockDefinition.hpp"
#include "Game\GameCommon.hpp"

class Block
{
public:
	Block();
	Block(BlockDefinition* definition);
	~Block();

	//helpers for bitfields
	inline bool IsAir();
	inline bool IsOpaque();
	inline bool IsVisible();
	inline bool IsSolid();
	inline bool IsSky();
	inline bool IsLightingInDirtyList();

	//helpers for lighting
	inline void SetIndoorLightingValue(const uchar8 lightingValue);
	inline void SetOutdoorLightingValue(const uchar8 lightingValue);
	inline uchar8 GetIndoorLightingValue();
	inline uchar8 GetOutDoorLightingValue();

	//setters for bit fields
	inline void SetSkyFlag(bool isSky);
	inline void SetLightingInDirtyListFlag(bool isLightingInDirtyList); 

public:
	uchar8 m_type = 0; //max 255 types
	uchar8 m_bits = 0; //state
	uchar8 m_lighting = 0; //lighting value
};


// global statics =========================================================================================
void SetBlockToType(Block* block, const uchar8 id);

//  =========================================================================================
//  INLINE FUNCTIONS
//  =========================================================================================
bool Block::IsAir()
{
	if(m_type == AIR_BLOCK_ID)
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
bool Block::IsLightingInDirtyList()
{
	return (m_bits & IS_BLOCK_IN_LIGHT_DIRTY_LIST_MASK) == IS_BLOCK_IN_LIGHT_DIRTY_LIST_MASK;
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
void Block::SetLightingInDirtyListFlag(bool isLightingDirty)
{
	if (isLightingDirty == true)
	{
		m_bits |= IS_BLOCK_IN_LIGHT_DIRTY_LIST_MASK;
	}
	else
	{
		m_bits &= (~IS_BLOCK_IN_LIGHT_DIRTY_LIST_MASK);
	}
}

//  =========================================================================================
void Block::SetIndoorLightingValue(const uchar8 lightingValue)
{
	//make sure the lighting value isn't greater than 15
	uchar8 lightingValueClamped = lightingValue & (~OUTDOOR_LIGHTING_MASK);

	m_lighting &= (~INDOOR_LIGHTING_MASK);
	m_lighting |= lightingValueClamped;
}

//  =========================================================================================
void Block::SetOutdoorLightingValue(const uchar8 lightingValue)
{
	uchar8 lightingValueShifted = lightingValue << BITS_WIDE_INDOOR_LIGHTING_MASK;

	m_lighting &= (~OUTDOOR_LIGHTING_MASK);
	m_lighting |= lightingValueShifted;
}

//  =========================================================================================
uchar8 Block::GetIndoorLightingValue()
{
	uchar8 indoorLightingValue = m_lighting & (~OUTDOOR_LIGHTING_MASK);
	return indoorLightingValue;
}

//  =========================================================================================
uchar8 Block::GetOutDoorLightingValue()
{
	uchar8 outDoorLightingValue = m_lighting >> BITS_WIDE_INDOOR_LIGHTING_MASK;
	return outDoorLightingValue;
}