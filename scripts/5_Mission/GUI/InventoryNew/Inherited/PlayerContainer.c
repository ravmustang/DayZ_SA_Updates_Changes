class PlayerContainer: CollapsibleContainer
{
	protected ref Container m_PlayerAttachmentsContainer;
	protected ref map<int, ref Widget> m_InventorySlots;
	protected ref map<EntityAI, ref Container> m_ShowedItems = new ref map<EntityAI, ref Container>;
	protected ref map<int, ref Container> m_ShowedItemsIDs = new ref map<int, ref Container>;
	protected PlayerBase m_Player;

	void MouseClick( Widget w, int x, int y, int button )
	{
		bool inventory_locked = GetGame().GetPlayer().GetInventory().IsInventoryLocked();
		ItemManager.GetInstance().SetWidgetDraggable( w, !inventory_locked );
		
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		EntityAI item = item_preview.GetItem();
		InventoryItem itemAtPos = InventoryItem.Cast( item );
		
		if( item )
		{
			if ( button == MouseState.MIDDLE )
			{
				InspectItem( itemAtPos );
				return;
			}
		}
	}
	
	override void SetFirstActive()
	{
		super.SetFirstActive();
		
		EntityAI focused_item = GetFocusedItem();
		if( focused_item )
		{
			float x, y;
			Container cnt = Container.Cast( m_Body.Get( 1 ) );
			cnt.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ).GetScreenPos( x, y );
			ItemManager.GetInstance().PrepareTooltip( focused_item, x, y );
		}
	}
	
	bool IsItemWithContainerActive()
	{
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
		EntityAI ent = item_preview.GetItem();
		return ent && ( ent.GetInventory().GetCargo() || ent.GetSlotsCountCorrect() > 0 );
	}
	
	bool IsItemWithQuantityActive()
	{
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
		EntityAI ent = item_preview.GetItem();
		return ent && QuantityConversions.HasItemQuantity( ent );
	}
	
	bool IsItemActive()
	{
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
		EntityAI ent = item_preview.GetItem();
		return ent && !IsItemWithQuantityActive() && !IsItemWithContainerActive();
	}
	
	bool IsEmptyItemActive()
	{
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
		EntityAI ent = item_preview.GetItem();
		return ent == NULL;
	}
	
	void UnfocusPlayerAttachmentsContainer()
	{
		m_PlayerAttachmentsContainer.UnfocusAll();
	}
	
	void DoubleClick(Widget w, int x, int y, int button)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if( w == NULL || player.GetInventory().IsInventoryLocked() )
		{
			return;
		}
		ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !iw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !iw )
		{
		  iw = ItemPreviewWidget.Cast( w );
		}
		if( !iw.GetItem() )
		{
			return;
		}

		EntityAI item = iw.GetItem();

		if ( player.GetInventory().HasEntityInInventory( item ) && player.GetHumanInventory().CanAddEntityInHands( item ) )
		{
			player.PredictiveTakeEntityToHands( item );
		}
		else
		{
			if (player.GetInventory().CanAddEntityToInventory( item ) && item.GetInventory().CanRemoveEntity())
			{
				player.PredictiveTakeEntityToInventory( FindInventoryLocationType.ANY, item );
			}
			else
			{
				if( player.GetHumanInventory().CanAddEntityInHands( item ) )
				{
					player.PredictiveTakeEntityToHands( item );
				}
			}
		}
		ToggleWidget( w );
		ItemManager.GetInstance().HideTooltip();

		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		for ( int i = 0; i < m_ShowedItems.Count(); i++ )
		{
			Container container = m_ShowedItems.GetElement( i );
			container.RefreshItemPosition( item_to_refresh );
		}
	}
	
	void ExpandCollapseContainer()
	{
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
		ToggleWidget( item_preview.GetParent() );		
	}
	
	Container GetFocusedContainer()
	{
		return m_FocusedContainer;
	}
	
	bool IsItemWithCargoActive()
	{
		return m_FocusedContainer.IsInherited( ItemWithCargo );
	}
	
	bool IsItemWithAttachmentsActive()
	{
		return m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments );
	}
	
	override float GetFocusedContainerHeight()
	{
		float x, y;
		m_FocusedContainer.GetMainPanel().GetScreenSize( x, y );
		return y;
	}
	
	override float GetFocusedContainerYPos()
	{
		float x, y;
		m_FocusedContainer.GetMainPanel().GetPos( x, y );
		return y;
	}
	
	void TransferItemToVicinity()
	{
		if( m_FocusedContainer.IsInherited( ItemWithCargo ) || m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.TransferItemToVicinity();
			}
			else if ( iwca )
			{
				iwca.TransferItemToVicinity();
			}
		}
		else
		{
			Man player = GetGame().GetPlayer();
			ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
			EntityAI item = item_preview.GetItem();
			if( item && player.CanDropEntity( item ) )
			{
				player.PredictiveDropEntity( item );
			}
		}
	}
	
	int GetRecipeCount( bool recipe_anywhere, EntityAI entity1, EntityAI entity2 )
	{
		PluginRecipesManager plugin_recipes_manager = PluginRecipesManager.Cast( GetPlugin( PluginRecipesManager ) );
		return plugin_recipes_manager.GetValidRecipes( ItemBase.Cast( entity1 ), ItemBase.Cast( entity2 ), NULL, PlayerBase.Cast( GetGame().GetPlayer() ) );
	}
	
	bool CanCombine()
	{
		ItemBase ent =  ItemBase.Cast( GetFocusedItem() );
		ItemBase item_in_hands = ItemBase.Cast(	GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() );
		
		return GetRecipeCount( false, ent, item_in_hands ) > 0;
	}
	
	override void EquipItem()
	{
		if( m_FocusedContainer.IsInherited( ItemWithCargo ) || m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.EquipItem();
			}
			else if ( iwca )
			{
				//iwca.Combine();
			}
		}
		else
		{
			ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
			EntityAI item = item_preview.GetItem();
			if( item )
			{
				GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.ATTACHMENT, item );
			}
		}
	}
	
	override void TransferItem()
	{
		LeftArea left_area = LeftArea.Cast( GetParent() );
		if( left_area )
		{
			if( m_FocusedContainer.IsInherited( ItemWithCargo ) || m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) )
			{
				ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
				ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( m_FocusedContainer );
				if( iwc )
				{
					iwc.TransferItem();
				}
				else if ( iwca )
				{
					iwca.TransferItem();
				}
			}
			else
			{
				ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
				EntityAI item = item_preview.GetItem();
				if( item )
				{
					GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.CARGO, item );
				}
			}
		}
		else
		{
			if( !m_FocusedContainer.IsInherited( ItemWithCargo ) && !m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) )
			{
				item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
				item = item_preview.GetItem();
				if( item )
				{
					GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.CARGO, item );
				}
			}
		}
	}
	
	EntityAI GetFocusedItem()
	{
		if( m_FocusedContainer.IsInherited( ItemWithCargo ) || m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( m_FocusedContainer );
			if( iwc )
			{
				return iwc.GetFocusedItem();
			}
			else if ( iwca )
			{
				return iwca.GetFocusedItem();
			}
			return NULL;
		}
		else
		{
			ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
			return item_preview.GetItem();
		}
	}	
	
	override void Combine()
	{
		if( m_FocusedContainer && ( m_FocusedContainer.IsInherited( ItemWithCargo ) || m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.Combine();
			}
			else if ( iwca )
			{
				//iwca.Combine();
			}
		}
		else
		{
			ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
			ItemBase item = ItemBase.Cast( item_preview.GetItem() );
			ItemBase item_in_hands = ItemBase.Cast(	GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() );
			
			Icon hands_icon = ItemManager.GetInstance().GetHandsPreview().GetIcon();
						
			if( item_in_hands && item )
			{
				hands_icon.CombineItems( item_in_hands, item );
			}
		}
	}
	
	override void SelectItem()
	{
		if( m_FocusedContainer.IsInherited( ItemWithCargo ) || m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.SelectItem();
			}
			else if ( iwca )
			{
				iwca.SelectItem();
			}
		}
		else
		{
			ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
			ItemBase item = ItemBase.Cast( item_preview.GetItem() );
			ItemManager.GetInstance().SetSelectedItem( item, NULL, NULL );
		}
	}
	
	override void Select()
	{
		if( m_FocusedContainer.IsInherited( ItemWithCargo ) || m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.Select();
			}
			else if ( iwca )
			{
				iwca.Select();
			}
		}
		else
		{
			if( ItemManager.GetInstance().IsItemMoving() )
			{
				EntityAI selected_item = ItemManager.GetInstance().GetSelectedItem();
				GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.ATTACHMENT, selected_item );
				ItemManager.GetInstance().SetSelectedItem( NULL, NULL, NULL );
				m_PlayerAttachmentsContainer.Get( 0 ).GetMainPanel().FindAnyWidget( "Cursor" + 0 ).Show( true );
			}
			else
			{
				ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_PlayerAttachmentsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
				EntityAI item = item_preview.GetItem();
				if( item )
				{
					EntityAI item_in_hands = GetGame().GetPlayer().GetHumanInventory().GetEntityInHands();
					if( item_in_hands )
					{
						if( GameInventory.CanSwapEntities( item_in_hands, item ) )
						{
							GetGame().GetPlayer().PredictiveSwapEntities( item_in_hands, item );
						}
						else
						{
							GetGame().GetPlayer().PredictiveSwapEntities( item, item_in_hands );
						}
					}
					else
					{
						if( GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( item ) )
						{
							GetGame().GetPlayer().PredictiveTakeEntityToHands( item );
						}
					}
				}
				
			}
		}
	}

	override void MoveGridCursor( int direction )
	{
		if( !m_FocusedContainer || !m_FocusedContainer.IsActive() )
		{
			return;
		}
		if( m_FocusedContainer && ( m_FocusedContainer.IsInherited( ItemWithCargo ) || m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.MoveGridCursor(direction);
			}
			else if( iwca )
			{
				iwca.MoveGridCursor(direction);
			}
		}
		else
		{
			ItemManager.GetInstance().HideTooltip();
			m_FocusedContainer.UnfocusAll();
			
			if( direction == Direction.UP )
			{
				m_FocusedRow--;
				if( m_FocusedRow < 0 )
				{
					m_FocusedRow  = 1 ;
					RightArea right_area = RightArea.Cast( GetParent() );
					LeftArea left_area = LeftArea.Cast( GetParent() );
					if( right_area )
					{
						right_area.SetPreviousActive();
					}
					else
					{
						left_area.SetPreviousActive();
					}
					return;
				}				
			}
			else if( direction == Direction.DOWN )
			{
				m_FocusedRow++;
				if( m_FocusedRow == 2 )
				{
					m_FocusedRow = 0 ;
					right_area = RightArea.Cast( GetParent() );
					left_area = LeftArea.Cast( GetParent() );
					if( right_area )
					{
						right_area.SetNextActive();
					}
					else
					{
						left_area.SetNextActive();
					}
					return;
				}				
			}
			else if( direction == Direction.RIGHT )
			{
				m_FocusedColumn++;
				if( m_FocusedColumn == ITEMS_IN_ROW )
				{
					m_FocusedColumn = 0;
				}
			}
			else if( direction == Direction.LEFT )
			{
				m_FocusedColumn--;
				if( m_FocusedColumn < 0 )
				{
					m_FocusedColumn = ITEMS_IN_ROW - 1;
				}				
			}
			
			Container cnt = Container.Cast( m_Body.Get( 1 ) );
   		cnt.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ).Show( true );
			
			EntityAI focused_item = GetFocusedItem();
			if( focused_item )
			{
				float x, y;
				cnt.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ).GetScreenPos( x, y );
				ItemManager.GetInstance().PrepareTooltip( focused_item, x, y );
			}
		}
	}

	void PlayerContainer( ContainerBase parent )
	{
		m_InventorySlots = new ref map<int, ref Widget>;
		m_PlayerAttachmentsContainer = new Container( this );
		m_PlayerAttachmentsContainer.Insert( new IconsContainer( m_PlayerAttachmentsContainer ) );
		m_Body.Insert( m_PlayerAttachmentsContainer );

		//START - SetHeaderName
			Header h = Header.Cast( m_Body.Get( 0 ) );
			h.SetName( "EQUIPMENT" );
		//END - SetHeaderName

		//START - InitGhostSlots
			string config_path_ghosts_slots = "CfgVehicles SurvivorBase InventoryEquipment playerSlots";
			ref array<string> player_ghosts_slots = new array<string>;
			GetGame().ConfigGetTextArray( config_path_ghosts_slots, player_ghosts_slots );

			for ( int i = 0; i < player_ghosts_slots.Count(); i++ )
			{
				string slot_name = player_ghosts_slots.Get ( i );
				string path = "CfgSlots" + " " + slot_name;

				if ( GetGame().ConfigIsExisting( path ) )
				{
					string icon_name;
					GetGame().ConfigGetText( path + " ghostIcon", icon_name );
					int slot_number = i;
					int column = slot_number % ITEMS_IN_ROW;

					//START - GetWidgetSlot
						int row = slot_number / ITEMS_IN_ROW;
						if( row >= m_PlayerAttachmentsContainer.Count() )
							m_PlayerAttachmentsContainer.Insert( new IconsContainer( m_PlayerAttachmentsContainer ) );
						Widget item_preview = m_PlayerAttachmentsContainer.Get( row ).GetMainPanel().FindAnyWidget( "Render" + column );
						if( !item_preview )
						{
							item_preview = m_PlayerAttachmentsContainer.Get( row ).GetMainPanel().FindAnyWidget( "Icon" + column );
						}
						WidgetEventHandler.GetInstance().RegisterOnDrag( item_preview.GetParent(),  this, "OnIconDrag" );
						WidgetEventHandler.GetInstance().RegisterOnDrop( item_preview.GetParent(),  this, "OnIconDrop" );
						WidgetEventHandler.GetInstance().RegisterOnDoubleClick( item_preview.GetParent(),  this, "DoubleClick" );

						WidgetEventHandler.GetInstance().RegisterOnMouseEnter( item_preview.GetParent(),  this, "MouseEnter" );
						WidgetEventHandler.GetInstance().RegisterOnMouseLeave( item_preview.GetParent(),  this, "MouseLeave" );
					//END - GetWidgetSlot

					//START - LoadIconIntoWidgetSlot
						ImageWidget image_widget = ImageWidget.Cast( item_preview.FindAnyWidget( "GhostSlot" + column ) );
						if(!image_widget)
						image_widget = ImageWidget.Cast( item_preview.GetParent().GetParent().FindAnyWidget( "GhostSlot" + column ) );
						image_widget.Show( true );
						WidgetEventHandler.GetInstance().RegisterOnDropReceived( item_preview.GetParent().GetParent().FindAnyWidget( "PanelWidget" + column ),  this, "OnDropReceivedFromGhostArea" );
						WidgetEventHandler.GetInstance().RegisterOnDropReceived( item_preview.GetParent().GetParent().FindAnyWidget( "GhostSlot" + column ),  this, "OnDropReceivedFromGhostArea" );
						WidgetEventHandler.GetInstance().RegisterOnDraggingOver( item_preview.GetParent().GetParent().FindAnyWidget( "PanelWidget" + column ),  this, "DraggingOver" );
						WidgetEventHandler.GetInstance().RegisterOnDraggingOver( item_preview.GetParent().GetParent().FindAnyWidget( "GhostSlot" + column ),  this, "DraggingOver" );
						WidgetEventHandler.GetInstance().RegisterOnDraggingOver( item_preview.GetParent().GetParent().GetParent().FindAnyWidget( "Icon" + column ),  this, "DraggingOver" );
						WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( item_preview.GetParent(),  this, "MouseClick" );
						//WidgetEventHandler.GetInstance().RegisterOnDraggingOver( item_preview.GetParent().GetParent().FindAnyWidget( "Icon" + column ),  this, "DraggingOver" );
						image_widget.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
					//END - LoadIconIntoWidgetSlot
					
					#ifdef PLATFORM_CONSOLE
					TextWidget tw = TextWidget.Cast( item_preview.GetParent().GetParent().FindAnyWidget( "ItemSize" + column ) );
					tw.Show(true);
					#endif

					GetGame().ConfigGetText( path + " name", slot_name );
					int slot_id = InventorySlots.GetSlotIdFromString( slot_name );
					image_widget.SetUserID( slot_id );
					m_InventorySlots.Set( slot_id, item_preview );
				}
			}
			//LoadDefaultState();
		//END - InitGhostSlots
		RecomputeOpenedContainers();
	}

	/*void LoadDefaultState()
	{
		m_Hidden = !ItemManager.GetInstance().GetDefaultHeaderOpenState( "EQUIPMENT" );

		if( m_Hidden )
		{
			OnHide();
			Print(GetMainPanel().GetName());

		}
		else
		{
			OnShow();

		}

		//GetMainPanel().FindAnyWidget("opened").Show(!m_Hidden);
		//GetMainPanel().FindAnyWidget("closed").Show(m_Hidden);
	}*/

	bool MouseEnter( Widget w, int x, int y )
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		if( ipw.GetItem() )
		ItemManager.GetInstance().PrepareTooltip( ipw.GetItem() );
		return true;
	}

	bool MouseLeave( Widget w, Widget s, int x, int y	)
	{
		ItemManager.GetInstance().HideTooltip();
		return true;
	}

	void SetPlayer( PlayerBase player )
	{
		m_Player = player;
	}

	void OnIconDrag( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();
		ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );

		ItemManager.GetInstance().SetIsDragging( true );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget(name) );
		float icon_x, icon_y, x_content, y_content;
		int m_sizeX, m_sizeY;

		InventoryItem i_item = InventoryItem.Cast( ipw.GetItem() );
		GetGame().GetInventoryItemSize( i_item, m_sizeX, m_sizeY );

		m_Parent.m_Parent.GetMainPanel().FindAnyWidget( "body" ).GetScreenSize( x_content, y_content );
		icon_x = x_content / 10;
		icon_y = x_content / 10;
		w.SetFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( icon_x, icon_y );
		w.SetSize( icon_x * m_sizeX - 1 , icon_y * m_sizeY + m_sizeY - 1 );

		if( !ipw.GetItem() )
		{
			return;
		}
		name.Replace( "Render", "Col" );
		w.FindAnyWidget( name ).Show( true );
		name.Replace( "Col", "RadialIcon" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "RadialIcon", "AmmoIcon" );
		w.GetParent().GetParent().FindAnyWidget( name ).Show( false );
	}

	void OnIconDrop( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();

		ItemManager.GetInstance().SetIsDragging( false );
		w.ClearFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( 1, 1 );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Col" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "Col", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		ipw.Show( false );
		name.Replace( "Render", "GhostSlot" );
		w.GetParent().FindAnyWidget( name ).Show( true );
		name.Replace( "GhostSlot", "AmmoIcon" );
		w.GetParent().FindAnyWidget( name ).Show( false );
	}

	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( GetItemPreviewWidget( w ) );

		if( !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;
		}

		EntityAI item = ipw.GetItem();
		if( !item )
		{
			return;
		}

		//PlayerBase player = GetGame().GetPlayer();
		if( ( m_Player.GetInventory().CanAddEntityToInventory( item ) && !m_Player.GetInventory().HasEntityInInventory( item ) ) || m_Player.GetHumanInventory().HasEntityInHands( item ) )
		{
			m_Player.PredictiveTakeEntityToInventory( FindInventoryLocationType.ANY, item );
		}
	}

	ItemPreviewWidget GetItemPreviewWidget( Widget w )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget("Render") );
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

		return ipw;
	}

	override void DraggingOverHeader( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		ItemPreviewWidget ipw = GetItemPreviewWidget( w );

		if( !ipw || !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;
		}

		EntityAI item = ipw.GetItem();
		if( !item )
		{
			return;
		}

		if( ( m_Player.GetInventory().CanAddEntityToInventory( item ) && !m_Player.GetInventory().HasEntityInInventory( item ) ) || m_Player.GetHumanInventory().HasEntityInHands( item ) )
		{
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
		}
		else
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			ItemManager.GetInstance().ShowSourceDropzone( item );
		}
	}

	override void DraggingOver( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		EntityAI receiver_item;
		if( receiver_iw )
		{
			receiver_item = receiver_iw.GetItem();
		}

		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( GetItemPreviewWidget( w ) );

		if( !ipw || !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;
		}

		EntityAI item = ipw.GetItem();
		//PlayerBase player = GetGame().GetPlayer();
		if( !item )
		{
			return;
		}
		if( receiver_item )
		{
			if( GameInventory.CanSwapEntities( receiver_item, item ) )
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
				ItemManager.GetInstance().HideDropzones();
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				return;
			}
			else if( receiver_item.GetInventory().CanAddAttachment(item) || receiver_item.GetInventory().CanAddEntityInCargo( item ) && !receiver_item.GetInventory().HasEntityInCargo( item ))
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
				ItemManager.GetInstance().HideDropzones();
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				return;
			}
			else if( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item ) ) )
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.COMBINE_COLOR );
				ItemManager.GetInstance().HideDropzones();
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				return;
			}

		}

		if( m_Player.GetInventory().CanAddAttachment( item) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else if( ( m_Player.GetInventory().CanAddEntityToInventory( item ) && !m_Player.GetInventory().HasEntityInInventory( item ) ) || m_Player.GetHumanInventory().HasEntityInHands( item ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else if ( receiver_item && GameInventory.CanSwapEntities( receiver_item, item ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			ItemManager.GetInstance().ShowSourceDropzone( item );
		}
	}

	void OnDropReceivedFromGhostArea( Widget w, int x, int y, Widget receiver )
	{
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		EntityAI receiver_item;
		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		if( receiver_iw )
		{
			receiver_item = receiver_iw.GetItem();
		}


		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( GetItemPreviewWidget( w ) );

		if( !ipw )
		{
			return;
		}

		EntityAI item = ipw.GetItem();
		PlayerBase real_player = PlayerBase.Cast( GetGame().GetPlayer() );
		if( !item )
		{
			return;
		}
		if( receiver_item )
		{
			if( GameInventory.CanSwapEntities( receiver_item, item ) )
			{
				GetGame().GetPlayer().PredictiveSwapEntities( item, receiver_item );

				if ( menu )
				{
					menu.RefreshQuickbar();
				}
				return;
			}
			else if( receiver_item.GetInventory().CanAddAttachment( item ) )
			{
				GetGame().GetPlayer().PredictiveTakeEntityToTargetAttachment( receiver_item, item );

				if ( menu )
				{
					menu.RefreshQuickbar();
				}
				return;
			}
			else if( receiver_item.GetInventory().CanAddEntityInCargo( item ) && !receiver_item.GetInventory().HasEntityInCargo( item ) )
			{
				GetGame().GetPlayer().PredictiveTakeEntityToTargetInventory( receiver_item, FindInventoryLocationType.ANY, item );

				if ( menu )
				{
					menu.RefreshQuickbar();
				}
				return;
			}
			else if( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item ) ) )
			{
				( ItemBase.Cast( receiver_item ) ).CombineItemsClient( ItemBase.Cast( item ) );
			}
		}

		if( m_Player.GetInventory().CanAddAttachmentEx( item, receiver.GetUserID() ) )
		{
			real_player.PredictiveTakeEntityToTargetAttachmentEx( m_Player, item, receiver.GetUserID() );
		}
		else if(  m_Player.GetInventory().CanAddAttachment( item ) )
		{
			real_player.PredictiveTakeEntityToTargetAttachment( m_Player, item );
		}
		else if( ( m_Player.GetInventory().CanAddEntityToInventory( item ) && !m_Player.GetInventory().HasEntityInInventory( item ) ) || m_Player.GetHumanInventory().HasEntityInHands( item ) )
		{
			real_player.PredictiveTakeEntityToTargetInventory( m_Player, FindInventoryLocationType.ANY, item );
		}

		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}

	override void UpdateInterval()
	{
		if( !m_Player || !m_Parent.m_Parent.GetMainPanel().IsVisible() )
		{
			return;
		}
		int attachments_count = m_Player.GetInventory().AttachmentCount();
		ref map<EntityAI, ref Container> new_showed_items = new ref map<EntityAI, ref Container>;
		ref array<EntityAI> showed_player_ghost_entities = new ref array<EntityAI>;
		ref map<int, ref Container> showed_items_IDs = new ref map<int, ref Container>;
		for ( int i = 0; i < attachments_count; i++ )
		{
			EntityAI entity = m_Player.GetInventory().GetAttachmentFromIndex( i );
			if ( entity )
			{
				int slot_id = entity.GetInventory().GetSlotId(0);
				ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_InventorySlots.Get( slot_id ) );
				if( ipw && ipw.GetParent() )
				{
					string name2 = ipw.GetParent().GetName();
					ipw.Show( true );
					ipw.GetParent().Show( true );
					name2.Replace( "PanelWidget", "GhostSlot" );
					ipw.GetParent().GetParent().FindAnyWidget( name2 ).Show( false );
					ipw.SetItem( entity );
					showed_player_ghost_entities.Insert( entity );
					
					int size_x, size_y;
					GetGame().GetInventoryItemSize( InventoryItem.Cast( entity ), size_x, size_y );
					int capacity = size_x * size_y;
					name2.Replace( "GhostSlot", "ItemSize" );
					TextWidget tw = TextWidget.Cast( ipw.GetParent().GetParent().FindAnyWidget( name2 ) );
					tw.SetText( capacity.ToString() );
	
					name2.Replace( "ItemSize", "AmmoIcon" );
					Weapon_Base wpn;
					if ( Class.CastTo(wpn,  ipw.GetItem() ) )
					{
						int mi = wpn.GetCurrentMuzzle();
						ipw.GetParent().GetParent().FindAnyWidget( name2 ).Show( wpn.IsChamberFull( mi )  );
					}
	
					ClosableContainer conta = ClosableContainer.Cast( m_ShowedItemsIDs.Get( ipw.GetParent().GetUserID() ) );
					string config = "CfgVehicles " + entity.GetType() + " GUIInventoryAttachmentsProps";
					if( conta && conta.IsInherited( ClosableContainer ) )
					{
						bool show_radial_icon = conta.IsOpened() && ( entity.GetInventory().GetCargo() || entity.GetSlotsCountCorrect() > 0 ) && !GetGame().ConfigIsExisting( config );
						name2.Replace( "AmmoIcon", "RadialIcon" );
						ipw.FindAnyWidget( name2 ).Show( !show_radial_icon );
						name2.Replace( "RadialIcon", "RadialIconClosed" );
						ipw.FindAnyWidget( name2 ).Show( show_radial_icon );
					}
	
					if( m_ShowedItems.Contains( entity ) == false )
					{
						string name;
						if( entity.GetSlotsCountCorrect() > 0 )
						{
							ItemWithCargoAndAttachments iwca = new ItemWithCargoAndAttachments( this );
							iwca.SetEntity( entity );
							new_showed_items.Insert( entity, iwca );
							showed_items_IDs.Insert( entity.GetID(), iwca );
							ipw.GetParent().SetUserID( entity.GetID() );
							WidgetEventHandler.GetInstance().RegisterOnMouseButtonUp( ipw.GetParent(),  this, "ToggleWidget" );
	
							name = ipw.GetName();
							name.Replace( "Icon", "RadialIcon" );
							ipw.GetParent().FindAnyWidget( name ).Show( true );
						}
						else if( entity.GetInventory().GetCargo() )
						{
							ItemWithCargo iwc = new ItemWithCargo( this );
							iwc.SetEntity( entity );
							new_showed_items.Insert( entity, iwc );
							showed_items_IDs.Insert( entity.GetID(), iwc );
							ipw.GetParent().SetUserID( entity.GetID() );
							WidgetEventHandler.GetInstance().RegisterOnMouseButtonUp( ipw.GetParent(),  this, "ToggleWidget" );
	
							name = ipw.GetName();
							name.Replace( "Icon", "RadialIcon" );
							ipw.GetParent().FindAnyWidget( name ).Show(true);
						}
					}
					else
					{
						new_showed_items.Insert( entity, m_ShowedItems.Get( entity ) );
						showed_items_IDs.Insert( entity.GetID(), m_ShowedItemsIDs.Get( entity.GetID() ) );
					}
				}
			}
		}

		for ( i = 0; i < m_ShowedItems.Count(); i++)
		{
			EntityAI ent = m_ShowedItems.GetKey( i );
			m_ShowedItems.GetElement( i ).UpdateInterval();
			if( new_showed_items.Contains( ent ) == false )
			{
				Container con = m_ShowedItems.GetElement( i );
				( Container.Cast( con.m_Parent ) ).Remove( con );
				RecomputeOpenedContainers();
			}
		}

		for ( i = 0; i < m_InventorySlots.Count(); i++)
		{
			ipw = ItemPreviewWidget.Cast( m_InventorySlots.GetElement(i) );
			if( ipw.GetItem() != NULL && showed_player_ghost_entities.Find( ipw.GetItem() ) == INDEX_NOT_FOUND )
			{
				name2 = ipw.GetParent().GetName();
				ipw.Show( false );
				name2.Replace( "PanelWidget", "GhostSlot" );
				ipw.GetParent().GetParent().FindAnyWidget( name2 ).Show( true );
				ipw.SetItem( NULL );
				Inventory in = Inventory.Cast( GetRoot() );
				if( in )
					in.UpdateConsoleToolbar();
				name2.Replace( "GhostSlot", "ItemSize" );
				TextWidget tw1 = TextWidget.Cast( ipw.GetParent().GetParent().FindAnyWidget( name2 ) );
				tw1.SetText( "" );
			}

		}

		m_ShowedItems = new_showed_items;
		m_ShowedItemsIDs = showed_items_IDs;
	}

	void ToggleContainer( Widget w , int number )
	{
		ClosableContainer c = ClosableContainer.Cast( m_ShowedItemsIDs.Get( number ) );
		if( c == NULL )
		{
			return;
		}

		if( c.IsOpened() )
		{
			c.Close();
		}
		else
		{
			c.Open();
		}
	}

	void ToggleWidget( Widget w )
	{
		if( ItemManager.GetInstance().IsDragging() )
		{
			return;
		}
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		if( ipw.GetItem() )
		{
			ToggleContainer( w, w.GetUserID() );
			RecomputeOpenedContainers();
		}
	}

	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		for ( int i = 0; i < m_ShowedItems.Count(); i++)
		{
			m_ShowedItems.GetElement( i ).RefreshQuantity( item_to_refresh );
		}
	}

	override void CollapseButtonOnMouseButtonDown(Widget w)
	{
		super.CollapseButtonOnMouseButtonDown(w);
		//ItemManager.GetInstance().SetDefaultHeaderOpenState( "EQUIPMENT", !m_Hidden );
		m_RootSpacer.Update();
		if( m_Parent.IsInherited( RightArea ) )
		{
			( RightArea.Cast( m_Parent ) ).UpdateSpacer();
		}
		else
		{
			( LeftArea.Cast( m_Parent ) ).UpdateSpacer();
		}
	}
	
	bool IsPlayerEquipmentActive()
	{
		if( m_FocusedContainer.IsInherited( ItemWithCargo ) || m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments ) )
		{
			return false;
		}
		
		return true;
	}
}
