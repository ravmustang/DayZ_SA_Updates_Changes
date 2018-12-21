class StaminaSoundEventBase extends PlayerSoundEventBase
{
	void StaminaSoundEventBase()
	{
		m_Type = EPlayerSoundEventType.STAMINA;
		m_HasPriorityOverTypes = EPlayerSoundEventType.STAMINA;
	}
	
	override bool HasPriorityOverCurrent(PlayerBase player, EPlayerSoundEventID other_state_id, EPlayerSoundEventType type_other)
	{
		return false;
	}
	
	override void OnPlay(PlayerBase player)
	{
		super.OnPlay(player);
		if( player.CanSpawnBreathVaporEffect() ) 
			player.SpawnBreathVaporEffect();
		
		
	}
}
class StaminaDownLight extends StaminaSoundEventBase
{
	void StaminaDownLight()
	{
		m_ID = EPlayerSoundEventID.STAMINA_DOWN_LIGHT;
		m_SoundVoiceAnimEventClassID = 1;
	}
}

class StaminaDownHeavy extends StaminaSoundEventBase
{
	void StaminaDownHeavy()
	{
		m_ID = EPlayerSoundEventID.STAMINA_DOWN_HEAVY;
		m_SoundVoiceAnimEventClassID = 2;
	}
}

class StaminaUpHeavy extends StaminaSoundEventBase
{
	void StaminaUpHeavy()
	{
		m_ID = EPlayerSoundEventID.STAMINA_UP_HEAVY;
		m_SoundVoiceAnimEventClassID = 3;
	}
}

class StaminaUpLight extends StaminaSoundEventBase
{
	void StaminaUpLight()
	{
		m_ID = EPlayerSoundEventID.STAMINA_UP_LIGHT;
		m_SoundVoiceAnimEventClassID = 4;
	}
}

class StaminaUpEnd extends StaminaSoundEventBase
{
	void StaminaUpEnd()
	{
		m_ID = EPlayerSoundEventID.STAMINA_UP_END;
		m_SoundVoiceAnimEventClassID = 26;
	}
	
	override bool HasPriorityOverCurrent(PlayerBase player, EPlayerSoundEventID other_state_id, EPlayerSoundEventType type_other)
	{
		return false;
	}
}

class StaminaNormalDummy extends StaminaSoundEventBase
{
	void StaminaNormalDummy()
	{
		m_HasPriorityOverTypes = 0;
		m_Type = EPlayerSoundEventType.DUMMY;
		m_ID = EPlayerSoundEventID.STAMINA_NORMAL_DUMMY;
		m_IsDummyType = true;
		m_DummySoundLength = 3000;//in ms
	}
}