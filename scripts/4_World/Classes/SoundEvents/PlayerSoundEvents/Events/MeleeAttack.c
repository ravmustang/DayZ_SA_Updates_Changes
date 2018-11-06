class MeleeAttackSoundEvents extends PlayerSoundEventBase
{
	override bool CanPlay()
	{
		return true;
	}
	
	override bool IsCurrentHasPriority(PlayerBase player, EPlayerSoundEventID other_state_id, EPlayerSoundEventType type)
	{
		if(type == EPlayerSoundEventType.DAMAGE)
		{
			return false;
		}
		return true;
	}
}


class MeleeAttackLightEvent extends MeleeAttackSoundEvents
{
	void MeleeAttackLightEvent()
	{
		m_Type = EPlayerSoundEventType.MELEE;
		m_ID = EPlayerSoundEventID.MELEE_ATTACK_LIGHT;
		m_SoundVoiceAnimEventClassID = 16;
	}
}

class MeleeAttackHeavyEvent extends MeleeAttackSoundEvents
{
	void MeleeAttackHeavyEvent()
	{
		m_Type = EPlayerSoundEventType.MELEE;
		m_ID = EPlayerSoundEventID.MELEE_ATTACK_HEAVY;
		m_SoundVoiceAnimEventClassID = 17;
	}
	
}

