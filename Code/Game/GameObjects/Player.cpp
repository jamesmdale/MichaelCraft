#include "Game\GameObjects\Player.hpp"
#include "Game\GameCommon.hpp"
#include "Game\Helpers\GameRendererHelpers.hpp"
#include "Game\GameObjects\Block.hpp"
#include "Game\GameObjects\Chunk.hpp"
#include "Game\GameObjects\World.hpp"
#include "Engine\Renderer\Mesh.hpp"
#include "Engine\Input\InputSystem.hpp"
#include "Engine\Core\EngineCommon.hpp"

//  =========================================================================================
Player::Player(World* world) : Entity(world)
{
	//generate visual bounds
	Vector3 bottomPivot = Vector3(0.f, 0.f, 0.f);
	float widthRadius = g_playerWidth / 2.f;
	float heightRadius = g_playerHeight / 2.f;
	m_visualBounds = AABB3(Vector3::ZERO, widthRadius, widthRadius, heightRadius);

	//generate physics sphere bounds
	m_physicsSphere = Sphere(Vector3::ZERO, widthRadius);

	if (m_visualsMesh == nullptr)
		GenerateDebugVisualsMesh();
	if (m_physicsMesh == nullptr)
		GenerateDebugPhysicsMesh();
}

//  =========================================================================================
Player::~Player()
{
	//nothing to delete yet
}

//  =========================================================================================
void Player::Update(float deltaSeconds)
{
	UNUSED(deltaSeconds);
}

//  =========================================================================================
void Player::UpdatePhysics(float deltaSeconds)
{
	//apply all forces into velocity
	m_velocity += (g_gravity * deltaSeconds * 0.001f);

	//move the player
	m_position += m_velocity;
	UpdateBoundsToCurrentPosition();

	std::vector<BlockLocator> neighborhood;
	GetBlockNeighborhood(neighborhood);

	int pushCount = 0;
	for (int blockIndex = 0; blockIndex < (int)neighborhood.size() && pushCount < 3; ++blockIndex)
	{
		bool didPush = PushOutOfBlock(neighborhood[blockIndex]);
		if (didPush)
			pushCount = 0;
	}

	//UpdateBoundsToCurrentPosition();
}

//  =========================================================================================
void Player::Render()
{
	Renderer* theRenderer = Renderer::GetInstance();
	theRenderer->BindMaterial(theRenderer->CreateOrGetMaterial("default"));
	theRenderer->SetTexture(*theRenderer->CreateOrGetTexture("default"));

	if (m_visualsMesh == nullptr)
		GenerateDebugVisualsMesh();
	if (m_physicsMesh == nullptr)
		GenerateDebugPhysicsMesh();

	Matrix44 meshMatrix;

	//position debug draw
	Sphere sphere = Sphere(m_position, 0.25f);
	Mesh* debugPositionMesh = CreateGameUVSphere(sphere, Rgba::PINK, 0.05f);
	theRenderer->DrawMesh(debugPositionMesh);

	//draw visual bounds
	meshMatrix.SetTranslation(m_visualBounds.GetCenter());
	theRenderer->DrawMesh(m_visualsMesh, meshMatrix);

	Vector3 pivotWorldPosition = GetBottomCenterPivot();

	Mesh* debugPivotMesh = CreateDebugStar(pivotWorldPosition, Rgba::PINK, 0.05f);
	theRenderer->DrawMesh(debugPivotMesh);

	//draw physics sphere
	Matrix44 physicsMatrix;
	Vector3 physicsSpherePosition = Vector3(pivotWorldPosition.x, pivotWorldPosition.y, pivotWorldPosition.z + m_physicsSphere.m_radius);
	physicsMatrix.SetTranslation(physicsSpherePosition);
	theRenderer->DrawMesh(m_physicsMesh, physicsMatrix);

	delete(debugPivotMesh);
	delete(debugPositionMesh);
}

//  =========================================================================================
void Player::PreRender()
{

}

//  =========================================================================================
float Player::UpdateFromInput(float deltaSeconds)
{
	//early out if input is disabled for entity
	if (!DoesReceiveInput())
		return deltaSeconds;

	InputSystem* theInput = InputSystem::GetInstance();

	Vector2 mouseDelta = Vector2::ZERO;
	mouseDelta = InputSystem::GetInstance()->GetMouse()->GetMouseDelta();

	theInput = nullptr;

	return deltaSeconds;
}

