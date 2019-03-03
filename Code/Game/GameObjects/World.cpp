#include "Game\GameObjects\World.hpp"
#include "Game\Game.hpp"
#include "Game\GameStates\PlayingState.hpp"
#include "Game\ObjectMeshbuilder.hpp"
#include "Game\GameCommon.hpp"
#include "Game\Helpers\Ray.hpp"
#include "Game\Helpers\GameRendererHelpers.hpp"
#include "Game\Helpers\ChunkFileLoader.hpp"
#include "Game\Definitions\BlockDefinition.hpp"
#include "Game\GameObjects\Block.hpp"
#include "Game\GameObjects\BlockLocator.hpp"
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
#include "Engine\Renderer\RendererTypes.hpp"
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
	m_gameCamera->Translate(Vector3(0.f, 0.f, 180.f));

	//m_camera->m_skybox = new Skybox("Data/Images/galaxy2.png");
	theRenderer->SetAmbientLightIntensity(0.15f);

	GenerateChunkBuildOrderCheatSheet();
	LoadSavedChunkReferences();

	theRenderer->SetLineWidth(3.f);

	InitializeSelectableBlockList();

	//int test1 = CountNumWordsInString(nullptr);

	//char str[] = { 'h', 'i'};

	//int test2 = CountNumWordsInString(str);
	//int value = CountNumWordsInString(" hello");
	//int value1 = CountNumWordsInString("a");
	//int value2 = CountNumWordsInString("A BIG DOG");
}

//  =========================================================================================
void World::Update(float deltaSeconds)
{
	UpdateFromInput(deltaSeconds);
	UpdatePlayerViewPosition();

	ActivateChunks();

	UpdateDirtyLighting();

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

	//render
	RenderChunks();
	RenderDebug();
	RenderUI();
}

