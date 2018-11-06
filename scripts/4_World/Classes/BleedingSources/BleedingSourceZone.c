class BleedingSourceZone
{
	string 		m_FireSelectionName;
	int			m_Bit;
	vector		m_EmitterOffset;
	vector		m_EmitterOrientation;
	string		m_Bone;
	
	void BleedingSourceZone(string selection_name, int bit, vector offset, vector orientation, string bone)
	{
		m_FireSelectionName = selection_name;
		m_Bit = bit;
		m_EmitterOffset = offset;
		m_EmitterOrientation = orientation;
		m_Bone = bone;
	}

	void ~BleedingSourceZone()
	{
	
	}
	
	string GetSelectionName()
	{
		return m_FireSelectionName;
	}
	
	int GetBit()
	{
		return m_Bit;
	}
	
	vector GetOrientation()
	{
		return m_EmitterOrientation;
	}
		
	vector GetOffset()
	{
		return m_EmitterOffset;
	}
	
	string GetBoneName()
	{
		return m_Bone;
	}
	
	
}