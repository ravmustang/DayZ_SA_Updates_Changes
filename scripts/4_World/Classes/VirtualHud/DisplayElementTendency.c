class DisplayElementTendency extends DisplayElementBase
{
	protected int TENDENCY_MASK = 7;// 3 bits
	protected int SERIOUSNESS_BIT_MASK = 3;// 2 bits
	protected int SERIOUSNESS_BIT_OFFSET = 3;//3 bit offset
	
	void DisplayElementTendency()
	{
		NUM_OF_BITS = 5;
	}
	
	void SetSeriousnessLevel( DSLevels level )
	{
		m_Value = (~(SERIOUSNESS_BIT_MASK << SERIOUSNESS_BIT_OFFSET)) & m_Value;//clear the last value
		m_Value = (level << SERIOUSNESS_BIT_OFFSET)  | m_Value;
	}
	
	override void UpdateHUD()
	{
		int seriousness = m_Value >> SERIOUSNESS_BIT_OFFSET;
		int tendency = TENDENCY_MASK & m_Value;
		//PrintString(tendency.ToString());
		
		if(tendency > 3) 
		{
			tendency = -(tendency - 3);
		}
		
		//PrintString("ser:"+ seriousness.ToString());
		//PrintString("tnd:"+ tendency.ToString());
		//PrintString(this.ToString());
		m_ModulePlayerStatus.DisplayTendency(m_Key, tendency, TranslateLevelToStatus(seriousness));
	}
	
	void SetTendency(int tendency)
	{
		m_Value = (~TENDENCY_MASK) & m_Value;//clear the last value
		m_Value =  tendency | m_Value;//insert the new one
		//PrintString(m_Value.ToString());
	}
}