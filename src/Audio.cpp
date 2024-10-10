#include "Audio.h"
#include "Log.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

Audio::Audio() : Module()
{
	music = NULL;
	name = "audio";
}

// Destructor
Audio::~Audio()
{}

// Called before render is available
bool Audio::Awake()
{
	LOG("Loading Audio Mixer");
	bool ret = true;


	musicPath = configParameters.child("music").attribute("path").as_string();
	sfxPath = configParameters.child("sfx").attribute("path").as_string();
	volume = configParameters.child("volume").attribute("value").as_int();


	SDL_Init(0);


	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// Load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	SetMasterVolume(volume);

	return ret;
}

// Called before quitting
bool Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	for (const auto& fxItem : fx) {
		Mix_FreeChunk(fxItem);
	}
	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool Audio::PlayMusic(const char* path, float fadeTime)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fadeTime > 0.0f)
		{
			Mix_FadeOutMusic(int(fadeTime * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS((musicPath + path).c_str());

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", (musicPath + path).c_str(), Mix_GetError());
		ret = false;
	}
	else
	{
		if(fadeTime > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fadeTime * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", (musicPath + path).c_str(), Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", (musicPath + path).c_str(), Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", (musicPath + path).c_str());
	return ret;
}

bool Audio::LoadParameters(xml_node parameters)
{
	configParameters = parameters;
	return true;
}

// Load WAV
int Audio::LoadFx(const char* path)
{
	int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV((sfxPath + path).c_str());
	Mix_VolumeChunk(chunk, volume);
	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", (sfxPath + path).c_str(), Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = (int)fx.size();
	}

	return ret;
}

// Play WAV
bool Audio::PlayFx(int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.size())
	{
		auto fxIt = fx.begin();
		std::advance(fxIt, id-1);
		Mix_PlayChannel(-1, *fxIt, repeat);
	}

	return ret;
}

// Set the volume for a specific sound effect
void Audio::SetFxVolume(int id, int volume)
{
	if (!active) return;

	// Clamp the volume between 0 and 128
	if (volume < 0) volume = 0;
	if (volume > 128) volume = 128;

	if (id > 0 && id <= fx.size())
	{
		auto fxIt = fx.begin();
		std::advance(fxIt, id - 1);
		Mix_VolumeChunk(*fxIt, volume);
		LOG("FX volume for sound effect %d set to %d", id, volume);
	}
}

// Set the volume for all sound effects
void Audio::SetMasterVolume(int volume)
{
	if (!active) return;

	this->volume = volume;
	// Clamp the volume between 0 and 128
	if (volume < 0) volume = 0;
	if (volume > 128) volume = 128;

	for (auto& chunk : fx)
	{
		Mix_VolumeChunk(chunk, volume);
	}

	Mix_VolumeMusic(volume);

	LOG("Volumes set to %d", volume);
}