#ifndef ___SOUND_SOURCE_H___
#define ___SOUND_SOURCE_H___

#include "visualobject.h"

#include "Framework.h"

class SoundSource :	public VisualObject
{
	public:

	SoundSource(string soundFileName, OpenGLApplicationBase * OpenGLApp = NULL);

	// Destructor
	virtual~SoundSource();
 
	virtual void initialize();

	virtual void update(float elapsedTimeSeconds);

	void play();

	void stop();

	void setLooping(bool loopingOn);

	void setRelative(bool input);

	void setVolume(float input);

	void setPitch(float input);

	void setSoundPosition(vec3 input);
	string soundFileName;
	ALuint      uiSource; 

	vec3 position;
	bool isPlaying;
	static bool soundInitialized;
};

#endif // ___SOUND_SOURCE_H___