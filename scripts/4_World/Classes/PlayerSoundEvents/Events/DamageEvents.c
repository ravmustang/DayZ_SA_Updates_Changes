class DamageLightSoundEvent extends PlayerSoundEventBase
{
	void DamageLightSoundEvent()
	{
		m_Type = EPlayerSoundEventType.DAMAGE;
		m_ID = EPlayerSoundEventID.TAKING_DMG_LIGHT;
		m_SoundSetNameRoot = "takingDamageLight";
	}
	
	override bool CanPlay()
	{
		return true;
	}
}