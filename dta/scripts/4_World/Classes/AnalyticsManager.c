class AnalyticsManager
{
	static void RegisterEvents()
	{
		ClientData.SyncEvent_OnEntityKilled.Insert(Event_OnEntityKilled);
	}
	
	static void UnregisterEvents()
	{
		ClientData.SyncEvent_OnEntityKilled.Remove(Event_OnEntityKilled);
	}
	
	//===================================
	// OnFinishedActionEat
	//===================================
	static void OnActionEat()
	{
		#ifdef PLATFORM_XBOX
			//AchievementsXbox.OnActionEat();
		#endif
	}
	
	//===================================
	// OnFinishedActionDrink
	//===================================
	static void OnActionDrink()
	{
		#ifdef PLATFORM_XBOX
			//AchievementsXbox.OnActionDrink();
		#endif
	}
	
	//===================================
	// OnFinishedActionDrink
	//===================================
	static void OnItemAttachedAtPlayer(EntityAI item, string slot_name)
	{
		/*
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		int slot_count = player.GetInventory().GetAttachmentSlotsCount();
		*/
		/*
		for ( int i = 0; i < slot_count; ++i )
		{
			int slot_id = player.GetInventory().GetSlotId (i);
			
			EntityAI att_item = player.GetInventory().FindAttachment( slot_id );
			
			Print("index: "+ i +" slot_id: "+ slot_id +" = "+ att_item);
		}
		*/
		
		
			/*
		if ( is_full )
		{
			#ifdef PLATFORM_XBOX
				AchievementsXbox.OnEquippedFullGear();
			#endif
		}
		*/
	}
	
	//===================================
	// Event_OnEntityKilled
	//===================================
	static void Event_OnEntityKilled(EntityAI victim, EntityAI killer, EntityAI source, bool is_headshot)
	{
		#ifdef PLATFORM_XBOX
		// Xbox Achievement
		if ( killer != null && killer.IsPlayer() && killer.GetID() == GetGame().GetPlayer().GetID() )
		{
			//AchievementsXbox.OnPlayerKilled(victim, killer, source, is_headshot);
		}
		#endif
	}
};