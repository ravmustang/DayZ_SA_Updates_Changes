class ActionForceDrinkDisinfectant: ActionForceDrink
{
	override int GetType()
	{
		return AT_FORCE_DRINK_DISINFECTANT;
	}

	override void ApplyModifiers( ActionData action_data )
	{
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		ntarget.m_ModifiersManager.ActivateModifier(eModifiers.MDF_POISONING);
	}
};