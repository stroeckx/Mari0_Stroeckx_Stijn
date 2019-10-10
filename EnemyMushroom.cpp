//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "EnemyMushroom.h"
#include "Mario.h"
#include "HUD.h"

//#define GAME_ENGINE (GameEngine::GetSingleton())

EnemyMushroom::EnemyMushroom(const DOUBLE2 Center, const double width, const double height, const double velocity) : Enemy(Center, width, height, velocity, String("Resources/Mushroom.png"))
{
	m_SndEatPtr = new FmodSound();
	m_SndEatPtr->CreateStream(String("Resources/Sounds/mushroomeat.ogg"));
}

EnemyMushroom::~EnemyMushroom()
{
	delete m_SndEatPtr;
	m_SndEatPtr = nullptr;
}

void EnemyMushroom::Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr)
{
	if (m_IsDieing)
	{
		if (m_ActPtr->GetPosition().x > -100)
		{
			HUDPtr->AddScore(1000, m_ActPtr->GetPosition() - DOUBLE2(m_Width/2, m_Height));
			m_ActPtr->SetPosition({ -500,-500 });
		}
		if (!m_SndEatPtr->IsPlaying())
		{
			m_IsDead = true;
		}
	}
	Enemy::Tick(deltaTime, enemyManagerPtr, levelPtr, HUDPtr);
}

void EnemyMushroom::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	if (actOtherPtr->GetUserData() == int(ActorType::MARIO))
	{
		actOtherPtr->SetUserData(int(ActorType::BIGMARIO));
		m_SndEatPtr->Play();
		SetDead();
	}
	else if (actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
	{
		Mario* marioPtr = (Mario*)actOtherPtr->GetUserPointer();
		marioPtr->SetSpecialBigMario(true);
		m_SndEatPtr->Play();
		SetDead();
	}
}
