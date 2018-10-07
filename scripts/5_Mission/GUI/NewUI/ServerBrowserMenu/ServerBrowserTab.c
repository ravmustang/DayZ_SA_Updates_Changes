enum TabType
{
	OFFICIAL,
	COMMUNITY,
	LAN
}

enum SelectedPanel
{
	BROWSER,
	FILTERS,
	MENU
}

class ServerBrowserPage
{
	Widget m_Root;
	
	void ServerBrowserPage( int index, Widget parent )
	{
		m_Root		= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/server_browser/server_browser_list_page.layout", parent );
		m_Root.SetName( "page_" + index );
		m_Root.SetSort( index );
	}
	
	void ~ServerBrowserPage()
	{
		delete m_Root;
	}
	
	void Show()
	{
		m_Root.Show( true );
	}
	
	void Hide()
	{
		if (!m_Root)
			return;
		
		m_Root.Show( false );
	}
	
	float GetYPos()
	{
		float x, y;
		m_Root.GetScreenPos( x, y );
		return y;
	}
}

class ServerBrowserTab extends ScriptedWidgetEventHandler
{
	protected Widget										m_Root;
	protected ScrollWidget									m_ServerListScroller;
	protected Widget										m_ServerList;
	
	 ref map<int, ref GetServersResultRowArray>	m_Entries;
	
	 ref array<ref ServerBrowserPage>				m_Pages;
	 ref map<string, ref ServerBrowserEntry>				m_EntryWidgets;

	protected ref ServerBrowserFilterContainer				m_Filters;
	
	protected ServerBrowserMenuNew							m_Menu;
	protected ServerBrowserEntry							m_SelectedServer;
	
	protected TabType										m_TabType;
	protected ESortType										m_SortType;
	protected ESortOrder									m_SortOrder;
	
	protected SelectedPanel									m_SelectedPanel;
	 bool											m_Initialized;
	 bool											m_BegunLoading;
	 bool											m_Loading;
	protected int											m_TotalServers;
	protected int											m_TotalLoadedServers;
	protected int											m_LastLoadedPage;
	protected int											m_TotalPages;
	
	protected string										m_CurrentSelectedServer;
	protected int											m_CurrentLoadedPage;
	protected ref GetServersInput							m_CurrentFilterInput;
	
	protected Widget										m_ApplyFilter;
	protected Widget										m_RefreshList;
	protected Widget										m_HostSort;
	protected Widget										m_TimeSort;
	protected Widget										m_PopulationSort;
	protected Widget										m_SlotsSort;
	protected Widget										m_PingSort;
	protected TextWidget									m_LoadingText;
	
	void ServerBrowserTab( Widget parent, ServerBrowserMenuNew menu, TabType type )
	{
		#ifdef PLATFORM_CONSOLE
			m_Root					= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/server_browser/xbox/server_browser_tab.layout", parent );
		#else
		#ifdef PLATFORM_WINDOWS
			m_Root					= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/server_browser/pc/server_browser_tab.layout", parent );
		#endif
		#endif
		
		m_ServerListScroller	= ScrollWidget.Cast( m_Root.FindAnyWidget( "server_list_scroller" ) );
		m_ServerList			= m_ServerListScroller.FindAnyWidget( "server_list_content" );
		m_ServerListScroller.VScrollToPos01( 0 );
		
		m_Entries				= new map<int, ref GetServersResultRowArray>;
		m_Pages					= new array<ref ServerBrowserPage>;
		m_EntryWidgets			= new map<string, ref ServerBrowserEntry>;
		m_Menu					= menu;
		m_TabType				= type;
		
		m_Filters				= new ServerBrowserFilterContainer( m_Root.FindAnyWidget( "filters_content" ), this );
		m_ApplyFilter			= m_Root.FindAnyWidget( "apply_filter_button" );
		m_RefreshList			= m_Root.FindAnyWidget( "refresh_list_button" );
		m_HostSort				= m_Root.FindAnyWidget( "server_list_content_header_host" );
		m_TimeSort				= m_Root.FindAnyWidget( "server_list_content_header_time" );
		m_PopulationSort		= m_Root.FindAnyWidget( "server_list_content_header_population" );
		m_SlotsSort				= m_Root.FindAnyWidget( "server_list_content_header_slots" );
		m_PingSort				= m_Root.FindAnyWidget( "server_list_content_header_ping" );
		m_LoadingText			= TextWidget.Cast( m_Root.FindAnyWidget( "loading_servers_info" ) );
		
		m_Root.SetHandler( this );
	}
	
