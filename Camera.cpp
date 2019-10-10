//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Camera.h"
#include "Level.h"
#include "Mario.h"


#define GAME_ENGINE (GameEngine::GetSingleton())

Camera::Camera(double width, double heigth, Mario * marioPtr, Level* levelPtr): m_Heigth(heigth), m_Width(width)
{
	m_Position = marioPtr->getPos();
	m_Position.y = levelPtr->GetHeight() / 2 * m_ScaleFactor;
	//m_targetPosition = m_Position;

}

Camera::~Camera()
{
}

void Camera::Paint(Level* levelPtr, Mario* marioPtr)
{
	DOUBLE2 pos{ 0,0 };
	TrackMario(pos, marioPtr);
	ClampToLevel(pos, levelPtr);
	COLOR activeClr = GAME_ENGINE->GetColor();
	GAME_ENGINE->SetColor(COLOR(0, 0, 255));
	GAME_ENGINE->DrawRect(pos.x - m_Width/2, pos.y - m_Heigth / 2, pos.x + m_Width / 2, pos.y + m_Heigth / 2);
	GAME_ENGINE->SetColor(activeClr);
	DOUBLE2 marioPos = marioPtr->getPos();
	DOUBLE2 ellPos = m_Position;
	ellPos.x = ellPos.x + ((m_Width / 2)*m_ScaleFactor) - m_DistanceFromSide;
	GAME_ENGINE->DrawEllipse(ellPos, 25, 25);
}

void Camera::TrackMario(DOUBLE2& posRef, Mario* marioPtr)
{
	DOUBLE2 marioPos = marioPtr->getPos();
	if ((marioPos.x < posRef.x - ((m_Width/2)*m_ScaleFactor) + m_DistanceFromSide ))
	{
		double distance = (posRef.x - ((m_Width / 2)*m_ScaleFactor) + m_DistanceFromSide) - marioPos.x;
		if (distance > 5)
		{
			posRef.x -= 5;
		}
		else
		{
			m_Position.x = marioPos.x + ((m_Width / 2)*m_ScaleFactor) - m_DistanceFromSide;
		}
	}
	else if ((marioPos.x > posRef.x + ((m_Width / 2)*m_ScaleFactor) - m_DistanceFromSide))
	{
		double distance = marioPos.x - (posRef.x + ((m_Width / 2)*m_ScaleFactor) - m_DistanceFromSide);
		if (distance > 5)
		{
			posRef.x += 5;
		}
		else
		{
			m_Position.x = marioPos.x - ((m_Width / 2)*m_ScaleFactor) + m_DistanceFromSide;
		}
	}
	//posRef.y = marioPos.y;
}

void Camera::ClampToLevel(DOUBLE2& posRef, Level* levelPtr)
{
	if ((posRef.x - (m_Width/2) * m_ScaleFactor) < 0)
	{
		posRef.x = (m_Width / 2) * m_ScaleFactor;
	}

	if ((posRef.x + (m_Width / 2) * m_ScaleFactor) > levelPtr->GetWidth())
	{
		posRef.x = levelPtr->GetWidth() - ((m_Width / 2) * m_ScaleFactor);
	}
}

MATRIX3X2 Camera::getViewMatrix(Level* levelPtr, Mario* marioPtr)
{
	//devtools
	if (GAME_ENGINE->IsKeyboardKeyDown(VK_ADD))	m_ScaleFactor -= 0.02;
	if (GAME_ENGINE->IsKeyboardKeyDown(VK_SUBTRACT)) m_ScaleFactor += 0.02;
	//


	TrackMario(m_Position, marioPtr);

	//ClampToLevel(m_targetPosition, levelPtr);

	/*if (m_targetPosition.x > m_Position.x + 2) m_Position.x += 1.3;
	else if (m_targetPosition.x < m_Position.x - 2) m_Position.x -= 1.3;
	else m_targetPosition = m_Position;*/

	ClampToLevel(m_Position, levelPtr);
	DOUBLE2 pos = m_Position;
	pos.x *= 1/ m_ScaleFactor;
	//m_Position.y *= 1/ m_ScaleFactor;
	MATRIX3X2 matTranslate, matScale, matView, matCenter;
	matCenter.SetAsTranslate(-(m_Width/2) * m_ScaleFactor, 0);
	matTranslate.SetAsTranslate(pos.x, pos.y);
	matScale.SetAsScale(m_ScaleFactor);
	matView = matTranslate * matScale * matCenter;
	return matView.Inverse();
}