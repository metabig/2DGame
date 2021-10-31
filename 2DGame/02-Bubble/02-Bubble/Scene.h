#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"



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

	void init();
	void update(int deltaTime);
	void render();

	ShaderProgram texProgram;
	void initShaders();
	void initSprites();
private:

	

private:


	float currentTime;
	glm::mat4 projection;




};


#endif // _SCENE_INCLUDE

