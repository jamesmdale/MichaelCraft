#pragma once
#include "Engine\Core\XMLUtilities.hpp"
#include "Engine\ThirdParty\tinyxml2\tinyxml2.h"
#include "Game\GameCommon.hpp"
#include <string>
#include <map>
#include <vector>


class BlockDefinition
{
public:
	explicit BlockDefinition(const tinyxml2::XMLElement& element);
	static void Initialize(const std::string& filePath);
	//static BlockDefinition* GetDefinitionByName(const std::string& blockName);
	static BlockDefinition* GetDefinitionById(const uchar8 id);


public:
	std::string m_name = "";
	uchar8 m_type = 0;
	uint8_t m_state = UINT8_MAX;

	//texture coordinates for block
	AABB2 m_frontTexCoords = AABB2::ZERO_TO_ONE;
	AABB2 m_rightSideTexCoords = AABB2::ZERO_TO_ONE;
	AABB2 m_backTexCoords = AABB2::ZERO_TO_ONE;
	AABB2 m_leftSideTexCoords = AABB2::ZERO_TO_ONE;
	AABB2 m_bottomTexCoords = AABB2::ZERO_TO_ONE;
	AABB2 m_topTexCoords = AABB2::ZERO_TO_ONE;

	bool m_doesTarget = false;

	static std::map<uchar8, BlockDefinition*> s_blockDefinitions;
};