#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
class HUD;
#include <fstream>

class ParseSession 
{
public:
	ParseSession();
	virtual ~ParseSession( );
	ParseSession( const ParseSession& ) = delete;
	ParseSession& operator=( const ParseSession& ) = delete;
	void SaveSessionInfoStart(HUD * HUDPtr);
	void SaveSessionInfoEnd(HUD * HUDPtr);
	std::string GetLastSessionData();
	std::string ParseInfo(std::string & sourceRef);

private: 
	std::ofstream m_InfoStringStream;
	std::string m_InputInfo;
	int m_LastIndex = -1;
	std::string m_ResultInfo;
};
