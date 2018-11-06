class RightArea: Container
{
	ref PlayerContainer m_PlayerContainer;
	void RightArea( LayoutHolder parent )
	{
		m_MainWidget.Show( true );
		m_MainWidget = m_MainWidget.FindAnyWidget( "Content" );
		
		m_PlayerContainer = new PlayerContainer( this );
		m_PlayerContainer.SetPlayer( PlayerBase.Cast( GetGame().GetPlayer() ) );
		m_Body.Insert( m_PlayerContainer );
		m_ActiveIndex = 0;
	}
	
	override Container GetFocusedContainer()
	{
		return m_FocusedContainer;
	}
	
	override void SetFocusedContainer( Container cont )
	{
		m_FocusedContainer = cont;
	}
	
	EntityAI GetFocusedItem()
	{
		return m_PlayerContainer.GetFocusedItem();
	}
	
	PlayerContainer GetPlayerContainer()
	{
		return m_PlayerContainer;
	}
	
	override void MoveGridCursor( int direction )
	{
		m_PlayerContainer.MoveGridCursor( direction );
	}
	
	override void Select()
	{
		m_PlayerContainer.Select();
	}
	
	override void SelectItem()
	{
		m_PlayerContainer.SelectItem();
	}
	
	override void Combine()
	{
		m_PlayerContainer.Combine();
	}
	
	override void EquipItem()
	{
		m_PlayerContainer.EquipItem();
	}
	
	override void TransferItem()
	{
		m_PlayerContainer.TransferItem();
	}
	
	void TransferItemToVicinity()
	{
		m_PlayerContainer.TransferItemToVicinity();
	}
	
	override void UnfocusGrid()
	{
		m_PlayerContainer.UnfocusGrid();
	}
	
	override void ResetFocusedContainer()
	{
		m_PlayerContainer.ResetFocusedContainer();
	}
	
	bool IsPlayerEquipmentActive()
	{
		return m_PlayerContainer.IsPlayerEquipmentActive();
	}
	
	void ExpandCollapseContainer()
	{
		m_PlayerContainer.ExpandCollapseContainer();
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		m_PlayerContainer.RefreshItemPosition( item_to_refresh );
	}
	
	override void SetActive( bool active )
	{
		m_PlayerContainer.SetActive( active );
		if( active )
		{
			m_PlayerContainer.SetFirstActive();
		}
	}
	
	override void SetNextActive()
	{
		m_PlayerContainer.SetNextActive();
	}
	
	override void SetPreviousActive()
	{
		m_PlayerContainer.SetPreviousActive();
	}
	
	override bool IsActive()
	{
		return m_PlayerContainer.IsActive( );
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.LeftArea;
	}
	
	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		m_PlayerContainer.RefreshQuantity( item_to_refresh );
	}

	override void SetParentWidget()
	{
		m_ParentWidget = m_Parent.GetMainWidget().FindAnyWidget( "RightPanel" );
	}

	override void OnShow()
	{
		super.OnShow();
		Refresh();
	}
}
