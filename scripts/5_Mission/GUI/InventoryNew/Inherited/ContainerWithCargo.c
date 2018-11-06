class ContainerWithCargo: ClosableContainer
{
	ref IconsContainer m_IconsContainer;
	protected ref UICargoGrid m_CargoGrid;

	void ContainerWithCargo( Container parent )
	{
		m_Parent = parent;

		m_IconsContainer = new IconsContainer( this );
		this.Insert( m_IconsContainer );
	}

	EntityAI GetFocusedItem()
	{
		if( m_CargoGrid.GetFocusedItem() == NULL )
		{
			return NULL;
		}
		return EntityAI.Cast( m_CargoGrid.GetFocusedItem().GetObject() );
	}
	
	bool IsEmpty()
	{
		return m_CargoGrid.IsEmpty();
	}
	
	bool IsItemActive()
	{
		return m_CargoGrid.IsItemActive();
	}
	
	bool CanEquip()
	{
		return m_CargoGrid.CanEquip();
	}
	
	bool CanCombine()
	{
		return m_CargoGrid.CanCombine();
	}
	
	bool CanCombineAmmo()
	{
		return m_CargoGrid.CanCombineAmmo();
	}
	
	bool IsItemWithQuantityActive()
	{
		return m_CargoGrid.IsItemWithQuantityActive();
	}
	
	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		m_IconsContainer.RefreshQuantity( item_to_refresh );
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		m_IconsContainer.RefreshItemPosition( item_to_refresh );
	}
	
	override void UpdateInterval()
	{
		if( m_Entity )
		{
			if( m_Entity.GetInventory().IsInventoryLockedForLockType( HIDE_INV_FROM_SCRIPT ) || m_Closed )
			{
				HideContent();
			}
			else
			{
				ShowContent();
			}
		
			m_CargoGrid.UpdateInterval();
			#ifdef PLATFORM_CONSOLE
				m_IconsContainer.RecomputeItemPositions();
			#endif
		}
	}
	
	override void Refresh()
	{
		super.Refresh();
		#ifdef PLATFORM_CONSOLE
			m_IconsContainer.RecomputeItemPositions();
		#endif
	}

	override void MoveGridCursor( int direction )
	{
		m_CargoGrid.MoveGridCursor( direction );
	}

	void SetDefaultFocus( bool while_micromanagment_mode = false )
	{
		m_CargoGrid.SetDefaultFocus( while_micromanagment_mode );
	}
	
	override void EquipItem()
	{
		m_CargoGrid.EquipItem();
	}

	override void UnfocusAll()
	{
		m_CargoGrid.Unfocus();
	}
	
	override void TransferItem()
	{
		m_CargoGrid.TransferItem();
	}
	
	void TransferItemToVicinity()
	{
		m_CargoGrid.TransferItemToVicinity();
	}
	
	override void SelectItem()
	{
		Icon focused_item = m_CargoGrid.GetFocusedItem();
		if( !focused_item )
		{
			return;
		}
		ItemManager.GetInstance().SetSelectedItem( ItemBase.Cast( focused_item.GetObject() ), focused_item, NULL );
	}

	override void Select()
	{
		EntityAI prev_item;
		if( m_CargoGrid.GetFocusedItem() )
		{
			prev_item = EntityAI.Cast( m_CargoGrid.GetFocusedItem().GetObject() );
		}
		Man player = GetGame().GetPlayer();
		
		if( ItemManager.GetInstance().IsItemMoving() )
		{
			EntityAI selected_item = ItemManager.GetInstance().GetSelectedItem();
			if( selected_item && GetEntity() )
			{
				bool can_add = m_Entity.GetInventory().CanAddEntityInCargo( selected_item );
				bool in_cargo = !player.GetInventory().HasEntityInInventory( selected_item ) || !m_Entity.GetInventory().HasEntityInCargo( selected_item );
				if( can_add && in_cargo )
				{
					player.PredictiveTakeEntityToTargetCargo(m_Entity, selected_item);
					m_CargoGrid.SetDefaultFocusAfterInitIcon();
					Widget selected_widget1 = ItemManager.GetInstance().GetSelectedWidget();
					if( selected_widget1 )
					{
						selected_widget1.Show( false );
					}
				}
				else
				{
					m_CargoGrid.SetDefaultFocus( true );
					Icon selected_icon = ItemManager.GetInstance().GetSelectedIcon();
					if( selected_icon )
					{
						selected_icon.SetActive( false );
					}
					Widget selected_widget = ItemManager.GetInstance().GetSelectedWidget();
					if( selected_widget )
					{
						selected_widget.Show( false );
					}
				}
				
				if( m_Parent.IsInherited( PlayerContainer ) )
				{
					PlayerContainer player_container_parent = PlayerContainer.Cast( m_Parent );
					player_container_parent.UnfocusPlayerAttachmentsContainer();
				}
			}
		}
		else
		{
			if ( prev_item )
			{
				Icon icon = m_IconsContainer.GetIcon( prev_item.GetID() );
				
				if( icon )
				{
					EntityAI item_in_hands = GetGame().GetPlayer().GetHumanInventory().GetEntityInHands();
					if( item_in_hands )
					{
						if( GameInventory.CanSwapEntities( item_in_hands, prev_item ) )
						{
							player.PredictiveSwapEntities( item_in_hands, prev_item );
						}
						else
						{
							player.PredictiveSwapEntities( prev_item, item_in_hands);
						}
					}
					else
					{
						if( player.GetHumanInventory().CanAddEntityInHands( prev_item ) )
						{
							player.PredictiveTakeEntityToHands( prev_item );
						}
					}
					m_CargoGrid.PrepareCursorReactivation();
				}
			}		
		}
	}
	
	override void Combine()
	{
		if( !m_CargoGrid.GetFocusedItem() )
			return;
		EntityAI prev_item = EntityAI.Cast( m_CargoGrid.GetFocusedItem().GetObject() );
		Icon icon;
		if(prev_item)
			icon = m_IconsContainer.GetIcon( prev_item.GetID() );
		
		if( icon )
		{
			EntityAI item_in_hands = GetGame().GetPlayer().GetHumanInventory().GetEntityInHands();
			if( item_in_hands )
			{
				icon.CombineItems( item_in_hands, prev_item );
			}
		}
	}
	
	Widget GetLastRowWidget()
	{
		return m_CargoGrid.GetLastRowWidget();
	}

	void SetEntity( EntityAI entity )
	{
		m_Entity = entity;
		SetOpenState( ItemManager.GetInstance().GetDefaultOpenState( m_Entity.GetType() ) );

		Header h = Header.Cast( m_Body.Get( 0 ) );
		h.SetName( m_Entity.GetDisplayName() );
		h.SetItemPreview( m_Entity );

		m_CargoGrid = new UICargoGrid( entity, m_IconsContainer );
		m_CargoGrid.SetParent(this);
		( Container.Cast( m_Parent ) ).Insert(this);
		
		if( m_Entity.GetInventory().IsInventoryLockedForLockType( HIDE_INV_FROM_SCRIPT ) || m_Closed )
		{
			HideContent();
		}
		else
		{
			ShowContent();
		}
		
		Refresh();
		GetMainWidget().Update();
	}

	EntityAI GetEntity()
	{
		return m_Entity;
	}
	
	int GetCargoHeight()
	{
		return m_CargoGrid.GetCargoHeight();
	}

	EntityAI GetItemPreviewItem( Widget w )
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
		if( !ipw || !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return NULL;
		}
		return ipw.GetItem();
	}

	bool DraggingOverGrid( Widget w, int x, int y, Widget reciever )
	{
		if( w == NULL )
		{
			return false;
		}
		
		EntityAI item = GetItemPreviewItem( w );
		
		if( !item )
		{
			return false;
		}

		int color, c_x, c_y;
		int idx = 0;
		
		if( m_Entity.GetInventory().GetCargo() )
		{
			c_x = m_Entity.GetInventory().GetCargo().GetHeight();
			c_y = m_Entity.GetInventory().GetCargo().GetWidth();
		}
		
		if( m_Entity && c_x > x && c_y > y && m_Entity.GetInventory().CanAddEntityInCargoEx( item, 0, x, y ) )
		{
			ItemManager.GetInstance().HideDropzones();
			if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
			}
			else
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
			}
			color = ColorManager.GREEN_COLOR;
		}
		else
		{
			color = ColorManager.RED_COLOR;
			ItemManager.GetInstance().ShowSourceDropzone( item );
		}

		if( w.FindAnyWidget("Color") )
		{
			w.FindAnyWidget("Color").SetColor( color );
		}
		else
		{
			string name = w.GetName();
			name.Replace( "PanelWidget", "Col" );
			if( w.FindAnyWidget( name ) )
			{
				w.FindAnyWidget( name ).SetColor( color );
			}
		}

		return true;
	}

	void DropReceived( Widget w, int x, int y )
	{
						float xx, yy;
		GetMainWidget().Update();
		GetMainWidget().GetScreenSize( xx, yy );
		if( GetMainWidget().FindAnyWidget("Background") )
		{
			GetMainWidget().FindAnyWidget("Background").Show( true );
			GetMainWidget().FindAnyWidget("Background").SetSize( xx, yy );
		}
		EntityAI item = GetItemPreviewItem( w );
		if( !item )
		{
			return;
		}

		int idx = 0;
		int c_x, c_y;
		
		if( m_Entity.GetInventory().GetCargo() )
		{
			c_x = m_Entity.GetInventory().GetCargo().GetHeight();
			c_y = m_Entity.GetInventory().GetCargo().GetWidth();
		}
		
		if( c_x > x && c_y > y && m_Entity.GetInventory().CanAddEntityInCargoEx( item, idx, x, y ) )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			
			TakeIntoCargo( player, m_Entity, item, idx, x, y );
			
			Icon icon = m_IconsContainer.GetIcon( item.GetID() );
			
			if( icon && w && w.FindAnyWidget("Color") )
			{
				w.FindAnyWidget("Color").SetColor( ColorManager.BASE_COLOR );
				icon.RefreshPos( x, y );
				icon.Refresh();
				Refresh();
			}
		}
	}
	
	void TakeIntoHands( notnull PlayerBase player, notnull EntityAI item )
	{
		ItemBase item_base = ItemBase.Cast( item );
		float stackable = item_base.ConfigGetFloat("varStackMax");
		if( stackable == 0 || stackable >= item_base.GetQuantity() )
		{
			player.PredictiveTakeEntityToHands( item );
		}
		else if( stackable != 0 && stackable < item_base.GetQuantity() )
		{
			item_base.SplitIntoStackMaxHandsClient( player );
		}
	}
	
	void TakeIntoCargo( notnull PlayerBase player, notnull EntityAI entity, notnull EntityAI item, int idx = -1, int row = 0, int col = 0 )
	{
		ItemBase item_base = ItemBase.Cast( item );
		float stackable = item_base.ConfigGetFloat("varStackMax");
		if( stackable == 0 || stackable >= item_base.GetQuantity() )
		{
			if( idx != -1 )
				player.PredictiveTakeEntityToTargetCargoEx( m_Entity, item, idx, row, col );
			else
				player.PredictiveTakeEntityToTargetCargo(m_Entity, item);
		}
		else if( stackable != 0 && stackable < item_base.GetQuantity() )
		{
			item_base.SplitIntoStackMaxCargoClient( entity, idx, row, col );
		}
	}

	override void DraggingOverHeader( Widget w, int x, int y, Widget receiver )
	{
		DraggingOver( w, x, y, receiver );
	}

	override void DraggingOver( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		EntityAI item = GetItemPreviewItem( w );
		if( !item )
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if( ( m_Entity.GetInventory().CanAddEntityInCargo( item ) && (!player.GetInventory().HasEntityInInventory( item ) || !m_Entity.GetInventory().HasEntityInCargo( item )) ) || player.GetHumanInventory().HasEntityInHands( item ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			ItemManager.GetInstance().HideDropzones();
			if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
			}
			else
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
			}
		}
		else
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			ItemManager.GetInstance().ShowSourceDropzone( item );
		}
	}

	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
	{
		EntityAI item = GetItemPreviewItem( w );
		if( !item )
		{
			return;
		}

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if( GetEntity() )
		{
			bool can_add = m_Entity.GetInventory().CanAddEntityInCargo( item );
			bool in_cargo = !player.GetInventory().HasEntityInInventory( item ) || !m_Entity.GetInventory().HasEntityInCargo( item );
			if( can_add && in_cargo )
			{
				TakeIntoCargo( player, m_Entity, item );
			}
		}
	}

}
