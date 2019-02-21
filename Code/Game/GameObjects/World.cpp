#include "Game\GameObjects\World.hpp"
#include "Game\Game.hpp"
#include "Game\GameStates\PlayingState.hpp"
#include "Game\ObjectMeshbuilder.hpp"
#include "Game\GameCommon.hpp"
#include "Game\Helpers\Ray.hpp"
#include "Game\Helpers\GameRendererHelpers.hpp"
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
	m_gameCamera->Translate(Vector3(0.f, 0.f, 300.f));

	//m_camera->m_skybox = new Skybox("Data/Images/galaxy2.png");
	theRenderer->SetAmbientLightIntensity(0.15f);

	GenerateChunkBuildOrderCheatSheet();

	theRenderer->SetLineWidth(5.f);
}

//  =========================================================================================
void World::Update(float deltaSeconds)
{
	UpdateFromInput(deltaSeconds);

	ActivateChunks();
	//MarkUnrenderedChunksDirtyWithinRenderRadius();
	GenerateDirtyChunks();
	DeactivateChunks();

	//player raycast
	m_raycastResult = Raycast(m_playerViewPosition, m_playerViewForwardNormalized, RAYCAST_MAX_DISTANCE);
	
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

	builder.CreateBasis(Matrix44::IDENTITY, Vector3( 19.375f, 2.582f, 107.458f ), 1.f);
	Mesh* axisMesh = builder.CreateMesh<VertexPCU>();

	////draw axis for debugging
	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default_always_depth"));
	theRenderer->DrawMesh(axisMesh);

	theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));
	theRenderer->DrawMesh(axisMesh);

	Mesh* raycastBlockHighlightMesh = nullptr;
	Mesh* raycastMesh = nullptr;
	//draw impact highlight box for raycast
	if (m_raycastResult.m_didImpact && m_raycastResult.m_impactBlockLocator.IsValid())
	{
		raycastMesh = CreateLine(m_raycastResult.m_ray.m_startPosition, m_raycastResult.m_impactWorldPosition);
		theRenderer->DrawMesh(raycastMesh);

		Vector3 blockCenter = m_raycastResult.m_impactBlockLocator.m_chunk->GetBlockWorldCenterForBlockIndex(m_raycastResult.m_impactBlockLocator.m_blockIndex);
		raycastBlockHighlightMesh = CreateBlockHighlightBox(blockCenter, m_raycastResult.m_impactNormal);
		theRenderer->DrawMesh(raycastBlockHighlightMesh);
	}

	//render all chunks
	RenderChunks();

	//generate and render test mesh
	/*theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));
	theRenderer->SetTexture(*GetTerrainSprites()->GetSpriteSheetTexture());

	Mesh* blockMesh = MakeBlockToMesh(Vector3(-2.f, -2.f, 0.f), 1);
	theRenderer->DrawMesh(blockMesh);*/


	TODO("later we need to just put these in the world or above it in the game so we aren't newing");
	delete(axisMesh);
	axisMesh = nullptr;

	if (raycastMesh != nullptr)
	{
		delete(raycastMesh);
		raycastMesh = nullptr;
	}

	if (raycastBlockHighlightMesh != nullptr)
	{
		delete(raycastBlockHighlightMesh);
		raycastBlockHighlightMesh = nullptr;
	}	
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
	cameraForward.Normalize();
	Vector3 cameraRight = Vector3(SinDegrees(m_gameCamera->m_yawDegreesZ), -CosDegrees(m_gameCamera->m_yawDegreesZ), 0);

	Vector3 positionToAdd = Vector3::ZERO;
	Vector3 positionAtStartOfFrame = positionToAdd;

	//update movement
	//forward (x)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_W))
	{
		//calculate movement for camera and use same movement for ship and light
		positionToAdd = cameraForward * deltaSeconds * PLAYER_MOVEMENT_SPEED;
	}

	//backward (-x)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_S))
	{
		positionToAdd = -cameraForward * deltaSeconds * PLAYER_MOVEMENT_SPEED;
	}

	//left is north (y)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_A))
	{
		positionToAdd = -cameraRight * deltaSeconds * PLAYER_MOVEMENT_SPEED;
	}

	//right is south (-y)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_D))
	{
		positionToAdd = cameraRight * deltaSeconds * PLAYER_MOVEMENT_SPEED;
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_SPACE) || theInput->IsKeyPressed(theInput->KEYBOARD_E))
	{
		positionToAdd = g_worldUp * deltaSeconds * PLAYER_MOVEMENT_SPEED;
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_CONTROL) || theInput->IsKeyPressed(theInput->KEYBOARD_Q))
	{
		positionToAdd = -g_worldUp * deltaSeconds * PLAYER_MOVEMENT_SPEED;
	}

	if (theInput->IsKeyPressed(theInput->KEYBOARD_LEFT_SHIFT))
	{
		positionToAdd = positionToAdd * 2;
	}

	if (theInput->WasKeyJustPressed(theInput->KEYBOARD_U))
	{
		DeactivateAllChunks();
	}

	if (theInput->WasKeyJustPressed(theInput->MOUSE_LEFT_CLICK))
	{
		DigBlock();
	}

	if (theInput->WasKeyJustPressed(theInput->MOUSE_RIGHT_CLICK))
	{
		PlaceBlock();
	}

	if (theInput->WasKeyJustPressed(theInput->KEYBOARD_Y))
	{
		ToggleCameraViewLocked();
	}

	if(InputSystem::GetInstance()->WasKeyJustPressed(InputSystem::GetInstance()->KEYBOARD_ESCAPE))
	{
		DeactivateAllChunks();
		g_isQuitting = true;
	}

	//deltaSeconds = m_player->UpdateFromInput(deltaSeconds);
	//deltaSeconds = UpdateFromInputDebug(deltaSeconds);

	// return 
	//return deltaSeconds; //new deltaSeconds

	m_gameCamera->Translate(positionToAdd);

	if (!IsCameraViewLocked())
	{
		/*m_gameCamera->CreateFliippedViewMatrix(m_engineCamera->m_viewMatrix);
		Matrix44 transformMatrix = m_engineCamera->m_viewMatrix.InvertFastToNew();
		Vector3 forward = transformMatrix.GetRotation();
		forward.Normalize();*/
		CopyCameraDataToPlayerView(m_gameCamera->m_position, cameraForward);
	}
	
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
		Chunk* chunk = chunkIterator->second;
		if(chunk->m_gpuMesh != nullptr)
			chunk->Render();
	}
}

