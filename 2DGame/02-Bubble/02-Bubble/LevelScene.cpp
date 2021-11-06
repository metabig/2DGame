#include "LevelScene.h"
#include "Scene.h"
#include "Game.h"
#include <GL/glut.h>

#include<iostream>

void LevelScene::init(int levelToPlay)
{
	god_mode = false;
	readykey = true;
	powerUp = false;
	deletedFromBarrier.clear();
	Scene::initShaders();

	bandera.loadFromFile(FILE_SPRITE_BANDERA, TEXTURE_PIXEL_FORMAT_RGBA);
	lever.loadFromFile(FILE_SPRITE_LEVER, TEXTURE_PIXEL_FORMAT_RGBA);
	cactus.loadFromFile(FILE_SPRITE_CACTUS, TEXTURE_PIXEL_FORMAT_RGBA);
	banderaI.loadFromFile(FILE_SPRITE_BANDERA_I, TEXTURE_PIXEL_FORMAT_RGBA);
	leverI.loadFromFile(FILE_SPRITE_LEVER_I, TEXTURE_PIXEL_FORMAT_RGBA);
	cactusI.loadFromFile(FILE_SPRITE_CACTUS_I, TEXTURE_PIXEL_FORMAT_RGBA);
	barril.loadFromFile(FILE_SPRITE_BARRIL, TEXTURE_PIXEL_FORMAT_RGBA);
	caixa.loadFromFile(FILE_SPRITE_CAIXA, TEXTURE_PIXEL_FORMAT_RGBA);
	potion.loadFromFile(FILE_SPRITE_POTION, TEXTURE_PIXEL_FORMAT_RGBA);
	potionI.loadFromFile(FILE_SPRITE_POTION_I, TEXTURE_PIXEL_FORMAT_RGBA);

	setUpGodMode();
	
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
	for (size_t i = 0; i < sprites_map.size(); ++i) {
		cout << sprites_map[i].x << " " << sprites_map[i].y << " " << sprites_map[i].z << endl;
		int code = sprites_map[i].z;
		sprites_codes.push_back(code);
		cout << "File: " << getFileByCode(code) << endl;
		special_sprite = spriteCreator(code);
		
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

void LevelScene::deathSetup(bool inv)
{
	if (!activateDeath) {
		activateDeath = true;
		deathTimer = 625;
		
	}
	if (inv) {
		inverted_player->death();
	}
	else {
		player->death();
	}
}

void LevelScene::deathCheckUp(int deltaTime)
{
	if (activateDeath) {
		deathTimer -= deltaTime;
		if (deathTimer < 0) {
			activateDeath = false;
			resetLevel();
		}
	}
}

void LevelScene::setUpGodMode()
{
	textGodMode.loadFromFile(FILE_SPRITE_GODMODE, TEXTURE_PIXEL_FORMAT_RGBA);
	godModeSprite = Sprite::createSprite(glm::ivec2(256, 64), glm::vec2(1.0, 1.0), &textGodMode, &texProgram);
	godModeSprite->setPosition(glm::vec2(SCREEN_WIDTH - 300, 20));
	god_mode = false;

	textLoading.loadFromFile(FILE_SPRITE_LOADING, TEXTURE_PIXEL_FORMAT_RGBA);
	loadingSprite = Sprite::createSprite(glm::ivec2(512, 128), glm::vec2(1.0, 1.0), &textLoading, &texProgram);
	loadingSprite->setPosition(glm::vec2(30, 20));
}

void LevelScene::activateGodMode()
{
	god_mode = !god_mode;
}

void LevelScene::activateLever()
{
	if (deletedFromBarrier.size()>0) return;
	map->setBarrelCollision(false);
	for (size_t i = 0; i < sprites.size(); ++i) {
		if (sprites_codes[i]== SPRITE_BARRIL) {
			deletedFromBarrier.push_back(i);
		}
	}
	playSound(SOUND_LEVER);
}

int LevelScene::checkMoveCaixa(int id)
{

	glm::vec2 pA = sprites[id]->getPosition();
	glm::ivec2 posP =  player->getPosition();
	if (abs(pA.y - posP.y) > 32) return 0;
	if (abs(pA.x - posP.x) < 90) {
		if (pA.x > posP.x) return 1;
		else if(abs(pA.x - posP.x) < 37) return -1;
	}
	return 0;
}

void LevelScene::moveCaixa(int id, int d)
{
	if (d == 0) return;
	glm::vec2 pA = sprites[id]->getPosition();
	if (map->collisionBox(d) ) return;
	sprites[id]->setPosition(glm::vec2(pA.x+d*2, pA.y));
	map->setPositionCaixa(pA.x + d * 2, pA.y);
	soundSystem->playShortSound(SOUND_GRUNT);
}

Sprite* LevelScene::spriteCreator(int code)
{
	cout << "code: " << code << endl;
	Sprite * special_sprite;
	switch(code){
		case SPRITE_CACTUS:
			return Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0, 1.0), &cactus, &texProgram);
	
		case SPRITE_INVERTEDCACTUS:
			return Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0, 1.0), &cactusI, &texProgram);
	
		case SPRITE_LEVER:
			special_sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0 / 3.0, 1.0), &lever, &texProgram);
			special_sprite->setNumberAnimations(2);
			special_sprite->setAnimationSpeed(1, 15);
			special_sprite->addKeyframe(1, glm::vec2(0.0 / 3.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(1.0 / 3.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(2.0 / 3.0, 0.0));
			special_sprite->setAnimationSpeed(0, 1);
			special_sprite->addKeyframe(0, glm::vec2(0.0, 0.0));
			special_sprite->changeAnimation(0);
			return special_sprite;

		case SPRITE_INVERTEDLEVER:
			special_sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0 / 3.0, 1.0), &leverI, &texProgram);
			special_sprite->setNumberAnimations(2);
			special_sprite->setAnimationSpeed(1, 15);
			special_sprite->addKeyframe(1, glm::vec2(0.0 / 3.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(1.0 / 3.0, 0.0));
			special_sprite->addKeyframe(1, glm::vec2(2.0 / 3.0, 0.0));
			special_sprite->setAnimationSpeed(0, 1);
			special_sprite->addKeyframe(0, glm::vec2(0.0, 0.0));
			special_sprite->changeAnimation(0);
			return special_sprite;
		case SPRITE_BANDERA:
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
			return special_sprite;
		case SPRITE_INVERTEDBANDERA:
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
			return special_sprite;

		case SPRITE_BARRIL:
			return Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0, 1.0), &barril, &texProgram);
		case SPRITE_CAIXA:
			return Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0, 1.0), &caixa, &texProgram);

		case SPRITE_POWERUP:
			return Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0, 1.0), &potion, &texProgram);
		case SPRITE_INVERTEDPOWERUP:
			return Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0, 1.0), &potionI, &texProgram);


	}

}

