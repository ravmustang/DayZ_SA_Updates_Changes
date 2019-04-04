class ClosableHeader: Header
{
	protected float			m_SquareSize;
	protected int			m_DefaultSort;
	
	protected Widget		m_MovePanel;
	protected ButtonWidget	m_MoveUp;
	protected ButtonWidget	m_MoveDown;
	
	protected bool			m_IsInLocalEquipment;
	
	void ClosableHeader( LayoutHolder parent, string function_name )
	{
		m_MovePanel	= GetMainWidget().FindAnyWidget( "MovePanel" );
		m_MoveUp	= ButtonWidget.Cast( GetMainWidget().FindAnyWidget( "MoveUp" ) );
		m_MoveDown	= ButtonWidget.Cast( GetMainWidget().FindAnyWidget( "MoveDown" ) );
		
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
		WidgetEventHandler.GetInstance().RegisterOnClick( m_MoveUp,  m_Parent, "EquipmentMoveUp" );
		WidgetEventHandler.GetInstance().RegisterOnClick( m_MoveDown,  m_Parent, "EquipmentMoveDown" );
		WidgetEventHandler.GetInstance().RegisterOnMouseLeave( m_MoveUp,  this, "MouseLeave" );
		WidgetEventHandler.GetInstance().RegisterOnMouseLeave( m_MoveDown,  this, "MouseLeave" );
		#endif
		#endif
		
		WidgetEventHandler.GetInstance().RegisterOnDrag( GetMainWidget() ,  this, "OnDragHeader" );
		WidgetEventHandler.GetInstance().RegisterOnDrop( GetMainWidget() ,  this, "OnDropHeader" );
		
		WidgetEventHandler.GetInstance().RegisterOnMouseEnter( GetMainWidget(),  this, "MouseEnter" );
		WidgetEventHandler.GetInstance().RegisterOnMouseLeave( GetMainWidget(),  this, "MouseLeave" );
		
		float temp;
		GetMainWidget().GetScreenSize( temp, m_SquareSize );
		
		m_DefaultColor			= GetMainWidget().FindAnyWidget( "PanelWidget" ).GetColor();
		m_DefaultFontSize		= 20;
		
		if( GetParent() && GetParent().GetParent() && GetParent().GetParent().GetParent() )
			m_IsInLocalEquipment	= GetParent().GetParent().GetParent().IsInherited( RightArea );
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
		if( m_IsInLocalEquipment && !GetDragWidget() )
		{
			m_MovePanel.Show( true );
		}
		return true;
	}

	bool MouseLeave( Widget w, Widget enter_w, int x, int y	)
	{
		if( enter_w != m_MoveUp && enter_w != m_MoveDown && enter_w != m_CollapseButton )
		{
			if( m_IsInLocalEquipment )
			{
				m_MovePanel.Show( false );
			}
		}
		return true;
	}
	
	override void SetActive( bool active )
	{
		super.SetActive( active );
		if( active )
		{
			GetMainWidget().FindAnyWidget( "PanelWidget" ).SetColor( ARGBF( 1, 1, 0, 0 ) );
		}
		else
		{
			GetMainWidget().FindAnyWidget( "PanelWidget" ).SetColor( m_DefaultColor );
		}
	}
}
