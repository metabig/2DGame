#include "LevelScene.h"
#include "Scene.h"
#include "Game.h"
#include <GL/glut.h>

#include<iostream>

void LevelScene::init(int levelToPlay)
{
	god_mode = false;
	Scene::initShaders();

	bandera.loadFromFile(FILE_SPRITE_BANDERA, TEXTURE_PIXEL_FORMAT_RGBA);
	lever.loadFromFile(FILE_SPRITE_LEVER, TEXTURE_PIXEL_FORMAT_RGBA);
	cactus.loadFromFile(FILE_SPRITE_CACTUS, TEXTURE_PIXEL_FORMAT_RGBA);
	banderaI.loadFromFile(FILE_SPRITE_BANDERA_I, TEXTURE_PIXEL_FORMAT_RGBA);
	leverI.loadFromFile(FILE_SPRITE_LEVER_I, TEXTURE_PIXEL_FORMAT_RGBA);
	cactusI.loadFromFile(FILE_SPRITE_CACTUS_I, TEXTURE_PIXEL_FORMAT_RGBA);

	// Background

	background.loadFromFile("images/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background_sprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &background, &texProgram);

	// Map
	string fileLevel = "levels/Level" + std::to_string(levelToPlay) + ".txt";
	cout << "LevelScene::init() " << fileLevel << endl;
	sprites_map.clear();
	sprites_codes.clear();
	sprites.clear();
	map = TileMap::createTileMap(fileLevel, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	sprites_map = map->getSpriteInfo();

	cout << "Sprites:" << endl;
	for (int i = 0; i < sprites_map.size(); ++i) {
		cout << sprites_map[i].x << " " << sprites_map[i].y << " " << sprites_map[i].z << endl;
		int code = sprites_map[i].z;
		sprites_codes.push_back(code);
		cout << "File: " << getFileByCode(code) << endl;
		if (code == SPRITE_CACTUS) {
			special_sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0, 1.0), &cactus, &texProgram);
		}
		else if (code == SPRITE_INVERTEDCACTUS) {
			special_sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0, 1.0), &cactusI, &texProgram);
		}
		else if (code == SPRITE_LEVER) {
			cout << "i: " << i << endl;
			special_sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0 / 3.0, 1.0), &lever, &texProgram);
			special_sprite->setNumberAnimations(2);
			special_sprite->setAnimationSpeed(1, 15);
			special_sprite->addKeyframe(1, glm::vec2(0.0 / 3.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(1.0 / 3.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(2.0 / 3.0, 0.0));
			special_sprite->setAnimationSpeed(0, 1);
			special_sprite->addKeyframe(0, glm::vec2(0.0, 0.0));
			special_sprite->changeAnimation(0);

		}
		else if (code == SPRITE_INVERTEDLEVER) {
			special_sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0 / 3.0, 1.0), &leverI, &texProgram);
			special_sprite->setNumberAnimations(2);
			special_sprite->setAnimationSpeed(1, 15);
			special_sprite->addKeyframe(1, glm::vec2(0.0 / 3.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(1.0 / 3.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(2.0 / 3.0, 0.0));
			special_sprite->setAnimationSpeed(0, 1);
			special_sprite->addKeyframe(0, glm::vec2(0.0, 0.0));
			special_sprite->changeAnimation(0);
		} 
		else if (code == SPRITE_BANDERA) {
			special_sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0 / 4.0, 1.0), &bandera, &texProgram);
			flag = false;
			special_sprite->setNumberAnimations(2);

			special_sprite->setAnimationSpeed(0, 15);
			special_sprite->addKeyframe(0, glm::vec2(0.0 / 4.0, 0.0));

			special_sprite->setAnimationSpeed(1, 15);
			special_sprite->addKeyframe(1, glm::vec2(0.0 / 4.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(1.0 / 4.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(2.0 / 4.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(3.0 / 4.0, 0.0));

			special_sprite->setAnimationSpeed(2, 15);
			special_sprite->addKeyframe(2, glm::vec2(3.0 / 4.0, 0.0));
			special_sprite->addKeyframe(2, glm::vec2(2.0 / 4.0, 0.0));
			special_sprite->addKeyframe(2, glm::vec2(1.0 / 4.0, 0.0));
			special_sprite->addKeyframe(2, glm::vec2(0.0 / 4.0, 0.0));
			special_sprite->changeAnimation(0);
		}
		else if (code == SPRITE_INVERTEDBANDERA) {
			special_sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0 / 4.0, 1.0), &banderaI, &texProgram);
			inv_flag = false;
			special_sprite->setNumberAnimations(2);

			special_sprite->setAnimationSpeed(0, 15);
			special_sprite->addKeyframe(0, glm::vec2(0.0 / 4.0, 0.0));

			special_sprite->setAnimationSpeed(1, 15);
			special_sprite->addKeyframe(1, glm::vec2(0.0 / 4.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(1.0 / 4.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(2.0 / 4.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(3.0 / 4.0, 0.0));

			special_sprite->setAnimationSpeed(2, 15);
			special_sprite->addKeyframe(2, glm::vec2(3.0 / 4.0, 0.0));
			special_sprite->addKeyframe(2, glm::vec2(2.0 / 4.0, 0.0));
			special_sprite->addKeyframe(2, glm::vec2(1.0 / 4.0, 0.0));
			special_sprite->addKeyframe(2, glm::vec2(0.0 / 4.0, 0.0));

			special_sprite->changeAnimation(0);
		}
		cout << "Created sprite " << sprites_map[i].z << " at " << float(64 * sprites_map[i].x) + SCREEN_X << " " << float(64 * sprites_map[i].y) + SCREEN_Y << endl;
		special_sprite->setPosition(glm::vec2(float(64 * sprites_map[i].x) + SCREEN_X, float(64 * sprites_map[i].y) + SCREEN_Y));
		
		sprites.push_back(special_sprite);
	}

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

	Scene::init();
}

glm::vec2 LevelScene::getSpritesheetSizeByCode(int code)
{
	switch (code)
	{
	case SPRITE_INVERTEDCACTUS:
		//cols       rows
		return glm::vec2(1.0 / 1.0, 1.0 / 2.0);
	case SPRITE_CACTUS:
		return glm::vec2(1.0 / 1.0, 2.0 / 2.0);
	case SPRITE_LEVER:
		return glm::vec2(1.0 / 3.0, 1.0 / 2.0);
	case SPRITE_INVERTEDLEVER:
		return glm::vec2(1.0 / 3.0, 2.0 / 2.0);
	case SPRITE_BANDERA:
		return glm::vec2(1.0 / 2.0, 1.0 / 2.0);
	case SPRITE_INVERTEDBANDERA:
		return glm::vec2(2.0 / 2.0, 2.0 / 2.0);
	default:
		return glm::vec2();
	}
}

bool LevelScene::update(int deltaTime)
{
	Scene::update(deltaTime);

	if (Game::instance().getKey(103))
	{
		god_mode = !god_mode;
	}

	player->update(deltaTime);
	inverted_player->update(deltaTime);
	glm::ivec2 c = player->getPosition(); // 1
	glm::ivec2 c2 = inverted_player->getPosition(); // 1

	cout << "c.y: " << c.y << endl;
	cout << "Half: " << map->mapSize.x*(64/2) << endl;
	if (c.y > 608) resetLevel();

	c.x += 64 / 2;
	c.y += 64 / 2;
	c2.x += 64 / 2;
	c2.y += 64 / 2;


	for (int i = 0; i < sprites.size(); ++i) {
		glm::vec2 pA = sprites[i]->getPosition();

		if ((c.x >= pA.x && c.x <= pA.x + 64 && c.y >= pA.y && c.y <= pA.y + 64)
			|| (c2.x >= pA.x && c2.x <= pA.x + 64 && c2.y >= pA.y && c2.y <= pA.y + 64)) {
			switch (sprites_codes[i]) {
			case SPRITE_LEVER:
				cout << "Lever collision" << endl;
				if (sprites[i]->animation() != 1)
					sprites[i]->changeAnimation(1);
				break;
			case SPRITE_INVERTEDLEVER:
				cout << "Lever collision" << endl;
				if (sprites[i]->animation() != 1)
					sprites[i]->changeAnimation(1);
				break;
			case SPRITE_CACTUS:
				cout << "Cactus collision" << endl;
				resetLevel();
				break;
			case SPRITE_INVERTEDCACTUS:
				cout << "iCactus collision" << endl;
				resetLevel();
				break;
			case SPRITE_BANDERA:
				if (sprites[i]->animation() == 0) {
					sprites[i]->changeAnimation(1);
					flag = true;
				}
				break;
			case SPRITE_INVERTEDBANDERA:
				if (sprites[i]->animation() == 0) {
					sprites[i]->changeAnimation(1);
					inv_flag = true;
				}
				break;
			}

		}
		else if (sprites_codes[i] == SPRITE_BANDERA && sprites[i]->animation() == 1) {
			sprites[i]->changeAnimation(0);
			cout << "Set flag to false" << endl;
			flag = false;
		}
		else if (sprites_codes[i] == SPRITE_INVERTEDBANDERA && sprites[i]->animation() == 1) {
			sprites[i]->changeAnimation(0);
			cout << "Set invflag to false" << endl;
			inv_flag = false;
		}
		sprites[i]->updateOneWay(deltaTime);
	}
	if (flag) {
		cout << "flag" << endl;
	}
	if (inv_flag)
		cout << " invflag" << endl;
	if (flag && inv_flag) {
		cout << "FLAAAAGS!" << endl;
		return true;
	}
	return false;
}

void LevelScene::resetLevel() {
	player->setPosition(glm::vec2(map->getStartingX(UPRIGHT_PLAYER) * map->getTileSize(), map->getStartingY(UPRIGHT_PLAYER) * map->getTileSize()));
	inverted_player->setPosition(glm::vec2(map->getStartingX(INVERTED_PLAYER) * map->getTileSize(), map->getStartingY(INVERTED_PLAYER) * map->getTileSize()));
	for (int i = 0; i < sprites.size(); ++i) sprites[i]->changeAnimation(0);
}

void LevelScene::render()
{
	Scene::render();
	background_sprite->render();
	map->render();
	for (int i = 0; i < sprites.size(); ++i) {
		sprites[i]->render();
	}
	player->render();
	inverted_player->render();
}

string LevelScene::getFileByCode(int code)
{
	switch (code)
	{
		case SPRITE_CACTUS:
			return FILE_SPRITE_CACTUS;
		case SPRITE_INVERTEDCACTUS:
			return FILE_SPRITE_CACTUS;
		case SPRITE_LEVER:
			return FILE_SPRITE_LEVER;
		case SPRITE_INVERTEDLEVER:
			return FILE_SPRITE_LEVER;
		case SPRITE_BANDERA:
			return FILE_SPRITE_BANDERA;
		case SPRITE_INVERTEDBANDERA:
			return FILE_SPRITE_BANDERA;
		default:
			return "images/error.png";
	}
}
