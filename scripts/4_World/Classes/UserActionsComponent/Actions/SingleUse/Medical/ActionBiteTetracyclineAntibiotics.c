class ActionBiteTetracyclineAntibiotics: ActionConsumeSingle
{
	void ActionBiteTetracyclineAntibiotics()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS;
	}

	override int GetType()
	{
		return AT_BITE_TETRACYCLINE_ANTIBIOTICS;
	}

	override void ApplyModifiers( ActionData action_data )
	{
		//player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};