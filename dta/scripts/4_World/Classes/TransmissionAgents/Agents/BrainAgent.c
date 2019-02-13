class BrainAgent extends AgentBase
{
	override void Init()
	{
		m_Type 					= eAgents.BRAIN;
		m_Invasibility 			= 1;//increase the agent count by this number per second if potent enough to grow
		m_TransferabilityIn		= 1;//to the player
		m_TransferabilityOut	= 0;//from the player
		m_AntibioticsResistance = 1;//[0..1], 0 means antibiotics have full effect, 1 means no effect
		m_MaxCount 				= 5000;
		m_ImmunityResistance	= 1;//resistence to immune system
		m_Potency = EStatLevels.GREAT;//grow when player's immune system is at this level or lower
		m_DieOffSpeed = 1;//how fast the agent dies off when not potent enough to grow(per sec)
	}
}