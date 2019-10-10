//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Koopa.h"
#include "EnemyManager.h"
//#define GAME_ENGINE (GameEngine::GetSingleton())

Koopa::Koopa(const DOUBLE2 Center, const double width, const double height, const double velocity) : Enemy(Center, width, height, velocity, String("Resources/Koopa.png"))
{

}

Koopa::~Koopa()
{
}

void Koopa::Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr)
{
	if (m_IsDieing)
	{
		enemyManagerPtr->QueueEnemy({ m_ActPtr->GetPosition() }, 16, 14, 100, EnemyType::SHELL);
		m_IsDead = true;
		Enemy::Tick(deltaTime, enemyManagerPtr, levelPtr, HUDPtr);
	}
	else
	{
		m_AnimTime += deltaTime;
		if (m_AnimTime >= 1.0 / m_NrFramesPerSec)
		{
			++m_AnimFrame;
			if (m_AnimFrame > m_NrOfFrames)
			{
				m_AnimFrame = 0;
			}
			m_AnimTime -= m_AnimTime / m_NrFramesPerSec;
		}

		Enemy::Tick(deltaTime, enemyManagerPtr, levelPtr, HUDPtr);
	}
}

void Koopa::Paint()
{
	Enemy::Paint({ m_AnimFrame * m_Width,0,(m_AnimFrame + 1) * m_Width,m_Height });
}