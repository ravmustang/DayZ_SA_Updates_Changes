class HudTextInfoXbox
{
	protected ref TStringArray	m_TextIDs;
	protected static const float ANIM_SEQUENCE_TIME		= 5;
	protected static const float ANIM_SEQUENCE_HIDE_TIME	= 3;
	
	protected int 			m_SequenceIndex = 0;
	protected float			m_TextAlpha = 0;
	protected float			m_SequenceTime;
	protected Widget		m_WidgetRoot;
	protected TextWidget	m_WidgetTextInfo;
	protected bool			m_IsGameTrial;
	
	void HudTextInfoXbox( Widget root )
	{
		m_WidgetRoot		= root;
		m_WidgetTextInfo	= TextWidget.Cast( m_WidgetRoot.FindAnyWidget( "TextInfoXbox" ) );
		
		#ifdef PLATFORM_XBOX
		m_IsGameTrial		= OnlineServices.IsGameTrial( false );
		//m_IsGameTrial = true;
		#else
		m_IsGameTrial		= true;
		#endif
		
		m_TextIDs = new TStringArray;
		m_TextIDs.Insert("#tutorial_introduction1");
		m_TextIDs.Insert("#tutorial_introduction2");
		m_TextIDs.Insert("#tutorial_introduction3");
	}
	
	void ~HudTextInfoXbox()
	{
		
	}
	
	void Update(float time_delta)
	{
		if ( m_IsGameTrial && m_SequenceIndex < m_TextIDs.Count() )
		{
			if ( m_SequenceTime == 0 )
			{
				// Anim Start
				m_WidgetTextInfo.SetText( m_TextIDs[m_SequenceIndex] );
				m_WidgetTextInfo.Update();
				m_WidgetTextInfo.Show( true );
				m_TextAlpha = 1;
			}
			
			if ( m_SequenceTime >= ANIM_SEQUENCE_TIME )
			{
				// Anim End
				m_SequenceTime = 0;
				m_SequenceIndex++;
				return;
			}
			else
			{
				// Anim Show
				m_SequenceTime = m_SequenceTime + time_delta;
			}
			
			if ( m_SequenceTime >= (ANIM_SEQUENCE_TIME - ANIM_SEQUENCE_HIDE_TIME) )
			{
				// Anim Fade Out				
				m_TextAlpha = m_TextAlpha - (time_delta / ANIM_SEQUENCE_HIDE_TIME);
			}		
			
			m_WidgetTextInfo.SetAlpha(m_TextAlpha);
		}
		else
		{
			m_WidgetTextInfo.Show( false );
		}
	}
}