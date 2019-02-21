#include "Game\GameObjects\Block.hpp"

//  =========================================================================================
Block::Block()
{

}

//  =========================================================================================
Block::~Block()
{

}

bool Block::IsAir()
{
	return false;
}

bool Block::IsOpaque()
{
	return false;
}
