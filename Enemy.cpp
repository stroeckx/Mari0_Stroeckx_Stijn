//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Enemy.h"
#include "EnemyManager.h"
#include "Mario.h"
#include "Level.h"

#include "HUD.h"
#define GAME_ENGINE (GameEngine::GetSingleton())

Enemy::Enemy(const DOUBLE2 Center, const double width, const double height, const double velocity, const String& bitmap) : m_Center(Center), m_Width(width), m_Height(height), m_Velocity(velocity)
{
	m_BmpPtr = new Bitmap(bitmap);
	m_ActPtr = new PhysicsActor(m_Center, 0, BodyType::DYNAMIC);
	m_ActPtr->AddBoxFixture(width, height,0,0);
	m_ActPtr->AddContactListener(this);
	m_ActPtr->SetFixedRotation(true);
	m_ActPtr->SetUserData(int(ActorType::ENEMY));
	m_ActPtr->SetUserPointer(this);
	b2Filter collisionFilter;
	collisionFilter.categoryBits = ActorCollideType::ENEMY;
	collisionFilter.maskBits = ActorCollideType::MARIO | ActorCollideType::SHELL | ActorCollideType::TERRAIN;
	m_ActPtr->SetCollisionFilter(collisionFilter);
	//m_ActPtr->SetSensor(true);
	m_SndDeadPtr = new FmodSound();
	m_SndDeadPtr->CreateStream(String("Resources/Sounds/stomp.ogg"));
	m_SndSpecialDeadPtr = new FmodSound();
	m_SndSpecialDeadPtr->CreateStream(String("Resources/Sounds/shot.ogg"));
}

Enemy::~Enemy()
{
	delete m_BmpPtr;
	m_BmpPtr = nullptr;
	delete m_ActPtr;
	m_ActPtr = nullptr;
	delete m_SndDeadPtr;
	m_SndDeadPtr = nullptr;
	delete m_SndSpecialDeadPtr;
	m_SndSpecialDeadPtr = nullptr;
}

void Enemy::Paint()
{
	if (m_IsSpecialDead)
	{

		MATRIX3X2 matCenter, matScale, matTranslate, matWorld, matOrigWorld;
		matOrigWorld = GAME_ENGINE->GetWorldMatrix();
		matCenter.SetAsTranslate(-m_ActPtr->GetPosition().x, -m_ActPtr->GetPosition().y);
		matScale.SetAsScale(1, -1);
		matTranslate.SetAsTranslate(m_ActPtr->GetPosition());
		matWorld = matCenter * matScale * matTranslate;
		GAME_ENGINE->SetWorldMatrix(matWorld);
		if (m_BmpPtr != nullptr)
		{
			GAME_ENGINE->DrawBitmap(m_BmpPtr, m_ActPtr->GetPosition() - DOUBLE2(m_Width / 2, (m_Height / 2) - m_BitmapOffsetY));
		}
		GAME_ENGINE->SetWorldMatrix(matOrigWorld);
	}
	 else if (m_BmpPtr != nullptr)
	{
		GAME_ENGINE->DrawBitmap(m_BmpPtr, m_ActPtr->GetPosition() - DOUBLE2(m_Width / 2, (m_Height / 2) - m_BitmapOffsetY));
	}
}

void Enemy::Paint(RECT2 rect)
{
	if (m_IsSpecialDead)
	{
		MATRIX3X2 matCenter, matScale, matTranslate, matWorld, matOrigWorld;
		matOrigWorld = GAME_ENGINE->GetWorldMatrix();
		matCenter.SetAsTranslate(-m_ActPtr->GetPosition().x, -m_ActPtr->GetPosition().y);
		matScale.SetAsScale(1, -1);
		matTranslate.SetAsTranslate(m_ActPtr->GetPosition());
		matWorld = matCenter * matScale * matTranslate;
		GAME_ENGINE->SetWorldMatrix(matWorld);
		if (m_BmpPtr != nullptr)
		{
			GAME_ENGINE->DrawBitmap(m_BmpPtr, m_ActPtr->GetPosition() - DOUBLE2(m_Width / 2, (m_Height / 2) - m_BitmapOffsetY), rect);
		}
		GAME_ENGINE->SetWorldMatrix(matOrigWorld);
	}
	else if (m_BmpPtr != nullptr)
	{
		GAME_ENGINE->DrawBitmap(m_BmpPtr, m_ActPtr->GetPosition() - DOUBLE2(m_Width / 2, (m_Height / 2) - m_BitmapOffsetY), rect);
	}
}


