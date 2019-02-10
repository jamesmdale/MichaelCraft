#pragma once
#include "Engine\Renderer\Mesh.hpp"
#include "Engine\Renderer\MeshBuilder.hpp"
#include "Engine\Math\AABB2.hpp"

extern Mesh* AddBlockToMesh(MeshBuilder* builder, const Vector3& center, const AABB2& texCoords);
extern Mesh* MakeChunkMesh();


