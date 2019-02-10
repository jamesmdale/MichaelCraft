#pragma once
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Core\EngineCommon.hpp"

class Block
{
public:
	Block();
	~Block();

public:
	uchar8 m_type; //max 255 types
	uchar8 m_bits; //state
	//uchar8 m_lighting; //lighting state
};