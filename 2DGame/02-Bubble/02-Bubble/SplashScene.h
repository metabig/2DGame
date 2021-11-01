#ifndef _SPLASHSCENE_INCLUDE
#define _SPLASHSCENE_INCLUDE

#include "Scene.h"

class SplashScene: public Scene {
	public:
		void init();
		void render();
		Sprite* background_sprite;
		Texture background;

};


#endif // _SPLASHSCENE_INCLUDE