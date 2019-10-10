#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
class PortalParticle;

class PortalParticleManager 
{
public:
	PortalParticleManager( DOUBLE2& positionRef, DOUBLE2& normalRef );
	virtual ~PortalParticleManager( );
	PortalParticleManager( const PortalParticleManager& ) = delete;
	PortalParticleManager& operator=( const PortalParticleManager& ) = delete;
	void Tick(double deltaTime, DOUBLE2& positionRef, DOUBLE2& normalRef);
	void Paint();

private: 
	void ClearPortalParticles();
	void CreatePortalParticles(DOUBLE2& positionRef, DOUBLE2& normalRef);
	void RespawnPortalParticle(PortalParticle*& PortalParticlePtrRef, DOUBLE2& positionRef, DOUBLE2& normalRef);

	std::vector<PortalParticle *> m_PortalParticlePtrArr;
	int m_PortalParticleCount = 10;
	int m_PortalHeight = 10;
	int m_PortalWidth = 30;
	DOUBLE2 m_Position;
	DOUBLE2 m_Normal;
};