	void ~ServerBrowserTab()
	{
		if (m_Filters)
			m_Filters.SaveFilters();
		
		if (m_Root)
			delete m_Root;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_ApplyFilter )
			{
				ApplyFilters();
				return true;
			}
			else if ( w == m_RefreshList )
			{
				RefreshList();
				return true;
			}
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if ( w == m_HostSort )
			{
				SelectHostSort();
				return true;
			}
			else if ( w == m_TimeSort )
			{
				SelectTimeSort();
				return true;
			}
			else if ( w == m_PopulationSort )
			{
				SelectPopulationSort();
				return true;
			}
			else if ( w == m_SlotsSort )
			{
				SelectSlotSort();
				return true;
			}
			else if ( w == m_PingSort )
			{
				SelectPingSort();
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
	
	void ScrollToEntry( ServerBrowserEntry entry )
	{
		if( entry )
		{
			float x, y;
			float x_s, y_s;
			float x_l, y_l;
			
			Widget root = entry.GetRoot();
			
			root.GetParent().Update();
			root.Update();
			
			m_ServerListScroller.GetScreenPos( x, y );
			m_ServerListScroller.GetScreenSize( x_s, y_s );
			
			float bottom_pos = y + y_s;
			
			root.GetScreenPos( x_l, y_l );
			root.GetScreenSize( x_s, y_s );
			
			if( y_l + y_s >= bottom_pos )
			{
				m_ServerListScroller.VScrollToPos( m_ServerListScroller.GetVScrollPos() + y_s );
			}
			else if( y_l <= y )
			{
				m_ServerListScroller.VScrollToPos( m_ServerListScroller.GetVScrollPos() - y_s );
			}
		}
	}
	
	void Focus()
	{
		if( m_EntryWidgets.Contains( m_CurrentSelectedServer ) )
		{
			m_EntryWidgets.Get( m_CurrentSelectedServer ).Focus();
			ScrollToEntry( m_EntryWidgets.Get( m_CurrentSelectedServer ) );
		}
	}
	
	void FilterFocus( bool focus )
	{
		m_SelectedPanel = SelectedPanel.FILTERS;
		m_Menu.FilterFocus( focus );
	}
	
	void ServerListFocus( bool focus, bool favorite )
	{
		m_SelectedPanel = SelectedPanel.BROWSER;
		
		if (!m_Menu)
			return;
		
		m_Menu.ServerListFocus( focus, favorite );
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
	
	bool IsFocusable( Widget w )
	{
		return ( w == m_ApplyFilter || w == m_RefreshList );
	}
	
	void SetPanelFocus()
	{
		
	}
	
	void PressA()
	{
		
	}
	
	void PressX()
	{
		if( !m_Menu.IsRefreshing() )
			RefreshList();
	}
	
	void PressY()
	{
		if(m_Menu.IsRefreshing() )
			return;

		switch( m_SelectedPanel )
		{
			case SelectedPanel.BROWSER:
			{
				if( m_SelectedServer )
				{
					m_Menu.ServerListFocus( true, m_SelectedServer.ToggleFavorite() );
				}
				break;
			}
			case SelectedPanel.FILTERS:
			{
				m_Filters.ResetFilters();
				break;
			}
		}
	}
	
	void Left()
	{
		
	}
	
	void Right()
	{
		
	}
	
	void Up()
	{
		
	}
	
	void Down()
	{
		
	}
	
	void GetNextEntry()
	{
		Widget focused = GetFocus();
		if( !focused )
			return;
		Widget sibling = focused.GetSibling();
		if( !sibling )
			return;
		
		if( focused.GetName() == "server_browser_list_entry_root" )
		{
			if( sibling )
				SetFocus( focused.GetSibling() );
			else
				SetFocus( focused.GetParent().GetChildren() );
		}
	}
	
	void GetPrevEntry()
	{
		Widget focused = GetFocus();
		if( focused && focused.GetName() == "server_browser_list_entry_root" )
		{
			Widget sibling = focused.GetParent().GetChildren();
			if( focused == sibling )
				return;
			
			while( sibling )
			{
				if( sibling && sibling.GetSibling() == focused )
				{
					SetFocus( sibling );
				}
				else
				{
					sibling = sibling.GetSibling();
				}
			}
		}
	}
	
	void GetNextFilter()
	{
	
	}
	
	void GetPrevFilter()
	{
	
	}
	
	bool IsNotInitialized()
	{
		return !m_Initialized;
	}
	
	void ApplyFilters()
	{
		m_Filters.SaveFilters();
		RefreshList();
	}
	
	void RefreshList()
	{
		m_Menu.SetRefreshing( true );
		m_Initialized = true;
		m_BegunLoading = false;
		m_LastLoadedPage = 0;
		m_TotalPages = -1;
		m_TotalServers = 0;
		m_TotalLoadedServers = 0;
		m_CurrentLoadedPage = 0;
		m_Entries.Clear();
		m_Pages.Clear();
		m_EntryWidgets.Clear();
		
		m_CurrentFilterInput = m_Filters.GetFilterOptions();
		m_CurrentFilterInput.m_Page = 0;
		m_CurrentFilterInput.m_SortBy = GetSortOption();
		m_CurrentFilterInput.m_SortOrder = m_SortOrder;
		
		m_Loading = true;
		switch( m_TabType )
		{
			case TabType.OFFICIAL:
			{
				m_CurrentFilterInput.SetOfficialFilter( true );
				OnlineServices.LoadServers( m_CurrentFilterInput );
				break;
			}
			case TabType.COMMUNITY:
			{
				m_CurrentFilterInput.SetOfficialFilter( false );
				OnlineServices.LoadServers( m_CurrentFilterInput );
				break;
			}
			case TabType.LAN:
			{
				
				break;
			}
		}
		m_ServerListScroller.VScrollToPos01( 0 );
	}
	
	void GetNextPage()
	{
		if( m_TotalPages > 0 && m_LastLoadedPage < m_TotalPages )
		{
			m_CurrentFilterInput.m_Page = m_LastLoadedPage + 1;
			OnlineServices.LoadServers( m_CurrentFilterInput );
		}
	}
	
	void SelectServer( ServerBrowserEntry server )
	{
		/*
		#ifdef PLATFORM_CONSOLE
			if( m_SelectedServer && m_EntryWidgets.Contains( m_SelectedServer.GetServerID() ) )
			{
				//m_SelectedServer.Lighten( m_SelectedServer.GetRoot(), server.GetRoot(), 0, 0 );
				//m_SelectedServer.Select( false );
				//m_SelectedServer.ServerListFocus( true );
				return;
			}
			else
			{
				ScrollToEntry( server );
			}
		#endif
		*/
		ScrollToEntry( server );
		m_SelectedServer = server;
		
		if (!m_Menu)
			return;
		
		m_Menu.SelectServer( server );
	}
	
	void OnLoadServersAsync( ref GetServersResult result_list, EBiosError error, string response )
	{
		if( !result_list || result_list.m_Pages == 0 )
		{
			m_Menu.SetRefreshing( false );
			string text = "Unable to get servers, ";
			if( !result_list )
				text += ( "Error code: " + error );
			else
				text += "there are no servers available with your current filter settings.";
			m_LoadingText.SetText( text );
			#ifdef PLATFORM_XBOX
				m_Filters.Focus();
			#endif
			return;
		}
		
		if( result_list.m_Page == 1 )
		{
			m_TotalPages = result_list.m_Pages;
			m_TotalServers = Math.Clamp( ( result_list.m_Pages - 1 ) * SERVER_BROWSER_PAGE_SIZE, 0, 10000000 );
			for( int i = 1; i <= m_TotalPages; i++ )
			{
				m_Pages.Insert( new ServerBrowserPage( i, m_ServerList ) );
			}
			m_LoadingText.SetText( "Loaded " + m_TotalLoadedServers + "/" + m_TotalServers + " servers" );
		}
		
		m_LastLoadedPage = result_list.m_Page;
		m_Entries.Insert( result_list.m_Page, result_list.m_Results );
		
		if( m_LastLoadedPage == m_TotalPages )
		{
			m_TotalServers += result_list.m_Results.Count();
		}
		if( m_TotalPages > m_LastLoadedPage )
			GetNextPage();
		
		if( !m_BegunLoading && m_TotalPages > 0 )
		{
			m_BegunLoading = true;
			GetGame().GameScript.Call( this, "LoadEntries", new Param2<int, int>( 1, m_TotalPages ) );
		}
	}
	
	bool PassFilter( GetServersResultRow result )
	{	
		if (!m_Menu || !m_Menu.IsRefreshing())
			return false;
		
		bool is_fav = m_Menu.IsFavorited( result.m_Id );
		bool is_vis = g_Game.IsVisited( result.m_HostIp, result.m_HostPort );
		
		if( m_Filters.m_FavoritedFilter.IsSet() )
		{
			if( is_fav != m_Filters.m_FavoritedFilter.IsEnabled() )
				return false;
		}
		if( m_Filters.m_PreviouslyPlayedFilter.IsSet() )
		{
			if( is_vis != m_Filters.m_PreviouslyPlayedFilter.IsEnabled() )
				return false;
		}
		return true;
	}
	
	void LoadEntries( Param2<int, int> pages_to_load )
	{
		int start_page	= Math.Clamp( pages_to_load.param1, 1, pages_to_load.param2 );
		int end_page	= pages_to_load.param2;
		
		for( int i = start_page; i <= end_page; i )
		{
			if( !m_Menu || !m_Menu.IsRefreshing() )
				return;
			ref GetServersResultRowArray results = m_Entries.Get( i );
			int index;
			if( results )
			{
				foreach( GetServersResultRow result : results )
				{
					if( PassFilter( result ) )
					{
						ref ServerBrowserEntry entry = new ServerBrowserEntry( m_ServerList.FindAnyWidget( "page_" + i ), index, this );
						entry.FillInfo( result );
						entry.SetFavorite( m_Menu.IsFavorited( result.m_Id ) );
						m_EntryWidgets.Insert( result.m_Id, entry );
						index++;
						m_TotalLoadedServers++;
						m_LoadingText.SetText( "Loaded " + m_TotalLoadedServers + "/" + m_TotalServers + " servers" );
					}
					
					if( !m_Menu || !m_Menu.IsRefreshing() )
						return;
					
					if( index % 10 == 0 )
					{
						Sleep( 0.01 );
					}
				}
				
				m_ServerList.FindAnyWidget( "page_" + i ).Update();
				if( i > 2 )
					m_Pages.Get( i - 1 ).Hide();
				m_ServerList.Update();
				i++;
			}
			else
			{
				if( !m_Menu )
					return;
				Sleep( 0.1 );
			}
		}
		
		if( m_EntryWidgets.Count() > 0 )
		{
			if( m_Entries.Count() > 0 )
			{
				GetServersResultRowArray res = m_Entries.GetElement( 0 );
				if( res && res.Count() > 0 )
					m_EntryWidgets.Get( res.Get( 0 ).m_Id ).Focus();
			}
		}
		else
		{
			m_LoadingText.SetText( "Unable to get servers, there are no servers available with your current filter settings." );
			m_Filters.Focus();
		}
		
		if (!m_Menu)
			return;
		m_Menu.SetRefreshing( false );
	}
	
	void Update( float timeslice )
	{
		if( m_TotalPages > 0 )
		{
			float pos = m_ServerListScroller.GetVScrollPos01();
			float next_page_pos = ( m_CurrentLoadedPage + 1 ) * ( 1 / m_TotalPages );
			float prev_page_pos = ( m_CurrentLoadedPage ) * ( 1 / m_TotalPages );
			
			if( pos > next_page_pos )
			{
				if( m_CurrentLoadedPage < m_TotalPages - 1 )
				{
					m_CurrentLoadedPage++;
					if( m_CurrentLoadedPage < m_TotalPages - 1 )
						m_Pages.Get( m_CurrentLoadedPage + 1 ).Show();
					for( int i = m_CurrentLoadedPage - 4; i >= 0; i-- )
						m_Pages.Get( i ).Hide();
				}
				m_ServerList.Update();
			}
			
			if( pos <= prev_page_pos )
			{
				if( m_CurrentLoadedPage > 1 )
				{
					m_CurrentLoadedPage--;
					m_Pages.Get( m_CurrentLoadedPage - 1 ).Show();
					if( m_CurrentLoadedPage < m_TotalPages - 2 )
						m_Pages.Get( m_CurrentLoadedPage + 2 ).Hide();
				}
				m_ServerList.Update();
			}
		}
	}
	
	void Connect( ServerBrowserEntry server )
	{
		if (!m_Menu)
			return;
		
		if (m_Menu.IsRefreshing())
			return;
		
		m_SelectedServer = server;
		m_Menu.Connect( server );
	}
	
	void SelectHostSort()
	{
		ESortOrder order;
		if( m_SortType == ESortType.HOST )
			order = !m_SortOrder;
		SetSort( ESortType.HOST, order );
	}
	
	void SelectTimeSort()
	{
		ESortOrder order;
		if( m_SortType == ESortType.TIME )
			order = !m_SortOrder;
		SetSort( ESortType.TIME, order );
	}
	
	void SelectPopulationSort()
	{
		ESortOrder order;
		if( m_SortType == ESortType.POPULATION )
			order = !m_SortOrder;
		SetSort( ESortType.POPULATION, order );
	}
	
	void SelectSlotSort()
	{
		ESortOrder order;
		if( m_SortType == ESortType.SLOTS )
			order = !m_SortOrder;
		SetSort( ESortType.SLOTS, order );
	}
	
	void SelectPingSort()
	{
		ESortOrder order;
		if( m_SortType == ESortType.PING )
			order = !m_SortOrder;
		SetSort( ESortType.PING, order );
	}
	
	string GetSortOption()
	{
		switch( m_SortType )
		{
			case ESortType.HOST:
			{
				return "name";
			}
			case ESortType.TIME:
			{
				return "name";
			}
			case ESortType.POPULATION:
			{
				return "currentNumberPlayers";
			}
			case ESortType.SLOTS:
			{
				return "freeSlots";
			}
			case ESortType.PING:
			{
				return "name";
			}
		}
		return "";
	}
	
	void SetSort( ESortType type, ESortOrder order )
	{
		m_SortType	= type;
		m_SortOrder = order;

		#ifdef PLATFORM_WINDOWS
		m_Root.FindAnyWidget( "host_sort" ).Show( false );
		m_Root.FindAnyWidget( "population_sort" ).Show( false );
		m_Root.FindAnyWidget( "slots_sort" ).Show( false );
		
		m_Root.FindAnyWidget( "time_sort" ).Show( false );
		m_Root.FindAnyWidget( "ping_sort" ).Show( false );
		
		TextWidget root;
		root = TextWidget.Cast( m_Root.FindAnyWidget( "host_label" ) );
		root.SetColor( ARGBF( 1, 1, 1, 1 ) );
		root = TextWidget.Cast( m_Root.FindAnyWidget( "population_label" ) );
		root.SetColor( ARGBF( 1, 1, 1, 1 ) );
		root = TextWidget.Cast( m_Root.FindAnyWidget( "slots_label" ) );
		root.SetColor( ARGBF( 1, 1, 1, 1 ) );
		
		root = TextWidget.Cast( m_Root.FindAnyWidget( "ping_label" ) );
		root.SetColor( ARGBF( 1, 1, 1, 1 ) );
		root = TextWidget.Cast( m_Root.FindAnyWidget( "time_label" ) );
		root.SetColor( ARGBF( 1, 1, 1, 1 ) );
		
		string r_name;
		string w_name;
		
		switch( type )
		{
			case ESortType.HOST:
			{
				r_name = "host_label";
				w_name = "host_sort";
				break;
			}
			case ESortType.TIME:
			{
				r_name = "time_label";
				w_name = "time_sort";
				break;
			}
			case ESortType.POPULATION:
			{
				r_name = "population_label";
				w_name = "population_sort";
				break;
			}
			case ESortType.SLOTS:
			{
				r_name = "slots_label";
				w_name = "slots_sort";
				break;
			}
			case ESortType.PING:
			{
				r_name = "ping_label";
				w_name = "ping_sort";
				break;
			}
		}
		
		root = TextWidget.Cast( m_Root.FindAnyWidget( r_name ) );
		root.SetColor( ARGBF( 1, 1, 0, 0 ) );
		
		m_Root.FindAnyWidget( w_name ).Show( true );
		m_Root.FindAnyWidget( w_name + "_dsc" ).Show( !m_SortOrder );
		m_Root.FindAnyWidget( w_name + "_asc" ).Show( m_SortOrder );
		
		#endif
	}
	
	void SetFavorite( string uid, bool favorite )
	{
		m_Menu.SetFavorite( uid, favorite );
	}
	
	void Unfavorite( string uid )
	{
		ServerBrowserEntry entry;
		if( m_EntryWidgets.Find( uid, entry ) )
		{
			entry.SetFavorite( false );
		}
	}
	
	TabType GetTabType()
	{
		return m_TabType;
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w, int x, int y )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 0, 0 ) );
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
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
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