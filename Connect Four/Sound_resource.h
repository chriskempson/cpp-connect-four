/*
 * Connect Four
 * MIT Licence, Copyright 2017 Chris Kempson (chriskempson.com)
 */

#pragma once
#include <SDL_mixer.h>
#include "Resource.h"

/**
 * Resource interface implementation for handling sound files
 */
class Sound_resource : public Resource
{
public:
	Sound_resource();
	~Sound_resource();

	/**
	 * See Resource Class for documentation
	 */
	bool load_from_file(char* file);

	/**
	 * Plays the loaded sound file
	 */
	void play();

	/**
	 * Plays the loaded sound only once regardless of how many times this 
	 * function is called
	 *
	 * Once called the sound will not play again
	 */
	void play_once_only();

private:
	/// Pointer to the sound file stored as a Mix_Chunk
	Mix_Chunk* chunk;

	/// Path to the loaded sound file
	std::string path;

	/// Flag used by play_once_only() to mark a sound as having been played
	bool played_once_only = false;
};