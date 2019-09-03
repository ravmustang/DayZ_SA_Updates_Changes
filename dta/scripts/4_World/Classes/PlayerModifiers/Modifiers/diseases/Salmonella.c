class SalmonellaMdfr: ModifierBase
{
	static const int AGENT_THRESHOLD_ACTIVATE = 100;
	static const int AGENT_THRESHOLD_DEACTIVATE = 20;

	static const int CHANCE_OF_VOMIT = 20;
	static const int WATER_DRAIN_FROM_VOMIT = 450;
	static const int ENERGY_DRAIN_FROM_VOMIT = 310;

	override void Init()
	{
		m_TrackActivatedTime	= false;
		m_ID 					= eModifiers.MDF_SALMONELLA;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	override string GetDebugText()
	{
		return ("Activate threshold: "+AGENT_THRESHOLD_ACTIVATE + "| " +"Deativate threshold: "+AGENT_THRESHOLD_DEACTIVATE);
	}
	
	override protected bool ActivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(eAgents.SALMONELLA) >= AGENT_THRESHOLD_ACTIVATE) 
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
		if(player.GetSingleAgentCount(eAgents.SALMONELLA) <= AGENT_THRESHOLD_DEACTIVATE) 
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
		float stomach_volume = player.m_PlayerStomach.GetStomachVolume();
		if( stomach_volume > 200 )
		{
			int roll = Math.RandomInt(0, 100);
			if( roll < CHANCE_OF_VOMIT )
			{
				SymptomBase symptom = player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);
				
				if( symptom )
				{
					symptom.SetDuration(5);

					// figure something more clever
					if (m_Player.GetStatWater().Get() > (WATER_DRAIN_FROM_VOMIT))
						m_Player.GetStatWater().Add(-1 * WATER_DRAIN_FROM_VOMIT);
					if (m_Player.GetStatEnergy().Get() > (ENERGY_DRAIN_FROM_VOMIT))
						m_Player.GetStatEnergy().Add(-1 * ENERGY_DRAIN_FROM_VOMIT);
				}
			}
		}
	}
};