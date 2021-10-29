#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enviorment.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define SCREEN_X 30
#define SCREEN_Y 14


#define INVERTED_PLAYER true
#define UPRIGHT_PLAYER false

class Scene
{

public:
	Scene();
	~Scene();

	void initSpecial();
	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void initSprites();

	

private:
	TileMap *map;
	Player *player;
	Player *inverted_player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Sprite* s;

	Texture menu;

	vector<Enviorment *> vecSprites;

};


#endif // _SCENE_INCLUDE

