#include "Game\Game.hpp"
#include "Game\GameStates\PlayingState.hpp"
#include "Game\ObjectMeshbuilder.hpp"
#include "Game\GameCommon.hpp"
#include "Engine\Window\Window.hpp"
#include "Engine\Debug\DebugRender.hpp"
#include "Engine\Core\LightObject.hpp"
#include "Engine\Debug\DebugRender.hpp"
#include "Engine\Renderer\RenderScene.hpp"
#include "Engine\Renderer\RenderScene2D.hpp"
#include "Engine\Core\StringUtils.hpp"
#include "Engine\Renderer\MeshBuilder.hpp"
#include "Engine\Renderer\Mesh.hpp"
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Math\MathUtils.hpp"
#include <map>
#include <string>
#include <algorithm>

//  =========================================================================================
World::World()
{

}

//  =========================================================================================
World::~World()
{
	//these will get deleted from game later becuase it's used in other gamestates
	m_engineCamera = nullptr;
	m_uiCamera = nullptr;

	delete(m_gameCamera);
	m_gameCamera = nullptr;
}

//  =========================================================================================
void World::Initialize()
{
	//initialize game camera
	Renderer* theRenderer = Renderer::GetInstance();

	m_engineCamera = Game::GetInstance()->m_engineCamera;
	m_uiCamera = Game::GetInstance()->m_uiCamera;

	m_gameCamera = new GameCamera();
	m_gameCamera->Translate(-Vector3(-1, 0, 30) * 10.f);

	//m_camera->m_skybox = new Skybox("Data/Images/galaxy2.png");
	theRenderer->SetAmbientLightIntensity(0.15f);

	GenerateChunkBuildOrderCheatSheet();

	////chunk 1
	//Chunk* chunk1 = new Chunk(IntVector2(0,0));
	//ActivateChunk(chunk1);

	////chunk 2
	//Chunk* chunk2 = new Chunk(IntVector2(-1, 2));
	//ActivateChunk(chunk2);

	////chunk 3
	//Chunk* chunk3 = new Chunk(IntVector2(0, 2));
	//ActivateChunk(chunk3);

	////chunk 4
	//Chunk* chunk4 = new Chunk(IntVector2(1, 2));
	//ActivateChunk(chunk4);

	////chunk 5
	//Chunk* chunk5 = new Chunk(IntVector2(0, 3));
	//ActivateChunk(chunk5);
}

//  =========================================================================================
void World::Update(float deltaSeconds)
{
	UpdateFromInput(deltaSeconds);

	ActivateChunks();
	GenerateDirtyChunks();
	DeactivateChunks();
}

//  =========================================================================================
void World::Render()
{
	Renderer* theRenderer = Renderer::GetInstance();

	//always do this first at the beginning of the frame's render
	theRenderer->SetCamera(m_engineCamera);	
	theRenderer->ClearDepth(1.f);
	theRenderer->ClearColor(Rgba::BLACK);

	//hammer over camera view matrix
	m_gameCamera->CreateFliippedViewMatrix(m_engineCamera->m_viewMatrix);

	MeshBuilder builder;

	builder.CreateBasis(Matrix44::IDENTITY, Vector3::ZERO, 1.f);
	Mesh* axisMesh = builder.CreateMesh<VertexPCU>();

	/*Mesh* axisMesh = CreateBasis(Vector3::ZERO, 1.f);*/

	//draw axis for debugging
	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default_always_depth"));
	theRenderer->DrawMesh(axisMesh);

	theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));
	theRenderer->DrawMesh(axisMesh);

	//render all chunks
	RenderChunks();

	//generate and render test mesh
	theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));
	theRenderer->SetTexture(*GetTerrainSprites()->GetSpriteSheetTexture());

	Mesh* blockMesh = MakeBlockToMesh(Vector3(-2.f, -2.f, 0.f), 1);
	theRenderer->DrawMesh(blockMesh);

	delete(axisMesh);
	axisMesh = nullptr;

	delete(blockMesh);
	blockMesh = nullptr;
}

