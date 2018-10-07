class Header: ContainerBase
{
	protected EntityAI	m_Entity;
	void Header( ContainerBase parent, string function_name )
	{
		#ifndef PLATFORM_CONSOLE
		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( GetMainPanel().FindAnyWidget( "collapse_button" ),  m_Parent, function_name );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "collapse_button" ),  m_Parent, "OnDropReceivedFromHeader" );
		#endif
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel(),  m_Parent, "OnDropReceivedFromHeader" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel(),  this, "DraggingOverHeader" );
	}
	
	void SetName( string name )
	{
		TextWidget text_widget = TextWidget.Cast( GetMainPanel().FindAnyWidget( "TextWidget0" ) );
		name.ToUpper();
		text_widget.SetText( name );
	}
	
	void SetItemPreview( EntityAI entity_ai )
	{
		m_Entity = entity_ai;
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( GetMainPanel().FindAnyWidget( "Render" ) );
		item_preview.SetItem(entity_ai);
		item_preview.SetView( entity_ai.GetViewIndex() );
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
}
