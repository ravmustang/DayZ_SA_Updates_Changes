class HandsArea: ContainerBase
{
	ref HandsContainer		m_HandsContainer;
	ref AutoHeightSpacer	m_Spacer;

	void HandsArea( ContainerBase parent )
	{
		Widget root_panel = m_MainPanel;
		
		m_MainPanel = m_MainPanel.FindAnyWidget( "Content" );

		m_HandsContainer = new HandsContainer( this );

		m_MainPanel.GetScript( m_Spacer );
		m_MainPanel.Update();
		m_Spacer.Update();
	}

	override void SetActive( bool active )
	{
			m_HandsContainer.SetActive( active );
	}

	override bool IsActive()
	{
		return m_HandsContainer.IsActive();
	}
	
	bool CanEquip()
	{
		return m_HandsContainer.CanEquip();
	}
	
	void UnfocusGrid()
	{
		m_HandsContainer.UnfocusGrid();
	}
	
	void SetNextActive()
	{
		m_HandsContainer.SetNextActive();
	}
	
	void SetPreviousActive()
	{
		m_HandsContainer.SetPreviousActive();
	}

	bool IsSwapActive()
	{
		Widget swap_cursor = m_MainPanel.FindAnyWidget("SwapCursor");
		if( swap_cursor )
		{
			return swap_cursor.IsVisible();
		}
		else
		{
			return false;
		}
	}

	bool IsCombineActive()
	{
		Widget combine_cursor = m_MainPanel.FindAnyWidget("CombineCursor");

		if( combine_cursor )
		{
			return combine_cursor.IsVisible();
		}
		else
		{
			return false;
		}
	}
	
	void MoveGridCursor( int direction )
	{
		if( IsActive() )
		{
			m_HandsContainer.MoveGridCursor( direction );
		}
	}
	
	void EquipItem()
	{
		m_HandsContainer.EquipItem();
	}
	
	void SelectItem()
	{
		m_HandsContainer.SelectItem();
	}

	void Select()
	{
		m_HandsContainer.Select();
	}
	
	void TransferItem()
	{
		m_HandsContainer.TransferItem();
	}
	
	void TransferItemToVicinity()
	{
		m_HandsContainer.TransferItemToVicinity();
	}
	
	bool IsItemActive()
	{
		return m_HandsContainer.IsItemActive();
	}
	
	bool IsItemWithQuantityActive()
	{
		return m_HandsContainer.IsItemWithQuantityActive();
	}
	
	bool IsEmpty()
	{
		return m_HandsContainer.IsEmpty();
	}

	override void UpdateInterval()
	{
		float x, y;
		GetMainPanel().GetSize( x, y );
		if( y > 500 )
		{
			y = 500;
		}
		GetMainPanel().GetParent().SetSize( x, y );
		m_HandsContainer.UpdateInterval();
	}

	void RefreshQuantity( EntityAI item_to_refresh )
	{
		m_HandsContainer.RefreshQuantity( item_to_refresh );
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.HandsArea;
	}

	override void SetParentWidget()
	{
		m_ParentWidget = m_Parent.GetMainPanel().FindAnyWidget( "HandsPanel" );
	}

	override void OnShow()
	{
		super.OnShow();
		m_HandsContainer.OnShow();
		m_Spacer.Update();
	}

	override void Refresh()
	{
		super.Refresh();
		m_HandsContainer.Refresh();
		m_MainPanel.Update();
		m_Spacer.Update();
	}
	
	void DraggingOverHandsPanel( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		
		if( !ipw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		
		if( !ipw )
		{
		  ipw = ItemPreviewWidget.Cast( w );
		}
		
		if( !ipw || !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;
		}

		ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
		ItemManager.GetInstance().HideDropzones();
		ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
	}
	
	void OnHandsPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		
		if( !ipw )
		{
		  ipw = ItemPreviewWidget.Cast( w );
		}
		
		if( !ipw || ipw.GetItem() == NULL || !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;
		}
		
		if( GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( ipw.GetItem() ) )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToHands( ipw.GetItem() );		
		}
	}
}
