#include "Engine/Renderer/D3D11/General/D3D11ShaderFileParsing.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/Renderer/D3D11/Shaders/D3D11ShaderProgram.hpp"


//---------------------------------------------------------------------------------------------------------------------------
static void ParseDefaultConstantBuffer(eWhichShaderBound )


//---------------------------------------------------------------------------------------------------------------------------
static void ParseShaderXML(const XMLNode& shaderNode) {

	const char* shaderName = shaderNode.getAttribute(0).lpszValue;
	D3D11ShaderProgram* newProg = D3D11ShaderProgram::CreateOrGetShaderProgram(shaderName);

	for (uint i = 0; i < shaderNode.nChildNode(); i++) {

		XMLNode childNode = shaderNode.getChildNode(i);
		const char* nodeName = childNode.getName();
		
		if(strcmp(nodeName, "VertexShader"))
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseInShaderData(const String& shaderName) {

	XMLNode root = XMLNode::parseFile(shaderName.c_str());

	for (uint i = 0; i < root.nChildNode(); i++) {

		XMLNode shaderNode = root.getChildNode(i);
		ASSERT_OR_DIE(strcmp(shaderNode.getName(), "Shader") == 0, "Only shader nodes allowed when parsing shader data.");

		ParseShaderXML(shaderNode);
	}
}



//---------------------------------------------------------------------------------------------------------------------------
void ParseInAllShaderData() {

	std::vector<String> shaderFiles = FileUtils::EnumerateFiles("Data/ShaderDefinitions/", "*.xml", true);

	std::vector<String>::const_iterator shaderFileIt = shaderFiles.begin();
	for (shaderFileIt; shaderFileIt != shaderFiles.end(); ++shaderFileIt) {

		String shaderFilename = *shaderFileIt;
		ParseInShaderData(shaderFilename);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void ParseInAllMaterialData() {

}