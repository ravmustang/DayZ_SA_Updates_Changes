class BleedingSourcesManagerServer extends BleedingSourcesManagerBase
{
	const float TICK_INTERVAL_SEC = 3;
	float 	m_Tick;
	bool	m_DisableBloodLoss = false;
	
	protected BleedingSourceZone GetBleedingSourceZone(int bit)
	{
		return m_BleedingSourceZone.Get(GetSelectionNameFromBit(bit));
	}
	
	void RemoveAnyBleedingSource()
	{
		int bleeding_sources_bits = m_Player.GetBleedingBits();
		int rightmost_bit = bleeding_sources_bits & (-bleeding_sources_bits);
		
		RemoveBleedingSource(rightmost_bit);
	}
	
	void OnTick(float delta_time)
	{
		m_Tick += delta_time;
		if( m_Tick > TICK_INTERVAL_SEC )
		{
			for(int i = 0; i < m_BleedingSources.Count(); i++)
			{
				m_BleedingSources.GetElement(i).OnUpdateServer(delta_time, m_DisableBloodLoss);
			}
		}
	}
	
	void ActivateAllBS()
	{
		for(int i = 0; i < m_BleedingSourceZone.Count(); i++)
		{
			int bit = m_BleedingSourceZone.GetElement(i).GetBit();
			if( CanAddBleedingSource(bit) )
			{
				AddBleedingSource(bit);
			}
		}
	}
	
	//damage must be to "Blood" healthType
	void ProcessHit(float damage, int component, string zone, string ammo, vector modelPos)
	{
		float dmg_max = m_Player.GetMaxHealth(zone, "Blood");
		float dmg = damage;
		float bleed_threshold = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " DamageApplied " + "bleedThreshold" );
		bleed_threshold = Math.Clamp(bleed_threshold,0,1);
		//Print("dmg_max = " + dmg_max);
		//Print("dmg = " + dmg);
		//Print("bleed_threshold = " + bleed_threshold);
		if ( dmg > (dmg_max * (1 - bleed_threshold)) )
		{
			AttemptAddBleedingSource(component);
			//Print("BLEEDING");
		}
	}
	
	void DebugActivateBleedingSource(int source)
	{
		RemoveAllSources();
		
		if(source >= m_BleedingSourceZone.Count() || !m_BleedingSourceZone.GetElement(source)) return;
		
		int bit = m_BleedingSourceZone.GetElement(source).GetBit();
		
		if( bit && CanAddBleedingSource(bit) )
		{
			AddBleedingSource(bit);
		}
	}
	
	void SetBloodLoss(bool status)
	{
		m_DisableBloodLoss = status;
	}
	
	void OnStoreSave( ParamsWriteContext ctx )
	{
		//int count = m_BleedingSources.Count();
		ctx.Write(m_Player.GetBleedingBits());
		
	}

	void OnStoreLoad( ParamsReadContext ctx )
	{
		int active_bits;
		ctx.Read(active_bits);
	
		int bit_offset = 0;
		for(int i = 0; i < BIT_INT_SIZE; i++)
		{
			int bit = 1 << bit_offset;
			if( (bit & active_bits) != 0 && CanAddBleedingSource(bit))
			{
				AddBleedingSource(bit);
			}
			bit_offset++;
		}
	}
	
}