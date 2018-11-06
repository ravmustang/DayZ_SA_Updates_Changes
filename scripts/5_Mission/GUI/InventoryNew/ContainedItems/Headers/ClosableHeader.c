class ClosableHeader: Header
{
	protected float		m_SquareSize;
	
	void ClosableHeader( LayoutHolder parent, string function_name )
	{
		WidgetEventHandler.GetInstance().RegisterOnDrag( GetMainWidget() ,  this, "OnDragHeader" );
		WidgetEventHandler.GetInstance().RegisterOnDrop( GetMainWidget() ,  this, "OnDropHeader" );
		WidgetEventHandler.GetInstance().RegisterOnMouseEnter( GetMainWidget(),  this, "MouseEnter" );
		WidgetEventHandler.GetInstance().RegisterOnMouseLeave( GetMainWidget(),  this, "MouseLeave" );
		float temp;
		GetMainWidget().GetScreenSize( temp, m_SquareSize );
	}

	override void SetLayoutName()
	{
		#ifdef PLATFORM_XBOX
		m_LayoutName = WidgetLayoutName.ClosableHeaderXbox;
		#else
		m_LayoutName = WidgetLayoutName.ClosableHeader;
		#endif
	}
	
	void OnDragHeader( Widget w, int x, int y )
	{
		ClosableContainer parent = ClosableContainer.Cast( m_Parent );
		if( parent && GetMainWidget() && m_Entity )
		{
			ItemPreviewWidget item_preview_drag = ItemPreviewWidget.Cast( GetMainWidget().FindAnyWidget( "Drag_Render" ) );
			item_preview_drag.SetFlags(WidgetFlags.EXACTPOS );
			item_preview_drag.GetParent().GetParent().GetParent().GetParent().GetParent().GetParent().GetParent().GetParent().AddChild(GetMainWidget());
			if( item_preview_drag && m_Entity )
			{
				item_preview_drag.SetItem( m_Entity );
				item_preview_drag.SetView( m_Entity.GetViewIndex() );
			}
			
			ItemManager.GetInstance().SetIsDragging( true );
			
			parent.HideContent();
			
			GetMainWidget().FindAnyWidget( "PanelWidget" ).Show( false );
			if( item_preview_drag )
				item_preview_drag.Show( true );
			
			int ww, hh;
			
			GetGame().GetInventoryItemSize( InventoryItem.Cast( m_Entity ), ww, hh );
			if( item_preview_drag )
			item_preview_drag.SetSize( ww * m_SquareSize, hh * m_SquareSize );
		}
	}
	
	void OnDropHeader( Widget w )
	{
		ClosableContainer parent = ClosableContainer.Cast( m_Parent );
		parent.GetMainWidget().AddChild( GetMainWidget() );
		if( parent )
		{
			GetMainWidget().FindAnyWidget( "PanelWidget" ).Show( true );
			GetMainWidget().FindAnyWidget( "Drag_Render" ).Show( false );
			
			parent.ShowContent();
			
			ItemManager.GetInstance().SetIsDragging( false );
			ItemManager.GetInstance().HideDropzones();
		}
	}
	
	bool MouseEnter(Widget w, int x, int y)
	{
		if( m_Entity )
			ItemManager.GetInstance().PrepareTooltip( EntityAI.Cast( m_Entity ) );
		return true;
	}

	bool MouseLeave( Widget w, Widget s, int x, int y	)
	{
		ItemManager.GetInstance().HideTooltip();
		return true;
	}
}
