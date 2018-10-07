class PlayerListEntryScriptedWidget extends ScriptedWidgetEventHandler
{
	protected string		m_Name;
	protected string		m_UID;
	protected bool			m_Mute;
	protected bool			m_GlobalMute;
	
	protected Widget		m_Root;
	protected TextWidget	m_PlayerName;
	protected ImageWidget	m_PlayerAvatar;
	protected ImageWidget	m_MicrophoneIcon;
	protected ImageWidget	m_MuteIcon;
	protected ButtonWidget	m_PlayerButton;
	
	void PlayerListEntryScriptedWidget( Widget parent, string name, string uid, bool show_permissions )
	{
		m_Name				= name;
		m_UID				= uid;
		
		m_Root				= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/xbox/ingamemenu_xbox/player_info_entry.layout", parent );
		m_PlayerName		= TextWidget.Cast( m_Root.FindAnyWidget( "Name" ) );
		m_PlayerAvatar		= ImageWidget.Cast( m_Root.FindAnyWidget( "Avatar" ) );
		m_MicrophoneIcon	= ImageWidget.Cast( m_Root.FindAnyWidget( "Microphone" ) );
		m_MuteIcon			= ImageWidget.Cast( m_Root.FindAnyWidget( "Muted" ) );
		m_PlayerButton		= ButtonWidget.Cast( m_Root.FindAnyWidget( "Button" ) );
		
		m_MicrophoneIcon.Show( show_permissions );
		
		m_PlayerName.SetText( name );
		m_Root.SetHandler( this );
	}
	
	void ~PlayerListEntryScriptedWidget()
	{
		delete m_Root;
	}
	
	Widget GetButtonWidget()
	{
		return m_PlayerButton;
	}
	
	void LoadPermissions( BiosPrivacyPermissionResultArray results )
	{
		foreach( BiosPrivacyPermissionResult result : results )
		{
			if( result.m_Permission == EBiosPrivacyPermission.COMMUNICATE_VOICE )
			{
				m_GlobalMute = !result.m_IsAllowed;
				if( result.m_IsAllowed && !m_Mute )
				{
					m_MuteIcon.Show( false );
				}
				else if( !result.m_IsAllowed || m_Mute )
				{
					m_MuteIcon.Show( true );
				}
			}
		}
	}
	
	bool IsMuted()
	{
		return m_Mute;
	}
	
	bool IsGloballyMuted()
	{
		return m_GlobalMute;
	}
	
	void MutePlayer( bool mute )
	{
		m_Mute = mute;
		if( !m_GlobalMute )
			m_MuteIcon.Show( m_Mute );
		else
			m_MuteIcon.Show( true );
	}
}