//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "PortalParticle.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

PortalParticle::PortalParticle(DOUBLE2 position, DOUBLE2 velocity) : m_Position(position), m_Velocity(velocity)
{
}

PortalParticle::PortalParticle(DOUBLE2 position) : m_Position(position)
{
	m_Velocity.x = (10.7586*rand() / RAND_MAX - 5.3793);
	m_Velocity.y = (10.7586*rand() / RAND_MAX - 5.3793);
}

PortalParticle::~PortalParticle()
{
}

void PortalParticle::Tick(double deltaTime)
{
	/*if (m_Position.x > 0 && m_Position.x < 3500 && m_Position.y <)
	{

	}*/
	m_Position.x += m_Velocity.x * deltaTime;
	m_Position.y += m_Velocity.y * deltaTime;
}

void PortalParticle::Paint()
{
	GAME_ENGINE->FillEllipse(m_Position, m_Radius, m_Radius);
}

DOUBLE2 PortalParticle::m_GetPosition()
{
	return m_Position;
}

void PortalParticle::SetPosition(DOUBLE2& positionRef)
{
	m_Position = positionRef;
}

void PortalParticle::SetVelocity(DOUBLE2& velocityRef)
{
	m_Velocity - velocityRef;
}