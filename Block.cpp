//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Block.h"
#include "HUD.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Block::Block(const DOUBLE2 Center, const double width, const double height, const String& bitmap, EnemyManager* enemyManager) : m_Center(Center), m_Width(width), m_Height(height)
{
	m_BmpPtr = new Bitmap(bitmap);
	m_ActPtr = new PhysicsActor(m_Center, 0, BodyType::STATIC);
	m_ActPtr->AddBoxFixture(width, height, 0, 0);
	m_ActPtr->AddContactListener(this);
	m_ActPtr->SetUserData(int(ActorType::TERRAIN));
	m_EnemyManager = enemyManager;
	m_SndHitPtr = new FmodSound();
	m_SndHitPtr->CreateStream(String("Resources/Sounds/blockhit.ogg"));
}

Block::~Block()
{
	delete m_BmpPtr;
	m_BmpPtr = nullptr;
	delete m_ActPtr;
	m_ActPtr = nullptr;
	delete m_SndHitPtr;
	m_SndHitPtr = nullptr;
}

void Block::Paint()
{
	if (m_BmpPtr != nullptr)
	{
		GAME_ENGINE->DrawBitmap(m_BmpPtr, m_Center - DOUBLE2(m_Width / 2, (m_Height / 2)+m_BmpOffsetY));
	}
}

void Block::Tick(double deltaTime, HUD* HUDPtr)
{
	if (m_Ishit)
	{
		if (m_AnimUpwards)
		{
			m_BmpOffsetY++;
			if (m_BmpOffsetY >= 5)
			{
				m_AnimUpwards = false;
			}
		}
		else
		{
			m_BmpOffsetY--;
			if (m_BmpOffsetY < 0)
			{
				m_BmpOffsetY = 0;
				m_Ishit = false;
			}
		}
	}
}

bool Block::IsHit()
{
	return false;
}

bool Block::IsOverlapping(PhysicsActor* actOtherPtr)
{
	if (m_ActPtr->IsOverlapping(actOtherPtr) && !m_Broken)
	{
		return true;
	}
	return false;
}

void  Block::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	if (m_Broken)
	{
		enableContactRef = false;
	}
	else if (actOtherPtr->GetUserData() == int(ActorType::MARIO) || actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
	{
		if (actOtherPtr->GetLinearVelocity().y < -1 && actOtherPtr->GetPosition().y > actThisPtr->GetPosition().y)
		{
			//checks if the middle of mario is overlapping to avoid multiple blocks being activated at once
			if (actOtherPtr->GetPosition().x > actThisPtr->GetPosition().x - m_Width / 2 && actOtherPtr->GetPosition().x < actThisPtr->GetPosition().x + m_Width / 2)
			{
				m_Ishit = true;
				m_AnimUpwards = true;
				if (actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
				{
					Break();
				}
				else
				{
					m_SndHitPtr->Play();
				}
			}
		}
	}
}

EnemyType Block::SpawnEnemy()
{
	if (m_SpawnEnemy)
	{
		m_SpawnEnemy = false;
		return EnemyType::MUSHROOM; //default spawn
	}
	return EnemyType::EMPTY;
}

DOUBLE2 Block::GetPosition()
{
	return m_Center;
}

double Block::GetWidth()
{
	return m_Width;
}

double Block::GetHeight()
{
	return m_Height;
}

void Block::Break()
{
	//m_Broken = true;
}

bool Block::IsDestroyed()
{
	return m_IsDestroyed;
}

bool Block::Raycast(DOUBLE2 point1, DOUBLE2 point2, DOUBLE2 &intersectionRef, DOUBLE2 &normalRef, double &fractionRef)
{
	return m_ActPtr->Raycast(point1, point2, intersectionRef, normalRef, fractionRef);
}