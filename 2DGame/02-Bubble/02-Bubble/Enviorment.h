#pragma once
#include "Sprite.h"
#include "TileMap.h"

#define FILE_SPRITE_CACTUS "images/SpriteCactus.png"
#define FILE_SPRITE_LEVER "images/SpritePalanca.png"
#define FILE_SPRITE_BANDERA "images/SpriteBandera.png"
#define FILE_SPRITE_CACTUS_I "images/SpriteCactusInverted.png"
#define FILE_SPRITE_LEVER_I "images/SpritePalancaInverted.png"
#define FILE_SPRITE_BANDERA_I "images/SpriteBanderaInverted.png"

#define FRAME_BANDERA 2
#define FRAME_CACTUS 1
#define FRAME_LEVER 3

#define ENV_WIDTH 64
#define ENV_HEIGHT 64
// Enviorment is an Entity without movement


class Enviorment
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int type);
	void action();
	void render();

	void setTileMap(TileMap *tileMap);


	string getFileSprite();


private:

	void setAnimation(int anim_id, int framesPerSecond);

	vector<float> getParametersAnimation();
	glm::ivec2 position;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int type;



};
