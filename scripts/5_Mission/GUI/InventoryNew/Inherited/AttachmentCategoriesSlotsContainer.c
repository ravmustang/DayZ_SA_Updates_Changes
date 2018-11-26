class AttachmentCategoriesSlotsContainer: Container
{
	ref SlotsContainer m_ItemsCont;
	void AttachmentCategoriesSlotsContainer( LayoutHolder parent )
	{
		m_ItemsCont = new SlotsContainer(this);
		m_Body.Insert(m_ItemsCont);
		for( int k = 0; k < 7; k++ )
		{
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_ItemsCont.GetMainWidget().FindAnyWidget( "Icon" + k ),  m_Parent, "OnDropReceivedFromHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_ItemsCont.GetMainWidget().FindAnyWidget( "GhostSlot" + k ),  m_Parent, "OnDropReceivedFromHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_ItemsCont.GetMainWidget().FindAnyWidget( "PanelWidget" + k ),  m_Parent, "OnDropReceivedFromHeader" );
			      
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetMainWidget().FindAnyWidget( "Icon" + k ),  m_Parent, "DraggingOverHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetMainWidget().FindAnyWidget( "GhostSlot" + k ),  m_Parent, "DraggingOverHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetMainWidget().FindAnyWidget( "PanelWidget" + k ),  m_Parent, "DraggingOverHeader" );
		}
	}
  
	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.ClosableContainer;
	}
  
	SlotsContainer GetSlotsContainer()
	{
		return m_ItemsCont;
	}
	
	override bool IsFirstContainerFocused()
	{
		
		Container first;
		if( m_Body.Count() > 0 )
			first = Container.Cast( m_Body[1] );
		return GetFocusedContainer() == first;
	}
	
	override bool IsLastContainerFocused()
	{
		Container last;
		if( m_Body.Count() > 0 )
			last = Container.Cast( m_Body[ m_Body.Count() - 1 ] );
		return GetFocusedContainer() == last;
	}
	
	override void MoveGridCursor( int direction )
	{
		/*
		m_ItemsCont.MoveGridCursor( int direction );
		ItemManager.GetInstance().HideTooltip();
		UnfocusAll();
		if( direction == Direction.UP )
		{
			m_FocusedRow--;
			if( m_FocusedRow < 0 )
			{
				m_FocusedRow  = m_Ics.Count() - 1;
				Container cnt = Container.Cast( m_Parent.GetParent().GetParent() );
				if( cnt )
					{
						cnt.SetPreviousActive();
					}
					else
					{
						cnt = Container.Cast( m_Parent );
						cnt.SetPreviousActive();
					}
				return;
				
			}
	
			int max_row = m_Ics.Get( m_FocusedRow ).GetColumnCount() - 1;
			if( max_row < m_FocusedColumn )
			{
				m_FocusedColumn = max_row;
			}
		}
	
		if( direction == Direction.DOWN )
		{
			m_FocusedRow++;
			if( m_FocusedRow == m_Ics.Count() )
			{
				m_FocusedRow = 0 ;
				cnt = Container.Cast( m_Parent.GetParent().GetParent() );
				if( cnt )
				{
					cnt.SetNextActive();
				}
				else
				{
					cnt = Container.Cast( m_Parent );
					cnt.SetNextActive();
				}
				return;
			}
	
			max_row = m_Ics.Get( m_FocusedRow ).GetColumnCount() - 1;
			if( max_row < m_FocusedColumn )
			{
				m_FocusedColumn = max_row;
			}
		}
	
		if( direction == Direction.RIGHT )
		{
			m_FocusedColumn++;
			if( m_FocusedColumn == m_Ics.Get( m_FocusedRow ).GetColumnCount() )
			{
				m_FocusedColumn = 0;
				
			}
		}
	
		if( direction == Direction.LEFT )
		{
			m_FocusedColumn--;
			if( m_FocusedColumn < 0 )
			{
				m_FocusedColumn = m_Ics.Get( m_FocusedRow ).GetColumnCount() - 1;
				
			}
		}
	
		//Container cnt = Container.Cast( m_Body.Get( 1 ) );
		m_Ics.Get( m_FocusedRow ).GetMainWidget().FindAnyWidget( "Cursor" + m_FocusedColumn ).Show( true );
		
		EntityAI focused_item = GetFocusedEntity();
		if( focused_item )
		{
			float x, y;
			m_Ics.Get( m_FocusedRow ).GetMainWidget().FindAnyWidget( "Cursor" + m_FocusedColumn ).GetScreenPos( x, y );
			ItemManager.GetInstance().PrepareTooltip( focused_item, x, y );
		}
		*/
	}
}
