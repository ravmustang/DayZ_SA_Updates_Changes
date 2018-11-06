class ElementStance extends DisplayElementBase 
{
	void ElementStance()
	{
		NUM_OF_BITS = 2;
		m_Type		=	eDisplayElements.DELM_STANCE;
		m_IsClientOnly = true;
	}
	
	override void UpdateHUD()
	{
		m_ModulePlayerStatus.SetStance( m_Value );
	}
}