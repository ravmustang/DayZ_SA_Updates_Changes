class VicinityIconsContainer: Container
{
	protected ref Container m_Container;

	void VicinityIconsContainer( ContainerBase parent )
	{
		m_Container = new Container( this );
		m_Container.Insert( new IconsContainer( m_Container ) );
		m_Body.Insert( m_Container );
		for( int j = 0; j < 7; j++ )
		{
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_Container.GetMainPanel().FindAnyWidget( "PanelWidget" + j ),  m_Parent, "OnDropReceivedFromIcon" );
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_Container.GetMainPanel().FindAnyWidget( "GhostSlot" + j ),  m_Parent, "OnDropReceivedFromHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_Container.GetMainPanel().FindAnyWidget( "Icon" + j ),  m_Parent, "OnDropReceivedFromHeader" );
			
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_Container.GetMainPanel().FindAnyWidget( "PanelWidget" + j ),  m_Parent, "DraggingOverIcon" );
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_Container.GetMainPanel().FindAnyWidget( "GhostSlot" + j ),  m_Parent, "DraggingOverHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_Container.GetMainPanel().FindAnyWidget( "Icon" + j ),  m_Parent, "DraggingOverHeader" );
			
			#ifdef PLATFORM_CONSOLE
			TextWidget tw = TextWidget.Cast( m_Container.GetMainPanel().FindAnyWidget( "ItemSize" + j) );
			tw.Show( true );
			#endif
		}		
	}
	
	EntityAI GetActiveItem()
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Container.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Icon" + m_FocusedColumn ).FindAnyWidget( "Render" + m_FocusedColumn ) );
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
				m_Container.Get( i ).GetMainPanel().FindAnyWidget( "Cursor" + j ).Show( false );
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
			Print( m_Container.m_Body.Count() );
			if( m_FocusedRow == m_Container.m_Body.Count() )
			{
				m_FocusedRow = 0 ;
				left_area = LeftArea.Cast( GetParent().GetParent() );
				left_area.SetNextActive();
				return;
			}				
		}
		
		Print( m_Container.Get( m_FocusedRow ) );
		m_Container.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ).Show( true );
		
		Container cnt = Container.Cast( m_Container.Get( m_FocusedRow ) );
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( cnt.GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
		EntityAI focused_item =  item_preview.GetItem();

		if( focused_item )
		{
			float x, y;
			cnt.GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ).GetScreenPos( x, y );
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
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Container.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Icon" + m_FocusedColumn ).FindAnyWidget( "Render" + m_FocusedColumn ) );
		ItemManager.GetInstance().SetSelectedVicinityItem( ipw );
		EntityAI ent = ipw.GetItem();
		
		if( ent && !ent.IsInherited( Magazine ))
		{
			GetGame().GetPlayer().PredictiveTakeEntityToInventory( FindInventoryLocationType.ATTACHMENT, ent );
		}
	}
		
	override void TransferItem( )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Container.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Icon" + m_FocusedColumn ).FindAnyWidget( "Render" + m_FocusedColumn ) );
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
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Container.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Icon" + m_FocusedColumn ).FindAnyWidget( "Render" + m_FocusedColumn ) );
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
		ItemManager.GetInstance().SetSelectedItem( ent, NULL, m_Container.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ) );
	}
	
	override void Select()
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( m_Container.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Icon" + m_FocusedColumn ).FindAnyWidget( "Render" + m_FocusedColumn ) );
		ItemManager.GetInstance().SetSelectedVicinityItem( ipw );
		EntityAI ent = ipw.GetItem();
		
		if( ItemManager.GetInstance().IsItemMoving() )
		{
			EntityAI selected_item = ItemManager.GetInstance().GetSelectedItem();
			if( selected_item && GetGame().GetPlayer().CanDropEntity( selected_item ) )
			{
				GetGame().GetPlayer().PredictiveDropEntity( selected_item );
				ItemManager.GetInstance().SetSelectedItem( NULL, NULL, NULL );
				GetMainPanel().FindAnyWidget( "Cursor" + 0 ).Show( true );
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
	
	void ~VicinityIconsContainer()
	{
		for ( int i = 0; i < m_Container.Count(); i++ )
		{
			Container c = ClosableContainer.Cast( m_Container.Get( i / ITEMS_IN_ROW ) );
			if( c && c.GetMainPanel() )
			{
				delete m_Container.Get( i / ITEMS_IN_ROW ).GetMainPanel();
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
		ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		
		ItemManager.GetInstance().SetIsDragging( true );
		string name = w.GetName();
		float icon_x, icon_y, x_content, y_content;
		int m_sizeX, m_sizeY;
		m_Parent.m_Parent.m_Parent.GetMainPanel().FindAnyWidget( "HandsPanel" ).GetScreenSize( x_content, y_content );			
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
		
		for( int x=0;x < items.Count(); x++ )
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
			item_preview = ItemPreviewWidget.Cast( m_Container.Get( x / ITEMS_IN_ROW ).GetMainPanel().FindAnyWidget( "Icon" + x % ITEMS_IN_ROW ).FindAnyWidget( "Render" + x % ITEMS_IN_ROW ) );
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
				TextWidget tw = TextWidget.Cast( m_Container.Get( x / ITEMS_IN_ROW ).GetMainPanel().FindAnyWidget( "ItemSize" + x % ITEMS_IN_ROW ) );
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
		
		for(int c = (items.Count() % ITEMS_IN_ROW); c < ITEMS_IN_ROW; c++)
		{
			item_preview = ItemPreviewWidget.Cast( m_Container.Get( items.Count() / ITEMS_IN_ROW ).GetMainPanel().FindAnyWidget( "Icon" + c ).FindAnyWidget( "Render" + c ) );
			item_preview.Show(false);
			item_preview.SetItem( NULL );
			item_preview.GetParent().Show(false);
			TextWidget tw1 = TextWidget.Cast( m_Container.Get( x / ITEMS_IN_ROW ).GetMainPanel().FindAnyWidget( "ItemSize" + x % ITEMS_IN_ROW ) );
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
		
		EntityAI item = iw.GetItem();
		
		if( GetGame().GetPlayer().GetInventory().HasInventoryReservation( item, NULL ) )
		{
			return;
		}

		InventoryManager manager = InventoryManager.GetInstance();
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
				
		if ( player.GetInventory().HasEntityInInventory( item ) && GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( item ) )
		{
			player.PredictiveTakeEntityToHands( item );
		}
		else
		{
			bool found = false;
			if( item.GetInventory().CanRemoveEntity())
			{
				InventoryLocation il = new InventoryLocation;
				found = player.GetInventory().FindFreeLocationFor(item,FindInventoryLocationType.ATTACHMENT,il);
				if (found)
				{
					if ( il.GetParent() != player )
					{
						found = player.GetInventory().FindFreeLocationFor(item,FindInventoryLocationType.ANY_CARGO,il);
					}
				}
				else
				{
					found = player.GetInventory().FindFreeLocationFor(item,FindInventoryLocationType.ANY,il);
				}
			}
			if ( found )
			{
				//player.PredictiveTakeToDst(true,item,il);
				player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, item);
			}
			else
			{
				if( GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( item ) )
				{
					player.PredictiveTakeEntityToHands( item );
				}
			}
		}
		
		ItemManager.GetInstance().HideTooltip();
		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
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
		ItemManager.GetInstance().SetWidgetDraggable( w, draggable );
	}
	
	void MouseClick( Widget w, int x, int y, int button)
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		EntityAI item = item_preview.GetItem();
		InventoryItem itemAtPos = InventoryItem.Cast( item );
		
		if( item )
		{
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
		ref map<int, ref Container> showed_items = ( VicinityContainer.Cast( m_Parent ) ).m_ShowedItemsIDs;
		ClosableContainer conta;
		int showed = showed_items.Count();
		conta = ClosableContainer.Cast( showed_items.Get( item.GetID() ) );
		string config = "CfgVehicles " + item.GetType() + " GUIInventoryAttachmentsProps";
		
		( VicinityContainer.Cast( m_Parent ) ).ToggleContainer( conta );
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
		Magazine mag;
		Weapon_Base wpn;
		bool handled_by_controller = false;
		EntityAI item = ipw.GetItem();
		if ( Class.CastTo( mag, item ) && Class.CastTo( wpn, mag.GetHierarchyParent() ) )
		{
			if (player.GetHumanInventory().GetEntityInHands() == wpn )
			{
				vector m4[4];
				Math3D.MatrixIdentity4(m4);
				GameInventory.PrepareDropEntityPos(player, mag, m4);
				InventoryLocation il_mag_next = new InventoryLocation;
				il_mag_next.SetGround(mag, m4);
				player.GetWeaponManager().DetachMagazine(il_mag_next);
				handled_by_controller = true;
			}
		} 
		if( !handled_by_controller && player.CanDropEntity( item ) )
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
					ContainerBase con = new IconsContainer( m_Container );
					m_Container.Insert( con );
					//WidgetEventHandler.GetInstance().RegisterOnDropReceived( con.GetMainPanel().FindAnyWidget( "widget" ),  m_Parent, "OnDropReceivedFromHeader" );
					//WidgetEventHandler.GetInstance().RegisterOnDraggingOver( con.GetMainPanel().FindAnyWidget( "widget" ),  m_Parent, "OnDropReceivedFromHeader" );

					for( int j = 0; j < 7; j++ )
					{
						WidgetEventHandler.GetInstance().RegisterOnDropReceived( con.GetMainPanel().FindAnyWidget( "PanelWidget" + j ),  m_Parent, "OnDropReceivedFromIcon" );
						WidgetEventHandler.GetInstance().RegisterOnDropReceived( con.GetMainPanel().FindAnyWidget( "GhostSlot" + j ),  m_Parent, "OnDropReceivedFromHeader" );
						WidgetEventHandler.GetInstance().RegisterOnDropReceived( con.GetMainPanel().FindAnyWidget( "Icon" + j ),  m_Parent, "OnDropReceivedFromHeader" );
						
						WidgetEventHandler.GetInstance().RegisterOnDraggingOver( con.GetMainPanel().FindAnyWidget( "PanelWidget" + j ),  m_Parent, "DraggingOverIcon" );
						WidgetEventHandler.GetInstance().RegisterOnDraggingOver( con.GetMainPanel().FindAnyWidget( "GhostSlot" + j ),  m_Parent, "DraggingOverHeader" );
						WidgetEventHandler.GetInstance().RegisterOnDraggingOver( con.GetMainPanel().FindAnyWidget( "Icon" + j ),  m_Parent, "DraggingOverHeader" );
						
						#ifdef PLATFORM_CONSOLE
						TextWidget tw = TextWidget.Cast( con.GetMainPanel().FindAnyWidget( "ItemSize" + j) );
						tw.Show( true );
						#endif
					}
				}
			}
			else
			{
				for( int gx = 0; gx < ( differnece * -1 ); gx++ )
				{
					Widget w = m_Container.m_Body.Get( 0 ).GetMainPanel();
					m_Container.m_Body.Remove( 0 );
					delete w;
				}
			}
			m_Parent.Refresh();
			m_Parent.m_Parent.Refresh();
		}
	}
}
