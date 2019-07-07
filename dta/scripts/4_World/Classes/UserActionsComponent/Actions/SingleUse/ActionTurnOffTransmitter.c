class ActionTurnOffTransmitterCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CASingleUse;
	}
}

class ActionTurnOffTransmitter: ActionTurnOffWhileInHands
{
	void ActionTurnOffTransmitter()
	{
		m_CallbackClass = ActionTurnOffTransmitterCB;
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;		
	}

	override string GetText()
	{
		return "#turn_off";
	}
	
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.IsTransmitter() )
		{
			if ( item.HasEnergyManager() && item.GetCompEM().IsWorking() )
			{
				return true;
			}
		}

		return false;
	}
		
	override void OnExecuteServer( ActionData action_data )
	{
		action_data.m_MainItem.GetCompEM().SwitchOff();
	}
}