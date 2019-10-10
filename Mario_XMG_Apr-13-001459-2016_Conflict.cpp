//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Mario.h"
//#include "PowerUpManager.h"
#include "Level.h"
//#include "PowerUp.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Mario::Mario(const DOUBLE2 & centerRef)
{
	m_ActPtr = new PhysicsActor({ 25,225 }, 0, BodyType::DYNAMIC);
	m_ActPtr->AddBoxFixture(m_WidthActor, m_HeightActor, 0,0);
	m_ActPtr->SetFixedRotation(true);
	m_ActPtr->SetUserData(2);
	m_ActFeetPtr = new PhysicsActor(m_ActPtr->GetPosition() + DOUBLE2(0,(m_HeightActor / 2) -1 ), 0, BodyType::DYNAMIC);
	m_ActFeetPtr->AddBoxFixture(m_WidthActor - 2, 2, 0, 0);
	m_ActFeetPtr->SetFixedRotation(true);
	m_ActFeetPtr->SetSensor(true);
	m_ActFeetPtr->SetGravityScale(0);


	//m_SndPowerUpPtr = new FmodSound();
	//m_SndPowerUpPtr->CreateSound(String("Resources/Sounds/powerUp.mp3"));
	m_BmpSpriteSheetPtr = new Bitmap(String("Resources/marioanimations.png"));
	m_SndJumpPtr = new FmodSound();
	m_SndJumpPtr->CreateSound(String("Resources/Jump.ogg"));
}

Mario::~Mario()
{
	delete m_ActPtr;
	m_ActPtr = nullptr;
	//delete m_SndPowerUpPtr;
	//m_SndPowerUpPtr = nullptr;
	delete m_BmpSpriteSheetPtr;
	m_BmpSpriteSheetPtr = nullptr;
	delete m_SndJumpPtr;
	m_SndJumpPtr = nullptr;
	delete m_ActFeetPtr;
	m_ActFeetPtr = nullptr;
}

void Mario::Tick(double deltaTime, Level* levelPtr/*, PowerUpManager* powerUpManagerPtr*/)
{
	if (m_ActPtr->GetPosition().y > GAME_ENGINE->GetHeight())
	{
		m_ActPtr->SetPosition(25, 225);
	}
	if (GAME_ENGINE->IsKeyboardKeyDown(VK_SHIFT))
	{	
		m_HorSpeed = 750;
	}
	else
	{
		m_HorSpeed = m_Velocity;
	}
	if (levelPtr->EndReached())
	{
		GAME_ENGINE->QuitGame();
	}

	if (levelPtr->IsOnGround(m_ActFeetPtr))
	{
		m_ActPtr->SetLinearVelocity({ 0,m_ActPtr->GetLinearVelocity().y });
		if (GAME_ENGINE->IsKeyboardKeyDown(VK_UP) || GAME_ENGINE->IsKeyboardKeyDown(VK_SPACE) || GAME_ENGINE->IsKeyboardKeyDown('W'))
		{
			m_ActPtr->SetLinearVelocity({ m_ActPtr->GetLinearVelocity().x,-m_JumpSpeed });
			m_SndJumpPtr->Play();
		}
	}
	if (GAME_ENGINE->IsKeyboardKeyDown(VK_LEFT) || GAME_ENGINE->IsKeyboardKeyDown('A'))
	{
		m_ActPtr->SetLinearVelocity({ -m_HorSpeed,m_ActPtr->GetLinearVelocity().y });
	}
	else if (GAME_ENGINE->IsKeyboardKeyDown(VK_RIGHT) || GAME_ENGINE->IsKeyboardKeyDown('D'))
	{
		m_ActPtr->SetLinearVelocity({ m_HorSpeed,m_ActPtr->GetLinearVelocity().y });

	}
	
	if (m_ActPtr->GetLinearVelocity().y <= 1 && (GAME_ENGINE->IsKeyboardKeyDown(VK_LEFT) || GAME_ENGINE->IsKeyboardKeyDown(VK_RIGHT)))
	{
		m_ActionState = ActionState::MOVING;
	}
	else if (m_ActPtr->GetLinearVelocity().y == 0 && !(GAME_ENGINE->IsKeyboardKeyDown(VK_LEFT) || GAME_ENGINE->IsKeyboardKeyDown(VK_RIGHT)))
	{
		m_ActionState = ActionState::WAITING;
	}

	if (m_ActionState == ActionState::MOVING)
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
	}

	m_ActFeetPtr->SetPosition(m_ActPtr->GetPosition() + DOUBLE2(0, (m_HeightActor / 2) - 1));
}

