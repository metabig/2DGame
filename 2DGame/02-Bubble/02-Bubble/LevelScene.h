#ifndef _LEVELSCENE_INCLUDE
#define _LEVELSCENE_INCLUDE

#include "Scene.h"
#include "Enviorment.h"

class LevelScene : public Scene {
public:
	void init(int levelToPlay);
	void update(int deltaTime);
	void render();
	void initSprites();
	Player *player;
	Player *inverted_player;
	TileMap *map;
	Sprite* background_sprite;
	Texture background;
	vector<Enviorment *> vecSprites;
};


#endif // _LEVELSCENE_INCLUDE