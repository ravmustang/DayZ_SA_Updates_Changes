class ActionSwitchLights: ActionInteractBase
{
	void ActionSwitchLights()
	{
		m_MessageSuccess    = "";
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}	
	
	override int GetType()
	{
		return AT_VEH_SWITCH_LIGHTS;
	}

	override string GetText()
	{
		return "Switch lights";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		return true;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Print(action_data.m_Target);
		Car car;
		if ( Class.CastTo(car, action_data.m_Target.GetObject()) )
		{
			car.SwitchLights();
		}
	}
};