class CharacterCreationMenu extends UIScriptedMenu
{
	#ifdef PLATFORM_WINDOWS
	DayZIntroScenePC										m_Scene;
	#else
	DayZIntroSceneXbox										m_Scene;
	#endif
	
	protected Widget										m_CharacterRotationFrame;
	protected Widget										m_Apply;
	protected Widget										m_RandomizeCharacter;
	protected Widget										m_BackButton;
	protected TextWidget									m_Version;
	
	protected ref OptionSelectorEditbox						m_NameSelector;
	protected ref OptionSelectorMultistateCharacterMenu		m_GenderSelector;
	protected ref OptionSelectorMultistateCharacterMenu		m_SkinSelector;
	protected ref OptionSelectorMultistateCharacterMenu		m_TopSelector;
	protected ref OptionSelectorMultistateCharacterMenu		m_BottomSelector;
	protected ref OptionSelectorMultistateCharacterMenu		m_ShoesSelector;
	
	void CharacterCreationMenu()
	{
		MissionMainMenu mission = MissionMainMenu.Cast( GetGame().GetMission() );
		
		#ifdef PLATFORM_WINDOWS
		m_Scene = mission.GetIntroScenePC();
		#else
		m_Scene = mission.GetIntroSceneXbox();
		#endif
		
		m_Scene.ResetIntroCamera();
	}
	
	PlayerBase GetPlayerObj()
	{
		#ifdef PLATFORM_WINDOWS
		return m_Scene.GetIntroCharacter().GetCharacterObj();
		#else
		return m_Scene.GetIntroCharacter().GetCharacterObj();
		#endif
	}
	
