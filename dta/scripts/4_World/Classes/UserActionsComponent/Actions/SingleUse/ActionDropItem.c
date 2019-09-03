class ActionDropItemCB : ActionSingleUseBaseCB
{
	override void EndActionComponent()
	{
		SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
		m_ActionData.m_State = UA_FINISHED;
	}
};

class ActionDropItem: ActionSingleUseBase
{
	string m_ItemName = "";

	void ActionDropItem()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_DROPITEM_HANDS;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_DROPITEM_HANDS;
		m_CallbackClass 	= ActionDropItemCB;
		//m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#drop_item";
	}
	
	override typename GetInputType()
	{
		return DropActionInput;
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override bool CanBeUsedThrowing()
	{
		return true;
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
	
	override void OnExecuteServer( ActionData action_data )
	{
		if ( !GetGame().IsMultiplayer() )
		{
			action_data.m_Player.PredictiveDropEntity(action_data.m_Player.GetItemInHands());
		
			if ( action_data.m_Player.IsPlacingLocal() )
			{
				action_data.m_Player.TogglePlacingLocal();
			}
		}
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		/*if (action_data.m_Player.GetItemInHands().IsHeavyBehaviour())
		{*/
			action_data.m_Player.PredictiveDropEntity(action_data.m_Player.GetItemInHands());
		
			if ( action_data.m_Player.IsPlacingLocal() )
			{
				action_data.m_Player.TogglePlacingLocal();
			}
		/*}
		else
		{
			vector dir = action_data.m_Player.GetOrientation();
			HumanInventory hin = action_data.m_Player.GetHumanInventory();
			ItemBase item = action_data.m_Player.GetItemInHands();
			hin.ThrowEntity(item,dir,0);
		}*/
	}
};