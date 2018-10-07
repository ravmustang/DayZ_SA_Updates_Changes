class InventoryQuickbar: InventoryGridController
{
	
	protected ref TItemsMap	m_Items;//tmp
	protected InventoryGrid	m_Grid;
	protected int			m_DraggingIndex;
	protected int			m_QuickbarSize;

	protected int			m_QuickBarGridWidth;
	
	void InventoryQuickbar(Widget quickbarGridWidget)
	{
		float x, y;
		quickbarGridWidget.GetScreenSize( x, y );
		m_QuickBarGridWidth = y;
		m_DraggingIndex = INDEX_NOT_FOUND;
		m_Items = new TItemsMap;
		UpdateItems( quickbarGridWidget );
	}

	void ~InventoryQuickbar()
	{
	}
	
	void UpdateItems( Widget quickbarGridWidget )
	{
		if( !quickbarGridWidget )
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( !player )
			return;
		
		int i;
		float width, height, x, y, screen_w, screen_h;
		quickbarGridWidget.GetScreenSize( x, y );
		m_QuickBarGridWidth = y;

		// create grid and align it to center
		if( quickbarGridWidget )
		{
			quickbarGridWidget.GetScript( m_Grid );
		}
		
		m_Items.Clear();
		
		if( m_Grid )
		{
			if( m_QuickbarSize != player.GetQuickBarSize() )
			{
				m_Grid.UpdateQuickbarItems( m_Items );
				
				while( m_Grid.GetRoot().GetChildren() )
				{
					delete m_Grid.GetRoot().GetChildren();
				}

				m_QuickbarSize = player.GetQuickBarSize();
				m_Grid.SetGridItemsSize( m_QuickBarGridWidth, m_QuickBarGridWidth );
				m_Grid.SetGridSize( m_QuickbarSize,1 );
				m_Grid.SetController( this );		
				m_Grid.GenerateQuickBarBackgroundTiles( m_QuickbarSize );
				
				float size = m_QuickBarGridWidth / 2.5;
				m_Grid.SetQuantityPanelSize( size );
			}
		}
		
		// align quickbar to center
		quickbarGridWidget.GetSize( width, height );
		quickbarGridWidget.GetPos(x, y);
		quickbarGridWidget.GetParent().GetScreenSize( screen_w, screen_h );
		float new_x =  0.5 - ( ( width / screen_w ) * 0.5 );
		quickbarGridWidget.SetPos( new_x, y );
		quickbarGridWidget.SetColor( 0x7FFFFFFF );

		m_QuickbarSize = player.GetQuickBarSize();
		for( i = 0; i < m_QuickbarSize; i++)
		{
			InventoryItem item = InventoryItem.Cast( player.GetQuickBarEntity(i) );
			if( item ) 
			{
				m_Items.Set( item, Vector(i, 1, 1) );
			}
		}
		
		if( m_Grid )
		{
			m_Grid.UpdateQuickbarItems( m_Items );
		}
	}

	void Remove(InventoryItem itemToRemove)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

		for ( int i = 0; i < m_QuickbarSize; i++ )
		{
			InventoryItem item = InventoryItem.Cast( player.GetQuickBarEntity(i) );
			if(item == itemToRemove) 
			{
				player.RemoveQuickBarEntityShortcut( item );
				m_Items.Remove( item );
			}
		}
	}
	
	// InventoryGridController override
	override void OnItemEnter(InventoryGrid grid, Widget w, int row, int col)
	{
		Widget quickbar = grid.GetRoot();
		if( quickbar )
		{
			// quickbar.SetColor( 0xFFFFFFFF );
		}
	}

	override void OnItemLeave(InventoryGrid grid, Widget w)
	{
		Widget quickbar = grid.GetRoot();
		if( quickbar )
		{
			// quickbar.SetColor( 0x7FFFFFFF );
		}
	}

	override void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col)
	{
		bool old_inventory = false;
		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().GetMenu() );
		if( menu )
		{
			// menu.HidePanelBorders();
		}
		ItemPreviewWidget iw;
		InventoryItem dragged_item;
		InventoryManager manager = InventoryManager.GetInstance();
		if( manager )
		{
			dragged_item = manager.GetItem();
			if( dragged_item )
			{
				old_inventory = true;
			}
			else
			{
				iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
				if( !iw )
				{
				  string name = w.GetName();
				  name.Replace( "PanelWidget", "Render" );
				  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				} 
				if( !iw || !iw.GetItem() )
				{
					return;
				}
			}
			if(!dragged_item && iw)
			dragged_item = InventoryItem.Cast( iw.GetItem() );

			if( dragged_item == NULL) return;
			
			if(dragged_item && dragged_item.GetInventory().CanRemoveEntity() == false)
			{
				if( old_inventory )
				{
					manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
				}
				else
				{
					int color;

					color = ARGB( 150, 255, 1, 1 );
	
					if( w.FindAnyWidget("Color") )
					{
						w.FindAnyWidget("Color").SetColor( color );
					}
					else
					{
						name = w.GetName();
						name.Replace( "PanelWidget", "Col" );
						w.FindAnyWidget( name ).SetColor( color );
						if( w.FindAnyWidget( name ) )
						{
							w.FindAnyWidget( name ).SetColor( color );
						}
					}
				}				
			}
			else
			{
				if( old_inventory )
				{
					manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
				}
				else
				{
					color = ARGB( 150, 1, 255, 1 );

					if( w.FindAnyWidget("Color") )
					{
						w.FindAnyWidget("Color").SetColor( color );
					}
					else
					{
						name = w.GetName();
						name.Replace( "PanelWidget", "Col" );
						if( w.FindAnyWidget( name ) )
						{
							w.FindAnyWidget( name ).SetColor( color );
						}
					}
				}
			}
		}
	}

	override void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col)
	{
		if( GetGame().IsOldInventory() )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			InventoryManager manager = InventoryManager.GetInstance();
			if ( manager )
			{
				InventoryItem dragged_item = manager.GetItem();
				
				ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget("Render") );
				if(!dragged_item && iw)
				dragged_item = InventoryItem.Cast( iw.GetItem() );
				
				InventoryItem quickbar_item = InventoryItem.Cast( player.GetQuickBarEntity(col) );
				
				/*if (manager.IsDragging() == false && m_DraggingIndex == INDEX_NOT_FOUND)
				{
					return;
				}*/
				
				if (m_DraggingIndex != INDEX_NOT_FOUND)
				{
					dragged_item = InventoryItem.Cast( player.GetQuickBarEntity(m_DraggingIndex) );
				}
					
				if (dragged_item && dragged_item.GetInventory().CanRemoveEntity())
				{
					player.SetQuickBarEntityShortcut(dragged_item, col);
					
					if (quickbar_item && quickbar_item != dragged_item && m_DraggingIndex != INDEX_NOT_FOUND)
					{
						player.SetQuickBarEntityShortcut(quickbar_item, m_DraggingIndex);
					}
				}
				m_DraggingIndex = INDEX_NOT_FOUND;
				//manager.Drop();
				InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
				if ( menu )
				{
					menu.RefreshQuickbar();
				}
			}
		}
		else
		{
			player = PlayerBase.Cast( GetGame().GetPlayer() );
			manager = InventoryManager.GetInstance();
			if ( manager )
			{
				dragged_item = manager.GetItem();
				
				iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
				if( !iw )
				{
				  string name = w.GetName();
				  name.Replace( "PanelWidget", "Render" );
				  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				}
				
				if( !dragged_item && iw )
					dragged_item = InventoryItem.Cast( iw.GetItem() );
				
				quickbar_item = InventoryItem.Cast( player.GetQuickBarEntity(col) );
				
				if (m_DraggingIndex != INDEX_NOT_FOUND)
				{
					dragged_item = InventoryItem.Cast( player.GetQuickBarEntity(m_DraggingIndex) );
				}
					
				if (dragged_item && dragged_item.GetInventory().CanRemoveEntity())
				{
					player.SetQuickBarEntityShortcut(dragged_item, col);
					
					if (quickbar_item && quickbar_item != dragged_item && m_DraggingIndex != INDEX_NOT_FOUND)
					{
						player.SetQuickBarEntityShortcut(quickbar_item, m_DraggingIndex);
					}
				}
				m_DraggingIndex = INDEX_NOT_FOUND;
				//manager.Drop();
				menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
				if ( menu )
				{
					menu.RefreshQuickbar();
				}
			}
		}
	}
	
	override void OnItemDrop(InventoryGrid grid, Widget w, int row, int col)
	{
		if (m_DraggingIndex != INDEX_NOT_FOUND)
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			InventoryItem dragged_item = InventoryItem.Cast( player.GetQuickBarEntity(m_DraggingIndex) );
			Remove(dragged_item);
			m_DraggingIndex = INDEX_NOT_FOUND;
		}
		
		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
		ItemManager.GetInstance().HideDropzones();
	}

	override void OnItemDrag(InventoryGrid grid, Widget w, int row, int col)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );		
		InventoryItem item = InventoryItem.Cast( player.GetQuickBarEntity(col) );
		if (item)
		{	
			m_DraggingIndex = col;
			m_Grid.SetItemColor(item, InventoryGrid.ITEM_COLOR_DRAG);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
	}
	
	int GetQuickbarWidth()
	{
		return m_QuickBarGridWidth;
	}
	
	override int GetQuickbarItemColor(InventoryGrid grid, InventoryItem item)
	{
		int color = InventoryGrid.ITEM_COLOR_QUICKBAR_NORMAL;
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI itemInHand = player.GetHumanInventory().GetEntityInHands();
		if( player && itemInHand == item )
		{
			if(player.GetInventory().CanAddEntityToInventory(item))
				color = InventoryGrid.ITEM_COLOR_QUICKBAR_H_GOOD;
			else
				color = InventoryGrid.ITEM_COLOR_QUICKBAR_H_BAD;
		}
		else if( itemInHand )
		{
			if(!player.GetInventory().CanAddEntityToInventory(itemInHand) && !GameInventory.CanSwapEntities(item, itemInHand) )	
			{
				color = InventoryGrid.ITEM_COLOR_QUICKBAR_I_BAD;
			}
			
		}
		return color;
	}

	
	override int HasItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		if ( manager )  
		{
			if ( item )
			{
				if ( item.IsInherited( InventoryItem) )
				{
					int has_quantity = manager.HasItemQuantity( item );
					return has_quantity;
				}
			}
		}
		return false;
	}

	override string GetItemQuantityText( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		string quantity_text = manager.GetItemQuantityText( item );
		return quantity_text;
	}

	override float GetItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		float quantity = manager.GetItemQuantity( item );
		return quantity;
	}

	/*void OnItemEnter(InventoryGrid grid, Widget w, int row, int col);
	void OnItemLeave(InventoryGrid grid, Widget w);
	void OnItemDrag(InventoryGrid grid, Widget w, int row, int col);
	void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col);
	void OnItemDrop(InventoryGrid grid, Widget w);
	void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col);
	void OnItemRightClick(InventoryGrid grid, Widget w, int row, int col);
	void OnItemDoubleClick(InventoryGrid grid, Widget w, int row, int col);*/
}
