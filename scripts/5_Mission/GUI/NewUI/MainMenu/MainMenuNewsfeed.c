class MainMenuNewsfeed extends ScriptedWidgetEventHandler
{
	protected Widget		m_Root;
	
	protected Widget		m_NewsMain;
	protected Widget		m_NewsSec1;
	protected Widget		m_NewsSec2;
	
	protected Widget		m_Feedback;
	protected Widget		m_DayZForum;
	protected Widget		m_Twitter;
	protected Widget		m_Youtube;
	
	protected ImageWidget	m_MainImage;
	protected ImageWidget	m_SecImage1;
	protected ImageWidget	m_SecImage2;
	
	protected TextWidget	m_MainText1;
	protected TextWidget	m_MainText2;
	protected TextWidget	m_SecText1;
	protected TextWidget	m_SecText2;
	
	void MainMenuNewsfeed( Widget root )
	{
		m_Root			= root;
		
		m_NewsMain		= m_Root.FindAnyWidget( "news_main" );
		m_NewsSec1		= m_Root.FindAnyWidget( "news_sec_1" );
		m_NewsSec2		= m_Root.FindAnyWidget( "news_sec_2" );
		
		m_Feedback		= m_Root.FindAnyWidget( "feedback_tracker" );
		m_DayZForum		= m_Root.FindAnyWidget( "dayz_forums" );
		m_Twitter		= m_Root.FindAnyWidget( "twitter" );
		m_Youtube		= m_Root.FindAnyWidget( "youtube" );
		
		m_MainImage		= ImageWidget.Cast( m_NewsMain.FindAnyWidget( "news_main_image" ) );
		m_SecImage1		= ImageWidget.Cast( m_NewsSec1.FindAnyWidget( "news_sec_1_image" ) );
		m_SecImage2		= ImageWidget.Cast( m_NewsSec2.FindAnyWidget( "news_sec_2_image" ) );
		
		m_MainText1		= TextWidget.Cast( m_NewsMain.FindAnyWidget( "news_main_text" ) );
		m_MainText2		= TextWidget.Cast( m_NewsMain.FindAnyWidget( "news_main_text_1" ) );
		m_SecText1		= TextWidget.Cast( m_NewsSec1.FindAnyWidget( "news_sec_1_text" ) );
		m_SecText2		= TextWidget.Cast( m_NewsSec2.FindAnyWidget( "news_sec_2_text" ) );
		
		if( g_Game.IsStressTest() )
		{
			m_MainImage.LoadImageFile( 0, "set:main_menu_newsfeed image:image_1_1" );
			m_MainText1.SetText( "Welcome to Stress Test Branch!" );
			m_MainText2.SetText( "Click to read more about Stress Tests." );
		}
		
		m_Root.SetHandler( this );
	}
	
	void ShowNewsfeed()
	{
		m_Root.Show( true );
	}
	
	void HideNewsfeed()
	{
		m_Root.Show( false );
	}
	
	void OpenNewsMain()
	{
		if( g_Game.IsStressTest() )
		{
			GetGame().OpenURL( "https://dayz.com/blog/0-63-stress-tests" );
		}
		else
		{
			GetGame().OpenURL( "https://dayz.com/experimental" );
		}
	}
	
	void OpenNewsSec1()
	{
		if( g_Game.IsStressTest() )
		{
			GetGame().OpenURL( "https://forums.dayz.com/topic/238136-stress-test-reports" );
		}
		else
		{
			GetGame().OpenURL( "https://dayz.com/blog" );
		}
	}
	
	void OpenNewsSec2()
	{
		GetGame().OpenURL( "https://feedback.dayz.com" );
	}
	
	void OpenFeedback()
	{
		GetGame().OpenURL( "https://feedback.bistudio.com/tag/dayz" );
	}
	
	void OpenForums()
	{
		GetGame().OpenURL( "https://forums.dayz.com" );
	}
	
	void OpenTwitter()
	{
		GetGame().OpenURL( "https://twitter.com/dayzdevteam" );
	}
	
	void OpenYoutube()
	{
		GetGame().OpenURL( "https://www.youtube.com/user/DayZDevTeam" );
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_NewsMain )
			{
				OpenNewsMain();
				return true;
			}
			else if ( w == m_NewsSec1 )
			{
				OpenNewsSec1();
				return true;
			}
			else if ( w == m_NewsSec2 )
			{
				OpenNewsSec2();
				return true;
			}
			else if ( w == m_Feedback )
			{
				OpenFeedback();
				return true;
			}
			else if ( w == m_DayZForum )
			{
				OpenForums();
				return true;
			}
			else if ( w == m_Twitter )
			{
				OpenTwitter();
				return true;
			}
			else if ( w == m_Youtube )
			{
				OpenYoutube();
				return true;
			}
		}
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w, x, y );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, enterW, x, y );
			return true;
		}
		return false;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w, x, y );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, null, x, y );
			return true;
		}
		return false;
	}
	
	bool IsFocusable( Widget w )
	{
		return ( w == m_NewsMain || w == m_NewsSec1 || w == m_NewsSec2 || w == m_Feedback || w == m_DayZForum || w == m_Twitter || w == m_Youtube );
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w, int x, int y )
	{
		SetFocus( w );
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 0, 0 ) );
			button.SetAlpha( 0.9 );
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
	
	void ColorWhite( Widget w, Widget enterW, int x, int y )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
			button.SetAlpha( 0.75 );
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
}