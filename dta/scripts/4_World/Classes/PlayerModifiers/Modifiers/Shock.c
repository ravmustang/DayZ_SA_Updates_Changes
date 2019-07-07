class ShockMdfr: ModifierBase
{
	private const float	 	UNCONSCIOUS_LIMIT = 50;
	private const float 	SHOCK_INCREMENT_PER_SEC = 1;
	
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_SHOCK;
		m_TickIntervalInactive 	= 1;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}	

	override bool ActivateCondition(PlayerBase player)
	{
		return false;
		return ( player.GetHealth("","Shock") <= UNCONSCIOUS_LIMIT );
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		return false;
		return !ActivateCondition(player);
	}

	override void OnActivate(PlayerBase player)
	{
		if( player.GetNotifiersManager() )
		{
			player.GetNotifiersManager().ActivateByType(eNotifiers.NTF_HEARTBEAT);
		}
	}
	
	override void OnDeactivate(PlayerBase player)
	{
		if( player.GetNotifiersManager() )
		{
			player.GetNotifiersManager().DeactivateByType(eNotifiers.NTF_HEARTBEAT);
		}
	}
	
	override void OnTick(PlayerBase player, float deltaT)
	{	

	}
	
	override void OnReconnect(PlayerBase player)
	{

	}
};