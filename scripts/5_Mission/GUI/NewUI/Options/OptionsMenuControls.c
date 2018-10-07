class OptionsMenuControls extends ScriptedWidgetEventHandler
{
	protected Widget						m_Root;
	
	protected Widget						m_SettingsRoot;
	protected Widget						m_DetailsRoot;
	protected TextWidget					m_DetailsLabel;
	protected RichTextWidget				m_DetailsText;
	protected ButtonWidget					m_Keybindings;
	
	protected GameOptions					m_Options;
	protected OptionsMenuNew				m_Menu;
	
	protected ref NumericOptionsAccess		m_VSensitivityOption;
	protected ref NumericOptionsAccess		m_HSensitivityOption;
	protected ref SwitchOptionsAccess		m_InvertOption;
	protected ref NumericOptionsAccess		m_ControllerVSensitivityOption;
	protected ref NumericOptionsAccess		m_ControllerHSensitivityOption;
	protected ref SwitchOptionsAccess		m_ControllerInvertOption;
	
	protected ref OptionSelectorSlider		m_VSensitivitySelector;
	protected ref OptionSelectorSlider		m_HSensitivitySelector;
	protected ref OptionSelectorMultistate	m_InvertSelector;
	protected ref OptionSelectorSlider		m_ControllerVSensitivitySelector;
	protected ref OptionSelectorSlider		m_ControllerHSensitivitySelector;
	protected ref OptionSelectorMultistate	m_ControllerInvertSelector;
	
	protected ref map<int, ref Param2<string, string>> m_TextMap;
	
	void OptionsMenuControls( Widget parent, Widget details_root, GameOptions options, OptionsMenuNew menu )
	{
		#ifdef PLATFORM_CONSOLE
			m_Root									= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/xbox/controls_tab.layout", parent );
		#else
		#ifdef PLATFORM_WINDOWS
			m_Root									= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/pc/controls_tab.layout", parent );
		#endif
		#endif
		
		m_DetailsRoot								= details_root;
		m_DetailsLabel								= TextWidget.Cast( m_DetailsRoot.FindAnyWidget( "details_label" ) );
		m_DetailsText								= RichTextWidget.Cast( m_DetailsRoot.FindAnyWidget( "details_content" ) );
		m_Keybindings								= ButtonWidget.Cast( m_Root.FindAnyWidget( "keybinding_setting_container" ) );
		
		m_Options 									= options;
		m_Menu										= menu;
		
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
		m_VSensitivityOption						= NumericOptionsAccess.Cast( m_Options.GetOptionByType( AT_CONFIG_YAXIS ) );
		m_HSensitivityOption						= NumericOptionsAccess.Cast( m_Options.GetOptionByType( AT_CONFIG_XAXIS ) );
		m_InvertOption								= SwitchOptionsAccess.Cast( m_Options.GetOptionByType( AT_CONFIG_YREVERSED ) );
		#endif
		#endif
		
		m_ControllerVSensitivityOption				= NumericOptionsAccess.Cast( m_Options.GetOptionByType( AT_CONFIG_CONTROLLER_YAXIS ) );
		m_ControllerHSensitivityOption				= NumericOptionsAccess.Cast( m_Options.GetOptionByType( AT_CONFIG_CONTROLLER_XAXIS ) );
		m_ControllerInvertOption					= SwitchOptionsAccess.Cast( m_Options.GetOptionByType( AT_CONFIG_CONTROLLER_REVERSED_LOOK ) );
		
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
		m_Root.FindAnyWidget( "vsensitivity_setting_option" ).SetUserID( AT_CONFIG_YAXIS );
		m_Root.FindAnyWidget( "hsensitivity_setting_option" ).SetUserID( AT_CONFIG_XAXIS );
		m_Root.FindAnyWidget( "invert_setting_option" ).SetUserID( AT_CONFIG_YREVERSED );
		m_Keybindings.SetUserID( 777 );
		m_Keybindings.SetHandler( this );
		#endif
		#endif
		
		m_Root.FindAnyWidget( "controller_vsensitivity_setting_option" ).SetUserID( AT_CONFIG_CONTROLLER_YAXIS );
		m_Root.FindAnyWidget( "controller_hsensitivity_setting_option" ).SetUserID( AT_CONFIG_CONTROLLER_XAXIS );
		m_Root.FindAnyWidget( "controller_invert_setting_option" ).SetUserID( AT_CONFIG_CONTROLLER_REVERSED_LOOK );

		ref array<string> opt						= { "Disabled", "Enabled" };
	
		m_ControllerVSensitivitySelector			= new OptionSelectorSlider( m_Root.FindAnyWidget( "controller_vsensitivity_setting_option" ), m_ControllerVSensitivityOption.ReadValue(), this, false, m_ControllerVSensitivityOption.GetMin(), m_ControllerVSensitivityOption.GetMax() );
		m_ControllerHSensitivitySelector			= new OptionSelectorSlider( m_Root.FindAnyWidget( "controller_hsensitivity_setting_option" ), m_ControllerHSensitivityOption.ReadValue(), this, false, m_ControllerHSensitivityOption.GetMin(), m_ControllerHSensitivityOption.GetMax() );
		m_ControllerInvertSelector					= new OptionSelectorMultistate( m_Root.FindAnyWidget( "controller_invert_setting_option" ), m_ControllerInvertOption.GetIndex(), this, false, opt );
		
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
		m_VSensitivitySelector						= new OptionSelectorSlider( m_Root.FindAnyWidget( "vsensitivity_setting_option" ), m_VSensitivityOption.ReadValue(), this, false, m_VSensitivityOption.GetMin(), m_VSensitivityOption.GetMax() );
		m_HSensitivitySelector						= new OptionSelectorSlider( m_Root.FindAnyWidget( "hsensitivity_setting_option" ), m_HSensitivityOption.ReadValue(), this, false, m_HSensitivityOption.GetMin(), m_HSensitivityOption.GetMax() );
		m_InvertSelector							= new OptionSelectorMultistate( m_Root.FindAnyWidget( "invert_setting_option" ), m_InvertOption.GetIndex(), this, false, opt );
		
		m_VSensitivitySelector.m_OptionChanged.Insert( UpdateVerticalSensitivity );
		m_HSensitivitySelector.m_OptionChanged.Insert( UpdateHorizontalSensitivity );
		m_InvertSelector.m_OptionChanged.Insert( UpdateInvertView );
		#endif
		#endif
		
		m_ControllerVSensitivitySelector.m_OptionChanged.Insert( UpdateControllerVerticalSensitivity );
		m_ControllerHSensitivitySelector.m_OptionChanged.Insert( UpdateControllerHorizontalSensitivity );
		m_ControllerInvertSelector.m_OptionChanged.Insert( UpdateControllerInvertView );
		
		FillTextMap();
	}
	
	void EnterKeybindingMenu()
	{
		m_Menu.EnterScriptedMenu( MENU_KEYBINDINGS );
	}
	
	void Focus()
	{
		#ifdef PLATFORM_CONSOLE
			SetFocus( m_ControllerInvertSelector.GetParent() );
		#endif
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			#ifdef PLATFORM_WINDOWS
			#ifndef PLATFORM_CONSOLE
			if( w == m_Keybindings )
			{
				EnterKeybindingMenu();
				return true;
			}
			#endif
			#endif
		}
		return false;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		OptionsMenuNew menu = OptionsMenuNew.Cast( GetGame().GetUIManager().GetMenu() );
		if( menu )
			menu.OnFocus( w, x, y );
		if( w )
		{
			Param2<string, string> p = m_TextMap.Get( w.GetUserID() );
			if( p )
			{
				m_DetailsRoot.Show( true );
				m_DetailsLabel.SetText( p.param1 );
				m_DetailsText.SetText( p.param2 );
				int sx, sy;
				float lines = m_DetailsText.GetContentHeight();
				m_DetailsText.SetSize( 1, lines );
				
				m_DetailsText.Update();
				m_DetailsLabel.Update();
				m_DetailsRoot.Update();
				return true;
			}
			
			if( IsFocusable( w ) )
			{
				ColorRed( w );
				return true;
			}
		}
		m_DetailsRoot.Show( false );
		return ( w != null );
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, null );
			return true;
		}
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, enterW );
			return true;
		}
		return false;
	}
	
	bool IsFocusable( Widget w )
	{
		return ( w == m_Keybindings );
	}
	
	bool IsChanged()
	{
		return false;
	}
	
	void Apply()
	{
		
	}
	
	void Revert()
	{
		m_ControllerVSensitivitySelector.SetValue( m_ControllerVSensitivityOption.ReadValue(), false );
		m_ControllerHSensitivitySelector.SetValue( m_ControllerHSensitivityOption.ReadValue(), false );
		m_ControllerInvertSelector.SetValue( m_ControllerInvertOption.GetIndex(), false );
		
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
		m_VSensitivitySelector.SetValue( m_VSensitivityOption.ReadValue(), false );
		m_HSensitivitySelector.SetValue( m_HSensitivityOption.ReadValue(), false );
		m_InvertSelector.SetValue( m_InvertOption.GetIndex(), false );
		#endif
		#endif
	}
	
