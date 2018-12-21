class ConstructionPart
{
	string 	m_Name;
	int		m_Id;
	string 	m_PartName;
	string 	m_MainPartName;
	bool 	m_IsBuilt;
	bool 	m_IsBase;
	bool 	m_IsGate;
	
	void ConstructionPart( string name, string part_name, string main_part_name, int part_id, bool is_built, bool is_base, bool is_gate )
	{
		m_Name = name;
		m_PartName = part_name;
		m_MainPartName = main_part_name;
		m_Id = part_id;
		m_IsBuilt = is_built;
		m_IsBase = is_base;
		m_IsGate = is_gate;
	}
	
	string GetName()
	{
		return m_Name;
	}
	
	string GetPartName()
	{
		return m_PartName;
	}

	string GetMainPartName()
	{
		return m_MainPartName;
	}
	
	int GetId()
	{
		return m_Id;
	}
	
	bool IsBuilt()
	{
		return m_IsBuilt;
	}
	
	void SetBuiltState( bool is_built )
	{
		m_IsBuilt = is_built;
	}
	
	bool IsBase()
	{
		return m_IsBase;
	}
	
	bool IsGate()
	{
		return m_IsGate;
	}
}
