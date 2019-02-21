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
	static BlockDefinition* GetDefinitionById(const uchar8 id);

public:
	std::string m_name = "";
	uchar8 m_type = 0;
	uint8_t m_state = UINT8_MAX;

	//texture coordinates for block
	IntVector2 m_frontTexCoords = IntVector2::ZERO;
	IntVector2 m_rightSideTexCoords = IntVector2::ZERO;
	IntVector2 m_backTexCoords = IntVector2::ZERO;
	IntVector2 m_leftSideTexCoords = IntVector2::ZERO;
	IntVector2 m_bottomTexCoords = IntVector2::ZERO;
	IntVector2 m_topTexCoords = IntVector2::ZERO;

	bool m_doesTarget = false;

	static std::map<uchar8, BlockDefinition*> s_blockDefinitions;
};