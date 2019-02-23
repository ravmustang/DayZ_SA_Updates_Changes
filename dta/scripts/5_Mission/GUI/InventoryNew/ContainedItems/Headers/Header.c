class Header: LayoutHolder
{
	protected int			m_DefaultColor;
	protected int			m_DefaultFontSize;
	
	protected EntityAI		m_Entity;
	
	protected TextWidget	m_HeaderText;
	
	void Header( LayoutHolder parent, string function_name )
	{
		#ifdef PLATFORM_CONSOLE
			GetMainWidget().FindAnyWidget( "collapse_button" ).Show( false );
		#else
		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( GetMainWidget().FindAnyWidget( "collapse_button" ),  m_Parent, function_name );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainWidget().FindAnyWidget( "collapse_button" ),  m_Parent, "OnDropReceivedFromHeader" );
		#endif
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainWidget(),  m_Parent, "OnDropReceivedFromHeader" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainWidget(),  this, "DraggingOverHeader" );
		
		m_HeaderText		= TextWidget.Cast( GetMainWidget().FindAnyWidget( "TextWidget0" ) );
		
		m_DefaultColor		= GetRootWidget().GetColor();
	}
	
	void SetName( string name )
	{
		TextWidget text_widget = TextWidget.Cast( GetRootWidget().FindAnyWidget( "TextWidget0" ) );
		name.ToUpper();
		text_widget.SetText( name );
	}
	
	void SetItemPreview( EntityAI entity_ai )
	{
		#ifndef PLATFORM_CONSOLE
		m_Entity = entity_ai;
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( GetMainWidget().FindAnyWidget( "Render" ) );
		item_preview.SetItem(entity_ai);
		item_preview.SetView( entity_ai.GetViewIndex() );
		#endif
	}
	
	void DraggingOverHeader( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		Container parent_container = Container.Cast( m_Parent );
		parent_container.DraggingOverHeader( w, x, y, receiver );
	}

	void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
	{
		Container parent_container = Container.Cast( m_Parent );
		parent_container.OnDropReceivedFromHeader( w, x, y, receiver );
	}
	
	override void SetActive( bool active )
	{
		super.SetActive( active );
		if( active )
		{
			GetMainWidget().SetColor( ARGBF( 1, 1, 0, 0 ) );
		}
		else
		{
			GetMainWidget().SetColor( m_DefaultColor );
		}
	}
}
