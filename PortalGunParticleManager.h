#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
class PortalParticle;
class Mario;

class PortalGunParticleManager 
{
public:
	PortalGunParticleManager(DOUBLE2& startRef, DOUBLE2& endRef);
	virtual ~PortalGunParticleManager( );
	PortalGunParticleManager( const PortalGunParticleManager& ) = delete;
	PortalGunParticleManager& operator=( const PortalGunParticleManager& ) = delete;

	void Tick(double deltaTime);
	void Paint();
	void ActivateNewAnimation(DOUBLE2 startPoint, DOUBLE2 endPoint, double length);

private: 
	void ClearPortalParticles();
	void CreatePortalParticles();

	std::vector<PortalParticle *> m_PortalParticlePtrArr;
	int m_PortalParticleCount = 350;
	DOUBLE2 m_StartPoint;
	DOUBLE2 m_EndPoint;
	double m_Length = 0;
	//const int m_MaxLength = 500;
	//DOUBLE2 m_MarioPos = { 0,0 };
	bool m_IsActive = false;
	double m_Angle = 0;
	double m_DecayTime = 1;
	double m_TimeActive = 0;
};
