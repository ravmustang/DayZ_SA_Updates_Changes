class ServerBrowserEntry extends ScriptedWidgetEventHandler
{
	protected Widget					m_Root;
	protected Widget					m_Favorite;
	protected Widget					m_Expand;
	
	//Basic info
	protected TextWidget				m_ServerName;
	protected TextWidget				m_ServerPopulation;
	protected TextWidget				m_ServerSlots;
	protected TextWidget				m_ServerPing;
	protected ImageWidget				m_ServerTime;
	protected ImageWidget				m_ServerLock;
	
	//Detailed info
	protected TextWidget				m_ServerShard;
	protected TextWidget				m_ServerCharacterAlive;
	protected TextWidget				m_ServerFriends;
	protected TextWidget				m_ServerMode;
	protected TextWidget				m_ServerBattleye;
	protected TextWidget				m_ServerIP;
	
	protected bool						m_IsExpanded;
	protected bool						m_IsFavorited;
	
	protected ref GetServersResultRow	m_ServerData;
	protected int						m_Index;
	protected ServerBrowserTab			m_Tab;
	protected bool						m_Selected;
	
	void ServerBrowserEntry( Widget parent, int index, ServerBrowserTab tab )
	{
		#ifdef PLATFORM_CONSOLE
			m_Root				= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/server_browser/xbox/server_browser_list_entry.layout", parent );
		#else
		#ifdef PLATFORM_WINDOWS
			m_Root				= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/server_browser/pc/server_browser_list_entry.layout", parent );
		#endif
		#endif
		m_Root.SetSort( index );
		
		m_Favorite				= m_Root.FindAnyWidget( "favorite_button" );
		m_Expand				= m_Root.FindAnyWidget( "expand_button" );
		m_ServerName			= TextWidget.Cast( m_Root.FindAnyWidget( "server_name" ) );
		m_ServerPopulation		= TextWidget.Cast( m_Root.FindAnyWidget( "server_population" ) );
		m_ServerSlots			= TextWidget.Cast( m_Root.FindAnyWidget( "server_slots" ) );
		m_ServerPing			= TextWidget.Cast( m_Root.FindAnyWidget( "server_ping" ) );
		m_ServerTime			= ImageWidget.Cast( m_Root.FindAnyWidget( "server_time" ) );
		m_ServerLock			= ImageWidget.Cast( m_Root.FindAnyWidget( "lock_icon" ) );
		
		m_ServerShard			= TextWidget.Cast( m_Root.FindAnyWidget( "shard_text" ) );
		m_ServerCharacterAlive	= TextWidget.Cast( m_Root.FindAnyWidget( "character_alive_text" ) );
		m_ServerFriends			= TextWidget.Cast( m_Root.FindAnyWidget( "steam_friends_text" ) );
		m_ServerMode			= TextWidget.Cast( m_Root.FindAnyWidget( "mode_text" ) );
		m_ServerBattleye		= TextWidget.Cast( m_Root.FindAnyWidget( "battlleye_text" ) );
		m_ServerIP				= TextWidget.Cast( m_Root.FindAnyWidget( "ip_text" ) );
		
		m_Index					= index;
		m_Tab					= tab;
		
		float alpha = 0.1;
		if( m_Index % 2 )
		{
			alpha = 0.2;
		}
		m_Root.SetAlpha( alpha );
		m_Root.SetHandler( this );
	}
	
	void ~ServerBrowserEntry()
	{
		delete m_Root;
	}
	
	Widget GetRoot()
	{
		return m_Root;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		#ifdef PLATFORM_CONSOLE
		if( w == m_Root	)
		{
			m_Tab.Connect( this );
			return true;
		}
		#endif
		return false;
	}
	
	override bool OnDoubleClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_Root )
			{
				m_Tab.Connect( this );
				return true;
			}
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_Favorite )
			{
				ToggleFavorite();
				return true;
			}
			else if ( w == m_Expand )
			{
				ToggleExpand();
				return true;
			}
			else
			if( w == m_Root )
			{
				Select();
				return true;
			}
		}
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			Darken( w, x, y );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			Lighten( w, enterW, x, y );
			return true;
		}
		return false;
	}
	
	void Focus()
	{
		OnFocus( m_Root, 0, 0 );
	}
	
	void ServerListFocus( bool focus )
	{
		m_Tab.ServerListFocus( focus, m_IsFavorited );
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( !m_Selected )
		{
			if( IsFocusable( w ) )
			{
				Darken( w, x, y );
			}
			#ifdef PLATFORM_CONSOLE
			if( w == m_Root )
			{
				Select();
				ServerListFocus( true );
			}
			#endif
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		#ifdef PLATFORM_CONSOLE
		if( w == m_Root )
		{
			Deselect();
			ServerListFocus( false );
		}
		#endif
		
		if( IsFocusable( w ) )
		{
			Lighten( w, null, x, y );
		}
		
		return true;
		return false;
	}
	
	bool IsFocusable( Widget w )
	{
		return ( w == m_Root || w == m_Favorite || w == m_Expand );
	}
	
	void FillInfo( GetServersResultRow server_info )
	{
		m_ServerData = server_info;
		
		SetName( server_info.m_Name );
		SetPasswordLocked( server_info.m_IsPasswordProtected );
		SetPopulation( server_info.m_CurrentNumberPlayers, server_info.m_MaxPlayers );
		SetSlots( server_info.m_MaxPlayers );
		SetPing( -1 );
		SetTime( -1 );
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
			SetShard( server_info.m_Official );
			SetCharacterAlive( "Missing data" );
			SetFriends( {"Missing data"} );
			SetMode( server_info.m_ModeId );
			SetBattleye( server_info.m_AntiCheat );
			SetIP( server_info.m_HostIp + ":" + server_info.m_HostPort.ToString() );
		#endif
		#endif
	}
	
	void SetName( string name )
	{
		m_ServerName.SetText( name );
	}
	
	void SetPasswordLocked( bool locked )
	{
		m_ServerLock.Show( locked );
	}
	
	void SetPopulation( int population, int slots )
	{
		/*
		string pop_text;
		float pop_percentage = population / slots;
		if( population == 0 )
			pop_text	= "Empty";
		else if( pop_percentage < 0.33 )
			pop_text	= "Low (" + population.ToString() + ")" ;
		else if( pop_percentage < 0.66 )
			pop_text	= "Medium (" + population.ToString() + ")" ;
		else if( pop_percentage != 1 )
			pop_text	= "High (" + population.ToString() + ")" ;
		else
			pop_text	= "Full";
		*/
		m_ServerPopulation.SetText( population.ToString() );
	}
	
	void SetSlots( int slots )
	{
		m_ServerSlots.SetText( slots.ToString() );
	}
	
	void SetPing( int ping )
	{
		int color;
		if( ping < 50 )
			color	= ARGBF( 1, 0, 1, 0 );
		else if( ping < 100 )
			color	= ARGBF( 1, 0.8, 0.8, 0 );
		else if( ping < 200 )
			color	= ARGBF( 1, 1, 0.5, 0 );
		else
			color	= ARGBF( 1, 1, 0, 0 );
		
		m_ServerPing.SetColor( color );
		m_ServerPing.SetText( ping.ToString() );
	}
	
	void SetTime( int time )
	{
		//m_ServerTime.SetText( time.ToString() );
	}
	
	void SetShard( int shard )
	{
		string text;
		switch( shard )
		{
			case 0:
			{
				text = "Private";
				break;
			}
			case 1:
			{
				text = "Official";
				break;
			}
		}
		m_ServerShard.SetText( text );
	}
	
	void SetCharacterAlive( string char_alive )
	{
		m_ServerCharacterAlive.SetText( char_alive );
	}
	
	void SetFriends( array<string> friends )
	{
		string friends_text;
		if( friends && friends.Count() > 0 )
		{
			friends_text = friends.Get( 0 );
			for( int i = 1; i < friends.Count(); i++ )
			{
				friends_text += ", " + friends.Get( i );
			}
		}
		m_ServerFriends.SetText( friends_text );
	}
	
	void SetMode( int mode )
	{
		string text;
		switch( mode )
		{
			case 0:
			{
				text = "1st/3rd Person";
				break;
			}
			case 1:
			{
				text = "1st Person Only";
				break;
			}
		}
		m_ServerMode.SetText( text );
	}
	
	void SetBattleye( bool battleye )
	{
		if( battleye )
		{
			m_ServerBattleye.SetText( "Enabled" );
			m_ServerBattleye.SetColor( ARGBF( 1, 0, 1, 0 ) );
		}
		else
		{
			m_ServerBattleye.SetText( "Disabled" );
			m_ServerBattleye.SetColor( ARGBF( 1, 1, 0, 0 ) );
		}
	}
	
	void SetIP( string ip )
	{
		m_ServerIP.SetText( ip );
	}
	
	string GetIP()
	{
		return m_ServerData.m_HostIp;
	}
	
	int GetPort()
	{
		return m_ServerData.m_HostPort;
	}
	
	string GetServerID()
	{
		return m_ServerData.m_Id;
	}
	
	void SetFavorite( bool favorite )
	{
		m_IsFavorited = favorite;
		m_Root.FindAnyWidget( "unfavorite_image" ).Show( m_IsFavorited );
	}
	
	bool ToggleFavorite()
	{
		m_IsFavorited = !m_IsFavorited;
		m_Root.FindAnyWidget( "unfavorite_image" ).Show( m_IsFavorited );
		m_Tab.SetFavorite( GetServerID(), m_IsFavorited );
		return m_IsFavorited;
	}
	
	bool ToggleExpand()
	{
		m_IsExpanded = !m_IsExpanded;
		m_Root.FindAnyWidget( "collapse_image" ).Show( m_IsExpanded );
		m_Root.FindAnyWidget( "expand_image" ).Show( !m_IsExpanded );
		m_Root.FindAnyWidget( "detailed_info" ).Show( m_IsExpanded );
		
		return m_IsExpanded;
	}
	
	void Select( bool notify = true )
	{
		//if( !m_Selected )
		{
			if( notify )
			{
				m_Tab.SelectServer( this );
			}
			m_Selected = true;
			#ifdef PLATFROM_XBOX
				m_Root.SetColor( 1, 0.3, 0.3, 0.3 ) );
			#endif
		}
	}
	
	void Deselect()
	{
		if( m_Selected )
		{
			m_Selected = false;
			m_Root.SetColor( ARGB( 0, 0, 0, 0 ) );
			Lighten( m_Root, null, 0, 0 );
		}
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void Darken( Widget w, int x, int y )
	{
		SetFocus( m_Root );
		
		if( m_Selected )
			return;
		if( w == m_Root || w == m_Favorite || w == m_Expand )
		{
			m_Root.SetAlpha( 1 );
			m_ServerName.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
	}
	
	void Lighten( Widget w, Widget enterW, int x, int y )
	{
		if( GetFocus() == w )
			return;
		if( m_Selected )
			return;
		if( w == m_Root && ( ( m_Favorite && enterW == m_Favorite ) || ( m_Expand && enterW == m_Expand ) ) )
		{
			return;
		}
		
		float alpha = 0.1;
		if( m_Index % 2 )
		{
			alpha = 0.2;
		}
		m_Root.SetAlpha( alpha );
		m_ServerName.SetColor( ARGB( 255, 255, 255, 255 ) );
	}
}