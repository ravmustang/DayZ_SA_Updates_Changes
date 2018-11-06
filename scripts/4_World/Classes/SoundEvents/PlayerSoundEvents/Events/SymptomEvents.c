class SymptomSoundEventBase extends PlayerSoundEventBase
{
	override bool IsCurrentHasPriority(PlayerBase player, EPlayerSoundEventID other_state_id, EPlayerSoundEventType type)
	{
		return true;
	}
	
	override bool CanPlay()
	{
		if( !super.CanPlay() )
		{
			return false;
		}
		return true;
	}
}

class CoughSoundEvent extends SymptomSoundEventBase
{
	void CoughSoundEvent()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
		m_ID = EPlayerSoundEventID.SYMPTOM_COUGH;
		m_SoundVoiceAnimEventClassID = 8;
	}
}

class LaugherSoundEvent extends SymptomSoundEventBase
{
	void LaugherSoundEvent()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
		m_ID = EPlayerSoundEventID.SYMPTOM_LAUGHTER;
		m_SoundVoiceAnimEventClassID = 10;
	}
}

class SneezeSoundEvent extends SymptomSoundEventBase
{
	void SneezeSoundEvent()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
		m_ID = EPlayerSoundEventID.SYMPTOM_SNEEZE;
		m_SoundVoiceAnimEventClassID = 9;
	}
}