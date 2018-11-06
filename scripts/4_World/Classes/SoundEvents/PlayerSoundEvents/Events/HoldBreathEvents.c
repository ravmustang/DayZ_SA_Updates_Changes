class HoldBreathSoundEventBase extends PlayerSoundEventBase
{
	void HoldBreathSoundEventBase()
	{
		m_SkipForControlledCharacter = true;
	}
	
	override bool CanPlay()
	{
		return true;
	}
}


class HoldBreathSoundEvent extends HoldBreathSoundEventBase
{
	void HoldBreathSoundEvent()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
		m_ID = EPlayerSoundEventID.HOLD_BREATH;
		m_SoundVoiceAnimEventClassID = 20;
	}
	
	override bool IsCurrentHasPriority(PlayerBase player, EPlayerSoundEventID other_state_id, EPlayerSoundEventType type)
	{
		if( other_state_id == EPlayerSoundEventID.RELEASE_BREATH)
		{
			return false;
		}
		return false;
	}
}

class ReleaseBreathSoundEvent extends HoldBreathSoundEventBase
{
	void ReleaseBreathSoundEvent()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
		m_ID = EPlayerSoundEventID.RELEASE_BREATH;
		m_SoundVoiceAnimEventClassID = 21;
	}
	
	override bool IsCurrentHasPriority(PlayerBase player, EPlayerSoundEventID other_state_id, EPlayerSoundEventType type)
	{
		if( other_state_id == EPlayerSoundEventID.HOLD_BREATH)
		{
			return false;
		}
		return false;
	}
}