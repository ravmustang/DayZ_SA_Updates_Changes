class ActionTurnOnChemlight: ActionTurnOnWhileInHands
{
	void ActionTurnOnChemlight()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LITCHEMLIGHT;
	}

	override string GetText()
	{
		return "#activate";
	}
};