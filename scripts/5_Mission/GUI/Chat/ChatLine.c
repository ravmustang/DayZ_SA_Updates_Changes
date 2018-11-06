class ChatLine
{
	const float FADE_TIMEOUT = 30;
	const float FADE_OUT_DURATION = 3;
	const float FADE_IN_DURATION = 0.5;
	Widget							m_RootWidget;
	
	TextWidget						m_NameWidget;
	TextWidget						m_TextWidget;
	
	Widget							m_CCSystemWidget;
	Widget							m_CCAdminWidget;
	Widget							m_CCDirectWidget;
	Widget							m_CCMegaphoneWidget;
	Widget							m_CCTransmitterWidget;
	Widget							m_CCPublicAddressSystemWidget;
	
	private ref WidgetFadeTimer 	m_FadeTimer;
	private ref Timer 				m_TimeoutTimer;

	void ChatLine(Widget root_widget)
	{
		m_RootWidget					= GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_chat_item.layout", root_widget);
	
		m_NameWidget					= TextWidget.Cast( m_RootWidget.FindAnyWidget( "ChatItemSenderWidget" ) );
		m_TextWidget					= TextWidget.Cast( m_RootWidget.FindAnyWidget( "ChatItemTextWidget" ) );
		m_CCSystemWidget				= m_RootWidget.FindAnyWidget( "CCSystem" );
		m_CCAdminWidget					= m_RootWidget.FindAnyWidget( "CCAdmin" );
		m_CCDirectWidget				= m_RootWidget.FindAnyWidget( "CCDirect" );
		m_CCMegaphoneWidget				= m_RootWidget.FindAnyWidget( "CCMegaphone" );
		m_CCTransmitterWidget			= m_RootWidget.FindAnyWidget( "CCTransmitter" );
		m_CCPublicAddressSystemWidget	= m_RootWidget.FindAnyWidget( "CCPublicAddressSystem" );
		
		m_FadeTimer			= new WidgetFadeTimer;
		m_TimeoutTimer		= new Timer(CALL_CATEGORY_GUI);
	}

	void ~ChatLine()
	{
		delete m_TextWidget;
	}

	void Set(ChatMessageEventParams params)
	{
		m_RootWidget.Show( true );
		if (params.param2 != "")
		{
			m_NameWidget.SetText( params.param2 );
			m_TextWidget.SetText( ": " + params.param3 );
		}
		else
		{
			m_NameWidget.SetText( "" );
			m_TextWidget.SetText( params.param3 );
		}

		int channel =  params.param1;
		if( channel & CCSystem )
 		{
			m_CCSystemWidget.Show( true );
 		}
		else
		{
			m_CCSystemWidget.Show( false );
		}
		if( channel & CCAdmin )
		{
			m_CCAdminWidget.Show( true );
		}
		else
		{
			m_CCAdminWidget.Show( false );
		}
		if( channel & CCDirect )
		{
			m_CCDirectWidget.Show( true );
		}
		else
		{
			m_CCDirectWidget.Show( false );
		}
		if( channel & CCMegaphone )
		{
			m_CCMegaphoneWidget.Show( true );
		}
		else
		{
			m_CCMegaphoneWidget.Show( false );
		}
		if( channel & CCTransmitter )
		{
			m_CCTransmitterWidget.Show( true );
		}
		else
		{
			m_CCTransmitterWidget.Show( false );
		}
		if( channel & CCPublicAddressSystem )
		{
			m_CCPublicAddressSystemWidget.Show( true );
		}
		else
		{
			m_CCPublicAddressSystemWidget.Show( false );
		}
		
		m_FadeTimer.FadeIn(m_RootWidget, FADE_IN_DURATION);
		
		m_TimeoutTimer.Run(FADE_TIMEOUT, m_FadeTimer, "FadeOut", new Param2<Widget, float>(m_RootWidget, FADE_OUT_DURATION));
	}

	void Clear()
	{
		m_RootWidget.Show( false );
		m_TimeoutTimer.Stop();
		m_FadeTimer.Stop();
	}
}