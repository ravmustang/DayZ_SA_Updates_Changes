class ActionTakeFireplaceIndoor: ActionInteractBase
{
	void ActionTakeFireplaceIndoor()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageSuccess = "I took the fireplace.";
	}

	override int GetType()
	{
		return AT_TAKE_FIREPLACE_INDOOR;
	}

	override string GetText()
	{
		return "Take fireplace";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		//empty hands	--interact action base condition
		if ( target_object )
		{
			FireplaceIndoor fireplace_indoor = FireplaceIndoor.Cast( target_object );
	
			if ( !fireplace_indoor.HasAshes() && !fireplace_indoor.IsBurning() && fireplace_indoor.IsCargoEmpty() && !fireplace_indoor.GetCookingEquipment() )
			{
				return true;
			}
		}

		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Object target_object = action_data.m_Target.GetObject();
		FireplaceIndoor fireplace_indoor = FireplaceIndoor.Cast( target_object );
		
		TakeFireplaceFromIndoorLambda lambda(fireplace_indoor, "Fireplace", action_data.m_Player);
		action_data.m_Player.ServerReplaceItemWithNew(lambda);
	}
}

class TakeFireplaceFromIndoorLambda : ReplaceItemWithNewLambdaBase
{
	PlayerBase m_Player;

	void TakeFireplaceFromIndoorLambda (EntityAI old_item, string new_item_type, PlayerBase player)
	{
		m_Player = player;

		InventoryLocation targetHnd = new InventoryLocation;
		targetHnd.SetHands(player, null);
		OverrideNewLocation(targetHnd);
	}

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);
		
		array<EntityAI> children = new array<EntityAI>;
		old_item.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, children);
		int count = children.Count();
		for (int i = 0; i < count; i++)
		{
			EntityAI child = children.Get(i);
			if (child)
			{
				InventoryLocation child_src = new InventoryLocation;
				child.GetInventory().GetCurrentInventoryLocation(child_src);
				
				InventoryLocation child_dst = new InventoryLocation;
				child_dst.Copy(child_src);
				child_dst.SetParent(new_item);
				
				m_Player.LocalTakeToDst(child_src, child_dst);
				
				GetGame().RemoteObjectTreeCreate(child); // this forces server to send CreateVehicle Message to client. This is needed for preserving the appearance of network operations on client (so that DeleteObject(old) arrives before CreateVehicle(new)). @NOTE: this does not delete the object on server, only it's network representation.
			}
		}
	}
};