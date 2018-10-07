class ActionOpen: ActionSingleUseBase
{
	void ActionOpen()
	{
		m_MessageSuccess = "I have opened it.";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENFOOD;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_OPENFOOD;
		//m_Animation = "open";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_OPEN;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "#open";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item && !item.IsOpen() )
		{ 
			return true;
		}
		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{
		return true;
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		ClearInventoryReservation(action_data);
	}

	override void OnExecuteServer( ActionData action_data )
	{
		if( !GetGame().IsMultiplayer() )
			ClearInventoryReservation(action_data);
		
		action_data.m_MainItem.Open();
	}
};