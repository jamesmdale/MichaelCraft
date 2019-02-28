#include "Game\Definitions\BlockDefinition.hpp"
#include "Engine\Core\EngineCommon.hpp"
#include "Engine\Core\StringUtils.hpp"

std::map<uchar8, BlockDefinition*> BlockDefinition::s_blockDefinitions;

//  =========================================================================================
BlockDefinition::BlockDefinition(const tinyxml2::XMLElement& element)
{
	m_type = ParseXmlAttribute(element, "id", m_type);
	m_name = ParseXmlAttribute(element, "name", m_name);

	//get default bit settings
	const tinyxml2::XMLElement* texcoordElement = element.FirstChildElement("BitSettings");
	if (texcoordElement)
	{
		m_isFullOpaque = ParseXmlAttribute(*texcoordElement, "isFullOpaque", m_isFullOpaque);
		m_isSolid = ParseXmlAttribute(*texcoordElement, "isSolid", m_isSolid);
		m_isVisible = ParseXmlAttribute(*texcoordElement, "isVisible", m_isVisible);

		//other bits are set dynamically
	}

	//get image value
	const tinyxml2::XMLElement* texcoordElement = element.FirstChildElement("TextureCoords");
	if (texcoordElement)
	{
		m_frontTexCoords = ParseXmlAttribute(*texcoordElement, "front", m_frontTexCoords);
		m_rightSideTexCoords = ParseXmlAttribute(*texcoordElement, "right", m_rightSideTexCoords);
		m_backTexCoords = ParseXmlAttribute(*texcoordElement, "back", m_backTexCoords);
		m_leftSideTexCoords = ParseXmlAttribute(*texcoordElement, "left", m_leftSideTexCoords);
		m_topTexCoords = ParseXmlAttribute(*texcoordElement, "top", m_topTexCoords);	
		m_bottomTexCoords = ParseXmlAttribute(*texcoordElement, "bottom", m_bottomTexCoords);		
	}

	ConstructBitsFromBools();
}

//  =========================================================================================
void BlockDefinition::Initialize(const std::string& filePath)
{
	tinyxml2::XMLDocument tileDefDoc;
	tileDefDoc.LoadFile(filePath.c_str());

	tinyxml2::XMLElement* pRoot = tileDefDoc.FirstChildElement();

	for (const tinyxml2::XMLElement* definitionNode = pRoot->FirstChildElement(); definitionNode; definitionNode = definitionNode->NextSiblingElement())
	{
		BlockDefinition* newDef = new BlockDefinition(*definitionNode);
		s_blockDefinitions.insert(std::pair<uchar8, BlockDefinition*>(newDef->m_type, newDef));
	}
}

//  =========================================================================================
BlockDefinition* BlockDefinition::GetDefinitionById(const uchar8 id)
{
	std::map<uchar8, BlockDefinition*>::iterator mapIterator = s_blockDefinitions.find(id);
	if (mapIterator == s_blockDefinitions.end()) {
		return nullptr;
	}
	else 
	{
		return mapIterator->second;
	}
}

//  =========================================================================================
void BlockDefinition::ConstructBitsFromBools()
{
	if (m_isFullOpaque)
		m_defaultBits |= IS_BLOCK_FULL_OPAQUE_MASK;

	if(m_isVisible)
		m_defaultBits |= IS_BLOCK_SOLID_MASK;

	if(m_isSolid)
		m_defaultBits |= IS_BLOCK_VISIBLE_MASK;
}


