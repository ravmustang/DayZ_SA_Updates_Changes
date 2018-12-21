enum RandomGeneratorSyncUsage
{
	RGSRecoil,
	RGSJam,
	RGSCount,
}

class RandomGeneratorSyncManager
{
	const int UsageCount = RandomGeneratorSyncUsage.RGSCount;
	
	float randomValuesArray[UsageCount];
	DayZPlayer m_player;
	
	void RandomGeneratorSyncManager(DayZPlayer player)
	{
		m_player = player;
	}

	void Update()
	{
		for(int i = 0; i < UsageCount; i++ )
		{
			randomValuesArray[i] = m_player.Random01();
		}
	}	
	
	
	float GetRandom01(int UsageIndex)
	{
		if(UsageIndex < UsageCount)
		{
			return randomValuesArray[UsageIndex];
		}
		return -1;
	}
	
	float GetRandomInRange(int UsageIndex, float min, float max)
	{
		return GetRandom01(UsageIndex)*(max - min) + min;
	}
}