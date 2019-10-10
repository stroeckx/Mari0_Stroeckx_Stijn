#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------

#include "ContactListener.h"

class EndGameFlag : public ContactListener
{
public:
	EndGameFlag(DOUBLE2 position);
	virtual ~EndGameFlag( );
	EndGameFlag( const EndGameFlag& ) = delete;
	EndGameFlag& operator=( const EndGameFlag& ) = delete;

	virtual void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);

private: 
	PhysicsActor* m_ActPtr{};
	DOUBLE2 m_Area{ 16,152 };
};
