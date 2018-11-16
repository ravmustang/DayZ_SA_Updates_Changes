class JsonFileLoader<Class T>
{
	protected static ref JsonSerializer m_Serializer = new JsonSerializer;
	
	static void JsonLoadFile( string filename, out T data )
	{
		if( FileExist( filename ) )
		{
			string file_content;
			string line_content;
			string error;
			
			FileHandle handle = OpenFile( filename, FileMode.READ );
			if ( handle == 0 )
				return;
			
			
			while ( FGets( handle,  line_content ) > 0 )
			{
				file_content += line_content;
			}
			
			CloseFile( handle );
			
			if( !m_Serializer.ReadFromString( data, file_content, error ) )
				Error( error );
		}
	}
	
	static void JsonSaveFile( string filename, T data )
	{
		string file_content;
		m_Serializer.WriteToString( data, false, file_content );
		
		FileHandle handle = OpenFile( filename, FileMode.WRITE );
		if ( handle == 0 )
			return;
		
		FPrint( handle, file_content );
		
		CloseFile( handle );
	}
	
	static void JsonLoadData( string string_data, out T data )
	{
		string error;
		if( !m_Serializer.ReadFromString( data, string_data, error ) )
			Error( error );
		
	}
	
	static string JsonMakeData( T data )
	{
		string string_data;
		m_Serializer.WriteToString( data, false, string_data );
		return string_data;
	}
}