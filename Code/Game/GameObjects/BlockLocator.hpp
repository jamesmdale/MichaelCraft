#pragma once

class Chunk;
class Block;

class BlockLocator
{
public:
	BlockLocator();
	BlockLocator(Chunk* chunk, int blockIndex);
	~BlockLocator();

	Block* GetBlock();
	BlockLocator GetBlockLocatorToNorth();
	BlockLocator GetBlockLocatorToWest();
	BlockLocator GetBlockLocatorToSouth();
	BlockLocator GetBlockLocatorToEast();
	BlockLocator GetBlockLocatorAbove();
	BlockLocator GetBlockLocatorBelow();


public:
	Chunk* m_chunk = nullptr;
	int m_blockIndex;

};

