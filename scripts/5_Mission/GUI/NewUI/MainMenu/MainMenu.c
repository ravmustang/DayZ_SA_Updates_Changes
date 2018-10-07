class MainMenu extends UIScriptedMenu
{
	protected ref MainMenuNewsfeed	m_Newsfeed;
	protected ref MainMenuStats		m_Stats;
	protected ref MainMenuVideo		m_Video;
	
	protected MissionMainMenu		m_Mission;
	protected DayZIntroScene 		m_ScenePC;
	
	protected TextWidget			m_PlayerName;
	protected TextWidget			m_Version;
	
	protected Widget				m_CharacterRotationFrame;
	
	protected Widget				m_Play;
	protected ButtonWidget			m_ChooseServer;
	protected ButtonWidget			m_CustomizeCharacter;
	protected ButtonWidget			m_Options;
	protected ButtonWidget			m_PlayVideo;
	protected ButtonWidget			m_Tutorials;
	protected Widget				m_StatButton;
	protected Widget				m_MessageButton;
	protected Widget				m_SettingsButton;
	protected Widget				m_Exit;
	protected Widget				m_NewsFeedOpen;
	protected Widget				m_NewsFeedClose;
	protected Widget				m_CharacterStatsOpen;
	protected Widget				m_CharacterStatsClose;
	protected Widget				m_NewsMain;
	protected Widget				m_NewsSec1;
	protected Widget				m_NewsSec2;
	protected Widget				m_PrevCharacter;
	protected Widget				m_NextCharacter;
	
	protected Widget				m_LastPlayedTooltip;
	protected Widget				m_LastPlayedTooltipLabel;
	protected TextWidget			m_LastPlayedTooltipIP;
	protected TextWidget			m_LastPlayedTooltipPort;
	
	protected ref WidgetFadeTimer	m_LastPlayedTooltipTimer;

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/main_menu.layout" );
		
		m_Play						= layoutRoot.FindAnyWidget( "play" );
		m_ChooseServer				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "choose_server" ) );
		m_CustomizeCharacter		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "customize_character" ) );
		m_Options					= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "options" ) );
		m_PlayVideo					= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "play_video" ) );
		m_Tutorials					= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "tutorials" ) );
		m_StatButton				= layoutRoot.FindAnyWidget( "stat_button" );
		m_MessageButton				= layoutRoot.FindAnyWidget( "message_button" );
		m_SettingsButton			= layoutRoot.FindAnyWidget( "settings_button" );
		m_Exit						= layoutRoot.FindAnyWidget( "exit_button" );
		m_NewsFeedOpen				= layoutRoot.FindAnyWidget( "news_feed_open" );
		m_NewsFeedClose				= layoutRoot.FindAnyWidget( "news_feed_close" );
		m_CharacterStatsOpen		= layoutRoot.FindAnyWidget( "character_stats_open" );
		m_CharacterStatsClose		= layoutRoot.FindAnyWidget( "character_stats_close" );
		m_PrevCharacter				= layoutRoot.FindAnyWidget( "prev_character" );
		m_NextCharacter				= layoutRoot.FindAnyWidget( "next_character" );

		m_Version					= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		m_CharacterRotationFrame	= layoutRoot.FindAnyWidget( "character_rotation_frame" );
		
		m_LastPlayedTooltip			= layoutRoot.FindAnyWidget( "last_server_info" );
		m_LastPlayedTooltipLabel	= m_LastPlayedTooltip.FindAnyWidget( "last_server_info_label" );
		m_LastPlayedTooltipIP		= TextWidget.Cast( m_LastPlayedTooltip.FindAnyWidget( "last_server_info_ip" ) );
		m_LastPlayedTooltipPort		= TextWidget.Cast( m_LastPlayedTooltip.FindAnyWidget( "last_server_info_port" ) );
		
		m_LastPlayedTooltipTimer	= new WidgetFadeTimer();
		
		m_Newsfeed					= new MainMenuNewsfeed( layoutRoot.FindAnyWidget( "news_feed_root" ) );
		m_Stats						= new MainMenuStats( layoutRoot.FindAnyWidget( "character_stats_root" ) );
		
		m_Mission					= MissionMainMenu.Cast( GetGame().GetMission() );
		
#ifdef PLATFORM_CONSOLE
		
#else
		m_ScenePC					= m_Mission.GetIntroScenePC();
