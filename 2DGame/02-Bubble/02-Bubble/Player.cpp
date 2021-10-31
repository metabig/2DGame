#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define SPRITE_PLAYER_WIDTH 64
#define SPRITE_PLAYER_HEIGHT 64

#define SPRITE_X_RATIO 0.1f  // 1/6
#define SPRITE_Y_RATIO 0.125f // 1/8

#define SPRITE_X_DISPLACEMENT SPRITE_X_RATIO

#define NUMBER_FRAME_OF_ANIMATION 10
#define NUMBER_OF_ANIMATIONS 10

#define FRAME_PER_SECONDS 16

enum PlayerAnims
{
	MOVE_RIGHT, MOVE_LEFT, STAND_RIGHT, STAND_LEFT, JUMP_RIGHT, JUMP_LEFT, DIE_RIGHT, DIE_LEFT
};

void setAnimation(Sprite* sprite, int animId) {
	for (int i = 0; i < NUMBER_FRAME_OF_ANIMATION; i++) {
		sprite->setAnimationSpeed(animId, FRAME_PER_SECONDS);
		sprite->addKeyframe(animId, glm::vec2(SPRITE_X_DISPLACEMENT * i, SPRITE_Y_RATIO * animId));
	}
}


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool i)
{
	inverted = i;
	bJumping = false;
	string file;
	if (i) file = "images/Sprite_Player_Inv.png";
	else file = "images/Sprite_Player_Normal.png";
	spritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT), glm::vec2(SPRITE_X_RATIO, SPRITE_Y_RATIO), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);

	for (int anim_id = 0; anim_id < NUMBER_OF_ANIMATIONS; anim_id++)
	{
		setAnimation(sprite, anim_id);//PlayerAnims
	}


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
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
		if (inverted) {
			jumpAngle -= JUMP_ANGLE_STEP;

			if (jumpAngle == -180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle < -90)
					bJumping = !map->collisionMoveUp(posPlayer, glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT), &posPlayer.y);
			}
		}
		else {
			jumpAngle += JUMP_ANGLE_STEP;

			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90) 
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT), &posPlayer.y);
			}
		}
		if (!bJumping)sprite->changeAnimation(STAND_RIGHT);

	}
	else
	{
		if (inverted) {
			posPlayer.y -= FALL_STEP;
			if (map->collisionMoveUp(posPlayer, glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT), &posPlayer.y))
			{
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					sprite->changeAnimation(JUMP_RIGHT);
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}
		else {
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(SPRITE_PLAYER_WIDTH, SPRITE_PLAYER_HEIGHT), &posPlayer.y))
			{
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					sprite->changeAnimation(JUMP_RIGHT);
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
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




