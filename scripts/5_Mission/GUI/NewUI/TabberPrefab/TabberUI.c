class TabberUI extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	
	protected ref map<int, Widget> m_TabControls;
	protected ref map<int, Widget> m_Tabs;
	
	protected int m_SelectedIndex;
	
	ref ScriptInvoker		m_OnTabSwitch = new ScriptInvoker();

	void OnWidgetScriptInit( Widget w )
	{
		m_TabControls	= new map<int, Widget>;
		m_Tabs			= new map<int, Widget>;
		
		m_Root = w;
		Widget tab_controls	= m_Root.FindAnyWidget( "Tab_Control_Container" );
		if( tab_controls )
		{
			Widget tab_child	= tab_controls.GetChildren();
			
			int tab_count;
			
			while( tab_child )
			{
				tab_count++;
				tab_child = tab_child.GetSibling();
			}
			
			AlignTabbers( m_Root.FindAnyWidget( "Tab_Control_Container" ) );
			
			for( int i = 0; i < tab_count; i++ )
			{
				Widget tab_control	= tab_controls.FindAnyWidget( "Tab_Control_" + i );
				Widget tab_widget	= m_Root.FindAnyWidget( "Tab_" + i );
				if( tab_control && tab_widget )
				{
					tab_control.SetHandler( this );
					m_TabControls.Insert( i, tab_control );
					m_Tabs.Insert( i, tab_widget );
				}
				else
				{
					Error( "Tabber could not find correctly named tab or control at index " + i );
				}
			}
			
			#ifdef PLATFORM_CONSOLE
				Widget xb_controls = m_Root.FindAnyWidget( "XboxControls" );
				if( xb_controls )
					xb_controls.Show( tab_count > 1 );
			#endif
		}
	}
	
	void AlignTabbers( Widget tab_controls )
	{
		float total_size;
		float x, y;
		
		tab_controls.Update();
		
		int tab_count;
		Widget tab_child = tab_controls.GetChildren();
		while( tab_child )
		{
			tab_count++;
			tab_child.GetSize( x, y );
			total_size += x;
			tab_child = tab_child.GetSibling();
		}
		
		tab_child = tab_controls.GetChildren();
		while( tab_child )
		{
			Widget tab_bg = tab_child.FindAnyWidget( tab_child.GetName() + "_Background" );
			tab_child.GetPos( x, y );
			//tab_bg.SetPos( -x, 0 );
			//tab_bg.SetSize( total_size, 1 );
			
			tab_child = tab_child.GetSibling();
		}
		
		#ifdef PLATFORM_CONSOLE
			m_Root.FindAnyWidget( "XboxControls" ).Show( tab_count > 1 );
		#endif
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			int index = m_TabControls.GetKeyByValue( w );
			if( m_SelectedIndex != index )
			{
				DeselectTabControl( m_SelectedIndex );
				DeselectTabPanel( m_SelectedIndex );
				
				SelectTabControl( index );
				SelectTabPanel( index );
				
				m_SelectedIndex = index;
				AlignTabbers( m_Root.FindAnyWidget( "Tab_Control_Container" ) );
				m_OnTabSwitch.Invoke( m_SelectedIndex );
				return true;
			}
		}
		
		return false;
	}
	
	override bool OnChildAdd( Widget w, Widget child )
	{
		if( w == m_Root.FindAnyWidget( "Tab_Control_Container" ) )
		{
			AlignTabbers( m_Root.FindAnyWidget( "Tab_Control_Container" ) );
			return true;
		}
		return false;
	}
	
	override bool OnChildRemove( Widget w, Widget child )
	{
		if( w == m_Root.FindAnyWidget( "Tab_Control_Container" ) )
		{
			AlignTabbers( m_Root.FindAnyWidget( "Tab_Control_Container" ) );
			return true;
		}
		return false;
	}
	
	void SelectTabControl( int index )
	{
		Widget tab_control = m_TabControls.Get( index );
		if( tab_control )
		{
			Widget tab_bg = tab_control.FindAnyWidget( tab_control.GetName() + "_Background" );
			if( tab_bg )
			{
				tab_bg.Show( true );
			}
		}
	}
	
	void SelectTabPanel( int index )
	{
		Widget tab = m_Tabs.Get( index );
		if( tab )
		{
			tab.Show( true );
		}
	}
	
	void DeselectTabControl( int index )
	{
		Widget tab_control = m_TabControls.Get( index );
		if( tab_control )
		{
			Widget tab_bg = tab_control.FindAnyWidget( tab_control.GetName() + "_Background" );
			if( tab_bg )
			{
				tab_bg.Show( false );
			}
		}
	}
	
	void DeselectTabPanel( int index )
	{
		Widget tab = m_Tabs.Get( index );
		if( tab )
		{
			tab.Show( false );
		}
	}
	
	void NextTab()
	{
		int next_index = m_SelectedIndex + 1;
		if( next_index >= m_Tabs.Count() )
		{
			next_index = 0;
		}
		
		if( m_SelectedIndex != next_index )
		{
			DeselectTabControl( m_SelectedIndex );
			DeselectTabPanel( m_SelectedIndex );
				
			SelectTabControl( next_index );
			SelectTabPanel( next_index );
			
			m_SelectedIndex = next_index;
			m_OnTabSwitch.Invoke( m_SelectedIndex );
		}
	}
	
	void PreviousTab()
	{
		int next_index = m_SelectedIndex - 1;
		if( next_index < 0 )
		{
			next_index = m_TabControls.Count() - 1;
		}
		
		if( m_SelectedIndex != next_index )
		{
			DeselectTabControl( m_SelectedIndex );
			DeselectTabPanel( m_SelectedIndex );
				
			SelectTabControl( next_index );
			SelectTabPanel( next_index );
			
			m_SelectedIndex = next_index;
			m_OnTabSwitch.Invoke( m_SelectedIndex );
		}
	}
	
	int GetSelectedIndex()
	{
		return m_SelectedIndex;
	}
}