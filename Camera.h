#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
class Level;
class Mario;
class Camera 
{
public:
	Camera(double width, double heigth, Mario * marioPtr, Level* levelPtr);
	virtual ~Camera( );
	Camera( const Camera& ) = delete;
	Camera& operator=( const Camera& ) = delete;
	void Paint(Level* levelPtr, Mario* marioPtr);
	MATRIX3X2 getViewMatrix(Level* levelPtr, Mario * marioPtr);
private: 
	void TrackMario(DOUBLE2& posRef, Mario* MarioPtr);
	void ClampToLevel(DOUBLE2& posRef, Level* levelPtr);
	
	double m_Width;
	double m_Heigth;
	double m_ScaleFactor = 0.44;
	DOUBLE2 m_Position;
	//DOUBLE2 m_targetPosition;
	double m_DistanceFromSide = 100;
	//double m_CameraPrecisionRange = 25;
	double m_PanSpeed;
};