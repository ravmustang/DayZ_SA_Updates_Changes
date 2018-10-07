class Wet: ModifierBase
{
	const float WET_THRESHOLD 	= 0.2;

	override void Init()
	{
		m_TrackActivatedTime 	= false;
		m_ID 					= eModifiers.MDF_WETNESS;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	override bool ActivateCondition(PlayerBase player)
	{
		if (player.GetStatWet().Get() > WET_THRESHOLD)
			return true;
		
		return false;
	}

	override void OnActivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) player.m_NotifiersManager.AttachByType(NTF_WETNESS);
		
	}

	override void OnReconnect(PlayerBase player)
	{
		this.OnActivate(player);
	}


	override void OnDeactivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) player.m_NotifiersManager.DetachByType(NTF_WETNESS);
	}


	override bool DeactivateCondition(PlayerBase player)
	{
		if (player.GetStatWet().Get() <= WET_THRESHOLD)
			return true;
		
		return false;
	}
};