#endif
		m_Mission.ResetIntroCamera();
		
		m_PlayerName				= TextWidget.Cast( layoutRoot.FindAnyWidget("character_name_text") );
		
		#ifdef PLATFORM_CONSOLE
			m_PlayerName			= TextWidget.Cast( layoutRoot.FindAnyWidget("character_name_xbox") );
			layoutRoot.FindAnyWidget( "character_name_xbox_background" ).Show( true );
			layoutRoot.FindAnyWidget( "settings_panel_root" ).Show( false );
			layoutRoot.FindAnyWidget( "toolbar_bg" ).Show( true );
			layoutRoot.FindAnyWidget( "character" ).Show( false );
			m_CustomizeCharacter.Show( false );
			m_Tutorials.Show( true );
			m_Options.Show( true );
		#endif
		#ifdef PLATFORM_XBOX
			m_ChooseServer.SetText( "CONTROLS" );
			m_PlayVideo.Show( true );
		#endif
		
		Refresh();
		
		string version;
		GetGame().GetVersion( version );
		if( version != "" )
			m_Version.SetText( "#main_menu_version" + " " + version );
		else
			m_Version.Show( false );
		
		GetGame().GetUIManager().ScreenFadeOut(0);

		#ifdef PLATFORM_CONSOLE
			ColorRed( m_Play );
		#else
			SetFocus( layoutRoot );
		#endif
		
		#ifdef PLATFORM_CONSOLE
		string launch_done;
		if( !GetGame().GetProfileString( "FirstLaunchDone", launch_done ) || launch_done != "true" )
		{
			GetGame().SetProfileString( "FirstLaunchDone", "true" );
			GetGame().GetUIManager().ShowDialog( "TUTORIALS", "Would you like to see a basic tutorial?", 555, DBT_YESNO, DBB_YES, DMT_QUESTION, this );
			GetGame().SaveProfile();
		}
		#endif
		
		#ifdef PLATFORM_PS4
			m_ChooseServer.Show( false );
			ImageWidget toolbar_a = layoutRoot.FindAnyWidget( "SelectIcon" );
			ImageWidget toolbar_b = layoutRoot.FindAnyWidget( "BackIcon" );
			toolbar_a.LoadImageFile( 0, "set:playstation_buttons image:cross" );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:circle" );
		#endif
		
		UpdateNewsFeed();
		
		return layoutRoot;
	}
	
	void UpdateNewsFeed()
	{
		#ifdef PLATFORM_XBOX
			layoutRoot.FindAnyWidget( "news_feed_root" ).Show( false );
		
			if( OnlineServices.IsGameTrial( false ) )
		    {
				layoutRoot.FindAnyWidget( "news_feed_root_xbox_trial" ).Show( true );
				layoutRoot.FindAnyWidget( "news_feed_root_xbox" ).Show( false );
		    }
		    else
		    {
				layoutRoot.FindAnyWidget( "news_feed_root_xbox_trial" ).Show( false );
				layoutRoot.FindAnyWidget( "news_feed_root_xbox" ).Show( true );
		    }
		#endif		
		#ifdef PLATFORM_PS4
			layoutRoot.FindAnyWidget( "news_feed_root" ).Show( false );
			layoutRoot.FindAnyWidget( "news_feed_root_xbox" ).Show( false );
			layoutRoot.FindAnyWidget( "news_feed_root_xbox_trial" ).Show( false );
		#endif
		#ifdef PLATFORM_WINDOWS
			layoutRoot.FindAnyWidget( "news_feed_root" ).Show( true );
			layoutRoot.FindAnyWidget( "news_feed_root_xbox" ).Show( false );
			layoutRoot.FindAnyWidget( "news_feed_root_xbox_trial" ).Show( false );
		#endif
	}
	
	void ~MainMenu()
	{
		
	}
	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		if ( w == m_CharacterRotationFrame )
		{
			if (m_ScenePC)
				m_ScenePC.CharacterRotationStart();
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if (m_ScenePC)
			m_ScenePC.CharacterRotationStop();
		return false;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_Play )
			{
				Play();
				return true;
			}
			else if ( w == m_ChooseServer )
			{
				ChooseServer();
				return true;
			}
			else if ( w == m_CustomizeCharacter )
			{
				CustomizeCharacter();
				return true;
			}
			else if ( w == m_Options || w == m_SettingsButton )
			{
				OpenSettings();
				return true;
			}
			else if ( w == m_StatButton )
			{
				OpenStats();
				return true;
			}
			else if ( w == m_MessageButton )
			{
				OpenMessages();
				return true;
			}
			else if ( w == m_Exit )
			{
				Exit();
				return true;
			}
			else if ( w == m_PrevCharacter )
			{
				PreviousCharacter();
				return true;
			}
			else if ( w == m_NextCharacter )
			{
				NextCharacter();
				return true;
			}
			else if ( w == m_NewsFeedOpen )
			{
				ShowNewsfeed();
				return true;
			}
			else if ( w == m_NewsFeedClose )
			{
				HideNewsfeed();
				return true;
			}
			else if ( w == m_CharacterStatsOpen )
			{
				ShowStats();
				return true;
			}
			else if ( w == m_CharacterStatsClose )
			{
				HideStats();
				return true;
			}
			else if ( w == m_PlayVideo )
			{
				PlayVideo();
				return true;
			}
			else if ( w == m_Tutorials )
			{
				OpenTutorials();
				return true;
			}
		}
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		#ifdef PLATFORM_WINDOWS
		if( w == m_Play )
		{
			string ip;
			int port;
			if( g_Game.GetLastVisitedServer( ip, port ) )
			{
				m_LastPlayedTooltipIP.SetText( "Server IP: " + ip );
				m_LastPlayedTooltipPort.SetText( "Server port: " + port.ToString() );
				m_LastPlayedTooltipTimer.FadeIn( m_LastPlayedTooltip, 0.3, true );
			}
		}
		#endif
		if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		#ifdef PLATFORM_WINDOWS
		if( w == m_Play )
		{
			m_LastPlayedTooltipTimer.FadeOut( m_LastPlayedTooltip, 5, true );
		}
		#endif
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
		if( w == m_Play || w == m_ChooseServer || w == m_CustomizeCharacter || w == m_StatButton || w == m_MessageButton || w == m_SettingsButton );
			return true;
		if( w == m_Exit || w == m_NewsFeedOpen || w == m_NewsFeedClose || w == m_PlayVideo || w == m_Options );
			return true;
		if( w == m_CharacterStatsOpen || w == m_CharacterStatsClose || w == m_NewsMain || w == m_NewsSec1 || w == m_NewsSec2 || w == m_PrevCharacter || w == m_NextCharacter );
			return true;
		return false;
	}
	
	override void Refresh()
	{
		string name;
		
		#ifdef PLATFORM_CONSOLE
			if( GetGame().GetUserManager() && GetGame().GetUserManager().GetSelectedUser() )
			{
				name = GetGame().GetUserManager().GetSelectedUser().GetName();
				if( name.LengthUtf8() > 18 )
				{
					name = name.SubstringUtf8(0, 18);
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
	
	override void OnShow()
	{
		#ifdef PLATFORM_CONSOLE
			ColorRed( m_Play );
		#else
			SetFocus( layoutRoot );
		#endif
		Refresh();
		
		if( m_ScenePC && m_ScenePC.GetCamera() )
		{
			m_ScenePC.GetCamera().LookAt( m_ScenePC.GetIntroSceneCharacter().GetPosition() + Vector( 0, 1, 0 ) );
		}
	}
	
	override void OnHide()
	{
		//super.OnHide();
	}

#ifdef PLATFORM_CONSOLE
	override void Update(float timeslice)
	{
		if ( GetGame().GetInput().GetActionDown(UAUIBack, false) )
		{
			g_Game.SetLoadState( DayZLoadState.MAIN_MENU_START );
			#ifndef PLATFORM_WINDOWS
			GetGame().GetInput().ResetActiveGamepad();
			#endif
		}
	}
#endif
	
	void Play()
	{
		if (m_ScenePC && m_ScenePC.GetIntroSceneCharacter())
		{
			//saves demounit for further use
			if (m_ScenePC.GetIntroSceneCharacter().GetInventory().FindAttachment(InventorySlots.BODY) && m_ScenePC.GetCurrentCharacterID() == -1)
			{
				m_ScenePC.SaveCharacterSetup();
			}
			
			m_ScenePC.SaveCharName();
		}

		#ifdef PLATFORM_XBOX
			if( OnlineServices.IsGameTrial( false ) )
		    {
				AutoConnect();
		    }
		    else
		    {
				EnterScriptedMenu(MENU_SERVER_BROWSER);
		    }
		#else
			#ifdef PLATFORM_PS4
				EnterScriptedMenu(MENU_SERVER_BROWSER);
			#else
				if( !g_Game.IsNewCharacter() )
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallByName(this, "ConnectLastSession");
				else
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallByName(this, "ConnectBestServer");
			#endif
		#endif
	}

	void ChooseServer()
	{
		#ifdef PLATFORM_CONSOLE
			EnterScriptedMenu( MENU_XBOX_CONTROLS );
			return;
		#endif
		
		if( m_ScenePC )
			m_ScenePC.SaveCharName();
		
		#ifdef NEW_UI
			EnterScriptedMenu(MENU_SERVER_BROWSER);
		#else
			g_Game.GetUIManager().EnterServerBrowser(this);
		#endif
		
		//saves demounit for further use
		if (m_ScenePC && m_ScenePC.GetIntroSceneCharacter() && m_ScenePC.GetIntroSceneCharacter().GetInventory().FindAttachment(InventorySlots.BODY) && m_ScenePC.GetCurrentCharacterID() == -1)
		{
			m_ScenePC.SaveCharacterSetup();
		}
	}
	
	void CustomizeCharacter()
	{
		EnterScriptedMenu(MENU_CHARACTER);
	}
	
	void NextCharacter()
	{
		if ( m_ScenePC )
		{
			m_ScenePC.SaveCharName();
			m_ScenePC.ChangeCharacter( m_ScenePC.NextCharacterID() );
			
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( m_ScenePC.SceneCharacterSetPos, 250 );
			m_PlayerName.SetText( g_Game.GetPlayerGameName() );
		}
	}
	
	void PreviousCharacter()
	{
		if ( m_ScenePC )
		{
			m_ScenePC.SaveCharName();
			m_ScenePC.ChangeCharacter( m_ScenePC.PrevCharacterID() );
			
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( m_ScenePC.SceneCharacterSetPos, 250 );
			m_PlayerName.SetText( g_Game.GetPlayerGameName() );
		}
	}
	
	void OpenStats()
	{
		
	}
	
	void OpenMessages()
	{
		
	}
	
	void ShowStats()
	{
		m_Stats.ShowStats();
		m_CharacterStatsOpen.Show( false );
	}
	
	void HideStats()
	{
		m_CharacterStatsOpen.Show( true );
		m_Stats.HideStats();
	}
	
	void ShowNewsfeed()
	{
		m_Newsfeed.ShowNewsfeed();
		m_NewsFeedOpen.Show( false );
	}
	
	void HideNewsfeed()
	{
		m_Newsfeed.HideNewsfeed();
		m_NewsFeedOpen.Show( true );
	}
	
	void OpenSettings()
	{
		EnterScriptedMenu(MENU_OPTIONS);
	}
	
	void PlayVideo()
	{
		EnterScriptedMenu(MENU_VIDEO);
	}
	
	void OpenTutorials()
	{
		EnterScriptedMenu(MENU_TUTORIAL);
	}
	
	void Exit()
	{
		GetGame().GetUIManager().ShowDialog("EXIT", "Are you sure you want to exit?", IDC_MAIN_QUIT, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
	}
		
	bool TryConnectLastSession( out string ip, out int port )
	{
		if( g_Game.GetLastVisitedServer( ip, port ) )
		{
			return true;
		}
		return false;
	}
	
	void AutoConnect()
	{
		OnlineServices.AutoConnectToEmptyServer();
	}
	
	void ConnectLastSession()
	{
		string ip;
		int port;
		
		if( TryConnectLastSession( ip, port ) )
		{
			g_Game.ConnectFromServerBrowser( ip, port );
		}
		else
		{
			#ifdef NEW_UI
				EnterScriptedMenu(MENU_SERVER_BROWSER);
			#else
				#ifdef PLATFORM_CONSOLE
					EnterScriptedMenu(MENU_SERVER_BROWSER);
				#else
					g_Game.GetUIManager().EnterServerBrowser(this);
				#endif
			#endif
		}
	}
	
	void ConnectBestServer()
	{
		ConnectLastSession();
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w )
	{
		SetFocus( w );
		
		ButtonWidget button = ButtonWidget.Cast( w );
		if( button && button != m_Play )
		{
			button.SetTextColor( ARGB( 255, 200, 0, 0 ) );
		}
		
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
		
		ButtonWidget button = ButtonWidget.Cast( w );
		if( button && button != m_Play )
		{
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
		}
		
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
	
	override bool OnModalResult( Widget w, int x, int y, int code, int result )
	{
		if( code == IDC_MAIN_QUIT )
		{
			if( result == 2 )
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(g_Game.RequestExit, IDC_MAIN_QUIT);
			#ifdef PLATFORM_WINDOWS
			if( result == 3 )
				ColorWhite( GetFocus(), null );
			#endif
			return true;
		}
		else if( code == 555 )
		{
			if( result == 2 )
				OpenTutorials();
		}
		return false;
	}
}
