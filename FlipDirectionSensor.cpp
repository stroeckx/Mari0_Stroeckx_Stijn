//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "FlipDirectionSensor.h"
#include "Enums.h"
#include "Enemy.h"
//#define GAME_ENGINE (GameEngine::GetSingleton())

FlipDirectionSensor::FlipDirectionSensor(DOUBLE2 position)
{
	m_ActPtr = new PhysicsActor(position,0, BodyType::KINEMATIC);
	m_ActPtr->AddBoxFixture(2, 16, 0, 0);
	m_ActPtr->AddContactListener(this);
	m_ActPtr->SetUserData(int(ActorType::TERRAIN));
	//m_ActPtr->SetSensor(true);
	//m_ActPtr->SetCollisionFilter()
	m_ActPtr->SetGravityScale(0);
}

FlipDirectionSensor::~FlipDirectionSensor()
{
	delete m_ActPtr;
	m_ActPtr = nullptr;
}

void FlipDirectionSensor::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	enableContactRef = false;
	if (actOtherPtr->GetUserData() == int(ActorType::ENEMY))
	{
		Enemy * enemyPtr = (Enemy*)actOtherPtr->GetUserPointer();
		if (!enemyPtr->GetFlipDirection())
		{
			enemyPtr->SetFlipDirection(true);
		}
	}
}