#ifdef PLATFORM_WINDOWS
#ifndef PLATFORM_CONSOLE
	void UpdateVerticalSensitivity( float value )
	{
		m_VSensitivityOption.WriteValue( value );
		m_Menu.OnChanged();
	}
	
	void UpdateHorizontalSensitivity( float value )
	{
		m_HSensitivityOption.WriteValue( value );
		m_Menu.OnChanged();
	}
	
	void UpdateInvertView( int index )
	{
		m_InvertOption.Switch();
		m_Menu.OnChanged();
	}
#endif
#endif
	
	void UpdateControllerVerticalSensitivity( float value )
	{
		m_ControllerVSensitivityOption.WriteValue( value );
		m_Menu.OnChanged();
	}
	
	void UpdateControllerHorizontalSensitivity( float value )
	{
		m_ControllerHSensitivityOption.WriteValue( value );
		m_Menu.OnChanged();
	}
	
	void UpdateControllerInvertView( int index )
	{
		m_ControllerInvertOption.Switch();
		m_Menu.OnChanged();
	}
	
	void ReloadOptions()
	{
		m_Menu.ReloadOptions();
	}
	
	void SetOptions( GameOptions options )
	{
		m_Options = options;
	}
	
	void FillTextMap()
	{
		m_TextMap = new map<int, ref Param2<string, string>>;
		m_TextMap.Insert( AT_CONFIG_YAXIS, new Param2<string, string>( "Vertical Sensitivity", "#options_controls_vertical_sensitivity_desc" ) );
		m_TextMap.Insert( AT_CONFIG_XAXIS, new Param2<string, string>( "Horizontal Sensitivity", "#options_controls_horizontal_sensitivity_desc" ) );
		m_TextMap.Insert( AT_CONFIG_YREVERSED, new Param2<string, string>( "Invert Vertical View", "#options_controls_invert_vertical_view_desc" ) );
		m_TextMap.Insert( AT_CONFIG_CONTROLLER_YAXIS, new Param2<string, string>( "Vertical Sensitivity", "#options_controls_vertical_sens_contr_desc" ) );
		m_TextMap.Insert( AT_CONFIG_CONTROLLER_XAXIS, new Param2<string, string>( "Horizontal Sensitivity", "#options_controls_horizontal_sens_contr_desc" ) );
		m_TextMap.Insert( AT_CONFIG_CONTROLLER_REVERSED_LOOK, new Param2<string, string>( "Invert Vertical View", "#options_controls_invert_vert_view_contr_desc" ) );
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w )
	{
		SetFocus( w );
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 0, 0 ) );
			button.SetAlpha( 0.9 );
		}
	}
	
	void ColorWhite( Widget w, Widget enterW )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
			button.SetAlpha( 0.75 );
		}
	}
}