class InfluenzaAgent extends AgentBase
{
	const float INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_HC = PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_WARNING;
	const float INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_IMMUNITY = 0.5;//[0..1]

	override void Init()
	{
		m_Type 					= eAgents.INFLUENZA;
		m_Invasibility 			= 1;//increase the agent count by this number per second if potent enough to grow
		m_TransferabilityIn		= 1;//to the player
		m_TransferabilityOut	= 1;//from the player
		m_MaxCount 				= 1000;
		m_AntibioticsResistance = 0;//[0..1], 0 means antibiotics have full effect, 1 means no effect
		m_AutoinfectProbability = 50;//
		m_TransferabilityAirOut = 1;
		m_Potency = EStatLevels.LOW;//grow when player's immune system is at this level or lower
		m_DieOffSpeed = 1;//how fast the agent dies off when not potent enough to grow(per sec)
	}

	override bool CanAutoinfectPlayer(PlayerBase player)
	{
		if( player.GetStatHeatComfort().Get() < INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_HC )
		{
			return true;			
		}
		else
		{
			return false;	
		}
	}

}