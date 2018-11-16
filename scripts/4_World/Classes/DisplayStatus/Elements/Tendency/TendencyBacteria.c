class TendencyBacteria extends DisplayElement
{
	void TendencyBacteria()
	{
		m_NumOfBits = 5;
		m_Type = eDisplayElements.DELM_TDCY_BACTERIA;
		m_Key = NTFKEY_BACTERIA;
	}
	
	override void UpdateHUD()
	{
		UpdateTendencyHUD();
	}
}