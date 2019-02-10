#include "Game\Definitions\BlockDefinition.hpp"
#include "Engine\Core\EngineCommon.hpp"
#include "Engine\Core\StringUtils.hpp"

std::map<uchar8, BlockDefinition*> BlockDefinition::s_blockDefinitions;

//  =========================================================================================
BlockDefinition::BlockDefinition(const tinyxml2::XMLElement& element)
{
	m_type = ParseXmlAttribute(element, "id", m_type);
	m_name = ParseXmlAttribute(element, "name", m_name);

	/*std::string type = "";
	type = ParseXmlAttribute(element, "type", type);*/
	//m_type = ConvertTypeToCardTypeEnum(type);	

	//m_subType = ParseXmlAttribute(element, "subtype", m_subType);

	//m_id = ParseXmlAttribute(element, "id", m_id);

	//get image value
	/*const tinyxml2::XMLElement* imageElement = element.FirstChildElement("Image");
	if (imageElement)
	{
		std::string fileName = "";
		fileName = ParseXmlAttribute(*imageElement, "file", fileName);

		m_imagePath = Stringf("Data/Images/Cards/%s", fileName.c_str());
	}*/
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
//BlockDefinition* BlockDefinition::GetDefinitionByName(const std::string& cardName)
//{
//	std::map<std::string, BlockDefinition*>::iterator mapIterator = s_blockDefinitions.find(cardName);
//	if (mapIterator == s_blockDefinitions.end()) {
//		return nullptr;
//	}
//	else 
//	{
//		return mapIterator->second;
//	}
//}

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