//  =========================================================================================
void World::UpdateFromInput(float deltaSeconds)
{
	InputSystem* theInput = InputSystem::GetInstance();

	Vector2 mouseDelta = Vector2::ZERO;
	mouseDelta = InputSystem::GetInstance()->GetMouse()->GetMouseDelta();

//calculate rotation for camera and use same rotation for player
//m_camera->m_transform->AddRotation(Vector3(mouseDelta.y, mouseDelta.x, 0.f) * deltaSeconds * 10.f);

m_gameCamera->m_yawDegreesZ += -mouseDelta.x * 0.05f;
m_gameCamera->m_pitchDegreesY += mouseDelta.y * 0.05f;

m_gameCamera->m_yawDegreesZ = Modulus(m_gameCamera->m_yawDegreesZ, 360.f);
m_gameCamera->m_pitchDegreesY = ClampFloat(m_gameCamera->m_pitchDegreesY, -90.f, 90.f);

/*Vector3 rotation = Vector3(clampedX, clampedY, 0.f);
m_camera->m_transform->SetLocalRotation(Vector3(rotation));*/

Vector3 cameraForward = Vector3(CosDegrees(m_gameCamera->m_yawDegreesZ), SinDegrees(m_gameCamera->m_yawDegreesZ), 0);
Vector3 cameraRight = Vector3(SinDegrees(m_gameCamera->m_yawDegreesZ), -CosDegrees(m_gameCamera->m_yawDegreesZ), 0);

Vector3 positionToAdd = Vector3::ZERO;
Vector3 positionAtStartOfFrame = positionToAdd;

//update movement
//forward (x)
if (theInput->IsKeyPressed(theInput->KEYBOARD_W))
{
	//calculate movement for camera and use same movement for ship and light
	positionToAdd = cameraForward * deltaSeconds * 10.f;
	m_gameCamera->Translate(positionToAdd);
}

//backward (-x)
if (theInput->IsKeyPressed(theInput->KEYBOARD_S))
{
	positionToAdd = -cameraForward * deltaSeconds * 10.f;
	m_gameCamera->Translate(positionToAdd);
}

//left is north (y)
if (theInput->IsKeyPressed(theInput->KEYBOARD_A))
{
	positionToAdd = -cameraRight * deltaSeconds * 10.f;
	m_gameCamera->Translate(positionToAdd);
}

//right is south (-y)
if (theInput->IsKeyPressed(theInput->KEYBOARD_D))
{
	positionToAdd = cameraRight * deltaSeconds * 10.f;
	m_gameCamera->Translate(positionToAdd);
}

if (theInput->IsKeyPressed(theInput->KEYBOARD_SPACE) || theInput->IsKeyPressed(theInput->KEYBOARD_E))
{
	positionToAdd = g_worldUp * deltaSeconds * 10.f;
	m_gameCamera->Translate(positionToAdd);
}

if (theInput->IsKeyPressed(theInput->KEYBOARD_CONTROL) || theInput->IsKeyPressed(theInput->KEYBOARD_Q))
{
	positionToAdd = -g_worldUp * deltaSeconds * 10.f;
	m_gameCamera->Translate(positionToAdd);
}


//deltaSeconds = m_player->UpdateFromInput(deltaSeconds);
//deltaSeconds = UpdateFromInputDebug(deltaSeconds);

// return 
//return deltaSeconds; //new deltaSeconds
}

//  =========================================================================================
void World::UpdateChunks()
{
	std::map<IntVector2, Chunk*>::iterator chunkIterator;

	for (chunkIterator = m_activeChunks.begin(); chunkIterator != m_activeChunks.end(); ++chunkIterator)
	{
		chunkIterator->second->Update();
	}
}

//  =========================================================================================
void World::RenderChunks()
{
	std::map<IntVector2, Chunk*>::iterator chunkIterator;

	for (chunkIterator = m_activeChunks.begin(); chunkIterator != m_activeChunks.end(); ++chunkIterator)
	{
		chunkIterator->second->Render();
	}
}

//  =========================================================================================
void World::ActivateChunks()
{
	IntVector2 playerChunkCoords = IntVector2(floorf(m_gameCamera->m_position.x / BLOCKS_WIDE_X), floorf(m_gameCamera->m_position.y / BLOCKS_WIDE_Y));
	Vector2 playerChunkCenter = Vector2(playerChunkCoords) + Vector2(0.5f, 0.5f);

	//loop through my neighborhood and find the first chunk that needs activating(loaded or generated, hooked up)
	for (int chunkIndex = 0; chunkIndex < (int)m_neighborHoodBuildOrder.size(); ++chunkIndex)
	{
		IntVector2 worldChunkCoord = playerChunkCoords + m_neighborHoodBuildOrder[chunkIndex];
		std::map<IntVector2, Chunk*>::iterator chunkIterator =  m_activeChunks.find(worldChunkCoord);

		if (chunkIterator == m_activeChunks.end())
		{
			//we didn't find the chunk in the activation list so we must confirm we should add it
			if (CompareDistanceFromLocationToLocationLessThanRadius(playerChunkCenter, playerChunkCenter + Vector2(m_neighborHoodBuildOrder[chunkIndex]), CHUNK_ACTIVATION_RADIUS))
			{
				ActivateChunk(worldChunkCoord);

				//for now we are only activating one chunk a frame
				return;
			}
		}	
	}
}

