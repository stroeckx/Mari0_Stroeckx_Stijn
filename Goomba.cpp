//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Goomba.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Goomba::Goomba(const DOUBLE2 Center, const double width, const double height, const double velocity): Enemy(Center, width, height, velocity, String("Resources/goomba.png"))
{

}

Goomba::~Goomba()
{
}

void Goomba::Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr)
{
	if (m_IsDieing)
	{
		m_TimeDead += deltaTime;
		if (m_TimeDead > 1)
		{
			m_IsDead = true;
		}
		Enemy::Tick(deltaTime, enemyManagerPtr, levelPtr, HUDPtr);
	}
	else
	{
		m_AnimTime += deltaTime;
		if (m_AnimTime >= 1.0 / m_NrFramesPerSec)
		{
			++m_AnimFrame;
			if (m_AnimFrame > m_NrOfFrames) m_AnimFrame = 0;
			m_AnimTime -= m_AnimTime / m_NrFramesPerSec;
		}

		Enemy::Tick(deltaTime, enemyManagerPtr, levelPtr,HUDPtr);
	}
}

void Goomba::Paint()
{
	if (m_IsDieing)
	{
		Enemy::Paint({ m_Width,0,m_Width*2,m_Height });
	}
	else
	{
		MATRIX3X2 matOrigWorld = GAME_ENGINE->GetWorldMatrix();

		if (m_AnimFrame != 0)
		{
			MATRIX3X2 matCenter, matScale, matTranslate, matWorld;
			matCenter.SetAsTranslate(-m_ActPtr->GetPosition().x, -m_ActPtr->GetPosition().y);
			matScale.SetAsScale(-1, 1);
			matTranslate.SetAsTranslate(m_ActPtr->GetPosition());
			matWorld = matCenter * matScale * matTranslate;
			GAME_ENGINE->SetWorldMatrix(matWorld);
		}

		Enemy::Paint({ 0,0,m_Width,m_Height });

		GAME_ENGINE->SetWorldMatrix(matOrigWorld);
	}
}

bool Goomba::IsHit(PhysicsActor* actOtherPtr)
{
	if (m_ActPtr->IsOverlapping(actOtherPtr) && actOtherPtr->GetLinearVelocity().y > 1)
	{
		return true;
	}
	return false;
}

/*bool Goomba::IsDead()
{
	return m_IsDead;
}

void Goomba::SetDead()
{
	m_IsDieing = true;
	Enemy::SetVelocity(0);
}*/
