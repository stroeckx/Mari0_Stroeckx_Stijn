//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "EnemyShell.h"

//#define GAME_ENGINE (GameEngine::GetSingleton())

EnemyShell::EnemyShell(const DOUBLE2 Center, const double width, const double height, const double velocity) : Enemy(Center, width, height, velocity, String("Resources/Shell.png"))
{
	b2Filter collisionFilter;
	collisionFilter.categoryBits = ActorCollideType::SHELL;
	collisionFilter.maskBits = ActorCollideType::ENEMY | ActorCollideType::MARIO;
	m_ActPtr->SetCollisionFilter(collisionFilter);
	m_SndSpecialDeadPtr->Play();
}

EnemyShell::~EnemyShell()
{
}

void EnemyShell::Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr)
{
	if (m_IsActivated)
	{
		m_TimeActivated += deltaTime;

		if (m_DistanceMario < 0)
		{
			SetVelocity(m_Velocity);
			m_DistanceMario = 0;
		}
		else if (m_DistanceMario > 0)
		{
			SetVelocity(-m_Velocity);
			m_DistanceMario = 0;
		}
	}

	Enemy::Tick(deltaTime, enemyManagerPtr, levelPtr, HUDPtr);
}

void EnemyShell::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	if (actOtherPtr->GetUserData() == int(ActorType::MARIO) || actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
	{
		if (!m_IsActivated)
		{
			m_IsActivated = true;
			m_DistanceMario = actOtherPtr->GetPosition().x - actThisPtr->GetPosition().x;
		}
		else
		{
			if (actOtherPtr->GetLinearVelocity().y > 1)
			{
				actOtherPtr->SetLinearVelocity(DOUBLE2(actOtherPtr->GetLinearVelocity().x, -250));
			}
			else if (m_TimeActivated > m_ActivatedDelay)
			{
				actOtherPtr->SetUserData(int(ActorType::DEAD));
			}
		}
	}
	else if (actOtherPtr->GetUserData() == int(ActorType::ENEMY))
	{
		Enemy* enemyPtr = (Enemy*)actOtherPtr->GetUserPointer();
		enemyPtr->SetSpecialDead();
	}
}
