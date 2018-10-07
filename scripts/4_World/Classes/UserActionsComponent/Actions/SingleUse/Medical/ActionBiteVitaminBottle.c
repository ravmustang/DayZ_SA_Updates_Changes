class ActionBiteVitaminBottle: ActionConsumeSingle
{
	void ActionBiteVitaminBottle()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATPILLS;
	}

	override int GetType()
	{
		return AT_BITE_VITAMIN_BOTTLE;
	}

	override void ApplyModifiers( ActionData action_data )
	{
		//player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};