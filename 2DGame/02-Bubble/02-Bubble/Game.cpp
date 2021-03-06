#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	splashscene.init();
	currentLevel = 1;
	levelscene.init(currentLevel);
}

bool Game::update(int deltaTime)
{
	if (changeLevel) {
		if (currentLevel > maxLevel) currentLevel = 1;//Loop to first level
		levelscene.init(currentLevel);
		changeLevel = false;
	}
	if (isLevelscene) {
		changeLevel = levelscene.update(deltaTime);
		if (changeLevel) ++currentLevel;
		if (currentLevel > maxLevel) {
			isLevelscene = false;
			splashscene.setCreditsScreen();
		}
	}
	else {
		splashscene.update(deltaTime);
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (isLevelscene) {
		levelscene.render();
	}
	else {
		splashscene.render();
	}
}

void Game::keyPressed(int key)
{
	switch (key) {
	case 27:
		bPlay = false; break;
	case 112:
		isLevelscene = true; break;
	case 115:
		currentLevel++;
		changeLevel = true; break;

	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}




