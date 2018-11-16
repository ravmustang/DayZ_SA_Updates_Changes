class TendencyHealth extends DisplayElement
{
	void TendencyHealth()
	{
		m_NumOfBits = 5;
		m_Type	=	eDisplayElements.DELM_TDCY_HEALTH;
		m_Key = NTFKEY_SICK;
	}
	
	override void UpdateHUD()
	{
		UpdateTendencyHUD();
	}
}