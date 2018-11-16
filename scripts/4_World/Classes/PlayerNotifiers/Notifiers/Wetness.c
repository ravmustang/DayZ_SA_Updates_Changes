class Wetness: NotifierBase
{
	void Wetness(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages 	= 3;
		m_MaxPauseBetweenMessages 	= 5;
		m_MinPauseBetweenSounds 	= 10;
		m_MaxPauseBetweenSounds 	= 12;
		m_MinPauseBetweenAnimations	= 10;
		m_MaxPauseBetweenAnimations	= 12;
		m_Active = false;
	}

	override int GetNotifierType()
	{
		return NTF_WETNESS;
	}

	override void DisplayBadge()
	{
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_WET, true);
	}

	override void HideBadge()
	{
		GetDisplayStatus().SetStatus(eDisplayElements.DELM_BADGE_WET, false);
	}

	override void DisplayMessage()
	{
	}

	override void PlayAnimation()
	{
		//PrintString(GetName() + " animation");
	}	

	override void PlaySound()
	{
		//PrintString(GetName() + " sound");
	}
};