class AnalyticsManager
{
	private static const int GEAR_COUNT = 3; 
	private static string m_FullGear[GEAR_COUNT] = {"Shoulder","Melee","Back"};
	
	static void RegisterEvents()
	{
		ClientData.SyncEvent_OnEntityKilled.Insert(Event_OnEntityKilled);
		ClientData.SyncEvent_OnPlayerIgnitedFireplace.Insert(Event_OnPlayerIgnitedFireplace);
	}
	
	static void UnregisterEvents()
	{
		ClientData.SyncEvent_OnEntityKilled.Remove(Event_OnEntityKilled);
		ClientData.SyncEvent_OnPlayerIgnitedFireplace.Remove(Event_OnPlayerIgnitedFireplace);
	}
	
	//===================================
	// OnActionEat
	//===================================
	static void OnActionEat()
	{
		#ifdef PLATFORM_CONSOLE
			AchievementsXbox.OnActionEat();
		#endif
	}
	
	//===================================
	// OnActionDrink
	//===================================
	static void OnActionDrink()
	{
		#ifdef PLATFORM_CONSOLE
			AchievementsXbox.OnActionDrink();
		#endif
	}
	
	//===================================
	// OnActionCookedSteak - not implemented
	//===================================
	static void OnActionCookedSteak()
	{
		#ifdef PLATFORM_CONSOLE
			AchievementsXbox.OnCookedSteak();
		#endif
	}
	
	//===================================
	// OnActionFinishedShaveSelf
	//===================================
	static void OnActionFinishedShaveSelf()
	{
		#ifdef PLATFORM_CONSOLE
			AchievementsXbox.OnActionShave();
		#endif
	}
	
	//===================================
	// OnActionFinishedGutDeer
	//===================================
	static void OnActionFinishedGutDeer()
	{
		#ifdef PLATFORM_CONSOLE
			AchievementsXbox.OnActionGutDeer();
		#endif
	}
	
	//===================================
	// OnActionRestrain
	//===================================
	static void OnActionRestrain()
	{		
		#ifdef PLATFORM_CONSOLE
			AchievementsXbox.OnActionHandcuff();
		#endif
	}
	
	//===================================
	// OnActionBandageTarget
	//===================================
	static void OnActionBandageTarget()
	{		
		#ifdef PLATFORM_CONSOLE
			AchievementsXbox.OnActionMedsSurvivor();
		#endif
	}
	
	//===================================
	// OnItemAttachedAtPlayer
	//===================================
	static void OnItemAttachedAtPlayer(EntityAI item, string slot_name)
	{
		//#ifdef PLATFORM_XBOX
			if ( GetDayZGame().GetGameState() != DayZGameState.IN_GAME )
			{
				return;
			}
		
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			
			if ( player && player.GetInventory() )
			{
				for ( int i = 0; i < GEAR_COUNT; ++i )
				{
					int slot_id = InventorySlots.GetSlotIdFromString(m_FullGear[i]);
					EntityAI att_item = player.GetInventory().FindAttachment( slot_id ); // Boris V [27.2.2019]: Consider using player.GetItemOnSlot(m_FullGear[i]) instead.
					
					if ( !att_item )
					{
						//Print("index: "+ i +" slot_id: "+ slot_id +" = "+ att_item + " EMPTY");
						return;
					}
					//Print("index: "+ i +" slot_id: "+ slot_id +" = "+ att_item + " ATTACHED");
				}
			
				AchievementsXbox.OnEquippedFullGear();
			}
		//#endif
	}
	
	//===================================
	// Event_OnPlayerIgnitedFireplace
	//===================================
	static void Event_OnPlayerIgnitedFireplace( EFireIgniteType ignite_type )
	{
		switch ( ignite_type )
		{
			case EFireIgniteType.Matchbox:
			{
				AchievementsXbox.OnActionIgniteMatchbox();
				break;
			}
			case EFireIgniteType.Roadflare:
			{
				AchievementsXbox.OnActionIgniteRoadflare();
				break;
			}
			case EFireIgniteType.HandDrill:
			{
				AchievementsXbox.OnActionIgniteDrill();
				break;
			}
		}
	}
	
	//===================================
	// Event_OnEntityKilled
	//===================================
	static void Event_OnEntityKilled(EntityAI victim, EntityAI killer, EntityAI source, bool is_headshot)
	{
		#ifdef PLATFORM_CONSOLE
		// Xbox Achievement
		if ( killer != null && killer.IsPlayer() && killer.GetID() == GetGame().GetPlayer().GetID() )
		{
			AchievementsXbox.OnPlayerKilled(victim, killer, source, is_headshot);
		}
		#endif
	}
};