//  =========================================================================================
void World::GenerateDirtyChunks()
{
	//loop through my active chunk list and generate the mesh for anyone marked as dirty
}

//  =========================================================================================
void World::DeactivateChunks()
{
	//loop through my active chunk list and determine find the first active chunk out of range to be deactivated(saved if needed, unhooked, deleted)

	IntVector2 playerChunkCoords = IntVector2(floorf(m_gameCamera->m_position.x / BLOCKS_WIDE_X), floorf(m_gameCamera->m_position.y / BLOCKS_WIDE_Y));
	Vector2 playerChunkCenter = Vector2(playerChunkCoords) + Vector2(0.5f, 0.5f);

	//loop through my neighborhood and find the first chunk that needs activating(loaded or generated, hooked up)
	std::map<IntVector2, Chunk*>::iterator activeChunkIterator;
	for (activeChunkIterator = m_activeChunks.begin(); activeChunkIterator != m_activeChunks.end(); ++activeChunkIterator)
	{
		//we didn't find the chunk in the activation list so we must confirm we should add it
		if (CompareDistanceFromLocationToLocationGreaterThanRadius(playerChunkCenter, Vector2(activeChunkIterator->first) + Vector2(0.5f, 0.5f), CHUNK_DISTANCE_DEACTIVATION))
		{
			DeactivateChunk(activeChunkIterator->first);

			//for now we are only activating one chunk a frame
			return;
		}
			
	}
}

//  =========================================================================================
void World::ActivateChunk(const IntVector2& chunkCoordinates)
{
	Chunk* chunk = new Chunk(chunkCoordinates);
	m_activeChunks.insert(std::pair<IntVector2, Chunk*>(chunk->m_chunkCoords, chunk));
}

//  =========================================================================================
void World::DeactivateChunk(const IntVector2& keyVal)
{
	//before deletion need to check saving and unhooking of neighbors

	Chunk* chunk = m_activeChunks.at(keyVal);

	delete(chunk);
	chunk = nullptr;

	m_activeChunks.erase(keyVal);
}

//  =========================================================================================
void World::GenerateChunkBuildOrderCheatSheet()
{
	float maxNumActiveBlocks = CHUNK_DISTANCE_DEACTIVATION;

	for (int chunkY = -maxNumActiveBlocks; chunkY <= maxNumActiveBlocks; ++chunkY)
	{
		for (int chunkX = -maxNumActiveBlocks; chunkX <= maxNumActiveBlocks; ++chunkX)
		{
			m_neighborHoodBuildOrder.push_back(IntVector2(chunkX, chunkY));
		}		
	}

	//now that we have all the indices we want relative to their distance from zero, let's sort them so we have our list
	std::sort(m_neighborHoodBuildOrder.begin(), m_neighborHoodBuildOrder.end(), CompareDistanceFromZeroLessThan);
}

//  =========================================================================================
Mesh* World::CreateBasis(const Vector3& center, float width, float scale)
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
bool CompareDistanceFromZeroLessThan(const IntVector2& first, const IntVector2& second)
{
	int firstDistanceFromZero = GetDistanceSquared(IntVector2::ZERO, first);
	int secondDistanceFromZero = GetDistanceSquared(IntVector2::ZERO, second);

	if(firstDistanceFromZero < secondDistanceFromZero)
		return true;

	return false;
}

//  =========================================================================================
bool CompareDistanceFromLocationToLocationLessThanRadius(const Vector2& startLocation, const Vector2& endLocation, const float radius)
{
	float distanceSquared = GetDistanceSquared(startLocation, endLocation);
	float radiusSquared = radius * radius;

	if(distanceSquared < radiusSquared)
		return true;

	return false;

}

//  =========================================================================================
bool CompareDistanceFromLocationToLocationGreaterThanRadius(const Vector2& startLocation, const Vector2& endLocation, const float radius)
{
	float distanceSquared = GetDistanceSquared(startLocation, endLocation);
	float radiusSquared = radius * radius;

	if(distanceSquared > radiusSquared)
		return true;

	return false;

}

