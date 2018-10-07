class KeybindingsMenu extends UIScriptedMenu
{
	protected TabberUI					m_Tabber;
	protected ref GeneralKeybindings	m_GeneralTab;
	protected ref CharacterKeybindings	m_CharacterTab;
	protected ref VehicleKeybindings	m_VehicleTab;
	protected ref AircraftKeybindings	m_AircraftTab;
	
	protected TextWidget				m_Version;
	
	protected ButtonWidget				m_Apply;
	protected ButtonWidget				m_Back;
	protected ButtonWidget				m_Reset;
	
	override Widget Init()
	{
		layoutRoot			= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/pc/keybinding_menu.layout", null );
		
		m_Version			= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		m_Apply				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "apply" ) );
		m_Back				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "back" ) );
		m_Reset				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "reset" ) );
		
		m_GeneralTab		= new GeneralKeybindings( layoutRoot.FindAnyWidget( "Tab_0" ), this );
		m_CharacterTab		= new CharacterKeybindings( layoutRoot.FindAnyWidget( "Tab_1" ), this );
		m_VehicleTab		= new VehicleKeybindings( layoutRoot.FindAnyWidget( "Tab_2" ), this );
		m_AircraftTab		= new AircraftKeybindings( layoutRoot.FindAnyWidget( "Tab_3" ), this );
		
		string version;
		GetGame().GetVersion( version );
		if( version != "" )
			m_Version.SetText( "#main_menu_version" + " " + version );
		else
			m_Version.Show( false );

		#ifdef PLATFORM_PS4
			ImageWidget toolbar_b = layoutRoot.FindAnyWidget( "BackIcon" );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:circle" );
		#endif
		return layoutRoot;
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
		bool changed = false;

		if( changed )
			g_Game.GetUIManager().ShowDialog("#main_menu_configure", "You have unsaved changes in the configuration. Would you like to discard them?", 1337, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
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