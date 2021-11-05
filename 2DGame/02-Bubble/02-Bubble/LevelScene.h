#ifndef _LEVELSCENE_INCLUDE
#define _LEVELSCENE_INCLUDE

#include "Scene.h"

#define FILE_SPRITE_CACTUS "images/SpriteCactus.png"
#define FILE_SPRITE_LEVER "images/SpritePalanca.png"
#define FILE_SPRITE_BANDERA "images/SpriteBandera.png"
#define FILE_SPRITE_CACTUS_I "images/SpriteCactusInverted.png"
#define FILE_SPRITE_LEVER_I "images/SpritePalancaInverted.png"
#define FILE_SPRITE_BANDERA_I "images/SpriteBanderaInverted.png"



class LevelScene : public Scene {
public:
	void init(int levelToPlay);
	bool update(int deltaTime);
	void resetLevel();
	void render();
	Player *player;
	Player *inverted_player;
	TileMap *map;
	Texture background;
	Texture bandera, lever, cactus, banderaI, leverI, cactusI;
	Sprite* background_sprite, *special_sprite;
	bool aux = false;
	vector<glm::ivec3> sprites_map;
	vector<Sprite*> sprites;
	string getFileByCode(int code);
	glm::vec2 getSpritesheetSizeByCode(int code);
	vector<int> sprites_codes;
	bool flag, inv_flag;
	bool god_mode = false;
};


#endif // _LEVELSCENE_INCLUDE