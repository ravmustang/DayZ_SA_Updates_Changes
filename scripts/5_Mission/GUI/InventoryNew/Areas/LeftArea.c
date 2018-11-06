class LeftArea: Container
{
	protected ref VicinityContainer m_VicinityContainer;
	
	void LeftArea( LayoutHolder parent )
	{
		m_MainWidget.Show( true );
		m_MainWidget = m_MainWidget.FindAnyWidget( "Content" );

		m_VicinityContainer = new VicinityContainer( this );
		m_Body.Insert( m_VicinityContainer );
		m_ActiveIndex = 0;
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.RefreshItemPosition( item_to_refresh );
	}
	
	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.RefreshQuantity( item_to_refresh );
	}
	
	override void MoveGridCursor( int direction )
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.MoveGridCursor( direction );
	}
	
	override void SelectItem()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.SelectItem();
	}
		
	override void Select()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.Select();
	}
	
	override void Combine()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.Combine();
	}
	
	override void TransferItem()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.TransferItem();
	}
	
	override void EquipItem()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.EquipItem();
	}
	
	override void UnfocusGrid()
	{
		Container active_container;
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			active_container = Container.Cast( m_Body.Get( i ) );
			active_container.UnfocusGrid();
		}
	}
	
	override void ResetFocusedContainer()
	{
		m_ActiveIndex = 0;
		Container active_container;
		for ( int i = 0; i < m_Body.Count(); i++ )
		{
			active_container = Container.Cast( m_Body.Get( i ) );
			active_container.ResetFocusedContainer();
		}
	}
	
	override bool IsActive()
	{
		if( m_Body.Count() <= m_ActiveIndex )
		{
			m_ActiveIndex = 0;
		}
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		return active_container.IsActive( );
	}
	
	override void SetActive( bool active )
	{
		if( m_Body.Count() <= m_ActiveIndex )
		{
			m_ActiveIndex = 0;
		}
		
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		active_container.SetActive( active );
		if( active )
		{
			if( active_container.IsInherited( CollapsibleContainer ) )
			{
				CollapsibleContainer.Cast( active_container ).SetFirstActive();
			}	
		}
	}
	
	override void SetNextActive()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		if( !active_container.IsActive() )
		{
			return;
		}

		if( active_container.IsLastContainerFocused() )
		{
			++m_ActiveIndex;
			if( m_ActiveIndex < m_Body.Count() )
			{
			}
			else
			{
				m_ActiveIndex = 0;
			}

			active_container.SetActive( false );
			active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
			active_container.SetActive( true );
			if( active_container.IsInherited( CollapsibleContainer ) )
			{
				CollapsibleContainer.Cast( active_container ).SetFirstActive();
			}
		}
		else
		{
			active_container.SetNextActive();
			float amount = active_container.GetFocusedContainerHeight() + active_container.GetFocusedContainerYPos();
		}
	}
	
	override void SetPreviousActive()
	{
		Container active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
		if( !active_container.IsActive() )
		{
			return;
		}
		
		if( active_container.IsFirstIndex() )
		{
			--m_ActiveIndex;
			if ( m_ActiveIndex < 0 )
			{
				m_ActiveIndex = m_Body.Count() - 1;
			}
			
			active_container.SetActive( false );
			active_container = Container.Cast( m_Body.Get( m_ActiveIndex ) );
			active_container.SetActive( true );
			if( active_container.IsInherited( CollapsibleContainer ) )
			{
				CollapsibleContainer.Cast( active_container ).SetLastActive();
			}
		}
		else
		{
			active_container.SetPreviousActive();
		}
		
	}
	
	void OnLeftPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		m_VicinityContainer.OnLeftPanelDropReceived( w, x, y, receiver );
	}
	
	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.LeftArea;
	}
	
	VicinityContainer GetVicinityContainer()
	{
		return m_VicinityContainer;
	}
	
	override void SetParentWidget()
	{
		m_ParentWidget = m_Parent.GetMainWidget().FindAnyWidget( "LeftPanel" );
	}
	
	override void OnShow()
	{
		super.OnShow();
		Refresh();
	}
}
