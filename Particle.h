#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------

class Particle 
{
public:
	Particle(DOUBLE2 position, double velocityX, double velocityY);
	virtual ~Particle( );
	Particle( const Particle& ) = delete;
	Particle& operator=( const Particle& ) = delete;
	void Tick(double deltaTime);
	void Paint();

private: 
	DOUBLE2 m_Position;
	DOUBLE2 m_Velocity;
};
