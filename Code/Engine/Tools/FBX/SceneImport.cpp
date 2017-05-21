#include "Engine/Tools/FBX/SceneImport.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
SceneImport::SceneImport(String filename, float scale, float fps) 
	//: m_meshes()
	: m_importSuccessful(false)
	, m_pointToDraw(Vector3(0.f, 0.f, 10.f))
	, m_foundAPoint(false)
	, m_fps(fps)
	, m_ind(0)
{ 
	m_importSuccessful = FbxLoadSceneFromFile(filename, Matrix4::BASIS, true, Matrix4::MakeScaleMatrix(scale));	
}
SceneImport::~SceneImport() { }

//---------------------------------------------------------------------------------------------------------------------------
//LOADER
//---------------------------------------------------------------------------------------------------------------------------
#if defined(TOOLS_BUILD)
bool SceneImport::FbxLoadSceneFromFile(String filename, Matrix4 engineBasis, bool isEngineBasisRightHanded, Matrix4 transform) {
	FbxManager* fbxManager = FbxManager::Create();

	ASSERT_OR_DIE(nullptr != fbxManager, "Something went wrong. FbxManager could not be created.");

	FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);

	fbxManager->SetIOSettings(ioSettings);

	FbxImporter* importer = FbxImporter::Create(fbxManager, "");
	bool result = importer->Initialize(filename.c_str(), -1, fbxManager->GetIOSettings());

	if (result) {
		FbxScene* scene = FbxScene::Create(fbxManager, "");
		bool importSuccessful = importer->Import(scene);
		if (importSuccessful) {
			MatrixStack44 matStack;

			matStack.Push(transform);
			matStack.Push(Matrix4::BASIS);

			//Set up initial transforms
			Matrix4 sceneBasis = GetSceneBasis(scene);
			sceneBasis.Transpose();

			if (!isEngineBasisRightHanded) {
				Vector3 forward = sceneBasis.GetForward();
				forward = -1.f * forward;

				sceneBasis.SetForward(forward);
			}

			matStack.Push(sceneBasis);

			ImportScene(scene, matStack);

			//m_meshBuilder.Reduce();
			//CombineMeshesAndUpdate();
		}
		else {
			Console::PrintOutput("Could not import scene.", CONSOLE_VERIFY);
			return false;
		}

		FBX_SAFE_DESTROY(scene);
	}
	else {
		String output = StringUtils::Stringf("Could not import scene from file: %s", filename.c_str());
		Console::PrintOutput(output, CONSOLE_VERIFY);
		return false;
	}

	FBX_SAFE_DESTROY(importer);
	FBX_SAFE_DESTROY(ioSettings);
	FBX_SAFE_DESTROY(fbxManager);

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//IMPORTING MESHES
//---------------------------------------------------------------------------------------------------------------------------
void SceneImport::TriangulateScene(FbxScene* scene) {
	FbxGeometryConverter converter(scene->GetFbxManager());

	converter.Triangulate(scene, true);
}
//---------------------------------------------------------------------------------------------------------------------------
void SceneImport::ImportScene(FbxScene* scene, MatrixStack44& matStack) {

	TriangulateScene(scene);

	FbxNode* root = scene->GetRootNode();

	ImportSkeletons(root, matStack, nullptr, -1);
	ImportSceneMeshes(root, matStack);
	ImportMotions(matStack.Top(), scene, m_fps);
}


