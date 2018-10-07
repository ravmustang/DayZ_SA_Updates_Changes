class ActionInjectEpinephrineSelf: ActionInjectSelf
{
	void ActionInjectEpinephrineSelf()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_MORPHINE;
	}
	
	override int GetType()
	{
		return AT_INJECT_EPINEPHRINE_S;
	}
		
	override void ApplyModifiers( ActionData action_data )
	{
		action_data.m_Player.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_HEART_ATTACK);
		action_data.m_Player.GiveShock(100);
	}
};