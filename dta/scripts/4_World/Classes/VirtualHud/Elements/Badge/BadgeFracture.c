class BadgeFracture extends DisplayElementBadge 
{
	void BadgeFracture()
	{
		NUM_OF_BITS = 2;
		m_Key 	= 	NTFKEY_FRACTURE;
		m_Type	=	eDisplayElements.DELM_BADGE_FRACTURE;
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