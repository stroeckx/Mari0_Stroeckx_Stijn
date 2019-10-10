//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "PortalParticleManager.h"
#include "PortalParticle.h"

//#define GAME_ENGINE (GameEngine::GetSingleton())

PortalParticleManager::PortalParticleManager(DOUBLE2& positionRef, DOUBLE2& normalRef)
{
	for (int i = 0; i < m_PortalParticleCount; i++)
	{
		m_PortalParticlePtrArr.push_back(nullptr);
	}
	CreatePortalParticles(positionRef, normalRef);
}

PortalParticleManager::~PortalParticleManager()
{
	ClearPortalParticles();
}

void PortalParticleManager::Tick(double deltaTime, DOUBLE2& positionRef, DOUBLE2& normalRef)
{
	if (m_Position != positionRef) CreatePortalParticles(positionRef, normalRef);

	for (PortalParticle * PortalParticlePtr : m_PortalParticlePtrArr)
	{
		if (PortalParticlePtr != nullptr)
		{
			PortalParticlePtr->Tick(deltaTime);
			if (abs(PortalParticlePtr->m_GetPosition().x - positionRef.x) > m_PortalHeight*1.5 || abs(PortalParticlePtr->m_GetPosition().y - positionRef.y) > m_PortalHeight*1.5)
			{
				RespawnPortalParticle(PortalParticlePtr,positionRef,normalRef);
			}
		}
	}
}

void PortalParticleManager::Paint()
{
	for (PortalParticle * PortalParticlePtr : m_PortalParticlePtrArr)
	{
		if (PortalParticlePtr != nullptr)
		{
			PortalParticlePtr->Paint();
		}
	}
}

void PortalParticleManager::CreatePortalParticles(DOUBLE2& positionRef, DOUBLE2& normalRef)
{
	m_Position = positionRef;
	m_Normal = normalRef;
	ClearPortalParticles();
	if (normalRef.x == 0)
	{
		for (int i = 0; i < m_PortalParticleCount; i++)
		{
			DOUBLE2 pos, vel;
			pos.x = positionRef.x + ((rand() % m_PortalWidth) - m_PortalWidth / 2);
			pos.y = positionRef.y + ((rand() % m_PortalHeight) * normalRef.y);
			vel.x = (rand() % 2) - 1;
			vel.y = ((rand() % 6) + 4) * normalRef.y;
			m_PortalParticlePtrArr[i] = new PortalParticle(pos, vel);
		}
	}
	else if (normalRef.y == 0)
	{
		for (int i = 0; i < m_PortalParticleCount; i++)
		{
			DOUBLE2 pos, vel;
			pos.x = positionRef.x + ((rand() % m_PortalHeight) * normalRef.x);
			pos.y = positionRef.y + ((rand() % m_PortalWidth) - m_PortalWidth / 2);
			vel.x = ((rand() % 6) + 4) * normalRef.x;
			vel.y = (rand() % 2) - 1;
			m_PortalParticlePtrArr[i] = new PortalParticle(pos, vel);
		}
	}

}

void PortalParticleManager::ClearPortalParticles()
{
	for (PortalParticle * PortalParticlePtr : m_PortalParticlePtrArr)
	{
		if (PortalParticlePtr != nullptr)
		{
			delete PortalParticlePtr;
			PortalParticlePtr = nullptr;
		}
	}
}

void PortalParticleManager::RespawnPortalParticle(PortalParticle*& PortalParticlePtrRef, DOUBLE2& positionRef, DOUBLE2& normalRef)
{
	if (normalRef.x == 0)
	{
			DOUBLE2 pos, vel;
			pos.x = positionRef.x + ((rand() % m_PortalWidth) - m_PortalWidth / 2);
			pos.y = positionRef.y;
			vel.x = (rand() % 2) - 1;
			vel.y = ((rand() % 6) + 4) * normalRef.y;
			PortalParticlePtrRef->SetPosition(pos);
			PortalParticlePtrRef->SetVelocity(vel);
	}
	else if (normalRef.y == 0)
	{
			DOUBLE2 pos, vel;
			pos.x = positionRef.x;
			pos.y = positionRef.y + ((rand() % m_PortalWidth) - m_PortalWidth / 2);
			vel.x = ((rand() % 6) + 4) * normalRef.x;
			vel.y = (rand() % 2) - 1;
			PortalParticlePtrRef->SetPosition(pos);
			PortalParticlePtrRef->SetVelocity(vel);
	}
}
