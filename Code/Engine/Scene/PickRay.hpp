#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Scene/ClickableObject.hpp"

void CreatePickRay(std::vector<ClickableObject*>& intersectedObjects, Vector3& start, Vector3& end);