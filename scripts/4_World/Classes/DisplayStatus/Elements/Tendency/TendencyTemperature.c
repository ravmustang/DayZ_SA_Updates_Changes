class TendencyTemperature extends DisplayElement
{

	void TendencyTemperature()
	{
		m_NumOfBits = 5;
		m_Type	=	eDisplayElements.DELM_TDCY_TEMPERATURE;
		m_Key = NTFKEY_FEVERISH;
	}
	
	override void UpdateHUD()
	{
		UpdateTendencyHUD();
	}
}