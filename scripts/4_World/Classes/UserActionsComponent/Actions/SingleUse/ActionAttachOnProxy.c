class ActionAttachOnProxy: ActionSingleUseBase
{
	void ActionAttachOnProxy()
	{
		m_MessageSuccess = "I've attached the object.";
	}

	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTParent(10);
	}
	
	override int GetType()
	{
		return AT_ATTACH_ON_PROXY;
	}
		
	override string GetText()
	{
		return "#attach";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		Object targetParent = target.GetParent();
	
		if ( targetObject && targetParent )
		{
			TentBase tent = TentBase.Cast( targetParent );
			
			if ( tent.CanAttach( item ) )
			{
				//return true;
				//Temporary disabled
				return false;
			}
		}
		
		return false;
	}
		
	override void OnExecuteServer( ActionData action_data )
	{		
		EntityAI target_entity = EntityAI.Cast( action_data.m_Target.GetParent() );
		EntityAI item_entity = EntityAI.Cast( action_data.m_MainItem );
		
		//find inventory location for attachment
		InventoryLocation target_location = new InventoryLocation;
		
		if( target_entity.GetInventory().FindFirstFreeLocationForNewEntity( item_entity.GetType(), FindInventoryLocationType.ATTACHMENT, target_location ) )
		{
			action_data.m_Player.PredictiveTakeEntityToTargetAttachmentEx( target_entity, item_entity, target_location.GetSlot() );
		}
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		EntityAI target_entity = EntityAI.Cast( action_data.m_Target.GetParent() );
		EntityAI item_entity = EntityAI.Cast( action_data.m_MainItem );
		
		//find inventory location for attachment
		InventoryLocation target_location = new InventoryLocation;
		
		if( target_entity.GetInventory().FindFirstFreeLocationForNewEntity( item_entity.GetType(), FindInventoryLocationType.ATTACHMENT, target_location ) )
		{
			action_data.m_Player.PredictiveTakeEntityToTargetAttachmentEx( target_entity, item_entity, target_location.GetSlot() );
		}
	}
}