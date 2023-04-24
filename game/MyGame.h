#pragma once

class CMyGame : public CGame
{
	// Define sprites and other instance variables here

	CSprite player;		// Player sprite
	CSprite background;	// Background sprite
	CSpriteList platforms;	// List of platforms
	CSpriteOval ball;
	CSprite MenuLevel1; // Level 1 Menu Sprite
	CSprite MenuLevel2; // Level 2 Menu Sprite

	bool back_colour;

	// Health Bar Sprites
	CSprite Healthbar10; // Max HP
	CSprite Healthbar9; // 90 HP
	CSprite Healthbar8; // 80 HP
	CSprite Healthbar7; // 70 HP
	CSprite Healthbar6; // 60 HP
	CSprite Healthbar5; // 50 HP
	CSprite Healthbar4; // 40 HP
	CSprite Healthbar3; // 30 HP
	CSprite Healthbar2; // 20 HP
	CSprite Healthbar1; // 10 HP
	CSprite Healthbar0; // 0 HP

	// Sounds
	CSoundPlayer MenuTheme;

	// Player State
	enum { STANDING, RUNNING, AIRBORNE, CLIMBING } m_state;
	enum { LEFT, RIGHT } m_side;
public:
	CMyGame(void);
	~CMyGame(void);

	bool gamewon;
	int keys_collected;
	int health;
	int MenuGameLV;

	virtual void PlayerController();
	virtual void BackgroundControl();

	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	// Game Life Cycle
	virtual void HealthBarControl();
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnStartLevel(Sint16 nLevel);
	virtual void OnGameOver();
	virtual void OnTerminate();
	virtual void MenuControl();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle);
	virtual void OnLButtonDown(Uint16 x, Uint16 y);
	virtual void OnLButtonUp(Uint16 x, Uint16 y);
	virtual void OnRButtonDown(Uint16 x, Uint16 y);
	virtual void OnRButtonUp(Uint16 x, Uint16 y);
	virtual void OnMButtonDown(Uint16 x, Uint16 y);
	virtual void OnMButtonUp(Uint16 x, Uint16 y);
};
