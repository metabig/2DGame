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
		levelscene.init(currentLevel);
	}
	if (isLevelscene) {
		levelscene.update(deltaTime);
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





