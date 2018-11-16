class TendencyHunger extends DisplayElement
{
	void TendencyHunger()
	{
		m_NumOfBits = 5;
		m_Type	=	eDisplayElements.DELM_TDCY_ENERGY;
		m_Key = NTFKEY_HUNGRY;
	}
	
	override void UpdateHUD()
	{
		UpdateTendencyHUD();
	}
}