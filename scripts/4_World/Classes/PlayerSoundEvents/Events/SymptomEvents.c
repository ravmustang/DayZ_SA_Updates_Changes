class SymptomSoundEventBase extends PlayerSoundEventBase
{
	bool ThisHasPriority(EPlayerSoundEventID other_state_id)
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
		m_SoundSetNameRoot = "cough";
	}
}

class LaugherSoundEvent extends SymptomSoundEventBase
{
	void LaugherSoundEvent()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
		m_ID = EPlayerSoundEventID.SYMPTOM_LAUGHTER;
		m_SoundSetNameRoot = "laugh";
	}
}

class SneezeSoundEvent extends SymptomSoundEventBase
{
	void SneezeSoundEvent()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
		m_ID = EPlayerSoundEventID.SYMPTOM_SNEEZE;
		m_SoundSetNameRoot = "sneez";
	}
}