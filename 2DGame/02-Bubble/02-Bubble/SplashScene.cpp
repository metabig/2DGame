#include<iostream>
#include "Game.h"
#include "SplashScene.h"
#include <GL/glut.h>


void SplashScene::init()
{
	Scene::initShaders();

	cout << "SplashScene::init()" << endl;
	background.loadFromFile("images/SplashScreenNew.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	background_sprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), 
												glm::vec2(SPRITE_X_RATIO, SPRITE_Y_RATIO), 
												&background, &texProgram);
	
	background_sprite->setNumberAnimations(3);

	background_sprite->addKeyframe(0, glm::vec2(0.3333333333f * 0, 0));
	background_sprite->addKeyframe(1, glm::vec2(0.3333333333f * 1, 1));
	background_sprite->addKeyframe(2, glm::vec2(0.3333333333f * 2, 2));

	background_sprite->changeAnimation(0);

	Scene::init();
}

void SplashScene::render()
{
	Scene::render();
	background_sprite->render();
}

void SplashScene::update(int deltaTime)
{
	Scene::update(deltaTime);
	if (Game::instance().getKey(104))
	{
		background_sprite->changeAnimation(INSTRUCTION_SCREEN);
	}
	else if (Game::instance().getKey(98))
	{
		background_sprite->changeAnimation(START_SCREEN);
	}
	else if (Game::instance().getKey(99))
	{
		background_sprite->changeAnimation(CREDITS_SCREEN);
	}
}

void SplashScene::setCreditsScreen() {
	background_sprite->changeAnimation(CREDITS_SCREEN);
}