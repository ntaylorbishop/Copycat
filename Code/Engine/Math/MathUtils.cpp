#include "MathUtils.hpp"


bool GetRayIntersectionWithSphere(const Ray& ray, const Sphere& sphere) {

	Vector3 originToSphere = sphere.m_pos - ray.m_origin;

	float lengthOfIntersectCheck = DotProduct(originToSphere, ray.m_dir);

	Vector3 eCheck = ray.m_origin + lengthOfIntersectCheck * ray.m_dir;

	Vector3 check = sphere.m_pos - eCheck;

	if (check.Length() < sphere.m_radius) {
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
bool GetRayIntersectionWithAABB3(const Ray& ray, const AABB3& box) {

	float tmin = (box.mins.x - ray.m_origin.x) / ray.m_dir.x;
	float tmax = (box.maxs.x - ray.m_origin.x) / ray.m_dir.x;

	if (tmin > tmax) {
		float tmp = tmin;
		tmin = tmax;
		tmax = tmp;
	}

	float tymin = (box.mins.y - ray.m_origin.y) / ray.m_dir.y;
	float tymax = (box.maxs.y - ray.m_origin.y) / ray.m_dir.y;

	if (tymin > tymax) {
		float tmp = tymin;
		tymin = tymax;
		tymax = tmp;
	}

	if ((tmin > tymax) || (tymin > tmax)) {
		return false;
	}

	if (tymin > tmin) {
		tmin = tymin;
	}

	if (tymax < tmax) {
		tmax = tymax;
	}

	float tzmin = (box.mins.z - ray.m_origin.z) / ray.m_dir.z;
	float tzmax = (box.maxs.z - ray.m_origin.z) / ray.m_dir.z;

	if (tzmin > tzmax) {
		float tmp = tzmin;
		tzmin = tzmax;
		tzmax = tmp;
	}

	if ((tmin > tzmax) || (tzmin > tmax)) {
		return false;
	}

	return true;
}


//---------------------------------------------------------------------------------------------------------------------------
bool IsPointInBox(const AABB3& box, const Vector3& pos) {

	float xMin = GetMinFloat(box.mins.x, box.maxs.x);
	float xMax = GetMaxFloat(box.mins.x, box.maxs.x);
	float yMin = GetMinFloat(box.mins.y, box.maxs.y);
	float yMax = GetMaxFloat(box.mins.y, box.maxs.y);
	float zMin = GetMinFloat(box.mins.z, box.maxs.z);
	float zMax = GetMaxFloat(box.mins.z, box.maxs.z);

	if (pos.x < xMax && pos.x > xMin &&
		pos.y < yMax && pos.y > yMin &&
		pos.z < zMax && pos.z > zMin)
	{
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
Vector2 Normalize(const Vector2& vec) {

	Vector2 norm;

	float length = sqrt(vec.x * vec.x + vec.y * vec.y);

	if (length != 0.f) {
		norm.x = vec.x / length;
		norm.y = vec.y / length;
	}

	return norm;
}


//---------------------------------------------------------------------------------------------------------------------------
Vector3 Normalize(const Vector3& vec) {

	Vector3 norm;

	float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	if (length != 0.f) {
		norm.x = vec.x / length;
		norm.y = vec.y / length;
		norm.z = vec.z / length;
	}

	return norm;
}


//---------------------------------------------------------------------------------------------------------------------------
Vector4 Normalize(const Vector4& vec) {

	Vector4 norm;

	float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);

	if (length != 0.f) {
		norm.x = vec.x / length;
		norm.y = vec.y / length;
		norm.z = vec.z / length;
		norm.w = vec.w / length;
	}

	return norm;
}


//---------------------------------------------------------------------------------------------------------------------------
void InitializeRandomSeedForRand() {
	srand((uint)time(NULL));
}


//---------------------------------------------------------------------------------------------------------------------------
bool DoDiscsOverlap(const Vector2& center1, float r1, const Vector2& center2, float r2) {
	float dist = DistanceBetweenPoints(center1, center2);
	float radii = r1 + r2;

	return (dist < radii);
}


//---------------------------------------------------------------------------------------------------------------------------
bool IsPointInDisk(const Vector2& point, const Vector2& center, float radius) {
	if (DistanceBetweenPoints(point, center) < radius)
		return true;
	else
		return false;
}


//---------------------------------------------------------------------------------------------------------------------------
bool IsPointInSquare(const IntVector2& point, const AABB2Int& square) {
	IntVector2 bl = square.mins;
	IntVector2 br = IntVector2(square.maxs.x, square.mins.y);
	IntVector2 tr = square.maxs;
	IntVector2 tl = IntVector2(square.mins.x, square.maxs.y);

	bool blBool = point.x >= bl.x && point.y >= bl.y;
	bool brBool = point.x <= br.x && point.y >= br.y;
	bool tlBool = point.x >= tl.x && point.y <= tl.y;
	bool trBool = point.x >= tr.x && point.y <= tr.y;

	if (blBool && brBool && tlBool && trBool) {
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
bool IsPointInSquare(const Vector2& point, const AABB2& square) {
	Vector2 bl = square.mins;
	Vector2 br = Vector2(square.maxs.x, square.mins.y);
	Vector2 tr = square.maxs;
	Vector2 tl = Vector2(square.mins.x, square.maxs.y);

	bool blBool = point.x >= bl.x && point.y >= bl.y;
	bool brBool = point.x <= br.x && point.y >= br.y;
	bool tlBool = point.x >= tl.x && point.y <= tl.y;
	bool trBool = point.x <= tr.x && point.y <= tr.y;

	if (blBool && brBool && tlBool && trBool) {
		return true;
	}
	else {
		return false;
	}
}
bool IsDiskInSquare(const Vector2& point, float radius, const AABB2& square) {

	Vector2 bl = square.mins;
	Vector2 br = Vector2(square.maxs.x, square.mins.y);
	Vector2 tr = square.maxs;
	Vector2 tl = Vector2(square.mins.x, square.maxs.y);

	bool blBool = (point.x - radius) >= bl.x && (point.y - radius) >= bl.y;
	bool brBool = (point.x + radius) <= br.x && (point.y - radius) >= br.y;
	bool tlBool = (point.x - radius) >= tl.x && (point.y + radius) <= tl.y;
	bool trBool = (point.x - radius) <= tr.x && (point.y + radius) <= tr.y;

	if (blBool && brBool && tlBool && trBool) {
		return true;
	}
	else {
		return false;
	}
}

int DistanceSquaredBetweenPoints(const IntVector2& v1, const IntVector2& v2) {
	int xDist = v2.x - v1.x;
	int yDist = v2.y - v1.y;
	return xDist * xDist + yDist * yDist;
}
float DistanceSquaredBetweenPoints(const Vector2& v1, const Vector2& v2) {
	float xDist = v2.x - v1.x;
	float yDist = v2.y - v1.y;
	return xDist * xDist + yDist * yDist;
}
int DistanceSquaredBetweenPoints(const IntVector3& v1, const IntVector3& v2) {
	int xDist = v2.x - v1.x;
	int yDist = v2.y - v1.y;
	return xDist * xDist + yDist * yDist;
}
float DistanceSquaredBetweenPoints(const Vector3& v1, const Vector3& v2) {
	float xDist = v2.x - v1.x;
	float yDist = v2.y - v1.y;
	return xDist * xDist + yDist * yDist;
}

int RandInt(int min, int max) {
	//ASSERT_OR_DIE(min < max, "ERROR: CANNOT SUPPLY RANDINT WITH MIN VAL GREATER THAN MAX VAL");
	return min + (rand() % (int)(max - min + 1));
}
int RandIntZeroToSize(int n) {
	n -= 1;
	return 0 + (rand() % (int)(n - 0 + 1));
}
float Randf(float Min, float Max) {
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

float ToRadians(float degrees) {
	return degrees * (PI_F / 180.f);
}
float ToDegrees(float radians) {
	return radians * (180.f / PI_F);
}
int Quadrant(float angle) {
	if (cos(angle) > 0 && sin(angle) > 0)
		return 1;
	else if (cos(angle) < 0 && sin(angle) > 0)
		return 2;
	else if (cos(angle) < 0 && sin(angle) < 0)
		return 3;
	else if (cos(angle) > 0 && sin(angle) < 0)
		return 4;
	else
		return -1;
}


float ClampFloat(float f, float min, float max) {
	if (f < min)
		return min;
	else if (f > max)
		return max;
	else
		return f;
} 

float CalcShortestAngularDisplacement(float fromDegrees, float toDegrees) {
	float angularDisplacement = toDegrees - fromDegrees;

	while (angularDisplacement > 180.f)
		angularDisplacement -= 360.f;
	while (angularDisplacement < -180.f)
		angularDisplacement += 360.f;

	return angularDisplacement;
}

//-----------------------------------------------------------------------------------------------
// Lookup table for sin() values for 256 evenly distributed angles around the unit circle,
//	where Byte 0 = 0 degrees, Byte 64 = 90 degrees, Byte 128 = 180 degrees, etc.
//
const float g_sinValuesForByteAngles[256] = {
	0.000000000f,	0.024541229f,	0.049067674f,	0.073564564f,	0.098017140f,	0.122410675f,	0.146730474f,	0.170961889f,
	0.195090322f,	0.219101240f,	0.242980180f,	0.266712757f,	0.290284677f,	0.313681740f,	0.336889853f,	0.359895037f,
	0.382683432f,	0.405241314f,	0.427555093f,	0.449611330f,	0.471396737f,	0.492898192f,	0.514102744f,	0.534997620f,
	0.555570233f,	0.575808191f,	0.595699304f,	0.615231591f,	0.634393284f,	0.653172843f,	0.671558955f,	0.689540545f,
	0.707106781f,	0.724247083f,	0.740951125f,	0.757208847f,	0.773010453f,	0.788346428f,	0.803207531f,	0.817584813f,
	0.831469612f,	0.844853565f,	0.857728610f,	0.870086991f,	0.881921264f,	0.893224301f,	0.903989293f,	0.914209756f,
	0.923879533f,	0.932992799f,	0.941544065f,	0.949528181f,	0.956940336f,	0.963776066f,	0.970031253f,	0.975702130f,
	0.980785280f,	0.985277642f,	0.989176510f,	0.992479535f,	0.995184727f,	0.997290457f,	0.998795456f,	0.999698819f,
	1.000000000f,	0.999698819f,	0.998795456f,	0.997290457f,	0.995184727f,	0.992479535f,	0.989176510f,	0.985277642f,
	0.980785280f,	0.975702130f,	0.970031253f,	0.963776066f,	0.956940336f,	0.949528181f,	0.941544065f,	0.932992799f,
	0.923879533f,	0.914209756f,	0.903989293f,	0.893224301f,	0.881921264f,	0.870086991f,	0.857728610f,	0.844853565f,
	0.831469612f,	0.817584813f,	0.803207531f,	0.788346428f,	0.773010453f,	0.757208847f,	0.740951125f,	0.724247083f,
	0.707106781f,	0.689540545f,	0.671558955f,	0.653172843f,	0.634393284f,	0.615231591f,	0.595699304f,	0.575808191f,
	0.555570233f,	0.534997620f,	0.514102744f,	0.492898192f,	0.471396737f,	0.449611330f,	0.427555093f,	0.405241314f,
	0.382683432f,	0.359895037f,	0.336889853f,	0.313681740f,	0.290284677f,	0.266712757f,	0.242980180f,	0.219101240f,
	0.195090322f,	0.170961889f,	0.146730474f,	0.122410675f,	0.098017140f,	0.073564564f,	0.049067674f,	0.024541229f,
	0.000000000f,	-0.024541229f,	-0.049067674f,	-0.073564564f,	-0.098017140f,	-0.122410675f,	-0.146730474f,	-0.170961889f,
	-0.195090322f,	-0.219101240f,	-0.242980180f,	-0.266712757f,	-0.290284677f,	-0.313681740f,	-0.336889853f,	-0.359895037f,
	-0.382683432f,	-0.405241314f,	-0.427555093f,	-0.449611330f,	-0.471396737f,	-0.492898192f,	-0.514102744f,	-0.534997620f,
	-0.555570233f,	-0.575808191f,	-0.595699304f,	-0.615231591f,	-0.634393284f,	-0.653172843f,	-0.671558955f,	-0.689540545f,
	-0.707106781f,	-0.724247083f,	-0.740951125f,	-0.757208847f,	-0.773010453f,	-0.788346428f,	-0.803207531f,	-0.817584813f,
	-0.831469612f,	-0.844853565f,	-0.857728610f,	-0.870086991f,	-0.881921264f,	-0.893224301f,	-0.903989293f,	-0.914209756f,
	-0.923879533f,	-0.932992799f,	-0.941544065f,	-0.949528181f,	-0.956940336f,	-0.963776066f,	-0.970031253f,	-0.975702130f,
	-0.980785280f,	-0.985277642f,	-0.989176510f,	-0.992479535f,	-0.995184727f,	-0.997290457f,	-0.998795456f,	-0.999698819f,
	-1.000000000f,	-0.999698819f,	-0.998795456f,	-0.997290457f,	-0.995184727f,	-0.992479535f,	-0.989176510f,	-0.985277642f,
	-0.980785280f,	-0.975702130f,	-0.970031253f,	-0.963776066f,	-0.956940336f,	-0.949528181f,	-0.941544065f,	-0.932992799f,
	-0.923879533f,	-0.914209756f,	-0.903989293f,	-0.893224301f,	-0.881921264f,	-0.870086991f,	-0.857728610f,	-0.844853565f,
	-0.831469612f,	-0.817584813f,	-0.803207531f,	-0.788346428f,	-0.773010453f,	-0.757208847f,	-0.740951125f,	-0.724247083f,
	-0.707106781f,	-0.689540545f,	-0.671558955f,	-0.653172843f,	-0.634393284f,	-0.615231591f,	-0.595699304f,	-0.575808191f,
	-0.555570233f,	-0.534997620f,	-0.514102744f,	-0.492898192f,	-0.471396737f,	-0.449611330f,	-0.427555093f,	-0.405241314f,
	-0.382683432f,	-0.359895037f,	-0.336889853f,	-0.313681740f,	-0.290284677f,	-0.266712757f,	-0.242980180f,	-0.219101240f,
	-0.195090322f,	-0.170961889f,	-0.146730474f,	-0.122410675f,	-0.098017140f,	-0.073564564f,	-0.049067674f,	-0.024541229f
};


//-----------------------------------------------------------------------------------------------
// A replacement for floor(), about 3x faster on my machine.
// Reliable within [-2 billion, +2 billion] or so.  I think.
//
float FastFloor(float f)
{
	if (f >= 0.f)
		return (float)(int)f;
	else
	{
		float f2 = (float)(int)f;
		if (f == f2)
			return f2;
		else
			return f2 - 1.f;
	}
}


//-----------------------------------------------------------------------------------------------
// A replacement for (int) floor(), about 3x faster on my machine
// Reliable within the range of int.  I think.
//
int FastFloorToInt(float f)
{
	if (f >= 0.f)
		return (int)f;
	else
	{
		int i = (int)f;
		float f2 = (float)i;
		if (f == f2)
			return i;
		else
			return i - 1;
	}
}

float AngleBetweenVectors(const Vector3& v1, const Vector3& v2) {
	float cosOfAngle = (DotProduct(v1, v2)) / (v1.Length() * v2.Length());
	cosOfAngle = ClampFloat(cosOfAngle, -1.f, 1.f);
	ASSERT_OR_DIE(cosOfAngle >= -1.f && cosOfAngle <= 1.f, "ERROR: Cannot calculate arccos of unbounded number");
	return acos(cosOfAngle);
}
Vector3 Slerp(const Vector3& from, const Vector3& to, const float step) {
	if (step == 0.f)
		return from;
	if (from == to || step == 1.0f)
		return to;

	float val = DotProduct(from, to) / (from.Length() * to.Length());
	val = ClampZeroToOne(val);

	float theta = acos(val);

	if (theta == 0.f)
		return to;

	float sinTheta = sin(theta);
	return (from * (sin((1 - step) * theta) / sinTheta)) + (to * (sin(step * theta) / sinTheta));
}


Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	//return v1 * t + v2 * (1.f - t);

	return v1 * (1.f - t) + v2 * t;
}

Vector2	Lerp(const Vector2& v1, const Vector2& v2, float t) {

	Vector2 r;
	r.x = v1.x * (1.f - t) + v2.x * t;
	r.y = v1.y * (1.f - t) + v2.y * t;
	return r;
}


float Lerp(float a, float b, float t) {
	return a * (1.f - t) + b * t;
}



double Lerp(double a, double b, float t) {
	return a * (1.f - t) + b * t;
}


RGBA Lerp(const RGBA& fromColor, const RGBA& toColor, float t) {
	
	RGBA lerpedColor;
	lerpedColor.r = fromColor.r * (1.f - t) + toColor.r * t;
	lerpedColor.g = fromColor.g * (1.f - t) + toColor.g * t;
	lerpedColor.b = fromColor.b * (1.f - t) + toColor.b * t;
	lerpedColor.a = fromColor.a * (1.f - t) + toColor.a * t;
	return lerpedColor;
}


IntVector2 Lerp(const IntVector2& fromVector, const IntVector2& toVector, float t) {

	IntVector2 lerpedVector;
	lerpedVector.x = (int)((float)fromVector.x * (1.f - (float)t) + (float)toVector.x * t);
	lerpedVector.y = (int)((float)fromVector.y * (1.f - (float)t) + (float)toVector.y * t);
	return lerpedVector;
}