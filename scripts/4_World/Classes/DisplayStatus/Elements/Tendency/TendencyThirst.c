class TendencyThirst extends DisplayElement
{
	void TendencyThirst()
	{
		m_NumOfBits = 5;
		m_Type	=	eDisplayElements.DELM_TDCY_WATER;
		m_Key = NTFKEY_THIRSTY;
	}
	
	override void UpdateHUD()
	{
		UpdateTendencyHUD();
	}
}