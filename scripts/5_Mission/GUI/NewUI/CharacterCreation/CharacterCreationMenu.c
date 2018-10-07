class CharacterCreationMenu extends UIScriptedMenu
{
	DayZIntroScene							m_Scene;
	
	protected Widget						m_CharacterRotationFrame;
	protected Widget						m_Apply;
	protected Widget						m_RandomizeCharacter;
	protected Widget						m_BackButton;
	
	protected EditBoxWidget					m_PlayerName;
	protected TextWidget					m_Version;
	
	protected ref OptionSelectorMultistate	m_GenderSelector;
	protected ref OptionSelectorMultistate	m_SkinSelector;
	protected ref OptionSelectorMultistate	m_TopSelector;
	protected ref OptionSelectorMultistate	m_BottomSelector;
	protected ref OptionSelectorMultistate	m_ShoesSelector;
	
	void CharacterCreationMenu()
	{
		MissionMainMenu mission = MissionMainMenu.Cast( GetGame().GetMission() );
		
		#ifdef PLATFORM_CONSOLE
			//m_SceneXbox = mission.GetIntroSceneXbox();
			//m_SceneXbox.ResetIntroCamera();
		#else
		#ifdef PLATFORM_WINDOWS
			m_Scene = mission.GetIntroScenePC();
			m_Scene.ResetIntroCamera();
		#endif
		#endif
	}
	
	override Widget Init()
	{
		layoutRoot							= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/character_creation.layout" );
		
		m_CharacterRotationFrame			= layoutRoot.FindAnyWidget( "character_rotation_frame" );
		m_Apply								= layoutRoot.FindAnyWidget( "apply" );
		m_RandomizeCharacter				= layoutRoot.FindAnyWidget( "randomize_character" );
		m_BackButton						= layoutRoot.FindAnyWidget( "back" );
		
		m_PlayerName						= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "general_name_setting_text" ) );
		m_Version							= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		
		Refresh();
		
		string version;
		GetGame().GetVersion( version );
		if( version != "" )
			m_Version.SetText( "#main_menu_version" + " " + version );
		else
			m_Version.Show( false );
		
		m_GenderSelector					= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_gender_setting_option" ), 0, null, false, m_Scene.m_CharGenderList );
		if ( m_Scene.IsCharacterFemale() )
		{
			m_GenderSelector.SetValue( "Female" );
			m_SkinSelector	= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, m_Scene.m_CharPersonalityFemaleList );
		}
		else
		{
			m_GenderSelector.SetValue( "Male" );
			m_SkinSelector	= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, m_Scene.m_CharPersonalityMaleList );
		}
		
		m_TopSelector						= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_top_setting_option" ), 0, null, false, m_Scene.m_CharShirtList );
		m_BottomSelector					= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_bottom_setting_option" ), 0, null, false, m_Scene.m_CharPantsList );
		m_ShoesSelector						= new OptionSelectorMultistate( layoutRoot.FindAnyWidget( "character_shoes_setting_option" ), 0, null, false, m_Scene.m_CharShoesList );
		
		if( m_Scene && m_Scene.GetIntroSceneCharacter() )
		{
			PlayerBase scene_char = m_Scene.GetIntroSceneCharacter();
			
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
		
		m_GenderSelector.m_OptionChanged.Insert( GenderChanged );
		m_SkinSelector.m_OptionChanged.Insert( SkinChanged );
		m_TopSelector.m_OptionChanged.Insert( TopChanged );
		m_BottomSelector.m_OptionChanged.Insert( BottomChanged );
		m_ShoesSelector.m_OptionChanged.Insert( ShoesChanged );
		
		#ifdef PLATFORM_PS4
			ImageWidget toolbar_a = layoutRoot.FindAnyWidget( "SelectIcon" );
			ImageWidget toolbar_b = layoutRoot.FindAnyWidget( "BackIcon" );
			ImageWidget toolbar_x = layoutRoot.FindAnyWidget( "ResetIcon" );
			ImageWidget toolbar_y = layoutRoot.FindAnyWidget( "RandomizeIcon" );
			toolbar_a.LoadImageFile( 0, "set:playstation_buttons image:cross" );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:circle" );
			toolbar_x.LoadImageFile( 0, "set:playstation_buttons image:square" );
			toolbar_y.LoadImageFile( 0, "set:playstation_buttons image:triangle" );
		#endif
		
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
		g_Game.SetPlayerGameName( m_PlayerName.GetText() );
		m_Scene.SaveCharacterSetup();
		m_Scene.SaveCharName();
		m_Scene.SaveDefaultCharacter();
		SetCharacter();
		GetGame().GetUIManager().Back();
	}
	
	void SetCharacter()
	{
		if (m_Scene.GetIntroSceneCharacter())
		{
			m_PlayerName.SetText( g_Game.GetPlayerGameName() );
			
			m_Scene.SetAttachment( m_TopSelector.GetStringValue(), InventorySlots.BODY );
			m_Scene.SetAttachment( m_BottomSelector.GetStringValue(), InventorySlots.LEGS );
			m_Scene.SetAttachment( m_ShoesSelector.GetStringValue(), InventorySlots.FEET );
			
			if (g_Game.IsNewCharacter())
			{
				m_Scene.SetAttachment("", InventorySlots.SHOULDER);
				m_Scene.SetAttachment("", InventorySlots.BOW);
				m_Scene.SetAttachment("", InventorySlots.MELEE);
				m_Scene.SetAttachment("", InventorySlots.VEST);
				m_Scene.SetAttachment("", InventorySlots.HIPS);
				m_Scene.SetAttachment("", InventorySlots.BACK);
				m_Scene.SetAttachment("", InventorySlots.HEADGEAR);
				m_Scene.SetAttachment("", InventorySlots.MASK);
				m_Scene.SetAttachment("", InventorySlots.EYEWEAR);
				m_Scene.SetAttachment("", InventorySlots.GLOVES);
				m_Scene.SetAttachment("", InventorySlots.ARMBAND);
				m_Scene.SetAttachment("", InventorySlots.HANDS);
			}
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( m_Scene.SceneCharacterSetPos, 250 );
		}
	}
	
	void RandomizeCharacter()
	{
		g_Game.SetNewCharacter(true);
		
		m_Scene.SetCurrentCharacterID( -1 );
		
		// make random selection
		m_Scene.RandomSelectGender();
		
		if ( m_Scene.IsCharacterFemale() )
		{
			m_GenderSelector.SetValue( "Female" );
			m_SkinSelector.LoadNewValues( m_Scene.m_CharPersonalityFemaleList, 0 );
			m_SkinSelector.SetRandomValue();
		}
		else
		{
			m_GenderSelector.SetValue( "Male" );
			m_SkinSelector.LoadNewValues( m_Scene.m_CharPersonalityMaleList, 0 );
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
		m_Scene.SetCharacterFemale( ( m_GenderSelector.GetStringValue() == "Female" ) );
		
		if ( m_Scene.IsCharacterFemale() )
		{
			m_SkinSelector.LoadNewValues( m_Scene.m_CharPersonalityFemaleList, 0 );
			m_SkinSelector.SetRandomValue();
		}
		else
		{
			m_SkinSelector.LoadNewValues( m_Scene.m_CharPersonalityMaleList, 0 );
			m_SkinSelector.SetRandomValue();
		}
	}
	
	void SkinChanged()
	{
		m_Scene.CreateNewCharacter( m_SkinSelector.GetStringValue() );
		
		layoutRoot.FindAnyWidget( "character_root" ).Show( g_Game.IsNewCharacter() );
		
		TopChanged();
		BottomChanged();
		ShoesChanged();
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( m_Scene.SceneCharacterSetPos, 100 );
	}
	
	void TopChanged()
	{
		m_Scene.SetAttachment( m_TopSelector.GetStringValue(), InventorySlots.BODY );
	}
	
	void BottomChanged()
	{
		m_Scene.SetAttachment( m_BottomSelector.GetStringValue(), InventorySlots.LEGS );
	}
	
	void ShoesChanged()
	{
		m_Scene.SetAttachment( m_ShoesSelector.GetStringValue(), InventorySlots.FEET );
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
			GetGame().GetUIManager().Back();
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		if ( w == m_CharacterRotationFrame )
		{
			if (m_Scene)
				m_Scene.CharacterRotationStart();
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if (m_Scene)
			m_Scene.CharacterRotationStop();
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
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
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
	
	bool IsFocusable( Widget w )
	{
		return ( w == m_Apply || w == m_RandomizeCharacter || w == m_BackButton );
	}
	
	void CheckNewOptions()
	{
		layoutRoot.FindAnyWidget( "character_gender_button" ).Show( g_Game.IsNewCharacter() );
		layoutRoot.FindAnyWidget( "character_head_button" ).Show( g_Game.IsNewCharacter() );
		layoutRoot.FindAnyWidget( "character_top_button" ).Show( g_Game.IsNewCharacter() );
		layoutRoot.FindAnyWidget( "character_bottom_button" ).Show( g_Game.IsNewCharacter() );
		layoutRoot.FindAnyWidget( "character_shoes_button" ).Show( g_Game.IsNewCharacter() );
	}
	
	override void OnShow()
	{
		#ifdef PLATFORM_CONSOLE
		SetFocus( m_Apply );
		#endif
		
		CheckNewOptions();
		
		if( m_Scene && m_Scene.GetCamera() )
		{
			m_Scene.GetCamera().LookAt( m_Scene.GetIntroSceneCharacter().GetPosition() + Vector( 0, 1, 0 ) );
		}
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
			g_Game.GetPlayerNameShort( 14, name );
		#endif
		
		m_PlayerName.SetText( name );
		
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		#else
			version = "#main_menu_version" + " " + version;
		#endif
		m_Version.SetText( version );
	}
	
	override void OnHide()
	{
		//super.OnHide();
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w )
	{
		SetFocus( w );

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
	
	void ColorWhite( Widget w, Widget enterW )
	{
		#ifdef PLATFORM_WINDOWS
		SetFocus( null );
		#endif
		
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