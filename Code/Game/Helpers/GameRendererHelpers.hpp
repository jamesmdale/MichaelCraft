#pragma once
#include "Engine\Math\Vector3.hpp"
#include "Engine\Core\Rgba.hpp"

class Mesh;

//custom render functions
Mesh* CreateLine(const Vector3& start, const Vector3& end, const Rgba& color = Rgba::WHITE, float scale = 1.f);
Mesh* CreateDebugStar(const Vector3 & center, const Rgba & color, float scale);
Mesh* CreateBlockHighlightBox(const Vector3& center, const Vector3& impactNormal, float scale = 1.1f);
Mesh* CreateBlockHighlightBoxOutline(const Vector3& center, const Vector3& impactNormal, float scale = 1.1f);
Mesh* CreateGameBasis(const Vector3& center, float width, float scale = 1.f);
Mesh* CreateBlockOutline(const Vector3 & center, Rgba tint = Rgba::WHITE, float scale = 1.f);


