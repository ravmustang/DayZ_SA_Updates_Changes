class WoundInfectionMdfr: ModifierBase
{
	static const int AGENT_THRESHOLD_ACTIVATE = 100;
	static const int AGENT_THRESHOLD_DEACTIVATE = 20;
	
	override void Init()
	{
		m_TrackActivatedTime	= false;
		m_ID 					= eModifiers.MDF_WOUND_INFECTION;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	override string GetDebugText()
	{
		return ("Activate threshold: "+AGENT_THRESHOLD_ACTIVATE + "| " +"Deativate threshold: "+AGENT_THRESHOLD_DEACTIVATE);
	}
	
	override protected bool ActivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(eAgents.WOUND_AGENT) >= AGENT_THRESHOLD_ACTIVATE) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	override protected void OnActivate(PlayerBase player)
	{
		//if( player.m_NotifiersManager ) player.m_NotifiersManager.ActivateByType(eNotifiers.NTF_SICK);
		player.IncreaseDiseaseCount();
		
	}

	override protected void OnDeactivate(PlayerBase player)
	{
		player.DecreaseDiseaseCount();
	}

	override protected bool DeactivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(eAgents.WOUND_AGENT) <= AGENT_THRESHOLD_DEACTIVATE) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{
		
	}
};