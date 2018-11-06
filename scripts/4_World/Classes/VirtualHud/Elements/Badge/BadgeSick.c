class BadgeSick extends DisplayElementBadge 
{
	void BadgeSick()
	{
		NUM_OF_BITS = 2;
		m_Key 	= 	NTFKEY_SICK;
		m_Type	=	eDisplayElements.DELM_BADGE_SICK;
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