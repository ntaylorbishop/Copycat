#pragma once


#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Utils/ErrorWarningAssert.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/Console/Command.hpp"
#include "Engine/Console/Console.hpp"

#if defined(TOOLS_BUILD)
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOCAL FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static String GetAttributeTypeName(FbxNodeAttribute::EType type) {

	return "";
}


//---------------------------------------------------------------------------------------------------------------------------
static void PrintAttribute(FbxNodeAttribute* attribute, int depth) {

	if (nullptr == attribute) {
		return;
	}
	
	FbxNodeAttribute::EType type = attribute->GetAttributeType();

	String typeName = GetAttributeTypeName(type);
	String attribName = attribute->GetName();

	String output = StringUtils::Stringf("%s- type ='%s', name='%s'", depth, "", typeName, attribName);
	Console::PrintOutput(output, CONSOLE_VERIFY);
}


//---------------------------------------------------------------------------------------------------------------------------
static void PrintNode(FbxNode* node, int depth) {

	String output = StringUtils::Stringf("%*sNode [%s]", depth, " ", node->GetName());
	Console::PrintOutput(output, CONSOLE_VERIFY);

	for (int i = 0; i < node->GetChildCount(); i++) {
		PrintNode(node->GetChild(i), depth + 1);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EXTERNAL FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FbxList(String filename) {

	FbxManager* fbxManager = FbxManager::Create();

	if (nullptr == fbxManager) {
		Console::PrintOutput("Could not create fbx manager.", CONSOLE_VERIFY);
		return;
	}

	FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT );

	fbxManager->SetIOSettings(ioSettings);

	//Create an importer
	FbxImporter* importer = FbxImporter::Create(fbxManager, "");
	bool result = importer->Initialize(filename.c_str(), -1, fbxManager->GetIOSettings());

	if (result) {
		//We have imported the FBX
		FbxScene* scene = FbxScene::Create(fbxManager, "");

		bool importSuccessful = importer->Import(scene);
		if (importSuccessful) {
			FbxNode* root = scene->GetRootNode();
			PrintNode(root, 0);
		}
		else {
			Console::PrintOutput("fuck", CONSOLE_VERIFY);
		}

		FBX_SAFE_DESTROY(scene);
	}
	else {
		String output = StringUtils::Stringf("Could not import scene: %s", filename.c_str());
		Console::PrintOutput(output, CONSOLE_VERIFY);
	}

	FBX_SAFE_DESTROY(importer);
	FBX_SAFE_DESTROY(ioSettings); 
	FBX_SAFE_DESTROY(fbxManager);
}


#else
	void FbxList(String) {}
#endif


//---------------------------------------------------------------------------------------------------------------------------
void FbxListScene(Command& args) {
	String name;
	args.GetNextString(name);

	String filepath = "Data/FBX/" + name + ".fbx";
	FbxList(filepath);
}