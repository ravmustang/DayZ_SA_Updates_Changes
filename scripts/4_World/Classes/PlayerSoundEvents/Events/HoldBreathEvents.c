class HoldBreathSoundEventBase extends PlayerSoundEventBase
{
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
		m_SoundSetNameRoot = "holdBreath";
	}
	
	override bool ThisHasPriority(EPlayerSoundEventID other_state_id)
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
		m_SoundSetNameRoot = "releaseBreath";
	}
	
	override bool ThisHasPriority(EPlayerSoundEventID other_state_id)
	{
		if( other_state_id == EPlayerSoundEventID.HOLD_BREATH)
		{
			return false;
		}
		return false;
	}
}