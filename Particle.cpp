//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Particle.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Particle::Particle(DOUBLE2 position, double velocityX, double velocityY) : m_Position(position)
{
	m_Velocity.x = velocityX;
	m_Velocity.y = velocityY;
}

Particle::~Particle()
{
}

void Particle::Tick(double deltaTime)
{
	m_Position.x *= m_Velocity.x / deltaTime;
	m_Position.y *= m_Velocity.y / deltaTime;
}

void Particle::Paint()
{
	GAME_ENGINE->FillEllipse(m_Position, 0.5, 0.5);
}