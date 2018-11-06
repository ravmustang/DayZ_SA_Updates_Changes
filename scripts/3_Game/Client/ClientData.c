class ClientData
{
	static ref ScriptInvoker SyncEvent_OnPlayerListUpdate = new ScriptInvoker();
	
	static ref SyncPlayerList		m_PlayerList;
	
	static ref OnlineServices		m_OnlineServices;
	
	
	static void SyncEvent_OnRecievedPlayerList( SyncPlayerList player_list )
	{
		SyncPlayerList new_players = SyncPlayerList.Compare( m_PlayerList, player_list );
		if ( m_PlayerList )
		{
			delete m_PlayerList;
		}
		m_PlayerList = player_list;
		
		#ifdef PLATFORM_CONSOLE
			#ifndef PLATFORM_WINDOWS // if app is not on Windows with -XBOX parameter
				OnlineServices.LoadPermissions( GetSimplePlayerList( new_players ) );
			#endif
		#endif
		SyncEvent_OnPlayerListUpdate.Invoke( player_list );
	}
	
	static array<string> GetSimplePlayerList( SyncPlayerList list )
	{
		ref array<string> ids = new array<string>;
		if( list )
		{
			for( int i = 0; i < list.m_PlayerList.Count(); i++ )
			{
				SyncPlayer player = list.m_PlayerList.Get( i );
				ids.Insert( player.m_UID );
			}
		}
		
		return ids;
	}
}