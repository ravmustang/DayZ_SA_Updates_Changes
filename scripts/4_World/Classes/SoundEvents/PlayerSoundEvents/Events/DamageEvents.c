class DamageSoundEvents extends PlayerSoundEventBase
{
	override bool CanPlay()
	{
		return true;
	}
	
	override bool IsCurrentHasPriority(PlayerBase player, EPlayerSoundEventID other_state_id,EPlayerSoundEventType type )
	{
		if(type != EPlayerSoundEventType.DAMAGE)
		{
			return true;
		}
		return false;
	}
}

class DamageLightSoundEvent extends DamageSoundEvents
{
	void DamageLightSoundEvent()
	{
		m_Type = EPlayerSoundEventType.DAMAGE;
		m_ID = EPlayerSoundEventID.TAKING_DMG_LIGHT;
		m_SoundVoiceAnimEventClassID = 12;
	}
}

class DamageHeavySoundEvent extends DamageSoundEvents
{
	void DamageLightSoundEvent()
	{
		m_Type = EPlayerSoundEventType.DAMAGE;
		m_ID = EPlayerSoundEventID.TAKING_DMG_HEAVY;
		m_SoundVoiceAnimEventClassID = 13;
	}
}