void LevelScene::setUpSound(Sound * s)
{
	soundSystem = s;
}

void LevelScene::playerSoundSystem()
{
	player->setUpSoundSystem(soundSystem);
	inverted_player->setUpSoundSystem(soundSystem);
}

void LevelScene::playSound(int type)
{
	soundSystem->playShortSound(type);
}



bool LevelScene::update(int deltaTime)
{
	Scene::update(deltaTime);
	LevelScene::deathCheckUp(deltaTime);
	if (readykey) {
		readykey = false;
		keyPressTimer = 60;
		if (Game::instance().getKey(103))//g
		{
			activateGodMode();
		}
		if (Game::instance().getKey(100))//d
		{
			activateLever();
		}
	}
	else {
		keyPressTimer -= deltaTime;
		if (keyPressTimer < 0)readykey = true;
	}

	player->update(deltaTime);
	inverted_player->update(deltaTime);
	glm::ivec2 c = player->getPosition(); // 1
	glm::ivec2 c2 = inverted_player->getPosition(); // 1

	//cout << "c.y: " << c.y << endl;
	//cout << "Half: " << map->mapSize.x*(64/2) << endl;
	if (c.y > 608) deathSetup(false);
	if (c2.y < 608) deathSetup(true);

	c.x += 64 / 2;
	c.y += 64 / 2;
	c2.x += 64 / 2;
	c2.y += 64 / 2;


	for (size_t i = 0; i < sprites.size(); ++i) {
		if (sprites_codes[i] == SPRITE_CAIXA) {
			moveCaixa(i, checkMoveCaixa(i));
			continue;
		}
		glm::vec2 pA = sprites[i]->getPosition();
		bool found = false;
		for (size_t j = 0; j < deletedFromBarrier.size() && !found; ++j)
		{
			found = (deletedFromBarrier[j] == i);//Finds the ones deleted because Lever
		}
		if(found) continue;//if delted from play go to next sprite

		if ((c.x >= pA.x && c.x <= pA.x + 64 && c.y >= pA.y && c.y <= pA.y + 64)
			|| (c2.x >= pA.x && c2.x <= pA.x + 64 && c2.y >= pA.y && c2.y <= pA.y + 64)) {
			switch (sprites_codes[i]) {
			case SPRITE_LEVER:
			case SPRITE_INVERTEDLEVER:
				//cout << "Lever collision" << endl;
				if (sprites[i]->animation() != 1) {
					activateLever();
					sprites[i]->changeAnimation(1);
				}
				break;
			case SPRITE_CACTUS:
				//cout << "Cactus collision" << endl;
				if (!god_mode)deathSetup(false);
				break;
			case SPRITE_INVERTEDCACTUS:
				//cout << "Cactus collision" << endl;
				if(!god_mode)deathSetup(true);
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
			case SPRITE_POWERUP:
				inverted_player->setPowerUp(true);
				powerUp = true;
				break;
			case SPRITE_INVERTEDPOWERUP:
				player->setPowerUp(true);
				powerUp = true;
				break;
			}

		}
		else if (sprites_codes[i] == SPRITE_BANDERA && sprites[i]->animation() == 1) {
			sprites[i]->changeAnimation(0);
			flag = false;
		}
		else if (sprites_codes[i] == SPRITE_INVERTEDBANDERA && sprites[i]->animation() == 1) {
			sprites[i]->changeAnimation(0);
			inv_flag = false;
		}
		sprites[i]->updateOneWay(deltaTime);
	}
	if (flag && inv_flag) {
		playSound(SOUND_VICTORY);
		levelChangeTimer -= deltaTime;
		readyChangeLevel = levelChangeTimer<0;
	}
	else {
		readyChangeLevel = false;
		levelChangeTimer = 250;
	}
	return readyChangeLevel;
}

