class Container extends LayoutHolder
{
	protected ref array<ref LayoutHolder>	m_Body;
	protected ref array<ref LayoutHolder>	m_OpenedContainers;
	protected int							m_ActiveIndex = 1;
	protected bool							m_LastIndex;
	protected ref Container 				m_FocusedContainer;
	
	const int ITEMS_IN_ROW = 7;
	
	int										m_FocusedRow = 0;
	int										m_FocusedColumn = 0;

	void Container( LayoutHolder parent )
	{
		m_Body = new array<ref LayoutHolder>;
		m_OpenedContainers = new array<ref LayoutHolder>;
	}
	
	Container GetFocusedContainer()
	{
		return m_FocusedContainer;
	}
	
	void SetFocusedContainer( Container cont )
	{
		m_FocusedContainer = cont;
	}
	
	float GetFocusedContainerHeight()
	{
		float x, y;
		GetFocusedContainer().GetMainWidget().GetScreenSize( x, y );
		return y;
	}
	
	float GetFocusedContainerYPos()
	{
		float x, y;
		GetFocusedContainer().GetMainWidget().GetPos( x, y );
		return y;
	}

	int Count()
	{
		return m_Body.Count();
	}
	
	void MoveGridCursor( int direction )
	{
		
	}
	
	void Select()
	{
		GetFocusedContainer().Select();
	}
	
	void SelectItem()
	{
		
	}
	
	void Combine()
	{
		if( GetFocusedContainer() )
		{
			GetFocusedContainer().Combine();
		}
	}
	
	void TransferItem( )
	{
		GetFocusedContainer().TransferItem();
	}
	
	void EquipItem()
	{
		GetFocusedContainer().EquipItem();
	}

