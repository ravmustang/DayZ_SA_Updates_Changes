class SyncEvents
{
	static void RegisterEvents()
	{
		DayZGame dz_game = DayZGame.Cast( GetGame() );
		
		dz_game.Event_OnRPC.Insert( Event_OnRPC );
		Print("SyncEvents -> RegisterEvents");
	}
	
	static void Event_OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{		
		if ( rpc_type == ERPCs.RPC_SYNC_EVENT && GetGame() && GetGame().IsMultiplayer() && GetGame().IsClient() )
		{			
			ref Param2<ESyncEvent, ref SyncData> event_data = new ref Param2<ESyncEvent, ref SyncData>( -1, null );
			
			if ( ctx.Read( event_data ) )
			{
				OnSyncEvent( event_data.param1, event_data.param2, target );
			}
		}
	}
	
	static void OnSyncEvent( ESyncEvent event_type, SyncData data, Object target )
	{
		switch ( event_type )
		{
			case ESyncEvent.PlayerList:
			{
				ClientData.SyncEvent_OnRecievedPlayerList( data.m_ServerPlayerList );
				break;
			}
			case ESyncEvent.EntityKill:
			{
				ClientData.SyncEvent_OnEntityKilled( data.m_EntityKill );
				break;
			}
			case ESyncEvent.PlayerIgnateFireplayce:
			{
				ClientData.SyncEvent_OnPlayerIgnitedFireplace( data.m_SyncInt );
				break;
			}
		}
	}
	
	static void SendPlayerList()
	{
		ref SyncData data = new SyncData;
		data.m_ServerPlayerList = new SyncPlayerList;
		data.m_ServerPlayerList.CreatePlayerList();
		
		SendSyncEvent( ERPCs.RPC_SYNC_EVENT, ESyncEvent.PlayerList, data, true );
	}
	
	static void SendEntityKilled( EntityAI victim, EntityAI killer, EntityAI source, bool is_headshot )
	{
		if ( (victim && victim.IsPlayer()) || (killer && killer.IsPlayer()) )
		{
			ref SyncData data = new SyncData;
			data.m_EntityKill = new SyncEntityKillInfo;
			
			data.m_EntityKill.m_EntityVictim	= victim;
			data.m_EntityKill.m_EntityKiller	= killer;
			data.m_EntityKill.m_EntitySource	= source;
			data.m_EntityKill.m_IsHeadShot		= is_headshot;
			
			if ( victim && victim.IsPlayer() )
			{
				Man man_victim = Man.Cast(victim);
				SendSyncEvent( ERPCs.RPC_SYNC_EVENT, ESyncEvent.EntityKill, data, true, man_victim.GetIdentity() );
			}
			
			if ( killer && killer.IsPlayer() && victim.GetID() != killer.GetID() )
			{
				Man man_killer = Man.Cast(killer);
				SendSyncEvent( ERPCs.RPC_SYNC_EVENT, ESyncEvent.EntityKill, data, true, man_killer.GetIdentity() );
			}
		}
	}
	
	static void SendPlayerIgnatedFireplace( Man player, EFireIgniteType ignite_type )
	{
		ref SyncData data = new SyncData;
		data.m_SyncInt = ignite_type;
		
		SendSyncEvent( ERPCs.RPC_SYNC_EVENT, ESyncEvent.PlayerIgnateFireplayce, data, true, player.GetIdentity() );
	}
	
	private static void SendSyncEvent( ERPCs rpc_event_id, ESyncEvent sync_event_type, ref SyncData data = null, bool guaranteed = true, PlayerIdentity player_target = null )
	{
		ref Param2<ESyncEvent, ref SyncData> event_data = new ref Param2<ESyncEvent, ref SyncData>( sync_event_type, data );
		if ( !player_target )
		{
 			GetGame().RPCSingleParam( null, rpc_event_id, event_data, guaranteed );
		}
		else
		{
			GetGame().RPCSingleParam( null, rpc_event_id, event_data, guaranteed, player_target );
		}
	}
}