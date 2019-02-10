#include "Game\GameObjects\Chunk.hpp"
#include "Game\Definitions\BlockDefinition.hpp"
#include "Engine\Renderer\MeshBuilder.hpp"
#include "Engine\Renderer\Mesh.hpp"

const Vector3 blockCenterOffset = Vector3(0.5f, 0.5f, 0.5f);

//  =========================================================================================
Chunk::Chunk(const IntVector2& coordinates)
{
	BlockDefinition* airBlockDef = BlockDefinition::GetDefinitionById(0);
	for (int blockIndex = 0; blockIndex < BLOCKS_PER_CHUNK; ++blockIndex)
	{
		m_blocks[blockIndex].m_type = airBlockDef->m_type;
	}

	Vector3 minsWorldCoordinates = Vector3((float)m_chunkCoords.x * BLOCKS_WIDE_X, (float)m_chunkCoords.y * BLOCKS_WIDE_Y, 0.f);
	Vector3 maxsWorldCoordinates = minsWorldCoordinates + Vector3(BLOCKS_WIDE_X, BLOCKS_WIDE_Y, BLOCKS_HIGH_Z);

	m_worldBounds = AABB3(minsWorldCoordinates, maxsWorldCoordinates);

	m_meshBuilder = new MeshBuilder();
	m_chunkCoords = coordinates;

	//randomization step goes here
	GenerateChunkMesh();
}

//  =========================================================================================
Chunk::~Chunk()
{
	delete(m_meshBuilder);
	m_meshBuilder = nullptr;

	delete(m_gpuMesh);
	m_gpuMesh = nullptr;
}

//  =========================================================================================
void Chunk::Update()
{

}

//  =========================================================================================
void Chunk::Render()
{
	Renderer* theRenderer = Renderer::GetInstance();

	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("Data/Images/coin.jpg"));
	theRenderer->m_defaultShader->SetFrontFace(WIND_COUNTER_CLOCKWISE);
	theRenderer->DrawMesh(m_gpuMesh);
}

//  =========================================================================================
void Chunk::GenerateChunkMesh()
{
	for (int blockIndex = 0; blockIndex < BLOCKS_PER_CHUNK; ++blockIndex)
	{
		AddBlockToMesh(GetBlockWorldCenterForBlockIndex(blockIndex), &m_blocks[blockIndex]);
	}

	m_gpuMesh = m_meshBuilder->CreateMesh<VertexPCU>();
}

//  =========================================================================================
int Chunk::GetBlockIndexForBlockCoords(const IntVector3& blockCoords)
{
	// FAST VERSION return blockCoords.x | (blockCoords.y << CHUNK_X_MASK) + (blockCoords.z << (CHUNK_X_MASK + CHUNK_Y_MASK));
	return blockCoords.x + (blockCoords.y * BLOCKS_WIDE_X) + (blockCoords.z * (BLOCKS_WIDE_X * BLOCKS_WIDE_Y));
}

//  =========================================================================================
IntVector3 Chunk::GetBlockCoordsForBlockIndex(int blockIndex)
{
	int x = blockIndex & CHUNK_X_MASK;
	int y = (blockIndex & CHUNK_Y_MASK) >> BITS_WIDE_X;
	int z = blockIndex >> (BITS_WIDE_X + BITS_WIDE_Y);	

	return IntVector3(x, y, z);
}

//  =========================================================================================
Vector3 Chunk::GetBlockWorldCenterForBlockIndex(int blockIndex)
{
	Vector3 worldCoordinates = Vector3(GetBlockCoordsForBlockIndex(blockIndex));
	return worldCoordinates + m_worldBounds.mins + blockCenterOffset;
}

