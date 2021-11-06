#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Scene.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 150 //96
#define FALL_STEP 4

#define SPRITE_PLAYER_WIDTH 64
#define SPRITE_PLAYER_HEIGHT 64

#define SPRITE_X_RATIO 0.1f  // 1/10
#define SPRITE_Y_RATIO 0.125f // 1/8

#define SPRITE_X_DISPLACEMENT SPRITE_X_RATIO

#define NUMBER_FRAME_OF_ANIMATION 10
#define NUMBER_OF_ANIMATIONS 10

#define FRAME_PER_SECONDS 16

enum PlayerAnims
{
	MOVE_RIGHT, MOVE_LEFT, STAND_RIGHT, STAND_LEFT, JUMP_RIGHT, JUMP_LEFT, DIE_RIGHT, DIE_LEFT
};

void setAnimation(Sprite* sprite, int animId, int NFrames) {
	for (int i = 0; i < NFrames; i++) {
		sprite->setAnimationSpeed(animId, FRAME_PER_SECONDS);
		sprite->addKeyframe(animId, glm::vec2(SPRITE_X_DISPLACEMENT * i, SPRITE_Y_RATIO * animId));
	}
}


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool i)
{
	inverted = i;
	bJumping = false;
	falling = false;
	setPowerUp(false);
	string file;
	if (i) file = "images/Sprite_Player_Inv.png";
	else file = "images/Sprite_Player_Normal.png";
	spritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT), glm::vec2(SPRITE_X_RATIO, SPRITE_Y_RATIO), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);

	for (int anim_id = 0; anim_id < NUMBER_OF_ANIMATIONS; anim_id++)
	{
		setAnimation(sprite, anim_id, NUMBER_FRAME_OF_ANIMATION);//PlayerAnims
	}


	

	if (i) file = "images/efectos_polvo.png";
	else   file = "images/efectos_polvo.png";

	effectsspritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	effectssprite = Sprite::createSprite(glm::ivec2(2*SPRITE_PLAYER_WIDTH, 2*SPRITE_PLAYER_HEIGHT), glm::vec2(SPRITE_X_RATIO*2, SPRITE_Y_RATIO), &effectsspritesheet, &shaderProgram);
	effectssprite->setNumberAnimations(10);

	for (int anim_id = 0; anim_id < NUMBER_OF_ANIMATIONS; anim_id++)
	{
		setAnimation(effectssprite, anim_id, NUMBER_FRAME_OF_ANIMATION/2);//PlayerAnims
	}

	sprite->changeAnimation(0);
	effectssprite->changeAnimation(1);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	effectssprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x - 32), float(tileMapDispl.y + posPlayer.y -32)));

	setUpPotionEffects(shaderProgram);

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	potionEffectSprite->update(deltaTime);
	effectssprite->update(deltaTime);
	if (playerDead) return;
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
		else {
			playSound(SOUND_RUN);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
		else {
			playSound(SOUND_RUN);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		int jHeight = JUMP_HEIGHT;
		int jAngle = JUMP_ANGLE_STEP;
		if (powerUp) {
			jHeight = jHeight * 1.5;
			jAngle = jAngle / 1.5;
		}
		if (inverted) {
			jumpAngle -= jAngle;
			
			if (jumpAngle == -180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - jHeight * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle < -90)
					bJumping = !map->collisionMoveUp(posPlayer, glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT), &posPlayer.y);
			}
		}
		else {
			jumpAngle += jAngle;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - jHeight * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90) 
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT), &posPlayer.y);
			}
		}
		if (!bJumping) {
			playSound(SOUND_IMPACT);
			sprite->changeAnimation(STAND_RIGHT);
		}

	}
	else
	{
		if (!falling) {
			falling = true;
			fallingspeed = FALL_STEP;
		}
		else {
			fallingspeed += 2;
			fallingspeed = std::min(FALL_STEP * 2, fallingspeed);
		}
		
		if (inverted) {
			
			posPlayer.y -= fallingspeed;
			if (map->collisionMoveUp(posPlayer, glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT), &posPlayer.y))
			{
				falling = false;
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{		
					if(sprite->animation()==STAND_RIGHT || MOVE_RIGHT)
						sprite->changeAnimation(JUMP_RIGHT);
					else sprite->changeAnimation(JUMP_LEFT);
					playSound(SOUND_JUMP);
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}
		else {
			posPlayer.y += fallingspeed;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT), &posPlayer.y))
			{
				falling = false;
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					if (sprite->animation() == STAND_RIGHT || MOVE_RIGHT)
						sprite->changeAnimation(JUMP_RIGHT);
					else sprite->changeAnimation(JUMP_LEFT);
					playSound(SOUND_JUMP);
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	potionEffectSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x -32), float(tileMapDispl.y + posPlayer.y - 32)));
	effectssprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x- 32), float(tileMapDispl.y + posPlayer.y- 32)));
}

void Player::render()
{
	sprite->render();
	effectssprite->render();
	if (powerUp) potionEffectSprite->render();
}

void Player::death()
{
	if (!playerDead) {
		int anim = sprite->animation();
		if (anim  == MOVE_LEFT || anim == STAND_LEFT || anim == JUMP_LEFT)
			sprite->changeAnimation(DIE_RIGHT);
		else sprite->changeAnimation(DIE_LEFT);
	}
	playerDead = true;
	playSound(SOUND_DEATH);
}


void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::reset()
{
	playerDead = false;
	setPowerUp(false);
	sprite->changeAnimation(0);
	falling = false;
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

void Player::setUpPotionEffects(ShaderProgram &shaderProgram)
{
	potionEffectSpritesheet.loadFromFile("images/Sprite_potionEffects.png", TEXTURE_PIXEL_FORMAT_RGBA);
	potionEffectSprite = Sprite::createSprite(glm::ivec2(128, 128), glm::vec2(1.0 / 5.0, 1.0), &potionEffectSpritesheet, &shaderProgram);
	potionEffectSprite->setNumberAnimations(1);
	potionEffectSprite->setAnimationSpeed(0, 15);
	potionEffectSprite->addKeyframe(1, glm::vec2(0.0 / 5.0, 0.0));
	potionEffectSprite->addKeyframe(1, glm::vec2(1.0 / 5.0, 0.0));
	potionEffectSprite->addKeyframe(1, glm::vec2(2.0 / 5.0, 0.0));
	potionEffectSprite->addKeyframe(1, glm::vec2(3.0 / 5.0, 0.0));
	potionEffectSprite->addKeyframe(1, glm::vec2(4.0 / 5.0, 0.0));

	
}

void Player::setPowerUp(bool p)
{
	if (p && !powerUp) {
		potionEffectSprite->changeAnimation(1);
		soundSystem->playShortSound(SOUND_POTION);
	}
	powerUp = p;
}

void Player::setUpSoundSystem(Sound * s)
{
	soundSystem = s;
}

void Player::playSound(int type)
{
	soundSystem->playShortSound(type);
}



