class KeybindingsMenu extends UIScriptedMenu
{
	protected TabberUI							m_Tabber;
	protected ref array<ref KeybindingsGroup>	m_Tabs;
	
	protected TextWidget						m_Version;
	
	protected ButtonWidget						m_Apply;
	protected ButtonWidget						m_Back;
	protected ButtonWidget						m_Reset;
	
	protected int								m_CurrentSettingKeyIndex = -1;
	protected int								m_CurrentSettingAlternateKeyIndex = -1;
	protected ref array<int>					m_SetKeybinds;
	
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
	
	void StartEnteringKeybind( int key_index )
	{
		m_CurrentSettingAlternateKeyIndex	= -1;
		m_CurrentSettingKeyIndex			= key_index;
	}
	
	void CancelEnteringKeybind()
	{
		GetCurrentTab().CancelEnteringKeybind();
		m_CurrentSettingKeyIndex = -1;
	}
	
	void StartEnteringAlternateKeybind( int key_index )
	{
		m_CurrentSettingKeyIndex			= -1;
		m_CurrentSettingAlternateKeyIndex	= key_index;
	}
	
	void CancelEnteringAlternateKeybind()
	{
		GetCurrentTab().CancelEnteringAlternateKeybind();
		m_CurrentSettingAlternateKeyIndex = -1;
	}
	
	void ConfirmKeybindEntry( TIntArray new_keys )
	{
		m_CurrentSettingKeyIndex = -1;
		m_Apply.Enable( true );
		m_Apply.ClearFlags( WidgetFlags.IGNOREPOINTER );
		m_Reset.Enable( true );
		m_Reset.ClearFlags( WidgetFlags.IGNOREPOINTER );
	}
	
	void ConfirmAlternateKeybindEntry( TIntArray new_keys )
	{
		m_CurrentSettingAlternateKeyIndex = -1;
		m_Apply.Enable( true );
		m_Apply.ClearFlags( WidgetFlags.IGNOREPOINTER );
		m_Reset.Enable( true );
		m_Reset.ClearFlags( WidgetFlags.IGNOREPOINTER );
	}
	
	override void Update(float timeslice)
	{
		if( GetGame().GetInput().GetActionDown(UAUIBack, false) )
		{
			Back();
		}
		
		if( GetCurrentTab() )
		{
			GetCurrentTab().Update( timeslice );
		}
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
		m_Reset.Enable( false );
		m_Reset.SetFlags( WidgetFlags.IGNOREPOINTER );
		
		foreach( KeybindingsGroup group : m_Tabs )
		{
			group.Apply();
		}
	}
	
	void Back()
	{
		if( m_CurrentSettingKeyIndex != -1 )
		{
			CancelEnteringKeybind();
			return;
		}
		
		if( m_CurrentSettingAlternateKeyIndex != -1 )
		{
			CancelEnteringAlternateKeybind();
			return;
		}
		
		bool changed = false;
		foreach( KeybindingsGroup group : m_Tabs )
		{
			changed = changed || group.IsChanged();
		}

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
		
		m_Reset.Enable( false );
		m_Reset.SetFlags( WidgetFlags.IGNOREPOINTER );
		
		foreach( KeybindingsGroup group : m_Tabs )
		{
			group.Reset();
		}
	}
	
	override bool OnModalResult( Widget w, int x, int y, int code, int result )
	{
		if( code == 1337 )
		{
			if( result == 2 )
			{
				Reset();
				GetGame().GetUIManager().Back();
			}
			return true;
		}
		return false;
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