class ActionDropItemCB : ActionSingleUseBaseCB
{
	override void EndActionComponent()
	{
		SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
		m_ActionData.m_State = UA_FINISHED;
	}
	
	override void OnAnimationEvent(int pEventID)
	{
		switch (pEventID)
		{
			case UA_ANIM_EVENT: 
				if (( ( !GetGame().IsServer() && GetGame().IsMultiplayer() ) || (!GetGame().IsMultiplayer()) ) && m_ActionData.m_Player)
				{
					m_ActionData.m_Player.PredictiveDropEntity(m_ActionData.m_Player.GetItemInHands());
				}
			break;
		}
	}
};

class ActionDropItem: ActionSingleUseBase
{
	string m_ItemName = "";

	void ActionDropItem()
	{
		m_MessageSuccess    = "";
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_INVENTORY;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_PICKUP_INVENTORY;
		m_CallbackClass 	= ActionDropItemCB;
		//m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}	
	
	override int GetType()
	{
		return AT_DROP_ITEM;
	}

	override string GetText()
	{
		return "#drop_item";
	}
	
	override bool HasProneException()
	{
		return false;
	}
	
	override bool HasTarget()
	{
		return false;
	}

	override bool ActionConditionContinue( ActionData action_data )
	{
		return true;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( player.GetCommand_Vehicle() )
			return false;
		
		if ( player && item )
		{
			return true;
		}
		return false;
	}
	
	/*override void OnExecuteServer( ActionData action_data )
	{
	}*/
	
	//would not work? Action probably already gone by then
	override void OnExecuteClient( ActionData action_data )
	{
		//action_data.m_Player.PredictiveDropEntity(action_data.m_Player.GetItemInHands());
	}
};