void Enemy::Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr)
{
	if (m_AddScore != 0)
	{
		HUDPtr->AddScore(m_AddScore, m_ActPtr->GetPosition() - DOUBLE2(0, m_Height*2));
		m_AddScore = 0;
	}

	if (m_ActPtr->GetLinearVelocity().y > GAME_ENGINE->GetHeight())
	{
		m_IsDead = true;
	}
	if (!m_IsSpecialDead)
	{
		if (m_ShouldTeleport)
		{
			m_ActPtr->SetPosition(m_TeleportPosition);
			m_ShouldTeleport = false;
			m_ActPtr->ApplyLinearImpulse(m_TeleportVelocity*2.1);
		}

		DOUBLE2 intersection, normal;
		double fraction = 0;
		//check if we have to flip because we are going to run into a wall
		if (m_Velocity > -1 && m_Velocity < 1 && !m_IsDieing)
		{
			m_Velocity = 100;
		}
		if (m_Velocity > 0)
		{
			DOUBLE2 rightMiddle = m_ActPtr->GetPosition() + DOUBLE2(m_Width / 2, 0);
			if (levelPtr->Raycast(rightMiddle, rightMiddle + DOUBLE2((m_Velocity*deltaTime) + 2, 0), intersection, normal, fraction))
			{
				m_FlipDirection = true;
			}
		}
		else if(m_Velocity < 0)
		{
			DOUBLE2 leftMiddle = m_ActPtr->GetPosition() - DOUBLE2(m_Width / 2, 0);
			if (levelPtr->Raycast(leftMiddle, leftMiddle + DOUBLE2((m_Velocity*deltaTime) - 2, 0), intersection, normal, fraction))
			{
				m_FlipDirection = true;
			}
		}

		m_ActPtr->SetLinearVelocity({ m_Velocity,m_ActPtr->GetLinearVelocity().y });
		if (m_FlipDirection) FlipDirection();
		int test = 0;
	}
}

bool Enemy::IsHit(PhysicsActor* actOtherPtr)
{
	return false;
}

bool Enemy::IsOverlapping(PhysicsActor* actOtherPtr)
{
	if (m_ActPtr->IsOverlapping(actOtherPtr))
	{
		return true;
	}
	return false;
}

void Enemy::SetVelocity(double velocity)
{
	m_Velocity = velocity;
	m_ActPtr->SetLinearVelocity({ m_Velocity, m_ActPtr->GetLinearVelocity().y });
}

void Enemy::SetDead()
{
	m_IsDieing = true;
	m_ActPtr->SetGravityScale(0);
	m_ActPtr->SetSensor(true);
	SetVelocity(0);
	m_SndDeadPtr->Play();
}

bool Enemy::IsDead()
{
	return m_IsDead;
}

void Enemy::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	if (actOtherPtr->GetUserData() == int(ActorType::MARIO) || actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
	{
		Mario* marioPtr = (Mario*)actOtherPtr->GetUserPointer();
		if (marioPtr->GetFlashing())
		{
			SetSpecialDead();
			m_AddScore = m_ScoreValue;

		}
		else if (actOtherPtr->GetLinearVelocity().y > 1)
		{
			SetDead();
			m_AddScore = m_ScoreValue;
			marioPtr->Bounce();
			enableContactRef = false;
		}
		else actOtherPtr->SetUserData(int(ActorType::DEAD));
	}
}

void Enemy::FlipDirection()
{
	SetVelocity(m_ActPtr->GetLinearVelocity().x * -1);
	m_FlipDirection = false;
}

void Enemy::SetFlipDirection(bool flag)
{
	m_FlipDirection = flag;
}

bool Enemy::GetFlipDirection()
{
	return m_FlipDirection;
}

void Enemy::SetSpecialDead()
{
	m_IsSpecialDead = true;
	m_ActPtr->SetLinearVelocity({ 0,0 });
	m_ActPtr->SetSensor(true);
	m_ActPtr->ApplyLinearImpulse({ 0,-500 });
	m_SndSpecialDeadPtr->Play();
}

void Enemy::Teleport(DOUBLE2 position, DOUBLE2 velocity)
{
	m_TeleportPosition = position;
	m_ShouldTeleport = true;
	m_TeleportVelocity = velocity;
}

void Enemy::IncreaseEndContactcount()
{
	m_EndContactcount++;
}

void Enemy::SetEndContactCount(int value)
{
	m_EndContactcount = value;
}

int Enemy::GetEndContactcount()
{
	return m_EndContactcount;
}

bool Enemy::GetReadyForTeleport()
{
	return m_ReadyForNextTeleport;
}

void Enemy::SetReadyForTeleport(bool flag)
{
	m_ReadyForNextTeleport = flag;
}

DOUBLE2 Enemy::GetPosition()
{
	return m_ActPtr->GetPosition();
}

PhysicsActor* Enemy::GetActor()
{
	return m_ActPtr;
}