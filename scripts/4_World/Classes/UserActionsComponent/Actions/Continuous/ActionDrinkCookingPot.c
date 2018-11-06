class ActionDrinkCookingPot: ActionDrink
{
	void ActionDrinkCookingPot()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINK;
	}

	override int GetType()
	{
		return AT_DRINK_COOKING_POT;
	}
};