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

constexpr uint BLOCKS_WIDE_X = 16;
constexpr uint BLOCKS_WIDE_Y = 16;
constexpr uint BLOCKS_HIGH_Z = 256;
constexpr uint BLOCKS_PER_CHUNK = BLOCKS_WIDE_X * BLOCKS_WIDE_Y * BLOCKS_HIGH_Z;

constexpr uint BITS_WIDE_X = 4;
constexpr uint BITS_WIDE_Y = 4;
constexpr uint BITS_TALL_Z = 8;

constexpr uint16 CHUNK_X_MASK = 0b0000'0000'0000'1111;
constexpr uint16 CHUNK_Y_MASK = 0b0000'0000'1111'0000;
constexpr uint16 CHUNK_Z_MASK = 0b1111'1111'0000'0000;

constexpr int CHUNK_DISTANCE_RENDER = 1;
constexpr int CHUNK_DISTANCE_ACTIVATION = 5;
constexpr int CHUNK_DISTANCE_DEACTIVATION = 7;

constexpr float CHUNK_RENDER_RADIUS = (float)BLOCKS_WIDE_X * (float)CHUNK_DISTANCE_RENDER;
constexpr float CHUNK_ACTIVATION_RADIUS = (float)BLOCKS_WIDE_X * (float)CHUNK_DISTANCE_ACTIVATION;
constexpr float CHUNK_DEACTIVATION_RADIUS = (float)BLOCKS_WIDE_X * (float)CHUNK_DISTANCE_DEACTIVATION;

//convenience directions
extern Vector3 g_worldUp;
extern Vector3 g_worldForward;
extern Vector3 g_worldRight;

extern Vector3 g_east;
extern Vector3 g_west;
extern Vector3 g_north;
extern Vector3 g_south;

extern Matrix44 g_flipMatrix;



