//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "EndGameFlag.h"
#include "Enums.h"
#include "Mario.h"

//#define GAME_ENGINE (GameEngine::GetSingleton())

EndGameFlag::EndGameFlag(DOUBLE2 position)
{
	m_ActPtr = new PhysicsActor(position + DOUBLE2(m_Area.x,m_Area.y/2 - m_Area.x/2), 0, BodyType::KINEMATIC);
	m_ActPtr->AddBoxFixture(m_Area.x, m_Area.y, 0, 0);
	//m_ActPtr->SetSensor(true);
	m_ActPtr->AddContactListener(this);
	m_ActPtr->SetUserData(int(ActorType::TERRAIN));
	b2Filter collisionFilter;
	collisionFilter.categoryBits = ActorCollideType::TERRAIN;
	collisionFilter.maskBits = ActorCollideType::MARIO | ActorCollideType::SHELL;
	m_ActPtr->SetCollisionFilter(collisionFilter);
}

EndGameFlag::~EndGameFlag()
{
	delete m_ActPtr;
	m_ActPtr = nullptr;
}

void EndGameFlag::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	enableContactRef = false;
	if (actOtherPtr->GetUserData() == int(ActorType::MARIO) || actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
	{
		Mario* marioPtr = (Mario*)actOtherPtr->GetUserPointer();
		marioPtr->SetEndReached(true);
	}
}