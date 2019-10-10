#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "ContactListener.h"
class FlipDirectionSensor : public ContactListener
{
public:
	FlipDirectionSensor(DOUBLE2 position );
	virtual ~FlipDirectionSensor( );
	FlipDirectionSensor( const FlipDirectionSensor& ) = delete;
	FlipDirectionSensor& operator=( const FlipDirectionSensor& ) = delete;
	virtual void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);


private: 
	PhysicsActor* m_ActPtr;
};
