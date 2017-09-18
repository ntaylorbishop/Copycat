#include "Engine/Renderer/D3D11/General/D3D11ShaderFileParsing.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/Renderer/D3D11/Shaders/D3D11ShaderProgram.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Renderer/D3D11/Texture/Texture2D.hpp"
#include "Engine/Renderer/D3D11/Shaders/D3D11PixelShader.hpp"


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
		}
		case UNIFORM_DOUBLE: {
			return (void*)&DOUBLE_ZERO;
		}
		case UNIFORM_VECTOR3: {
			return (void*)&Vector3::ZERO;
		}
		case UNIFORM_VECTOR2: {
			return (void*)&Vector2::ZERO;
		}
		case UNIFORM_VECTOR4: {
			return (void*)&Vector4::ZERO;
		}
		case UNIFORM_MAT4: {
			return (void*)&Matrix44::IDENTITY;
		}
		case UNIFORM_BOOL: {
			return (void*)&BOOL_FALSE;
		}
		case UNIFORM_INT: {
			return (void*)&INT_ZERO;
		}
		case UNIFORM_UINT: {
			return (void*)&UINT_ZERO;
		}
		case UNIFORM_SHORT: {
			return (void*)&SHORT_ZERO;
		}
		case UNIFORM_RGBA: {
			return (void*)&RGBA::WHITE;
		}
		default: {
			return nullptr;
		}
		}
	}
	else {
		return nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseDefaultConstantBuffer(const XMLNode& cBufferNode) {

	String cBufferName = cBufferNode.getAttribute(0).lpszValue;
	int cBufferSize = XMLUtils::ParseInt(cBufferNode.getAttribute(1).lpszValue);

	D3D11ConstantBuffer* newBuffer = D3D11ConstantBuffer::CreateOrGetConstantBuffer(cBufferName, cBufferSize);

	for (int i = 0; i < cBufferNode.nChildNode(); i++) {

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
static void ParseDefaultConstantBuffer(D3D11ShaderProgram* newProg, const XMLNode& cBufferNode, eWhichShaderBound whichShader) {

	String cBufferName = cBufferNode.getAttribute(0).lpszValue;
	String bindPointStr = cBufferNode.getAttribute(1).lpszValue;
	int bindPoint = XMLUtils::ParseInt(bindPointStr);

	D3D11ConstantBuffer* defBuffer = D3D11ConstantBuffer::GetConstantBuffer(cBufferName);
	ASSERT_OR_DIE(defBuffer != nullptr, "ERROR: No default buffer exists by that name.");
	newProg->AddConstantBuffer(bindPoint, defBuffer, whichShader);
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseProgramDefaultConstantBuffers(D3D11ShaderProgram* newProg, const XMLNode& cBuffersNode, eWhichShaderBound whichShader) {

	for (int i = 0; i < cBuffersNode.nChildNode(); i++) {

		XMLNode cBufferNode = cBuffersNode.getChildNode(i);
		ParseDefaultConstantBuffer(newProg, cBufferNode, whichShader);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseConstantBuffer(D3D11ShaderProgram* newProg, const XMLNode& cBufferNode, eWhichShaderBound whichShader) {

	String cBufferName = cBufferNode.getAttribute(0).lpszValue;
	int cBufferSize = XMLUtils::ParseInt(cBufferNode.getAttribute(1).lpszValue);
	int cBufferBindPoint = XMLUtils::ParseInt(cBufferNode.getAttribute(2).lpszValue);

	D3D11ConstantBuffer* newBuffer = D3D11ConstantBuffer::CreateOrGetConstantBuffer(cBufferName, cBufferSize);

	for (int i = 0; i < cBufferNode.nChildNode(); i++) {

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

	newProg->AddConstantBuffer(cBufferBindPoint, newBuffer, whichShader);
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseShaderResource(D3D11ShaderProgram* newProg, const XMLNode& cBufferNode, eWhichShaderBound whichShader) {

	String bindPointStr = cBufferNode.getAttribute(0).lpszValue;
	int bindPoint = XMLUtils::ParseInt(bindPointStr);
	

	Texture2D* defaultTex = Texture2D::GetTexture("Data/Textures/Brick2.png");
	D3D11Resource* texID = defaultTex->GetSRVResource();
	newProg->AddResource(bindPoint, texID, whichShader);
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseSampler(D3D11ShaderProgram* newProg, const XMLNode& samplerNode, eWhichShaderBound whichShader) {

	String samplerName = samplerNode.getAttribute(0).lpszValue;
	String bindPointStr = samplerNode.getAttribute(1).lpszValue;
	int bindPoint = XMLUtils::ParseInt(bindPointStr);
	D3D11SamplerState* sampler = D3D11SamplerState::GetSampler(samplerName);

	newProg->AddSampler(bindPoint, sampler, whichShader);
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseVertexShader(D3D11ShaderProgram* newProg, const XMLNode& vertexShaderNode, eVertexType vertType) {

	String vertPath = vertexShaderNode.getAttribute(0).lpszValue;
	D3D11VertexShader* vertShader = (D3D11VertexShader*)D3D11Shader::CreateOrGetShader(vertPath, D3D11SHADERTYPE_VERTEX);
	newProg->SetVertexShader(vertShader);

	vertShader->BindVertexLayoutToDeviceWindow(vertType);

	for (int i = 0; i < vertexShaderNode.nChildNode(); i++) {

		XMLNode childNode = vertexShaderNode.getChildNode(i);
		String nodeName = childNode.getName();

		if (nodeName == "ConstantBuffer") {
			ParseConstantBuffer(newProg, childNode, WHICH_SHADER_VERTEX);
		}
		else if (nodeName == "ShaderResourceView") {
			ParseShaderResource(newProg, childNode, WHICH_SHADER_VERTEX);
		}
		else if (nodeName == "Sampler") {
			ParseSampler(newProg, childNode, WHICH_SHADER_VERTEX);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseFragmentShader(D3D11ShaderProgram* newProg, const XMLNode& fragShaderNode) {

	String fragShaderPath = fragShaderNode.getAttribute(0).lpszValue;
	D3D11PixelShader* pixShader = (D3D11PixelShader*)D3D11Shader::CreateOrGetShader(fragShaderPath, D3D11SHADERTYPE_VERTEX);
	newProg->SetPixelShader(pixShader);

	for (int i = 0; i < fragShaderNode.nChildNode(); i++) {

		XMLNode childNode = fragShaderNode.getChildNode(i);
		String nodeName = childNode.getName();

		if (nodeName == "ConstantBuffer") {
			ParseConstantBuffer(newProg, childNode, WHICH_SHADER_FRAGMENT);
		}
		else if (nodeName == "ShaderResourceView") {
			ParseShaderResource(newProg, childNode, WHICH_SHADER_FRAGMENT);
		}
		else if (nodeName == "Sampler") {
			ParseSampler(newProg, childNode, WHICH_SHADER_FRAGMENT);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseShaderXML(D3D11ShaderProgram* newProg, const XMLNode& shaderNode, eVertexType vertType) {

	const char* nodeName = shaderNode.getName();

	if (strcmp(nodeName, "VertexShader") == 0) {
		ParseVertexShader(newProg, shaderNode, vertType);
	}
	else if (strcmp(nodeName, "FragmentShader") == 0) {
		ParseFragmentShader(newProg, shaderNode);
	}
	else if (strcmp(nodeName, "ShaderResourceView") == 0) {
		ParseShaderResource(newProg, shaderNode, WHICH_SHADER_BOTH);
	}
	else if (strcmp(nodeName, "DefaultConstantBuffers") == 0) {
		ParseProgramDefaultConstantBuffers(newProg, shaderNode, WHICH_SHADER_BOTH);
	}
	else if (strcmp(nodeName, "ConstantBuffer") == 0) {
		ParseConstantBuffer(newProg, shaderNode, WHICH_SHADER_BOTH);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseInShaderData(const String& shaderName) {

	XMLNode root = XMLNode::parseFile(shaderName.c_str());
	root = root.getChildNode(0);

	const char* shaderProgName = root.getAttribute(0).lpszValue;

	if (shaderProgName == nullptr) {
		return; //Isn't a shader file
	}

	D3D11ShaderProgram* newProg = D3D11ShaderProgram::CreateOrGetShaderProgram(shaderProgName);

	String vertTypeStr = root.getAttribute(1).lpszValue;
	eVertexType vertType = ParseVertexTypeFromString(vertTypeStr);

	for (int i = 0; i < root.nChildNode(); i++) {

		XMLNode shaderNode = root.getChildNode(i);
		ParseShaderXML(newProg, shaderNode, vertType);
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
void ParseInDefaultConstantBuffers() {

	const char* cBuffersFilename = "Data/ShaderDefinitions/DefaultConstantBuffers.xml";
	XMLNode root = XMLNode::parseFile(cBuffersFilename);
	root = root.getChildNode(0);

	for (int i = 0; i < root.nChildNode(); i++) {

		XMLNode cBufferNode = root.getChildNode(i);

		ParseDefaultConstantBuffer(cBufferNode);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void CreateDefaultSamplers() {

	D3D11SamplerState* linearSampler = new D3D11SamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_NEVER);
	D3D11SamplerState::AddSamplerToRegistry(linearSampler, "linear");
}