//  =========================================================================================
void World::ActivateChunks()
{
	IntVector2 playerChunkCoords = IntVector2(floorf(m_gameCamera->m_position.x * BLOCKS_WIDE_X_DIVISOR), floorf(m_gameCamera->m_position.y * BLOCKS_WIDE_Y_DIVISOR));
	Vector2 playerChunkCenter = Vector2(playerChunkCoords) + Vector2(0.5f, 0.5f);

	//loop through my neighborhood and find the first chunk that needs activating(loaded or generated, hooked up)
	for (int chunkIndex = 0; chunkIndex < (int)m_neighborHoodBuildOrder.size(); ++chunkIndex)
	{
		IntVector2 worldChunkCoord = playerChunkCoords + m_neighborHoodBuildOrder[chunkIndex];
		std::map<IntVector2, Chunk*>::iterator chunkIterator =  m_activeChunks.find(worldChunkCoord);

		if (chunkIterator == m_activeChunks.end())
		{
			float distanceOut = 0.f;
			//we didn't find the chunk in the activation list so we must confirm we should add it
			if (CompareDistanceFromLocationToLocationLessThanRadius(distanceOut, playerChunkCenter, playerChunkCenter + Vector2(m_neighborHoodBuildOrder[chunkIndex]), (float)CHUNK_DISTANCE_ACTIVATION))
			{
				ActivateChunk(worldChunkCoord);

				//for now we are only activating one chunk a frame
				return;
			}
			else
			{
				continue;
			}
		}	
	}
}

