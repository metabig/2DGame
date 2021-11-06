#ifndef _LEVELSCENE_INCLUDE
#define _LEVELSCENE_INCLUDE

#include "Scene.h"
#include "Sound.h"

#define FILE_SPRITE_CACTUS "images/SpriteCactus.png"
#define FILE_SPRITE_LEVER "images/SpritePalanca.png"
#define FILE_SPRITE_BANDERA "images/SpriteBandera.png"
#define FILE_SPRITE_CACTUS_I "images/SpriteCactusInverted.png"
#define FILE_SPRITE_LEVER_I "images/SpritePalancaInverted.png"
#define FILE_SPRITE_BANDERA_I "images/SpriteBanderaInverted.png"

#define FILE_SPRITE_BARRIL "images/Sprite_Barrel.png"
#define FILE_SPRITE_CAIXA "images/Sprite_Box.png"
#define FILE_SPRITE_POTION "images/Sprite_Potion.png"
#define FILE_SPRITE_POTION_I "images/Sprite_Potion_I.png"

#define FILE_SPRITE_GODMODE "images/Sprite_GodMode.png"

#define FILE_SPRITE_LOADING "images/Sprite_Loading.png"


class LevelScene : public Scene {
public:
	void init(int levelToPlay);
	bool update(int deltaTime);
	void resetLevel();
	void render();
	Player *player;
	Player *inverted_player;
	TileMap *map;
	Texture background, textGodMode, textLoading;
	Texture bandera, lever, cactus, banderaI, leverI, cactusI, barril, caixa, potion, potionI;
	Sprite* background_sprite, *special_sprite;
	bool aux = false;
	vector<glm::ivec3> sprites_map;
	vector<Sprite*> sprites;
	string getFileByCode(int code);
	glm::vec2 getSpritesheetSizeByCode(int code);
	vector<int> sprites_codes;
	Sprite * godModeSprite;
	Sprite * loadingSprite;
	bool flag, inv_flag;
	bool god_mode; 
	int deathTimer;
	bool activateDeath = false;
	void deathSetup(bool inv);
	void deathCheckUp(int deltaTime);
	void setUpGodMode();
	void activateGodMode();
	void activateLever();

	int checkMoveCaixa(int id);
	void moveCaixa(int id, int d);
	Sprite* spriteCreator(int code);
	vector<int> deletedFromBarrier;

	bool readykey;
	int keyPressTimer;

	bool readyChangeLevel;
	int levelChangeTimer;

	bool powerUp;

	void setUpSound(Sound* s);

	void playerSoundSystem();
	void playSound(int type);

	Sound* soundSystem;
};


#endif // _LEVELSCENE_INCLUDE