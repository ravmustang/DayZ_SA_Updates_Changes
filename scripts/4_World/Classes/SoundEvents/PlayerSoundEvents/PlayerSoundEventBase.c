enum EPlayerSoundEventType
{
	GENERAL 	= 1,
	MELEE 		= 2,
	STAMINA 	= 4,
	DAMAGE 		= 8,
	DUMMY 		= 16,
	INJURY 		= 32,
}

class PlayerSoundEventBase extends SoundEventBase
{
	PlayerBase 	m_Player;
	float		m_DummySoundLength;
	float 		m_DummyStartTime;
	bool		m_IsDummyType;
	float 		m_PlayTime;
	EPlayerSoundEventType m_HasPriorityOverTypes;
	
	bool IsDummy()
	{
		return m_IsDummyType;
	}
	
	EPlayerSoundEventType GetPriorityOverTypes()
	{
		return m_HasPriorityOverTypes;
	}
	
	void PlayerSoundEventBase()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
	}
	
	void ~PlayerSoundEventBase()
	{
		if(m_SoundSetCallback) m_SoundSetCallback.Stop();
	}
	
	int GetSoundVoiceAnimEventClassID()
	{
		return m_SoundVoiceAnimEventClassID;
	}
	
	bool HasPriorityOverCurrent(PlayerBase player, EPlayerSoundEventID other_state_id, EPlayerSoundEventType type_other)
	{
		return true;
	}
	
	bool IsFinished()
	{
		if(IsDummy())
		{
			return IsDummyFinished();
		}
		else
		{
			return !IsSoundCallbackExist();
		}
	}
	
	bool IsDummyFinished()
	{
		return GetGame().GetTime() > (m_DummyStartTime + m_DummySoundLength);
	}
	
	
	void OnTick(float delta_time)
	{
		if(m_SoundSetCallback)
			m_SoundSetCallback.SetPosition(m_Player.GetPosition());
	}

	bool CanPlay(PlayerBase player)
	{
		if( player.IsHoldingBreath() ) 
		{
			return false;
		}
		return true;
	}
	
	void Init(PlayerBase player)
	{
		m_Player = player;
	}
	
	void OnEnd()
	{
		//PrintString("OnEnd - " + this.ToString());
	}
	
	void OnInterupt()
	{
		//PrintString("OnInterupt - " + this.ToString());
	}

	override void Play()
	{
		super.Play();
	
		if( !IsDummy() )
		{
			m_SoundSetCallback = m_Player.ProcessVoiceEvent("","", m_SoundVoiceAnimEventClassID);
			
			if(m_SoundSetCallback)
			{
				AbstractWaveEvents events = AbstractWaveEvents.Cast(m_SoundSetCallback.GetUserData());
				events.Event_OnSoundWaveEnded.Insert( OnEnd );
				events.Event_OnSoundWaveStopped.Insert( OnInterupt );
			}
		}
		else
		{
			m_DummyStartTime = GetGame().GetTime();
		}
	}
}