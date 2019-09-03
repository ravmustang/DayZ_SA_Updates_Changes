class FoodPoisonAgent extends AgentBase
{
	override void Init()
	{
		m_Type 					= eAgents.FOOD_POISON;
		m_Invasibility 			= 1;//increase the agent count by this number per second if potent enough to grow
		m_TransferabilityIn		= 1;//to the player
		m_TransferabilityOut	= 0;//from the player
		m_AntibioticsResistance = 0.5;//[0..1], 0 means antibiotics have full effect, 1 means no effect
		m_MaxCount 				= 400;
		m_Potency = EStatLevels.CRITICAL;//grow when player's immune system is at this level or lower
		m_DieOffSpeed = 1;//how fast the agent dies off when not potent enough to grow(per sec)
		m_Digestibility = 1;
	}
}