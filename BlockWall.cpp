//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "BlockWall.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

BlockWall::BlockWall(const DOUBLE2 Center, const double width, const double height, EnemyManager* enemyManager) : Block(Center, width, height, String("Resources/Wall.png"), enemyManager)
{
	m_TopLeftPosOrig = DOUBLE2(m_Center.x - width / 4, m_Center.y - height / 4);
	m_ActTopLeftPtr = new PhysicsActor(m_TopLeftPosOrig, 0, BodyType::DYNAMIC);
	m_ActTopLeftPtr->SetGravityScale(0);
	m_TopRightPosOrig = DOUBLE2(m_Center.x + width / 4, m_Center.y - height / 4);
	m_ActTopRightPtr = new PhysicsActor(m_TopRightPosOrig, 0, BodyType::DYNAMIC);
	m_ActTopRightPtr->SetGravityScale(0);
	m_BottomLeftPosOrig = DOUBLE2(m_Center.x - width / 4, m_Center.y + height / 4);
	m_ActBottomLeftPtr = new PhysicsActor(m_BottomLeftPosOrig, 0, BodyType::DYNAMIC);
	m_ActBottomLeftPtr->SetGravityScale(0);
	m_BottomRightPosOrig = DOUBLE2(m_Center.x + width / 4, m_Center.y + height / 4);
	m_ActBottomRightPtr = new PhysicsActor(m_BottomRightPosOrig, 0, BodyType::DYNAMIC);
	m_ActBottomRightPtr->SetGravityScale(0);
	m_SndBreakPtr = new FmodSound();
	m_SndBreakPtr->CreateStream(String("Resources/Sounds/blockbreak.ogg"));
}

BlockWall::~BlockWall()
{
	delete m_ActTopLeftPtr;
	m_ActTopLeftPtr = nullptr;
	delete m_ActTopRightPtr;
	m_ActTopRightPtr = nullptr;
	delete m_ActBottomLeftPtr;
	m_ActBottomLeftPtr = nullptr;
	delete m_ActBottomRightPtr;
	m_ActBottomRightPtr = nullptr;
	delete m_SndBreakPtr;
	m_SndBreakPtr = nullptr;
}

void BlockWall::Paint()
{
	if (m_Broken)
	{
		PaintQuarter(m_ActTopLeftPtr, m_TopLeftPosOrig);
		PaintQuarter(m_ActTopRightPtr, m_TopRightPosOrig);
		PaintQuarter(m_ActBottomLeftPtr, m_BottomLeftPosOrig);
		PaintQuarter(m_ActBottomRightPtr, m_BottomRightPosOrig);
	}
	else Block::Paint();
}

void BlockWall::Tick(double deltaTime, HUD* HUDPtr)
{
	if (m_Broken)
	{
		m_Angle += deltaTime * 3;
	}
	if (m_ActTopLeftPtr->GetPosition().y > GAME_ENGINE->GetHeight())
	{
		m_IsDestroyed = true;
	}
	Block::Tick(deltaTime, HUDPtr);
}

void BlockWall::PaintQuarter(PhysicsActor* actPtr, DOUBLE2 pos)
{
	DOUBLE2 actorPos = actPtr->GetPosition(), topLeft = m_ActPtr->GetPosition() - DOUBLE2(m_Width / 2, m_Height / 2);
	double width = m_Width / 4;
	double height = m_Height / 4;
	MATRIX3X2 matOriginal, matCenter, matRotate, matTranslate, matTotal;
	matOriginal = GAME_ENGINE->GetWorldMatrix();
	matCenter.SetAsTranslate(-actPtr->GetPosition());
	matRotate.SetAsRotate(m_Angle);
	matTranslate.SetAsTranslate(actPtr->GetPosition());
	matTotal = matCenter * matRotate * matTranslate;
	GAME_ENGINE->SetWorldMatrix(matTotal);
	GAME_ENGINE->DrawBitmap(m_BmpPtr, actorPos - DOUBLE2(width, height), { pos.x- topLeft.x - width, pos.y - topLeft.y - height, pos.x - topLeft.x + width, pos.y - topLeft.y + height});
	//GAME_ENGINE->DrawBitmap(m_BmpPtr, pos - DOUBLE2(width, height));
	GAME_ENGINE->SetWorldMatrix(matOriginal);
}

void BlockWall::Break()
{
	m_ActTopLeftPtr->SetGravityScale(1);
	m_ActTopLeftPtr->ApplyLinearImpulse(DOUBLE2(-10000, -40000));
	m_ActTopRightPtr->SetGravityScale(1);
	m_ActTopRightPtr->ApplyLinearImpulse(DOUBLE2(+10000, -40000));
	m_ActBottomLeftPtr->SetGravityScale(1);
	m_ActBottomLeftPtr->ApplyLinearImpulse(DOUBLE2(-10000, -20000));
	m_ActBottomRightPtr->SetGravityScale(1);
	m_ActBottomRightPtr->ApplyLinearImpulse(DOUBLE2(+10000, -20000));
	m_Broken = true;
	m_SndBreakPtr->Play();
}
