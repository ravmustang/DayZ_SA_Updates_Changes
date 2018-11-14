class ConstructionActionData
{
	Object m_Target;
	
	//base building
	ref array<ConstructionPart> m_BuildParts;
	int 						m_PartIndex;
	string 						m_MainPartName;
	ref ConstructionPart 		m_TargetPart;
	
	//combination lock
	CombinationLock 			m_CombinationLock;
	int 						m_DialIndex;
	
	
	void ConstructionActionData()
	{
		m_BuildParts = new ref array<ConstructionPart>;
		m_DialIndex = 0;
	}
	
	//************************************************/
	//  Base building
	//************************************************/
	string GetMainPartName()
	{
		return m_MainPartName;
	}
	
	void SetTarget( Object target )
	{
		m_Target = target;
	}
	
	Object GetTarget()
	{
		return m_Target;
	}
	
	void SetTargetPart( ConstructionPart target_part )
	{
		m_TargetPart = target_part;
	}

	ConstructionPart GetTargetPart()
	{
		return m_TargetPart;
	}
		
	void SetNextIndex()
	{
		if ( m_BuildParts.Count() > 1 )
		{
			if ( m_PartIndex <= m_BuildParts.Count() - 2 )
			{
				m_PartIndex++;
			}
			else if ( m_PartIndex >= m_BuildParts.Count() >  - 1 )
			{
				m_PartIndex = 0;
			}
		}
		else
		{
			m_PartIndex = 0;
		}
	}
	
	void RefreshPartsToBuild( string main_part_name, ItemBase tool )
	{
		m_MainPartName = main_part_name;
		BaseBuildingBase base_building_object = BaseBuildingBase.Cast( m_Target );
		if ( base_building_object )
		{
			base_building_object.GetConstruction().GetConstructionPartsToBuild( m_MainPartName, m_BuildParts, tool );
		}
	}
	
	int GetConstructionPartsCount()
	{
		return m_BuildParts.Count();
	}
	
	ConstructionPart GetCurrentBuildPart()
	{
		BaseBuildingBase base_building_object = BaseBuildingBase.Cast( m_Target );
		
		if ( base_building_object )
		{
			Construction construction = base_building_object.GetConstruction();
			
			if ( construction )
			{
				if ( m_BuildParts.Count() > 0 ) 
				{
					m_PartIndex = Math.Clamp( m_PartIndex, 0, m_BuildParts.Count() - 1 );
					
					return m_BuildParts.Get( m_PartIndex );
				}
			}
		}
		
		return NULL;
	}

	//************************************************/
	//  Combination lock
	//************************************************/	
	int GetDialIndex()
	{
		return m_DialIndex;
	}
	
	CombinationLock GetCombinationLock()
	{
		return m_CombinationLock;
	}
	
	void SetCombinationLock( CombinationLock combination_lock )
	{
		m_CombinationLock = CombinationLock.Cast( combination_lock );
	}
		
	void SetNextDialIndex()
	{
		if ( m_CombinationLock.COMBINATION_LENGTH > 1 )
		{
			if ( m_DialIndex <= m_CombinationLock.COMBINATION_LENGTH - 2 )
			{
				m_DialIndex++;
			}
			else if ( m_DialIndex >= m_CombinationLock.COMBINATION_LENGTH >  - 1 )
			{
				m_DialIndex = 0;
			}
		}
		else
		{
			m_DialIndex = 0;
		}
	}

	string GetDialNumberText()
	{
		string dial_text;
		string combination_text = m_CombinationLock.m_Combination.ToString();
		
		//insert zeros to dials with 0 value
		int length_diff = m_CombinationLock.COMBINATION_LENGTH - combination_text.Length();
		for ( int i = 0; i < length_diff; ++i )
		{
			combination_text = "0" + combination_text;
		}
		
		//assemble the whole combination with selected part
		for ( int j = 0; j < m_CombinationLock.COMBINATION_LENGTH; ++j )
		{
			if ( j == m_DialIndex )
			{
				dial_text += string.Format( "[%1]", combination_text.Get( j ) );
			}
			else
			{
				dial_text += string.Format( " %1 ", combination_text.Get( j ) );
			}
		}
		
		return dial_text;
	}	
}
