//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "EnemyStar.h"
#include "Level.h"
#include "Mario.h"
#include "HUD.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

EnemyStar::EnemyStar(const DOUBLE2 Center, const double width, const double height, const double velocity) : Enemy(Center, width, height, velocity, String("Resources/Star.png"))
{
}

EnemyStar::~EnemyStar()
{
}

void EnemyStar::Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr)
{
	if (m_IsDieing)
	{
		HUDPtr->AddScore(1000, m_ActPtr->GetPosition() - DOUBLE2(m_Width / 2, m_Height));
		m_IsDead = true;
	}
	else
	{
		m_AnimTime += deltaTime;
		if (m_AnimTime >= 1.0 / m_NrFramesPerSec)
		{
			++m_AnimFrame;
			if (m_AnimFrame >= m_NrOfFrames)
			{
				m_AnimFrame = 0;
			}
			m_AnimTime -= m_AnimTime / m_NrFramesPerSec;
		}
	}
	if (levelPtr->IsOnGround(m_ActPtr) && m_ActPtr->GetLinearVelocity().y > -1)
	{
		m_ActPtr->ApplyLinearImpulse({ 0, -m_BounceHeight });
	}
	Enemy::Tick(deltaTime, enemyManagerPtr, levelPtr, HUDPtr);
}
void EnemyStar::Paint()
{
	//double starWidth = m_BmpStarPtr->GetWidth() / m_NrOfFrames;
	//double starHeight = m_BmpStarPtr->GetHeight();

	GAME_ENGINE->DrawBitmap(m_BmpPtr, m_ActPtr->GetPosition() - DOUBLE2(m_Width / 2, m_Height / 2), { m_AnimFrame * m_Width,0,(m_AnimFrame + 1) * m_Width, m_Height });
}

void EnemyStar::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	if (actOtherPtr->GetUserData() == int(ActorType::MARIO) || actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
	{
		Mario* marioPtr = (Mario*)actOtherPtr->GetUserPointer();
		marioPtr->SetFlashing(true);
		SetDead();
	}
}

void EnemyStar::FlipDirection()
{
	//stars should never flip
}