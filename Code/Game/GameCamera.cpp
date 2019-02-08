#include "Game\GameCamera.hpp"
#include "Game\GameCommon.hpp"


//  =========================================================================================
GameCamera::GameCamera()
{
}

//  =========================================================================================
GameCamera::~GameCamera()
{
}

//  =========================================================================================
void GameCamera::CreateFliippedViewMatrix(Matrix44& outMatrix)
{
	outMatrix.SetIdentity();
	outMatrix.Append(g_flipMatrix);
	outMatrix.RotateDegreesAroundX3D(-1.f * m_rollDegreesX);
	outMatrix.RotateDegreesAroundY3D(-1.f * m_pitchDegreesY);
	outMatrix.RotateDegreesAroundZ3D(-1.f * m_yawDegreesZ);
	outMatrix.Translate3D(-1.f * m_position);
}

//  =========================================================================================
void GameCamera::Translate(Vector3 translation)
{
	m_position += translation;
}
