class ActionInjectMorphineSelf: ActionInjectSelf
{
	override int GetType()
	{
		return AT_INJECT_MORPHINE_S;
	}
		
	override void ApplyModifiers( ActionData action_data )
	{
		//player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};