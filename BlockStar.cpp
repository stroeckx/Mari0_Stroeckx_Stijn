//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "BlockStar.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

BlockStar::BlockStar(const DOUBLE2 Center, const double width, const double height, EnemyManager* enemyManager) : Block(Center, width, height, String("Resources/EmptyQuestion.png"), enemyManager)
{

}

BlockStar::~BlockStar()
{
	delete m_BmpStarPtr;
	m_BmpStarPtr = nullptr;
	delete m_BmpActivePtr;
	m_BmpActivePtr = nullptr;
}

void BlockStar::Tick(double deltaTime, HUD* HUDPtr)
{
	if (m_SpawnStar)
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

		m_Counter++;
		if (m_Counter % 3 == 1)
		{
			m_StarOffsetY += 1;
			if (m_StarOffsetY >= m_Height)
			{
				m_SpawnStar = false;
				m_SpawnEnemy = true;
				//m_EnemyManager->Add(m_Center + DOUBLE2(0, m_Height), m_Width, m_Height, 50, EnemyType::GOOMBA);
			}
		}

	}
	Block::Tick(deltaTime, HUDPtr);
}

void BlockStar::Paint()
{
	double starWidth = m_BmpStarPtr->GetWidth() / m_NrOfFrames;
	double starHeight = m_BmpStarPtr->GetHeight();

	if (m_SpawnStar)
	{
		GAME_ENGINE->DrawBitmap(m_BmpStarPtr, m_Center - DOUBLE2(starWidth / 2, (starHeight / 2) + m_StarOffsetY), { m_AnimFrame * starWidth,0,(m_AnimFrame + 1) * starWidth, starHeight });
	}

	if (!m_IsEmpty)
	{
		GAME_ENGINE->DrawBitmap(m_BmpActivePtr, m_Center - DOUBLE2(m_Width / 2, (m_Height / 2) + m_BmpOffsetY));
		return;
	}



	Block::Paint();
}

void BlockStar::PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef)
{
	Block::PreSolve(actThisPtr, actOtherPtr, enableContactRef);

	if (m_Ishit)
	{
		if (!m_IsEmpty)
		{
			m_SpawnStar = true;
		}
		m_IsEmpty = true;
	}
}

EnemyType BlockStar::SpawnEnemy()
{
	if (m_SpawnEnemy)
	{
		m_SpawnEnemy = false;
		return EnemyType::STAR;
	}
	return EnemyType::EMPTY;
}