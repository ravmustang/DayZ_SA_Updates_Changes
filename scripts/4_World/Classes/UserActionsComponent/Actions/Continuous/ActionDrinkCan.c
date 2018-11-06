class ActionDrinkCan: ActionDrink
{
	void ActionDrinkCan()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINK;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_DRINK;
	}
	
	override int GetType()
	{
		return AT_DRINK_CAN;
	}
	
	override string GetText()
	{
		return "#drink_the_can";
	}
};
