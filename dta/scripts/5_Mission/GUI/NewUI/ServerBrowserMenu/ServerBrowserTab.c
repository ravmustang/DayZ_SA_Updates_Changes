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
	
	//protected ref array<ref GetServersResultRow>			m_Entries;
	
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
		Construct(parent, menu, type);
	}
	
	protected void Construct( Widget parent, ServerBrowserMenuNew menu, TabType type )
	{
		
		
		/*
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
		
		SetSort( ESortType.HOST, ESortOrder.ASCENDING );
		
		m_Root.SetHandler( this );
		*/
	}
	
	void ~ServerBrowserTab()
	{
		if( m_Filters )
			m_Filters.SaveFilters();
		
		if(m_Root)
			delete m_Root;
	}
	
	/*
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
	*/
	
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
			ColorHighlight( w );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorNormal( w );
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
	
	void ResetFilters()
	{
		m_Filters.ResetFilters();
	}
	
	void ApplyFilters()
	{
		m_Filters.SaveFilters();
		m_FiltersChanged.Show( false );
		m_CurrentFilterInput = m_Filters.GetFilterOptions();		
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
		
		//m_Entries.Clear();
		m_EntryWidgets.Clear();
		
		#ifdef PLATFORM_WINDOWS
		//if( !m_CurrentFilterInput )
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
		//m_ServerListScroller.VScrollToPos01( 0 );
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
		/*
		if( result_list )
		{
			if( result_list.m_Results.Count() > 0 )
			{
				foreach( GetServersResultRow result : result_list.m_Results )
				{
					if( PassFilter( result ) )
					{
						ref ServerBrowserEntry entry = new ServerBrowserEntry( null, m_TotalLoadedServers, this );
						
						entry.FillInfo( result );
						entry.SetFavorite( m_Menu.IsFavorited( result.m_Id ) );
						m_EntryWidgets.Insert( result.m_Id, entry );
						AddSorted( result );
						m_TotalLoadedServers++;
						m_LoadingText.SetText( "#server_browser_tab_loaded" + " " + m_EntryWidgets.Count() + " " + "#server_browser_servers_desc" );
					}
					
					if( !m_Menu || m_Menu.IsRefreshing() != m_TabType )
						return;
				}
			}
			else
			{
				m_LoadingText.SetText( "#server_browser_tab_finished_loading" + " " + m_EntryWidgets.Count() + " " + "#server_browser_servers_desc" );
				m_Menu.SetRefreshing( TabType.NONE );
			}
			//m_ServerList.Update();
		}
		else
		{
			m_LoadingText.SetText( "#server_browser_tab_finished_loading" + " " + m_EntryWidgets.Count() + " " + "#server_browser_servers_desc" );
			m_Menu.SetRefreshing( TabType.NONE );
		}
		*/
	}
	
	void OnLoadServersAsync( ref GetServersResult result_list, EBiosError error, string response )
	{
		/*
		if( m_Menu.IsRefreshing() != m_TabType || !result_list || ( !result_list.m_Results || result_list.m_Results.Count() == 0 ) )
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
		*/
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
		bool pass = true;
		
		if ( !m_Menu || m_Menu.IsRefreshing() != m_TabType )
		{
			return false;
		}
		
		if( pass && m_Filters.PingIsSet() )
		{
			pass = IsPingInRange( result.m_Ping, m_Filters.m_PingFilter.GetStringValue() );
		}
		
		if( pass && m_Filters.FavoriteIsSet() )
		{
			bool is_fav = m_Menu.IsFavorited( result.m_Id );
			
			if( is_fav != m_Filters.m_FavoritedFilter.IsEnabled() )
			{
				pass = false;
			}
		}
		
		if( pass && m_Filters.PreviouslyIsSet() )
		{
			bool is_vis	= g_Game.IsVisited( result.m_HostIp, result.m_HostPort );
			
			if ( is_vis )
			{
				Print(result.m_HostIp +":"+ result.m_HostPort);
			}
			
			if( is_vis != m_Filters.m_PreviouslyPlayedFilter.IsEnabled() )
			{
				pass = false;
			}
		}
		
		return pass;
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
	
	int AddSorted( GetServersResultRow entry )
	{	
		return -1;
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
	
	void ButtonEnable( Widget w )
	{
		w.ClearFlags( WidgetFlags.IGNOREPOINTER );
		ColorNormal(w);
	}
	
	void ButtonDisable( Widget w )
	{
		w.SetFlags( WidgetFlags.IGNOREPOINTER );
		ColorDisable(w);
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorHighlight( Widget w )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 200, 0, 0 ) );
		}
		
		w.SetColor( ARGB( 255, 0, 0, 0) );
		
		TextWidget text1	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_label" ) );
		TextWidget text3	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_text_1" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindAnyWidget( w.GetName() + "_image" ) );
		
		Widget option	= Widget.Cast( w.FindAnyWidget( w.GetName() + "_option_wrapper" ) );
		
		if( text1 )
		{
			text1.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if( text2 )
		{
			text2.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if( text3 )
		{
			text3.SetColor( ARGB( 255, 255, 0, 0 ) );
			w.SetAlpha(1);
		}
		
		if( image )
		{
			image.SetColor( ARGB( 255, 200, 0, 0 ) );
		}
		
		if ( option )
		{
			option.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
	}
	
	void ColorNormal( Widget w )
	{
		//Print("ColorNormal -> "+ w.GetName());
		//DumpStack();
		
		if ( (w.GetFlags() & WidgetFlags.IGNOREPOINTER) == WidgetFlags.IGNOREPOINTER )
		{
			return;
		}
		
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		TextWidget text1	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_text_1" ) );
		TextWidget text3	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_label" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindAnyWidget( w.GetName() + "_image" ) );
		
		Widget option	= Widget.Cast( w.FindAnyWidget( w.GetName() + "_option_wrapper" ) );
		
		if( text1 )
		{
			text1.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if( text2 )
		{
			text2.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if( text3 )
		{
			text3.SetColor( ARGB( 255, 255, 255, 255 ) );
			w.SetAlpha(0);
		}
		
		if( image )
		{
			image.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if ( option )
		{
			option.SetColor( ARGB( 150, 255, 255, 255 ) );
		}
	}
	
	void ColorDisable( Widget w )
	{
		#ifdef PLATFORM_WINDOWS
		SetFocus( null );
		#endif
		
		ButtonWidget button = ButtonWidget.Cast( w );
		if( button )
		{
			button.SetTextColor( ColorManager.COLOR_DISABLED_TEXT );
		}
	}
}