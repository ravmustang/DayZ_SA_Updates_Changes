class VicinitySlotsContainer: Container
{
	protected ref Container m_Container;

	void VicinitySlotsContainer( LayoutHolder parent )
	{
		m_Container = new Container( this );
		m_Container.Insert( new SlotsContainer( m_Container ) );
		m_Body.Insert( m_Container );
		for( int j = 0; j < 7; j++ )
		{
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_Container.GetMainWidget().FindAnyWidget( "PanelWidget" + j ),  m_Parent, "OnDropReceivedFromIcon" );
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_Container.GetMainWidget().FindAnyWidget( "GhostSlot" + j ),  m_Parent, "OnDropReceivedFromHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_Container.GetMainWidget().FindAnyWidget( "Icon" + j ),  m_Parent, "OnDropReceivedFromHeader" );
			
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_Container.GetMainWidget().FindAnyWidget( "PanelWidget" + j ),  m_Parent, "DraggingOverIcon" );
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_Container.GetMainWidget().FindAnyWidget( "GhostSlot" + j ),  m_Parent, "DraggingOverHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_Container.GetMainWidget().FindAnyWidget( "Icon" + j ),  m_Parent, "DraggingOverHeader" );
			
			#ifdef PLATFORM_CONSOLE
			TextWidget tw = TextWidget.Cast( m_Container.GetMainWidget().FindAnyWidget( "ItemSize" + j) );
			tw.Show( true );
			#endif
		}		
	}
	
	EntityAI GetActiveItem()
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Container.Get( m_FocusedRow ).GetMainWidget().FindAnyWidget( "Icon" + m_FocusedColumn ).FindAnyWidget( "Render" + m_FocusedColumn ) );
		ItemManager.GetInstance().SetSelectedVicinityItem( ipw );
		return ipw.GetItem();
	}
	
	bool IsItemWithContainerActive()
	{
		EntityAI ent = GetActiveItem();
		return ent && ( ent.GetInventory().GetCargo() || ent.GetSlotsCountCorrect() > 0 );
	}
	
	bool IsItemWithQuantityActive()
	{
		EntityAI ent = GetActiveItem();
		return ent && QuantityConversions.HasItemQuantity( ent );
	}
	
	bool IsItemActive()
	{
		EntityAI ent = GetActiveItem();
		return ent && !IsItemWithQuantityActive() && !IsItemWithContainerActive();
	}
	
	bool IsEmptyItemActive()
	{
		EntityAI ent = GetActiveItem();
		return ent == NULL;
	}
	
	override void UnfocusAll()
	{
		for ( int i = 0; i < m_Container.m_Body.Count(); i++ )
		{
			for ( int j = 0; j < ITEMS_IN_ROW; j++ )
			{
				m_Container.Get( i ).GetMainWidget().FindAnyWidget( "Cursor" + j ).Show( false );
			}
		}
	}
	
	override void MoveGridCursor( int direction )
	{
		ItemManager.GetInstance().HideTooltip();
		UnfocusAll();

		if( direction == Direction.RIGHT )
		{
			m_FocusedColumn++;
			if( m_FocusedColumn == ITEMS_IN_ROW )
			m_FocusedColumn = 0;
		}
		else if( direction == Direction.LEFT )
		{
			m_FocusedColumn--;
			if( m_FocusedColumn < 0 )
			m_FocusedColumn = ITEMS_IN_ROW - 1;
		}
		else if( direction == Direction.UP )
		{
			m_FocusedRow--;
			if( m_FocusedRow < 0 )
			{
				m_FocusedRow = 0 ;
				LeftArea left_area = LeftArea.Cast( GetParent().GetParent() );
				left_area.SetPreviousActive();
				return;
			}				
		}
		else if( direction == Direction.DOWN )
		{
			m_FocusedRow++;
			if( m_FocusedRow == m_Container.m_Body.Count() )
			{
				m_FocusedRow = 0 ;
				left_area = LeftArea.Cast( GetParent().GetParent() );
				left_area.SetNextActive();
				return;
			}				
		}
		
		m_Container.Get( m_FocusedRow ).GetMainWidget().FindAnyWidget( "Cursor" + m_FocusedColumn ).Show( true );
		
		LayoutHolder cnt = LayoutHolder.Cast( m_Container.Get( m_FocusedRow ) );
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( cnt.GetMainWidget().FindAnyWidget( "Render" + m_FocusedColumn ) );
		EntityAI focused_item =  item_preview.GetItem();

		if( focused_item )
		{
			float x, y;
			cnt.GetMainWidget().FindAnyWidget( "Cursor" + m_FocusedColumn ).GetScreenPos( x, y );
			ItemManager.GetInstance().PrepareTooltip( focused_item, x, y );
		}
	}
	
	int GetRecipeCount( bool recipe_anywhere, EntityAI entity1, EntityAI entity2 )
	{
		PluginRecipesManager plugin_recipes_manager = PluginRecipesManager.Cast( GetPlugin( PluginRecipesManager ) );
		return plugin_recipes_manager.GetValidRecipes( ItemBase.Cast( entity1 ), ItemBase.Cast( entity2 ), NULL, PlayerBase.Cast( GetGame().GetPlayer() ) );
	}
	
	bool CanCombine()
	{
		ItemBase ent = ItemBase.Cast(  GetActiveItem() );
		ItemBase item_in_hands = ItemBase.Cast(	GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() );
		
		return GetRecipeCount( false, ent, item_in_hands ) > 0;
	}
	
	bool CanCombineAmmo()
	{
		PlayerBase m_player = PlayerBase.Cast( GetGame().GetPlayer() );
		ItemBase ent = ItemBase.Cast(  GetActiveItem() );
		ItemBase item_in_hands = ItemBase.Cast(	GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() );
		ActionManagerClient amc;
		Class.CastTo(amc, m_player.GetActionManager());

		return amc.CanPerformActionFromInventory( item_in_hands, ent );
	}
	
	bool CanEquip()
	{
		EntityAI ent = GetActiveItem();
		InventoryLocation il = new InventoryLocation;
		bool found = GetGame().GetPlayer().GetInventory().FindFreeLocationFor(ent,FindInventoryLocationType.ATTACHMENT,il);
		if (found)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	override void EquipItem( )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Container.Get( m_FocusedRow ).GetMainWidget().FindAnyWidget( "Icon" + m_FocusedColumn ).FindAnyWidget( "Render" + m_FocusedColumn ) );
		ItemManager.GetInstance().SetSelectedVicinityItem( ipw );
		EntityAI ent = ipw.GetItem();
		
		if( ent && !ent.IsInherited( Magazine ))
		{
			GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.ATTACHMENT, ent );
		}
	}
		
	override void TransferItem( )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Container.Get( m_FocusedRow ).GetMainWidget().FindAnyWidget( "Icon" + m_FocusedColumn ).FindAnyWidget( "Render" + m_FocusedColumn ) );
		ItemManager.GetInstance().SetSelectedVicinityItem( ipw );
		EntityAI ent = ipw.GetItem();
		if( ent )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.CARGO, ent );
		}
		else
		{
		}
	}
	
	override void Combine( )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Container.Get( m_FocusedRow ).GetMainWidget().FindAnyWidget( "Icon" + m_FocusedColumn ).FindAnyWidget( "Render" + m_FocusedColumn ) );
		ItemManager.GetInstance().SetSelectedVicinityItem( ipw );
		ItemBase ent = ItemBase.Cast( ipw.GetItem() );
		
		if( ent && !ent.IsInherited( Magazine ))
		{
			GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.ATTACHMENT, ent );
		}
		
		ItemBase item_in_hands = ItemBase.Cast(	GetGame().GetPlayer().GetHumanInventory().GetEntityInHands() );
		
		Icon hands_icon = ItemManager.GetInstance().GetHandsPreview().GetIcon();
		
		if( item_in_hands && ent && hands_icon )
		{
			hands_icon.CombineItems( item_in_hands, ent );
		}
	}
	
	override void SelectItem()
	{
		EntityAI ent = GetActiveItem();
		ItemManager.GetInstance().SetSelectedItem( ent, NULL, m_Container.Get( m_FocusedRow ).GetMainWidget().FindAnyWidget( "Cursor" + m_FocusedColumn ) );
	}
	
	override void Select()
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Container.Get( m_FocusedRow ).GetMainWidget().FindAnyWidget( "Icon" + m_FocusedColumn ).FindAnyWidget( "Render" + m_FocusedColumn ) );
		ItemManager.GetInstance().SetSelectedVicinityItem( ipw );
		EntityAI ent = ipw.GetItem();
		
		if( ItemManager.GetInstance().IsItemMoving() )
		{
			EntityAI selected_item = ItemManager.GetInstance().GetSelectedItem();
			if( selected_item && GetGame().GetPlayer().CanDropEntity( selected_item ) )
			{
				GetGame().GetPlayer().PredictiveDropEntity( selected_item );
				ItemManager.GetInstance().SetSelectedItem( NULL, NULL, NULL );
				GetMainWidget().FindAnyWidget( "Cursor" + 0 ).Show( true );
				return;
			}
		}
		
		if( ent )
		{
			EntityAI item_in_hands = GetGame().GetPlayer().GetHumanInventory().GetEntityInHands();
			if( item_in_hands )
			{
				if( GameInventory.CanSwapEntities( item_in_hands, ent ) )
				{
					GetGame().GetPlayer().PredictiveSwapEntities( item_in_hands, ent );
				}
			}
			else
			{
				if( GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( ent ) )
				{
					GetGame().GetPlayer().PredictiveTakeEntityToHands( ent );
				}
			}
		}
		else
		{
		}
	}
	
	void ~VicinitySlotsContainer()
	{
		for ( int i = 0; i < m_Container.Count(); i++ )
		{
			Container c = ClosableContainer.Cast( m_Container.Get( i / ITEMS_IN_ROW ) );
			if( c && c.GetMainWidget() )
			{
				delete m_Container.Get( i / ITEMS_IN_ROW ).GetMainWidget();
			}
		}
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.ClosableContainer;
	}

	override void OnShow()
	{
		super.OnShow();
	}
	
	void OnIconDrag( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();
		ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
		
		ItemManager.GetInstance().SetIsDragging( true );
		string name = w.GetName();
		float icon_x, icon_y, x_content, y_content;
		int m_sizeX, m_sizeY;
		m_Parent.m_Parent.m_Parent.GetMainWidget().FindAnyWidget( "HandsPanel" ).GetScreenSize( x_content, y_content );			
		name.Replace( "PanelWidget", "Render" );
		icon_x = x_content / 10;
		icon_y = x_content / 10;
		
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		InventoryItem i_item = InventoryItem.Cast( ipw.GetItem() );
		GetGame().GetInventoryItemSize( i_item, m_sizeX, m_sizeY );
		
		w.SetFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( icon_x, icon_y );
		w.SetSize( icon_x * m_sizeX - 1 , icon_y * m_sizeY + m_sizeY - 1 );
		name.Replace( "Render", "Col" );
		w.FindAnyWidget( name ).Show( true );
		name.Replace( "Col", "Temperature" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "Temperature", "RadialIcon" );
		w.FindAnyWidget( name ).Show( false );
	}
	
	void OnIconDrop( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();
		
		ItemManager.GetInstance().SetIsDragging( false );
		w.ClearFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( 1, 1 );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Col" );
		w.FindAnyWidget( name ).Show(false);
		/*name.Replace( "Col", "Render" );
		ItemPreviewWidget ipw = w.FindAnyWidget( name );
		ipw.SetItem( NULL );
		ipw.Show(false);
		Print(name);*/
	}
	
	Widget widgett;

	void ShowItemsInContainers( array<Object> items )
	{
		this.RecomputeNumberOfContainers( items );

		ItemPreviewWidget item_preview;
		int visible_items = 0;
		for( int x=0; x < items.Count(); x++ )
		{
			EntityAI item = EntityAI.Cast( items.Get( x ) );
			if( item.IsInherited(DayZAnimal) && item.IsAlive() )
			{
				continue;
			}
			else if( item.IsInherited( DayZInfected ) || item.IsInherited( SurvivorBase ) || item.IsInherited( Car ) || item.IsInherited( GardenBase ) || item.IsInherited(DayZAnimal) )
			{
				continue;
			}
			visible_items++;
			item_preview = ItemPreviewWidget.Cast( m_Container.Get( x / ITEMS_IN_ROW ).GetMainWidget().FindAnyWidget( "Icon" + x % ITEMS_IN_ROW ).FindAnyWidget( "Render" + x % ITEMS_IN_ROW ) );
			item_preview.Show( true );
			item_preview.GetParent().Show( true );
			item_preview.SetItem( item );
			item_preview.SetModelOrientation( Vector( 0, 0, 0 ) );
			item_preview.SetView( item.GetViewIndex() );
			item_preview.GetParent().SetUserID( item.GetID() );
			
			InventoryItem inventory_item = InventoryItem.Cast( item);
			if( inventory_item )
			{
				int size_x, size_y;
				GetGame().GetInventoryItemSize( inventory_item, size_x, size_y );
				int capacity = size_x * size_y;
				TextWidget tw = TextWidget.Cast( m_Container.Get( x / ITEMS_IN_ROW ).GetMainWidget().FindAnyWidget( "ItemSize" + x % ITEMS_IN_ROW ) );
				tw.SetText( capacity.ToString() );
			}
			
			if( !ItemManager.GetInstance().IsDragging() )
			{
				ItemManager.GetInstance().SetTemperature( item, item_preview );
			}
			
			WidgetEventHandler.GetInstance().RegisterOnDrag( item_preview.GetParent(),  this, "OnIconDrag" );
			WidgetEventHandler.GetInstance().RegisterOnDrop( item_preview.GetParent(),  this, "OnIconDrop" );
			WidgetEventHandler.GetInstance().RegisterOnDoubleClick( item_preview.GetParent(),  this, "DoubleClick" );
			WidgetEventHandler.GetInstance().RegisterOnMouseEnter( item_preview.GetParent(),  this, "MouseEnter" );
			WidgetEventHandler.GetInstance().RegisterOnMouseLeave( item_preview.GetParent(),  this, "MouseLeave" );
			WidgetEventHandler.GetInstance().RegisterOnMouseButtonUp( item_preview.GetParent(),  this, "MouseClick" );
			WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( item_preview.GetParent(),  this, "MouseButtonDown" );
			//WidgetEventHandler.GetInstance().RegisterOnController( item_preview.GetParent(),  this, "OnController" );
					//SetFocus( item_preview.GetParent() );
					//Print(item_preview.GetParent());
					widgett = item_preview.GetParent();

			ref map<int, ref Container> showed_items = ( VicinityContainer.Cast( m_Parent ) ).m_ShowedItemsIDs;
			ClosableContainer conta;
			int showed = showed_items.Count();
			conta = ClosableContainer.Cast( showed_items.Get( item.GetID() ) );
			string config = "CfgVehicles " + item.GetType() + " GUIInventoryAttachmentsProps";
			string name = item_preview.GetName();
			if(conta && conta.IsInherited( ClosableContainer ))
			{
				bool show_radial_icon = conta.IsOpened() && ( item.GetInventory().GetCargo() || item.GetSlotsCountCorrect() > 0 ) && !GetGame().ConfigIsExisting( config );
				name.Replace( "Render", "RadialIcon" );
				item_preview.FindAnyWidget( name ).Show( !show_radial_icon );
				name.Replace( "RadialIcon", "RadialIconClosed" );
				item_preview.FindAnyWidget( name ).Show( show_radial_icon );		
			}
			else
			{
				name.Replace( "Render", "RadialIcon" );
				item_preview.FindAnyWidget( name ).Show( false );
				name.Replace( "RadialIcon", "RadialIconClosed" );
				item_preview.FindAnyWidget( name ).Show( false );		
			}
			Widget item_w = item_preview;//.GetParent();
			if ( item_w )
			{
				int has_quantity = HasItemQuantity( item );
				Widget quantity_panel = item_w.FindAnyWidget( "QuantityPanel" + x % ITEMS_IN_ROW );
				TextWidget item_quantity = TextWidget.Cast( item_w.FindAnyWidget( "Quantity" + x % ITEMS_IN_ROW ) );
				ProgressBarWidget quantity_progress = ProgressBarWidget.Cast( item_w.FindAnyWidget( "QuantityBar" + x % ITEMS_IN_ROW ) );
				Widget quantity_stack = item_w.FindAnyWidget( "QuantityStackPanel" + x % ITEMS_IN_ROW );
				
				Weapon_Base wpn;
				if ( Class.CastTo(wpn,  item ) )
				{
					int mi = wpn.GetCurrentMuzzle();
					item_w.GetParent().GetParent().FindAnyWidget( "AmmoIcon" + x % ITEMS_IN_ROW ).Show( wpn.IsChamberFull( mi ) );
				}
				else
				{
					item_w.GetParent().GetParent().FindAnyWidget( "AmmoIcon" + x % ITEMS_IN_ROW ).Show( false );
				}
				
				if ( has_quantity == QUANTITY_HIDDEN )
				{
					quantity_panel.Show( false );
				}
				else
				{
					quantity_panel.Show( true );
					if ( has_quantity == QUANTITY_COUNT )
					{
						item_quantity.SetText( GetItemQuantityText( item ) );
						quantity_stack.Show( true );
						quantity_progress.Show( false );
					}
					else if ( has_quantity == QUANTITY_PROGRESS )
					{
						float progress_max = quantity_progress.GetMax();
						int max = item.ConfigGetInt( "varQuantityMax" );
						int count = item.ConfigGetInt( "count" );
						float quantity = GetItemQuantity( InventoryItem.Cast( item ) );
						
						if ( count > 0 )
						{
							max = count;
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
		}
		
		for(int c = visible_items; c < ITEMS_IN_ROW; c++)
		{
			int f = items.Count() / ITEMS_IN_ROW;
			item_preview = ItemPreviewWidget.Cast( m_Container.Get( f ).GetMainWidget().FindAnyWidget( "Icon" + c ).FindAnyWidget( "Render" + c ) );
			item_preview.Show(false);
			item_preview.SetItem( null );
			item_preview.GetParent().Show(false);
			TextWidget tw1 = TextWidget.Cast( m_Container.Get( x / ITEMS_IN_ROW ).GetMainWidget().FindAnyWidget( "ItemSize" + x % ITEMS_IN_ROW ) );
			tw1.SetText( "" );
		}
	}
	
	/*override bool OnController( Widget w, int control, int value )
	{
		super.OnController( w, control, value );

		//switch controls
		//m_UsingMouse = false;

		//B button
		if ( control == ControlID.CID_LEFT )
		{
			SetActive(false);
			return true;
		}
		return false;
	}*/
	
	/*void OnController()
	{
		Print("OnController");
	}*/
		
	
	void DoubleClick(Widget w, int x, int y, int button)
	{
		if( w == NULL )
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
		
		ItemBase item = ItemBase.Cast( iw.GetItem() );
		
		if( GetGame().GetPlayer().GetInventory().HasInventoryReservation( item, null ) )
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
				
		if ( player.GetInventory().HasEntityInInventory( item ) && GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( item ) )
		{
			player.PredictiveTakeEntityToHands( item );
		}
		else
		{
			if( item.GetInventory().CanRemoveEntity() )
			{
				InventoryLocation il = new InventoryLocation;
				if ( player.GetInventory().FindFreeLocationFor( item, FindInventoryLocationType.ANY | FindInventoryLocationType.NO_SLOT_AUTO_ASSIGN, il) )
				{
					if( item.ConfigGetFloat("varStackMax") )
						item.SplitIntoStackMaxClient( player, -1, );
					else
						player.PredictiveTakeEntityToInventory( FindInventoryLocationType.ANY | FindInventoryLocationType.NO_SLOT_AUTO_ASSIGN, InventoryItem.Cast( item ) );
				}
				else if( GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( item ) )
				{
					player.PredictiveTakeEntityToHands( item );
				}
			}
		}
		
		ItemManager.GetInstance().HideTooltip();
		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}
	
	bool MouseEnter( Widget w, int x, int y )
	{
		//SetFocus( widgett );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		if( ipw.GetItem() )
		{
			ItemManager.GetInstance().PrepareTooltip( ipw.GetItem() );
		}
		return true;
	}
	
	bool MouseLeave( Widget w, Widget s, int x, int y	)
	{
		ItemManager.GetInstance().HideTooltip();
		return true;
	}
	
	string GetItemQuantityText( EntityAI item )
	{
		string quantity_text = "";
		if ( item.IsInherited( InventoryItem ) )
		{
			ItemBase item_base = ItemBase.Cast( item );
			float quantity = item_base.GetQuantity();
			int ammo;
			if ( item.IsInherited( Magazine ) )
			{
				Magazine magazine_item = Magazine.Cast( item );
				ammo = magazine_item.GetAmmoCount();
				quantity_text = ammo.ToString();
				return quantity_text;
			}
			else if ( item.IsInherited( ItemBook ) )
			{
				return "";
			}
			int max = item.ConfigGetInt( "varQuantityMax" );
			string unit = item.ConfigGetString( "stackedUnit" );

			if ( max > 0 )
			{
				if ( max == 1 )
				{
					float tmp = Math.Round( ( quantity / max ) * 100 );
					quantity_text = tmp.ToString() + "%";
				}
				else
				{
					quantity_text = quantity.ToString();
				}
			}
		}
		return quantity_text;
	}
	
	int HasItemQuantity( EntityAI item )
	{
		if ( item.IsInherited( InventoryItem ) )
		{
			ItemBase item_base = ItemBase.Cast( item );
			if ( item.IsInherited( Magazine ) )
			{
				return QUANTITY_COUNT;
			}
			else if ( item.IsInherited( ItemBook ) )
			{
				return QUANTITY_HIDDEN;
			}
			int max = item.ConfigGetInt( "varQuantityMax" );
			bool bar = item.ConfigGetBool( "quantityBar" );
			if ( max > 0 )
			{
				if ( max == 1 || bar )
				{
					return QUANTITY_PROGRESS;
				}
				else
				{
					return QUANTITY_COUNT;
				}
			}
		}
		return QUANTITY_HIDDEN;
	}
	
	float GetItemQuantity( InventoryItem item )
	{
		float quantity = 0;
		if ( item.IsInherited( InventoryItem ) )
		{
			ItemBase item_base = ItemBase.Cast( item );
			if ( item.IsInherited( Magazine ) )
			{
				Magazine magazine_item = Magazine.Cast( item );
				quantity = magazine_item.GetAmmoCount();
			}
			else
			{
				quantity = item_base.GetQuantity();
			}
		}
		return quantity;
	}
	
	void MouseButtonDown( Widget w, int x, int y, int button)
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		EntityAI item = item_preview.GetItem();
		bool draggable = !GetGame().GetPlayer().GetInventory().HasInventoryReservation( item, NULL );
		draggable = draggable && item.CanPutIntoHands( GetGame().GetPlayer() );
		ItemManager.GetInstance().SetWidgetDraggable( w, draggable );
	}
	
	void MouseClick( Widget w, int x, int y, int button)
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		EntityAI item = item_preview.GetItem();
		InventoryItem itemAtPos = InventoryItem.Cast( item );
		ClosableContainer conta;
		
		if( m_Parent )
		{
			ref map<int, ref Container> showed_items = ( VicinityContainer.Cast( m_Parent ) ).m_ShowedItemsIDs;
		
			if( item && showed_items )
			{
				conta = ClosableContainer.Cast( showed_items.Get( item.GetID() ) );
				( VicinityContainer.Cast( m_Parent ) ).ToggleContainer( conta );
				int showed = showed_items.Count();
				string config = "CfgVehicles " + item.GetType() + " GUIInventoryAttachmentsProps";
				
				if ( button == MouseState.RIGHT )
				{
					if ( itemAtPos )
					{
						itemAtPos.OnRightClick();
					}
				}
				else if ( button == MouseState.MIDDLE )
				{
					InspectItem( itemAtPos );
					return;
				}
			}
		}
	}
	
	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
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
		
		if( !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;	
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		EntityAI item = ipw.GetItem();
		if( player.CanDropEntity( item ) )
		{
			player.PredictiveDropEntity( item );
		}
	}
	
	void RecomputeNumberOfContainers( array<Object> items )
	{
		int number_of_containers = m_Container.m_Body.Count();
		int number_of_containers_needed = ( items.Count() / ITEMS_IN_ROW ) + 1;
		int differnece = number_of_containers_needed - number_of_containers;

		if( differnece != 0 )
		{	
			if( differnece > 0 )
			{
				for( int g = 0; g < differnece; g++ )
				{
					LayoutHolder con = new SlotsContainer( m_Container );
					m_Container.Insert( con );
					//WidgetEventHandler.GetInstance().RegisterOnDropReceived( con.GetMainWidget().FindAnyWidget( "widget" ),  m_Parent, "OnDropReceivedFromHeader" );
					//WidgetEventHandler.GetInstance().RegisterOnDraggingOver( con.GetMainWidget().FindAnyWidget( "widget" ),  m_Parent, "OnDropReceivedFromHeader" );
					for( int j = 0; j < 7; j++ )
					{
						WidgetEventHandler.GetInstance().RegisterOnDropReceived( con.GetMainWidget().FindAnyWidget( "PanelWidget" + j ),  m_Parent, "OnDropReceivedFromIcon" );
						WidgetEventHandler.GetInstance().RegisterOnDropReceived( con.GetMainWidget().FindAnyWidget( "GhostSlot" + j ),  m_Parent, "OnDropReceivedFromHeader" );
						WidgetEventHandler.GetInstance().RegisterOnDropReceived( con.GetMainWidget().FindAnyWidget( "Icon" + j ),  m_Parent, "OnDropReceivedFromHeader" );
						
						WidgetEventHandler.GetInstance().RegisterOnDraggingOver( con.GetMainWidget().FindAnyWidget( "PanelWidget" + j ),  m_Parent, "DraggingOverIcon" );
						WidgetEventHandler.GetInstance().RegisterOnDraggingOver( con.GetMainWidget().FindAnyWidget( "GhostSlot" + j ),  m_Parent, "DraggingOverHeader" );
						WidgetEventHandler.GetInstance().RegisterOnDraggingOver( con.GetMainWidget().FindAnyWidget( "Icon" + j ),  m_Parent, "DraggingOverHeader" );
						
						#ifdef PLATFORM_CONSOLE
						TextWidget tw = TextWidget.Cast( con.GetMainWidget().FindAnyWidget( "ItemSize" + j) );
						tw.Show( true );
						#endif
					}
				}
			}
			else
			{
				for( int gx = 0; gx < ( differnece * -1 ); gx++ )
				{
					Widget w = m_Container.m_Body.Get( 0 ).GetMainWidget();
					m_Container.m_Body.Remove( 0 );
					delete w;
				}
			}
			if( m_Parent )
			{
				m_Parent.Refresh();
				if( m_Parent.m_Parent )
				{
					m_Parent.m_Parent.Refresh();
				}
			}
		}
	}
}
