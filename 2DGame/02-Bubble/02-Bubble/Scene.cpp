#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

Scene::Scene()
{
	background_sprite = NULL;
	map = NULL;
	player = NULL;
	inverted_player = NULL;
}

Scene::~Scene()
{
	if (background_sprite != NULL)
		delete background_sprite;
	if(map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (inverted_player != NULL)
		delete player;
}

void Scene::init()
{
	//Shaders
	initShaders();

	// Background
	background.loadFromFile("images/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background_sprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &background, &texProgram);
	
	// Map
	map = TileMap::createTileMap("levels/customlevel.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	
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

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	inverted_player->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background_sprite->render();
	map->render();
	player->render();
	inverted_player->render();
	for each (Enviorment* e in vecSprites)
	{
		e->render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::initSprites()
{
	vector<int> spriteInfo = map->getSpriteInfo();
	for (size_t i = 0; i < spriteInfo.size(); i=i+3)
	{
		int x = spriteInfo[i];
		int y = spriteInfo[i+1];
		int type = spriteInfo[i+2];

		Enviorment* env = new Enviorment();
		env->init(glm::ivec2(x* map->getTileSize(), y* map->getTileSize()), texProgram, type);
		vecSprites.push_back(env);
		cout << "Created Sprite at: " << x << " " << y << " " << type << endl;
	}

}





