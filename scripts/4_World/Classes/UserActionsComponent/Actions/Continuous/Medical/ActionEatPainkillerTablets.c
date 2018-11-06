class ActionEatPainkillerTablets: ActionConsume
{
	void ActionEatPainkillerTablets()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EAT;
	}
	
	override int GetType()
	{
		return AT_EAT_PAINKILLER_TABLETS;
	}

	override void ApplyModifiers( ActionData action_data )
	{
		//action_data.m_Player.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_POISONING);
	}
};