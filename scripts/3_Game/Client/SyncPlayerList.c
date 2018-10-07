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
}