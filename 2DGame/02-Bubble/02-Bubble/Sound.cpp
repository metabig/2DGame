#include "Sound.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void Sound::update(int deltaTime)
{
	for (size_t i = 0; i < noRepeatSound.size(); i++)
	{
		noRepeatSound[i] -= deltaTime;
	}
}

Sound::Sound()
{
	noRepeatSound = { 0,0,0,0,0,0,0,0 };
	engine = createIrrKlangDevice();
	if (!engine) {
		cout << " error starting up the engine" << endl;
		exit(-1);
	}
}


Sound::~Sound()
{
}

void Sound::playMusic()
{
	playAudioFile(MUSICAAMBIENTE, true);
}

void Sound::stopMusic()
{
}

void Sound::playShortSound(int type)
{
	if (noRepeatSound[type] > 0) return;
	else noRepeatSound[type] = 1000;
	std::string filemusic;
	switch (type)
	{
	case SOUND_LEVER:
		filemusic = SOUND_LEVER_FILE; break;
	case SOUND_POTION:
		filemusic = SOUND_POTION_FILE; break;
	case SOUND_VICTORY:
		filemusic = SOUND_VICTORY_FILE; break;
	case SOUND_DEATH:
		filemusic = SOUND_DEATH_FILE; break;
	case SOUND_JUMP:
		filemusic = SOUND_JUMP_FILE; break;
	case SOUND_RUN:
		noRepeatSound[type] = 1800;
		filemusic = SOUND_RUN_FILE; break;
	case SOUND_IMPACT:
		filemusic = SOUND_IMPACT_FILE; break;
	case SOUND_GRUNT:
		filemusic = SOUND_GRUNT_FILE; break;
	default:
		return;
	}
	std::cout << "playing: " << filemusic << endl;
	playAudioFile(filemusic, false);
	
}

void Sound::playAudioFile(string file, bool loop)
{
	const char * c = file.c_str();
	engine->play2D(c, loop);
}

