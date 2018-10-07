class ActionTakeItem: ActionInteractBase
{
	string m_ItemName = "";

	void ActionTakeItem()
	{
		m_MessageSuccess    = "";
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_INVENTORY;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_PICKUP_INVENTORY;
		m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}	
	
	override int GetType()
	{
		return AT_TAKE_ITEM;
	}

	override string GetText()
	{
		return "#take";
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override bool ActionConditionContinue( ActionData action_data )
	{
		return true;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase tgt_item = ItemBase.Cast( target.GetObject() );
		if ( tgt_item && !tgt_item.IsTakeable() ) return false;
		if ( tgt_item && tgt_item.IsBeingPlaced() ) return false;

		if ( player.GetCommand_Vehicle() )
			return false;
		
		EntityAI tgt_parent = EntityAI.Cast( target.GetParent() );
		EntityAI tgt_entity = EntityAI.Cast( target.GetObject() );
		
		if ( (tgt_entity && !tgt_entity.GetParent()) || ( tgt_parent && tgt_parent.GetInventory().CanRemoveAttachment( tgt_entity ) ) )
		{
			if( tgt_entity && tgt_entity.IsItemBase() && player.GetInventory().CanAddEntityIntoInventory(tgt_entity) && tgt_entity.GetHierarchyRootPlayer() != player )
			{
				return true;
			}
		}
		return false;
	}
	
	override void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
	{
		super.WriteToContext(ctx, action_data);
		InventoryLocation il = action_data.m_ReservedInventoryLocations.Get(0);
		il.WriteToContext(ctx);
	}
	
	override bool ReadFromContext(ParamsReadContext ctx, ActionData action_data)
	{
		if(super.ReadFromContext(ctx, action_data))
		{
			InventoryLocation il;
			il = new InventoryLocation;
			if(il.ReadFromContext(ctx))
			{
				action_data.m_ReservedInventoryLocations.Insert(il);
				return true;
			}
		}
			
		return false;
	}
	
	
	
	override bool InventoryReservation(ActionData action_data)
	{		
		bool success = true;
		
		InventoryLocation il = new InventoryLocation;
		
		ItemBase targetItem;
		if ( ItemBase.CastTo(targetItem, action_data.m_Target.GetObject()) )
		{
			action_data.m_Player.GetInventory().FindFreeLocationFor( targetItem , FindInventoryLocationType.ANY, il );
			if ( !action_data.m_Player.GetInventory().AddInventoryReservation( targetItem, il, 10000) )
			{
				success = false;
			}				
		}	
		
		if ( success )
		{
			if( il )
				action_data.m_ReservedInventoryLocations.Insert(il);
		}
		
		return success;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		ItemBase ntarget = ItemBase.Cast(action_data.m_Target.GetObject());
		
		if (!GetGame().IsMultiplayer())
		{
			ActionManagerClient am = ActionManagerClient.Cast(action_data.m_Player.GetActionManager());
			am.UnlockInventory(action_data);
		}
		/*InventoryLocation targetInventoryLocation = new InventoryLocation;
		ntarget.GetInventory().GetCurrentInventoryLocation(targetInventoryLocation);
		
		ntarget.GetInventory().TakeToDst(InventoryMode.PREDICTIVE,targetInventoryLocation, il);*/
		
		action_data.m_Player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, ntarget);
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		ItemBase ntarget = ItemBase.Cast(action_data.m_Target.GetObject());

		ActionManagerClient am = ActionManagerClient.Cast(action_data.m_Player.GetActionManager());
		am.UnlockInventory(action_data);
		/*InventoryLocation targetInventoryLocation = new InventoryLocation;
		ntarget.GetInventory().GetCurrentInventoryLocation(targetInventoryLocation);
		
		ntarget.LocalTakeToDst(targetInventoryLocation, il);*/
		action_data.m_Player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, ntarget);
	}
};