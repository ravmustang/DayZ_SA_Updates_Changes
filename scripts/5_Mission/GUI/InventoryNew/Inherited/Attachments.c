class Attachments
{
	Container m_Parent;
	EntityAI m_Entity;
	ref array<ref IconsContainer> m_Ics;
	int m_FocusedRow, m_FocusedColumn;

	void Attachments( Container parent, EntityAI entity )
	{
		m_Parent = parent;
		m_Entity = entity;
		m_Ics = new array<ref IconsContainer>();
	}
	
	bool IsEmpty()
	{
		ref array<string> attachments_slots = GetItemSlots( m_Entity );
		int count = attachments_slots.Count();
		return count == 0;
	}
	
	void SelectItem()
	{
		ItemBase item = ItemBase.Cast( GetFocusedEntity() );
		ItemManager.GetInstance().SetSelectedItem( item, NULL, m_Ics.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ) );
	}
	
	bool IsItemActive()
	{
		ItemBase item = ItemBase.Cast( GetFocusedEntity() );
		if( item == NULL )
		{
			return false;
		}
		return !IsEmpty() && ( !QuantityConversions.HasItemQuantity( item )  || ( QuantityConversions.HasItemQuantity( item ) && !item.CanBeSplit() ) );
	}
	
	bool IsItemWithQuantityActive()
	{
		ItemBase item = ItemBase.Cast( GetFocusedEntity() );
		if( item == NULL )
		{
			return false;
		}
		return !IsEmpty() && QuantityConversions.HasItemQuantity( item ) && item.CanBeSplit();
	}

	void UnfocusAll()
	{
		for ( int i = 0; i < m_Ics.Count(); i++ )
		{
			for ( int j = 0; j < 7; j++ )
			{
				m_Ics.Get( i ).GetMainPanel().FindAnyWidget( "Cursor" + j ).Show( false );
			}
		}
	}

	void SetDefaultFocus()
	{
		m_FocusedRow = 0;
		m_FocusedColumn = 0;
		if( ItemManager.GetInstance().IsMicromanagmentMode() )
		{
			return;
		}
		m_Ics.Get( 0 ).GetMainPanel().FindAnyWidget( "Cursor" + 0 ).Show( true );
		
		EntityAI focused_item = GetFocusedEntity();
		if( focused_item )
		{
			float x, y;
			m_Ics.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ).GetScreenPos( x, y );
			ItemManager.GetInstance().PrepareTooltip( focused_item, x, y );
		}
	}
	
	void SetActive( bool active )
	{
		float alpha;
		if( active )
		{			
			SetDefaultFocus();
			alpha = 1;
		}
		else
		{
			alpha = 0;
		}
		m_Parent.GetMainPanel().FindAnyWidget("AttachmentsWrapper").SetAlpha( alpha );
		m_Parent.GetMainPanel().FindAnyWidget("AttachmentsWrapper").Update();
	}
	
	EntityAI GetFocusedEntity()
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Ics.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
		return ipw.GetItem();
	}
	
	void Select()
	{
		EntityAI prev_item = EntityAI.Cast( GetFocusedEntity() );
		Man player = GetGame().GetPlayer();
		
		if( ItemManager.GetInstance().IsItemMoving() )
		{
			EntityAI selected_item = ItemManager.GetInstance().GetSelectedItem();
			if( selected_item /*&& GetEntity()*/ )
			{
				bool can_add = m_Entity.GetInventory().CanAddAttachment( selected_item );
				//bool in_cargo = !player.GetInventory().HasEntityInInventory( selected_item ) || !m_Entity.GetInventory().HasEntityInCargo( selected_item );
				if( can_add)
				{
					player.PredictiveTakeEntityToTargetAttachment(m_Entity, selected_item);
					ItemManager.GetInstance().SetSelectedItem( NULL, NULL, NULL );
				}
				else
				{
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
		ItemBase ent = ItemBase.Cast( GetFocusedEntity() );
		ItemBase item_in_hands = ItemBase.Cast(	GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() );
		
		return GetRecipeCount( false, ent, item_in_hands ) > 0;
	}
	
	bool CanEquip()
	{
		EntityAI entity = ItemBase.Cast( GetFocusedEntity() );
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
	
	void EquipItem()
	{
		ItemBase entity = ItemBase.Cast( GetFocusedEntity() );
		if( entity && !entity.IsInherited( Magazine ) )
		{
			if( entity.HasQuantity() )
			{
				entity.OnRightClick();
			}
			else
			{
				GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.ATTACHMENT, entity );
			}
		}
	}
	
	void TransferItem()
	{
		EntityAI entity = GetFocusedEntity();
		if( entity )
		{
			Weapon_Base wpn;
			Magazine mag;
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if( Class.CastTo(mag, entity) && Class.CastTo(wpn, mag.GetHierarchyParent()))
			{
				if (GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() == wpn)
				{
					InventoryLocation inv_loc_dst = new InventoryLocation;
					player.GetInventory().FindFreeLocationFor( entity, FindInventoryLocationType.CARGO, inv_loc_dst );
					player.GetWeaponManager().DetachMagazine(inv_loc_dst);
				}
			}
			else
			{
				GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.CARGO, entity );
			}
		}
	}
	
	void TransferItemToVicinity()
	{
		EntityAI item = GetFocusedEntity();
		Weapon_Base wpn;
		Magazine mag;
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if( Class.CastTo(mag, item) && Class.CastTo(wpn, mag.GetHierarchyParent()))
		{
			if (GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() == wpn)
			{
				InventoryLocation next = new InventoryLocation;
				vector m4[4];
				Math3D.MatrixIdentity4(m4);
				GameInventory.PrepareDropEntityPos(player, mag, m4);
				next.SetGround(mag, m4);

				player.GetWeaponManager().DetachMagazine(next);
			}
		}
		else if( item && player.CanDropEntity( item ) )
		{
			player.PredictiveDropEntity( item );
		}
	}
	
	bool IsActive()
	{
		return m_Parent.GetMainPanel().FindAnyWidget("AttachmentsWrapper").GetAlpha() > 0;
	}

	void MoveGridCursor( int direction )
	{
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
		m_Ics.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ).Show( true );
		
		EntityAI focused_item = GetFocusedEntity();
		if( focused_item )
		{
			float x, y;
			m_Ics.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ).GetScreenPos( x, y );
			ItemManager.GetInstance().PrepareTooltip( focused_item, x, y );
		}
	}

	void Remove()
	{
		for( int i = 0; i < m_Ics.Count(); i++ )
		{
			m_Parent.Remove( m_Ics.Get( i ) );
			delete m_Ics.Get( i ).GetMainPanel();
		}
		m_Ics.Clear();

		m_Parent.Refresh();
		m_Parent.UpdateSpacer();
	}

	void RefreshAtt( )
	{
		if( !m_Entity )
		{
			return;
		}

		ref array<string> attachments_slots = GetItemSlots( m_Entity );
		array<string> slots = attachments_slots;
		int count =  attachments_slots.Count();

		Att( count, slots );
	}

	void Att( int count, array<string> slots )
	{
		Widget ghost_widget;
		int slot_id;

		for ( int i = 0; i < count; i++ )
		{
			string slot_name = slots.Get ( i );
			string path = "CfgSlots" + " Slot_" + slot_name;
			string icon_name2;
			GetGame().ConfigGetText( path + " ghostIcon", icon_name2 );
			//icon_name2 = "missing";

			ImageWidget ghost_icon;
			if ( ghost_icon )
			{
				ghost_icon.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name2 );
			}

			GetGame().ConfigGetText( path + " name", slot_name );
			slot_id = InventorySlots.GetSlotIdFromString( slot_name );

			ItemPreviewWidget item_preview2 = ItemPreviewWidget.Cast( m_Parent.Get( ( i / 7 ) + 1 ).GetMainPanel().FindAnyWidget( "Icon" + i % 7 ).FindAnyWidget( "Render" + i % 7 ) );
			ImageWidget image_widget2 = ImageWidget.Cast( item_preview2.GetParent().GetParent().FindAnyWidget( "GhostSlot" + i % 7 ) );
			if( image_widget2 && m_Entity.GetInventory().FindAttachment( slot_id ) == NULL )
			{
				image_widget2.Show( true );
			}
			else
			{
				image_widget2.Show( false );
			}

			item_preview2.GetParent().Show( true );

			image_widget2.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name2 );
			item_preview2.SetItem( m_Entity.GetInventory().FindAttachment( slot_id ) );
			item_preview2.SetModelOrientation( Vector( 0, 0, 0 ) );
			item_preview2.SetView( m_Entity.GetViewIndex() );
			
			int size_x, size_y;
			GetGame().GetInventoryItemSize( InventoryItem.Cast( m_Entity.GetInventory().FindAttachment( slot_id ) ), size_x, size_y );
			int capacity = size_x * size_y;
			TextWidget tw = TextWidget.Cast( item_preview2.GetParent().GetParent().FindAnyWidget( "ItemSize" + i % 7 ) );
			
			#ifdef PLATFORM_CONSOLE
			tw.Show( true );
			#endif
			if(capacity == 0)
			{
				tw.SetText( "" );
			}
			else
			{
				tw.SetText( capacity.ToString() );
			}

			if( !ItemManager.GetInstance().IsDragging() )
			{
				ItemManager.GetInstance().SetTemperature( m_Entity.GetInventory().FindAttachment( slot_id ), item_preview2 );
			}

			Widget item_w = item_preview2;
			EntityAI item = m_Entity.GetInventory().FindAttachment( slot_id );
			if( item == NULL )
			{
				item_w.FindAnyWidget( "QuantityPanel" + i % 7 ).Show( false );
				item_w.GetParent().FindAnyWidget( "OutOfReach" + i % 7 ).Show( false );
			}

			item_w.FindAnyWidget( "Mounted" + i % 7 ).Show( false );

			if ( item_w && item )
			{
				if( m_Entity.GetInventory().GetSlotLock( slot_id ) && ItemManager.GetInstance().GetDraggedItem() != item )
				{
					item_w.FindAnyWidget( "Mounted" + i % 7 ).Show( true );
				}

				int has_quantity	= QuantityConversions.HasItemQuantity( item );
				int stack_max		= InventorySlots.GetStackMaxForSlotId( slot_id );
				Widget quantity_panel = item_w.FindAnyWidget( "QuantityPanel" + i % 7 );
				TextWidget item_quantity = TextWidget.Cast( item_w.FindAnyWidget( "Quantity" + i % 7 ) );
				ProgressBarWidget quantity_progress = ProgressBarWidget.Cast( item_w.FindAnyWidget( "QuantityBar" + i % 7 ) );
				Widget quantity_stack = item_w.FindAnyWidget( "QuantityStackPanel" + i % 7 );
				if ( has_quantity == QUANTITY_HIDDEN )
				{
					quantity_panel.Show( false );
				}
				else
				{
					quantity_panel.Show( true );
					if ( has_quantity == QUANTITY_COUNT || stack_max > 1 )
					{
						item_quantity.SetText( QuantityConversions.GetItemQuantityText( item ) );
						quantity_stack.Show( true );
						quantity_progress.Show( false );
					}
					else if ( has_quantity == QUANTITY_PROGRESS )
					{
						float progress_max = quantity_progress.GetMax();
						int max = item.ConfigGetInt( "varQuantityMax" );
						int count2 = item.ConfigGetInt( "count" );
						float quantity = QuantityConversions.GetItemQuantity( InventoryItem.Cast( item ) );
						if ( count2 > 0 )
						{
							max = count2;
						}
						if ( max > 0 )
						{

							float value = Math.Round( ( quantity / max ) * 100 );
							quantity_progress.SetCurrent( value );
						}
						quantity_stack.Show( false );
						quantity_progress.Show( true );
					}
				}
			}

			if( m_Entity.GetInventory().FindAttachment( slot_id ) != NULL )
			{
				ImageWidget image_widget3 = ImageWidget.Cast( item_preview2.GetParent().FindAnyWidget( "OutOfReach" + i % 7 ) );
				Weapon_Base wpn;
				PlayerBase p = PlayerBase.Cast( GetGame().GetPlayer() );
				if ( AttchmentsOutOfReach.IsAttachmentReachable( m_Entity, slot_name ) && ( !Class.CastTo(wpn,  m_Entity ) || p.GetHumanInventory().GetEntityInHands() == m_Entity ) )
				{
					image_widget3.Show( false );
				}
				else
				{
					image_widget3.Show( true );
				}
			}
		}
	}

	ref AttachmentsWrapper container;
	void InitAttachmentGrid( )
	{
		ref array<string> attachments_slots = GetItemSlots( m_Entity );

		array<string> slots = attachments_slots;
		int count = attachments_slots.Count();

		int number_of_rows = ( count / 7 );
		if( count % 7 == 0 )
		{
			number_of_rows--;
		}
		container = new AttachmentsWrapper( m_Parent );
		for ( int i = 0; i < number_of_rows + 1; i++ )
		{
			container.UpdateSpacer();
			IconsContainer ic = new IconsContainer( container );
			container.Insert( ic );

			if( i == number_of_rows && count % 7 != 0 )
			{
				ic.SetColumnCount( count % 7 );
			}
			else
			{
				ic.SetColumnCount( 7 );
			}

			m_Ics.Insert( ic );
			m_Parent.Insert( ic );
			for( int j = 0; j < 7; j++ )
			{
				WidgetEventHandler.GetInstance().RegisterOnDropReceived( ic.GetMainPanel().FindAnyWidget( "Icon" + j ), m_Parent, "OnDropReceivedFromHeader2" );
				WidgetEventHandler.GetInstance().RegisterOnDropReceived( ic.GetMainPanel().FindAnyWidget( "GhostSlot" + j ), m_Parent, "OnDropReceivedFromHeader2" );
				WidgetEventHandler.GetInstance().RegisterOnDropReceived( ic.GetMainPanel().FindAnyWidget( "PanelWidget" + j ), m_Parent, "OnDropReceivedFromHeader2" );

				WidgetEventHandler.GetInstance().RegisterOnDraggingOver( ic.GetMainPanel().FindAnyWidget( "Icon" + j ),  m_Parent, "DraggingOverHeader2" );
				WidgetEventHandler.GetInstance().RegisterOnDraggingOver( ic.GetMainPanel().FindAnyWidget( "GhostSlot" + j ), m_Parent, "DraggingOverHeader2" );
				WidgetEventHandler.GetInstance().RegisterOnDraggingOver( ic.GetMainPanel().FindAnyWidget( "PanelWidget" + j ), m_Parent, "DraggingOverHeader2" );
				
				WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( ic.GetMainPanel().FindAnyWidget( "Icon" + j ), m_Parent, "MouseClick2" );
				WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( ic.GetMainPanel().FindAnyWidget( "GhostSlot" + j ), m_Parent, "MouseClick2" );
				WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( ic.GetMainPanel().FindAnyWidget( "PanelWidget" + j ), m_Parent, "MouseClick2" );
			}
		}

		Att( count, slots );

		for ( i = 0; i < count; i++ )
		{
			ItemPreviewWidget item_preview2 = ItemPreviewWidget.Cast( m_Parent.Get( ( i / 7 ) + 1 ).GetMainPanel().FindAnyWidget( "Icon" + i % 7 ).FindAnyWidget( "Render" + i % 7 ) );
			//Print(item_preview2);
			//Print(item_preview2.GetParent());
			WidgetEventHandler.GetInstance().RegisterOnDrag( item_preview2.GetParent(), m_Parent, "OnIconDrag" );
			WidgetEventHandler.GetInstance().RegisterOnDrop( item_preview2.GetParent(), m_Parent, "OnIconDrop" );
			WidgetEventHandler.GetInstance().RegisterOnDoubleClick( item_preview2.GetParent(), m_Parent, "DoubleClick" );
		}
	}

	array<string> GetItemSlots( EntityAI e )
	{
		TStringArray searching_in = new TStringArray;
		searching_in.Insert( CFG_VEHICLESPATH );
		searching_in.Insert( CFG_WEAPONSPATH );
		searching_in.Insert( CFG_MAGAZINESPATH );

		array<string> attachments_slots	= new array<string>;

		for ( int s = 0; s < searching_in.Count(); ++s )
		{
			string cfg_name = searching_in.Get( s );
			string path = cfg_name + " " + e.GetType();

			if ( GetGame().ConfigIsExisting( path ) )
			{
				g_Game.ConfigGetTextArray( path + " attachments", attachments_slots );
				if ( e.IsWeapon() )
				{
					attachments_slots.Insert( "magazine" );
				}
				return attachments_slots;
			}
		}
		if ( e.IsWeapon() )
		{
			attachments_slots.Insert( "magazine" );
		}
		return attachments_slots;
	}
}
