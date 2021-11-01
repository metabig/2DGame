#ifndef _SPLASHSCENE_INCLUDE
#define _SPLASHSCENE_INCLUDE

#include "Scene.h"

#define SPRITE_X_RATIO 0.333333333333333f  // 1/3
#define SPRITE_Y_RATIO 1.0f // 1

#define START_SCREEN 0
#define INSTRUCTION_SCREEN 1
#define CREDITS_SCREEN 2

class SplashScene: public Scene {
	public:
		void init();
		void render();
		void update(int deltaTime);
		Sprite* background_sprite;
		Texture background;

};


#endif // _SPLASHSCENE_INCLUDE