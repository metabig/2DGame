#include "Enviorment.h"
#include <iostream>



enum EnviormentAnimation {
	NORMAL, ACTIVE
};

enum paramAnimation {
	FRAMES, RATIO_X, RATIO_Y, FRAMES_PER_SECOND
};

void Enviorment::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, int envtype)
{
	type = envtype;
	vector<float> paramAnim = getParametersAnimation();
	string file = getFileSprite();
	spritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(ENV_WIDTH, ENV_HEIGHT), glm::vec2(paramAnim[RATIO_X], paramAnim[RATIO_Y]), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	setAnimation(NORMAL, (int)paramAnim[FRAMES_PER_SECOND]);//PlayerAnims
	sprite->changeAnimation(0);
	position = tileMapPos;
	sprite->setPosition(glm::vec2(float(position.x), float(position.y)));
}

void Enviorment::action()
{
	switch (type) {
	case SPRITE_CACTUS:
		cout << "Action cactus" << endl; break;
	case SPRITE_BANDERA:
		cout << "Action bandera" << endl; break;
	case SPRITE_LEVER:
		cout << "Action lever" << endl; break;
	default:
		cout << "No Action" << endl; break;
	}

}

void Enviorment::render()
{
	sprite->render();
}

void Enviorment::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}



string Enviorment::getFileSprite()
{
	switch (type)
	{
	case SPRITE_CACTUS:
		return FILE_SPRITE_CACTUS;
	case SPRITE_INVERTEDCACTUS:
		return FILE_SPRITE_CACTUS_I;

	case SPRITE_LEVER:
		return FILE_SPRITE_LEVER;
	case SPRITE_INVERTEDLEVER:
		return FILE_SPRITE_LEVER_I;
	case SPRITE_BANDERA:
		return FILE_SPRITE_BANDERA;
	case SPRITE_INVERTEDBANDERA:
		return FILE_SPRITE_BANDERA_I;
	default:
		return "images/error.png";
	}
}

void Enviorment::setAnimation(int anim_id, int framesPerSecond)
{
	for (int i = 0; i < 1; i++) {
		sprite->setAnimationSpeed(anim_id, framesPerSecond);
		sprite->addKeyframe(anim_id, glm::vec2(i, anim_id));
	}
}

vector<float> Enviorment::getParametersAnimation()
{
	vector<float> param;
	//FRAMES, RATIO_X,RATIO_Y, FRAMES_PER_SECOND
	switch (type)
	{
	case SPRITE_CACTUS:
	case SPRITE_INVERTEDCACTUS:
		param.push_back(FRAME_CACTUS);
		param.push_back(1.0 / FRAME_CACTUS);
		param.push_back(1.0);
		param.push_back(FRAME_CACTUS); break;

	case SPRITE_LEVER:
	case SPRITE_INVERTEDLEVER:
		param.push_back(FRAME_LEVER);
		param.push_back(1.0 / FRAME_LEVER);
		param.push_back(1.0);
		param.push_back(FRAME_LEVER); break;

	case SPRITE_BANDERA:
	case SPRITE_INVERTEDBANDERA:
		param.push_back(FRAME_BANDERA);
		param.push_back(1.0 / FRAME_BANDERA);
		param.push_back(1.0);
		param.push_back(FRAME_BANDERA); break;

	default:
		param.push_back(1.0);
		param.push_back(1.0);
		param.push_back(1.0);
		param.push_back(1.0); break;
	}

	return param;
}