class InGameMenuXbox extends UIScriptedMenu
{
	protected ref PlayerListScriptedWidget	m_ServerInfoPanel;
	
	protected Widget						m_OnlineMenu;
	
	protected TextWidget					m_Version;
	
	protected ButtonWidget					m_ContinueButton;
	protected ButtonWidget					m_ExitButton;
	protected ButtonWidget					m_RestartButton;
	protected ButtonWidget					m_OptionsButton;
	protected ButtonWidget					m_ControlsButton;
	protected ButtonWidget					m_OnlineButton;
	protected ButtonWidget					m_TutorialsButton;
	
	const int 								BUTTON_XBOX_CONTROLS = 201;
	
	void InGameMenuXbox()
	{
	}

	void ~InGameMenuXbox()
	{
		ClientData.SyncEvent_OnPlayerListUpdate.Remove( SyncEvent_OnRecievedPlayerList );
		OnlineServices.m_PermissionsAsyncInvoker.Remove( OnPermissionsUpdate );
		
		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.ToggleHud( g_Game.GetProfileOption( EDayZProfilesOptions.HUD ), true );
			}
		}
		PPEffects.SetBlurMenu( 0 );
	}
	
	override Widget Init()
	{
		layoutRoot		= GetGame().GetWorkspace().CreateWidgets("gui/layouts/xbox/day_z_ingamemenu_xbox.layout");
		m_OnlineMenu	= GetGame().GetWorkspace().CreateWidgets("gui/layouts/xbox/ingamemenu_xbox/online_info_menu.layout", layoutRoot);
		
		m_OnlineMenu.Show( false );
		
		m_ContinueButton	= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "continuebtn" ) );
		m_ExitButton		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "exitbtn" ) );
		m_RestartButton		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "restartbtn" ) );
		m_OptionsButton		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "optionsbtn" ) );
		m_ControlsButton	= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "controlsbtn" ) );
		m_OnlineButton		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "onlinebtn" ) );
		m_TutorialsButton	= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "tutorialsbtn" ) );
		m_Version			= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		
		#ifdef PLATFORM_PS4
		m_ControlsButton.Show( false );
		m_OnlineButton.Show( false );
		#endif
		
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		#else
			version = "#main_menu_version" + " " + version;
		#endif
		m_Version.SetText( version );
		
		Man player = GetGame().GetPlayer();
		bool player_is_alive = false;
		if (player)
		{
			int life_state = player.GetPlayerState();

			if (life_state == EPlayerStates.ALIVE)
			{
				player_is_alive = true;
			}
		}
		
		if( !player_is_alive )
		{
			layoutRoot.FindAnyWidget( "bottom" ).Show( false );
		}
		
		if( GetGame().IsMultiplayer() )
		{
			#ifdef PLATFORM_XBOX
			m_OnlineButton.Show( true );
			#endif
			
			m_ServerInfoPanel = new PlayerListScriptedWidget( m_OnlineMenu.FindAnyWidget( "ServerInfoPanel" ), "SERVER PLAYERS" );
			
			OnlineServices.m_PermissionsAsyncInvoker.Insert( OnPermissionsUpdate );
			ClientData.SyncEvent_OnPlayerListUpdate.Insert( SyncEvent_OnRecievedPlayerList );
			
			m_ServerInfoPanel.Reload( ClientData.m_PlayerList );
			m_ServerInfoPanel.ReloadLocal( OnlineServices.GetMuteList() );
			
			#ifndef PLATFORM_WINDOWS // if app is not on Windows with -XBOX parameter
				ClientData.m_OnlineServices.LoadPermissions( ClientData.GetSimplePlayerList() );
			#endif
			
			TextWidget mute_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "Mute" ).FindAnyWidget( "MuteText" ) );
			
			string uid = m_ServerInfoPanel.FindPlayerByWidget( GetFocus() );
			if( uid != "" )
			{
				if( IsLocalPlayer( uid ) || m_ServerInfoPanel.IsEmpty() )
				{
					layoutRoot.FindAnyWidget( "Mute" ).Show( false );
					layoutRoot.FindAnyWidget( "Gamercard" ).Show( false );
				}
				else
				{
					layoutRoot.FindAnyWidget( "Mute" ).Show( true );
					layoutRoot.FindAnyWidget( "Gamercard" ).Show( true );
					if( OnlineServices.IsPlayerMuted( uid ) )
					{
						mute_text.SetText( "Unmute" );
					}
					else
					{
						mute_text.SetText( "Mute" );
					}
				}
				
				if( m_ServerInfoPanel.IsGloballyMuted( uid ) )
				{
					layoutRoot.FindAnyWidget( "Mute" ).Show( false );
				}
			}
		}
		
		layoutRoot.FindAnyWidget( "Gamercard" ).Show( false );
		layoutRoot.FindAnyWidget( "Mute" ).Show( false );
		
		//RESPAWN & RESTART
		ButtonWidget restart_btn = ButtonWidget.Cast( layoutRoot.FindAnyWidgetById( IDC_INT_RETRY ) );
		if (GetGame().IsMultiplayer())
		{
			restart_btn.SetText("#main_menu_respawn");
		}
		else
		{
			restart_btn.SetText("#main_menu_restart");
		}
		
		if ( GetGame().IsMultiplayer() && !( GetGame().CanRespawnPlayer() || ( player && player.IsUnconscious() ) ) )
		{
			restart_btn.Enable( false );
			restart_btn.Show( false );
		}
		//
		
	#ifdef BULDOZER
		delete restart_btn;
	#endif
		
		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.ToggleHud( false, true );
			}
		}
		PPEffects.SetBlurMenu( 0.6 );
			
		#ifdef PLATFORM_PS4
			m_OptionsButton.Show( false );
		
			ImageWidget toolbar_a = layoutRoot.FindAnyWidget( "SelectIcon" );
			ImageWidget toolbar_b = layoutRoot.FindAnyWidget( "BackIcon" );
			ImageWidget toolbar_x = layoutRoot.FindAnyWidget( "GamercardIcon" );
			ImageWidget toolbar_y = layoutRoot.FindAnyWidget( "MuteIcon" );
			toolbar_a.LoadImageFile( 0, "set:playstation_buttons image:cross" );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:circle" );
			toolbar_x.LoadImageFile( 0, "set:playstation_buttons image:square" );
			toolbar_y.LoadImageFile( 0, "set:playstation_buttons image:triangle" );
		#endif
	
		return layoutRoot;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		Mission mission = GetGame().GetMission();

		switch (w.GetUserID())
		{
		case IDC_MAIN_CONTINUE:
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().Continue);
			return true;

		case IDC_MAIN_OPTIONS:
			EnterScriptedMenu(MENU_OPTIONS);
			return true;
		break;
			
		case BUTTON_XBOX_CONTROLS:
			EnterScriptedMenu(MENU_XBOX_CONTROLS);
			return true;

		case IDC_MAIN_QUIT:
			GetGame().GetUIManager().ShowDialog("EXIT", "Are you sure you want to exit?", IDC_INT_EXIT, DBT_YESNO, DBB_YES, DMT_QUESTION, NULL);
			return true;
		case IDC_INT_RETRY:
			if ( GetGame().IsMultiplayer() )
			{
				Respawn();
				
				return true;
			}
			else
			{
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().RestartMission);
			}
			
			return true;
		case IDC_MAIN_ONLINE:
		#ifndef PLATFORM_PS4
			m_OnlineMenu.Show( true );
		#endif
			layoutRoot.FindAnyWidget( "play_panel_root" ).Show( false );
			layoutRoot.FindAnyWidget( "dayz_logo" ).Show( false );
			layoutRoot.FindAnyWidget( "Select" ).Show( false );
			m_ServerInfoPanel.FocusFirst();
			return true;
		case 117:
			EnterScriptedMenu(MENU_TUTORIAL);
			return true;
		}

		return false;
	}
	
	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		super.OnModalResult(w, x, y, code, result);
		
		if ( code == IDC_INT_EXIT && result == DBB_YES )
		{
			GetGame().GetMission().AbortMission();
			g_Game.CancelQueueTime();	
			
			return true;		
		}
		else if ( code == IDC_INT_EXIT && result == DBB_NO )
		{
			g_Game.CancelQueueTime();
		}
		
		return false;
	}
	
	void Respawn()
	{
		//turns off dead screen, hides HUD for countdown
		//---------------------------------------------------
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if(player)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(player.ShowDeadScreen, DayZPlayerImplement.DEAD_SCREEN_DELAY, false, false);
		}
		GetGame().GetUIManager().CloseAll();
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().RespawnPlayer);
		//---------------------------------------------------
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().Continue);
	}
	
	bool IsLocalPlayer( string uid )
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		string local_uid;
		if( GetGame().GetUserManager() )
			local_uid = GetGame().GetUserManager().GetSelectedUser().GetUid();
		return ( uid == local_uid );
	}
	
	override void Update( float timeslice )
	{
		string uid;
		if( GetGame().IsMultiplayer() && layoutRoot.FindAnyWidget( "OnlineInfo" ).IsVisible() )
		{
			TextWidget mute_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "Mute" ).FindAnyWidget( "MuteText" ) );
			if( GetGame().GetInput().GetActionDown( UAUIUp, false ) )
			{
				if( m_ServerInfoPanel )
				{
					m_ServerInfoPanel.ScrollToEntry( m_ServerInfoPanel.FindEntryByWidget( GetFocus() ) );
					uid = m_ServerInfoPanel.FindPlayerByWidget( GetFocus() );
					if( uid != "" )
					{
						if( IsLocalPlayer( uid ) || m_ServerInfoPanel.IsEmpty() )
						{
							layoutRoot.FindAnyWidget( "Mute" ).Show( false );
							layoutRoot.FindAnyWidget( "Gamercard" ).Show( false );
						}
						else
						{
							layoutRoot.FindAnyWidget( "Mute" ).Show( true );
							layoutRoot.FindAnyWidget( "Gamercard" ).Show( true );
							if( OnlineServices.IsPlayerMuted( uid ) )
							{
								mute_text.SetText( "Unmute" );
							}
							else
							{
								mute_text.SetText( "Mute" );
							}
						}
						
						if( m_ServerInfoPanel.IsGloballyMuted( uid ) )
						{
							layoutRoot.FindAnyWidget( "Mute" ).Show( false );
						}
					}
				}
			}
			if( GetGame().GetInput().GetActionDown( UAUIDown, false ) )
			{
				if( m_ServerInfoPanel )
				{
					m_ServerInfoPanel.ScrollToEntry( m_ServerInfoPanel.FindEntryByWidget( GetFocus() ) );
					uid = m_ServerInfoPanel.FindPlayerByWidget( GetFocus() );
					if( uid != "" )
					{
						if( IsLocalPlayer( uid ) || m_ServerInfoPanel.IsEmpty() )
						{
							layoutRoot.FindAnyWidget( "Mute" ).Show( false );
							layoutRoot.FindAnyWidget( "Gamercard" ).Show( false );
						}
						else
						{
							layoutRoot.FindAnyWidget( "Mute" ).Show( true );
							layoutRoot.FindAnyWidget( "Gamercard" ).Show( true );
							if( OnlineServices.IsPlayerMuted( uid ) )
							{
								mute_text.SetText( "Unmute" );
							}
							else
							{
								mute_text.SetText( "Mute" );
							}
						}
						
						if( m_ServerInfoPanel.IsGloballyMuted( uid ) )
						{
							layoutRoot.FindAnyWidget( "Mute" ).Show( false );
						}
					}
				}
			}
			
			if( GetGame().GetInput().GetActionDown( UAUIFastEquipOrSplit, false ) )
			{
				bool muted;
				ScriptInputUserData ctx;
				if( m_ServerInfoPanel )
				{
					uid = m_ServerInfoPanel.FindPlayerByWidget( GetFocus() );
					if( uid == "" )
						return;
					if( !IsLocalPlayer( uid ) && !m_ServerInfoPanel.IsGloballyMuted( uid ) )
					{
						muted = OnlineServices.IsPlayerMuted( uid );
						if ( uid != "" && ScriptInputUserData.CanStoreInputUserData() )
						{
							ctx = new ScriptInputUserData;
							ctx.Write( INPUT_UDT_USER_MUTE_XBOX );
							ctx.Write( uid );
							ctx.Write( !muted );
							ctx.Send();
							OnlineServices.MutePlayer( uid, !muted );
							m_ServerInfoPanel.MutePlayer( uid, !muted );
							if( !muted )
							{
								mute_text.SetText( "Unmute" );
							}
							else
							{
								mute_text.SetText( "Mute" );
							}
						}
					}
				}
			}
			
			if( GetGame().GetInput().GetActionDown( UAQuickReload, false ) )
			{
				if( m_ServerInfoPanel )
				{
					uid = m_ServerInfoPanel.FindPlayerByWidget( GetFocus() );
				}
				if( uid == "" )
					return;
				OnlineServices.ShowUserProfile( uid );
			}
		}
		
		if ( GetGame().GetInput().GetActionDown(UAUIBack, false) )
		{
			//g_Game.CancelQueueTime();
		}
	}

	void SelectServer()
	{
		layoutRoot.FindAnyWidget( "Mute" ).Show( true );
		
		if( m_ServerInfoPanel )
		{
			m_ServerInfoPanel.FocusFirst();
			
			string uid = m_ServerInfoPanel.FindPlayerByWidget( GetFocus() );
			if( IsLocalPlayer( uid ) || m_ServerInfoPanel.IsEmpty() )
			{
				layoutRoot.FindAnyWidget( "Gamercard" ).Show( false );
				layoutRoot.FindAnyWidget( "Mute" ).Show( false );
			}
			else
			{
				layoutRoot.FindAnyWidget( "Gamercard" ).Show( true );
				layoutRoot.FindAnyWidget( "Mute" ).Show( true );
			}
		}
	}
	
	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
	{
		return false;
	}
	
	void SyncEvent_OnRecievedPlayerList( SyncPlayerList player_list )
	{
		m_ServerInfoPanel.Reload( player_list );
		TextWidget mute_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "Mute" ).FindAnyWidget( "MuteText" ) );
		string uid = m_ServerInfoPanel.FindPlayerByWidget( GetFocus() );
		if( uid != "" )
		{
			if( IsLocalPlayer( uid ) || m_ServerInfoPanel.IsEmpty() )
			{
				layoutRoot.FindAnyWidget( "Mute" ).Show( false );
				layoutRoot.FindAnyWidget( "Gamercard" ).Show( false );
			}
			else
			{
				layoutRoot.FindAnyWidget( "Mute" ).Show( true );
				layoutRoot.FindAnyWidget( "Gamercard" ).Show( true );
				if( OnlineServices.IsPlayerMuted( uid ) )
				{
					mute_text.SetText( "Unmute" );
				}
				else
				{
					mute_text.SetText( "Mute" );
				}
			}
			
			if( m_ServerInfoPanel.IsGloballyMuted( uid ) )
			{
				layoutRoot.FindAnyWidget( "Mute" ).Show( false );
			}
		}
	}
	
	void OnPermissionsUpdate( BiosPrivacyUidResultArray result_list )
	{
		m_ServerInfoPanel.Reload( result_list );
		
		TextWidget mute_text = TextWidget.Cast( layoutRoot.FindAnyWidget( "Mute" ).FindAnyWidget( "MuteText" ) );
		string uid = m_ServerInfoPanel.FindPlayerByWidget( GetFocus() );
		if( uid != "" )
		{
			if( IsLocalPlayer( uid ) || m_ServerInfoPanel.IsEmpty() )
			{
				layoutRoot.FindAnyWidget( "Mute" ).Show( false );
				layoutRoot.FindAnyWidget( "Gamercard" ).Show( false );
			}
			else
			{
				layoutRoot.FindAnyWidget( "Mute" ).Show( true );
				layoutRoot.FindAnyWidget( "Gamercard" ).Show( true );
				if( OnlineServices.IsPlayerMuted( uid ) )
				{
					mute_text.SetText( "Unmute" );
				}
				else
				{
					mute_text.SetText( "Mute" );
				}
			}
			
			if( m_ServerInfoPanel.IsGloballyMuted( uid ) )
			{
				layoutRoot.FindAnyWidget( "Mute" ).Show( false );
			}
		}
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
		if( m_ContinueButton || w == m_ExitButton || w == m_RestartButton || w == m_OptionsButton || w == m_ControlsButton || w == m_OnlineButton || w == m_TutorialsButton );
			return true;
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
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w )
	{
		SetFocus( w );
		
		ButtonWidget button = ButtonWidget.Cast( w );
		if( button && button != m_ContinueButton )
		{
			button.SetTextColor( ARGB( 255, 200, 0, 0 ) );
		}
	}
	
	void ColorWhite( Widget w, Widget enterW )
	{
		#ifdef PLATFORM_WINDOWS
		SetFocus( null );
		#endif
		
		ButtonWidget button = ButtonWidget.Cast( w );
		if( button && button != m_ContinueButton )
		{
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
		}
	}
}