//  =========================================================================================
void Chunk::AddBlockToMesh(const Vector3& center, Block* block)
{
	float xVal = 0.5f;
	float yVal = 0.5f;
	float zVal = 0.5f;

	BlockDefinition* blockDef = BlockDefinition::GetDefinitionById(block->m_type);
	AABB2 frontTexCoords = blockDef->m_frontTexCoords;
	AABB2 rightTexCoords = blockDef->m_rightSideTexCoords;
	AABB2 backTexCoords = blockDef->m_backTexCoords;
	AABB2 leftTexCoords = blockDef->m_leftSideTexCoords;
	AABB2 topTexCoords = blockDef->m_topTexCoords;
	AABB2 bottomTexCoords = blockDef->m_bottomTexCoords;

	Rgba tint = Rgba::WHITE;

	int vertSize = m_meshBuilder->GetVertexCount();

	m_meshBuilder->Begin(TRIANGLES_DRAW_PRIMITIVE, true); //begin is does use index buffer

	//front face
	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(frontTexCoords.mins.x, frontTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 0.f, -1.f));
	m_meshBuilder->SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(frontTexCoords.maxs.x, frontTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 0.f, -1.f));
	m_meshBuilder->SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(frontTexCoords.maxs.x, frontTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 0.f, -1.f));
	m_meshBuilder->SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(frontTexCoords.mins.x, frontTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 0.f, -1.f));
	m_meshBuilder->SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	m_meshBuilder->AddQuadIndices(vertSize, vertSize + 1, vertSize + 2, vertSize + 3);

	//right face
	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(rightTexCoords.mins.x, rightTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(1.f, 0.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(0.f, 0.f, -1.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(rightTexCoords.maxs.x, rightTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(1.f, 0.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(0.f, 0.f, -1.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(rightTexCoords.maxs.x, rightTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(1.f, 0.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(0.f, 0.f, -1.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(rightTexCoords.mins.x, rightTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(1.f, 0.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(0.f, 0.f, -1.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	m_meshBuilder->AddQuadIndices(vertSize + 4, vertSize + 5, vertSize + 6, vertSize + 7);

	//back face
	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(backTexCoords.mins.x, backTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 0.f, 1.f));
	m_meshBuilder->SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(backTexCoords.maxs.x, backTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 0.f, 1.f));
	m_meshBuilder->SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(backTexCoords.maxs.x, backTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 0.f, 1.f));
	m_meshBuilder->SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(backTexCoords.mins.x, backTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 0.f, 1.f));
	m_meshBuilder->SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	m_meshBuilder->AddQuadIndices(vertSize + 8, vertSize + 9, vertSize + 10, vertSize + 11);

	//left face
	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(leftTexCoords.mins.x, leftTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(-1.f, 0.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(0.f, 0.f, 1.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(leftTexCoords.maxs.x, leftTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(-1.f, 0.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(0.f, 0.f, 1.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(leftTexCoords.maxs.x, leftTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(-1.f, 0.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(0.f, 0.f, 1.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(leftTexCoords.mins.x, leftTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(-1.f, 0.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(0.f, 0.f, 1.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	m_meshBuilder->AddQuadIndices(vertSize + 12, vertSize + 13, vertSize + 14, vertSize + 15);

	//top face
	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(topTexCoords.mins.x, topTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 1.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(topTexCoords.maxs.x, topTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 1.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(topTexCoords.maxs.x, topTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 1.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(topTexCoords.mins.x, topTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(0.f, 1.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	m_meshBuilder->AddQuadIndices(vertSize + 16, vertSize + 17, vertSize + 18, vertSize + 19);

	//bottom face
	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(bottomTexCoords.mins.x, bottomTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(0.f, -1.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(bottomTexCoords.maxs.x, bottomTexCoords.mins.y);
	m_meshBuilder->SetNormal(Vector3(0.f, -1.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(bottomTexCoords.maxs.x, bottomTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(0.f, -1.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	m_meshBuilder->SetColor(tint);
	m_meshBuilder->SetUV(bottomTexCoords.mins.x, bottomTexCoords.maxs.y);
	m_meshBuilder->SetNormal(Vector3(0.f, -1.f, 0.f));
	m_meshBuilder->SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	m_meshBuilder->PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	m_meshBuilder->AddQuadIndices(vertSize + 20, vertSize + 21, vertSize + 22, vertSize + 23);
}

//  =========================================================================================
//Vector3 Chunk::GetWorldPositionOfBlockZero()
//{
//	return Vector3((float)m_chunkCoords.x * BLOCKS_WIDE_X, (float)m_chunkCoords.y * BLOCKS_WIDE_Y, 0.f);
//}
