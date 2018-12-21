class BadgePoisoned extends DisplayElementBadge 
{
	void BadgePoisoned()
	{
		NUM_OF_BITS = 2;
		m_Key 	= 	NTFKEY_POISONED;
		m_Type	=	eDisplayElements.DELM_BADGE_POISONED;
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