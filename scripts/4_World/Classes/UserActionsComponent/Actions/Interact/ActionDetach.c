class ActionDetach: ActionInteractBase
{
	string m_ItemName = "";

	void ActionDetach()
	{
		m_MessageSuccess    = "";
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_PICKUP_HANDS;
		m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}	
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_DETACH;
	}

	override string GetText()
	{
		return "#take_to_hands";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase tgt_item = ItemBase.Cast( target.GetObject() );
		EntityAI tgt_parent = EntityAI.Cast( target.GetParent() );
		EntityAI tgt_entity = EntityAI.Cast( target.GetObject() );

		if ( !tgt_item || !tgt_entity || !tgt_parent )
			return false;

		if ( !tgt_parent || !tgt_item.IsItemBase() || !tgt_item.IsTakeable() || tgt_item.IsBeingPlaced() )
			return false;

		if ( player.GetCommand_Vehicle() )
			return false;

		if ( player.GetInventory().CanAddEntityIntoHands(tgt_entity) && !player.GetInventory().CanAddEntityIntoInventory(tgt_entity) )
		{
			if ( tgt_entity.GetHierarchyRootPlayer() != player )
			{
				InventoryLocation inv = new InventoryLocation;
				if (tgt_entity.GetInventory().GetCurrentInventoryLocation(inv) )
				{
					int slot = inv.GetSlot();
				}

				if ( tgt_entity.CanDetachAttachment( tgt_parent ) && tgt_parent.CanReleaseAttachment( tgt_entity ) )
					return true;
			}		
		}

		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		if (!GetGame().IsMultiplayer())
		{
			ActionManagerClient am = ActionManagerClient.Cast(action_data.m_Player.GetActionManager());
			am.UnlockInventory(action_data);
		}
		
		EntityAI ntarget = EntityAI.Cast(action_data.m_Target.GetObject());
		action_data.m_Player.PredictiveTakeEntityToHands(ntarget);
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		ActionManagerClient am = ActionManagerClient.Cast(action_data.m_Player.GetActionManager());
		am.UnlockInventory(action_data);

		EntityAI ntarget = EntityAI.Cast(action_data.m_Target.GetObject());
		action_data.m_Player.PredictiveTakeEntityToHands(ntarget);
	}
};