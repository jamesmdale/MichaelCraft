#pragma once
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Core\EngineCommon.hpp"

class Block
{
public:
	Block();
	~Block();

public:
	bool IsAir();
	bool IsOpaque();

public:
	uchar8 m_type = 0; //max 255 types
	uchar8 m_bits = 0; //state
	//uchar8 m_lighting; //lighting state
};