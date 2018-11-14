
bool TryAcquireInventoryJunctureFromServer (notnull Man player, notnull InventoryLocation src, notnull InventoryLocation dst)
{
	if (player.NeedInventoryJunctureFromServer(src.GetItem(), src.GetParent(), dst.GetParent()))
	{
		bool test_dst_occupancy = true;
		if (GetGame().AddInventoryJuncture(player, src.GetItem(), dst, test_dst_occupancy, GameInventory.c_InventoryReservationTimeoutMS))
		{
			return JunctureRequestResult.JUNCTURE_ACQUIRED; // ok
		}
		else
		{
			syncDebugPrint("[syncinv] juncture request DENIED");
			return JunctureRequestResult.JUNCTURE_DENIED; // permission to perform juncture denied
		}
	}
	else
	{
		syncDebugPrint("[syncinv] juncture not required");
		return JunctureRequestResult.JUNCTURE_NOT_REQUIRED; // juncture not necessary
	}
}

bool TryAcquireTwoInventoryJuncturesFromServer (notnull Man player, notnull InventoryLocation src1, notnull InventoryLocation src2, notnull InventoryLocation dst1, notnull InventoryLocation dst2)
{
	//syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms TryAcquireTwoInventoryJuncturesFromServer src1=" + src1.DumpToString() + "dst1=" + dst1.DumpToString() +  "src2=" + src2.DumpToString() + "dst2=" + dst2.DumpToString());

	bool need_j1 = player.NeedInventoryJunctureFromServer(src1.GetItem(), src1.GetParent(), dst1.GetParent());
	bool need_j2 = player.NeedInventoryJunctureFromServer(src2.GetItem(), src2.GetParent(), dst2.GetParent());
	if (need_j1 || need_j2)
	{
		if (need_j1)
		{
			if (!GetGame().AddInventoryJuncture(player, src1.GetItem(), dst1, false, GameInventory.c_InventoryReservationTimeoutMS))
			{
				syncDebugPrint("[syncinv] item1 juncture request DENIED");
				return JunctureRequestResult.JUNCTURE_DENIED; // permission to perform juncture denied
			}
		}

		if (need_j2)
		{
			if (!GetGame().AddInventoryJuncture(player, src2.GetItem(), dst2, false, GameInventory.c_InventoryReservationTimeoutMS))
			{
				syncDebugPrint("[syncinv] item2 juncture request DENIED");
				if (need_j1)
				{
					GetGame().ClearJuncture(player, src1.GetItem()); // release already acquired juncture for item1
					syncDebugPrint("[syncinv] item2 juncture request DENIED, cleaning acquired juncture for item1");
				}
				return JunctureRequestResult.JUNCTURE_DENIED; // permission to perform juncture denied
			}
		}

		return JunctureRequestResult.JUNCTURE_ACQUIRED; // ok, both junctures acquired
	}
	else
	{
		syncDebugPrint("[syncinv] junctures not required");
		return JunctureRequestResult.JUNCTURE_NOT_REQUIRED; // juncture not necessary
	}
}

