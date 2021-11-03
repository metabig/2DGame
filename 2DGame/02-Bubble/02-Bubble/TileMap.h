#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE

#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

#define SPRITE_MAINPLAYER -1
#define SPRITE_INVERTEDPLAYER -2
#define SPRITE_CACTUS -3
#define SPRITE_INVERTEDCACTUS -4
#define SPRITE_LEVER -5
#define SPRITE_INVERTEDLEVER -6
#define SPRITE_BANDERA -7
#define SPRITE_INVERTEDBANDERA -8


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	


	int getStartingX(bool inverted);
	int getStartingY(bool inverted);

	vector<int> getSpriteInfo();
private:
	void setStartPosition(int X, int Y, bool inverted);
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

	void setSpritePosition(int X, int Y, int SpriteType);
	bool checkSpriteCollision(int x, int y) const;

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

	int action = 0;
	
	int startingX, startingY;
	int startingX_inv, startingY_inv;

	vector<int> spriteinfo;

};


#endif // _TILE_MAP_INCLUDE


