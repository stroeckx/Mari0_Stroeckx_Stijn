//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "PortalGunParticleManager.h"
#include "PortalParticle.h"
#include "Mario.h"
#include<cmath>


#define GAME_ENGINE (GameEngine::GetSingleton())
	
PortalGunParticleManager::PortalGunParticleManager(DOUBLE2& startRef, DOUBLE2& endRef): m_StartPoint(startRef), m_EndPoint(endRef)
{
	m_PortalParticlePtrArr.resize(m_PortalParticleCount, nullptr);
	CreatePortalParticles();
}

PortalGunParticleManager::~PortalGunParticleManager()
{
	ClearPortalParticles();
}

void PortalGunParticleManager::Tick(double deltaTime)
{
	m_TimeActive += deltaTime;
	//m_MarioPos = marioPtr->getPos();
	MATRIX3X2 matView = GAME_ENGINE->GetViewMatrix();
	//DOUBLE2 mousePos = matView.Inverse().TransformPoint(GAME_ENGINE->GetMousePosition());
	DOUBLE2 directionVector = m_EndPoint - m_StartPoint;
	double hypotenuse = sqrt((directionVector.x * directionVector.x) + (directionVector.y*directionVector.y));
	m_Angle = asin((sin((90 * M_PI / 180)) * directionVector.y) / hypotenuse);
	if (directionVector.x < 0) m_Angle = ((M_PI / 2) + ((M_PI / 2) - m_Angle));

	for (PortalParticle * PortalParticlePtr : m_PortalParticlePtrArr)
	{
		if (PortalParticlePtr != nullptr) PortalParticlePtr->Tick(deltaTime);
	}
}

void PortalGunParticleManager::Paint()
{
	if (m_TimeActive < m_DecayTime)
	{
		COLOR ClrOriginal = GAME_ENGINE->GetColor();
		GAME_ENGINE->SetColor(COLOR(0, 0, 0, int((1-m_TimeActive) * 255)));

		MATRIX3X2 matWorldOriginal = GAME_ENGINE->GetWorldMatrix(), matTranslate, matRotate, matWorld;
		matTranslate.SetAsTranslate(m_StartPoint);
		matRotate.SetAsRotate(m_Angle);
		matWorld = matRotate*matTranslate;
		GAME_ENGINE->SetWorldMatrix(matWorld);

		for (PortalParticle * PortalParticlePtr : m_PortalParticlePtrArr)
		{
			if (PortalParticlePtr != nullptr) PortalParticlePtr->Paint();
		}

		GAME_ENGINE->SetWorldMatrix(matWorldOriginal);
		GAME_ENGINE->SetColor(ClrOriginal);
	}
}

void PortalGunParticleManager::CreatePortalParticles()
{
	ClearPortalParticles();
	int index = 0;
	for (double i = 0; i < m_Length; i += 3)
	{
		m_PortalParticlePtrArr[index] = new PortalParticle(DOUBLE2(i, 5 * sin(i/13)));
		index++;
	}
	int secondIndex = 0;
	for (double i = 0; i < m_Length; i += 3)
	{
		m_PortalParticlePtrArr[secondIndex+index] = new PortalParticle(DOUBLE2(i, 5 * -sin(i / 13)));
		secondIndex++;
	}
}

void PortalGunParticleManager::ClearPortalParticles()
{
	for (int i = 0; i < m_PortalParticleCount; i++)
	{
		delete m_PortalParticlePtrArr[i];
		m_PortalParticlePtrArr[i] = nullptr;
	}
}

void PortalGunParticleManager::ActivateNewAnimation(DOUBLE2 startPoint, DOUBLE2 endPoint, double length)
{
	m_StartPoint = startPoint;
	m_EndPoint = endPoint;
	m_Length = length;
	m_TimeActive = 0;
	CreatePortalParticles();
}