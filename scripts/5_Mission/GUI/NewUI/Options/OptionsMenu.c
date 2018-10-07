class OptionsMenuNew extends UIScriptedMenu
{
	const int SLIDER_WIDGET = 42;
	protected TabberUI					m_Tabber;
	protected ref OptionsMenuGame		m_GameTab;
	protected ref OptionsMenuSounds		m_SoundsTab;
	protected ref OptionsMenuVideo		m_VideoTab;
	protected ref OptionsMenuControls	m_ControlsTab;
	
	protected ref GameOptions			m_Options;
	
	protected ButtonWidget				m_Apply;
	protected ButtonWidget				m_Back;
	protected ButtonWidget				m_Reset;
	
	protected Widget					m_Details;
	protected TextWidget				m_Version;
	
	void OptionsMenuNew()
	{
		
	}
	
	override Widget Init()
	{
		m_Options			= new GameOptions();
		
		#ifdef PLATFORM_CONSOLE
			layoutRoot		= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/xbox/options_menu.layout", null );
		#else
		#ifdef PLATFORM_WINDOWS
			layoutRoot		= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/pc/options_menu.layout", null );
		#endif
		#endif
		
		layoutRoot.FindAnyWidget( "Tabber" ).GetScript( m_Tabber );
		
		m_Details			= layoutRoot.FindAnyWidget( "settings_details" );
		m_Version			= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		
		m_GameTab			= new OptionsMenuGame( layoutRoot.FindAnyWidget( "Tab_0" ), m_Details, m_Options, this );
		m_SoundsTab			= new OptionsMenuSounds( layoutRoot.FindAnyWidget( "Tab_1" ), m_Details, m_Options, this );
		
		#ifdef PLATFORM_CONSOLE
			m_ControlsTab	= new OptionsMenuControls( layoutRoot.FindAnyWidget( "Tab_2" ), m_Details, m_Options, this );
		#else
		#ifdef PLATFORM_WINDOWS
			m_VideoTab		= new OptionsMenuVideo( layoutRoot.FindAnyWidget( "Tab_2" ), m_Details, m_Options, this );
			m_ControlsTab	= new OptionsMenuControls( layoutRoot.FindAnyWidget( "Tab_3" ), m_Details, m_Options, this );
		#endif
		#endif
		
		m_Apply				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "apply" ) );
		m_Back				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "back" ) );
		m_Reset				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "reset" ) );
		
		string version;
		GetGame().GetVersion( version );
		if( version != "" )
			m_Version.SetText( "#main_menu_version" + " " + version );
		else
			m_Version.Show( false );
		#ifdef PLATFORM_WINDOWS
			SetFocus( layoutRoot );
		#else
			SliderFocus();
		#endif
		
		m_Tabber.m_OnTabSwitch.Insert( OnTabSwitch );
		
		#ifdef PLATFORM_PS4
			ImageWidget toolbar_a = layoutRoot.FindAnyWidget( "ToggleIcon" );
			ImageWidget toolbar_b = layoutRoot.FindAnyWidget( "BackIcon" );
			ImageWidget toolbar_x = layoutRoot.FindAnyWidget( "ApplyIcon" );
			ImageWidget toolbar_y = layoutRoot.FindAnyWidget( "ResetIcon" );
			toolbar_a.LoadImageFile( 0, "set:playstation_buttons image:cross" );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:circle" );
			toolbar_x.LoadImageFile( 0, "set:playstation_buttons image:square" );
			toolbar_y.LoadImageFile( 0, "set:playstation_buttons image:triangle" );
		#endif
		
		return layoutRoot;
	}
	
	void ~OptionsMenuNew()
	{
		Reset();
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
	
	void OnTabSwitch( int tab )
	{
		switch( tab )
		{
			case 0:
			{
				m_GameTab.Focus();
				break;
			}
			case 1:
			{
				m_SoundsTab.Focus();
				break;
			}
			case 2:
			{
				#ifdef PLATFORM_CONSOLE
					m_ControlsTab.Focus();
				#else
				#ifdef PLATFORM_WINDOWS
					m_VideoTab.Focus();
				#endif
				#endif
				break;
			}
			case 3:
			{
				#ifdef PLATFORM_WINDOWS
					m_ControlsTab.Focus();
				#endif
				break;
			}
		}
	}
	
	void Apply()
	{
		if( m_Options.IsChanged() || m_GameTab.IsChanged() )
		{
			m_Options.Test();
			m_Options.Apply();
		}
		
		if( m_GameTab.IsChanged() )
		{
			m_GameTab.Apply();
		}
		
		#ifdef PLATFORM_CONSOLE
			layoutRoot.FindAnyWidget( "Reset" ).Show( false );
			layoutRoot.FindAnyWidget( "Apply" ).Show( false );
		#else
		#ifdef PLATFORM_WINDOWS
			m_Apply.Enable( false );
			m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
			m_Reset.Enable( false );
			m_Reset.SetFlags( WidgetFlags.IGNOREPOINTER );
		#endif
		#endif
		
		if( m_Options.NeedRestart() )
			g_Game.GetUIManager().ShowDialog("#main_menu_configure", "#menu_restart_needed", 117, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
	}
	
	void Back()
	{
		bool changed = false;
		
		if( m_Options.IsChanged() )
		{
			changed = true;
		}
		
		if( m_GameTab.IsChanged() )
		{
			changed = true;
		}
		
		if( m_SoundsTab.IsChanged() )
		{
			changed = true;
		}
		
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
		else if( m_VideoTab.IsChanged() )
		{
			changed = true;
		}
		#endif
		#endif
		
		if( changed )
			g_Game.GetUIManager().ShowDialog("#main_menu_configure", "You have unsaved changes in the configuration. Would you like to discard them?", 1337, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
		else
		{
			m_Options.Revert();
			GetGame().EndOptionsVideo();
			GetGame().GetUIManager().Back();
		}
	}
	
	void OnChanged()
	{
		bool changed = false;
		if( m_Options.IsChanged() || m_GameTab.IsChanged() || m_SoundsTab.IsChanged() )
		{
			changed = true;
		}
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
		else if( m_VideoTab.IsChanged() )
		{
			changed = true;
		}
		#endif
		#endif
		
		#ifdef PLATFORM_CONSOLE
			layoutRoot.FindAnyWidget( "Apply" ).Show( changed );
			layoutRoot.FindAnyWidget( "Reset" ).Show( changed );
		#else
		#ifdef PLATFORM_WINDOWS
			m_Apply.Enable( changed );
			m_Reset.Enable( changed );
			
			if( changed )
			{
				m_Apply.ClearFlags( WidgetFlags.IGNOREPOINTER );
				m_Reset.ClearFlags( WidgetFlags.IGNOREPOINTER );
			}
			else
			{
				m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
				m_Reset.SetFlags( WidgetFlags.IGNOREPOINTER );
			}
		#endif
		#endif
	}
	
	void Reset()
	{
		//if( m_Options.IsChanged() )
			m_Options.Revert();
		//if( m_GameTab.IsChanged() )
			m_GameTab.Revert();
		//if( m_SoundsTab.IsChanged() )
			m_SoundsTab.Revert();
			m_ControlsTab.Revert();
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
		//if( m_VideoTab.IsChanged() )
			m_VideoTab.Revert();
		#endif
		#endif
		
		#ifdef PLATFORM_CONSOLE
			layoutRoot.FindAnyWidget( "Apply" ).Show( false );
			layoutRoot.FindAnyWidget( "Reset" ).Show( false );
		#else
		#ifdef PLATFORM_WINDOWS
			m_Apply.Enable( false );
			m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
		
			m_Reset.Enable( false );
			m_Reset.SetFlags( WidgetFlags.IGNOREPOINTER );
		#endif
		#endif
	}
	
	void SliderFocus()
	{
		#ifdef PLATFORM_CONSOLE
		layoutRoot.FindAnyWidget( "Toggle" ).Show( false );
		#endif
	}
	
	void ToggleFocus()
	{
		#ifdef PLATFORM_CONSOLE
		layoutRoot.FindAnyWidget( "Toggle" ).Show( true );
		#endif
	}
	
	void ReloadOptions()
	{
		m_Options = new GameOptions();
		if( m_GameTab )
			m_GameTab.SetOptions( m_Options );
		if( m_SoundsTab )
			m_SoundsTab.SetOptions( m_Options );
		if( m_ControlsTab )
			m_ControlsTab.SetOptions( m_Options );
		
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
			if( m_VideoTab )
				m_VideoTab.SetOptions( m_Options );
		#endif
		#endif
	}
	
	override bool OnModalResult( Widget w, int x, int y, int code, int result )
	{
		if( code == 1337 )
		{
			if( result == 2 )
			{
				m_Options.Revert();
				GetGame().EndOptionsVideo();
				GetGame().GetUIManager().Back();
			}
			return true;
		}
		else if( code == 117 )
		{
			g_Game.RequestRestart(IDC_MAIN_QUIT);
		}
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( w && IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( w && IsFocusable( w ) )
		{
			ColorWhite( w, enterW );
			return true;
		}
		return false;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( w && IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		if( w )
		{
			if( w.IsInherited( SliderWidget ) || w.GetUserID() == SLIDER_WIDGET )
			{
				SliderFocus();
				return true;
			}
			else
			{
				ToggleFocus();
				return true;
			}
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( w && IsFocusable( w ) )
		{
			ColorWhite( w, null );
			return true;
		}
		if( x == -1 && y == 2 )
		{
			ToggleFocus();
		}
		return false;
	}
	
	bool IsFocusable( Widget w )
	{
		return ( w == m_Apply || w == m_Back || w == m_Reset );
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
	
	override void OnShow()
	{
		super.OnShow();
		Refresh();
	}
	
	override void OnHide()
	{
		super.OnHide();
	}
	
	override void Update( float timeslice )
	{
		super.Update( timeslice );
		if( GetGame().GetInput().GetActionDown( UAUITabLeft, false ) )
		{
			m_Tabber.PreviousTab();
		}
		
		//RIGHT BUMPER - TAB RIGHT
		if( GetGame().GetInput().GetActionDown( UAUITabRight, false ) )
		{
			m_Tabber.NextTab();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIFastEquipOrSplit, false ) )
		{
			Apply();
		}
		
		if( GetGame().GetInput().GetActionDown( UAQuickReload, false ) )
		{
			Reset();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIBack, false ) )
		{
			Reset();
		}
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w )
	{
		SetFocus( w );
	}
	
	void ColorWhite( Widget w, Widget enterW )
	{
		#ifdef PLATFORM_WINDOWS
		SetFocus( null );
		#endif
	}
}
