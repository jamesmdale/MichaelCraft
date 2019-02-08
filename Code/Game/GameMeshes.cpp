#include "GameMeshes.hpp"
#include "Engine\Renderer\MeshBuilder.hpp"
#include "Engine\Renderer\Mesh.hpp"

//  =========================================================================================
Mesh* MakeBlock(const Vector3& center)
{
	float xVal = 0.5f;
	float yVal = 0.5f;
	float zVal = 0.5f;

	Vector2 texCoordsAtMins = Vector2::ZERO;
	Vector2 texCoordsAtMaxs = Vector2::ONE;

	Rgba tint = Rgba::WHITE;

	MeshBuilder builder;

	int vertSize = builder.GetVertexCount();

	builder.Begin(TRIANGLES_DRAW_PRIMITIVE, true); //begin is does use index buffer

	//front face
	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,0.f));
	builder.SetNormal(Vector3(0.f, 0.f, -1.f));
	builder.SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));	

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,0.f));
	builder.SetNormal(Vector3(0.f, 0.f, -1.f));
	builder.SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));	

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,1.f));
	builder.SetNormal(Vector3(0.f, 0.f, -1.f));
	builder.SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal,  center.y - yVal, center.z + zVal));	

	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,1.f));
	builder.SetNormal(Vector3(0.f, 0.f, -1.f));
	builder.SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));		

	builder.AddQuadIndices(vertSize, vertSize + 1, vertSize + 2, vertSize + 3);

	//right face
	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,0.f));
	builder.SetNormal(Vector3(1.f, 0.f, 0.f));
	builder.SetTangent(Vector4(0.f, 0.f, -1.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));	

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,0.f));
	builder.SetNormal(Vector3(1.f, 0.f, 0.f));
	builder.SetTangent(Vector4(0.f, 0.f, -1.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,1.f));
	builder.SetNormal(Vector3(1.f, 0.f, 0.f));
	builder.SetTangent(Vector4(0.f, 0.f, -1.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal,  center.y - yVal, center.z + zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,1.f));
	builder.SetNormal(Vector3(1.f, 0.f, 0.f));
	builder.SetTangent(Vector4(0.f, 0.f, -1.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));	

	builder.AddQuadIndices(vertSize + 4, vertSize + 5, vertSize + 6, vertSize + 7);

	//back face
	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,0.f));
	builder.SetNormal(Vector3(0.f, 0.f, 1.f));
	builder.SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,0.f));
	builder.SetNormal(Vector3(0.f, 0.f, 1.f));
	builder.SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,1.f));
	builder.SetNormal(Vector3(0.f, 0.f, 1.f));
	builder.SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal,  center.y + yVal, center.z + zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,1.f));
	builder.SetNormal(Vector3(0.f, 0.f, 1.f));
	builder.SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	builder.AddQuadIndices(vertSize + 8, vertSize + 9, vertSize + 10, vertSize + 11);

	//left face
	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,0.f));
	builder.SetNormal(Vector3(-1.f, 0.f, 0.f));
	builder.SetTangent(Vector4(0.f, 0.f, 1.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,0.f));
	builder.SetNormal(Vector3(-1.f, 0.f, 0.f));
	builder.SetTangent(Vector4(0.f, 0.f, 1.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,1.f));
	builder.SetNormal(Vector3(-1.f, 0.f, 0.f));
	builder.SetTangent(Vector4(0.f, 0.f, 1.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal,  center.y + yVal, center.z + zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,1.f));
	builder.SetNormal(Vector3(-1.f, 0.f, 0.f));
	builder.SetTangent(Vector4(0.f, 0.f, 1.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	builder.AddQuadIndices(vertSize + 12, vertSize + 13, vertSize + 14, vertSize + 15);

	//top face
	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,0.f));
	builder.SetNormal(Vector3(0.f, 1.f, 0.f));
	builder.SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,0.f));
	builder.SetNormal(Vector3(0.f, 1.f, 0.f));
	builder.SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,1.f));
	builder.SetNormal(Vector3(0.f, 1.f, 0.f));
	builder.SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal,  center.y - yVal, center.z + zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,1.f));
	builder.SetNormal(Vector3(0.f, 1.f, 0.f));
	builder.SetTangent(Vector4(1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));
	
	builder.AddQuadIndices(vertSize + 16, vertSize + 17, vertSize + 18, vertSize + 19);

	//bottom face
	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,0.f));
	builder.SetNormal(Vector3(0.f, -1.f, 0.f));
	builder.SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,0.f));
	builder.SetNormal(Vector3(0.f, -1.f, 0.f));
	builder.SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(1.f,1.f));
	builder.SetNormal(Vector3(0.f, -1.f, 0.f));
	builder.SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal,  center.y - yVal, center.z - zVal));

	builder.SetColor(tint);
	builder.SetUV(Vector2(0.f,1.f));
	builder.SetNormal(Vector3(0.f, -1.f, 0.f));
	builder.SetTangent(Vector4(-1.f, 0.f, 0.f, 1.f));
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.AddQuadIndices(vertSize + 20, vertSize + 21, vertSize + 22, vertSize + 23);

	return builder.CreateMesh<VertexPCU>();
}

//  =========================================================================================
Mesh* MakeChunk()
{
	return nullptr;
}

// add other game meshes here (players, weapons items, etc) =========================================================================================
