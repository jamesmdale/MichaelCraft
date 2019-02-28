#pragma once
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Core\EngineCommon.hpp"

class BlockDefinition;

class Block
{
public:
	Block();
	Block(BlockDefinition* definition);
	~Block();

public:
	//helpers for bitfields
	bool IsAir();
	bool IsOpaque();
	bool IsVisible();
	bool IsSolid();
	bool IsSky();
	bool IsLightingDirty();

	//setters for bit fields
	void SetSkyFlag(bool isSky);
	void SetLightingFlag(bool isLightingDirty); 

public:
	uchar8 m_type = 0; //max 255 types
	uchar8 m_bits = 0; //state
	//uchar8 m_lighting; //lighting state
};