void Mario::Paint(Level * levelPtr)
{

	DOUBLE2 position = { m_ActPtr->GetPosition().x - m_WidthActor / 2, m_ActPtr->GetPosition().y - m_HeightActor / 2 };
	//GAME_ENGINE->DrawString(String(3 - m_NrPowerUps) + String(" power-ups"),10,10);
	//GAME_ENGINE->SetColor(COLOR(255, 0, 0));
	//GAME_ENGINE->FillRect(position, position + DOUBLE2(m_WidthActor, m_HeightActor));
	/*GAME_ENGINE->SetColor(COLOR(255, 0, 255));
	GAME_ENGINE->DrawString(String(m_ActPtr->GetLinearVelocity().x),10,30);
	if (m_ActionState == ActionState::MOVING)
	{
		GAME_ENGINE->DrawString(String("MOVING"), position + DOUBLE2(5, 5));
	}
	else if (m_ActionState == ActionState::TRANSFORMING)
	{
		GAME_ENGINE->DrawString(String("TRANSFORMING"), position + DOUBLE2(5, 5));
		GAME_ENGINE->DrawString(String(m_AccuTransformTime), position + DOUBLE2(5, 20));
		switch (m_NrPowerUps)
		{
		case 0:
			GAME_ENGINE->DrawString(String("GREEN"), position + DOUBLE2(5, 40));
			break;
		case 1:
			GAME_ENGINE->DrawString(String("BLUE"), position + DOUBLE2(5, 40));
			break;
		case 2:
			GAME_ENGINE->DrawString(String("RED"), position + DOUBLE2(5, 40));
			break;
		case 3:
			GAME_ENGINE->DrawString(String("YELLOW"), position + DOUBLE2(5, 40));
			break;
		default:
			break;
		}
	}
	else if (m_ActionState == ActionState::WAITING)
	{
		GAME_ENGINE->DrawString(String("WAITING"), position + DOUBLE2(5, 5));
	}
	switch (m_NrPowerUps)
	{
	case 0:
		GAME_ENGINE->DrawString(String("GREEN"), position + DOUBLE2(5, 40));
		break;
	case 1:
		GAME_ENGINE->DrawString(String("BLUE"), position + DOUBLE2(5, 40));
		break;
	case 2:
		GAME_ENGINE->DrawString(String("RED"), position + DOUBLE2(5, 40));
		break;
	case 3:
		GAME_ENGINE->DrawString(String("YELLOW"), position + DOUBLE2(5, 40));
		break;
	default:
		break;
	}*/

	//game2
	DOUBLE2 pos = m_ActPtr->GetPosition();
	int width = m_BmpSpriteSheetPtr->GetWidth();
	int height = m_BmpSpriteSheetPtr->GetHeight();
	int frameY = 0;
	/*switch (m_PowerState)
	{
	case Mario::PowerState::GREEN:
		frameY = int(m_ActionState);
		break;
	case Mario::PowerState::BLUE:
	case Mario::PowerState::RED:
	case Mario::PowerState::YELLOW:
		frameY = (int(m_PowerState) * 3) + int(m_ActionState);
		break;
	default:
		break;
	}*/
	MATRIX3X2 matOrigWorld = GAME_ENGINE->GetWorldMatrix();
	if (m_ActPtr->GetLinearVelocity().x < 0)
	{
		matOrigWorld = GAME_ENGINE->GetWorldMatrix();
		MATRIX3X2 matCenter, matScale, matTranslate, matWorld;
		matCenter.SetAsTranslate(-m_ActPtr->GetPosition().x, -m_ActPtr->GetPosition().y);
		matScale.SetAsScale(-1, 1);
		matTranslate.SetAsTranslate(m_ActPtr->GetPosition());
		matWorld = matCenter * matScale * matTranslate;
		GAME_ENGINE->SetWorldMatrix(matWorld);
	}

	//animations
	if (!levelPtr->IsOnGround(m_ActFeetPtr))
	{
		GAME_ENGINE->DrawBitmap(m_BmpSpriteSheetPtr, pos.x - (m_ClipWidth / 2.0) - m_BitmapOffset, pos.y - (m_ClipHeight / 2.0) + m_BitmapOffset, { m_ClipWidth * 5.0, 0, double(m_ClipWidth) * 6, double(m_ClipHeight)});
	}
	else if (levelPtr->IsOnGround(m_ActFeetPtr) && (m_ActPtr->GetLinearVelocity().x < 0.1 && m_ActPtr->GetLinearVelocity().x > -0.1))
	{
		GAME_ENGINE->DrawBitmap(m_BmpSpriteSheetPtr, pos.x - (m_ClipWidth / 2.0) - m_BitmapOffset, pos.y - (m_ClipHeight / 2.0) + m_BitmapOffset, {0,0,double(m_ClipHeight),double(m_ClipWidth)});
	}
	else
	{
		GAME_ENGINE->DrawBitmap(m_BmpSpriteSheetPtr, pos.x - (m_ClipWidth/2.0) - m_BitmapOffset, pos.y - (m_ClipHeight / 2.0) + m_BitmapOffset,{ double(m_AnimFrame * m_ClipWidth) + m_ClipWidth, double(frameY * m_ClipHeight), ((m_AnimFrame + 1.0) * m_ClipWidth) + m_ClipWidth , (frameY + 1.0) * m_ClipHeight });
	}


	if (m_ActPtr->GetLinearVelocity().x < 0)
	{
		GAME_ENGINE->SetWorldMatrix(matOrigWorld);
	}
}

/*int Mario::GetNrPowerUps()
{
	return m_NrPowerUps;
}*/

/*RECT2 Mario::getRect()
{
	//does not return standard rect variables, instead uses the rect datatype to store postition, width and height. using a rect doesn't make any sense here to me.
	return{ m_ActPtr->GetPosition().x, m_ActPtr->GetPosition().y, double(m_ClipWidth), double(m_ClipHeight) };
}*/

DOUBLE2 Mario::getPos()
{
	return m_ActPtr->GetPosition();
}
