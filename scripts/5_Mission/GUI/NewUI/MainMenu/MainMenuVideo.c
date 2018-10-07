class MainMenuVideo extends UIScriptedMenu
{
	protected TextWidget			m_PlayPauseText;
	protected VideoWidget			m_Video;
	protected ref Timer				m_VideoPlayTimer;
	protected ref WidgetFadeTimer	m_VideoFadeTimer;
	bool							m_IsPaused;
	
	override Widget Init()
	{
		layoutRoot 				= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/xbox/video_menu.layout" );
		m_Video					= VideoWidget.Cast( layoutRoot.FindAnyWidget( "video" ) );
		m_PlayPauseText			= TextWidget.Cast( layoutRoot.FindAnyWidget( "PauseVideoText" ) );
		
		m_VideoPlayTimer		= new Timer();
		m_VideoFadeTimer		= new WidgetFadeTimer();
		
		m_Video.LoadVideo( "G:\\video\\DayZ_xbox_onboarding_MASTER.mp4", 0 );
		m_Video.Play( VideoCommand.REWIND );
		m_Video.Play( VideoCommand.PLAY );
		m_VideoFadeTimer.FadeIn( m_Video, 1.5 );
		m_VideoPlayTimer.Run( 0.005, this, "PlayVideoLoop", null, true );
		
		return layoutRoot;
	}
	
	void ~MainMenuVideo()
	{
	}
	
	void PlayVideoLoop()
	{
		if( m_Video && !m_IsPaused && !m_Video.Play( VideoCommand.ISPLAYING ) )
		{
			StopVideo();
		}
	}
	
	void StopVideo()
	{
		if( m_Video )
		{
			m_VideoPlayTimer.Stop();
			m_Video.Play( VideoCommand.KILL );
			GetGame().GetUIManager().Back();
		}
	}
	
	void PlayPauseVideo()
	{
		if( m_Video )
		{
			if( m_Video.Play( VideoCommand.ISPLAYING ) )
			{
				m_Video.Play( VideoCommand.STOP );
				//m_PlayPauseText.SetText( "Play" );
				m_IsPaused = true;
			}
			else
			{
				m_Video.Play( VideoCommand.PLAY );
				//m_PlayPauseText.SetText( "Pause" );
				m_IsPaused = false;
			}
		}
	}
	
	override void Update(float timeslice)
	{
		if( GetGame().GetInput().GetActionDown( UAUIFastEquipOrSplit, false ) )
		{
			//PlayPauseVideo();
		}
		if( GetGame().GetInput().GetActionDown( UAUIBack, false ) )
		{
			StopVideo();
		}
	}
}