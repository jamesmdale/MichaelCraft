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

//gameplay globals  ----------------------------------------------
constexpr float PLAYER_MOVEMENT_SPEED = 10.f;
constexpr float RAYCAST_MAX_DISTANCE = 8.f;
constexpr float RAYCAST_STEP_AMOUNT = 0.015f;
constexpr float PLAYER_INPUT_DELAY_IN_SECONDS = 0.25f;

// chunk data ----------------------------------------------

constexpr uint BITS_WIDE_X = 4;
constexpr uint BITS_WIDE_Y = 4;
constexpr uint BITS_TALL_Z = 8;

constexpr uint BLOCKS_WIDE_X =  1 << BITS_WIDE_X;
constexpr uint BLOCKS_WIDE_Y = 1 << BITS_WIDE_Y;
constexpr uint BLOCKS_HIGH_Z = 1 << BITS_TALL_Z;
constexpr uint BLOCKS_PER_LAYER = BLOCKS_WIDE_X * BLOCKS_WIDE_Y;
constexpr uint BLOCKS_PER_CHUNK = BLOCKS_WIDE_X * BLOCKS_WIDE_Y * BLOCKS_HIGH_Z;

constexpr uint16 CHUNK_X_MASK = BLOCKS_WIDE_X - 1;
constexpr uint16 CHUNK_Y_MASK = (BLOCKS_WIDE_Y - 1) << BITS_WIDE_X;
constexpr uint16 CHUNK_Z_MASK = (BLOCKS_HIGH_Z - 1) << (BITS_WIDE_X + BITS_WIDE_Y);

constexpr int CHUNK_DISTANCE_RENDER = 2;
constexpr int CHUNK_DISTANCE_ACTIVATION = 4;
constexpr int CHUNK_DISTANCE_DEACTIVATION = 5;

constexpr float BLOCKS_WIDE_X_DIVISOR =  1.f / (float)BLOCKS_WIDE_X;
constexpr float BLOCKS_WIDE_Y_DIVISOR =  1.f / (float)BLOCKS_WIDE_Y;
constexpr float BLOCKS_WIDE_Z_DIVISOR =  1.f / (float)BLOCKS_HIGH_Z;

constexpr float CHUNK_RENDER_RADIUS = (float)BLOCKS_WIDE_X * (float)CHUNK_DISTANCE_RENDER;
constexpr float CHUNK_ACTIVATION_RADIUS = (float)BLOCKS_WIDE_X * (float)CHUNK_DISTANCE_ACTIVATION;
constexpr float CHUNK_DEACTIVATION_RADIUS = (float)BLOCKS_WIDE_X * (float)CHUNK_DISTANCE_DEACTIVATION;

constexpr uint8 CHUNK_SAVE_VERSION = 1;

//block data ----------------------------------------------
constexpr uint8 IS_BLOCK_SOLID_MASK = 0b0000'0001;
constexpr uint8 IS_BLOCK_FULL_OPAQUE_MASK = 0b0000'0010;
constexpr uint8 IS_BLOCK_VISIBLE_MASK = 0b0000'0100;
constexpr uint8 IS_BLOCK_SKY_MASK = 0b0001'0000;
constexpr uint8 IS_BLOCK_IN_LIGHT_DIRTY_LIST_MASK = 0b0010'0000;
constexpr uint8 DOES_BLOCK_EMIT_LIGHT = 0b0000'1000;

constexpr uint8 INDOOR_LIGHTING_MASK = 0b0000'1111;
constexpr uint8 OUTDOOR_LIGHTING_MASK = 0b1111'0000;
constexpr uint8 BITS_WIDE_INDOOR_LIGHTING_MASK = 4;
constexpr uint8 BITS_WIDE_OUTDOOR_LIGHTING_MASK = 4;
constexpr int MAX_INDOOR_LIGHTING_VALUE = 15;
constexpr int MAX_OUTDOOR_LIGHTING_VALUE = 15;

//lighting defaults ----------------------------------------------
constexpr float MIN_LIGHT_EXPOSURE_PERCENTAGE = 0.15f;
constexpr float MAX_LIGHT_EXPOSURE_PERCENTAGE = 1.f;

extern Rgba g_minLightColor;
extern Rgba g_maxLightColor;

//convenience directions ----------------------------------------------
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


