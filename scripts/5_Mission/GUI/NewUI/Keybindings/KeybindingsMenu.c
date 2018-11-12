class KeybindingsMenu extends UIScriptedMenu
{
	protected TabberUI							m_Tabber;
	protected ref array<ref KeybindingsGroup>	m_Tabs;
	
	protected ref KeybindingsEntryWindow		m_BindingEntryWindow;
	
	protected TextWidget						m_Version;
	
	protected ButtonWidget						m_Apply;
	protected ButtonWidget						m_Back;
	protected ButtonWidget						m_Reset;
	
	protected int								m_CurrentSettingKeyIndex = -1;
	
	override Widget Init()
	{
		layoutRoot			= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/pc/keybinding_menu.layout", null );
		
		m_Version			= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		m_Apply				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "apply" ) );
		m_Back				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "back" ) );
		m_Reset				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "reset" ) );
		
		m_Tabs				= new array<ref KeybindingsGroup>;
		layoutRoot.FindAnyWidget( "Tabber" ).GetScript( m_Tabber );
		
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		#else
			version = "#main_menu_version" + " " + version;
		#endif
		m_Version.SetText( version );

		#ifdef PLATFORM_PS4
			ImageWidget toolbar_b = layoutRoot.FindAnyWidget( "BackIcon" );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:circle" );
		#endif
		
		Input input	= GetGame().GetInput();
		int group_count = input.GetActionGroupsCount();
		
		for( int i = 0; i < group_count; i++ )
		{
			AddGroup( i, input );
		}
		
		m_Tabber.SelectTabControl( 0 );
		m_Tabber.SelectTabPanel( 0 );
		g_Game.SetKeyboardHandle( this );
		return layoutRoot;
	}
	
	void AddGroup( int index, Input input )
	{
		string group_name;
		input.GetActionGroupName( index, group_name );
		m_Tabber.AddTab( group_name );
		m_Tabs.Insert( new KeybindingsGroup( index, input, m_Tabber.GetTab( index ), this ) );
	}
	
	KeybindingsGroup GetCurrentTab()
	{
		return m_Tabs.Get( m_Tabber.GetSelectedIndex() );
	}
	
	void ShowKeybiningInputWindow( int key_index )
	{
		m_CurrentSettingKeyIndex	= key_index;
		m_BindingEntryWindow		= new KeybindingsEntryWindow( m_CurrentSettingKeyIndex, layoutRoot, this );
	}
	
	void CancelKeybindEntry()
	{
		m_BindingEntryWindow = null;
		m_CurrentSettingKeyIndex = -1;
	}
	
	void ConfirmKeybindEntry( TIntArray new_keys )
	{
		GetGame().GetInput().SetActionKeys( m_CurrentSettingKeyIndex, new_keys );
		GetCurrentTab().ReloadAction( m_CurrentSettingKeyIndex );
		m_BindingEntryWindow = null;
		m_CurrentSettingKeyIndex = -1;
	}
	
	override void Update(float timeslice)
	{
		if( GetGame().GetInput().GetActionDown(UAUIBack, false) )
		{
			Back();
		}
		
		if( m_BindingEntryWindow && m_BindingEntryWindow.IsListening() )
		{
			m_BindingEntryWindow.Update( timeslice );
		}
	}
	
	override bool OnKeyDown( Widget w, int x, int y, int key )
	{
		if( m_CurrentSettingKeyIndex > -1 && m_BindingEntryWindow && m_BindingEntryWindow.IsListening() )
		{
			m_BindingEntryWindow.OnKeyDown( w, x, y, key );
		}
		return false;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_Apply )
			{
				Apply();
				return true;
			}
			else if( w == m_Back )
			{
				Back();
				return true;
			}
			else if( w == m_Reset )
			{
				Reset();
				return true;
			}
		}
		return false;
	}
	
	void Apply()
	{
		m_Apply.Enable( false );
		m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
		layoutRoot.FindAnyWidget( "Apply" ).Show( false );
		m_Reset.Enable( false );
		m_Reset.SetFlags( WidgetFlags.IGNOREPOINTER );
		layoutRoot.FindAnyWidget( "Reset" ).Show( false );
	}
	
	void Back()
	{
		if( m_CurrentSettingKeyIndex > -1 || m_BindingEntryWindow )
		{
			m_BindingEntryWindow.Cancel();
			return;
		}
		
		bool changed = false;

		if( changed )
			g_Game.GetUIManager().ShowDialog("#main_menu_configure", "#main_menu_configure_desc", 1337, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
		else
		{
			GetGame().GetUIManager().Back();
		}
	}
	
	void Reset()
	{
		m_Apply.Enable( false );
		m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
		layoutRoot.FindAnyWidget( "Apply" ).Show( false );
		
		m_Reset.Enable( false );
		m_Reset.SetFlags( WidgetFlags.IGNOREPOINTER );
		layoutRoot.FindAnyWidget( "Reset" ).Show( false );
	}
	
	override void Refresh()
	{
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		#else
			version = "#main_menu_version" + " " + version;
		#endif
		m_Version.SetText( version );
	}
}