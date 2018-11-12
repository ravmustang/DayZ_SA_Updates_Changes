class InfluenzaAgent extends AgentBase
{
	const float INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_HC = -0.2;
	const float INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_IMMUNITY = 0.5;//[0..1]

	override void Init()
	{
		m_Type 					= eAgents.INFLUENZA;
		m_Invasibility 			= 0.1;//increase the agent count by this number per second
		m_TransferabilityIn		= 1;//to the player
		m_TransferabilityOut	= 1;//from the player
		m_MaxCount 				= 1000;
		m_AutoinfectProbability = 50;//
		m_ImmunityResistance	= 0;//resistence to immune system
		m_TransferabilityAirOut = 1;
	}

	override bool CanAutoinfectPlayer(PlayerBase player)
	{
		if( player.GetStatHeatComfort().Get() < INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_HC && player.GetImmunity() < INFLUENZA_AGENT_AUTOINFECT_THRESHOLD_IMMUNITY )
		{
			return true;			
		}
		else
		{
			return false;	
		}
	}

}