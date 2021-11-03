#include "LevelScene.h"
#include "Scene.h"
#include "Game.h"
#include<iostream>

void LevelScene::init(int levelToPlay)
{
	Scene::initShaders();

	// Background

	background.loadFromFile("images/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background_sprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &background, &texProgram);

	// Map
	string fileLevel = "levels/Level" + std::to_string(levelToPlay) + ".txt";
	cout << "LevelScene::init() " << fileLevel << endl;
	vecSprites.clear();
	map = TileMap::createTileMap(fileLevel, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	// Upright Player
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	player->setPosition(glm::vec2(map->getStartingX(UPRIGHT_PLAYER) * map->getTileSize(), map->getStartingY(UPRIGHT_PLAYER) * map->getTileSize()));
	player->setTileMap(map);

	// Inverted Player
	inverted_player = new Player();
	inverted_player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
	inverted_player->setPosition(glm::vec2(map->getStartingX(INVERTED_PLAYER) * map->getTileSize(), map->getStartingY(INVERTED_PLAYER) * map->getTileSize()));
	inverted_player->setTileMap(map);

	initSprites();
	Scene::init();
}

void LevelScene::update(int deltaTime)
{
	Scene::update(deltaTime);
	player->update(deltaTime);
	inverted_player->update(deltaTime);
}

void LevelScene::render()
{
	Scene::render();
	background_sprite->render();
	map->render();
	player->render();
	inverted_player->render();
	for each (Enviorment* e in vecSprites)
	{
		e->render();
	}
}

void LevelScene::initSprites() {
	vector<int> spriteInfo = map->getSpriteInfo();
	for (size_t i = 0; i < spriteInfo.size(); i = i + 3)
	{
		int x = spriteInfo[i];
		int y = spriteInfo[i + 1];
		int type = spriteInfo[i + 2];

		Enviorment* env = new Enviorment();
		env->init(glm::ivec2(x* map->getTileSize(), y* map->getTileSize()), texProgram, type);
		vecSprites.push_back(env);
		cout << "Created Sprite at: " << x << " " << y << " " << type << endl;
	}
}