//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "BlockMushroom.h"
#include "Enums.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

BlockMushroom::BlockMushroom(const DOUBLE2 Center, const double width, const double height, EnemyManager* enemyManager) : Block(Center, width, height, String("Resources/EmptyQuestion.png"),enemyManager)
{
	m_SndSpawnPtr = new FmodSound();
	m_SndSpawnPtr->CreateStream(String("Resources/Sounds/mushroomappear.ogg"));
}

BlockMushroom::~BlockMushroom()
{
	delete m_BmpActivePtr;
	m_BmpActivePtr = nullptr;
	delete m_BmpMushroomPtr;
	m_BmpMushroomPtr = nullptr;
	delete m_SndSpawnPtr;
	m_SndSpawnPtr = nullptr;
}

void BlockMushroom::Paint()
{
	double width = m_BmpActivePtr->GetWidth() / 3;
	double height = m_BmpActivePtr->GetHeight();

	if (m_SpawnMushroom)
	{
		GAME_ENGINE->DrawBitmap(m_BmpMushroomPtr, m_Center - DOUBLE2(m_Width / 2, (m_Height / 2) + m_MushroomOffsetY));
	}

	if (!m_IsEmpty)
	{
		GAME_ENGINE->DrawBitmap(m_BmpActivePtr, m_Center - DOUBLE2(m_Width / 2, (m_Height / 2) + m_BmpOffsetY), { m_AnimFrame * width,0,(m_AnimFrame + 1) * width, height });
		return;
	}



	Block::Paint();
}

void BlockMushroom::Tick(double deltaTime, HUD* HUDPtr)
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
	if (m_SpawnMushroom)
	{
		m_Counter++;
		if (m_Counter%3 == 1)
		{
			m_MushroomOffsetY += 1;
			if (m_MushroomOffsetY >= m_Height)
			{
				m_SpawnMushroom = false;
				m_SpawnEnemy = true;
				//m_EnemyManager->Add(m_Center + DOUBLE2(0, m_Height), m_Width, m_Height, 50, EnemyType::GOOMBA);
			}
		}
		
	}
	Block::Tick(deltaTime, HUDPtr);
}

void BlockMushroom::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	Block::PreSolve(actThisPtr, actOtherPtr, enableContactRef);

	if (m_Ishit)
	{
		if (!m_IsEmpty)
		{
			m_SpawnMushroom = true;
			m_SndSpawnPtr->Play();
		}
		m_IsEmpty = true;
	}
}