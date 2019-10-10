#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------
class ParseSession;

class HUD 
{
public:
	HUD( );
	virtual ~HUD( );
	HUD( const HUD& ) = delete;
	HUD& operator=( const HUD& ) = delete;
	void Tick(double deltaTime);
	void Paint(ParseSession* sessionPtr);
	void DecreaseLives();
	int GetLives();
	void drawStringBitmap(const String &text, const DOUBLE2 &position);
	RECT2 GetCharRect(char character);
	void AddCoin();
	void AddScore(int score, DOUBLE2 position);
	double GetTime();

private: 
	Bitmap* m_BmpFontPtr{};
	int m_Lives = 3;
	bool m_IsSessionDataOn = false;
	int m_NrOfCoins = 0;
	int m_Score = 000000;
	double m_Time = 400;
	int m_TextSize = 0;
	const int m_NrOfFrames = 3;
	const int m_NrFramesPerSec = 2;
	double m_AnimTime = 0.0;
	int m_AnimFrame = 0;
	Bitmap* m_BmpCoinHUDPtr{};
	double m_CoinWidth = 10;
	int m_LastScore = 0;
	DOUBLE2 m_LastScorePos;
	DOUBLE2 m_LastScorePosOffset;
	int m_LastScoreAlpha;
	Font* m_FntScorePtr{};
	Bitmap* m_BmpLivesPtr{};
};