#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

using namespace std;
#define SOUND_LEVER 0
#define SOUND_POTION 1
#define SOUND_VICTORY 2
#define SOUND_DEATH 3
#define SOUND_JUMP 4
#define SOUND_RUN 5
#define SOUND_IMPACT 6
#define SOUND_GRUNT 7

#define SOUND_LEVER_FILE "sounds/lever.wav"
#define SOUND_POTION_FILE "sounds/potion.wav"
#define SOUND_VICTORY_FILE "sounds/victory.wav"
#define SOUND_DEATH_FILE "sounds/death.wav"
#define SOUND_JUMP_FILE "sounds/jump.wav"
#define SOUND_RUN_FILE "sounds/run.wav"
#define SOUND_IMPACT_FILE "sounds/impact.wav"
#define SOUND_GRUNT_FILE "sounds/grunt.wav"


#define MUSICAAMBIENTE "sounds/musicaAmbiente.wav"
class Sound
{
public:
	void update(int deltaTime);
	Sound();
	~Sound();

	void playShortSound(int type);

	
	void playMusic();
	void stopMusic();

private:
	void playAudioFile(string file, bool loop);
	vector<int> noRepeatSound;
	ISoundEngine* engine;
};


