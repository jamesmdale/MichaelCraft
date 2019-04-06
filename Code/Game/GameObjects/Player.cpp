#include "Game\GameObjects\Player.hpp"
#include "Game\GameCommon.hpp"
#include "Game\Helpers\GameRendererHelpers.hpp"
#include "Game\GameObjects\BlockLocator.hpp"
#include "Game\GameObjects\Block.hpp"
#include "Engine\Renderer\Mesh.hpp"
#include "Engine\Input\InputSystem.hpp"
#include "Engine\Core\EngineCommon.hpp"

//  =========================================================================================
Player::Player()
{
	//generate visual bounds
	Vector3 bottomPivot = Vector3(0.f, 0.f, 0.f);
	float widthRadius = g_playerWidth / 2.f;
	float heightRadius = g_playerHeight / 2.f;
	m_visualBounds = AABB3(Vector3::ZERO, widthRadius, widthRadius, heightRadius);

	//generate physics sphere bounds
	m_physicsSphere = Sphere(Vector3::ZERO, widthRadius);
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

	//last step is updating my physics/visual bounds to the new position
	UpdateBoundsToCurrentPosition();
}

//  =========================================================================================
void Player::UpdatePhysics(float deltaSeconds)
{
	//apply all forces into velocity
	m_velocity += g_gravity * deltaSeconds;

	//move the player
	m_position += m_velocity;
	UpdateBoundsToCurrentPosition();

	//std::vector<BlockLocator*> neighborhood;
	//std::vector<BlockLocator*> neighborhood;
	//GetBlockNeighborhood();
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

	//draw visual bounds
	meshMatrix.SetTranslation(m_visualBounds.GetCenter());
	theRenderer->DrawMesh(m_visualsMesh, meshMatrix);

	Vector3 bottomCenterPivot = GetBottomCenterPivot();

	Mesh* debugPivotMesh = CreateDebugStar(bottomCenterPivot, Rgba::PINK, 0.05f);
	theRenderer->DrawMesh(debugPivotMesh, meshMatrix);

	//draw physics sphere
	Matrix44 physicsMatrix;
	physicsMatrix.SetTranslation(m_physicsSphere.m_position);
	theRenderer->DrawMesh(m_physicsMesh, meshMatrix);
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
	return Vector3(m_position.x, m_position.y, m_position.z - (m_visualBounds.GetDimensions().z / 2.f));
}

//  =========================================================================================
void Player::UpdateBoundsToCurrentPosition()
{
	m_visualBounds.SetCenter(m_position);

	Vector3 bottomCenterPivot = GetBottomCenterPivot();
	m_physicsSphere.m_position = Vector3(bottomCenterPivot.x, bottomCenterPivot.y, bottomCenterPivot.z + m_physicsSphere.m_radius);
}
