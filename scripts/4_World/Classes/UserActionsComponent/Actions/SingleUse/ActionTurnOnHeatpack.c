class ActionTurnOnHeatpack : ActionTurnOnWhileInHands
{
	void ActionTurnOnHeatpack()
	{
		m_MessageSuccess = "I've activated the heatpack.";
		m_MessageFail = "It's ran out.";
		//m_Animation = "break";
	}

	override int GetType()
	{
	 	return AT_TURN_ON_HEATPACK;
	}

	override string GetText()
	{
		return "#activate";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.HasEnergyManager()  &&  item.GetCompEM().CanSwitchOn() )
		{
			return true;
		}
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		action_data.m_MainItem.GetCompEM().SwitchOn();
	}
};