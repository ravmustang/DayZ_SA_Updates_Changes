class JsonPlayerPositions
{
    ref array<ref Vector2> m_Positions = new array<ref Vector2>();

	//--------------------------------------------
	// InsertPos
	//--------------------------------------------
	void InsertPos( Vector2 pos )
    {
        m_Positions.Insert( pos );
    }	
	
	//--------------------------------------------
	// SaveJson
	//--------------------------------------------
	void SaveJson()
	{
		string file_name = GetCurrentDate("_")+"-"+GetCurrentTime("_");
		file_name = file_name + ".json";
		
		if ( !FileExist( "$profile:ServerPlayers" ) )
		{
			MakeDirectory("$profile:ServerPlayers");
		}
		
		JsonFileLoader<JsonPlayerPositions>.JsonSaveFile( "$profile:ServerPlayers/"+ file_name, this );
	}
	
	//--------------------------------------------
	// GetCurrentDate
	//--------------------------------------------
	static string GetCurrentDate(string separator)
	{
		int year = 0;
		int month = 0;
		int day = 0;
		
		GetYearMonthDay( year, month, day );
		
		return (year.ToStringLen(4) + separator + month.ToStringLen(2) + separator + day.ToStringLen(2));
	}	
	
	//--------------------------------------------
	// GetCurrentTime
	//--------------------------------------------
	static string GetCurrentTime(string separator)
	{
		int hour = 0;
		int minute = 0;
		int second = 0;
		
		GetHourMinuteSecond(hour, minute, second );
			
		return (hour.ToStringLen(2) + separator + minute.ToStringLen(2) + separator + second.ToStringLen(2));
	}
}