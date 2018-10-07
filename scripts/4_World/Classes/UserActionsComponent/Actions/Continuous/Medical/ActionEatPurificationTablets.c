class ActionEatPurificationTablets: ActionConsume
{
	void ActionEatPurificationTablets()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EATTABLETS;
	}
	
	override int GetType()
	{
		return AT_EAT_PURIFICATION_TABLETS;
	}

	override void ApplyModifiers( ActionData action_data )
	{
		//action_data.m_Player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};