	override Widget Init()
	{
		#ifdef PLATFORM_CONSOLE
			layoutRoot = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/character_creation/xbox/character_creation.layout" );
		#else
			layoutRoot = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/character_creation/pc/character_creation.layout" );
		#endif
		
		m_CharacterRotationFrame			= layoutRoot.FindAnyWidget( "character_rotation_frame" );
		m_Apply								= layoutRoot.FindAnyWidget( "apply" );
		m_RandomizeCharacter				= layoutRoot.FindAnyWidget( "randomize_character" );
		m_BackButton						= layoutRoot.FindAnyWidget( "back" );
		m_Version							= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		#else
			version = "#main_menu_version" + " " + version;
		#endif
		m_Version.SetText( version );
		
		m_NameSelector		= new OptionSelectorEditbox( layoutRoot.FindAnyWidget( "character_name_setting_option" ), m_Scene.GetIntroCharacter().GetCharacterName(), null, false );
		m_GenderSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_gender_setting_option" ), 0, null, false, m_Scene.GetIntroCharacter().GetCharGenderList() );
		if ( m_Scene.GetIntroCharacter().IsCharacterFemale() )
		{
			m_GenderSelector.SetValue( "Female" );
			m_SkinSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, m_Scene.GetIntroCharacter().GetCharList( ECharGender.Female ) );
		}
		else
		{
			m_GenderSelector.SetValue( "Male" );
			m_SkinSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, m_Scene.GetIntroCharacter().GetCharList( ECharGender.Male ) );
		}
		
		m_TopSelector		= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_top_setting_option" ), 0, null, false, m_Scene.GetIntroCharacter().GetCharShirtsList() );
		m_BottomSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_bottom_setting_option" ), 0, null, false, m_Scene.GetIntroCharacter().GetCharPantsList() );
		m_ShoesSelector		= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_shoes_setting_option" ), 0, null, false, m_Scene.GetIntroCharacter().GetCharShoesList() );
		
		if( m_Scene && m_Scene.GetIntroCharacter() )
		{
			PlayerBase scene_char = GetPlayerObj();
			if( scene_char )
			{
				Object obj = scene_char.GetInventory().FindAttachment(InventorySlots.BODY);
				if( obj )
					m_TopSelector.SetValue( obj.GetType() );
				
				obj = scene_char.GetInventory().FindAttachment(InventorySlots.LEGS);
				if( obj )
					m_BottomSelector.SetValue( obj.GetType() );
				
				obj = scene_char.GetInventory().FindAttachment(InventorySlots.FEET);
				if( obj )
					m_ShoesSelector.SetValue( obj.GetType() );
				
				m_SkinSelector.SetValue( scene_char.GetType() );
			}
		}
		
		m_GenderSelector.m_OptionChanged.Insert( GenderChanged );
		m_SkinSelector.m_OptionChanged.Insert( SkinChanged );
		m_TopSelector.m_OptionChanged.Insert( TopChanged );
		m_BottomSelector.m_OptionChanged.Insert( BottomChanged );
		m_ShoesSelector.m_OptionChanged.Insert( ShoesChanged );
		
		#ifdef PLATFORM_PS4
			string confirm = "cross";
			string back = "circle";
			if( GetGame().GetInput().GetEnterButton() == GamepadButton.A )
			{
				confirm = "cross";
				back = "circle";
			}
			else
			{
				confirm = "circle";
				back = "cross";
			}
		
			ImageWidget toolbar_a = layoutRoot.FindAnyWidget( "SelectIcon" );
			ImageWidget toolbar_b = layoutRoot.FindAnyWidget( "BackIcon" );
			ImageWidget toolbar_x = layoutRoot.FindAnyWidget( "ResetIcon" );
			ImageWidget toolbar_y = layoutRoot.FindAnyWidget( "RandomizeIcon" );
			toolbar_a.LoadImageFile( 0, "set:playstation_buttons image:" + confirm );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:" + back );
			toolbar_x.LoadImageFile( 0, "set:playstation_buttons image:square" );
			toolbar_y.LoadImageFile( 0, "set:playstation_buttons image:triangle" );
		#endif
		
		Refresh();
		SetCharacter();
		CheckNewOptions();
		return layoutRoot;
	}
	
	void ~CharacterCreationMenu()
	{
		m_GenderSelector.m_OptionChanged.Remove( GenderChanged );
		m_SkinSelector.m_OptionChanged.Remove( SkinChanged );
		m_TopSelector.m_OptionChanged.Remove( TopChanged );
		m_BottomSelector.m_OptionChanged.Remove( BottomChanged );
		m_ShoesSelector.m_OptionChanged.Remove( ShoesChanged );
	}
	
	//Button Events
	void Apply()
	{
		//g_Game.SetPlayerGameName( m_PlayerName.GetText() );
		m_Scene.GetIntroCharacter().SaveCharacterSetup();
		if ( m_Scene.GetIntroCharacter().IsDefaultCharacter() )
		{
			m_Scene.GetIntroCharacter().SaveDefaultCharacter();
			m_Scene.GetIntroCharacter().SetToDefaultCharacter();
		}
		string name = m_NameSelector.GetValue();
		if( name == "" )
			name = GameConstants.DEFAULT_CHARACTER_NAME;
		
		m_Scene.GetIntroCharacter().SaveCharName(name);
		
		//SaveCharacters
		
		GetGame().GetUIManager().Back();
	}
	
	void Back()
	{
		GetGame().GetUIManager().Back();
	}
	
	void SetCharacter()
	{
		if (m_Scene.GetIntroCharacter().IsDefaultCharacter())
		{
			m_Scene.GetIntroCharacter().SetAttachment( m_TopSelector.GetStringValue(), InventorySlots.BODY );
			m_Scene.GetIntroCharacter().SetAttachment( m_BottomSelector.GetStringValue(), InventorySlots.LEGS );
			m_Scene.GetIntroCharacter().SetAttachment( m_ShoesSelector.GetStringValue(), InventorySlots.FEET );
			
			if (g_Game.IsNewCharacter())
			{
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.SHOULDER);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.BOW);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.MELEE);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.VEST);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.HIPS);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.BACK);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.HEADGEAR);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.MASK);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.EYEWEAR);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.GLOVES);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.ARMBAND);
				m_Scene.GetIntroCharacter().SetAttachment("", InventorySlots.HANDS);
			}
			
			//GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( m_Scene.SceneCharacterSetPos, 250 );
		}
	}
	
	void RandomizeCharacter()
	{
		m_Scene.GetIntroCharacter().SetToDefaultCharacter();
		
		// make random selection
		m_Scene.GetIntroCharacter().SetCharacterGender( Math.RandomInt(0, 2) );
		
		if ( m_Scene.GetIntroCharacter().IsCharacterFemale() )
		{
			m_GenderSelector.SetValue( "Female" );
			m_SkinSelector.LoadNewValues( m_Scene.GetIntroCharacter().GetCharList( ECharGender.Female ), 0 );
			m_SkinSelector.SetRandomValue();
		}
		else
		{
			m_GenderSelector.SetValue( "Male" );
			m_SkinSelector.LoadNewValues( m_Scene.GetIntroCharacter().GetCharList( ECharGender.Male ), 0 );
			m_SkinSelector.SetRandomValue();
		}
		
		m_TopSelector.SetRandomValue();
		m_BottomSelector.SetRandomValue();
		m_ShoesSelector.SetRandomValue();
		
		SetCharacter();
		
		CheckNewOptions();
	}
	
	//Selector Events
	void GenderChanged()
	{
		ECharGender gender = ECharGender.Male;
		
		if ( m_GenderSelector.GetStringValue() == "Female" )
		{
			gender = ECharGender.Female;
		}
		
		m_Scene.GetIntroCharacter().SetCharacterGender( gender );
		
		m_SkinSelector.LoadNewValues( m_Scene.GetIntroCharacter().GetCharList( gender ) , 0 );
		m_SkinSelector.SetRandomValue();
	}
	
	void SkinChanged()
	{
		m_Scene.GetIntroCharacter().CreateNewCharacterByName( m_SkinSelector.GetStringValue() );
		
		layoutRoot.FindAnyWidget( "character_root" ).Show( m_Scene.GetIntroCharacter().IsDefaultCharacter() );
	}
	
	void TopChanged()
	{
		m_Scene.GetIntroCharacter().SetAttachment( m_TopSelector.GetStringValue(), InventorySlots.BODY );
	}
	
	void BottomChanged()
	{
		m_Scene.GetIntroCharacter().SetAttachment( m_BottomSelector.GetStringValue(), InventorySlots.LEGS );
	}
	
	void ShoesChanged()
	{
		m_Scene.GetIntroCharacter().SetAttachment( m_ShoesSelector.GetStringValue(), InventorySlots.FEET );
	}
	
	override bool OnKeyPress( Widget w, int x, int y, int key )
	{		
		super.OnKeyPress( w, x, y, key );
		return false;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( w == m_Apply )
		{
			Apply();
			return true;
		}
		else if ( w == m_RandomizeCharacter )
		{
			RandomizeCharacter();
			return true;
		}
		else if ( w == m_BackButton )
		{
			Back();
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		#ifndef PLATFORM_CONSOLE
		if ( w == m_CharacterRotationFrame )
		{
			if (m_Scene)
				DayZIntroScenePC.Cast( m_Scene ).CharacterRotationStart();
			return true;
		}
		#endif
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		#ifndef PLATFORM_CONSOLE
		if (m_Scene)
			DayZIntroScenePC.Cast( m_Scene ).CharacterRotationStop();
		#endif
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		ColorHighlight( w );
		return true;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		ColorNormal( w );
		SetFocus( null );
		
		return true;
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
			return ( w == m_Apply || w == m_RandomizeCharacter || w == m_BackButton );
		}
		return false;
	}
	
	void CheckNewOptions()
	{
		bool show_widgets = m_Scene.GetIntroCharacter().IsDefaultCharacter();
		layoutRoot.FindAnyWidget( "character_gender_button" ).Show( show_widgets );
		layoutRoot.FindAnyWidget( "character_head_button" ).Show( show_widgets );
		layoutRoot.FindAnyWidget( "character_top_button" ).Show( show_widgets );
		layoutRoot.FindAnyWidget( "character_bottom_button" ).Show( show_widgets );
		layoutRoot.FindAnyWidget( "character_shoes_button" ).Show( show_widgets );
	}
	
	override void OnShow()
	{
#ifdef PLATFORM_CONSOLE
		m_GenderSelector.Focus();
#endif
		CheckNewOptions();
		
		/*
		if( m_Scene && m_Scene.GetIntroCamera() )
		{
			m_Scene.GetIntroCamera().LookAt( m_Scene.GetIntroCharacter().GetPosition() + Vector( 0, 1, 0 ) );
		}
		*/
	}
	
	override void Refresh()
	{
		string name;
		#ifdef PLATFORM_CONSOLE
			if( GetGame().GetUserManager() && GetGame().GetUserManager().GetSelectedUser() )
			{
				name = GetGame().GetUserManager().GetSelectedUser().GetName();
				if( name.LengthUtf8() > 16 )
				{
					name = name.SubstringUtf8(0, 16);
					name += "...";
				}
			}
		#else
			name = m_Scene.GetIntroCharacter().GetCharacterName();
			if( name == "" )
				name = GameConstants.DEFAULT_CHARACTER_NAME;
		#endif
		
		m_NameSelector.SetValue( name );
		
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		#else
			version = "#main_menu_version" + " " + version;
		#endif
		m_Version.SetText( version );
	}
	
	override void Update(float timeslice)
	{
		if ( GetGame().GetInput().LocalPress("UAUIBack",false) )
		{
			Back();
		}
		
		if ( GetGame().GetInput().LocalPress("UAUICtrlX",false) )
		{
			RandomizeCharacter();
		}
	}
	
	override void OnHide()
	{
		//super.OnHide();
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
		Widget option		= Widget.Cast( w.FindAnyWidget( w.GetName() + "_option_wrapper" ) );
		Widget option_label = w.FindAnyWidget( "option_label" );
		
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
		
		if ( option_label )
		{
			option_label.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
	}
	
	void ColorNormal( Widget w )
	{
		//Print("ColorNormal -> "+ w.GetName());
		//DumpStack();
		
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		TextWidget text1	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_text_1" ) );
		TextWidget text3	= TextWidget.Cast(w.FindAnyWidget( w.GetName() + "_label" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindAnyWidget( w.GetName() + "_image" ) );
		Widget option		= w.FindAnyWidget( w.GetName() + "_option_wrapper" );
		Widget option_label = w.FindAnyWidget( "option_label" );
		
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
		
		if ( option_label )
		{
			option_label.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
	}
	
	void ColorDisable( Widget w )
	{
		#ifdef PLATFORM_WINDOWS
		SetFocus( null );
		#endif
		
		if ( w )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			if( button )
			{
				button.SetTextColor( ColorManager.COLOR_DISABLED_TEXT );
			}
		}
	}
}