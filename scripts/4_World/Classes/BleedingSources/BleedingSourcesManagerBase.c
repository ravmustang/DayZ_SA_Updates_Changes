class BleedingSourcesManagerBase
{
	ref map<int, ref BleedingSource> m_BleedingSources = new map<int, ref BleedingSource>;
	ref map<string, ref BleedingSourceZone> m_BleedingSourceZone = new map<string, ref BleedingSourceZone>;
	
	PlayerBase m_Player;
	//ref map<string, int> m_FireGeomToBit = new map<string, int>;
	ref map<int, string> m_BitToFireGeom = new map<int, string>;
	int m_BitOffset = 0;
	
	void BleedingSourcesManagerBase( PlayerBase player )
	{
		m_Player = player;
		Init();
	}
	
	protected void Init()
	{
		RegisterBleedingZone("LeftArm", "", "0 90 90" , "0 -0.05 0");
		RegisterBleedingZone("LeftArmRoll", "", "0 90 0" , "0 -0.05 0");
		RegisterBleedingZone("LeftFoot", "", "0 180 0" , "0 0 0.035");
		RegisterBleedingZone("LeftForeArm", "", "0 90 0" , "0 -0.05 0");
		RegisterBleedingZone("LeftHand", "", "0 90 0" , "0 -0.05 0");
		RegisterBleedingZone("LeftLeg", "", "0 90 0" , "0 -0.07 0");
		RegisterBleedingZone("LeftLegRoll", "", "0 90 0" , "0 -0.07 0");
		RegisterBleedingZone("LeftShoulder", "", "0 -90 0" , "0 0.07 0");
		RegisterBleedingZone("LeftToeBase", "", "0 -90 0" , "0 0.07 0");
		RegisterBleedingZone("LeftUpLeg", "", "0 90 0" , "0 -0.12 0");
		RegisterBleedingZone("LeftUpLegRoll", "", "0 90 0" , "0 -0.07 0");
		
		RegisterBleedingZone("RightArm", "", "0 -90 0" , "0 0.05 0");
		RegisterBleedingZone("RightArmRoll", "", "0 -90 0" , "0 0.05 0");
		RegisterBleedingZone("RightFoot", "", "0 0 0" , "0 0 -0.035");
		RegisterBleedingZone("RightForeArm", "", "0 -90 0" , "0 0.05 0");
		RegisterBleedingZone("RightHand", "", "0 90 0" , "0 -0.05 0");
		RegisterBleedingZone("RightLeg", "", "0 -90 0" , "0 0.06 0");
		RegisterBleedingZone("RightLegRoll", "", "0 -90 0" , "0 0 0");
		RegisterBleedingZone("RightShoulder", "", "0 90 0" , "0 -0.07 0");
		RegisterBleedingZone("RightToeBase", "", "0 90 0" , "0 -0.07 0");
		RegisterBleedingZone("RightUpLeg", "", "0 -90 0" , "0 0.12 0");
		RegisterBleedingZone("RightUpLegRoll", "", "0 -90 0" , "0 0.06 0");
		
		RegisterBleedingZone("Spine", "", "0 55 0" , "0 -0.1 0");
		RegisterBleedingZone("Spine1", "", "0 155 0" , "0 -0.1 0");
		RegisterBleedingZone("Spine2", "", "0 55 0" , "0 -0.07 0");
		RegisterBleedingZone("Spine3", "", "0 155 0" , "0 -0.05 0");
		
		RegisterBleedingZone("Neck", "", "-180 0 0" , "0.02 -0.05 0.05");
		RegisterBleedingZone("Pelvis", "", "0 -135 0" , "0 0.12 0");
	}
	
	protected int GetBitFromSelectionID(int id)
	{
		CachedObjectsArrays.ARRAY_STRING.Clear();
		m_Player.GetActionComponentNameList(id, CachedObjectsArrays.ARRAY_STRING, "fire");
		
		for(int i = 0; i < CachedObjectsArrays.ARRAY_STRING.Count(); i++)
		{
			/*
			string name = CachedObjectsArrays.ARRAY_STRING.Get(i);
			PrintString(name);
			*/
			int bit = GetBitFromSelectionName(CachedObjectsArrays.ARRAY_STRING.Get(i));
			if(  bit !=0 )
			{
				return bit;
			}
		}
		return 0;
	}
	
	protected BleedingSourceZone GetBleedingSourceMeta(int bit)
	{
		return m_BleedingSourceZone.Get(GetSelectionNameFromBit(bit));
	}
	
	protected void RegisterBleedingZone(string name, string bone = "", vector orientation = "0 0 0", vector offset = "0 0 0")
	{
		if( m_BitOffset == BIT_INT_SIZE)
		{
			Error("Too many bleeding sources, max is "+BIT_INT_SIZE.ToString());
		}
		else
		{
			name.ToLower();
			//PrintString(name);
			//int bit = Math.Pow(2, m_BitOffset);
			int bit = 1 << m_BitOffset;
			//PrintString(bit.ToString());
			string bone_name = bone;
			
			if(bone_name == "")
			{
				bone_name = name;
			}
			
			m_BleedingSourceZone.Insert(name, new BleedingSourceZone(name, bit, offset, orientation, bone_name));
			m_BitToFireGeom.Insert(bit, name);
			m_BitOffset++;
		}
	}
	
	void RemoveAllSources()
	{
		//int count = m_BleedingSources.Count();
		while (m_BleedingSources.Count() > 0 )
		{
			int bit = m_BleedingSources.GetKey(0);
			RemoveBleedingSource(bit);
		}
	}
	
	protected int GetBitFromSelectionName(string name)
	{
		if(!m_BleedingSourceZone.Get(name))
		{
			return 0;
		}
		return m_BleedingSourceZone.Get(name).GetBit();
	}	
	
	protected string GetSelectionNameFromBit(int bit)
	{
		return m_BitToFireGeom.Get(bit);
	}
	
	bool AttemptAddBleedingSource(int component)
	{
		int bit = GetBitFromSelectionID(component);
		
		if( bit == 0 )
		{
			return false;
		}
		
		if( CanAddBleedingSource(bit) )
		{
			AddBleedingSource(bit);
		}
		
		return true;
	}
	
	protected bool CanAddBleedingSource(int bit)
	{
		if(!GetBleedingSourceMeta(bit)) return false;
		return ((m_Player.GetBleedingBits() & bit) == 0 );
	}
	
	protected void AddBleedingSource(int bit)
	{
		m_Player.SetBleedingBits(m_Player.GetBleedingBits() | bit );
		vector orientation = GetBleedingSourceMeta(bit).GetOrientation();
		vector offset = GetBleedingSourceMeta(bit).GetOffset();
		string bone_name =  GetBleedingSourceMeta(bit).GetBoneName();
		m_BleedingSources.Insert(bit, new BleedingSource(m_Player, bit,bone_name, orientation, offset) );
	}

	protected void RemoveBleedingSource(int bit)
	{
		int inverse_bit_mask = ~bit;
		//m_Player.GetBleedingBits();
		m_Player.SetBleedingBits(m_Player.GetBleedingBits() & inverse_bit_mask );
		m_BleedingSources.Remove(bit);
	}

	int GetBleedingSourcesCount()
	{
		return m_BleedingSources.Count();
	}
}