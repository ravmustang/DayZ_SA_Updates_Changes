class TendencyBlood extends DisplayElement
{

	void TendencyBlood()
	{
		m_NumOfBits = 5;
		m_Type	=	eDisplayElements.DELM_TDCY_BLOOD;
		m_Key = NTFKEY_BLEEDISH;
	}
	
	override void UpdateHUD()
	{
		UpdateTendencyHUD();
	}
}