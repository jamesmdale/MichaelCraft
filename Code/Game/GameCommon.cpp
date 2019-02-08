#include "Game\GameCommon.hpp"


// globals ----------------------------------------------
bool g_isDebug = false;
bool g_isQuitting = false;

//convenience directions
Vector3 g_worldUp = Vector3(0.f, 0.f, 1.f);
Vector3 g_worldForward = Vector3(1.f, 0.f, 0.f);
Vector3 g_worldRight = Vector3(0.f, -1.f, 0.f);

Vector3 g_east = Vector3(1.f, 0.f, 0.f);
Vector3 g_west = Vector3(-1.f, 0.f, 0.f);
Vector3 g_north = Vector3(0.f, 1.f, 0.f);
Vector3 g_south = Vector3(0.f, -1.f, 0.f);

//convenience matricies
Matrix44 g_flipMatrix = Matrix44(Vector4(0.f, 0.f, 1.f, 0.f),
								Vector4(-1.f, 0.f, 0.f, 0.f),
								Vector4(0.f, 1.f, 0.f, 0.f),
								Vector4(0.f, 0.f, 0.f, 1.f));