////  =========================================================================================
//void World::MarkUnrenderedChunksDirtyWithinRenderRadius()
//{
//	IntVector2 playerChunkCoords = IntVector2(floorf(m_gameCamera->m_position.x / BLOCKS_WIDE_X), floorf(m_gameCamera->m_position.y / BLOCKS_WIDE_Y));
//	Vector2 playerChunkCenter = Vector2(playerChunkCoords) + Vector2(0.5f, 0.5f);
//
//	std::map<IntVector2, Chunk*>::iterator activeChunkIterator;
//	for (activeChunkIterator = m_activeChunks.begin(); activeChunkIterator != m_activeChunks.end(); ++activeChunkIterator)
//	{
//		//we the chunk doesn't have a mesh yet, we should check to see if need to draw it.
//		Chunk* chunk = activeChunkIterator->second;
//		
//		//we've already got a mesh for them
//		if(chunk->m_gpuMesh != nullptr)
//			continue;
//
//		float distanceOut = 0.f;
//		if (chunk->DoesHaveAllNeighbors() && CompareDistanceFromLocationToLocationLessThanRadius(distanceOut, playerChunkCenter, Vector2(activeChunkIterator->first) + Vector2(0.5f, 0.5f), (float)CHUNK_DISTANCE_RENDER))
//		{
//			chunk->m_isMeshDirty = true;
//			return;
//		}
//	}
//}

//  =========================================================================================
void World::GenerateDirtyChunks()
{
	IntVector2 playerChunkCoords = IntVector2(floorf(m_gameCamera->m_position.x * BLOCKS_WIDE_X_DIVISOR), floorf(m_gameCamera->m_position.y * BLOCKS_WIDE_Y_DIVISOR));
	Vector2 playerChunkCenter = Vector2(playerChunkCoords) + Vector2(0.5f, 0.5f);

	//loop through my active chunk list and generate the mesh for anyone marked as dirty
	std::map<IntVector2, Chunk*>::iterator activeChunkIterator;
	for (activeChunkIterator = m_activeChunks.begin(); activeChunkIterator != m_activeChunks.end(); ++activeChunkIterator)
	{
		Chunk* chunk = activeChunkIterator->second;
		float distanceOut = 0.f;
		if (chunk->m_isMeshDirty && chunk->DoesHaveAllNeighbors() && CompareDistanceFromLocationToLocationLessThanRadius(distanceOut, playerChunkCenter, Vector2(activeChunkIterator->first) + Vector2(0.5f, 0.5f), (float)CHUNK_DISTANCE_RENDER))
		{
			chunk->GenerateChunkMesh();
			chunk->m_isMeshDirty = false;
		}
	}
}

//  =========================================================================================
void World::DeactivateChunks()
{
	//loop through my active chunk list and determine find the first active chunk out of range to be deactivated(saved if needed, unhooked, deleted)

	IntVector2 playerChunkCoords = IntVector2(floorf(m_gameCamera->m_position.x * BLOCKS_WIDE_X_DIVISOR), floorf(m_gameCamera->m_position.y * BLOCKS_WIDE_Y_DIVISOR));
	Vector2 playerChunkCenter = Vector2(playerChunkCoords) + Vector2(0.5f, 0.5f);

	//loop through my neighborhood and find the first chunk that needs activating(loaded or generated, hooked up)
	std::map<IntVector2, Chunk*>::iterator furthestAwayChunkIterator = m_activeChunks.end();
	float furthestDistance = 0.f;

	std::map<IntVector2, Chunk*>::iterator activeChunkIterator;
	for (activeChunkIterator = m_activeChunks.begin(); activeChunkIterator != m_activeChunks.end(); ++activeChunkIterator)
	{
		//we didn't find the chunk in the activation list so we must confirm we should add it
		float distanceOut = 0.f;
		if (CompareDistanceFromLocationToLocationGreaterThanRadius(distanceOut, playerChunkCenter, Vector2(activeChunkIterator->first) + Vector2(0.5f, 0.5f), (float)CHUNK_DISTANCE_DEACTIVATION))
		{
			//store the chunk that is the furthest away
			if (distanceOut > furthestDistance)
			{
				furthestAwayChunkIterator = activeChunkIterator;
			}
		}			
	}

	if (furthestAwayChunkIterator != m_activeChunks.end())
	{
		DeactivateChunk(furthestAwayChunkIterator->first);
	}
}