void LevelScene::resetLevel() {
	player->setPosition(glm::vec2(map->getStartingX(UPRIGHT_PLAYER) * map->getTileSize(), map->getStartingY(UPRIGHT_PLAYER) * map->getTileSize()));
	player->reset();
	inverted_player->setPosition(glm::vec2(map->getStartingX(INVERTED_PLAYER) * map->getTileSize(), map->getStartingY(INVERTED_PLAYER) * map->getTileSize()));
	inverted_player->reset();
	for (size_t i = 0; i < sprites.size(); ++i) {
		if (sprites_codes[i] == SPRITE_CAIXA) {//reset caixa to starting position
			sprites[i]->setPosition(map->resetPositionCaixa());
		}
		sprites[i]->changeAnimation(0);
	}
	deletedFromBarrier.clear();//reset barrels deleted because lever
	map->setBarrelCollision(true);
	powerUp = false;
}

void LevelScene::render()
{
	Scene::render();
	background_sprite->render();
	map->render();
	for (size_t i = 0; i < sprites.size(); ++i) {
		switch (sprites_codes[i]) {
		case SPRITE_BARRIL:
			if(deletedFromBarrier.size()==0) sprites[i]->render();
			break;
		case SPRITE_POWERUP:
		case SPRITE_INVERTEDPOWERUP:
			if (!powerUp) sprites[i]->render();
			break;
		default:
			sprites[i]->render();
		}
	}
	player->render();
	inverted_player->render();
	if(god_mode) godModeSprite->render();
	if (levelChangeTimer!=250) loadingSprite->render();
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
		case SPRITE_BARRIL:
			return FILE_SPRITE_BARRIL;
		case SPRITE_CAIXA:
			return FILE_SPRITE_CAIXA;
		default:
			return "images/error.png";
	}
}