//  =========================================================================================
void World::UpdateFromInput(float deltaSeconds)
{
	InputSystem* theInput = InputSystem::GetInstance();
	Game* theGame = Game::GetInstance();

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

	Vector3 cameraCardinalForward = Vector3(CosDegrees(m_gameCamera->m_yawDegreesZ), SinDegrees(m_gameCamera->m_yawDegreesZ), 0);
	cameraCardinalForward.Normalize();
	Vector3 cameraRight = Vector3(SinDegrees(m_gameCamera->m_yawDegreesZ), -CosDegrees(m_gameCamera->m_yawDegreesZ), 0);

	Vector3 positionToAdd = Vector3::ZERO;
	Vector3 positionAtStartOfFrame = positionToAdd;

	//update movement
	//forward (x)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_W))
	{
		//calculate movement for camera and use same movement for ship and light
		positionToAdd = cameraCardinalForward * deltaSeconds * PLAYER_MOVEMENT_SPEED;
	}

	//backward (-x)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_S))
	{
		positionToAdd = -cameraCardinalForward * deltaSeconds * PLAYER_MOVEMENT_SPEED;
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

	//up is (+z)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_SPACE) || theInput->IsKeyPressed(theInput->KEYBOARD_E))
	{
		positionToAdd = g_worldUp * deltaSeconds * PLAYER_MOVEMENT_SPEED;
	}

	//up is (-z)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_CONTROL) || theInput->IsKeyPressed(theInput->KEYBOARD_Q))
	{
		positionToAdd = -g_worldUp * deltaSeconds * PLAYER_MOVEMENT_SPEED;
	}

	//sprint move speed (twice as fast)
	if (theInput->IsKeyPressed(theInput->KEYBOARD_SHIFT))
	{
		positionToAdd *= 2.f;
	}

	//reset chunks
	if (theInput->WasKeyJustPressed(theInput->KEYBOARD_U) && theGame->m_inputDelayTimer->HasElapsed())
	{
		DeactivateAllChunks();
	}

	//dig block
	if (theInput->WasKeyJustPressed(theInput->MOUSE_LEFT_CLICK) && theGame->m_inputDelayTimer->HasElapsed())
	{
		DigBlock();
		theGame->m_inputDelayTimer->Reset();
	}

	//place block
	if (theInput->WasKeyJustPressed(theInput->MOUSE_RIGHT_CLICK) && theGame->m_inputDelayTimer->HasElapsed())
	{
		PlaceBlock();
		theGame->m_inputDelayTimer->Reset();
	}

	//mouse wheel scrolling for changing blocks
	if ((theInput->GetMouseWheelUp() || theInput->GetMouseWheelDown()) && theGame->m_inputDelayTimer->HasElapsed())
	{
		if (theInput->GetMouseWheelUp())
		{
			m_selectedBlockIndex++;
			m_selectedBlockIndex = m_selectedBlockIndex % ((int)m_selectableBlockTypes.size());
		}
		else if (theInput->GetMouseWheelDown())
		{
			m_selectedBlockIndex--;
			if(m_selectedBlockIndex < 0)
				m_selectedBlockIndex = UINT8_MAX;

			m_selectedBlockIndex = m_selectedBlockIndex % ((int)m_selectableBlockTypes.size());
		}
	}

	//exit player position and freelook
	if (theInput->WasKeyJustPressed(theInput->KEYBOARD_PAGEUP) && theGame->m_inputDelayTimer->HasElapsed())
	{
		ToggleCameraViewLocked();
		theGame->m_inputDelayTimer->Reset();
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
}

//  =========================================================================================
void World::UpdatePlayerViewPosition()
{
	if (!IsCameraViewLocked())
	{
		Vector3 cameraForward = Vector3(CosDegrees(m_gameCamera->m_yawDegreesZ) * CosDegrees(m_gameCamera->m_pitchDegreesY)
			, SinDegrees(m_gameCamera->m_yawDegreesZ) * CosDegrees(m_gameCamera->m_pitchDegreesY),
			-1.f * SinDegrees(m_gameCamera->m_pitchDegreesY));

		CopyCameraDataToPlayerView(m_gameCamera->m_position, cameraForward);
	}	
}

//  =========================================================================================
void World::UpdateDirtyLighting()
{
	//process until no blocks are dirty
	while (m_blocksWithDirtyLighting.size() > 0)
	{
		BlockLocator blockToProcess = m_blocksWithDirtyLighting.front();
		m_blocksWithDirtyLighting.pop_front();

		ProcessLightingForBlock(blockToProcess);
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
void World::RenderUI()
{
	Renderer* theRenderer = Renderer::GetInstance();
	theRenderer->SetCamera(m_uiCamera);

	Mesh* texturedUIMesh = CreateTexturedUIMesh();
	if (texturedUIMesh != nullptr)
	{
		theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));
		theRenderer->SetTexture(*GetTerrainSprites()->GetSpriteSheetTexture());
		theRenderer->m_defaultShader->SetFrontFace(WIND_COUNTER_CLOCKWISE);

		theRenderer->DrawMesh(texturedUIMesh);
		delete(texturedUIMesh);
	}

	//handle text
	Mesh* textMesh = CreateUITextMesh();
	if (textMesh != nullptr)
	{
		theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("text"));
		theRenderer->DrawMesh(textMesh);
		delete(textMesh);
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
void World::RenderDebug()
{
	Renderer* theRenderer = Renderer::GetInstance();

	// debug drawing ----------------------------------------------
	MeshBuilder builder;

	builder.CreateBasis(Matrix44::IDENTITY, Vector3::ZERO, 1.f);
	Mesh* axisMesh = builder.CreateMesh<VertexPCU>();

	//draw axis for debugging
	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default_always_depth"));
	theRenderer->DrawMesh(axisMesh);	

	theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));
	theRenderer->DrawMesh(axisMesh);

	Mesh* raycastBlockHighlightMesh = nullptr;
	Mesh* raycastMesh = nullptr;

	//draw raycastline
	raycastMesh = CreateLine(m_raycastResult.m_ray.m_startPosition, m_raycastResult.m_impactWorldPosition, Rgba::BLUE);
	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	theRenderer->DrawMesh(raycastMesh);

	delete(raycastMesh);
	raycastMesh = nullptr;

	raycastMesh = CreateLine(m_raycastResult.m_impactWorldPosition, m_raycastResult.m_endPosition, Rgba::PINK);
	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	theRenderer->DrawMesh(raycastMesh);

	delete(raycastMesh);
	raycastMesh = nullptr;

	raycastMesh = CreateDebugStar(m_raycastResult.m_impactWorldPosition, Rgba::BLUE, 0.05f);
	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	theRenderer->DrawMesh(raycastMesh);

	delete(raycastMesh);
	raycastMesh = nullptr;

	raycastMesh = CreateDebugStar(m_raycastResult.m_endPosition, Rgba::PINK, 0.05f);
	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	theRenderer->DrawMesh(raycastMesh);

	delete(raycastMesh);
	raycastMesh = nullptr;

	/* NEIGHBORING BLOCK LOCATOR DEBUG =========================================================================================

	//IntVector2 chunkCoordsOfWorldPosition = IntVector2((int)floorf(m_raycastResult.m_ray.m_startPosition.x * BLOCKS_WIDE_X_DIVISOR), (int)floorf(m_raycastResult.m_ray.m_startPosition.y * BLOCKS_WIDE_Y_DIVISOR));

	//std::map<IntVector2, Chunk*>::iterator activeChunkIterator = m_activeChunks.find(chunkCoordsOfWorldPosition);
	//Chunk* currentRayChunk = activeChunkIterator->second;

	//uint outBlockIndex;
	//bool success = currentRayChunk->GetBlockIndexForWorldPositionWithinBounds(outBlockIndex, m_raycastResult.m_ray.m_startPosition);
	//
	////render me
	//BlockLocator locator = BlockLocator(currentRayChunk, outBlockIndex);
	//Vector3 worldCenter = currentRayChunk->GetBlockWorldCenterForBlockIndex(outBlockIndex);
	//raycastMesh = CreateBlockOutline(worldCenter, Rgba::WHITE);
	//theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	//theRenderer->DrawMesh(raycastMesh);

	//delete(raycastMesh);
	//raycastMesh = nullptr;

	////render above
	//BlockLocator aboveLocator = locator.GetBlockLocatorAbove();
	//if (aboveLocator.IsValid())
	//{	
	//	worldCenter = aboveLocator.m_chunk->GetBlockWorldCenterForBlockIndex(aboveLocator.m_blockIndex);
	//	raycastMesh = CreateBlockOutline(worldCenter, Rgba::BLUE);
	//	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	//	theRenderer->DrawMesh(raycastMesh);

	//	delete(raycastMesh);
	//	raycastMesh = nullptr;
	//}

	//BlockLocator belowLocator = locator.GetBlockLocatorBelow();
	//if (belowLocator.IsValid())
	//{
	//	worldCenter = belowLocator.m_chunk->GetBlockWorldCenterForBlockIndex(belowLocator.m_blockIndex);
	//	raycastMesh = CreateBlockOutline(worldCenter, Rgba::LIGHT_BLUE);
	//	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	//	theRenderer->DrawMesh(raycastMesh);

	//	delete(raycastMesh);
	//	raycastMesh = nullptr;
	//}
	//
	//BlockLocator eastLocator = locator.GetBlockLocatorToEast();
	//if (eastLocator.IsValid())
	//{
	//	worldCenter = eastLocator.m_chunk->GetBlockWorldCenterForBlockIndex(eastLocator.m_blockIndex);
	//	raycastMesh = CreateBlockOutline(worldCenter, Rgba::RED);
	//	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	//	theRenderer->DrawMesh(raycastMesh);

	//	delete(raycastMesh);
	//	raycastMesh = nullptr;
	//}

	//BlockLocator westLocator = locator.GetBlockLocatorToWest();
	//if (westLocator.IsValid())
	//{
	//	worldCenter = westLocator.m_chunk->GetBlockWorldCenterForBlockIndex(westLocator.m_blockIndex);
	//	raycastMesh = CreateBlockOutline(worldCenter, Rgba::LIGHT_RED);
	//	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	//	theRenderer->DrawMesh(raycastMesh);

	//	delete(raycastMesh);
	//	raycastMesh = nullptr;
	//}

	//BlockLocator northLocator = locator.GetBlockLocatorToNorth();
	//if (northLocator.IsValid())
	//{
	//	worldCenter = northLocator.m_chunk->GetBlockWorldCenterForBlockIndex(northLocator.m_blockIndex);
	//	raycastMesh = CreateBlockOutline(worldCenter, Rgba::GREEN);
	//	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	//	theRenderer->DrawMesh(raycastMesh);

	//	delete(raycastMesh);
	//	raycastMesh = nullptr;
	//}

	//BlockLocator southLocator = locator.GetBlockLocatorToSouth();
	//if (southLocator.IsValid())
	//{
	//	worldCenter = southLocator.m_chunk->GetBlockWorldCenterForBlockIndex(southLocator.m_blockIndex);
	//	raycastMesh = CreateBlockOutline(worldCenter, Rgba::LIGHT_GREEN);
	//	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
	//	theRenderer->DrawMesh(raycastMesh);

	//	delete(raycastMesh);
	//	raycastMesh = nullptr;
	//}

	//  =========================================================================================
	*/

	//draw raycast block highlighting
	if (m_raycastResult.m_didImpact && m_raycastResult.m_impactBlockLocator.IsValid())
	{
		Vector3 blockCenter = m_raycastResult.m_impactBlockLocator.m_chunk->GetBlockWorldCenterForBlockIndex(m_raycastResult.m_impactBlockLocator.m_blockIndex);
		raycastBlockHighlightMesh = CreateBlockOutline(blockCenter, Rgba::WHITE, 1.01f);
		theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
		theRenderer->DrawMesh(raycastBlockHighlightMesh);

		delete(raycastBlockHighlightMesh);
		raycastBlockHighlightMesh = nullptr;

		raycastBlockHighlightMesh = CreateBlockHighlightBox(blockCenter, m_raycastResult.m_impactNormal, 1.01f);
		theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));
		theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));
		theRenderer->DrawMesh(raycastBlockHighlightMesh);
		theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));

		delete(raycastBlockHighlightMesh);
		raycastBlockHighlightMesh = nullptr;
	}	

	//cleanup
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

	bool success = ReadChunkDataFromFile(chunk);

	if (!success)
	{
		//randomization
		chunk->GenerateBlockDataWithPerlin();
	}
	else
	{
		int i = 0;
	}

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

	//handle lighting
	InitializeChunkLighting(chunk);
}

