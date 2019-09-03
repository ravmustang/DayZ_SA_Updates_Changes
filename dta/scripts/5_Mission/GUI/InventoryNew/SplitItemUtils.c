class SplitItemUtils
{
	static void TakeOrSplitToInventory ( notnull PlayerBase player, notnull EntityAI target, notnull EntityAI item)
	{
		ItemBase item_base = ItemBase.Cast( item );
		float stackable = item_base.ConfigGetFloat("varStackMax");
		
		if( !item.GetInventory().CanRemoveEntity() || !player.CanManipulateInventory() )
			return;
		
		if( stackable == 0 || stackable >= item_base.GetQuantity() )
		{
			player.PredictiveTakeEntityToTargetAttachment(target, item);
		}
		else if( stackable != 0 && stackable < item_base.GetQuantity() )
		{
			item_base.SplitIntoStackMaxCargoClient( target, -1, 0, 0 );
		}
	}
	
	static void TakeOrSplitToInventoryLocation ( notnull PlayerBase player, notnull InventoryLocation dst)
	{
		ItemBase item_base = ItemBase.Cast( dst.GetItem() );
		float stackable = item_base.ConfigGetFloat("varStackMax");
		
		if( !dst.GetItem().GetInventory().CanRemoveEntity() || !player.CanManipulateInventory() )
			return;
		
		if( stackable == 0 || stackable >= item_base.GetQuantity() )
		{
			InventoryLocation src = new InventoryLocation;
			if (dst.GetItem().GetInventory().GetCurrentInventoryLocation(src))
			{
				player.PredictiveTakeToDst(src, dst);
			}
			else
				Error("TakeIntoCargoEx cannot get src for dst=" + dst.DumpToString());
		}
		else if( stackable != 0 && stackable < item_base.GetQuantity() )
		{
			item_base.SplitIntoStackMaxToInventoryLocationClient( dst );
		}
	}
}