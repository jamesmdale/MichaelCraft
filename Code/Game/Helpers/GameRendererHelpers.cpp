#include "Game\Helpers\GameRendererHelpers.hpp"
#include "Engine\Renderer\Mesh.hpp"
#include "Engine\Renderer\MeshBuilder.hpp"
#include "Engine\Math\Vector3.hpp"
#include "Game\GameCommon.hpp"

//  =========================================================================================
Mesh* CreateLine(const Vector3& start, const Vector3& end, const Rgba& color, float scale)
{
	MeshBuilder builder;

	builder.Begin(LINES_DRAW_PRIMITIVE, false);

	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(start.x, start.y, start.z));

	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(end.x, end.y, end.z));

	return builder.CreateMesh<VertexPCU>();
}

//  =========================================================================================
Mesh* CreateDebugStar(const Vector3& center, const Rgba& color, float scale)
{
	MeshBuilder builder;

	float halfScale = scale * 0.5f;

	builder.Begin(LINES_DRAW_PRIMITIVE, false);

	//east to west
	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) + (g_east * scale));

	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) + (g_west * scale));

	//north to south
	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) + (g_north * scale));

	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) + (g_south * scale));

	//up to down
	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) + (g_worldUp * scale));

	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) - (g_worldUp * scale));

	//diagonals
	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) + (Vector3::ONE * halfScale));

	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) - (Vector3::ONE * halfScale));

	//diagonals

	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) + (Vector3(1.f, -1.f, -1.f) * halfScale));

	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) - (Vector3(1.f, -1.f, -1.f) * halfScale));

	//diagonals
	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) + (Vector3(-1.f, 1.f, -1.f) * halfScale));

	builder.SetColor(color);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center) - (Vector3(-1.f, 1.f, -1.f) * halfScale));

	

	return builder.CreateMesh<VertexPCU>();
}

