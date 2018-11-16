enum EPlayerSoundEventType
{
	GENERAL,
	STAMINA,
	DAMAGE,
}

class PlayerSoundEventBase
{
	EPlayerSoundEventType m_Type = EPlayerSoundEventType.GENERAL;
	EPlayerSoundEventID m_ID;
	PlayerBase m_Player;
	AbstractWave m_SoundSetCallback;
	bool m_RequestDestroy;
	string m_SoundSetNameRoot;
	
	
	void ~PlayerSoundEventBase()
	{
		if(m_SoundSetCallback) m_SoundSetCallback.Stop();
	}
	
	
	void Tick()
	{
		if(!m_SoundSetCallback)
		{
			m_RequestDestroy = true;
		}
	}
	
	bool ThisHasPriority(EPlayerSoundEventID other_state_id)
	{
		return true;
	}
	
	bool IsDestroyRequested()
	{
		return m_RequestDestroy;
	}
	
	bool CanPlay()
	{
		if( m_Player.IsHoldingBreath() ) 
		{
			return false;
		}
		return true;
	}
	
	void Init(PlayerBase player)
	{
		m_Player = player;
	}
	
	EPlayerSoundEventID GetPlayerSoundEventID()
	{
		return m_ID;
	}
	
	EPlayerSoundEventType GetPlayerSoundEventType()
	{
		return m_Type;
	}
	
	void Play()
	{
		if( m_Player )
		{
			string soundset_name;
			string gender;
			
			if( m_Player.IsMale() )
			{
				gender = "male";
			}
			else
			{
				gender = "female";
			}
			
			soundset_name = m_SoundSetNameRoot + "_" + gender + "_Char_SoundSet";
			m_SoundSetCallback = m_Player.SaySoundSet(soundset_name);
		}
	}
	
	void Stop()
	{
		if(m_SoundSetCallback) 
		{
			m_SoundSetCallback.Stop();
		}
	}
}