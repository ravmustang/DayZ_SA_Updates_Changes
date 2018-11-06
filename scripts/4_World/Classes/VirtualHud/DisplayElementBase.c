class DisplayElementBase extends MessageReceiverBase
{
	int		m_Value;
	int 	m_Key = 0;
	int		m_Type = 0;
	int 	m_Range = 0;
	int		m_Colors[7];
	string 	m_Labels[7];
	int		m_Pos = 1;
	int		NUM_OF_BITS;
	bool 	m_IsClientOnly = false;
	
	PluginPlayerStatus 	m_ModulePlayerStatus;

	void DisplayElementBase()
	{
		m_System = "VirtualHud";
		m_ModulePlayerStatus = PluginPlayerStatus.Cast(GetPlugin(PluginPlayerStatus));
		//SetMaxValue();
	}
	
	//for elements which get their value client side and can be updated much more quickly
	bool IsClientOnly()
	{
		return m_IsClientOnly;
	}
		
	void UpdateHUD();
	
	int GetNumberOfBits()
	{
		return NUM_OF_BITS;
	}
	
	void SetValue(int value, int range = 0)
	{
		m_Value = value;
		if (range != 0 )
		{
			m_Range = range;
		}
	}

	int GetValue()
	{
		return m_Value;
	}

	int BitToDec(int mask, int index, int length)
	{
		int value = mask & (GetCompareMask() << index);
		value = value >> index;
		return value;
	}

	int GetCompareMask()
	{
		int mask = Math.Pow(2, GetNumberOfBits() ) - 1;
		return mask;
	}
	
	int GetType()
	{
		return m_Type;
	}
	
	int TranslateLevelToStatus(int level)
	{
		if( level == DSLevels.WARNING ) return 2;
		if( level == DSLevels.CRITICAL ) return 3;
		if( level == DSLevels.BLINKING ) return 4;
		return 1;
	}
}