//  =========================================================================================
Mesh* CreateBlockHighlightBox(const Vector3& center, const Vector3& impactNormal, float scale)
{
	MeshBuilder builder;
	builder.Begin(TRIANGLES_DRAW_PRIMITIVE, true);
	
	int vertSize = builder.GetVertexCount();

	float xVal = 0.5f * scale;
	float yVal = 0.5f * scale;
	float zVal = 0.5f * scale;

	Rgba westTint = Rgba::WHITE;
	Rgba eastTint = Rgba::WHITE;
	Rgba northTint = Rgba::WHITE;
	Rgba southTint = Rgba::WHITE;
	Rgba topTint = Rgba::WHITE;
	Rgba bottomTint = Rgba::WHITE;

	//set the box color for the impact normal
	if (impactNormal == Vector3(-1.f, 0.f, 0.f))
		westTint = Rgba::PINK;
	else if (impactNormal == Vector3(1.f, 0.f, 0.f))
		eastTint = Rgba::PINK;
	else if (impactNormal == Vector3(0.f, 1.f, 0.f))
		northTint = Rgba::PINK;
	else if (impactNormal == Vector3(0.f, -1.f, 0.f))
		southTint = Rgba::PINK;
	else if (impactNormal == Vector3(0.f, 0.f, 1.f))
		topTint = Rgba::PINK;
	else if (impactNormal == Vector3(0.f, 0.f, -1.f))
		bottomTint = Rgba::PINK;
	
	//west face
	//front face	
	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	builder.AddQuadIndices(vertSize, vertSize + 1, vertSize + 2, vertSize + 3);
	
	//south face
	//right face
	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);;
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	builder.AddQuadIndices(vertSize + 4, vertSize + 5, vertSize + 6, vertSize + 7);	

	//east face
	//back face	
	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	builder.AddQuadIndices(vertSize + 8, vertSize + 9, vertSize + 10, vertSize + 11);
	
	//north face
	//left face
	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(northTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	builder.AddQuadIndices(vertSize + 12, vertSize + 13, vertSize + 14, vertSize + 15);

	//up face
	//top face
	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(topTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	builder.AddQuadIndices(vertSize + 16, vertSize + 17, vertSize + 18, vertSize + 19);
	
	//down face
	//bottom face
	builder.SetColor(bottomTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(bottomTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(bottomTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(bottomTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.AddQuadIndices(vertSize + 20, vertSize + 21, vertSize + 22, vertSize + 23);

	return builder.CreateMesh<VertexPCU>();
}

//  =========================================================================================
Mesh* CreateBlockHighlightBoxOutline(const Vector3& center, const Vector3& impactNormal, float scale)
{
	MeshBuilder builder;
	builder.Begin(LINES_DRAW_PRIMITIVE, false);

	int vertSize = builder.GetVertexCount();

	float xVal = 0.5f * scale;
	float yVal = 0.5f * scale;
	float zVal = 0.5f * scale;

	Rgba westTint = Rgba::WHITE;
	Rgba eastTint = Rgba::WHITE;
	Rgba northTint = Rgba::WHITE;
	Rgba southTint = Rgba::WHITE;
	Rgba topTint = Rgba::WHITE;
	Rgba bottomTint = Rgba::WHITE;

	//set the box color for the impact normal
	if (impactNormal == Vector3(-1.f, 0.f, 0.f))
		westTint = Rgba::PINK;
	else if (impactNormal == Vector3(1.f, 0.f, 0.f))
		eastTint = Rgba::PINK;
	else if (impactNormal == Vector3(0.f, 1.f, 0.f))
		northTint = Rgba::PINK;
	else if (impactNormal == Vector3(0.f, -1.f, 0.f))
		southTint = Rgba::PINK;
	else if (impactNormal == Vector3(0.f, 0.f, 1.f))
		topTint = Rgba::PINK;
	else if (impactNormal == Vector3(0.f, 0.f, -1.f))
		bottomTint = Rgba::PINK;

	//west face
	//front face	
	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));
	
	//  ----------------------------------------------

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	//south face
	//right face
	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);;
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));
	
	//  ----------------------------------------------

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);;
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	//east face
	//back face	
	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	//north face
	//left face
	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(northTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(northTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	//up face
	//top face
	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(topTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(topTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	//down face
	//bottom face
	builder.SetColor(bottomTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(bottomTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(bottomTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(bottomTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(bottomTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(bottomTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(bottomTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(bottomTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));


	//  create final mesh ----------------------------------------------
	return builder.CreateMesh<VertexPCU>();
}

//  =========================================================================================
Mesh* CreateGameBasis(const Vector3& center, float width, float scale)
{
	UNUSED(width);

	MeshBuilder builder;

	builder.Begin(TRIANGLES_DRAW_PRIMITIVE, true);

	//up - z
	builder.SetColor(Rgba::BLUE);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center.x, center.y, center.z));

	builder.SetColor(Rgba::BLUE);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center.x, center.y, center.z) + (scale * g_worldUp));

	builder.SetColor(Rgba::BLUE);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center.x, center.y, center.z));

	//forward - x
	builder.SetColor(Rgba::RED);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center.x, center.y, center.z));

	builder.SetColor(Rgba::RED);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center.x, center.y, center.z) + (scale * g_east));

	builder.SetColor(Rgba::RED);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center.x, center.y, center.z));

	//left - y
	builder.SetColor(Rgba::GREEN);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center.x, center.y, center.z));

	builder.SetColor(Rgba::GREEN);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center.x, center.y, center.z) + (scale * g_north));

	builder.SetColor(Rgba::GREEN);
	builder.SetUV(0, 0);
	builder.PushVertex(Vector3(center.x, center.y, center.z));

	return builder.CreateMesh<VertexPCU>();
}


//  =========================================================================================
Mesh* CreateBoxOutline(const Vector3& center, Rgba tint, float scale)
{
	MeshBuilder builder;
	builder.Begin(LINES_DRAW_PRIMITIVE, false);

	int vertSize = builder.GetVertexCount();

	float xVal = 0.5f * scale;
	float yVal = 0.5f * scale;
	float zVal = 0.5f * scale;

	Rgba westTint = tint;
	Rgba eastTint = tint;
	Rgba northTint = tint;
	Rgba southTint = tint;
	Rgba topTint = tint;
	Rgba bottomTint = tint;

	//west face
	//front face	
	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(westTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	//south face
	//right face
	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);;
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(southTint);
	builder.SetUV(0.f, 0.f);;
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	//east face
	//back face	
	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(eastTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	//north face
	//left face
	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(northTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(northTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(northTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	//up face
	//top face
	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(topTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(topTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z + zVal));

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	//  ----------------------------------------------

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z + zVal));

	builder.SetColor(topTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z + zVal));

	//down face
	//bottom face
	builder.SetColor(bottomTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(bottomTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(bottomTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(bottomTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(bottomTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y - yVal, center.z - zVal));

	builder.SetColor(bottomTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	//  ----------------------------------------------

	builder.SetColor(bottomTint);	
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x - xVal, center.y + yVal, center.z - zVal));

	builder.SetColor(bottomTint);
	builder.SetUV(0.f, 0.f);
	builder.PushVertex(Vector3(center.x + xVal, center.y + yVal, center.z - zVal));


	//  create final mesh ----------------------------------------------
	return builder.CreateMesh<VertexPCU>();
}

