#pragma once

#include "Module.h"
#include "SDL2/SDL_mixer.h"
#include <list>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake();

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	bool LoadParameters(xml_node parameters);

	// Load a WAV in memory
	int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(int fx, int repeat = 0);

	void SetFxVolume(int id, int volume);   
	void SetMasterVolume(int volume);

private:
	xml_node configParameters;
	std::string sfxPath;
	std::string musicPath;

	_Mix_Music* music;
	std::list<Mix_Chunk*> fx;
	int volume = 128;
};