//  =========================================================================================
void Player::SetCamera(GameCamera* camera)
{
	m_attachedCamera = camera;
}

//  =========================================================================================
void Player::GenerateDebugVisualsMesh()
{	
	m_visualsMesh = CreateCubeOutline(m_visualBounds, Rgba::LIGHT_PINK_TRANSPARENT);
}

//  =========================================================================================
void Player::GenerateDebugPhysicsMesh()
{
	m_physicsMesh = CreateGameUVSphere(m_physicsSphere, Rgba::LIGHT_BLUE_TRANSPARENT);
}

//  =========================================================================================
Vector3 Player::GetBottomCenterPivot()
{
	Vector3 boundsCenter = m_visualBounds.GetCenter();
	return Vector3(boundsCenter.x, boundsCenter.y, boundsCenter.z - (m_visualBounds.GetDimensions().z / 2.f));
}

//  =========================================================================================
void Player::UpdateBoundsToCurrentPosition()
{
	m_visualBounds.SetCenter(m_position);

	Vector3 bottomCenterPivot = GetBottomCenterPivot();
	m_physicsSphere.m_position = Vector3(bottomCenterPivot.x, bottomCenterPivot.y, bottomCenterPivot.z + m_physicsSphere.m_radius);
}

//  =========================================================================================
void Player::GetBlockNeighborhood(std::vector<BlockLocator>& outBlockLocators)
{
	if (m_world == nullptr)
		return;

	BlockLocator currentBlock = m_world->GetChunkByPositionFromChunkList(m_physicsSphere.m_position);

	if (!currentBlock.IsValid())
		return;

	//top priority
	BlockLocator checkedBlock = currentBlock.GetBlockLocatorBelow();
	if(checkedBlock.IsValid())
		outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorAbove();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToNorth();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToSouth();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToEast();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToWest();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	//second priority
	checkedBlock = currentBlock.GetBlockLocatorToNorthEast();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToNorthWest();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToSouthEast();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToSouthWest();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToBelowNorth();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToBelowSouth();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToBelowEast();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToBelowWest();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToAboveNorth();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToAboveSouth();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToAboveEast();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToAboveWest();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	//third priority
	checkedBlock = currentBlock.GetBlockLocatorToBelowSouthEast();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToBelowSouthWest();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToBelowNorthEast();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToBelowNorthWest();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToAboveNorthEast();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToAboveNorthWest();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToAboveSouthEast();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);

	checkedBlock = currentBlock.GetBlockLocatorToAboveSouthWest();
	if (checkedBlock.IsValid())
	outBlockLocators.push_back(checkedBlock);
}

//  =========================================================================================
bool Player::PushOutOfBlock(BlockLocator locator)
{
	if (!locator.IsValid())
		return false;

	Block* block = locator.GetBlock();
	if (!locator.GetBlock()->IsSolid())
		return false;

	Vector3 worldCenter = locator.m_chunk->GetBlockWorldCenterForBlockIndex(locator.m_blockIndex);
	AABB3 blockBounds = AABB3(worldCenter, 0.5f, 0.5f, 0.5f);
	
	//determine if point is in 
	Vector3 closestPointOnBounds = blockBounds.GetClosestPointOnAABB3(m_physicsSphere.m_position);

	//we aren't overlapping so no need to push out
	float distanceFromSphereToClosestPointOnAABB3 = GetDistance(closestPointOnBounds, m_physicsSphere.m_position);
	float separation = distanceFromSphereToClosestPointOnAABB3 - m_physicsSphere.m_radius;

	//we aren't overlapping
	if (separation >= 0.f || IsNearZero(separation))
		return false;
	
	//we need to push out
	Vector3 pushDirectionNormalized = (m_physicsSphere.m_position - closestPointOnBounds).GetNormalized();		
	m_position += (pushDirectionNormalized * (-1.f * separation));

	UpdateBoundsToCurrentPosition();

	//kill velocity in the direction we are pushing
	Vector3 projection = GetProjectedVector(m_velocity, (-1.f * pushDirectionNormalized));
	m_velocity -= projection;

	return true;
}
