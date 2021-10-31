#include<iostream>
#include "Game.h"
#include "SplashScene.h"


void SplashScene::init()
{
	Scene::initShaders();

	cout << "SplashScene::init()" << endl;
	background.loadFromFile("images/splashscreen.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	background_sprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &background, &texProgram);
	Scene::init();
}

void SplashScene::render()
{
	Scene::render();
	background_sprite->render();
}