	void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver ) {}

	void DraggingOver( Widget w, int x, int y, Widget receiver ) {}

	void DraggingOverHeader( Widget w, int x, int y, Widget receiver ) {}

	override void UpdateInterval()
	{
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			m_Body.Get( i ).UpdateInterval();
		}
	}

	override void SetActive( bool active )
	{
		m_IsActive = active;
		if( m_MainWidget.FindAnyWidget("SelectedContainer") )
		{
			m_MainWidget.FindAnyWidget("SelectedContainer").Show(active);
		}
		else
		{
			if( active )
			{
				m_MainWidget.SetColor( ARGB( 255, 160, 160, 160 ) );
			}
			else
			{
				m_MainWidget.SetColor( ARGB( 255, 115, 115, 115 ) );
			}
		}
		
		if( active )
		{
			if( this.IsInherited( ContainerWithCargo ) )
			{
				ContainerWithCargo iwc = ContainerWithCargo.Cast( this );
				if( iwc )
				{
					iwc.SetDefaultFocus();
				}
			}
			else if( this.IsInherited( ContainerWithCargoAndAttachments ) )
			{
				ContainerWithCargoAndAttachments iwca = ContainerWithCargoAndAttachments.Cast( this );
				if( iwca )
				{
					iwca.SetDefaultFocus();
				}
			}
			else if(this.IsInherited( ContainerWithElectricManager ))
			{
			}
			else if(this.IsInherited( AttachmentCategoriesRow ))
			{
				AttachmentCategoriesRow att_row = AttachmentCategoriesRow.Cast( this );
				if( att_row )
				{
					att_row.SetDefaultFocus();
				}
			}
			else
			{
				if( !ItemManager.GetInstance().IsMicromanagmentMode() )
				{
					m_FocusedColumn = 0;
					m_FocusedRow = 0;
					if( m_Body.Count() )
					{
						LayoutHolder cnt = LayoutHolder.Cast( m_Body.Get( 0 ) );
						cnt.GetMainWidget().FindAnyWidget( "Cursor" + m_FocusedColumn ).Show( true );
						ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( cnt.GetMainWidget().FindAnyWidget( "Render" + m_FocusedColumn ) );
					}
					if( item_preview == NULL )
					{
						return;
					}
					EntityAI focused_item =  item_preview.GetItem();
					
					if( focused_item )
					{
						float x, y;
						cnt.GetMainWidget().FindAnyWidget( "Cursor" + m_FocusedColumn ).GetScreenPos( x, y );
						ItemManager.GetInstance().PrepareTooltip( focused_item, x, y );
					}
				}
			}
		}
	}

	void UnfocusAll()
	{
		for ( int i = 0; i < Count(); i++ )
		{
			for ( int j = 0; j < ITEMS_IN_ROW; j++ )
			{
				Get( i ).GetMainWidget().FindAnyWidget( "Cursor" + j ).Show( false );
			}
		}
	}

	void UnfocusGrid()
	{
		if( GetFocusedContainer() )
		{
			m_FocusedRow = 0;
			m_FocusedColumn = 0;
			GetFocusedContainer().UnfocusAll();
		}
		/*if( GetFocusedContainer() && !GetFocusedContainer().IsInherited( ContainerWithCargo ) && !GetFocusedContainer().IsInherited( ContainerWithCargoAndAttachments ) && !GetFocusedContainer().IsInherited( AttachmentCategoriesRow ) )
		{
			UnfocusAll();
			m_FocusedRow = 0;
			m_FocusedColumn = 0;
		}
		else
		{
			ContainerWithCargo iwc = ContainerWithCargo.Cast( GetFocusedContainer() );
			ContainerWithCargoAndAttachments iwca = ContainerWithCargoAndAttachments.Cast( GetFocusedContainer() );
			AttachmentCategoriesRow iwca2 = AttachmentCategoriesRow.Cast( GetFocusedContainer() );
			if( iwc )
			{
				iwc.UnfocusGrid();
			}
			else if( iwca )
			{
				iwca.UnfocusGrid();
			}
			else if( iwca2 )
			{
				iwca2.UnfocusAll();
			}
		}*/
	}

	bool IsLastIndex()
	{
		return m_LastIndex;
	}
	
	bool IsFirstIndex()
	{
		return m_ActiveIndex == 1; 
	}
	
	bool IsFirstContainerFocused()
	{
		Container first = Container.Cast( m_Body[1] );
		return GetFocusedContainer() == first;
	}
	
	bool IsLastContainerFocused()
	{
		Container first = Container.Cast( m_Body[ m_Body.Count() - 1 ] );
		return GetFocusedContainer() == first;
	}
	
	void SetFocused()
	{
		SetFocusedContainer( this );
	}
	
	void ResetFocusedContainer()
	{
		SetFocusedContainer( null );
	}

	void SetNextActive()
	{
		ItemManager.GetInstance().HideTooltip( );
		if( ItemManager.GetInstance().IsMicromanagmentMode() )
		{
			ItemManager.GetInstance().SetItemMoving( true );
		}
		Container active = Container.Cast( m_OpenedContainers[m_ActiveIndex] );
		if( !active.IsActive() )
		{
			return;
		}
		Container next;
		++m_ActiveIndex;
		if( m_ActiveIndex < m_OpenedContainers.Count() )
		{
			next = Container.Cast( m_OpenedContainers[m_ActiveIndex] );
			if( m_ActiveIndex == m_OpenedContainers.Count() - 1 )
			{
				m_LastIndex = true;
			}
			else
			{
				m_LastIndex = false;
			}
		}
		while( next && !next.GetMainWidget().IsVisible() )
		{
			++m_ActiveIndex;
			if( m_ActiveIndex < m_OpenedContainers.Count() )
			{
				next = Container.Cast( m_OpenedContainers[m_ActiveIndex] );
			}
		}
		if( next )
		{
			active.SetActive( false );
			next.SetActive( true );
			SetFocusedContainer( next );
		}
		else
		{
			m_ActiveIndex = 1;
			Container first = Container.Cast( m_OpenedContainers[m_ActiveIndex] );
			if( first )
			{
				active.SetActive( false );
				first.SetActive( true );
				SetFocusedContainer( first );
			}
		}
	}

	void SetPreviousActive()
	{
		if( ItemManager.GetInstance().IsMicromanagmentMode() )
		{
			ItemManager.GetInstance().SetItemMoving( true );
		}
		Container active = Container.Cast( m_OpenedContainers[m_ActiveIndex] );
		if( !active.IsActive() )
		{
			return;
		}
		Container prev;
		if( --m_ActiveIndex > 0 )
		{
			prev = Container.Cast( m_OpenedContainers[m_ActiveIndex] );
			if( m_ActiveIndex == m_OpenedContainers.Count() - 1 )
			{
				m_LastIndex = true;
			}
			else
			{
				m_LastIndex = false;
			}
		}
		while( prev && !prev.GetMainWidget().IsVisible() )
		{
			if( --m_ActiveIndex > 0 )
			{
				prev = Container.Cast( m_OpenedContainers[m_ActiveIndex] );
			}
		}
		if(prev)
		{
			active.SetActive( false );
			prev.SetActive( true );
			SetFocusedContainer( prev );
		}
		else
		{
			m_ActiveIndex = m_OpenedContainers.Count() - 1;
			Container first = Container.Cast( m_OpenedContainers[m_ActiveIndex] );
			if( first )
			{
				active.SetActive( false );
				first.SetActive( true );
				SetFocusedContainer( first );
			}
		}
	}
	
	void RecomputeOpenedContainers()
	{
		m_OpenedContainers.Clear();
		m_OpenedContainers.Insert( m_Body[0] );
		m_OpenedContainers.Insert( m_Body[1] );
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			ClosableContainer cnt = ClosableContainer.Cast( m_Body.Get( i ) );
			if( cnt && cnt.IsOpened() )
			{
				m_OpenedContainers.Insert( cnt );
			}
		}
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.ClosableContainer;
	}

	void Insert( LayoutHolder container )
	{
		m_Body.Insert( container );
	}

	void Remove( LayoutHolder container )
	{
		if( m_Body )
		{
			m_Body.RemoveOrdered( m_Body.Find( container ) );
		}
	}

	/*void Remove( int x )
	{
		if( m_Body )
		{
			m_Body.Remove( x );
		}
	}*/

	LayoutHolder Get( int x )
	{
		return m_Body.Get( x );
	}

	override void Refresh()
	{
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			m_Body.Get( i ).Refresh();
		}
	}

	void RefreshQuantity( EntityAI item_to_refresh )
	{
	}
	
	void RefreshItemPosition( EntityAI item_to_refresh )
	{
	}

	void UpdateSpacer()
	{
	}

	void UpdateBodySpacers()
	{
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			Container c = Container.Cast( m_Body.Get( i ) );
			if( c && c.IsInherited( Container ) )
			{
				c.UpdateSpacer();
			}
		}
		
		UpdateSpacer();
	}

	void HideContent()
	{
		for( int i = 1; i < m_Body.Count(); i++ )
		{
			m_Body.Get( i ).OnHide();
		}
	}
	
	void ShowContent()
	{
		for( int i = 1; i < m_Body.Count(); i++ )
		{
			m_Body.Get( i ).OnShow();
		}
	}
}
