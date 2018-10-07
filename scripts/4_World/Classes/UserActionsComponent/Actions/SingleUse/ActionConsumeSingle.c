class ActionConsumeSingleCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CASingleUseQuantityEdible(UAQuantityConsumed.DEFAULT);
	}
};

class ActionConsumeSingle: ActionSingleUseBase
{
	void ActionConsumeSingle()
	{
		m_CallbackClass = ActionConsumeSingleCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_LICKBATTERY;
		m_MessageSuccess = "I have taken one.";
		m_MessageStart = "I started taking one.";
		m_MessageStartFail = "Its' empty";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINotEmpty;
		m_ConditionTarget = new CCTSelf;
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_CONSUME_SINGLE;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "#take_one";
	}
	
	override void OnCompleteServer( ActionData action_data )
	{	
		if ( action_data.m_MainItem && action_data.m_MainItem.GetQuantity() <= 0 )
		{
			action_data.m_MainItem.SetQuantity(0);
		}
	}
};