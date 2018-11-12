class MissionMainMenu extends MissionBase
{
	private UIScriptedMenu m_mainmenu;
	private ref DayZIntroScenePC m_IntroScenePC;
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
	
	DayZIntroScenePC GetIntroScenePC()
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

	void CreateIntroScene()
	{		
#ifdef PLATFORM_CONSOLE
		m_IntroSceneXbox = new DayZIntroSceneXbox;
#else
		Print("misssionMainMenu CreateIntroScene");
		m_IntroScenePC = new DayZIntroScenePC;
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
		if ( g_Game.IsLoading() )
		{
			return;
		}
				
		if (m_IntroScenePC)
		{
			m_IntroScenePC.Update();
		}
	}
}
