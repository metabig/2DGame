#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "SplashScene.h"
#include "LevelScene.h"

#define SCREEN_WIDTH 25*64 //1920
#define SCREEN_HEIGHT 20*64 //896


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}


	static Game &instance()
	{
		static Game G;

		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;
	bool getSpecialKey(int key) const;


private:
	bool bPlay;                       // Continue to play game?
	bool changeLevel;					//true if we want to load a new level
	LevelScene levelscene;                      // Scene to render
	SplashScene splashscene;				  // Splash Screen
	bool isLevelscene = false;
	bool keys[256], specialKeys[256]; // Store key states so that 
									  // we can have access at any time
	int currentLevel;
	int maxLevel = 3;

};


#endif // _GAME_INCLUDE

