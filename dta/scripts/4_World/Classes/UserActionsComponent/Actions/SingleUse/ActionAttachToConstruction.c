class ActionAttachToConstruction: ActionSingleUseBase
{
	void ActionAttachToConstruction()
	{
		m_MessageSuccess = "I've attached the object.";
	}

	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override int GetType()
	{
		return AT_ATTACH_TO_CONSTRUCTION;
	}
		
	override string GetText()
	{
		return "#attach";
	}
	
	override ActionData CreateActionData()
	{
		AttachActionData action_data = new AttachActionData;
		return action_data;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_entity = EntityAI.Cast( target.GetObject() );
		
		if ( target_entity && target_entity.CanUseConstruction() )
		{
			string selection = target_entity.GetActionComponentName( target.GetComponentIndex() );
			ConstructionActionData construction_action_data = player.GetConstructionActionData();
			int slot_id  = construction_action_data.GetAttachmentSlotFromSelection( target_entity, item, selection );
			
			
			if ( slot_id != -1 )
			{
				if( target_entity.GetInventory().CanAddAttachmentEx( item, slot_id ) && target_entity.CanReceiveAttachment( item, slot_id ) )	
				{
					construction_action_data.SetSlotId( slot_id );
					return true;
				}
				else
				{
					//is entity on slot and can be combined?
					ItemBase attachment_item = ItemBase.Cast( target_entity.GetInventory().FindAttachment( slot_id ) );
					if ( attachment_item && attachment_item.CanBeCombined( item, player ) )
					{
						construction_action_data.SetSlotId( slot_id );
						return true;
					}
				}
			}
		}	
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		EntityAI target_entity = EntityAI.Cast( action_data.m_Target.GetObject() );
		ItemBase item = ItemBase.Cast( action_data.m_MainItem );
		
		ConstructionActionData construction_action_data = action_data.m_Player.GetConstructionActionData();
		int slot_id = construction_action_data.GetSlotId();
		
		if ( slot_id != -1 )
		{
			ItemBase attachment = ItemBase.Cast( target_entity.GetInventory().FindAttachment( slot_id ) );
			
			if ( attachment )
			{
				//combine
				construction_action_data.CombineItems( attachment, item );
			}
			else
			{
				//attach
				if ( GetGame().IsMultiplayer() )
				{
					action_data.m_Player.ServerTakeEntityToTargetAttachmentEx( target_entity, item, slot_id );
				}
				else
				{
					action_data.m_Player.PredictiveTakeEntityToTargetAttachmentEx( target_entity, item, slot_id );
				}
			}
		}
	}
	

}