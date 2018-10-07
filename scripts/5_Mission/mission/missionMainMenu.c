class MissionMainMenu extends MissionBase
{
	private UIScriptedMenu m_mainmenu;
	private ref DayZIntroScene m_IntroScenePC;
	private ref DayZIntroSceneXbox m_IntroSceneXbox;
	bool m_NoCutscene;

	override void OnInit()
	{
		if (!m_NoCutscene)
		{
			CreateIntroScene();
		}
		
		if (!m_mainmenu)
		{
			#ifdef PLATFORM_XBOX
				m_mainmenu = UIScriptedMenu.Cast( g_Game.GetUIManager().EnterScriptedMenu( MENU_TITLE_SCREEN, null ) );
			#else
				m_mainmenu = UIScriptedMenu.Cast( g_Game.GetUIManager().EnterScriptedMenu( MENU_MAIN, null ) );
			#endif
		}
	}
	
	override void Reset()
	{
		#ifdef PLATFORM_CONSOLE
			delete m_IntroSceneXbox;
		#else
			delete m_IntroScenePC;
		#endif
		
		CreateIntroScene();
	}
	
	DayZIntroScene GetIntroScenePC()
	{
		#ifdef PLATFORM_CONSOLE
			Error("missionMainMenu->GetIntroScenePC on PLATFORM_CONSOLE is not implemented!");
			return null;
		#else
			return m_IntroScenePC;
		#endif
	}
	
	DayZIntroSceneXbox GetIntroSceneXbox()
	{
		#ifdef PLATFORM_CONSOLE
			return m_IntroSceneXbox;
		#else
			Error("missionMainMenu->GetIntroScenePC on PLATFORM_PC is not implemented!");
			return null;
		#endif
	}
	
	Camera GetIntroSceneCamera()
	{
		#ifdef PLATFORM_CONSOLE
			return m_IntroSceneXbox.GetCamera();
		#else
			return m_IntroScenePC.GetCamera();
		#endif
	}
	
	PlayerBase GetIntroSceneCharacter()
	{
		#ifdef PLATFORM_CONSOLE
			return m_IntroSceneXbox.GetIntroSceneCharacter();
		#else
			return m_IntroScenePC.GetIntroSceneCharacter();
		#endif
	}
	
	void SetCharacterFemale(bool fem)
	{
		#ifdef PLATFORM_CONSOLE
			m_IntroSceneXbox.SetCharacterFemale( fem );
		#else
			m_IntroScenePC.SetCharacterFemale( fem );
		#endif
	}
	
	void CharChangePart( Direction dir, int inv_slot )
	{
		#ifdef PLATFORM_CONSOLE
			Error("missionMainMenu->CharChangePart on PLATFORM_CONSOLE is not implemented!");
		#else
			m_IntroScenePC.CharChangePart( dir, inv_slot );
		#endif
	}
	
	bool IsCharacterFemale()
	{
		#ifdef PLATFORM_CONSOLE
			return m_IntroSceneXbox.IsCharacterFemale();
		#else
			return m_IntroScenePC.IsCharacterFemale();
		#endif
	}
	
	void SetClickEnable(bool enable)
	{
		#ifdef PLATFORM_CONSOLE
			Error("missionMainMenu->SetClickEnable on PLATFORM_CONSOLE is not implemented!");
		#else
			m_IntroScenePC.SetClickEnable( enable );
		#endif
	}
	
	bool IsClickEnabled()
	{
		#ifdef PLATFORM_CONSOLE
			Error("missionMainMenu->IsClickEnabled on PLATFORM_CONSOLE is not implemented!");
			return true;
		#else
			return m_IntroScenePC.IsClickEnabled();
		#endif
	}
	
	void SaveCharName()
	{
		#ifdef PLATFORM_CONSOLE
			Error("missionMainMenu->SaveCharName on PLATFORM_CONSOLE is not implemented!");
		#else
			m_IntroScenePC.SaveCharName();
		#endif
	}
	
	void SaveCharacterSetup()
	{
		#ifdef PLATFORM_CONSOLE
			Error("missionMainMenu->SaveCharacterSetup on PLATFORM_CONSOLE is not implemented!");
		#else
			m_IntroScenePC.SaveCharacterSetup();
		#endif
	}
	
	int GetCurrentCharacterID()
	{
		#ifdef PLATFORM_CONSOLE
			Error("missionMainMenu->GetCurrentCharacterID on PLATFORM_CONSOLE is not implemented!");
			return -1;
		#else
		if ( m_IntroScenePC )
		{
			return m_IntroScenePC.GetCurrentCharacterID();
		}
		return -1;
		#endif
		
		return -1;
	}
	
	void ResetIntroCamera()
	{
		#ifdef PLATFORM_CONSOLE
		if ( m_IntroSceneXbox )
		{
			m_IntroSceneXbox.ResetIntroCamera();
		}
		#else
		if ( m_IntroScenePC )
		{
			m_IntroScenePC.ResetIntroCamera();
		}
		#endif
	}

	void CreateIntroScene()
	{		
#ifdef PLATFORM_CONSOLE
		m_IntroSceneXbox = new DayZIntroSceneXbox;
#else
		m_IntroScenePC = new DayZIntroScene;
#endif
	}

	override void OnMissionStart()
	{
		if (m_mainmenu)
		{
			//m_mainmenu.FadeIn(2.0);	//Fade in method is currently commented in MainMenu class
		}
		g_Game.GetUIManager().ShowUICursor(true);
		g_Game.SetMissionState( DayZGame.MISSION_STATE_MAINMENU );
	}
	
	override void OnMissionFinish()
	{
		if( m_mainmenu )
			m_mainmenu.Cleanup();
		GetGame().GetUIManager().CloseAll();
		m_mainmenu = NULL;
		
		m_IntroScenePC = null;
		m_IntroSceneXbox = null;
		g_Game.GetUIManager().ShowUICursor(false);
	}

	override void OnUpdate(float timeslice)
	{
		if (g_Game.IsLoading()) return;
		
		if (m_IntroScenePC)
		{
			m_IntroScenePC.Update();
		}
		
		if (m_IntroSceneXbox)
		{
			m_IntroSceneXbox.Update();
		}

		if( m_mainmenu )
		{
			m_mainmenu.Update(timeslice);
		}

		if (GetGame().GetInput().GetActionDown(UAUIBack, false))
		{
			if (GetGame().GetUIManager().GetMenu() != m_mainmenu)
			{
				GetGame().GetUIManager().Back();
			}
		}
	}
}
