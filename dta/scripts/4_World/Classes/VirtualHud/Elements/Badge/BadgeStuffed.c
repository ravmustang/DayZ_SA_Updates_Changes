class BadgeStuffed extends DisplayElementBadge 
{
	void BadgeStuffed()
	{
		NUM_OF_BITS = 2;
		m_Key 	= 	NTFKEY_STUFFED;
		m_Type	=	eDisplayElements.DELM_BADGE_STUFFED;
	}
	
	override void UpdateHUD()
	{
		if(m_Value == 1) 
		{
			m_ModulePlayerStatus.SetBadge(m_Key, true);
		}
		else
		{
			m_ModulePlayerStatus.SetBadge(m_Key, false);
		}
	}
}