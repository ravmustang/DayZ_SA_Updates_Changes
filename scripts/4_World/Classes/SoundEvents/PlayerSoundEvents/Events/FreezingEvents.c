class FreezingEventsBase extends PlayerSoundEventBase
{
	
	void FreezingEventsBase()
	{
		m_HasPriorityOverTypes = EPlayerSoundEventType.DUMMY | EPlayerSoundEventType.INJURY | EPlayerSoundEventType.STAMINA;//-1 for all
		m_Type = EPlayerSoundEventType.FREEZING;
	}
	
	override bool CanPlay()
	{
		return true;
	}
	
	override bool HasPriorityOverCurrent(PlayerBase player, EPlayerSoundEventID other_state_id,EPlayerSoundEventType type_other )
	{
		return true;
	}
}

class FreezingSoundEvent extends FreezingEventsBase
{
	void FreezingSoundEvent()
	{
		
		m_ID = EPlayerSoundEventID.FREEZING;
		m_SoundVoiceAnimEventClassID = 11;
	}
}