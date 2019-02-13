class ClosableHeader: Header
{
	protected float		m_SquareSize;
	protected int		m_DefaultSort;
	
	void ClosableHeader( LayoutHolder parent, string function_name )
	{
		WidgetEventHandler.GetInstance().RegisterOnDrag( GetMainWidget() ,  this, "OnDragHeader" );
		WidgetEventHandler.GetInstance().RegisterOnDrop( GetMainWidget() ,  this, "OnDropHeader" );
		WidgetEventHandler.GetInstance().RegisterOnMouseEnter( GetMainWidget(),  this, "MouseEnter" );
		WidgetEventHandler.GetInstance().RegisterOnMouseLeave( GetMainWidget(),  this, "MouseLeave" );
		float temp;
		GetMainWidget().GetScreenSize( temp, m_SquareSize );
		
		m_DefaultColor		= GetMainWidget().FindAnyWidget( "PanelWidget" ).GetAlpha();
		m_DefaultFontSize	= 20;
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.ClosableHeader;
	}
	
	void OnDragHeader( Widget w, int x, int y )
	{
		ClosableContainer parent = ClosableContainer.Cast( m_Parent );
		if( parent && GetMainWidget() && m_Entity )
		{
			ItemPreviewWidget item_preview_drag = ItemPreviewWidget.Cast( GetMainWidget().FindAnyWidget( "Drag_Render" ) );
			item_preview_drag.SetFlags(WidgetFlags.EXACTPOS );
			m_DefaultSort = GetRootWidget().GetSort();
			item_preview_drag.GetParent().GetParent().GetParent().GetParent().GetParent().GetParent().GetParent().GetParent().AddChild( GetRootWidget() );
			if( item_preview_drag && m_Entity )
			{
				item_preview_drag.SetItem( m_Entity );
				item_preview_drag.SetView( m_Entity.GetViewIndex() );
			}
			
			ItemManager.GetInstance().SetIsDragging( true );
			
			parent.HideContent( true );
			
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
		parent.GetMainWidget().AddChild( GetRootWidget() );
		GetRootWidget().SetSort( 0 );
		if( parent )
		{
			GetMainWidget().FindAnyWidget( "PanelWidget" ).Show( true );
			GetMainWidget().FindAnyWidget( "Drag_Render" ).Show( false );
			
			parent.ShowContent( true );
			
			ItemManager.GetInstance().SetIsDragging( false );
			ItemManager.GetInstance().HideDropzones();
		}
	}
	
	bool MouseEnter(Widget w, int x, int y)
	{
		if( m_Entity )
			ItemManager.GetInstance().PrepareTooltip( EntityAI.Cast( m_Entity ) );
		ClosableContainer p = ClosableContainer.Cast( m_Parent );
		if( p )
		{
			p.EnterContainer();
		}
		return true;
	}

	bool MouseLeave( Widget w, Widget s, int x, int y	)
	{
		ItemManager.GetInstance().HideTooltip();
		ClosableContainer p = ClosableContainer.Cast( m_Parent );
		if( p )
		{
			p.EnterContainer();
		}
		return true;
	}
	
	override void SetActive( bool active )
	{
		super.SetActive( active );
		if( active )
		{
			GetMainWidget().FindAnyWidget( "PanelWidget" ).SetAlpha( m_DefaultColor + 0.1 );
			m_HeaderText.SetTextExactSize( m_DefaultFontSize * 1.1 );
		}
		else
		{
			GetMainWidget().FindAnyWidget( "PanelWidget" ).SetAlpha( m_DefaultColor );
			m_HeaderText.SetTextExactSize( m_DefaultFontSize );
		}
	}
}
