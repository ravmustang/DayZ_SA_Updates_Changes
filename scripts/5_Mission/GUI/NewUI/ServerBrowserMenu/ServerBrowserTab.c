enum TabType
{
	OFFICIAL,
	COMMUNITY,
	LAN,
	NONE
}

enum SelectedPanel
{
	BROWSER,
	FILTERS,
	MENU
}

class ServerBrowserTab extends ScriptedWidgetEventHandler
{
	protected Widget										m_Root;
	protected ScrollWidget									m_ServerListScroller;
	protected SpacerBaseWidget								m_ServerList;
	
	protected ref array<ref GetServersResultRow>			m_Entries;
	
	protected ref map<string, ref ServerBrowserEntry>		m_EntryWidgets;

	protected ref ServerBrowserFilterContainer				m_Filters;
	
	protected ServerBrowserMenuNew							m_Menu;
	protected ServerBrowserEntry							m_SelectedServer;
	
	protected TabType										m_TabType;
	protected ESortType										m_SortType;
	protected ESortOrder									m_SortOrder;
	
	protected SelectedPanel									m_SelectedPanel;
	protected bool											m_Initialized;
	protected bool											m_BegunLoading;
	protected bool											m_Loading;
	protected int											m_TotalServers;
	protected int											m_TotalLoadedServers;
	protected int											m_LastLoadedPage;
	protected int											m_TotalPages;
	
	protected string										m_CurrentSelectedServer;
	protected int											m_CurrentLoadedPage;
	protected ref GetServersInput							m_CurrentFilterInput;
	
	protected Widget										m_ApplyFilter;
	protected Widget										m_RefreshList;
	protected Widget										m_FiltersChanged;
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
		m_ServerList			= SpacerBaseWidget.Cast( m_ServerListScroller.FindAnyWidget( "server_list_content" ) );
		m_ServerListScroller.VScrollToPos01( 0 );
		
		m_Entries				= new array<ref GetServersResultRow>;
		m_EntryWidgets			= new map<string, ref ServerBrowserEntry>;
		m_Menu					= menu;
		m_TabType				= type;
		
		m_ApplyFilter			= m_Root.FindAnyWidget( "apply_filter_button" );
		m_RefreshList			= m_Root.FindAnyWidget( "refresh_list_button" );
		m_FiltersChanged		= m_Root.FindAnyWidget( "unapplied_filters_notify" );
		m_HostSort				= m_Root.FindAnyWidget( "server_list_content_header_host" );
		m_TimeSort				= m_Root.FindAnyWidget( "server_list_content_header_time" );
		m_PopulationSort		= m_Root.FindAnyWidget( "server_list_content_header_population" );
		m_SlotsSort				= m_Root.FindAnyWidget( "server_list_content_header_slots" );
		m_PingSort				= m_Root.FindAnyWidget( "server_list_content_header_ping" );
		m_LoadingText			= TextWidget.Cast( m_Root.FindAnyWidget( "loading_servers_info" ) );
		
		if( type == TabType.LAN )
			m_Root.FindAnyWidget( "filters_content" ).Show( false );
		
		m_Filters				= new ServerBrowserFilterContainer( m_Root.FindAnyWidget( "filters_content" ), this );
		
		SetSort( ESortType.HOST, ESortOrder.DESCENDING );
		
