class ActionDrinkCan: ActionDrink
{
	void ActionDrinkCan()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINK;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_DRINK;
	}
	
	override string GetText()
	{
		return "#drink";
	}
};
