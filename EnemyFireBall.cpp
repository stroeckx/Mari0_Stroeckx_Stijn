//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "EnemyFireBall.h"
#include "Level.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

EnemyFireBall::EnemyFireBall(const DOUBLE2 Center, const double width, const double height, const double velocity, bool autoExplode) : Enemy(Center, width, height, velocity, String("Resources/FireBall.png"))
{

	b2Filter collisionFilter;
	collisionFilter.categoryBits = ActorCollideType::SHELL;
	collisionFilter.maskBits = ActorCollideType::MARIO | ActorCollideType::SHELL | ActorCollideType::TERRAIN | ActorCollideType::ENEMY;
	m_ActPtr->SetCollisionFilter(collisionFilter);
	m_SndDeathPtr = new FmodSound();
	m_SndDeathPtr->CreateSound(String("Resources/Sounds/boom.ogg"));
	m_BmpDestroyPtr = new Bitmap(String("Resources/FireBallDestroy.png"));
	if (autoExplode)
	{
		Destroy();
	}
}


EnemyFireBall::~EnemyFireBall()
{
	delete m_SndDeathPtr;
	m_SndDeathPtr = nullptr;
	delete m_BmpDestroyPtr;
	m_BmpDestroyPtr = nullptr;
}

void EnemyFireBall::Tick(double deltaTime, EnemyManager * enemyManagerPtr, Level* levelPtr, HUD* HUDPtr)
{
	if (m_De_Stroyed)
	{
		m_ActPtr->SetSensor(true);
		m_ActPtr->SetGravityScale(0);
		m_ActPtr->SetLinearVelocity({ 0,0 });
		m_DeathTimer += deltaTime;
		if (m_DeathTimer > m_SoundLength)
		{
			m_IsDead = true;
		}
	}
	else
	{
		if (levelPtr->IsOnGround(m_ActPtr))
		{
			m_ActPtr->ApplyLinearImpulse({ 0, -175 });
		}

		m_AnimTime += deltaTime;
		if (m_AnimTime >= 1.0 / m_NrFramesPerSec)
		{
			++m_AnimFrame;
			if (m_AnimFrame > m_NrOfFrames) m_AnimFrame = 0;
			m_AnimTime -= m_AnimTime / m_NrFramesPerSec;
		}

		Enemy::Tick(deltaTime, enemyManagerPtr, levelPtr, HUDPtr);
	}
}

void EnemyFireBall::Paint()
{
	if (m_De_Stroyed)
	{
		if (m_DeathTimer < (m_SoundLength/3))
		{
			GAME_ENGINE->DrawBitmap(m_BmpDestroyPtr, m_ActPtr->GetPosition() - DOUBLE2(m_DestroyedSize / 2, (m_DestroyedSize / 2) - m_BitmapOffsetY), { 0,0,m_DestroyedSize,m_DestroyedSize });
		}
		else if (m_DeathTimer < 2 * (m_SoundLength / 3))
		{
			GAME_ENGINE->DrawBitmap(m_BmpDestroyPtr, m_ActPtr->GetPosition() - DOUBLE2(m_DestroyedSize / 2, (m_DestroyedSize / 2) - m_BitmapOffsetY), { m_DestroyedSize,0,m_DestroyedSize*2,m_DestroyedSize });
		}
		else if (m_DeathTimer < m_SoundLength)
		{
			GAME_ENGINE->DrawBitmap(m_BmpDestroyPtr, m_ActPtr->GetPosition() - DOUBLE2(m_DestroyedSize / 2, (m_DestroyedSize / 2) - m_BitmapOffsetY), { m_DestroyedSize * 2,0,m_DestroyedSize * 3,m_DestroyedSize });
		}
		else
		{
			m_IsDead = true;
		}
	}
	else Enemy::Paint({ m_AnimFrame * m_Width,0,(m_AnimFrame + 1) * m_Width,m_Height });
}

void EnemyFireBall::FlipDirection()
{
	Destroy();
}

void EnemyFireBall::Destroy()
{
	m_De_Stroyed = true;
	m_SndDeathPtr->Play();
}

void EnemyFireBall::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	if (actOtherPtr->GetUserData() == int(ActorType::MARIO) || actOtherPtr->GetUserData() == int(ActorType::BIGMARIO))
	{
			actOtherPtr->SetUserData(int(ActorType::DEAD));
	}
	else if (actOtherPtr->GetUserData() == int(ActorType::ENEMY))
	{
		Enemy* enemyPtr = (Enemy*)actOtherPtr->GetUserPointer();
		enemyPtr->SetSpecialDead();
		Destroy();
	}
}
