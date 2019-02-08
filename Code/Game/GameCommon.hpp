#pragma once
#include "Engine\Math\Vector2.hpp"
#include "Engine\Math\Vector3.hpp"
#include "Engine\Math\Matrix44.hpp"
#include "Engine\Core\Rgba.hpp"
#include "Engine\Renderer\Renderer.hpp"
#include "Engine\Input\InputSystem.hpp"

// game related globals =========================================================================================
extern bool g_isDebug;
extern bool g_isQuitting;

//convenience directions
extern Vector3 g_worldUp;
extern Vector3 g_worldForward;
extern Vector3 g_worldRight;

extern Vector3 g_east;
extern Vector3 g_west;
extern Vector3 g_north;
extern Vector3 g_south;

extern Matrix44 g_flipMatrix;



