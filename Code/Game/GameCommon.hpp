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

constexpr uint BITS_WIDE_X = 4;
constexpr uint BITS_WIDE_Y = 4;
constexpr uint BITS_TALL_Z = 8;

constexpr uint BLOCKS_WIDE_X =  1 << BITS_WIDE_X;
constexpr uint BLOCKS_WIDE_Y = 1 << BITS_WIDE_Y;
constexpr uint BLOCKS_HIGH_Z = 1 << BITS_TALL_Z;
constexpr uint BLOCKS_PER_CHUNK = BLOCKS_WIDE_X * BLOCKS_WIDE_Y * BLOCKS_HIGH_Z;

constexpr uint16 CHUNK_X_MASK = BLOCKS_WIDE_X - 1;
constexpr uint16 CHUNK_Y_MASK = (BLOCKS_WIDE_Y - 1) << BITS_WIDE_X;
constexpr uint16 CHUNK_Z_MASK = (BLOCKS_HIGH_Z - 1) << (BITS_WIDE_X + BITS_WIDE_Y);

constexpr int CHUNK_DISTANCE_RENDER = 4;
constexpr int CHUNK_DISTANCE_ACTIVATION = 6;
constexpr int CHUNK_DISTANCE_DEACTIVATION = 7;

constexpr float BLOCKS_WIDE_X_DIVISOR =  1.f / (float)BLOCKS_WIDE_X;
constexpr float BLOCKS_WIDE_Y_DIVISOR =  1.f / (float)BLOCKS_WIDE_Y;
constexpr float BLOCKS_WIDE_Z_DIVISOR =  1.f / (float)BLOCKS_HIGH_Z;

constexpr float CHUNK_RENDER_RADIUS = (float)BLOCKS_WIDE_X * (float)CHUNK_DISTANCE_RENDER;
constexpr float CHUNK_ACTIVATION_RADIUS = (float)BLOCKS_WIDE_X * (float)CHUNK_DISTANCE_ACTIVATION;
constexpr float CHUNK_DEACTIVATION_RADIUS = (float)BLOCKS_WIDE_X * (float)CHUNK_DISTANCE_DEACTIVATION;

//block data
constexpr uint8 IS_BLOCK_SOLID_MASK = 0b0000'0001;


//gameplay globals
constexpr float PLAYER_MOVEMENT_SPEED = 10.f;
constexpr float RAYCAST_MAX_DISTANCE = 8.f;
constexpr float RAYCAST_STEP_AMOUNT = 0.05f;

//convenience directions
extern Vector3 g_worldUp;
extern Vector3 g_worldForward;
extern Vector3 g_worldRight;

extern Vector3 g_east;
extern Vector3 g_west;
extern Vector3 g_north;
extern Vector3 g_south;

extern IntVector2 g_chunkNorth;
extern IntVector2 g_chunkSouth;
extern IntVector2 g_chunkEast;
extern IntVector2 g_chunkWest;

extern Matrix44 g_flipMatrix;