//---------------------------------------------------------------------------------------------------------------------------
void SceneImport::ImportSceneMeshes(FbxNode* node, MatrixStack44& matStack) {
	if (nullptr == node) {
		return;
	}

	Matrix4 nodeLocalTransform = GetNodeTransform(node);
	matStack.Push(nodeLocalTransform);

	//Load meshes
	int attribCount = node->GetNodeAttributeCount();
	for (int attribIdx = 0; attribIdx < attribCount; attribIdx++) {
		ImportSceneAttribute(node, node->GetNodeAttributeByIndex(attribIdx), matStack);
	}

	//Import Children
	int childCount = node->GetChildCount();
	for (int childIdx = 0; childIdx < childCount; childIdx++) {
		ImportSceneMeshes(node->GetChild(childIdx), matStack);
	}

	matStack.Pop();
}
//---------------------------------------------------------------------------------------------------------------------------
void SceneImport::ImportSceneAttribute(FbxNode* node, FbxNodeAttribute* attrib, MatrixStack44& matStack) {
	if (nullptr == attrib) {
		return;
	}

	switch (attrib->GetAttributeType()) {
	case FbxNodeAttribute::eMesh:
		GetMaterialName(node);
		ImportMesh((FbxMesh*)attrib, matStack);
		break;
		//Switch on lights, skeletons, etc.
	default:
		break;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void SceneImport::GetMaterialName(FbxNode* node) {

	FbxSurfaceMaterial* mat = node->GetMaterial(0);
	if (mat) {

		String matName = mat->GetName();
		m_meshBuilder.AddMaterialName(matName);
		DebuggerPrintf("%s\n", matName.c_str());
	}
}



//---------------------------------------------------------------------------------------------------------------------------
void SceneImport::ImportMesh(FbxMesh* mesh, MatrixStack44& matStack) {
	m_meshBuilder.AddMesh();

	Matrix4 geoTransform = GetGeometricTransform(mesh->GetNode());
	matStack.Push(geoTransform);
	//m_meshBuilder.AddPosition(geoTransform.GetPosition());
	Matrix4 transform = matStack.Top();

	ASSERT_OR_DIE(mesh->IsTriangleMesh(), "Cannot import mesh.");

	int controlPointCount = mesh->GetControlPointsCount();
	std::vector<SkinWeight> skinWeights;
	skinWeights.resize(controlPointCount);
	if (HasSkinWeights(mesh)) {
		GetSkinWeights(skinWeights, mesh);
	}
	else {
		//FbxNode *node = mesh->GetNode();
		// walk tree up till node associated with joint. 
		// Find first parent node that has a joint associated with it
		// all vertices (fully weighted)
		// all skin_weights = indices{ joint_idx, 0, 0, 0 }  weights{ 1.0f, 0.0f, 0.0f, 0.0f };

		FbxNode* node = mesh->GetNode();
		int idx = -2;
		for (unsigned int i = 0; i < m_fbxNodeJoints.size(); i++) {
			if (m_fbxNodeJoints[i] == node) {
				idx = i;
			}
		}

		if (idx != -2) {
			for (unsigned int i = 0; i < skinWeights.size(); i++) {
				skinWeights[i].m_inds		= IntVector4(idx, 0, 0, 0);
				skinWeights[i].m_weights	= Vector4(1.f, 0.f, 0.f, 0.f);
			}
		}
		else {
			for (unsigned int i = 0; i < skinWeights.size(); i++) {
				skinWeights[i].m_inds		= IntVector4(0, 0, 0, 0);
				skinWeights[i].m_weights	= Vector4(1.f, 0.f, 0.f, 0.f);
			}
		}
	}



	int polyCount = mesh->GetPolygonCount();
	for (int polyIdx = 0; polyIdx < polyCount; polyIdx++) {
		int vertexCount = mesh->GetPolygonSize(polyIdx);
		ASSERT_OR_DIE(vertexCount == 3, "ERROR: COULDN'T LOAD MESH");

		for (int vertIdx = 0; vertIdx < vertexCount; vertIdx++) {

			ImportVertex(transform, mesh, polyIdx, vertIdx, skinWeights);
		}
	}


	matStack.Pop();
	m_meshBuilder.CloseMesh();
	m_ind = 0;
}
//---------------------------------------------------------------------------------------------------------------------------
void SceneImport::ImportVertex(Matrix4& transform, FbxMesh* fbxMesh, int polyIdx, int vertIdx, std::vector<SkinWeight>& skinWeights) {
	Vector3 position;
	RGBA color = RGBA::WHITE;
	Vector2 uv;
	Vector3 tan;
	Vector3 bitan;
	Vector3 normal;

	Vector4 weights = Vector4(1.f, 0.f, 0.f, 0.f);
	IntVector4 inds = IntVector4(0, 0, 0, 0);

	int controlIdx = fbxMesh->GetPolygonVertex(polyIdx, vertIdx);

	GetColor(&color, fbxMesh, polyIdx, vertIdx);
	GetUV(&uv, fbxMesh, polyIdx, vertIdx, 0);
	GetPosition(&position, transform, fbxMesh, polyIdx, vertIdx);

	if (!GetTangent(&tan, fbxMesh, polyIdx, vertIdx, transform) || !GetBitangent(&bitan, fbxMesh, polyIdx, vertIdx, transform)) {
		GetNormal(&normal, fbxMesh, polyIdx, vertIdx, transform);
		//ConstructTBN(normal, &tan, &bitan);
	}
	else {
		m_meshBuilder.DontBuildMikkt();
	}

	if (controlIdx < skinWeights.size()) {
		weights = skinWeights[controlIdx].m_weights;
		inds = skinWeights[controlIdx].m_inds;
	}
	
	m_meshBuilder.AddVertex(TexturedVertex_TBNBN(position, color, uv, tan, bitan, normal, weights, inds));
	m_meshBuilder.AddInd(m_ind++);
}

//---------------------------------------------------------------------------------------------------------------------------
//SKINNING
//---------------------------------------------------------------------------------------------------------------------------
bool SceneImport::HasSkinWeights(FbxMesh* mesh) {
	int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	return (deformerCount > 0);
}
void SceneImport::AddHighestWeight(SkinWeight* skinWeights, int jointIdx, float weight) {
	//EXERCISE TO THE READER
	//Replace lowest with this if its higher

	float currWeight = skinWeights->m_weights.x;
	int indOfLowestWeight = 0;

	//Find lowest weight
	if (currWeight > skinWeights->m_weights.y) {
		currWeight = skinWeights->m_weights.y;
		indOfLowestWeight = 1;
	}

	if (currWeight > skinWeights->m_weights.z) {
		currWeight = skinWeights->m_weights.z;
		indOfLowestWeight = 2;
	}

	if (currWeight > skinWeights->m_weights.w) {
		currWeight = skinWeights->m_weights.w;
		indOfLowestWeight = 3;
	}

	switch (indOfLowestWeight) {
	case 0:
		if (weight >= skinWeights->m_weights.x) {
			skinWeights->m_weights.x = weight;
			skinWeights->m_inds.x = jointIdx;
		}
		break;
	case 1:
		if (weight >= skinWeights->m_weights.y) {
			skinWeights->m_weights.y = weight;
			skinWeights->m_inds.y = jointIdx;
		}
		break;
	case 2:
		if (weight >= skinWeights->m_weights.z) {
			skinWeights->m_weights.z = weight;
			skinWeights->m_inds.z = jointIdx;
		}
		break;
	case 3:
		if (weight >= skinWeights->m_weights.w) {
			skinWeights->m_weights.w = weight;
			skinWeights->m_inds.w = jointIdx;
		}
		break;
	}
}
int SceneImport::GetJointIndexForNode(FbxNode* node) {
	for (unsigned int i = 0; i < m_fbxNodeJoints.size(); i++) {
		if (node == m_fbxNodeJoints[i]) {
			return i;
		}
	}

	return -2;
}

void SceneImport::GetSkinWeights(std::vector<SkinWeight>& skinWeights, FbxMesh* mesh) {
	size_t controlPointCount = mesh->GetControlPointsCount();

	for (int i = 0; i < skinWeights.size(); i++) {
		skinWeights[i].m_inds = IntVector4(0, 0, 0, 0);
		skinWeights[i].m_weights = Vector4(0.f, 0.f, 0.f, 0.f);
	}


	int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < deformerCount; i++) {
		FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(i, FbxDeformer::eSkin);

		if (!skin) {
			continue;
		}

		//Clusters are a link between this skin object, bones, and the verts that bone affects
		int clusterCount = skin->GetClusterCount();
		for (int j = 0; j < clusterCount; j++) {
			FbxCluster* cluster = skin->GetCluster(j);
			FbxNode* linkNode = cluster->GetLink();

			if (!linkNode) {
				continue;
			}

			int jointIdx = GetJointIndexForNode(linkNode); //ou guys should have something like this
			if (jointIdx == -2) {
				continue;
			}

			int* controlPointIndices = cluster->GetControlPointIndices();
			int idxCount = cluster->GetControlPointIndicesCount();
			double* weights = cluster->GetControlPointWeights();

			for (int k = 0; k < idxCount; k++) {
				int controlIdx = controlPointIndices[k];
				double weight = weights[k];

				SkinWeight* skinWeight = &skinWeights[controlIdx];

				AddHighestWeight(skinWeight, jointIdx, (float)weight);
			}
		}
	}


	for (int i = 0; i < skinWeights.size(); i++) {
		SkinWeight& currWeight = skinWeights[i];

		float sum = currWeight.m_weights.x + currWeight.m_weights.y + currWeight.m_weights.z + currWeight.m_weights.w;

		if (sum > 0) {
			currWeight.m_weights.x /= sum;
			currWeight.m_weights.y /= sum;
			currWeight.m_weights.z /= sum;
			currWeight.m_weights.w /= sum;
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
//IMPORTING MOTIONS
//---------------------------------------------------------------------------------------------------------------------------
void SceneImport::ImportMotions(Matrix4& importTransform, FbxScene* scene, float framerate) {
	//Get number of animations
	int animCount = scene->GetSrcObjectCount<FbxAnimStack>();
	if (0 == animCount || 0 == m_skeletons.size()) {
		return;
	}

	//Timing info for animation in this scene
	FbxGlobalSettings& settings = scene->GetGlobalSettings();
	FbxTime::EMode timeMode = settings.GetTimeMode();

	double sceneFramerate;

	if (timeMode == FbxTime::eCustom) {
		sceneFramerate = settings.GetCustomFrameRate();
	}
	else {
		sceneFramerate = framerate;
	}

	//Only supporting motion for one skeleton right now, update when needed
	uint32_t skeletonCount = m_skeletons.size();
	ASSERT_OR_DIE(skeletonCount == 1, "ERROR: FBX CONTAINS MORE THAN 1 SKELETON");

	//Time between frames
	FbxTime advance;
	advance.SetSecondDouble((double)(1.f / framerate));

	for (int animIdx = 0; animIdx < animCount; animIdx++) {
		FbxAnimStack* anim = scene->GetSrcObject<FbxAnimStack>();

		if (nullptr == anim) {
			continue;
		}

		//Get duration of this specific animation.
		FbxTime startTime = anim->LocalStart;
		FbxTime endTime = anim->LocalStop;
		FbxTime duration = endTime - startTime;

		scene->SetCurrentAnimationStack(anim);

		String motionName = anim->GetName();

		float timeSpan = (float)duration.GetSecondDouble();
		Motion* motion = new(MEMORY_RENDERING) Motion(motionName, timeSpan, framerate, m_skeletons[0]);

		int jointCount = m_skeletons[0]->GetJointCount();
		for (int jointIdx = 0; jointIdx < jointCount; jointIdx++) {
			FbxNode* node = m_fbxNodeJoints[jointIdx];

			FbxTime evalTime = FbxTime(0);
			
			std::vector<Matrix4>* boneKeyframes = motion->GetJointKeyframes(jointIdx);
			for (int frameIdx = 0; frameIdx < motion->m_frameCount; frameIdx++) {

				Matrix4* currKeyframe = &boneKeyframes->at(frameIdx);

				Matrix4 boneTransform = GetNodeWorldTransformAtTime(node, evalTime, importTransform);

				//FIX THIS: Pass keyframes by reference so they can be changed
				*currKeyframe = boneTransform;
				evalTime += advance;
			}
		}

		m_motions.push_back(motion);
	}
}

Matrix4 SceneImport::GetNodeWorldTransformAtTime(FbxNode* node, FbxTime time, const Matrix4& importTransform) {
	if (nullptr == node) {
		return Matrix4::IDENTITY;
	}

	FbxMatrix fbx_mat = node->EvaluateGlobalTransform(time);
	Matrix4 engineMatrix = ToEngineMatrix(fbx_mat);
	return engineMatrix * importTransform;
}

//---------------------------------------------------------------------------------------------------------------------------
//IMPORTING SKELETONS
//---------------------------------------------------------------------------------------------------------------------------
void SceneImport::ImportSkeletons(FbxNode* node, MatrixStack44& matStack, Skeleton* skeleton, int parentJointIdx) {

	if (nullptr == node) {
		return;
	}

	Matrix4 mat = GetNodeTransform(node);
	matStack.Push(mat);

	//Walk attribs, looking for doot doots
	int AttribCount = node->GetNodeAttributeCount();

	for (int attribIdx = 0; attribIdx < AttribCount; attribIdx++) {
		FbxNodeAttribute* attrib = node->GetNodeAttribute();

		if (attrib && attrib->GetAttributeType() == FbxNodeAttribute::eSkeleton) {

			//So we have a doot
			FbxSkeleton* fbxSkeleton = (FbxSkeleton*)attrib;
			Skeleton* newSkeleton = ImportJoint(matStack, skeleton, parentJointIdx, fbxSkeleton);
			m_fbxNodeJoints.push_back(fbxSkeleton->GetNode());

			//New skeleton will either be the same skelly passed, or a new skelly, or ne skelly if it was a bad node.
			//If we get something back - it is what we pass on to the next generation.
			if (nullptr != newSkeleton) {
				skeleton = newSkeleton;
				parentJointIdx = skeleton->GetLastAddedJointIndex();
			}
		}
	}

	//Do the rest of the tree
	int childCount = node->GetChildCount();
	for (int childIdx = 0; childIdx < childCount; childIdx++) {
		ImportSkeletons(node->GetChild(childIdx), matStack, skeleton, parentJointIdx);
	}

	matStack.Pop();
}

//---------------------------------------------------------------------------------------------------------------------------
Skeleton* SceneImport::ImportJoint(MatrixStack44& matStack, Skeleton* skeleton, int parentJointIdx, FbxSkeleton* fbxSkeleton) {
	Skeleton* returnedSkeleton = nullptr;

	if (fbxSkeleton->IsSkeletonRoot()) {
		//THIS IS NEW SKELLY
		returnedSkeleton = new(MEMORY_RENDERING) Skeleton();

		m_skeletons.push_back(returnedSkeleton);
	}
	else {
		returnedSkeleton = skeleton;
		ASSERT_OR_DIE(returnedSkeleton != nullptr, "ERROR: LE DOOT NOT DOOTING");
	}

	Matrix4 geoTransform = GetGeometricTransform(fbxSkeleton->GetNode());
	matStack.Push(geoTransform);

	Matrix4 modelSpace = matStack.Top();
	returnedSkeleton->AddJoint(fbxSkeleton->GetNode()->GetName(), parentJointIdx, modelSpace);
	matStack.Pop();
	return returnedSkeleton;
}

//---------------------------------------------------------------------------------------------------------------------------
//OTHER GETTERS
//---------------------------------------------------------------------------------------------------------------------------
Matrix4 SceneImport::GetGeometricTransform(FbxNode* node) {
	Matrix4 retVal = Matrix4::IDENTITY;

	if (nullptr != node && nullptr != node->GetNodeAttribute()) {
		FbxVector4 const geoTransform = node->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 const geoRotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 const geoScale = node->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxMatrix geoMat;      
		geoMat.SetTRS(geoTransform, geoRotation, geoScale);

		retVal = ToEngineMatrix(geoMat);
	}

	return retVal;
}

//---------------------------------------------------------------------------------------------------------------------------
//GET OBJECT
//---------------------------------------------------------------------------------------------------------------------------
template <typename ElemType, typename OutType>
static bool GetObjectFromElement(FbxMesh* mesh, int polyIdx, int vertIdx, ElemType* elem, OutType* outVector2) {
	if (nullptr == elem) {
		return false;
	}

	switch (elem->GetMappingMode()) {
	case FbxGeometryElement::eByControlPoint: {
		int controlPointIdx = mesh->GetPolygonVertex(polyIdx, vertIdx);

		switch (elem->GetReferenceMode()) {
		case FbxGeometryElement::eDirect: {
			if (controlPointIdx < elem->GetDirectArray().GetCount()) {
				*outVector2 = elem->GetDirectArray().GetAt(controlPointIdx);
				return true;
			}
			break;
		}
		case FbxGeometryElement::eIndexToDirect: {
			if (controlPointIdx < elem->GetIndexArray().GetCount()) {
				int index = elem->GetIndexArray().GetAt(controlPointIdx);
				*outVector2 = elem->GetDirectArray().GetAt(index);
				return true;
			}
			break;
		}
		default:
			break;
		}
		break;
	}

	case FbxGeometryElement::eByPolygonVertex: {
		int directVertexIdx = (polyIdx * 3) + vertIdx;

		switch (elem->GetReferenceMode()) {
		case FbxGeometryElement::eDirect: {
			if (directVertexIdx < elem->GetDirectArray().GetCount()) {
				*outVector2 = elem->GetDirectArray().GetAt(directVertexIdx);
				return true;
			}
			break;
		}
		case FbxGeometryElement::eIndexToDirect: {
			if (directVertexIdx < elem->GetIndexArray().GetCount()) {
				int index = elem->GetIndexArray().GetAt(directVertexIdx);
				*outVector2 = elem->GetDirectArray().GetAt(index);
				return true;
			}
			break;
		}
		default:
			break;
		}
	}

	default:
		DebuggerPrintf("Undefined mapping mode.");
	}

	return false;
}

//---------------------------------------------------------------------------------------------------------------------------
//DATA GETTERS
//---------------------------------------------------------------------------------------------------------------------------
bool SceneImport::GetColor(RGBA* outColor, FbxMesh* mesh, int polyIdx, int vertIdx) {
	FbxColor color;
	FbxGeometryElementVertexColor* uvs = mesh->GetElementVertexColor();

	if (GetObjectFromElement(mesh, polyIdx, vertIdx, uvs, &color)) {

		*outColor = RGBA((float)color.mRed, (float)color.mGreen, (float)color.mBlue, (float)color.mAlpha);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------
bool SceneImport::GetBitangent(Vector3* outBitan, FbxMesh* mesh, int polyIdx, int vertIdx, const Matrix4& matStack) {
	FbxVector4 normal;
	FbxGeometryElementBinormal* uvs = mesh->GetElementBinormal(0);

	if (GetObjectFromElement(mesh, polyIdx, vertIdx, uvs, &normal)) {
		Vector4 bitan = Vector4((float)normal.mData[0], (float)normal.mData[1], (float)normal.mData[2], 0.f);
		bitan = bitan * matStack;
		bitan.Normalize();

		*outBitan = Vector3(bitan.x, bitan.y, bitan.z);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------
bool SceneImport::GetTangent(Vector3* outTan, FbxMesh* mesh, int polyIdx, int vertIdx, const Matrix4& matStack) {
	FbxVector4 tangent;
	FbxGeometryElementTangent* uvs = mesh->GetElementTangent(0);

	if (GetObjectFromElement(mesh, polyIdx, vertIdx, uvs, &tangent)) {
		Vector4 tan = Vector4((float)tangent.mData[0], (float)tangent.mData[1], (float)tangent.mData[2], 0.f);
		tan = tan * matStack;
		tan.Normalize();

		*outTan = Vector3(tan.x, tan.y, tan.z);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------
bool SceneImport::GetNormal(Vector3* outNormal, FbxMesh* mesh, int polyIdx, int vertIdx, const Matrix4& matStack) {

	FbxVector4 normal;
	FbxGeometryElementNormal* uvs = mesh->GetElementNormal(0);

	if (GetObjectFromElement(mesh, polyIdx, vertIdx, uvs, &normal)) {
		Vector4 norm = Vector4((float)normal.mData[0], (float)normal.mData[1], (float)normal.mData[2], 0.f);
		norm = norm * matStack;
		norm.Normalize();

		*outNormal = Vector3(norm.x, norm.y, norm.z);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------
bool SceneImport::GetUV(Vector2* outUV, FbxMesh* mesh, int polyIdx, int vertIdx, int uvIdx) {
	FbxVector2 uv;
	FbxGeometryElementUV* uvs = mesh->GetElementUV(uvIdx);

	if (GetObjectFromElement(mesh, polyIdx, vertIdx, uvs, &uv)) {
		*outUV = Vector2((float)uv.mData[0], (float)(1.f - uv.mData[1]));
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------
bool SceneImport::GetPosition(Vector3* outPosition, Matrix4 transform, FbxMesh* mesh, int polyIdx, int vertIdx) {
	FbxVector4 fbxPos;

	int controlIdx = mesh->GetPolygonVertex(polyIdx, vertIdx);

	fbxPos = mesh->GetControlPointAt(controlIdx);

	Vector4 pos = Vector4((float)fbxPos.mData[0], (float)fbxPos.mData[1], (float)fbxPos.mData[2], 1.f);
	pos = pos * transform;

	String name = mesh->GetNode()->GetName();
	if (name == "skeleton_archer_armor" && !m_foundAPoint) {
		m_pointToDraw = Vector3(pos.x, pos.y, pos.z);
		m_foundAPoint = true;
	}

	*outPosition = Vector3(pos.x, pos.y, pos.z);
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//UTILITY FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
Matrix4 SceneImport::GetSceneBasis(FbxScene* scene) {
	FbxAxisSystem axisSystem = scene->GetGlobalSettings().GetAxisSystem();

	FbxAMatrix sceneMatrix;
	axisSystem.GetMatrix(sceneMatrix);

	return ToEngineMatrix(sceneMatrix);
}
//---------------------------------------------------------------------------------------------------------------------------
Vector4 SceneImport::ToEngineVector4(FbxDouble4 val) {
	return Vector4((float)val.mData[0], (float)val.mData[1], (float)val.mData[2], (float)val.mData[3]);
}
//---------------------------------------------------------------------------------------------------------------------------
Matrix4 SceneImport::ToEngineMatrix(FbxMatrix fbxMatrix) {

	Matrix4 mat;

	mat.SetFirstRow(ToEngineVector4(fbxMatrix.mData[0]));
	mat.SetSecondRow(ToEngineVector4(fbxMatrix.mData[1]));
	mat.SetThirdRow(ToEngineVector4(fbxMatrix.mData[2]));
	mat.SetFourthRow(ToEngineVector4(fbxMatrix.mData[3]));

	return mat;
}
//---------------------------------------------------------------------------------------------------------------------------
Matrix4 SceneImport::GetNodeTransform(FbxNode* node) {
	FbxMatrix fbxLocalMatrix = node->EvaluateLocalTransform(); //Can do this at different points in time and lerp through them
	return ToEngineMatrix(fbxLocalMatrix);
}

//---------------------------------------------------------------------------------------------------------------------------
void SceneImport::ConstructTBN(const Vector3& normal, Vector3* outTan, Vector3* outBitan) {

	ASSERT_OR_DIE(nullptr != outTan && nullptr != outBitan, "ERROR: CANNOT CONSTRUCT TBN FROM NORMAL");

	Vector3 worldUp = Vector3::UP;

	*outTan = CrossProduct(worldUp, normal);
	*outBitan = CrossProduct(normal, *outTan);
	outBitan->Normalize();
	outTan->Normalize();

/*
	*outBitan = CrossProduct(normal, worldUp);
	outBitan->Normalize();
	*outTan = CrossProduct(*outBitan, normal);
	outTan->Normalize();*/
}


#else
bool SceneImport::FbxLoadSceneFromFile(String filename, Matrix4 engineBasis, bool isEngineBasisRightHanded, Matrix4 transform) {
	filename;
	engineBasis;
	isEngineBasisRightHanded;
	transform;
	return false;
}
#endif

