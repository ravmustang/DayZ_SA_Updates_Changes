enum WaveKind 
{
	WAVEEFFECT,
	WAVEEFFECTEX,
	WAVESPEECH,
	WAVEMUSIC,
	WAVESPEECHEX,
	WAVEENVIRONMENT,
	WAVEENVIRONMENTEX,
	WAVEWEAPONS,
	WAVEWEAPONSEX,
	WAVEATTALWAYS,
	WAVEUI
}

class AbstractSoundScene
{
	proto native AbstractWave Play2D(SoundObject soundObject, SoundObjectBuilder soundBuilder);
	proto native AbstractWave Play3D(SoundObject soundObject, SoundObjectBuilder soundBuilder);
	proto native SoundObject BuildSoundObject(SoundObjectBuilder soundObjectbuilder);

	proto native float GetRadioVolume();
	proto native void SetRadioVolume(float vol, float time);

	proto native float GetSpeechExVolume();
	proto native void SetSpeechExVolume(float vol, float time);

	proto native float GetMusicVolume();
	proto native void SetMusicVolume(float vol, float time);

	proto native float GetSoundVolume();
	proto native void SetSoundVolume(float vol, float time);

	proto native float GetVOIPVolume();
	proto native void SetVOIPVolume(float vol, float time);
}


class SoundObjectBuilder
{
	void SoundObjectBuilder(SoundParams soundParams);	
	
	SoundObject BuildSoundObject()
	{
		return GetGame().GetSoundScene().BuildSoundObject(this);
	}

	proto native void Initialize(SoundParams soundParams);
	proto native void UpdateEnvSoundControllers(vector position);
	proto native void SetVariable(string name, float value);
}


class SoundObject
{
	void SoundObject(SoundParams soundParams);
	
	proto native void SetPosition(vector position);
	proto native void SetKind(WaveKind kind);
	proto native void Initialize(SoundParams soundParams);
}


//soundsys.hpp
class SoundParams
{
	void SoundParams(string name);
		
	proto native bool Load(string name);
	proto native bool IsValid();
}

class AbstractWave
{
	proto native void Play();
	void PlayWithOffset(float offset) { Play(); SetStartOffset(offset); }
	//proto native void Mute();
	proto native void Stop();
	proto native void Restart();
	proto native void SetStartOffset(float offset);
	proto native float GetLength();
	proto native void Repeat(int count);
	proto native void SetVolume(float value);
	//proto native float GetCurrPosition();
	
	void OnPlay()
	{
		//Print("[lukasikjak] AbstractWave::OnPlay");
	}
	
	void OnStop()
	{
		//Print("[lukasikjak] AbstractWave::OnStop");
	}
	
	void OnLoad()
	{
		//Print("[lukasikjak] AbstractWave::OnLoad");
	}
	
	void OnEnd()
	{
		//Print("[lukasikjak] AbstractWave::OnEnd");
	}
}