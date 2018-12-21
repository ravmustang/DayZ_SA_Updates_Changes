class ActionBitePainkillerTablets: ActionConsumeSingle
{
	void ActionBitePainkillerTablets()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY;
	}

	override int GetType()
	{
		return AT_BITE_PAINKILLER_TABLETS;
	}

	override void ApplyModifiers( ActionData action_data )
	{
		//player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};