		m_Root.SetHandler( this );
	}
	
	void ~ServerBrowserTab()
	{
		if( m_Filters )
			m_Filters.SaveFilters();
		
		if(m_Root)
			delete m_Root;
	}
	
	void LoadFakeData( int entries )
	{
		ref GetServersResult result = new GetServersResult;
		m_CurrentFilterInput = m_Filters.GetFilterOptions();
		result.m_Page = 1;
		result.m_Pages = 2;
		result.m_Results = new GetServersResultRowArray;
		int i;
		for( i = 0; i < entries; i++ )
		{
			ref GetServersResultRow row = new GetServersResultRow;
			row.m_Id = "#server_browser_tab_id" + i.ToString();
			row.m_Name = "#server_browser_tab_server" + i.ToString();
			row.m_Official = true;
			row.m_MaxPlayers = 10;
			row.m_MinPlayers = 0;
			row.m_CurrentNumberPlayers = 0;
			
			result.m_Results.Insert( row );
		}
		
		m_Menu.SetRefreshing( m_TabType );
		m_Initialized = true;
		m_BegunLoading = false;
		
		m_Entries.Clear();
		m_EntryWidgets.Clear();
		
		m_Loading = true;
		OnLoadServersAsync( result, EBiosError.OK, "" );
		
		ref GetServersResult result2 = new GetServersResult;
		
		result2.m_Page = 2;
		result2.m_Pages = 2;
		result2.m_Results = new GetServersResultRowArray;
		
		for( i = 0; i < entries; i++ )
		{
			ref GetServersResultRow row2 = new GetServersResultRow;
			row2.m_Id = "wtf" + i.ToString();
			row2.m_Name = "wtf" + i.ToString();
			row2.m_Official = true;
			row2.m_MaxPlayers = 10;
			row2.m_MinPlayers = 0;
			row2.m_CurrentNumberPlayers = 0;
			
			result2.m_Results.Insert( row2 );
		}
		OnLoadServersAsync( result2, EBiosError.OK, "" );
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
			
			Widget root			= entry.GetRoot();
			Widget first_child	= root.GetParent().GetChildren();
			Widget last_child	= first_child;
			while( last_child )
			{
				if( last_child.GetSibling() )
					last_child = last_child.GetSibling();
				else
					break;
			}
			
			root.GetParent().Update();
			root.Update();
			
			m_ServerListScroller.GetScreenPos( x, y );
			m_ServerListScroller.GetScreenSize( x_s, y_s );
			
			float bottom_pos = y + y_s;
			
			root.GetScreenPos( x_l, y_l );
			root.GetScreenSize( x_s, y_s );
			
			if( root == first_child )
			{
				m_ServerListScroller.VScrollToPos01( 0 );
			}
			else if( root == last_child )
			{
				m_ServerListScroller.VScrollToPos01( 1 );
			}
			else if( y_l + y_s >= bottom_pos )
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
		if( w )
		{
			return ( w == m_ApplyFilter || w == m_RefreshList );
		}
		return false;
	}
	
	void SetPanelFocus()
	{
		
	}
	
	void PressA()
	{
		
	}
	
	void PressX()
	{
		if( m_Menu.IsRefreshing() == TabType.NONE )
			RefreshList();
	}
	
	void PressY()
	{
		if( m_Menu.IsRefreshing() != TabType.NONE )
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
				if( m_Filters )
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
				SetFocus( focused.GetParent().GetSibling().GetChildren() );
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
	
	void OnFilterChanged()
	{
		m_FiltersChanged.Show( true );
	}
	
	void ApplyFilters()
	{
		m_Filters.SaveFilters();
		m_FiltersChanged.Show( false );
		RefreshList();
	}
	
	void RefreshList()
	{
		m_Menu.SetRefreshing( m_TabType );
		m_Initialized = true;
		m_BegunLoading = false;
		m_LastLoadedPage = 0;
		m_TotalPages = -1;
		m_TotalServers = 0;
		m_TotalLoadedServers = 0;
		m_CurrentLoadedPage = 0;
		
		m_Entries.Clear();
		m_EntryWidgets.Clear();
		
		#ifdef PLATFORM_WINDOWS
		if( !m_CurrentFilterInput )
		#endif
			m_CurrentFilterInput = m_Filters.GetFilterOptions();
		
		m_CurrentFilterInput.m_Page = 0;
		
		#ifdef PLATFORM_CONSOLE
		m_CurrentFilterInput.m_SortBy = GetSortOption();
		m_CurrentFilterInput.m_SortOrder = m_SortOrder;
		#endif
		
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
				m_CurrentFilterInput.SetLAN();
				OnlineServices.LoadServers( m_CurrentFilterInput );
				break;
			}
		}
		m_ServerListScroller.VScrollToPos01( 0 );
	}
	
	void GetNextPage()
	{
		if( m_TotalPages > 0 && m_LastLoadedPage < m_TotalPages )
		{
			//m_CurrentFilterInput.m_Page = m_LastLoadedPage + 1;
			//OnlineServices.LoadServers( m_CurrentFilterInput );
		}
	}
	
	void SelectServer( ServerBrowserEntry server )
	{
		#ifdef PLATFORM_CONSOLE
		ScrollToEntry( server );
		#endif
		
		m_SelectedServer = server;
		
		if (!m_Menu)
			return;
		
		m_Menu.SelectServer( server );
	}
	
	void OnLoadServersAsyncPC( ref GetServersResult result_list, EBiosError error, string response )
	{
		if( result_list )
		{
			if( result_list.m_Results.Count() > 0 )
			{
				foreach( GetServersResultRow result : result_list.m_Results )
				{
					if( PassFilter( result ) )
					{
						m_Entries.Insert( result );
						ref ServerBrowserEntry entry = new ServerBrowserEntry( m_ServerList, m_TotalLoadedServers, this );
						entry.FillInfo( result );
						entry.SetFavorite( m_Menu.IsFavorited( result.m_Id ) );
						m_EntryWidgets.Insert( result.m_Id, entry );
						//AddSorted( result );
						m_TotalLoadedServers++;
						m_LoadingText.SetText( "#server_browser_tab_loaded" + " " + m_EntryWidgets.Count() + " " + "#server_browser_servers_desc" );
					}
					
					if( !m_Menu || m_Menu.IsRefreshing() != m_TabType )
						return;
				}
			}
			else
			{
				m_LoadingText.SetText( "Finished loading " + m_EntryWidgets.Count() + " " + "#server_browser_servers_desc" );
				m_Menu.SetRefreshing( TabType.NONE );
			}
			m_ServerList.Update();

		}
		else
		{
			m_LoadingText.SetText( "Finished loading " + m_EntryWidgets.Count() + " " + "#server_browser_servers_desc" );
			m_Menu.SetRefreshing( TabType.NONE );
		}
	}
	
	void OnLoadServersAsync( ref GetServersResult result_list, EBiosError error, string response )
	{
		if( m_Menu.IsRefreshing() != m_TabType || !result_list )
		{
			m_Menu.SetRefreshing( TabType.NONE );
			string text = "#server_browser_tab_unable_to_get_server";
			if( !result_list )
				text += ( "Error code: " + error );
			else
				text += "#server_browser_tab_no_servers_with_filter";
			m_LoadingText.SetText( text );
			m_Filters.Focus();
			return;
		}
		
		if( result_list.m_Page == 1 )
		{
			m_TotalPages = result_list.m_Pages;
			m_TotalServers = Math.Clamp( ( result_list.m_Pages - 1 ) * SERVER_BROWSER_PAGE_SIZE, 0, 10000000 );
			m_LoadingText.SetText( "#server_browser_tab_loaded" + " " + m_EntryWidgets.Count() + "/" + m_TotalServers + " " +  "#server_browser_servers_desc" );
		}

		m_LastLoadedPage = result_list.m_Page;
		for ( int i = 0; i < result_list.m_Results.Count(); i++ )
		{
			m_Entries.Insert( result_list.m_Results.Get(i) );
		}
		
		if( m_LastLoadedPage == m_TotalPages )
		{
			m_TotalServers += result_list.m_Results.Count();
		}
		
		if( m_TotalPages > m_LastLoadedPage )
		{
			GetNextPage();
		}
		
		if( m_TotalPages > 0 )
		{
			GetGame().GameScript.Call( this, "LoadEntries", new Param2<int, ref GetServersResultRowArray>( result_list.m_Page, result_list.m_Results ) );
		}
	}
	
	bool IsPingInRange( int ping, string max_ping )
	{
		int max = max_ping.Substring( 1, max_ping.Length() - 1 ).ToInt();
		
		if( ping < max )
			return true;
		return false;
	}
		
	bool PassFilter( GetServersResultRow result )
	{	
		if ( !m_Menu || m_Menu.IsRefreshing() != m_TabType )
			return false;
		
		bool is_fav		= m_Menu.IsFavorited( result.m_Id );
		bool is_vis		= g_Game.IsVisited( result.m_HostIp, result.m_HostPort );
		bool is_ping	= IsPingInRange( result.m_Ping, m_Filters.m_PingFilter.GetStringValue() );
		
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
		if( m_Filters.m_PingFilter.IsSet() )
		{
			if( !is_ping )
				return false;
		}
		return true;
	}
	
	void LoadEntries( Param2<int, ref GetServersResultRowArray> page )
	{
		if( !m_Menu || m_Menu.IsRefreshing() != m_TabType )
			return;
		int index = page.param1 * SERVER_BROWSER_PAGE_SIZE;
		ref GetServersResultRowArray page_entries = page.param2;
		if( page_entries )
		{
			foreach( GetServersResultRow result : page_entries )
			{
				if( PassFilter( result ) )
				{
					ref ServerBrowserEntry entry = new ServerBrowserEntry( m_ServerList, index, this );
					entry.FillInfo( result );
					entry.SetFavorite( m_Menu.IsFavorited( result.m_Id ) );
					m_EntryWidgets.Insert( result.m_Id, entry );
					index++;
					m_LoadingText.SetText( "#server_browser_tab_loaded" + " " + m_EntryWidgets.Count() + "/" + m_TotalServers + " " + "#server_browser_servers_desc" );
				}
				
				if( !m_Menu || m_Menu.IsRefreshing() != m_TabType )
					return;
				
				if( index % 10 == 0 )
				{
					Sleep( 0.01 );
				}
			}
			Sleep( 0.1 );
			m_ServerList.Update();
		}
		
		if( m_EntryWidgets.Count() > 0 )
		{
			if( m_Entries.Count() > 0 )
			{
				m_EntryWidgets.Get( m_Entries.Get( 0 ).m_Id ).Focus();
			}
		}
		else
		{
			m_LoadingText.SetText( "#server_browser_unable_with_filter" );
			m_Filters.Focus();
		}
		
		if ( !m_Menu )
			return;
		
		if( m_LastLoadedPage == m_TotalPages )
		{
			m_LoadingText.SetText( "#server_browser_tab_loaded" + " " + m_EntryWidgets.Count() + "/" + m_EntryWidgets.Count() + " " + "#server_browser_servers_desc" );
			m_Menu.SetRefreshing( TabType.NONE );
		}
	}
	
	void Update( float timeslice )
	{
		m_ServerList.Update();
	}
	
	void Connect( ServerBrowserEntry server )
	{
		if( !m_Menu )
			return;
		
		if( m_Menu.IsRefreshing() != TabType.NONE )
			return;
		
		m_SelectedServer = server;
		m_Menu.Connect( server );
	}
	
	void SelectHostSort()
	{
		ESortOrder order = ESortOrder.DESCENDING;
		if( m_SortType == ESortType.HOST )
		{
			order = !m_SortOrder;
			SetSort( ESortType.HOST, order );
			InvertSort();
		}
		else
		{
			SetSort( ESortType.HOST, order );
			Sort();
		}
	}
	
	void SelectTimeSort()
	{
		ESortOrder order = ESortOrder.DESCENDING;
		if( m_SortType == ESortType.TIME )
		{
			order = !m_SortOrder;
			SetSort( ESortType.TIME, order );
			GetGame().GameScript.Call( this, "InvertSort", null );
		}
		else
		{
			SetSort( ESortType.TIME, order );
			GetGame().GameScript.Call( this, "Sort", null );
		}
	}
	
	void SelectPopulationSort()
	{
		ESortOrder order = ESortOrder.DESCENDING;
		if( m_SortType == ESortType.POPULATION )
		{
			order = !m_SortOrder;
			SetSort( ESortType.POPULATION, order );
			GetGame().GameScript.Call( this, "InvertSort", null );
		}
		else
		{
			SetSort( ESortType.POPULATION, order );
			GetGame().GameScript.Call( this, "Sort", null );
		}
	}
	
	void SelectSlotSort()
	{
		ESortOrder order = ESortOrder.DESCENDING;
		if( m_SortType == ESortType.SLOTS )
		{
			order = !m_SortOrder;
			SetSort( ESortType.SLOTS, order );
			GetGame().GameScript.Call( this, "InvertSort", null );
		}
		else
		{
			SetSort( ESortType.SLOTS, order );
			GetGame().GameScript.Call( this, "Sort", null );
		}
	}
	
	void SelectPingSort()
	{
		ESortOrder order = ESortOrder.DESCENDING;
		if( m_SortType == ESortType.PING )
		{
			order = !m_SortOrder;
			SetSort( ESortType.PING, order );
			GetGame().GameScript.Call( this, "InvertSort", null );
		}
		else
		{
			SetSort( ESortType.PING, order );
			GetGame().GameScript.Call( this, "Sort", null );
		}
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
		m_Root.FindAnyWidget( w_name + "_dsc" ).Show( m_SortOrder );
		m_Root.FindAnyWidget( w_name + "_asc" ).Show( !m_SortOrder );
		#endif
	}
	
	void InvertSort()
	{
		if( m_Entries && m_Entries.Count() > 1 )
		{
			reversearray( m_Entries );
			for( int i = 0; i < m_Entries.Count(); i++ )
			{
				if( m_EntryWidgets.Contains( m_Entries.Get( i ).m_Id ) )
				{
					m_EntryWidgets.Get( m_Entries.Get( i ).m_Id ).GetRoot().SetSort( i );
				}
			}
		}
	}
	
	void Sort()
	{
		if( m_Entries && m_Entries.Count() > 1 )
		{
			Sort( m_Entries, 0, m_Entries.Count() - 1 );
			for( int i = 0; i < m_Entries.Count(); i++ )
			{
				if( m_EntryWidgets.Contains( m_Entries.Get( i ).m_Id ) )
				{
					m_EntryWidgets.Get( m_Entries.Get( i ).m_Id ).GetRoot().SetSort( i );
				}
			}
		}
	}
	
	void Sort( array<ref GetServersResultRow> entries, int low, int high )
	{
		array<int> stack = new array<int>;
		int stack_size = 2 * Math.Log2( high - low ) + 10;
  		for( int i = 0; i <= stack_size; i++ )
		{
			stack.Insert( 0 );
		}
		
	    int top = -1; 
	  
	    stack[ ++top ] = low;
	    stack[ ++top ] = high; 
	  
	    while( top >= 0 ) 
	    { 
	        high = stack[ top-- ]; 
	        low = stack[ top-- ]; 
	  
	        int p = Partition( entries, low, high ); 
	  
	        if( ( p - 1 ) > low ) 
	        { 
	            stack[ ++top ] = low; 
	            stack[ ++top ] = p - 1; 
	        } 
	  
	        if( ( p + 1 ) < high ) 
	        { 
	            stack[ ++top ] = p + 1; 
	            stack[ ++top ] = high; 
	        } 
	    }
	}
	
	int Partition( array<ref GetServersResultRow> entries, int low, int high )
	{
		GetServersResultRow pivot = entries.Get( high );
		int i = low - 1;
	
		for( int j = low; j <= high - 1; j++)
		{
	        if( Compare( entries.Get( j ), pivot ) )
	        {
				i++;
				entries.SwapItems( i, j );
			}
		}
		entries.SwapItems( i + 1, high );
		return ( i + 1 );
	}
	
	void InsertSort( array<ref GetServersResultRow> entries )
	{
		int i, j;
		int n = entries.Count();
		
		for (i = 1; i < n; i++) 
		{ 
			GetServersResultRow key = entries[i];
			j = i - 1;
					
			while( j >= 0 && Compare( entries[j], key ) )
			{
				entries[j + 1] = entries[j]; 
				j = j - 1;
			}
			entries[j + 1] = key;
		} 
	}
	
	bool Compare( GetServersResultRow a, GetServersResultRow b )
	{
		GetServersResultRow a1 = a;
		GetServersResultRow b1 = b;
		
		if( m_SortOrder == ESortOrder.ASCENDING )
		{
			a1 = b;
			b1 = a;
		}
		
		switch( m_SortType )
		{
			case ESortType.HOST:
			{
				return a1.m_Name <= b1.m_Name;
			}
			case ESortType.TIME:
			{
				return a1.m_TimeOfDay <= b1.m_TimeOfDay;
			}
			case ESortType.POPULATION:
			{
				return a1.m_CurrentNumberPlayers <= b1.m_CurrentNumberPlayers;
			}
			case ESortType.SLOTS:
			{
				return a1.m_MaxPlayers <= b1.m_MaxPlayers;
			}
			case ESortType.PING:
			{
				return a1.m_Ping <= b1.m_Ping;
			}
		}
		return false;
	}
	
	void AddSorted( Param1<ref GetServersResultRow> entry )
	{
		ref GetServersResultRow row = entry.param1;
		if( m_Entries )
		{
			m_Entries.Insert( row );
			if( m_Entries.Count() > 1 )
				InsertSort( m_Entries );
			int index = m_Entries.Find( row );
			if( index > 0 )
			{
				ServerBrowserEntry curr = m_EntryWidgets.Get( m_Entries.Get( index ).m_Id );
				ServerBrowserEntry prev = m_EntryWidgets.Get( m_Entries.Get( index - 1 ).m_Id );
				if( prev && curr )
				{
					m_ServerList.AddChildAfter( curr.GetRoot(), prev.GetRoot() );
				}
			}
		}
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