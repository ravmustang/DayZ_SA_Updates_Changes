class ServerBrowserFilterContainer extends ScriptedWidgetEventHandler
{
	protected EditBoxWidget					m_SearchByName;
	protected EditBoxWidget					m_SearchByIP;
			ref OptionSelectorMultistate	m_CharacterAliveFilter;
			ref OptionSelectorMultistate	m_SortingFilter;
			ref OptionSelectorMultistate	m_RegionFilter;
			ref OptionSelectorMultistate	m_PingFilter;
			ref OptionSelector				m_FavoritedFilter;
			ref OptionSelector				m_FriendsPlayingFilter;
			ref OptionSelector				m_BattleyeFilter;
			ref OptionSelector				m_PasswordFilter;
			ref OptionSelector				m_PreviouslyPlayedFilter;
			ref OptionSelector				m_VersionMatchFilter;
			ref OptionSelector				m_FullServerFilter;
			ref OptionSelector				m_ThirdPersonFilter;
			ref OptionSelector				m_PublicFilter;
			ref OptionSelector				m_AcceleratedTimeFilter;
	protected ServerBrowserTab				m_Tab;
	
	void ServerBrowserFilterContainer( Widget root, ServerBrowserTab parent )
	{
		string player_name;
		GetGame().GetPlayerName( player_name );
		m_Tab = parent;
		
		ref array<string> character_name_options ={ "#server_browser_disabled", player_name };
		ref array<string> region_options = { "#server_browser_all", "#server_browser_americas", "#server_browser_europe", "#server_browser_asia" };
		ref array<string> sort_options = { "#server_browser_name_descending", "server_browser_name_ascending", "#server_browser_slots_descending", "#server_browser_slots_ascending" };
		ref array<string> ping_options = { "#server_browser_disabled", "<30", "<50", "<100", "<200", "<300", "<500" };
		ref array<string> three_options = { "#server_browser_disabled", "#server_browser_show", "#server_browser_hide" };
		
		m_SearchByName				= EditBoxWidget.Cast( root.FindAnyWidget( "search_name_setting_option" ) );
		m_SearchByIP				= EditBoxWidget.Cast( root.FindAnyWidget( "search_ip_setting_option" ) );
		
		m_RegionFilter				= new OptionSelectorMultistate( root.FindAnyWidget( "region_setting_option" ), 0, this, false, region_options );
		m_PingFilter				= new OptionSelectorMultistate( root.FindAnyWidget( "ping_setting_option" ), 0, this, false, ping_options );
		m_FavoritedFilter			= new OptionSelector( root.FindAnyWidget( "favorites_setting_option" ), 0, this, false );
		m_FriendsPlayingFilter		= new OptionSelector( root.FindAnyWidget( "friends_setting_option" ), 0, this, false );
		m_PreviouslyPlayedFilter	= new OptionSelector( root.FindAnyWidget( "prev_played_setting_option" ), 0, this, false );
		m_FullServerFilter			= new OptionSelector( root.FindAnyWidget( "full_server_setting_option" ), 0, this, false );
		
		m_RegionFilter.m_OptionChanged.Insert( OnFilterChanged );
		m_PingFilter.m_OptionChanged.Insert( OnFilterChanged );
		m_FavoritedFilter.m_OptionChanged.Insert( OnFilterChanged );
		m_FriendsPlayingFilter.m_OptionChanged.Insert( OnFilterChanged );
		m_PreviouslyPlayedFilter.m_OptionChanged.Insert( OnFilterChanged );
		m_FullServerFilter.m_OptionChanged.Insert( OnFilterChanged );
		
		#ifdef PLATFORM_CONSOLE
			m_SortingFilter			= new OptionSelectorMultistate( root.FindAnyWidget( "sort_setting_option" ), 0, this, false, sort_options );
			m_SortingFilter.m_OptionChanged.Insert( OnSortChanged );
			m_SortingFilter.m_OptionChanged.Insert( OnFilterChanged );
			m_PingFilter.Disable();
			m_FriendsPlayingFilter.Disable();
		#endif
		
		#ifdef PLATFORM_WINDOWS
			#ifndef PLATFORM_CONSOLE
				m_CharacterAliveFilter		= new OptionSelectorMultistate( root.FindAnyWidget( "restrict_char_setting_option" ), 0, this, false, character_name_options );
				m_BattleyeFilter			= new OptionSelector( root.FindAnyWidget( "battleye_setting_option" ), 0, this, false );
				m_PasswordFilter			= new OptionSelector( root.FindAnyWidget( "password_setting_option" ), 0, this, false  );
				m_VersionMatchFilter		= new OptionSelector( root.FindAnyWidget( "ver_match_setting_option" ), 0, this, false );
				m_ThirdPersonFilter			= new OptionSelector( root.FindAnyWidget( "tps_setting_option" ), 0, this, false );
				m_PublicFilter				= new OptionSelector( root.FindAnyWidget( "public_setting_option" ), 0, this, false );
				m_AcceleratedTimeFilter		= new OptionSelector( root.FindAnyWidget( "accelerated_time_setting_option" ), 0, this, false );
		
				m_CharacterAliveFilter.m_OptionChanged.Insert( OnFilterChanged );
				m_BattleyeFilter.m_OptionChanged.Insert( OnFilterChanged );
				m_PasswordFilter.m_OptionChanged.Insert( OnFilterChanged );
				m_VersionMatchFilter.m_OptionChanged.Insert( OnFilterChanged );
				m_ThirdPersonFilter.m_OptionChanged.Insert( OnFilterChanged );
				m_PublicFilter.m_OptionChanged.Insert( OnFilterChanged );
				m_AcceleratedTimeFilter.m_OptionChanged.Insert( OnFilterChanged );
			#endif
		#endif
		LoadFilters();
	}
	
	void OnFilterChanged()
	{
		m_Tab.OnFilterChanged();
	}
	
	void LoadFilters()
	{
		string data;
		GetGame().GetProfileString( "SB_Filter_" + m_Tab.GetTabType(), data );
		ref map<string, string> options = new map<string, string>;
		if( data != "" )
		{
			JsonFileLoader<ref map<string, string>>.JsonLoadData( data, options );
		}
		
		if( options && options.Count() >= 6 )
		{
			m_RegionFilter.SetStringOption( options.Get( "m_RegionFilter" ), false );
			m_PingFilter.SetStringOption( options.Get( "m_PingFilter" ), false );
			m_FavoritedFilter.SetStringOption( options.Get( "m_FavoritedFilter" ), false );
			m_FriendsPlayingFilter.SetStringOption( options.Get( "m_FriendsPlayingFilter" ), false );
			m_PreviouslyPlayedFilter.SetStringOption( options.Get( "m_PreviouslyPlayedFilter" ), false );
			m_FullServerFilter.SetStringOption( options.Get( "m_FullServerFilter" ), false );
			
			#ifdef PLATFORM_CONSOLE
				m_SortingFilter.SetStringOption( options.Get( "m_SortingFilter" ), false );
			#endif
			
			if( options.Count() >= 12 )
			{
				#ifdef PLATFORM_WINDOWS
					#ifndef PLATFORM_CONSOLE
						m_SearchByName.SetText( options.Get( "m_SearchByName" ) );
						m_SearchByIP.SetText( options.Get( "m_SearchByIP" ) );
						m_CharacterAliveFilter.SetStringOption( options.Get( "m_CharacterAliveFilter" ), false );
						m_BattleyeFilter.SetStringOption( options.Get( "m_BattleyeFilter" ), false );
						m_PasswordFilter.SetStringOption( options.Get( "m_PasswordFilter" ), false );
						m_VersionMatchFilter.SetStringOption( options.Get( "m_VersionMatchFilter" ), false );
						m_ThirdPersonFilter.SetStringOption( options.Get( "m_ThirdPersonFilter" ), false );
						m_PublicFilter.SetStringOption( options.Get( "m_PublicFilter" ), false );
						m_AcceleratedTimeFilter.SetStringOption( options.Get( "m_AcceleratedTimeFilter" ), false );
					#endif
				#endif
			}
		}
	}
	
	void SaveFilters()
	{
		ref map<string, string> options = new map<string, string>;
		
		options.Insert( "m_RegionFilter", m_RegionFilter.GetStringValue() );
		options.Insert( "m_PingFilter", m_PingFilter.GetStringValue() );
		options.Insert( "m_FavoritedFilter", m_FavoritedFilter.GetStringValue() );
		options.Insert( "m_FriendsPlayingFilter", m_FriendsPlayingFilter.GetStringValue() );
		options.Insert( "m_PreviouslyPlayedFilter", m_PreviouslyPlayedFilter.GetStringValue() );
		options.Insert( "m_FullServerFilter", m_FullServerFilter.GetStringValue() );
		
		#ifdef PLATFORM_CONSOLE
			options.Insert( "m_SortingFilter", m_SortingFilter.GetStringValue() );
		#endif
		
		#ifdef PLATFORM_WINDOWS
			#ifndef PLATFORM_CONSOLE
				options.Insert( "m_SearchByName", m_SearchByName.GetText() );
				options.Insert( "m_SearchByIP", m_SearchByIP.GetText() );
				options.Insert( "m_CharacterAliveFilter", m_CharacterAliveFilter.GetStringValue() );
				options.Insert( "m_BattleyeFilter", m_BattleyeFilter.GetStringValue() );
				options.Insert( "m_PasswordFilter", m_PasswordFilter.GetStringValue() );
				options.Insert( "m_VersionMatchFilter", m_VersionMatchFilter.GetStringValue() );
				options.Insert( "m_ThirdPersonFilter", m_ThirdPersonFilter.GetStringValue() );
				options.Insert( "m_PublicFilter", m_PublicFilter.GetStringValue() );
				options.Insert( "m_AcceleratedTimeFilter", m_AcceleratedTimeFilter.GetStringValue() );
			#endif
		#endif
		string data = JsonFileLoader<ref map<string, string>>.JsonMakeData( options );
		GetGame().SetProfileString( "SB_Filter_" + m_Tab.GetTabType(), data );
	}
	
	void ResetFilters()
	{
		m_RegionFilter.Reset();
		m_PingFilter.Reset();
		m_FavoritedFilter.Reset();
		m_FriendsPlayingFilter.Reset();
		m_PreviouslyPlayedFilter.Reset();
		m_FullServerFilter.Reset();
		
		#ifdef PLATFORM_CONSOLE
			m_SortingFilter.Reset();
		#endif
		
		#ifdef PLATFORM_WINDOWS
			#ifndef PLATFORM_CONSOLE
				m_SearchByName.SetText( "" );
				m_SearchByIP.SetText( "" );
				m_CharacterAliveFilter.Reset();
				m_BattleyeFilter.Reset();
				m_PasswordFilter.Reset();
				m_VersionMatchFilter.Reset();
				m_ThirdPersonFilter.Reset();
				m_PublicFilter.Reset();
				m_AcceleratedTimeFilter.Reset();
			#endif
		#endif
	}
	
	void OnSortChanged( int value )
	{
		switch( value )
		{
			case 0:
			{
				m_Tab.SetSort( ESortType.HOST, ESortOrder.DESCENDING );
				break;
			}
			case 1:
			{
				m_Tab.SetSort( ESortType.HOST, ESortOrder.ASCENDING );
				break;
			}
			case 2:
			{
				m_Tab.SetSort( ESortType.SLOTS, ESortOrder.DESCENDING );
				break;
			}
			case 3:
			{
				m_Tab.SetSort( ESortType.SLOTS, ESortOrder.ASCENDING );
				break;
			}
		}
	}
	
	void Focus()
	{
		#ifdef PLATFORM_CONSOLE
			m_SortingFilter.Focus();
		#else
		#ifdef PLATFORM_WINDOWS
			SetFocus( m_SearchByName );
		#endif
		#endif
	}
	
	override bool OnEvent( EventType eventType, Widget target, int parameter0, int parameter1 )
	{
		if( target )
		{
			if( target == m_SearchByName || target == m_SearchByIP )
			{
				OnFilterChanged();
				return false;
			}
		}
		return true;
	}
	
	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		if( w )
		{
			if( w == m_SearchByName || w == m_SearchByIP )
			{
				OnFilterChanged();
				return false;
			}
		}
		return true;
	}
	
	override bool OnUpdate( Widget w )
	{
		if( w )
		{
			if( w == m_SearchByName || w == m_SearchByIP )
			{
				OnFilterChanged();
				return false;
			}
		}
		return true;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		m_Tab.FilterFocus( ( w != null ) );
		return true;
	}
	
	GetServersInput GetFilterOptions()
	{
		ref GetServersInput input = new GetServersInput;
		
		input.m_RowsPerPage = SERVER_BROWSER_PAGE_SIZE;
		//input.m_Platform = 1;

		#ifdef PLATFORM_XBOX
			input.m_Platform = 2;
		#endif
		#ifdef PLATFORM_PS4
			input.m_Platform = 3;
		#endif
		
		if( m_SearchByName )
		{
			string name_text = m_SearchByName.GetText();
			if( name_text != "" )
			{
				input.SetNameFilter( name_text );
			}
		}
		#ifdef PLATFORM_XBOX
		if( m_RegionFilter.IsSet() )
		{
			input.SetRegionIdFilter( m_RegionFilter.GetValue() );
		}
		#endif
		if( m_PingFilter.IsSet() )
		{
			//Character filter
		}
		if( m_FavoritedFilter.IsSet() )
		{
			//Character filter
		}
		if( m_FriendsPlayingFilter.IsSet() )
		{
			#ifdef PLATFORM_WINDOWS
				input.SetFriendsPlaying( m_FriendsPlayingFilter.IsEnabled() );
			#endif
		}
		if( m_PreviouslyPlayedFilter.IsSet() )
		{
			#ifdef PLATFORM_WINDOWS
				input.SetPreviouslyPlayed( m_PreviouslyPlayedFilter.IsEnabled() );
			#endif
		}
		if( m_FullServerFilter.IsSet() )
		{
			input.SetFullServer( m_FullServerFilter.IsEnabled() );
			if( m_FullServerFilter.IsEnabled() )
			{
				input.SetFreeSlotsMax( 1 );
			}
			else
			{
				input.SetFreeSlotsMin( 1 );
			}
		}
		
		#ifdef PLATFORM_WINDOWS
			#ifndef PLATFORM_CONSOLE
				if( m_SearchByIP )
				{
					string ip_text = m_SearchByIP.GetText();
					if( ip_text != "" )
					{
						TStringArray arr	= new TStringArray;
						ip_text.Split( ":", arr );
						string ip;
						int port;
						ip					= arr.Get( 0 );
				
						if( arr.Count() > 1 )
							port			= arr.Get( 1 ).ToInt();
						
						#ifdef PLATFORM_CONSOLE
						input.SetHostIp( ip );
						if( port > 0 )
							input.SetHostPort( port );
						#else
						input.SetHostIp( ip_text );
						if( port > 0 )
							input.SetHostPort( port );
						#endif
					}
				}
				if( m_CharacterAliveFilter.IsSet() )
				{
					//Character filter
				}
				if( m_ThirdPersonFilter.IsSet() )
				{
					input.SetModeIdFilter( m_ThirdPersonFilter.IsEnabled() );
					input.SetThirdPerson( m_ThirdPersonFilter.IsEnabled() );
				}
				if( m_VersionMatchFilter.IsSet() )
				{
					input.SetProperVersionMatch( m_VersionMatchFilter.IsEnabled() );
				}
				if( m_PasswordFilter.IsSet() )
				{
					input.SetIsPasswordProtectedFilter( m_PasswordFilter.IsEnabled() );
				}
				if( m_BattleyeFilter.IsSet() )
				{
					input.SetAntiCheatFilter( m_BattleyeFilter.IsEnabled() );
				}
				if( m_PublicFilter.IsSet() )
				{
					input.SetPublic( m_PublicFilter.IsEnabled() );
				}
				if( m_AcceleratedTimeFilter.IsSet() )
				{
					input.SetAcceleratedTime( m_AcceleratedTimeFilter.IsEnabled() );
				}
			#endif
		#endif
		
		return input;
	}
}