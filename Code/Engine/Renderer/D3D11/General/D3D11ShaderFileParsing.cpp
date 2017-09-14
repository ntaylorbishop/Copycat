#include "Engine/Renderer/D3D11/General/D3D11ShaderFileParsing.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/Renderer/D3D11/Shaders/D3D11ShaderProgram.hpp"
#include "Engine/Math/Matrix44.hpp"


bool	D3D11_BOOL_FALSE	= false;
bool	D3D11_BOOL_TRUE		= true;
float	D3D11_FLOAT_ZERO	= 0.f;
double	DOUBLE_ZERO			= 0.0;
int		INT_ZERO			= 0;
uint	UINT_ZERO			= 0;
short	SHORT_ZERO			= 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static void ParseDefaultConstantBuffers(D3D11ShaderProgram* newProg, const XMLNode& cBufferNode) {

	for (int i = 0; i < cBufferNode.nChildNode(); i++) {

		XMLNode cBuffer = cBufferNode.getChildNode(i);


	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseConstantBuffer(D3D11ShaderProgram* newProg, const XMLNode& cBufferNode) {

}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseShaderResources(D3D11ShaderProgram* newProg, const XMLNode& cBufferNode) {

}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseVertexShader(D3D11ShaderProgram* newProg, const XMLNode& vertexShaderNode) {

}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseFragmentShader(D3D11ShaderProgram* newProg, const XMLNode& vertexShaderNode) {

}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseShaderXML(const XMLNode& shaderNode) {

	const char* shaderName = shaderNode.getAttribute(0).lpszValue;
	D3D11ShaderProgram* newProg = D3D11ShaderProgram::CreateOrGetShaderProgram(shaderName);

	for (int i = 0; i < shaderNode.nChildNode(); i++) {

		XMLNode childNode = shaderNode.getChildNode(i);
		const char* nodeName = childNode.getName();
		
		if (strcmp(nodeName, "VertexShader") == 0) {
			ParseVertexShader(newProg, childNode);
		}
		else if (strcmp(nodeName, "FragmentShader") == 0) {
			ParseFragmentShader(newProg, childNode);
		}
		else if (strcmp(nodeName, "ShaderResourceView") == 0) {
			ParseShaderResources(newProg, childNode);
		}
		else if (strcmp(nodeName, "DefaultConstantBuffers") == 0) {
			ParseDefaultConstantBuffers(newProg, childNode);
		}
		else if (strcmp(nodeName, "ConstantBuffer") == 0) {
			ParseConstantBuffer(newProg, childNode);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseInShaderData(const String& shaderName) {

	XMLNode root = XMLNode::parseFile(shaderName.c_str());

	for (int i = 0; i < root.nChildNode(); i++) {

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ParseInAllMaterialData() {

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static void* GetDefaultUniformValue(const String& uniValue, eUniformType uniType) {

	//WARNING: THESE NEED TO BE UPDATED WHEN THEY'RE IN
	if (uniValue == "defaultProj") {
		return nullptr;
	}
	else if (uniValue == "defaultProjUI") {
		return nullptr;
	}
	else if (uniValue == "cameraPos") {
		return nullptr;
	}
	else if (uniValue == "defaultView") {
		return nullptr;
	}
	else if (uniValue == "screenSizeX") {
		return nullptr;
	}
	else if (uniValue == "screenSizeY") {
		return nullptr;
	}
	else if (uniValue == "screenSize") {
		return nullptr;
	}
	else if (uniValue == "defaultNear") {
		return nullptr;
	}
	else if (uniValue == "defaultFar") {
		return nullptr;
	}
	else if (uniValue == "defaultDepth") {
		return nullptr;
	}
	else if (uniValue == "blank") {
		return nullptr;
	}
	else if (uniValue == "defaultColorTarget") {
		return nullptr;
	}
	else if (uniValue == "tileSizePixels") {
		return nullptr;
	}
	else if (uniValue == "maxLightsPerTile") {
		return nullptr;
	}
	else if (uniValue == "defaultNormals") {
		return nullptr;
	}
	else if (uniValue == "false") {
		return (void*)&D3D11_BOOL_FALSE;
	}
	else if (uniValue == "true") {
		return (void*)&D3D11_BOOL_TRUE;
	}
	else if (uniValue == "default") {
		switch (uniType) {
		case UNIFORM_FLOAT: {
			return (void*)&D3D11_FLOAT_ZERO;
			break;
		}
		case UNIFORM_DOUBLE: {
			return (void*)&DOUBLE_ZERO;
			break;
		}
		case UNIFORM_VECTOR3: {
			return (void*)&Vector3::ZERO;
			break;
		}
		case UNIFORM_VECTOR2: {
			return (void*)&Vector2::ZERO;
			break;
		}
		case UNIFORM_VECTOR4: {
			return (void*)&Vector4::ZERO;
			break;
		}
		case UNIFORM_MAT4: {
			return (void*)&Matrix44::IDENTITY;
			break;
		}
		case UNIFORM_BOOL: {
			return (void*)&BOOL_FALSE;
			break;
		}
		case UNIFORM_INT: {
			return (void*)&INT_ZERO;
			break;
		}
		case UNIFORM_UINT: {
			return (void*)&UINT_ZERO;
			break;
		}
		case UNIFORM_SHORT: {
			return (void*)&SHORT_ZERO;
			break;
		}
		case UNIFORM_RGBA: {
			return (void*)&RGBA::WHITE;
			break;
		}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
static void ParseDefaultConstantBuffer(const XMLNode& cBufferNode) {

	ASSERT_OR_DIE(strcmp(cBufferNode.getName(), "ConstantBuffer") == 0, "ERROR: Constant buffers in default constant buffers file need to be named properly.");
	
	String cBufferName = cBufferNode.getAttribute(0).lpszValue;
	int cBufferSize = XMLUtils::ParseInt(cBufferNode.getAttribute(1).lpszValue);

	D3D11ConstantBuffer* newBuffer = D3D11ConstantBuffer::CreateOrGetConstantBuffer(cBufferName, cBufferSize);

	for (uint i = 0; i < cBufferNode.nChildNode(); i++) {

		XMLNode bufferUniNode = cBufferNode.getChildNode(i);

		ASSERT_OR_DIE(strcmp(bufferUniNode.getName(), "Uniform") == 0, "ERROR: Constant buffers can only contain uniforms in XML.");

		String uniName = bufferUniNode.getAttribute(0).lpszValue;
		String uniTypeStr = bufferUniNode.getAttribute(1).lpszValue;
		String uniValStr = bufferUniNode.getAttribute(2).lpszValue;
		eUniformType uniType = D3D11Uniform::UnserializeType(uniTypeStr);
		void* defaultData = GetDefaultUniformValue(uniValStr, uniType);

		D3D11Uniform* newUni = new D3D11Uniform(uniName.c_str(), uniType, defaultData);
		newBuffer->AddUniform(newUni);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void ParseInDefaultConstantBuffers() {

	size_t size = sizeof(Matrix44);

	const char* cBuffersFilename = "Data/ShaderDefinitions/DefaultConstantBuffers.xml";
	XMLNode root = XMLNode::parseFile(cBuffersFilename);
	root = root.getChildNode(0);

	for (uint i = 0; i < root.nChildNode(); i++) {

		XMLNode cBufferNode = root.getChildNode(i);

		ParseDefaultConstantBuffer(cBufferNode);
	}

	int a = 0;
}