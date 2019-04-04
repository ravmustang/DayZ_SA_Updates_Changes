class BadgeBleeding extends DisplayElementBadge 
{
	int m_BleedingSourceCountCurr;//current number of bleeding sources
	
	void BadgeBleeding(PlayerBase player)
	{
		NUM_OF_BITS = 1;
		m_Key 	= 	NTFKEY_BLEEDISH;
		m_Type	=	eDisplayElements.DELM_BADGE_BLEEDING;
		m_IsClientOnly = true;
	}
		
	override void UpdateHUD()
	{
		super.UpdateHUD();
		//m_Player GetBleedingSourceCount()
		//m_ModulePlayerStatus.SetBadge(m_Key, true);
	}
}