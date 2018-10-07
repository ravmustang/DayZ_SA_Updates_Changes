class CTSaveStructure
{
	int		m_InterpType;
	int		m_InterpSpeed;
	float	m_BlurValue;
	
	ref array<ref Param5<vector, vector, float, float, float>> m_Cameras;
	ref array<ref Param5<vector, vector, float, string, string>> m_Events;
	
	void CTSaveStructure()
	{
		m_Cameras = new array<ref Param5<vector, vector, float, float, float>>;
		m_Events = new array<ref Param5<vector, vector, float, string, string>>;
	}
}