#pragma once
#include <vector>

class Chunk;
class Block;

class BlockLocator
{
public:
	BlockLocator();
	BlockLocator(Chunk* chunk, int blockIndex);
	~BlockLocator();

	Block* GetBlock();

	void StepNorth();
	void StepWest();
	void StepSouth();
	void StepEast();
	void StepUp();
	void StepDown();

	BlockLocator GetBlockLocatorToNorth();
	BlockLocator GetBlockLocatorToWest();
	BlockLocator GetBlockLocatorToSouth();
	BlockLocator GetBlockLocatorToEast();
	BlockLocator GetBlockLocatorAbove();
	BlockLocator GetBlockLocatorBelow();
	bool IsValid();

	bool IsBlockIndexOnEdge(std::vector<Chunk*>& outConnectingChunks);
	bool IsBlockIndexOnEdgeFast();

public:
	Chunk* m_chunk = nullptr;
	int m_blockIndex = -1;
};