//  =========================================================================================
void World::DeactivateAllChunks()
{
	std::map<IntVector2, Chunk*>::iterator activeChunkIterator = m_activeChunks.begin();
	while(m_activeChunks.size() > 0)
	{
		DeactivateChunk(activeChunkIterator->first);
		activeChunkIterator = m_activeChunks.begin();
	}
}

//  =========================================================================================
void World::ActivateChunk(const IntVector2& chunkCoordinates)
{
	Chunk* chunk = new Chunk(chunkCoordinates);
	m_activeChunks.insert(std::pair<IntVector2, Chunk*>(chunk->m_chunkCoords, chunk));

	// hook neighbors if they exist ----------------------------------------------
	std::map<IntVector2, Chunk*>::iterator activeChunkIterator;

	//find north neighbor and hook them if they exist
	activeChunkIterator = m_activeChunks.find(chunkCoordinates + g_chunkNorth);
	if (activeChunkIterator != m_activeChunks.end())
	{
		chunk->AddNeighbor(activeChunkIterator->second, NORTH_NEIGHBOR_TYPE);
	}

	//find west neighbor and hook them if they exist
	activeChunkIterator = m_activeChunks.find(chunkCoordinates + g_chunkWest);
	if (activeChunkIterator != m_activeChunks.end())
	{
		chunk->AddNeighbor(activeChunkIterator->second, WEST_NEIGHBOR_TYPE);
	}

	//find south neighbor and hook them if they exist
	activeChunkIterator = m_activeChunks.find(chunkCoordinates + g_chunkSouth);
	if (activeChunkIterator != m_activeChunks.end())
	{
		chunk->AddNeighbor(activeChunkIterator->second, SOUTH_NEIGHBOR_TYPE);
	}

	//find east neighbor and hook them if they exist
	activeChunkIterator = m_activeChunks.find(chunkCoordinates + g_chunkEast);
	if (activeChunkIterator != m_activeChunks.end())
	{
		chunk->AddNeighbor(activeChunkIterator->second, EAST_NEIGHBOR_TYPE);
	}

	chunk->m_isMeshDirty = true;
}

