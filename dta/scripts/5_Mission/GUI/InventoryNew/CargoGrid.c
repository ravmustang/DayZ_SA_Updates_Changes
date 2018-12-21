class UICargoGrid
{
	protected const int ROWS_NUMBER_XBOX = 5;
	protected EntityAI						m_Entity;
	protected IconsContainer				m_IconsContainer;
	protected ref array<int>				m_ShowedItems;
	protected ref array<ref GridContainer>	m_Rows;
	protected Container						m_Parent;
	protected bool							m_SetDefaultFocusAfterInitIcon;
	protected bool							m_RefreshItemPositions = true;
	
	void SetParent( Container parent )
	{
		m_Parent			= parent;
		
		TextWidget tw		= TextWidget.Cast( GetParent().GetMainWidget().FindAnyWidget("CargoCount") );
		
		if( tw )
			tw.Show( true );
		
		int capacity;
		int occupied_cargo;
		if( m_Entity )
		{
		
			#ifdef PLATFORM_WINDOWS
			CargoGrid grid	= CargoGrid.Cast( m_Entity.GetInventory().GetCargo() );
			capacity		= grid.GetWidth() * grid.GetHeight();
			for( int i = 0; i < grid.GetItemCount(); i++ )
			{
				int w, h;
				grid.GetItemSize( i, w, h );
				occupied_cargo += ( w*h );
			}
			#else
			#ifdef PLATFORM_CONSOLE
			capacity		= CargoList.Cast( m_Entity.GetInventory().GetCargo() ).GetMaxWeight();
			occupied_cargo	= CargoList.Cast( m_Entity.GetInventory().GetCargo() ).GetTotalWeight( null );
			#endif
			#endif
		}
		
		if(tw)
			tw.SetText( occupied_cargo.ToString() + "/" + capacity.ToString() );
	}
	
	void UICargoGrid( EntityAI entity, IconsContainer items_container )
	{
		m_Entity				= entity;
		m_IconsContainer		= items_container;
		m_ShowedItems			= new array<int>;
		m_Rows					= new array<ref GridContainer>;
		
		#ifndef PLATFORM_CONSOLE
		InitGridHeight();
		#endif
		
		m_RefreshItemPositions	= true;
	}
	
	Widget GetLastRowWidget()
	{
		return m_Rows.Get( m_Rows.Count() - 1 ).GetMainWidget();
	}
	
	void SetActive( bool active )
	{
		m_IconsContainer.SetActive( active );
		if( active )
		{
			SetDefaultFocus();
		}
	}
	
	Container GetParent()
	{
		return m_Parent;
	}
	
	bool IsActive()
	{
		return m_IconsContainer.IsActive();
	}
	
	bool IsEmpty()
	{
		return m_IconsContainer.GetItemCount() == 0;
	}
	
	bool IsItemActive()
	{
		if( GetFocusedItem() == NULL )
		{
			return false;
		}
		ItemBase item = ItemBase.Cast( GetFocusedItem().GetObject() );
		return !IsEmpty() && ( !QuantityConversions.HasItemQuantity( item )  || ( QuantityConversions.HasItemQuantity( item ) && !item.CanBeSplit() ) );
	}
	
	bool IsItemWithQuantityActive()
	{
		if( GetFocusedItem() == NULL )
		{
			return false;
		}
		ItemBase item = ItemBase.Cast( GetFocusedItem().GetObject() );
		return !IsEmpty() && QuantityConversions.HasItemQuantity( item ) && item.CanBeSplit();
	}
	
	int GetCargoHeight()
	{
		return m_Rows.Count();
	}
	
	void InitGridHeight()
	{
		for( int i = 0; i < m_Rows.Count(); i++ )
		{
			m_IconsContainer.Remove( m_Rows.Get( i ) );
		}
		
		m_Rows.Clear();
		
		//START - Init grid rows
		
		#ifdef PLATFORM_CONSOLE
			int cargo_height = ( ( m_IconsContainer.GetItemCount() / ROWS_NUMBER_XBOX ) * 2 ) + 2;
			if( m_IconsContainer.GetItemCount() % ROWS_NUMBER_XBOX == 0 )
			{
				cargo_height = ( ( m_IconsContainer.GetItemCount() / ROWS_NUMBER_XBOX ) * 2 );
			}
			if( m_IconsContainer.GetItemCount() == 0 )
			{
				cargo_height = 2;
			}
		#else
			int cargo_height = 	m_Entity.GetInventory().GetCargo().GetHeight();
		#endif

		for ( int j = 0; j < cargo_height; j++ )
		{
			GridContainer row = new GridContainer( m_IconsContainer );
			row.SetNumber( j );
			row.SetEntity( m_Entity );
			row.SetWidth( m_Entity.GetInventory().GetCargo().GetWidth() );
			row.GetMainWidget().SetSort( j );
			m_IconsContainer.Insert( row );
			m_Rows.Insert( row );
		}
	
		#ifndef PLATFORM_CONSOLE
		m_IconsContainer.Refresh();
		m_IconsContainer.RecomputeItemPositions();
		#endif
		//END - Init grid rows
	}
	
	void RecomputeGridHeight()
	{
		
	}
	
	void Remove()
	{
		m_Rows.Clear();
		delete m_IconsContainer;
	}
	
	void TransferItemToVicinity()
	{
		Man player = GetGame().GetPlayer();
		Icon icon = GetFocusedItem();
		if( icon )
		{
			EntityAI entity = EntityAI.Cast( icon.GetObject() );
			if( entity && player.CanDropEntity( entity ) )
			{
				player.PredictiveDropEntity( entity );
			}
		}
		m_ReactivateCursor = true;
	}
	
	void TransferItem()
	{
		if( GetFocusedItem() == NULL )
		{
			return;
		}
		EntityAI entity = EntityAI.Cast( GetFocusedItem().GetObject() );
		if( entity )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.CARGO, entity );
		}
		m_ReactivateCursor = true;
	}
	
	bool m_ReactivateCursor;
	
	void PrepareCursorReactivation()
	{
		m_ReactivateCursor = true;
	}
	
	bool CanEquip()
	{
		if( GetFocusedItem() == NULL )
		{
			return false;
		}
		EntityAI entity = EntityAI.Cast( GetFocusedItem().GetObject() );
		InventoryLocation il = new InventoryLocation;
		if( entity.IsInherited( Magazine ) )
		{
			return false;
		}
		bool found = GetGame().GetPlayer().GetInventory().FindFreeLocationFor(entity,FindInventoryLocationType.ATTACHMENT,il);
		if (found)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	int GetRecipeCount( bool recipe_anywhere, EntityAI entity1, EntityAI entity2 )
	{
		PluginRecipesManager plugin_recipes_manager = PluginRecipesManager.Cast( GetPlugin( PluginRecipesManager ) );
		return plugin_recipes_manager.GetValidRecipes( ItemBase.Cast( entity1 ), ItemBase.Cast( entity2 ), NULL, PlayerBase.Cast( GetGame().GetPlayer() ) );
	}
	
	bool CanCombine()
	{
		if( GetFocusedItem() == NULL )
		{
			return false;
		}
		ItemBase ent = ItemBase.Cast( GetFocusedItem().GetObject() );
		ItemBase item_in_hands = ItemBase.Cast(	GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() );
		
		return GetRecipeCount( false, ent, item_in_hands ) > 0;
	}
	
	bool CanCombineAmmo()
	{
		if( GetFocusedItem() == NULL )
		{
			return false;
		}
		PlayerBase m_player = PlayerBase.Cast( GetGame().GetPlayer() );
		ItemBase ent = ItemBase.Cast( GetFocusedItem().GetObject() );
		ItemBase item_in_hands = ItemBase.Cast(	GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() );
		ActionManagerClient amc;
		Class.CastTo(amc, m_player.GetActionManager());

		return amc.CanPerformActionFromInventory( item_in_hands, ent );
	}
	
	void EquipItem()
	{
		if( GetFocusedItem() == NULL )
		{
			return;
		}
		ItemBase entity = ItemBase.Cast( GetFocusedItem().GetObject() );
		if( entity )
		{
			if( entity.HasQuantity() )
			{
				entity.OnRightClick();
				Icon icon = m_IconsContainer.GetIcon( entity.GetID() );
				
				if ( icon )
				{
					icon.RefreshQuantity();
				}
			}
			else
			{
				GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.ATTACHMENT, entity );
			}
			m_ReactivateCursor = true;		
		}
	}
	
	Icon GetFocusedItem()
	{
		if( m_IconsContainer.GetItemCount() <= m_FocusedItemPosition )
		{
			return NULL;
		}
		else
		{
			return m_IconsContainer.GetIconByIndex( m_FocusedItemPosition );
		}
	}
	
	int m_FocusedItemPosition = 0;
	void MoveGridCursor( int direction )
	{
		if( !ItemManager.GetInstance().IsMicromanagmentMode() )
		{
			Container cnt;
			
			Icon icon = m_IconsContainer.GetIconByIndex( m_FocusedItemPosition );
			if( icon != NULL )
			{
				icon.SetActive( false );
				ItemManager.GetInstance().HideTooltip( );
				//m_FocusedItemPosition = 0;
				//return;
			}
			
			int focused_row = m_FocusedItemPosition / ROWS_NUMBER_XBOX;
			int max_rows = m_IconsContainer.GetItemCount() / ROWS_NUMBER_XBOX + 1;
			if( m_IconsContainer.GetItemCount() % ROWS_NUMBER_XBOX == 0 )
			{
				max_rows = m_IconsContainer.GetItemCount() / ROWS_NUMBER_XBOX;
			}
			int row_min = focused_row * ROWS_NUMBER_XBOX;
			int row_max = row_min + ROWS_NUMBER_XBOX - 1;
			
			if(row_max >= m_IconsContainer.GetItemCount() )
			{
				row_max = m_IconsContainer.GetItemCount() - 1;
			}
			
			if( direction == Direction.UP )
			{
				m_FocusedItemPosition -= ROWS_NUMBER_XBOX;
				if( m_FocusedItemPosition < 0 )
				{
					m_FocusedItemPosition = m_IconsContainer.GetItemCount() - 1;
					cnt = Container.Cast( m_IconsContainer.GetParent().GetParent().GetParent() );
					if( cnt )
					{
						cnt.SetPreviousActive();
					}
					else
					{
						cnt = Container.Cast( m_IconsContainer.GetParent() );
						cnt.SetPreviousActive();
					}
					return;
				}
			}
			else if( direction == Direction.DOWN )
			{
				m_FocusedItemPosition += ROWS_NUMBER_XBOX;
				if( m_FocusedItemPosition > m_IconsContainer.GetItemCount() - 1 )
				{
					if( focused_row < max_rows - 1 )
					{
						m_FocusedItemPosition = m_IconsContainer.GetItemCount() - 1;
					}
					else
					{
						cnt = Container.Cast( m_IconsContainer.GetParent().GetParent().GetParent() );
						if( cnt )
						{
							cnt.SetNextActive();
						}
						else
						{
							cnt = Container.Cast( m_IconsContainer.GetParent() );
							cnt.SetNextActive();
						}
						m_FocusedItemPosition = 0;
						return;
					}
				}
			}
			else if( direction == Direction.RIGHT )
			{
				m_FocusedItemPosition++;
				if( m_FocusedItemPosition > row_max  )
				{
					m_FocusedItemPosition = row_min;
				}
			}
			else if( direction == Direction.LEFT )
			{
				m_FocusedItemPosition--;
				if( m_FocusedItemPosition < row_min  )
				{
					m_FocusedItemPosition = row_max;
				}
			}
			
			icon = m_IconsContainer.GetIconByIndex( m_FocusedItemPosition );
			if( icon )
			{
				icon.SetActive( true );
			}
		}
	}
	
	void SetDefaultFocus( bool while_micromanagment_mode = false )
	{
		if( !while_micromanagment_mode && ItemManager.GetInstance().IsMicromanagmentMode() )
		{
			return;
		}
		Unfocus();
		if( m_IconsContainer.GetItemCount() )
		{
			Icon icon = m_IconsContainer.GetIconByIndex( 0 );
			icon.SetActive( true );
		}
	}

	void Unfocus()
	{
		Icon focused_item_old = GetFocusedItem();
		if(!focused_item_old)
		{
			return;
		}
		focused_item_old.SetActive( false );
		m_FocusedItemPosition = 0;
	}
	
	void SetDefaultFocusAfterInitIcon()
	{
		m_SetDefaultFocusAfterInitIcon = true;
	}
	
	void UpdateInterval()
	{
		if( m_Entity )
		{
			ref array<int> showed_items = new array<int>;
			
			//START - Add new item Icons
			for ( int j = 0; j < m_Entity.GetInventory().GetCargo().GetItemCount(); j++ )
			{
				EntityAI item = InitIcon( j );
				showed_items.Insert( item.GetID() );
			}
			
			m_IconsContainer.UpdateItemsTemperature();

			//END - Add new Icons
			#ifdef PLATFORM_CONSOLE
				if( m_RefreshItemPositions )
				{
						RecomputeGridHeight();
						if( GetParent() )
							GetParent().Refresh();
						m_IconsContainer.RecomputeItemPositions();
						if(Container.Cast( GetParent().GetParent().GetParent() ) )
							( Container.Cast( GetParent().GetParent().GetParent() ) ).UpdateBodySpacers();
						else
							HandsContainer.Cast( GetParent() ).RefreshHands();
					
					m_RefreshItemPositions = false;
				}
			#endif
			
			//START - Remove removed item Icons
			for ( int i = 0; i < m_ShowedItems.Count(); i++ )
			{
				int showed_item = m_ShowedItems.Get( i );
				if( showed_items.Find( showed_item ) == INDEX_NOT_FOUND )
				{
					int size_x, size_y;
					Icon iconn = m_IconsContainer.GetIcon( showed_item );
					if( iconn )
					{
						InventoryItem ent = InventoryItem.Cast( iconn.GetObject() );
						if( ent )
						{
							m_IconsContainer.RemoveItem( showed_item );
							ItemManager.GetInstance().HideTooltip();
							#ifdef PLATFORM_CONSOLE
								RecomputeGridHeight();
								GetParent().Refresh();
								m_IconsContainer.RecomputeItemPositions();
								
								if( !ItemManager.GetInstance().IsMicromanagmentMode() && m_ReactivateCursor && m_IconsContainer.GetItemCount() > 0 )
								{
									if( m_IconsContainer.GetItemCount() == m_FocusedItemPosition )
									{
										m_FocusedItemPosition--;
									}
									Icon icon = m_IconsContainer.GetIconByIndex( m_FocusedItemPosition );
									if( icon )
									{
										icon.SetActive( true );
										m_ReactivateCursor = false;
									}
									Inventory.Cast( GetParent().GetParent().GetParent().GetParent() ).UpdateConsoleToolbar();
								}
								
								Container cnt = Container.Cast( m_IconsContainer.GetParent().GetParent().GetParent() );
								if( cnt )
								{
									cnt.UpdateBodySpacers();
								}
								else
								{
									cnt = Container.Cast( m_IconsContainer.GetParent() );
									cnt.UpdateBodySpacers();
								}
							Inventory.Cast( GetParent().GetParent().GetParent().GetParent() ).UpdateConsoleToolbar();
							#endif
						}
						else
						{
							m_IconsContainer.RemoveItem( iconn );
							#ifdef PLATFORM_CONSOLE
							Inventory.Cast( GetParent().GetParent().GetParent().GetParent() ).UpdateConsoleToolbar();
								RecomputeGridHeight();
								if( GetParent() )
									GetParent().Refresh();
								m_IconsContainer.RecomputeItemPositions();
								if(Container.Cast( GetParent().GetParent().GetParent() ) )
									( Container.Cast( GetParent().GetParent().GetParent() ) ).UpdateBodySpacers();
								else
									HandsContainer.Cast( GetParent() ).RefreshHands();
							#endif
							delete iconn;
							
						}
					}
				}
			}
			
			//END - Remove removed item Icons
			TextWidget tw	= TextWidget.Cast( GetParent().GetMainWidget().FindAnyWidget("CargoCount") );
			
			int capacity;
			int occupied_cargo;
			#ifdef PLATFORM_WINDOWS
			CargoGrid grid	= CargoGrid.Cast( m_Entity.GetInventory().GetCargo() );
			capacity		= grid.GetWidth() * grid.GetHeight();
			for( int s = 0; s < grid.GetItemCount(); s++ )
			{
				int w, h;
				grid.GetItemSize( s, w, h );
				occupied_cargo += ( w*h );
			}
			#else
			#ifdef PLATFORM_CONSOLE
				capacity		= CargoList.Cast( m_Entity.GetInventory().GetCargo() ).GetMaxWeight();
				occupied_cargo	= CargoList.Cast( m_Entity.GetInventory().GetCargo() ).GetTotalWeight( null );
			#endif
			#endif
			
			if( tw )
				tw.SetText( occupied_cargo.ToString() + "/" + capacity.ToString() );
			m_ShowedItems = showed_items;
		}
	}

	EntityAI InitIcon( int index )
	{
		EntityAI item = m_Entity.GetInventory().GetCargo().GetItem( index );
		if( !m_IconsContainer.ContainsEntity( item ) )
		{
			int pos_x, pos_y, size_x, size_y;
			m_Entity.GetInventory().GetCargo().GetItemSize( index, size_x, size_y );
			m_Entity.GetInventory().GetCargo().GetItemRowCol( index, pos_y, pos_x );
			TextWidget tw		= TextWidget.Cast( GetParent().GetMainWidget().FindAnyWidget("CargoCount") );
			
			int capacity;
			int occupied_cargo;
			#ifdef PLATFORM_WINDOWS
			CargoGrid grid	= CargoGrid.Cast( m_Entity.GetInventory().GetCargo() );
			capacity		= grid.GetWidth() * grid.GetHeight();
			for( int i = 0; i < grid.GetItemCount(); i++ )
			{
				int w, h;
				grid.GetItemSize( i, w, h );
				occupied_cargo += ( w*h );
			}
			#else
			#ifdef PLATFORM_CONSOLE
			capacity		= CargoList.Cast( m_Entity.GetInventory().GetCargo() ).GetMaxWeight();
			occupied_cargo	= CargoList.Cast( m_Entity.GetInventory().GetCargo() ).GetTotalWeight( null );
			#endif
			#endif
			
			if(tw)
				tw.SetText( occupied_cargo.ToString() + "/" + capacity.ToString() );
			
			Icon icon = new Icon( m_IconsContainer );
			icon.Init( item );
			#ifdef PLATFORM_CONSOLE
				icon.SetSize( 2, 2 );
				icon.SetPos( (m_IconsContainer.GetItemCount() % ROWS_NUMBER_XBOX)*2, (m_IconsContainer.GetItemCount() / ROWS_NUMBER_XBOX)*2 );
				icon.m_posX = (m_IconsContainer.GetItemCount() % ROWS_NUMBER_XBOX)*2;
				icon.m_posY = (m_IconsContainer.GetItemCount() / ROWS_NUMBER_XBOX)*2;
				icon.SetCargoPos( index );
			#else
				icon.SetSize( size_x, size_y );
				icon.SetPos( pos_x, pos_y );
				icon.m_posX = pos_x;
				icon.m_posY = pos_y;
			#endif
			m_IconsContainer.AddItem( icon );
			
			#ifdef PLATFORM_CONSOLE
			m_RefreshItemPositions = true;
			#endif
			
			if( m_SetDefaultFocusAfterInitIcon )
			{
				SetDefaultFocus();
				m_SetDefaultFocusAfterInitIcon = false;	
			}
		}

		return item;
	}
}
