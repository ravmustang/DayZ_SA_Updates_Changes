class CharcoalMdfr: ModifierBase
{
	float m_Killrate;
	const int CHARCOAL_LIFETIME = 300;
	const int KILL_AGENTS_COUNT = 400;
	
	override void Init()
	{
		m_TrackActivatedTime = true;
		m_IsPersistent = true;
		m_ID 					= eModifiers.MDF_CHARCOAL;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= 1;
		m_Killrate = CalculateKillrate();
	}

	override bool ActivateCondition(PlayerBase player)
	{
		return false;
	}
	
	override void OnReconnect(PlayerBase player)
	{
		OnActivate(player);
	}
	
	override string GetDebugText()
	{
		return (CHARCOAL_LIFETIME - GetAttachedTime()).ToString();
	}

	
	override void OnActivate(PlayerBase player)
	{
		if( player.GetNotifiersManager() ) player.GetNotifiersManager().ActivateByType(eNotifiers.NTF_PILLS);
	}
	
	override void OnDeactivate(PlayerBase player)
	{
		if( player.GetNotifiersManager() ) player.GetNotifiersManager().DeactivateByType(eNotifiers.NTF_PILLS);
	}
	
	override bool DeactivateCondition(PlayerBase player)
	{
		float attached_time = GetAttachedTime();
		
		if( attached_time >= CHARCOAL_LIFETIME )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override void OnTick(PlayerBase player, float deltaT)
	{
		player.m_AgentPool.AddAgent(eAgents.SALMONELLA, -m_Killrate * deltaT);
	}
	
	float CalculateKillrate()
	{
		float active_time = KILL_AGENTS_COUNT / CHARCOAL_LIFETIME;
		return active_time;
	}
	
};