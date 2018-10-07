class ActionTurnOnWhileInHands: ActionSingleUseBase
{
	void ActionTurnOnWhileInHands()
	{
		m_MessageSuccess = "It's turned on.";
		m_MessageFail = "It's out of energy";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_TURN_ON_WHILE_IN_HANDS;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "#switch_on";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.IsInherited(Roadflare) )
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LIGHTFLARE;
		}
		else if (item.IsInherited(Chemlight_ColorBase))
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LITCHEMLIGHT;
		}
		else
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
		}
		
		
		if ( item.HasEnergyManager()  &&  item.GetCompEM().CanSwitchOn()  &&  item.GetCompEM().CanWork() )
		{
			return true;
		}
		
		return false;
	}

	override void OnExecuteServer( ActionData action_data )  //Takhle to nedelat, vyrob si action componentu a tehle check patri do jeji Execute metody. Message o selhani pak napis jako messageStartFail
	{
		if ( action_data.m_MainItem.GetCompEM().CanWork() )
		{
			action_data.m_MainItem.GetCompEM().SwitchOn();
		} 
		else
		{
			InformPlayers(action_data.m_Player,action_data.m_Target,UA_FAILED);
		}
	}
};
