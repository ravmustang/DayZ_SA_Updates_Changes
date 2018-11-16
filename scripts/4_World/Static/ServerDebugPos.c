class ServerDebugPos
{
	static void SavePlayerPositions()
	{
		array<Man> players = new array<Man>();
		
		GetGame().GetPlayers( players );
		
		if ( players.Count() >= 20 )
		{
			JsonPlayerPositions json_data_positions = new JsonPlayerPositions();
			for ( int i = 0; i < players.Count(); ++i )
			{
				vector pos = players[i].GetPosition();
				
				json_data_positions.InsertPos( new Vector2(pos[0], pos[2]) );
			}
			
			json_data_positions.SaveJson();
		}
	}
}