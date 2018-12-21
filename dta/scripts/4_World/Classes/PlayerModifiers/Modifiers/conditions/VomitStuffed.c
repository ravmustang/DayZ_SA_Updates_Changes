class VomitStuffedMdfr: ModifierBase
{
	protected const int	 	VOMIT_THRESHOLD 			= 2000;
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_VOMITSTUFFED;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	override void OnTick(PlayerBase player, float deltaT)
	{

	}
	
	override bool ActivateCondition(PlayerBase player)
	{
		float stomach = player.GetStatStomachVolume().Get();
		if ( stomach >= VOMIT_THRESHOLD )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override void OnActivate(PlayerBase player)
	{
		player.GetSymptomManager().QueueUpPrimarySymptom( SymptomIDs.SYMPTOM_VOMIT );
	}


	override bool DeactivateCondition(PlayerBase player)
	{
		return !ActivateCondition(player);
	}
};