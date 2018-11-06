class ConstructionActionData
{
	Object m_Target;
	
	ref array<ConstructionPart> m_BuildParts;
	int m_PartIndex;
	string m_MainPartName;
	
	ref ConstructionPart m_TargetPart;
	
	void ConstructionActionData()
	{
		m_BuildParts = new ref array<ConstructionPart>;
	}
	
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
	
	void RefreshPartsToBuild( string main_part_name )
	{
		m_MainPartName = main_part_name;
		BaseBuildingBase base_building_object = BaseBuildingBase.Cast( m_Target );
		if ( base_building_object )
		{
			base_building_object.GetConstruction().GetConstructionPartsToBuild( m_MainPartName, m_BuildParts );
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
}