//  =========================================================================================
void World::DeactivateChunk(const IntVector2& keyVal)
{
	//before deletion need to check saving and unhooking of neighbors

	Chunk* chunk = m_activeChunks.at(keyVal);

	chunk->UnhookNeighbors();

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
RaycastResult World::Raycast(const Vector3& start, const Vector3& forward, float maxDistance)
{

	// create ray that we are moving along line ----------------------------------------------
	RaycastResult result;
	Ray raycast = Ray(start, forward, maxDistance);

	// get chunk information we need for  ----------------------------------------------
	IntVector2 chunkCoordsOfWorldPosition = IntVector2((int)floorf(raycast.m_startPosition.x * BLOCKS_WIDE_X_DIVISOR), (int)floorf(raycast.m_startPosition.y * BLOCKS_WIDE_Y_DIVISOR));

	std::map<IntVector2, Chunk*>::iterator activeChunkIterator = m_activeChunks.find(chunkCoordsOfWorldPosition);

	if (activeChunkIterator == m_activeChunks.end())
	{
		return result;
	}

	Chunk* currentRayChunk = activeChunkIterator->second;

	uint outBlockIndex;
	bool success = currentRayChunk->GetBlockIndexForWorldPositionWithinBounds(outBlockIndex, raycast.m_startPosition);
	
	//we are out of bounds of the chunk bounds
	if (!success)
	{
		return result;
	}
	
	BlockLocator blockLocator = BlockLocator(currentRayChunk, outBlockIndex);
	
	// ready to begin step and sample to find impact ----------------------------------------------
	bool isRaycastComplete = false;
	Vector3 currentPosition = raycast.m_startPosition;
	IntVector3 currentCoordinates = currentPosition.FloorAndCastToInt();
	float totalStepDistance = 0.f;
	IntVector3 movementDirection = IntVector3::ZERO;
	bool didImpact = false;

	//early out if we started from inside a blcok that is impassable
	if(blockLocator.GetBlock()->m_type != 0)
	{
		isRaycastComplete = true;
		didImpact = true;
	}
	
	while (!isRaycastComplete)
	{		
		bool didStepDuringIteration = false;

		totalStepDistance += RAYCAST_STEP_AMOUNT;
		ClampFloat(totalStepDistance, 0.f, raycast.m_maxDistance);

		currentPosition += raycast.m_direction * RAYCAST_STEP_AMOUNT;
		IntVector3 newCoordinates = currentPosition.FloorAndCastToInt();

		if (newCoordinates.x > currentCoordinates.x)
		{
			blockLocator.StepEast();
			movementDirection = IntVector3(1, 0, 0); 
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;
		}
		else if (newCoordinates.x < currentCoordinates.x)
		{
			blockLocator.StepWest();
			movementDirection = IntVector3(-1, 0, 0);
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;
		}
		else if (newCoordinates.y > currentCoordinates.y)
		{
			blockLocator.StepNorth();
			movementDirection = IntVector3(0, 1, 0);
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;
		}
		else if (newCoordinates.y < currentCoordinates.y)
		{
			blockLocator.StepSouth();
			movementDirection = IntVector3(0, -1, 0);
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;
		}
		else if (newCoordinates.z > currentCoordinates.z)
		{
			blockLocator.StepUp();
			movementDirection = IntVector3(0, 0, 1);
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;
		}
		else if (newCoordinates.z > currentCoordinates.z)
		{
			blockLocator.StepDown();
			movementDirection = IntVector3(0, 0, -1);
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;
		}

		//check to see if your current block is solid.
		if (didStepDuringIteration)
		{
			if (blockLocator.IsValid())
			{
				if(blockLocator.GetBlock()->m_type != 0)
				{
					isRaycastComplete = true;
					didImpact = true;
				}
			}
			else
			{
				isRaycastComplete = true;
				didImpact = false;
			}
		}

		//if we've walked the length of the cast, we didn't hit anything
		if (totalStepDistance > raycast.m_maxDistance)
		{
			isRaycastComplete = true;
		}

		didStepDuringIteration = false;
	}

	result = RaycastResult(raycast, blockLocator, didImpact, currentPosition, totalStepDistance, -1 * Vector3(movementDirection));	
	return result;
}

//  =========================================================================================
void World::DigBlock()
{

}

//  =========================================================================================
void World::PlaceBlock()
{

}

//  =========================================================================================
void World::ToggleCameraViewLocked()
{
	m_isCameraViewLocked = !m_isCameraViewLocked;
}

//  =========================================================================================
void World::CopyCameraDataToPlayerView(const Vector3& cameraPosition, const Vector3& cameraForward)
{
	m_playerViewPosition = cameraPosition;
	m_playerViewForwardNormalized = cameraForward;
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
bool CompareDistanceFromLocationToLocationLessThanRadius(float& outDistanceSquared, const Vector2& startLocation, const Vector2& endLocation, const float radius)
{
	outDistanceSquared = GetDistanceSquared(startLocation, endLocation);
	float radiusSquared = radius * radius;

	if(outDistanceSquared < radiusSquared)
		return true;

	return false;

}

//  =========================================================================================
bool CompareDistanceFromLocationToLocationGreaterThanRadius(float& outDistanceSquared, const Vector2& startLocation, const Vector2& endLocation, const float radius)
{
	outDistanceSquared = GetDistanceSquared(startLocation, endLocation);
	float radiusSquared = radius * radius;

	if(outDistanceSquared > radiusSquared)
		return true;

	return false;

}

