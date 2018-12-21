class ActionDrinkDisinfectant: ActionDrink
{
	override int GetType()
	{
		return AT_DRINK_DISINFECTANT;
	}
		
	override void ApplyModifiers( ActionData action_data )
	{
		action_data.m_Player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_POISONING);
	}
};