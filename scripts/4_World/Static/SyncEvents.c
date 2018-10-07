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