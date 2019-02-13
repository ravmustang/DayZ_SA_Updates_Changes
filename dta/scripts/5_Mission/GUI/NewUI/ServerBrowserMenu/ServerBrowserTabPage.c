class ServerBrowserTabPage extends ServerBrowserTab
{
	protected const int SERVERS_VISIBLE_COUNT	= 22;
	protected const int PAGES_BUTTONS_COUNT		= 10;
	
	protected int									m_TotalServersCount;
	protected int									m_PageIndex;
	protected int									m_PageStartNum;
	protected int									m_PageEndNum;
	protected int									m_PagesCount;
	protected int									m_ServersEstimateCount;
	protected bool									m_LoadingFinished;
	protected ref array<ref GetServersResultRow>	m_ServerResultsFiltred;
	
	protected Widget								m_PnlPagesPanel;
	protected TextWidget							m_PnlPagesLoadingText;
	protected ButtonWidget							m_BtnPagesFirst;
	protected ButtonWidget							m_BtnPagesLast;
	protected ButtonWidget							m_BtnPagePrev;
	protected ButtonWidget							m_BtnPageNext;
	
	protected Widget								m_FilterSearchText;
	protected Widget								m_FilterSearchIP;
	
	protected ButtonWidget							m_BtnFilterReset;
	
	protected ref array<ButtonWidget>				m_BtnPages;
	protected ref array<ref ServerBrowserEntry>		m_ServerListEntiers;
	
	protected override void Construct( Widget parent, ServerBrowserMenuNew menu, TabType type )
	{
		#ifdef PLATFORM_CONSOLE
			m_Root					= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/server_browser/xbox/server_browser_tab.layout", parent );
		#else
		#ifdef PLATFORM_WINDOWS
			m_Root					= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/server_browser/pc/server_browser_tab_pages.layout", parent );
		#endif
		#endif
		
		m_ServerListScroller	= ScrollWidget.Cast( m_Root.FindAnyWidget( "server_list_scroller" ) );
		m_ServerList			= SpacerBaseWidget.Cast( m_ServerListScroller.FindAnyWidget( "server_list_content" ) );
		m_ServerListScroller.VScrollToPos01( 0 );
				
		m_Entries				= new array<ref GetServersResultRow>;
		m_EntryWidgets			= new map<string, ref ServerBrowserEntry>;
		m_ServerListEntiers		= new array<ref ServerBrowserEntry>;
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
		
		m_FilterSearchText		= m_Root.FindAnyWidget( "search_name_setting" );
		m_FilterSearchIP		= m_Root.FindAnyWidget( "search_ip_setting" );
		
		m_LoadingText			= TextWidget.Cast( m_Root.FindAnyWidget( "loading_servers_info" ) );
		
		if( type == TabType.LAN )
		{
			m_Root.FindAnyWidget( "filters_header" ).Show( false );
			m_Root.FindAnyWidget( "filters_content" ).Show( false );
			m_Root.FindAnyWidget( "spacer" ).Show( false );
			m_Root.FindAnyWidget( "apply_filter_button" ).Show( false );
			m_Root.FindAnyWidget( "reset_filter_button" ).Show( false );
		}
		
		m_Filters				= new ServerBrowserFilterContainer( m_Root.FindAnyWidget( "filters_content" ), this );
		
		SetSort( ESortType.HOST, ESortOrder.DESCENDING );
		
		m_Root.SetHandler( this );		
		
		m_BtnPages = new array<ButtonWidget>();
		
		int page_button_num = 1;
		ButtonWidget page_button = ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_page"+ page_button_num ) );
		
		if ( PAGES_BUTTONS_COUNT < 3 || PAGES_BUTTONS_COUNT > 10 )
		{
			Error("ServerBrowserTabPage->PAGES_BUTTONS_COUNT must be in range 3 - 10!");
			return;
		}
		
		while ( page_button )
		{
			page_button.Show(false);
			
			if ( page_button_num <= PAGES_BUTTONS_COUNT )
			{
				m_BtnPages.Insert( page_button );
			}
			
			page_button_num++;
			page_button = ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_page"+ page_button_num ) );
		}		
		
		m_BtnFilterReset		= ButtonWidget.Cast( m_Root.FindAnyWidget( "reset_filter_button" ) ) ;
		
		m_PnlPagesPanel			= m_Root.FindAnyWidget( "servers_navigation_spacer" );
		m_BtnPagePrev			= ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_prev" ) ) ;
		m_BtnPageNext			= ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_next" ) ) ;
		m_BtnPagesFirst			= ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_page_first" ) ) ;
		m_BtnPagesLast			= ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_page_last" ) ) ;
		m_ServerResultsFiltred	= new array<ref GetServersResultRow>();
		m_PageIndex				= 0;
		
		UpdateServerList();
	}
	
	void ~ServerBrowserTabPage()
	{
		
	}
	
	override void RefreshList()
	{
		OnlineServices.m_ServersAsyncInvoker.Remove( OnLoadServersAsync );
		
		m_ServerResultsFiltred.Clear();
		m_Entries.Clear();
		
		m_LoadingFinished = false;
		m_TotalServersCount = 0;
		m_PageIndex = 0;
		
		super.RefreshList();
	}
	
	override void OnLoadServersAsyncPC( ref GetServersResult result_list, EBiosError error, string response )
	{
		if( result_list )
		{
			if( result_list.m_Results.Count() > 0 )
			{
				foreach( GetServersResultRow result : result_list.m_Results )
				{
					m_TotalServersCount++;
					
					if( PassFilter( result ) )
					{
						AddSorted( result );
					}
					
					if( !m_Menu || m_Menu.IsRefreshing() != m_TabType )
					{
						return;
					}
				}
			}
			else
			{
				m_LoadingText.SetText( "#servers_count "+ m_Entries.Count() );
				m_Menu.SetRefreshing( TabType.NONE );
			}

		}
		else
		{
			m_LoadingText.SetText( "#servers_count "+ m_Entries.Count() );
			m_Menu.SetRefreshing( TabType.NONE );
		}
		
		if ( response == "thelastone" )
		{
			OnLoadServersAsyncPCFinished();
		}
		else
		{
			m_ServersEstimateCount = result_list.m_NumServers;
		}
		
		UpdateServerList();
	}
	
	void OnLoadServersAsyncPCFinished()
	{
		m_LoadingFinished = true;
		
		ShowServerList();
	}
	
	override void AddSorted( GetServersResultRow entry )
	{
		ref GetServersResultRow row = entry;
		if( m_Entries )
		{
			m_Entries.Insert( row );
			if( m_Entries.Count() > 1 )
			{
				InsertSort( m_Entries );
			}
		}
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick( w, x, y, button );
		
		if( button == MouseState.LEFT )
		{
			if ( w == m_BtnFilterReset )
			{
				ResetFilters();
			}
			else if ( w == m_BtnPagePrev )
			{
				OnClickPagePrev();
			}
			else if ( w == m_BtnPageNext )
			{
				OnClickPageNext();
			}
			else if ( w == m_BtnPagesLast )
			{
				OnClickPageEnd();
			}
			else if ( w == m_BtnPagesFirst )
			{
				OnClickPageFirst();
			}
			else
			{
				bool click = OnClickPage( w );
				
				if ( click )
				{
					return true;
				}
				
				return false;
			}
		}
		return false;
	}
	
	void OnClickPagePrev()
	{
		if ( m_PageIndex > 0 )
		{
			SetPageIndex( m_PageIndex - 1);
		}
		else
		{
			SetPageIndex( m_PagesCount - 1 );
		}
	}
	
	void OnClickPageNext()
	{
		if ( m_PageIndex < (m_PagesCount - 1) )
		{
			SetPageIndex( m_PageIndex + 1 );
		}
		else
		{
			SetPageIndex( 0 );
		}
	}
	
	void OnClickPageEnd()
	{
		SetPageIndex( m_PagesCount - 1 );
	}
	
	void OnClickPageFirst()
	{
		SetPageIndex( 0 );
	}
	
	bool OnClickPage(Widget w)
	{
		for ( int i = 0; i < m_BtnPages.Count(); ++i )
		{
			if ( m_BtnPages[i] == w )
			{
				SetPageIndex( m_PageStartNum + i - 1 );
				return true;
			}
		}
		
		return false;
	}
	
	void SetPageIndex(int page_index)
	{
		m_PageIndex = page_index;
		UpdateServerList();
	}
	
	void UpdateServerList()
	{
		m_PagesCount = Math.Ceil( m_Entries.Count() / SERVERS_VISIBLE_COUNT );
		
		//Print("page num: ("+ (m_PageIndex + 1) +") m_PageIndex="+ m_PageIndex +" m_PagesCount="+ m_PagesCount);
		
		if ( m_PagesCount > 1 )
		{
			int i;
			int btn_index_center = Math.Round( PAGES_BUTTONS_COUNT / 2) - 1; 
			m_PageStartNum = 1;
			m_PageEndNum = PAGES_BUTTONS_COUNT;
			
			if ( m_PagesCount <= PAGES_BUTTONS_COUNT )
			{
				m_PageEndNum = m_PagesCount;
				
				// Pages are less like buttons - changing selected button
				for ( i = 1; i <= m_BtnPages.Count(); ++i )
				{
					bool is_page_button_enable = ( i <= m_PagesCount );
										
					int page_index = (i - 1);
					m_BtnPages[page_index].Show( is_page_button_enable );
					m_BtnPages[page_index].SetText( i.ToString() );
					
					if ( m_PageIndex == page_index )
					{
						m_BtnPages[page_index].SetState( true );
					}
					else
					{
						m_BtnPages[page_index].SetState( false );
					}
				}
			}
			else
			{
				// Pages are more like buttons				
				if ( m_PageIndex <= btn_index_center )
				{
					//Left side (start of pages)
					//m_PageStartNum = 1;
					//m_PageEndNum = PAGES_BUTTONS_COUNT;
				}
				else
				{
					m_PageStartNum = m_PageIndex - btn_index_center + 1;
					m_PageEndNum = m_PageIndex + (PAGES_BUTTONS_COUNT - btn_index_center);
							
					if ( m_PageEndNum > m_PagesCount )
					{
						// Right side (end of pages)
						m_PageStartNum -= m_PageEndNum - m_PagesCount;
						m_PageEndNum -= m_PageEndNum - m_PagesCount;
					}
				}
				
				
				// Button Selection
				int selected_btn_index = m_PageIndex;
				if ( m_PageIndex > btn_index_center )
				{
					if ( m_PageEndNum == m_PagesCount )
					{
						//Right
						 selected_btn_index = PAGES_BUTTONS_COUNT - (m_PagesCount - m_PageIndex);
					}
					else
					{
						//Center
						selected_btn_index = btn_index_center;
					}
				}
				
				
				for ( i = 0; i < m_BtnPages.Count(); ++i )
				{
					m_BtnPages[i].SetText( (m_PageStartNum + i).ToString() );
					m_BtnPages[i].SetState( ( i == selected_btn_index ) );
				}
			}
			
			// Button GoTo Prev Page
			if ( m_PageIndex > 0 )
			{
				m_BtnPagePrev.SetText( "<" );
				m_BtnPagePrev.Enable( true );
			}
			else
			{
				m_BtnPagePrev.SetText( "" );
				m_BtnPagePrev.Enable( false );					
			}
			
			// Button GoTo Next Page
			if ( m_PageIndex < (m_PagesCount - 1) )
			{
				m_BtnPageNext.SetText( ">" );
				m_BtnPageNext.Enable( true );
			}
			else
			{
				m_BtnPageNext.SetText( "" );
				m_BtnPageNext.Enable( false );
			}
			
			// Button GoTo First Page
			if ( m_PageStartNum == 1 )
			{
				m_BtnPagesFirst.SetText( "" );
				m_BtnPagesFirst.Enable( false );
			}
			else
			{
				m_BtnPagesFirst.SetText( "<<" );
				m_BtnPagesFirst.Enable( true );
			}
			
			// Button GoTo Last Page
			if ( m_PageEndNum == m_PagesCount )
			{
				m_BtnPagesLast.SetText( "" );
				m_BtnPagesLast.Enable( false );
			}
			else
			{
				m_BtnPagesLast.SetText( ">>" );
				m_BtnPagesLast.Enable( true );
			}
		}
		else
		{
			m_PnlPagesPanel.Show(false);
		}
		
		ShowServerList();
	}
	
	override void Sort()
	{
		super.Sort();
		
		ShowServerList();
	}
	
	override void Sort( array<ref GetServersResultRow> entries, int low, int high )
	{
		super.Sort(entries, low, high);
		
		ShowServerList();
	}
	
	override void InvertSort()
	{
		super.InvertSort();
		
		ShowServerList();
	}
	
	override void InplaceReverse()
	{
		super.InplaceReverse();
		
		ShowServerList();
	}
	
	void ShowServerList()
	{		
		int i;
		ServerBrowserEntry entry;
		
		m_EntryWidgets.Clear();
		
		m_PnlPagesPanel.Show( (m_PagesCount > 1) );
		
		for ( i = 0; i < SERVERS_VISIBLE_COUNT; ++i )
		{
			int server_index = i + (SERVERS_VISIBLE_COUNT * m_PageIndex);
			
			if ( server_index >= 0 && server_index < m_Entries.Count() )
			{
				GetServersResultRow server_info = m_Entries[server_index];
				
				entry = GetServerEntryByIndex(i, server_info.m_Id);
				
				entry.Show( true );
				entry.FillInfo( server_info );
				entry.SetFavorite( m_Menu.IsFavorited( server_info.m_Id ) );
				entry.SetExpand( server_info.m_IsExpanded );
				m_TotalLoadedServers++;
				
				if ( m_LoadingFinished )
				{
					m_LoadingText.SetText( "#servers_found: "+ m_Entries.Count() );
				}
				else
				{
					if ( m_ServersEstimateCount > 0 )
					{
						int loading_percentage = Math.Round((m_TotalServersCount / m_ServersEstimateCount) * 100);
						m_LoadingText.SetText( "#server_browser_tab_loaded "+ loading_percentage.ToString() +"%" );
					}
					else
					{
						m_LoadingText.SetText( "#dayz_game_loading" );
					}
				}
			}
			else
			{
				entry = GetServerEntryByIndex(i, "-1");
				
				if ( entry )
				{
					entry.Show( false );
				}
			}
		}
		
		m_ServerList.Update();
	}
	
	protected ServerBrowserEntry GetServerEntryByIndex( int index, string server_id )
	{
		ref ServerBrowserEntry entry;
		
		if ( index >= 0 )
		{
			if ( index < m_ServerListEntiers.Count() )
			{
				entry = m_ServerListEntiers[index];
			}
			else
			{
				entry = new ServerBrowserEntry( null, index, this );
				m_ServerList.AddChild( entry.GetRoot(), false );
				
				m_ServerListEntiers.Insert(entry);
			}
		}
		else
		{
			return null;
		}
		
		// Refresh list
		m_EntryWidgets.Insert( server_id, entry );
		
		return entry;
	}
	
	override bool IsFocusable( Widget w )
	{
		if( w )
		{			
			if ( w == m_ApplyFilter || w == m_RefreshList || w == m_BtnFilterReset || w == m_FilterSearchText || w == m_FilterSearchIP )
			{				
				return true;
			}
			
			return false;
		}
		return false;
	}
}