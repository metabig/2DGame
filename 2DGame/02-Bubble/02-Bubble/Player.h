#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "Sound.h"
#include "TileMap.h"
#include <algorithm>


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool i);
	void update(int deltaTime);
	void render();
	void death();
	void setTileMap(TileMap *tileMap);

	void setPosition(const glm::vec2 &pos);
	bool playerDead;
	void reset();
	glm::ivec2 getPosition();

	void setUpPotionEffects(ShaderProgram &shaderProgram);
	void setPowerUp(bool p);
	void setUpSoundSystem(Sound * s);
private:
	
	bool inverted;
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	Texture effectsspritesheet;
	Sprite *effectssprite;

	Texture potionEffectSpritesheet;
	Sprite *potionEffectSprite;
	TileMap *map;
	bool powerUp;

	bool falling;
	int fallingspeed;

	void playSound(int type);

	Sound* soundSystem;

};


#endif // _PLAYER_INCLUDE


