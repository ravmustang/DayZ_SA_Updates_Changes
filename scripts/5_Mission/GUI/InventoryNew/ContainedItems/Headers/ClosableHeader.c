class ClosableHeader: Header
{
	protected float		m_SquareSize;
	
	void ClosableHeader( ContainerBase parent, string function_name )
	{
		WidgetEventHandler.GetInstance().RegisterOnDrag( GetMainPanel() ,  this, "OnDragHeader" );
		WidgetEventHandler.GetInstance().RegisterOnDrop( GetMainPanel() ,  this, "OnDropHeader" );
		WidgetEventHandler.GetInstance().RegisterOnMouseEnter( GetMainPanel(),  this, "MouseEnter" );
		WidgetEventHandler.GetInstance().RegisterOnMouseLeave( GetMainPanel(),  this, "MouseLeave" );
		float temp;
		GetMainPanel().GetScreenSize( temp, m_SquareSize );
	}

	override void SetLayoutName()
	{
		#ifdef PLATFORM_CONSOLE
		m_LayoutName = WidgetLayoutName.ClosableHeaderXbox;
		#else
		m_LayoutName = WidgetLayoutName.ClosableHeader;
		#endif
	}
	
	void OnDragHeader( Widget w, int x, int y )
	{
		ClosableContainer parent = ClosableContainer.Cast( m_Parent );
		if( parent && GetMainPanel() && m_Entity )
		{
			ItemPreviewWidget item_preview_drag = ItemPreviewWidget.Cast( GetMainPanel().FindAnyWidget( "Drag_Render" ) );

			if( item_preview_drag && m_Entity )
			{
				item_preview_drag.SetItem( m_Entity );
				item_preview_drag.SetView( m_Entity.GetViewIndex() );
			}
			
			ItemManager.GetInstance().SetIsDragging( true );
			
			parent.HideContent();
			
			GetMainPanel().FindAnyWidget( "PanelWidget" ).Show( false );
			if( GetMainPanel().FindAnyWidget( "Drag_Render" ) )
				GetMainPanel().FindAnyWidget( "Drag_Render" ).Show( true );
			
			int ww, hh;
			
			GetGame().GetInventoryItemSize( InventoryItem.Cast( m_Entity ), ww, hh );
			
			GetMainPanel().FindAnyWidget( "Drag_Render" ).SetSize( ww * m_SquareSize, hh * m_SquareSize );
			
			parent.UpdateSpacer();
		}
	}
	
	void OnDropHeader( Widget w )
	{
		ClosableContainer parent = ClosableContainer.Cast( m_Parent );
		if( parent )
		{
			GetMainPanel().FindAnyWidget( "PanelWidget" ).Show( true );
			GetMainPanel().FindAnyWidget( "Drag_Render" ).Show( false );
			
			parent.ShowContent();
			parent.UpdateSpacer();
			
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