//  =========================================================================================
void World::InitializeChunkLighting(Chunk* chunk)
{
	ASSERT_OR_DIE(chunk != nullptr, "INVALID CHUNK PASSED TO 'INITIALIZECHUNKLIGHTING'!!!");
	
	for (int blockIndex = 0; blockIndex < BLOCKS_PER_CHUNK; ++blockIndex)
	{
		BlockLocator block = BlockLocator(chunk, blockIndex);

		//if the block emits light, mark it as dirty
		if (chunk->m_blocks[blockIndex].DoesEmitLight())
		{
			AddBlockLocatorToDirtyLightingQueue(block);
		}

		//if the block is on an edge and is not visible, mark it as dirty
		if (block.IsBlockIndexOnEdgeFast() && !block.GetBlock()->IsVisible())
		{
			AddBlockLocatorToDirtyLightingQueue(block);
		}		
	}
}

//  =========================================================================================
void World::DeactivateChunk(const IntVector2& keyVal)
{
	//before deletion need to check saving and unhooking of neighbors

	Chunk* chunk = m_activeChunks.at(keyVal);

	if (chunk->m_doesRequireSave)
	{
		bool success = WriteChunkDataToFile(chunk);

		ASSERT_OR_DIE(success, "CHUNK SAVE INVALID");
	}

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
void World::LoadSavedChunkReferences()
{
	TODO("Load chunks from start so we have a list");
}

//  =========================================================================================
void World::ProcessLightingForBlock(BlockLocator blockLocator)
{
	Block* block = blockLocator.GetBlock();

	//if this is the invalid block, do not process
	if(!block->IsValid())
		return;

	BlockDefinition* blockDef = BlockDefinition::GetDefinitionById(block->m_type);

	//get all neighbors ----------------------------------------------
	BlockLocator northNeighbor = blockLocator.GetBlockLocatorToNorth();
	Block* northBlock = northNeighbor.GetBlock();

	BlockLocator southNeighbor = blockLocator.GetBlockLocatorToSouth();
	Block* southBlock = southNeighbor.GetBlock();

	BlockLocator eastNeighbor = blockLocator.GetBlockLocatorToEast();
	Block* eastBlock = eastNeighbor.GetBlock();

	BlockLocator westNeighbor = blockLocator.GetBlockLocatorToWest();
	Block* westBlock = westNeighbor.GetBlock();

	BlockLocator aboveNeighbor = blockLocator.GetBlockLocatorAbove();
	Block* aboveBlock = aboveNeighbor.GetBlock();

	BlockLocator belowNeighbor = blockLocator.GetBlockLocatorBelow();
	Block* belowBlock = belowNeighbor.GetBlock();

	//remove the bit flag for the block saying it's dirty ----------------------------------------------
	block->SetLightingInDirtyListFlag(false);

	uint8 minLightingValue = blockDef->m_minimumLightingValue;
	uint8 currentLightingValue = block->GetIndoorLightingValue();
	uint8 newLightingValue = currentLightingValue;

	uint8 brightestNeighborLightingValue = 0;

	//process north neighbor ----------------------------------------------	
	if (northNeighbor.IsValid())
	{
		uint8 neighborLightingValue = northBlock->GetIndoorLightingValue();

		if(neighborLightingValue > brightestNeighborLightingValue)
			brightestNeighborLightingValue = neighborLightingValue;
	}

	//process south neighbor ----------------------------------------------
	if (southNeighbor.IsValid())
	{
		uint8 neighborLightingValue = southBlock->GetIndoorLightingValue();

		if(neighborLightingValue > brightestNeighborLightingValue)
			brightestNeighborLightingValue = neighborLightingValue;
	}

	//process east neighbor ----------------------------------------------
	if (eastNeighbor.IsValid())
	{
		uint8 neighborLightingValue = eastBlock->GetIndoorLightingValue();

		if(neighborLightingValue > brightestNeighborLightingValue)
			brightestNeighborLightingValue = neighborLightingValue;
	}

	//process west neighbor ----------------------------------------------
	if (westNeighbor.IsValid())
	{
		uint8 neighborLightingValue = westBlock->GetIndoorLightingValue();

		if(neighborLightingValue > brightestNeighborLightingValue)
			brightestNeighborLightingValue = neighborLightingValue;
	}

	//process above neighbor ----------------------------------------------
	if (aboveNeighbor.IsValid())
	{
		uint8 neighborLightingValue = aboveBlock->GetIndoorLightingValue();

		if(neighborLightingValue > brightestNeighborLightingValue)
			brightestNeighborLightingValue = neighborLightingValue;
	}

	// process below neighbor ----------------------------------------------
	if (belowNeighbor.IsValid())
	{
		uint8 neighborLightingValue = belowBlock->GetIndoorLightingValue();

		if(neighborLightingValue > brightestNeighborLightingValue)
			brightestNeighborLightingValue = neighborLightingValue;
	}

	//now that we have checked all the neighbors, change the value to 1 less than the highest neighbor ----------------------------------------------
	if ((brightestNeighborLightingValue - 1) > minLightingValue && (brightestNeighborLightingValue - 1) >= 0)
		newLightingValue = brightestNeighborLightingValue - 1;
	else
		newLightingValue = minLightingValue;

	//if we have a change, update the value to the new lighting value and dirty my neighbors
	if (newLightingValue != currentLightingValue)
	{
		if (!block->IsFullOpaque() || block->DoesEmitLight())
		{
			block->SetIndoorLightingValue(newLightingValue);
			blockLocator.m_chunk->m_isMeshDirty = true;
		}	

		//foreach neighbor, if they AREN'T opaque, add them to the dirty list
		if(northBlock->IsValid() && !northBlock->IsFullOpaque())
			AddBlockLocatorToDirtyLightingQueue(northNeighbor);
		if(northBlock->IsValid() &&!southBlock->IsFullOpaque())
			AddBlockLocatorToDirtyLightingQueue(southNeighbor);
		if(northBlock->IsValid() &&!eastBlock->IsFullOpaque())
			AddBlockLocatorToDirtyLightingQueue(eastNeighbor);
		if(northBlock->IsValid() &&!westBlock->IsFullOpaque())
			AddBlockLocatorToDirtyLightingQueue(westNeighbor);
		if(northBlock->IsValid() &&!aboveBlock->IsFullOpaque())
			AddBlockLocatorToDirtyLightingQueue(aboveNeighbor);
		if(northBlock->IsValid() &&!belowBlock->IsFullOpaque())
			AddBlockLocatorToDirtyLightingQueue(belowNeighbor);
	}
}

//  =========================================================================================
void World::AddBlockLocatorToDirtyLightingQueue(BlockLocator blockLocator)
{
	Block* block = blockLocator.GetBlock();

	//if the block is fully opaque OR it's already in the list, there is no need to add it to the dirty list
	if(block->IsLightingInDirtyList())
		return;

	block->SetLightingInDirtyListFlag(true);

	m_blocksWithDirtyLighting.emplace_back(blockLocator);
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
	Vector3 endPosition = raycast.m_startPosition + (forward * maxDistance);	
	Vector3 currentPosition = raycast.m_startPosition;
	IntVector3 currentCoordinates = currentPosition.FloorAndCastToInt();
	float totalStepDistance = 0.f;
	IntVector3 movementDirection = IntVector3::ZERO;
	bool didImpact = false;

	//early out if we started from inside a blcok that is impassable
	if(blockLocator.GetBlock()->IsSolid())
	{
		isRaycastComplete = true;
		didImpact = true;
	}
	
	while (!isRaycastComplete)
	{		
		bool didStepDuringIteration = false;

		totalStepDistance += RAYCAST_STEP_AMOUNT;

		currentPosition += raycast.m_direction * RAYCAST_STEP_AMOUNT;
		IntVector3 newCoordinates = currentPosition.FloorAndCastToInt();
		IntVector3 coordinateDifference = currentCoordinates - newCoordinates;

		if (newCoordinates.x > currentCoordinates.x)
		{
			blockLocator.StepEast();
			movementDirection = IntVector3(1, 0, 0); 
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;

			if (blockLocator.IsValid())
			{
				if(blockLocator.GetBlock()->IsSolid())
				{
					isRaycastComplete = true;
					didImpact = true;
					break;
				}
			}
			else
			{
				isRaycastComplete = true;
				didImpact = false;
			}
		}
		if (newCoordinates.x < currentCoordinates.x)
		{
			blockLocator.StepWest();
			movementDirection = IntVector3(-1, 0, 0);
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;

			if (blockLocator.IsValid())
			{
				if(blockLocator.GetBlock()->IsSolid())
				{
					isRaycastComplete = true;
					didImpact = true;
					break;
				}
			}
			else
			{
				isRaycastComplete = true;
				didImpact = false;
			}
		}
		if (newCoordinates.y > currentCoordinates.y)
		{
			blockLocator.StepNorth();
			movementDirection = IntVector3(0, 1, 0);
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;

			if (blockLocator.IsValid())
			{
				if(blockLocator.GetBlock()->IsSolid())
				{
					isRaycastComplete = true;
					didImpact = true;
					break;
				}
			}
			else
			{
				isRaycastComplete = true;
				didImpact = false;
			}
		}
		if (newCoordinates.y < currentCoordinates.y)
		{
			blockLocator.StepSouth();
			movementDirection = IntVector3(0, -1, 0);
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;

			if (blockLocator.IsValid())
			{
				if(blockLocator.GetBlock()->IsSolid())
				{
					isRaycastComplete = true;
					didImpact = true;
					break;
				}
			}
			else
			{
				isRaycastComplete = true;
				didImpact = false;
			}
		}
		if (newCoordinates.z > currentCoordinates.z)
		{
			blockLocator.StepUp();
			movementDirection = IntVector3(0, 0, 1);
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;

			if (blockLocator.IsValid())
			{
				if(blockLocator.GetBlock()->IsSolid())
				{
					isRaycastComplete = true;
					didImpact = true;
					break;
				}
			}
			else
			{
				isRaycastComplete = true;
				didImpact = false;
			}
		}
		if (newCoordinates.z < currentCoordinates.z)
		{
			blockLocator.StepDown();
			movementDirection = IntVector3(0, 0, -1);
			didStepDuringIteration = true;
			currentCoordinates = newCoordinates;

			if (blockLocator.IsValid())
			{
				if(blockLocator.GetBlock()->IsSolid())
				{
					isRaycastComplete = true;
					didImpact = true;
					break;
				}
			}
			else
			{
				isRaycastComplete = true;
				didImpact = false;
			}
		}

		//if we've walked the length of the cast, we didn't hit anything
		if (totalStepDistance >= raycast.m_maxDistance)
		{
			isRaycastComplete = true;
		}

		didStepDuringIteration = false;
	}

	return RaycastResult(raycast, blockLocator, didImpact, currentPosition, endPosition, totalStepDistance, -1 * Vector3(movementDirection));
}

//  =========================================================================================
void World::InitializeSelectableBlockList()
{
	m_selectableBlockTypes.emplace_back(GRASS_BLOCK_ID);
	m_selectableBlockTypes.emplace_back(DIRT_BLOCK_ID);
	m_selectableBlockTypes.emplace_back(STONE_BLOCK_ID);
	m_selectableBlockTypes.emplace_back(GLOWSTONE_BLOCK_ID);
}

//  =========================================================================================
void World::DigBlock()
{
	//early out if we don't have a block to remmove
	if (!m_raycastResult.m_didImpact)
		return;

	BlockLocator impactLocator = m_raycastResult.m_impactBlockLocator;
	if (impactLocator.IsValid())
	{
		Block* impactBlock = impactLocator.GetBlock();
		SetBlockToType(impactBlock, AIR_BLOCK_ID);
		AddBlockLocatorToDirtyLightingQueue(impactLocator);
		impactLocator.m_chunk->m_isMeshDirty = true;
		impactLocator.m_chunk->m_doesRequireSave = true;
		
		//check to see if we need to set the neighboring chunks to dirty
	/*	std::vector<Chunk*> outNeighboringChunks;
		if (impactLocator.IsBlockIndexOnEdge(outNeighboringChunks))
		{
			for (int chunkIndex = 0; chunkIndex < (int)outNeighboringChunks.size(); ++chunkIndex)
			{
				outNeighboringChunks[chunkIndex]->m_isMeshDirty = true;
			}
		}*/
	}
}

//  =========================================================================================
void World::PlaceBlock()
{
	//early out if we don't have a place to put the block
	if (!m_raycastResult.m_didImpact)
		return;

	BlockLocator targetedBlockLocator;

	if (m_raycastResult.m_impactNormal == g_north)
	{
		targetedBlockLocator = m_raycastResult.m_impactBlockLocator.GetBlockLocatorToNorth();
	}
	else if (m_raycastResult.m_impactNormal == g_south)
	{
		targetedBlockLocator = m_raycastResult.m_impactBlockLocator.GetBlockLocatorToSouth();
	}
	else if (m_raycastResult.m_impactNormal == g_east)
	{
		targetedBlockLocator = m_raycastResult.m_impactBlockLocator.GetBlockLocatorToEast();
	}
	else if (m_raycastResult.m_impactNormal == g_west)
	{
		targetedBlockLocator = m_raycastResult.m_impactBlockLocator.GetBlockLocatorToWest();
	}
	else if (m_raycastResult.m_impactNormal == g_worldUp)
	{
		targetedBlockLocator = m_raycastResult.m_impactBlockLocator.GetBlockLocatorAbove();
	}
	else if (m_raycastResult.m_impactNormal == -g_worldUp)
	{
		targetedBlockLocator = m_raycastResult.m_impactBlockLocator.GetBlockLocatorBelow();
	}

	if (targetedBlockLocator.IsValid())
	{
		Block* targetedBlock = targetedBlockLocator.GetBlock();
		SetBlockToType(targetedBlock, m_selectableBlockTypes[m_selectedBlockIndex]);
		AddBlockLocatorToDirtyLightingQueue(targetedBlockLocator);

		targetedBlockLocator.m_chunk->m_isMeshDirty = true;
		targetedBlockLocator.m_chunk->m_doesRequireSave = true;

		//check to see if we need to set the neighboring chunks to dirty
	/*	std::vector<Chunk*> outNeighboringChunks;
		if (targetedBlockLocator.IsBlockIndexOnEdge(outNeighboringChunks))
		{
			for (int chunkIndex = 0; chunkIndex < (int)outNeighboringChunks.size(); ++chunkIndex)
			{
				outNeighboringChunks[chunkIndex]->m_isMeshDirty = true;
			}
		}*/
	}
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
Mesh* World::CreateTexturedUIMesh()
{
	MeshBuilder builder = MeshBuilder();
	Mesh* textMesh = nullptr;
	Window* theWindow = Window::GetInstance();

	// fps counter ----------------------------------------------		
	AABB2 selectedBlockTypeBox = AABB2(theWindow->GetClientWindow(), Vector2(0.95f, 0.9f), Vector2(0.99f, 0.975f));

	BlockDefinition* blockDef = BlockDefinition::GetDefinitionById(m_selectableBlockTypes[m_selectedBlockIndex]);
	AABB2 frontTexCoords = GetTerrainSprites()->GetTexCoordsForSpriteCoords(blockDef->m_frontTexCoords);

	builder.CreateTexturedQuad2D( selectedBlockTypeBox.GetCenter(), selectedBlockTypeBox.GetDimensions(), frontTexCoords.maxs, frontTexCoords.mins, Rgba::WHITE);

	// create mesh ----------------------------------------------
	if (builder.m_vertices.size() > 0)
	{
		textMesh = builder.CreateMesh<VertexPCU>();
	}

	return textMesh;
}

//  =========================================================================================
Mesh* World::CreateUITextMesh()
{
	MeshBuilder builder = MeshBuilder();
	Mesh* textMesh = nullptr;
	Window* theWindow = Window::GetInstance();

	// fps counter ----------------------------------------------		
	AABB2 selectedBlockTypeBox = AABB2(theWindow->GetClientWindow(), Vector2(0.9f, 0.8f), Vector2(0.99f, 0.89f));
	builder.CreateText2DInAABB2( selectedBlockTypeBox.GetCenter(), selectedBlockTypeBox.GetDimensions(), 1.f, Stringf("CurrentBlockType:"), Rgba::WHITE);

	// create mesh ----------------------------------------------
	if (builder.m_vertices.size() > 0)
	{
		textMesh = builder.CreateMesh<VertexPCU>();
	}

	return textMesh;
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

