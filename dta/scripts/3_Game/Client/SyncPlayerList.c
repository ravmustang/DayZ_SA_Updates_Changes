class SyncPlayerList
{
	ref array<ref SyncPlayer> m_PlayerList;
	
	void CreatePlayerList()
	{
		if ( GetGame().IsServer() )
		{
			m_PlayerList = new array<ref SyncPlayer>;
			
			array<Man> players = new array<Man>;
			GetGame().GetWorld().GetPlayerList(players);
			
			for ( int i = 0; i < players.Count(); ++i ) 
			{
				Man player = players[i];
				PlayerIdentity p_identity = player.GetIdentity();
				
				if( p_identity )
				{
					ref SyncPlayer sync_player = new SyncPlayer;
					sync_player.m_UID = p_identity.GetPlainId();
					sync_player.m_PlayerName = p_identity.GetName();
					m_PlayerList.Insert( sync_player );
				}
				else
				{
					DebugPrint.LogErrorAndTrace( "No Identity in Server Player List" );
				}
			}
		}
	}
	
	static SyncPlayerList Compare( SyncPlayerList a, SyncPlayerList b )
	{
		ref SyncPlayerList new_list = new SyncPlayerList;
		new_list.m_PlayerList = new array<ref SyncPlayer>;
		
		if( !a && b )
			return b;
		else if( !b )
			return new_list;
		
		array<ref SyncPlayer> array_a = a.m_PlayerList;
		array<ref SyncPlayer> array_b = b.m_PlayerList;
		
		foreach( SyncPlayer player : array_b )
		{
			bool found = false;
			foreach( SyncPlayer player2 : array_a )
			{
				if( player.m_UID == player2.m_UID )
				{
					found = true;
					break;
				}
			}
			
			if( !found )
			{
				new_list.m_PlayerList.Insert( player );
			}
		}
		
		return new_list;
	}
}