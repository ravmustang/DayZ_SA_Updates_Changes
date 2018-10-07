const int MAX_FAVORITES = 50;
const int SERVER_BROWSER_PAGE_SIZE = 50;

class ServerBrowserMenuNew extends UIScriptedMenu
{
	protected Widget				m_Play;
	protected Widget				m_Back;
	protected Widget				m_CustomizeCharacter;
	protected TextWidget			m_PlayerName;
	protected TextWidget			m_Version;
	
	protected TabberUI				m_Tabber;
	protected ref ServerBrowserTab	m_OfficialTab;
	protected ref ServerBrowserTab	m_CommunityTab;
	protected ref ServerBrowserTab	m_LANTab;
	
	protected bool					m_IsRefreshing;
	protected ref TStringArray		m_Favorites;
	protected ServerBrowserEntry	m_SelectedServer;
	
	override Widget Init()
	{
		#ifdef PLATFORM_CONSOLE
			layoutRoot = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/server_browser/xbox/server_browser.layout" );
		#else
		#ifdef PLATFORM_WINDOWS
			layoutRoot = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/server_browser/pc/server_browser.layout" );
		#endif
		#endif
		
		m_OfficialTab	= new ServerBrowserTab( layoutRoot.FindAnyWidget( "Tab_0" ), this, TabType.OFFICIAL );
		#ifndef PLATFORM_CONSOLE
			m_CommunityTab	= new ServerBrowserTab( layoutRoot.FindAnyWidget( "Tab_1" ), this, TabType.COMMUNITY );
			m_LANTab		= new ServerBrowserTab( layoutRoot.FindAnyWidget( "Tab_2" ), this, TabType.LAN );
		#endif
		
		layoutRoot.FindAnyWidget( "Tabber" ).GetScript( m_Tabber );
		
		m_Play					= layoutRoot.FindAnyWidget( "play" );
		m_Back					= layoutRoot.FindAnyWidget( "back_button" );
		m_CustomizeCharacter	= layoutRoot.FindAnyWidget( "customize_character" );
		m_PlayerName			= TextWidget.Cast( layoutRoot.FindAnyWidget( "character_name_text" ) );
		m_Version				= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		
		Refresh();
		
		string version;
		GetGame().GetVersion( version );
		if( version != "" )
			m_Version.SetText( "#main_menu_version" + " " + version );
		else
			m_Version.Show( false );
		
		OnlineServices.m_ServersAsyncInvoker.Insert( OnLoadServersAsync );
		m_Tabber.m_OnTabSwitch.Insert( OnTabSwitch );
		
		LoadData();
		
		m_OfficialTab.RefreshList();
		
		#ifdef PLATFORM_PS4
			ImageWidget toolbar_a = layoutRoot.FindAnyWidget( "ConnectIcon" );
			ImageWidget toolbar_b = layoutRoot.FindAnyWidget( "BackIcon" );
			ImageWidget toolbar_x = layoutRoot.FindAnyWidget( "RefreshIcon" );
			ImageWidget toolbar_y = layoutRoot.FindAnyWidget( "ResetIcon" );
			ImageWidget toolbar_rt = layoutRoot.FindAnyWidget( "SortIcon" );
			toolbar_a.LoadImageFile( 0, "set:playstation_buttons image:cross" );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:circle" );
			toolbar_x.LoadImageFile( 0, "set:playstation_buttons image:square" );
			toolbar_y.LoadImageFile( 0, "set:playstation_buttons image:triangle" );
			toolbar_rt.LoadImageFile( 0, "set:playstation_buttons image:R2" );
		#endif
		
		#ifdef PLATFORM_CONSOLE
			//Sort init
			TextWidget sort_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "SortText" ) );
			sort_text.SetText( "Sort host ASC" );
		#endif
		
		/*
		ref GetServersResult result = new GetServersResult;
		
		result.m_Page = 1;
		result.m_Pages = 1;
		result.m_Results = new GetServersResultRowArray;
		
		for( int i = 0; i < 50; i++ )
		{
			ref GetServersResultRow row = new GetServersResultRow;
			row.m_Id = "id" + i.ToString();
			row.m_Name = "Server " + i.ToString();
			row.m_Official = true;
			row.m_MaxPlayers = 10;
			row.m_MinPlayers = 0;
			row.m_CurrentNumberPlayers = 0;
			
			result.m_Results.Insert( row );
		}
		SetRefreshing( true );
		m_OfficialTab.m_Initialized = true;
		m_OfficialTab.m_BegunLoading = false;
		
		m_OfficialTab.m_Entries.Clear();
		m_OfficialTab.m_Pages.Clear();
		m_OfficialTab.m_EntryWidgets.Clear();
		
		m_OfficialTab.m_Loading = true;
		m_OfficialTab.OnLoadServersAsync( result, EBiosError.OK, "" );
		*/
		
		PPEffects.SetBlurMenu( 0.5 );
		return layoutRoot;
	}
	
	void ~ServerBrowserMenuNew()
	{
		SaveData();
		OnlineServices.m_ServersAsyncInvoker.Remove( OnLoadServersAsync );
		m_Tabber.m_OnTabSwitch.Remove( OnTabSwitch );
		PPEffects.SetBlurMenu( 0.0 );
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_Play )
			{
				Play();
				return true;
			}
			else if ( w == m_Back )
			{
				ReturnMainMenu();
				return true;
			}
			else if ( w == m_CustomizeCharacter )
			{
				CustomizeCharacter();
				return true;
			}
		}
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w, x, y );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, enterW, x, y );
			return true;
		}
		return false;
	}
	
	void SetRefreshing( bool refreshing )
	{
		m_IsRefreshing = refreshing;
	}

	bool IsRefreshing()
	{
		return m_IsRefreshing;
	}

	bool IsFavorited( string uid )
	{
		int index = m_Favorites.Find( uid );
		return ( index >= 0 );
	}
	
	void SetFavorite( string uid, bool favorite )
	{
		if( m_Favorites )
		{
			if( favorite && m_Favorites.Find( uid ) < 0 )
			{
				if( m_Favorites.Count() < MAX_FAVORITES )
					m_Favorites.Insert( uid );
				else
				{
					m_OfficialTab.Unfavorite( m_Favorites.Get( 0 ) );
					#ifndef PLATFORM_CONSOLE
					m_CommunityTab.Unfavorite( m_Favorites.Get( 0 ) );
					m_LANTab.Unfavorite( m_Favorites.Get( 0 ) );
					#endif
					m_Favorites.Remove( 0 );
					m_Favorites.Insert( uid );
				}
			}
			else if ( m_Favorites.Find( uid ) >= 0 )
			{
				m_Favorites.RemoveItem( uid );
				m_OfficialTab.Unfavorite( uid );
				#ifndef PLATFORM_CONSOLE
				m_CommunityTab.Unfavorite( uid );
				m_LANTab.Unfavorite( uid );
				#endif
			}
		}
	}
	
	void FilterFocus( bool focus )
	{
		#ifdef PLATFORM_CONSOLE
		TextWidget con_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "ConnectText" ) );
		TextWidget ref_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "RefreshText" ) );
		TextWidget res_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "ResetText" ) );
		Widget reset_frame	= layoutRoot.FindAnyWidget( "Reset" );
		
		if( focus )
		{
			con_text.SetText( "Toggle" );
			ref_text.SetText( "Refresh Servers" );
			res_text.SetText( "Reset Filters" );
		}
		#endif
	}
	
	void ServerListFocus( bool focus, bool favorite )
	{
		#ifdef PLATFORM_CONSOLE
		TextWidget con_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "ConnectText" ) );
		TextWidget ref_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "RefreshText" ) );
		TextWidget res_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "ResetText" ) );
		Widget reset_frame	= layoutRoot.FindAnyWidget( "Reset" );
		
		if( focus )
		{
			con_text.SetText( "Connect" );
			
			float x, y;
			res_text.GetSize( x, y );
			if( favorite )
			{
				res_text.SetText( "Unfavorite" );
			}
			else
			{
				res_text.SetText( "Favorite" );
			}
		}
		#endif
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w, x, y );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, null, x, y );
			return true;
		}
		return false;
	}
	
	override void Refresh()
	{
		string name;
		
		#ifdef PLATFORM_CONSOLE
			if( GetGame().GetUserManager() && GetGame().GetUserManager().GetSelectedUser() )
			{
				name = GetGame().GetUserManager().GetSelectedUser().GetName();
				if( name.LengthUtf8() > 18 )
				{
					name = name.SubstringUtf8(0, 18);
					name += "...";
				}
			}
		#else
			g_Game.GetPlayerNameShort( 14, name );
		#endif
		
		if( m_PlayerName )
			m_PlayerName.SetText( name );
		
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		#else
			version = "#main_menu_version" + " " + version;
		#endif
		m_Version.SetText( version );
	}
	
	override void Update( float timeslice )
	{
		if( GetGame().GetInput().GetActionDown( UAUITabLeft, false ) )
		{
			m_Tabber.PreviousTab();
		}
		
		//RIGHT BUMPER - TAB RIGHT
		if( GetGame().GetInput().GetActionDown( UAUITabRight, false ) )
		{
			m_Tabber.NextTab();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUISelect, false ) )
		{
			GetSelectedTab().PressA();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIFastEquipOrSplit, false ) )
		{
			GetSelectedTab().PressX();
		}
		
		if( GetGame().GetInput().GetActionDown( UAQuickReload, false ) )
		{
			GetSelectedTab().PressY();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUINextDown, false ) )
		{
			
		}		
		
		if( GetGame().GetInput().GetActionDown( UAUILeft, false ) )
		{
			GetSelectedTab().Left();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIRight, false ) )
		{
			GetSelectedTab().Right();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIUp, false ) )
		{
			GetSelectedTab().Up();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIDown, false ) )
		{
			GetSelectedTab().Down();
		}
		
		GetSelectedTab().Update( timeslice );
		
		super.Update( timeslice );
	}
	
	bool IsFocusable( Widget w )
	{
		return ( w == m_Play || w == m_CustomizeCharacter || w == m_Back );
	}
	
	void LoadData()
	{
		m_Favorites = new TStringArray;
		GetGame().GetProfileStringList( "SB_Favorites", m_Favorites );
	}
	
	void SaveData()
	{
		GetGame().SetProfileStringList( "SB_Favorites", m_Favorites );
		GetGame().SaveProfile();
	}
	
	void SelectServer( ServerBrowserEntry server )
	{
		if( m_SelectedServer )
			m_SelectedServer.Deselect();
		m_SelectedServer = server;
	}
	
	void Connect( ServerBrowserEntry server )
	{
		SelectServer( server );
		SaveData();
		Play();
	}
	
	void Play()
	{
		if( m_SelectedServer )
		{
			g_Game.ConnectFromServerBrowser( m_SelectedServer.GetIP(), m_SelectedServer.GetPort(), "" );
		}
	}

	void ReturnMainMenu()
	{
		SaveData();
		GetGame().GetUIManager().Back();
	}
	
	void CustomizeCharacter()
	{
		EnterScriptedMenu(MENU_CHARACTER);
	}
	
	void NextCharacter()
	{
		
	}
	
	void PreviousCharacter()
	{
		
	}
	
	ServerBrowserTab GetSelectedTab()
	{
		switch( m_Tabber.GetSelectedIndex() )
		{
			case 0:
			{
				return m_OfficialTab;
			}
			case 1:
			{
				return m_CommunityTab;
			}
			case 2:
			{
				return m_LANTab;
			}
		}
		return null;
	}
	
	void OnTabSwitch()
	{
		if( GetSelectedTab().IsNotInitialized() )
			GetSelectedTab().RefreshList();
		GetSelectedTab().Focus();
	}
	
	void OnLoadServersAsync( ref GetServersResult result_list, EBiosError error, string response )
	{
		GetSelectedTab().OnLoadServersAsync( result_list, error, response );
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w, int x, int y )
	{
		SetFocus( w );
		
		ButtonWidget button = ButtonWidget.Cast( w );
		if( button && button != m_Play )
		{
			button.SetTextColor( ARGB( 255, 200, 0, 0 ) );
		}
		
		TextWidget text		= TextWidget.Cast(w.FindWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindWidget( w.GetName() + "_text_1" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_image" ) );
		
		if( text )
		{
			text.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if( text2 )
		{
			text2.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if( image )
		{
			image.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
	}
	
	void ColorWhite( Widget w, Widget enterW, int x, int y )
	{
		#ifdef PLATFORM_WINDOWS
		SetFocus( null );
		#endif
		
		ButtonWidget button = ButtonWidget.Cast( w );
		if( button && button != m_Play )
		{
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		TextWidget text		= TextWidget.Cast(w.FindWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindWidget( w.GetName() + "_text_1" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_image" ) );
		
		if( text )
		{
			text.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if( text2 )
		{
			text2.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if( image )
		{
			image.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
	}
}