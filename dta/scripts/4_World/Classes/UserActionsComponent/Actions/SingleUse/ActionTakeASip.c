class ActionTakeASip: ActionTakeABite
{
	void ActionTakeASip()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINK;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		return false;
	}

	override string GetText()
	{
		return "#take_a_sip";
	}
};