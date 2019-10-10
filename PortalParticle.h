#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------

class PortalParticle 
{
public:
	PortalParticle(DOUBLE2 position, DOUBLE2 velocity);
	PortalParticle(DOUBLE2 position);
	virtual ~PortalParticle( );
	PortalParticle( const PortalParticle& ) = delete;
	PortalParticle& operator=( const PortalParticle& ) = delete;
	void Tick(double deltaTime);
	void Paint();
	DOUBLE2 m_GetPosition();
	void SetPosition(DOUBLE2& positionRef);
	void SetVelocity(DOUBLE2& velocityRef);

private: 
	DOUBLE2 m_Position = { 0,0 };
	DOUBLE2 m_Velocity = { 0,0 };
	double m_Radius = 0.4;

};
