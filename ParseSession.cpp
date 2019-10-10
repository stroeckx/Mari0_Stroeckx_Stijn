//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "ParseSession.h"
#include "HUD.h"
#define GAME_ENGINE (GameEngine::GetSingleton())

ParseSession::ParseSession()
{
	//if the game crashed, remove the last saved data since it is incomplete
	std::ifstream readInfoStringStream;
	readInfoStringStream.open("Resources/GameSessions.txt");
	if (readInfoStringStream.fail())
	{
		std::cout << "Reading file failed";
	}
	std::string lastLine, tempLine;
	while (!readInfoStringStream.eof())
	{
		std::getline(readInfoStringStream, tempLine);
		if (tempLine != "")
		{
			lastLine = tempLine;
		}
	}
	readInfoStringStream.close();

	if (lastLine == "\t</Start>")
	{
		std::cout << "Removing last incomplete save data." << std::endl;

		std::ifstream readInfoStringStream;
		readInfoStringStream.open("Resources/GameSessions.txt");
		if (readInfoStringStream.fail())
		{
			std::cout << "Reading file failed" << std::endl;
		}
		std::string InputInfoTemp;
		std::string InputInfo;
		while (!readInfoStringStream.eof())
		{
			std::getline(readInfoStringStream, InputInfoTemp);
			InputInfo += InputInfoTemp;
			InputInfo += "\n";
		}
		readInfoStringStream.close();
		int index = InputInfo.rfind("<Session>");
		InputInfo = InputInfo.substr(0,index);
	//	std::cout << InputInfo << std::endl;
	//	std::cout << index << std::endl;
		std::ofstream correctedInfo;
		correctedInfo.open("Resources/GameSessions.txt");
		correctedInfo << InputInfo;
		correctedInfo.close();

	}
}

ParseSession::~ParseSession()
{
}

void ParseSession::SaveSessionInfoStart(HUD * HUDPtr)
{
	SYSTEMTIME sysTime{};
	GetSystemTime(&sysTime);
	std::stringstream stringstream;

	m_InfoStringStream.open("Resources/GameSessions.txt", std::ios::app);
	m_InfoStringStream << "<Session>" << std::endl << "	<Start>" << std::endl << "		<Date>";
	m_InfoStringStream << sysTime.wYear << "-" << std::setw(2) << std::setfill('0') << sysTime.wMonth << "-" << std::setw(2) << std::setfill('0') << sysTime.wDay;
	m_InfoStringStream << "</Date>" << std::endl << "		<Time>";
	m_InfoStringStream << std::setw(2) << std::setfill('0') << sysTime.wHour << "-" << std::setw(2) << std::setfill('0') << sysTime.wMinute << "-" << std::setw(2) << std::setfill('0') << sysTime.wSecond;
	m_InfoStringStream << "</Time>" << std::endl << "		<Lives>" << HUDPtr->GetLives() << "</Lives>" << std::endl;
	m_InfoStringStream << "	</Start>" << std::endl;
	m_InfoStringStream.close();
	//std::cout << m_InfoStringStream.str();
}
 
void ParseSession::SaveSessionInfoEnd(HUD * HUDPtr)
{
	SYSTEMTIME sysTime{};
	GetSystemTime(&sysTime);
	std::stringstream stringstream;

	m_InfoStringStream.open("Resources/GameSessions.txt", std::ios::app);
	m_InfoStringStream << "	<End>" << std::endl  << "		<Date>";
	m_InfoStringStream << sysTime.wYear << "-" << std::setw(2) << std::setfill('0') << sysTime.wMonth << "-" << std::setw(2) << std::setfill('0') << sysTime.wDay;
	m_InfoStringStream << "</Date>" << std::endl << "		<Time>";
	m_InfoStringStream << std::setw(2) << std::setfill('0') << sysTime.wHour << "-" << std::setw(2) << std::setfill('0') << sysTime.wMinute << "-" << std::setw(2) << std::setfill('0') << sysTime.wSecond;
	m_InfoStringStream << "</Time>" << std::endl << "		<Lives>" << HUDPtr->GetLives() << "</Lives>" << std::endl;
	m_InfoStringStream << "	</End>" << std::endl << "</Session>" << std::endl;
	m_InfoStringStream.close();
	//std::cout << m_InfoStringStream.str() << std::endl;
}

std::string ParseSession::GetLastSessionData()
{
	int index = -1000;
	if (m_LastIndex > 0)
	{
		index = m_LastIndex;
	}
	if (GAME_ENGINE->IsKeyboardKeyPressed(VK_PRIOR))
	{
		if (index > 1)
		{
			--index;
		}
	}
	if (GAME_ENGINE->IsKeyboardKeyPressed(VK_NEXT))
	{
		++index;
	}
	if (index == m_LastIndex)
	{
		return m_ResultInfo;
	}

	std::ifstream readInfoStringStream;
	readInfoStringStream.open("Resources/GameSessions.txt");
	if (readInfoStringStream.fail())
	{
		return "Reading file failed";
	}
	std::string InputInfoTemp;
	while (!readInfoStringStream.eof())
	{
		std::getline(readInfoStringStream, InputInfoTemp);
		m_InputInfo += InputInfoTemp;
		m_InputInfo += "\n";
	}
	readInfoStringStream.close();

	
	m_InputInfo = m_InputInfo.substr(0,m_InputInfo.rfind("<Session>"));
	int count = 0;
	for (size_t i = m_InputInfo.find("<Session>"); i != std::string::npos; i = m_InputInfo.find("<Session>", i + 8))
	{
		++count;
	}
	if (index < 0 || index > count)
	{
		index = count;
	}
	//m_LastIndex = count;
	for (int i = 0; i < index-1; i++)
	{
		m_InputInfo = m_InputInfo.substr(m_InputInfo.find("</Session>") + 10);
	}
	m_InputInfo += "";
	m_ResultInfo = "";
	m_ResultInfo += ParseInfo(m_InputInfo);
	m_ResultInfo += "\n";
	m_ResultInfo += ParseInfo(m_InputInfo);
	//std::cout << ResultInfo << std::endl;
	m_LastIndex = index;
	return m_ResultInfo;
}

std::string ParseSession::ParseInfo(std::string & sourceRef)
{
	//std::string sourceRef = sourceRef2;
	std::string info;
	int test = sourceRef.find("<Date>");
	if (test<0)
	{
		return "An error was made while saving, data cannot be read.";
	}
	
	sourceRef = sourceRef.substr(sourceRef.find("<Date>"));
	sourceRef = sourceRef.substr(sourceRef.find(">") + 1);
	info += sourceRef.substr(0, sourceRef.find("</Date>"));
	info += "\n";
	sourceRef = sourceRef.substr(sourceRef.find("<Time>"));
	sourceRef = sourceRef.substr(sourceRef.find(">") + 1);
	info += sourceRef.substr(0, sourceRef.find("</Time>"));
	info += "\n";
	sourceRef = sourceRef.substr(sourceRef.find("<Lives>"));
	sourceRef = sourceRef.substr(sourceRef.find(">") + 1);
	info += sourceRef.substr(0, sourceRef.find("</Lives>"));
	info += "\n";
	return info;
}