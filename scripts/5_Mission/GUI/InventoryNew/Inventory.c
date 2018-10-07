enum Direction
{
	RIGHT,
	LEFT,
	UP,
	DOWN
}

enum ConsoleToolbarType
{
	PLAYER_EQUIPMENT_SLOTS_ITEM,
	PLAYER_EQUIPMENT_SLOTS_ITEM_WITH_CARGO,
	PLAYER_EQUIPMENT_SLOTS_ITEM_WITH_ATTACHMENTS,
	PLAYER_EQUIPMENT_SLOTS_ITEM_FREE,
	
	PLAYER_CARGO_CONTAINER_EMPTY_CONTAINER,
	PLAYER_CARGO_CONTAINER_ITEM,
	PLAYER_CARGO_CONTAINER_ITEM_NO_EQUIP,
	PLAYER_CARGO_CONTAINER_ITEM_WITH_QUANTITY,
	PLAYER_CARGO_CONTAINER_ITEM_WITH_ATTACHMENTS,
	PLAYER_CARGO_CONTAINER_ITEM_WITH_ATTACHMENTS_NO_EQUIP,
	
	PLAYER_ITEM_WITH_ATTACHMENTS_CONTAINER_ITEM,
	
	HANDS_ITEM,
	HANDS_ITEM_NO_EQUIP,
	HANDS_ITEM_WITH_QUANTITY,
	HANDS_ITEM_EMPTY,
	
	VICINITY_CONTAINER_LIST_ITEM_WITH_CONTAINER,
	VICINITY_CONTAINER_LIST_ITEM_WITH_CONTAINER_NO_EQUIP,
	VICINITY_CONTAINER_LIST_ITEM,
	VICINITY_CONTAINER_LIST_ITEM_NO_EQUIP,
	VICINITY_CONTAINER_LIST_ITEM_WITH_QUANTITY,
	VICINITY_CONTAINER_LIST_EMPTY_ITEM,
	
	VICINITY_CONTAINER_DETAILS_EMPTY,
	VICINITY_CONTAINER_DETAILS_ITEM,
	VICINITY_CONTAINER_DETAILS_ITEM_NO_EQUIP,
	VICINITY_CONTAINER_DETAILS_ITEM_WITH_QUANTITY
}

class Inventory: ContainerBase
{
	protected ref LeftArea			m_LeftArea;
	protected ref RightArea			m_RightArea;
	protected ref HandsArea			m_HandsArea;
	protected ref PlayerPreview		m_PlayerPreview;
	protected ref InventoryQuickbar	m_Quickbar;
	
	protected Widget				m_QuickbarWidget;
	protected Widget				m_ConsoleToolbar;

	protected ref ContextMenu m_context_menu;
	
	void Inventory( ContainerBase parent )
	{
		m_ControllerRightStickTimer = new Timer();
		new ItemManager( GetMainPanel() );
		new ColorManager();
		//Deserialize();
		m_LeftArea = new LeftArea( this );
		m_RightArea = new RightArea( this );
		m_HandsArea = new HandsArea( this );
		m_PlayerPreview = new PlayerPreview( this );

#ifdef PLATFORM_CONSOLE
#else
		m_QuickbarWidget = GetMainPanel().FindAnyWidget( "QuickbarGrid" );
		m_Quickbar = new InventoryQuickbar( m_QuickbarWidget );
		m_Quickbar.UpdateItems( m_QuickbarWidget );
#endif			
		
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "LeftBackground" ),  this, "OnLeftPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "LeftBackground" ),  this, "DraggingOverLeftPanel" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "RightBackground" ),  this, "OnRightPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "RightBackground" ),  this, "DraggingOverRightPanel" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "CharacterPanel" ),  this, "OnCenterPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "CharacterPanel" ),  this, "DraggingOverCenterPanel" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "HandsPanel" ),  this, "OnHandsPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "HandsPanel" ),  this, "DraggingOverHandsPanel" );
		WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainPanel().FindAnyWidget( "InventoryWindow" ),  this, "OnLeftPanelDropReceived" );
		WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainPanel().FindAnyWidget( "InventoryWindow" ),  this, "DraggingOverLeftPanel" );
		
		#ifdef PLATFORM_CONSOLE
		
				PluginDiagMenu plugin_diag_menu = PluginDiagMenu.Cast( GetPlugin(PluginDiagMenu) );
				GetGame().GetUIManager().ShowUICursor( false );
				ResetFocusedContainers();
				GetMainPanel().FindAnyWidget( "CursorCharacter" ).Show( false );
		
				//console inventory toolbar
				m_ConsoleToolbar = GetMainPanel().FindAnyWidget( "ConsoleToolbar" );
				m_ConsoleToolbar.Show( true );
				UpdateConsoleToolbar();
		#endif
	}

	protected int GetProperControllerStickAngle( int angle )
	{
		int proper_angle = ( 360 - angle ) % 360;	//rotation correction

		return proper_angle;
	}

	protected int AngleToDirection( int angle )
	{
		if( angle < 45 || angle > 315)
		{
			return Direction.RIGHT;
		}
		else if ( angle < 135 && angle > 45 )
		{
			return Direction.DOWN;
		}
		else if ( angle < 225 && angle > 135 )
		{
			return Direction.LEFT;
		}
		else if ( angle < 315 && angle > 225 )
		{
			return Direction.UP;
		}
		return -1;
	}

	void TimerEnd()
	{
		m_ControllerRightStickTimerEnd = true;
		m_ControllerRightStickTimer.Stop();
	}
	int once;
	protected int m_ControllerAngle;
	protected int m_ControllerTilt;
	protected bool m_ControllerRightStickTimerEnd = true;
	ref Timer m_ControllerRightStickTimer;


	bool Controller( Widget w, int control, int value )
	{
		//Print("control: "+ control +" value: "+value);
		
		//Right stick
		if ( control == ControlID.CID_RADIALMENU )
		{
			m_ControllerAngle 	= value >> 4; 	// <0,360>
			m_ControllerTilt	= value & 0xF; 	// <0,10>

			m_ControllerAngle = GetProperControllerStickAngle(m_ControllerAngle);

			m_ControllerAngle = AngleToDirection(m_ControllerAngle);


			if(m_ControllerTilt>5)
			{
				if( m_ControllerRightStickTimerEnd )
				{
					m_RightArea.MoveGridCursor(m_ControllerAngle);
					m_LeftArea.MoveGridCursor(m_ControllerAngle);
					m_ControllerRightStickTimerEnd = false;
					m_ControllerRightStickTimer.Run( 0.1, this, "TimerEnd" );
				}
			}
			return true;
		}
		
		if ( control == ControlID.CID_RIGHT && value == 1 )
		{
			m_RightArea.MoveGridCursor(Direction.RIGHT);
			m_LeftArea.MoveGridCursor(Direction.RIGHT);
			m_HandsArea.MoveGridCursor(Direction.RIGHT);
		}
		else if ( control == ControlID.CID_LEFT && value == 1 )
		{
			m_RightArea.MoveGridCursor(Direction.LEFT);
			m_LeftArea.MoveGridCursor(Direction.LEFT);
			m_HandsArea.MoveGridCursor(Direction.LEFT);
		}
		else if ( control == ControlID.CID_UP && value == 1 )
		{
			m_RightArea.MoveGridCursor(Direction.UP);
			m_LeftArea.MoveGridCursor(Direction.UP);
			m_HandsArea.MoveGridCursor(Direction.UP);
		}
		else if ( control == ControlID.CID_DOWN && value == 1 )
		{
			m_RightArea.MoveGridCursor(Direction.DOWN);
			m_LeftArea.MoveGridCursor(Direction.DOWN);
			m_HandsArea.MoveGridCursor(Direction.DOWN);
		}
		
		UpdateConsoleToolbar();
		
		once++;
		if( control == ControlID.CID_SELECT && value != 0 )
		{
			if( m_RightArea.IsActive() )
			{
				m_RightArea.Select();
			}
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.Select();
			}
			if( m_HandsArea.IsActive() )
			{
				m_HandsArea.Select();
			}
			ItemManager.GetInstance().SetItemMicromanagmentMode( false );
			ItemManager.GetInstance().SetItemMoving( false );
			UpdateConsoleToolbar();
			return true;
		}

		if( control == ControlID.CID_BACK && value != 0 )
		{
			if( GetMainPanel().IsVisible() )
			{
				if( m_RightArea.IsActive() )
				{
					m_RightArea.Combine();
					m_RightArea.UnfocusGrid();

				}
				if( m_LeftArea.IsActive() )
				{
					m_LeftArea.Combine();
					m_LeftArea.UnfocusGrid();
				}
			}
			return true;
		}

		return false;
	}

	void Serialize()
	{
		//m_LeftArea.GetVicinityContainer().TraverseShowedItems();
		ItemManager.GetInstance().SerializeDefaultOpenStates();
		ItemManager.GetInstance().SerializeDefaultHeaderOpenStates();
		//ItemManager.GetInstance().SetDefaultOpenStateHands( true );
	}

	void Deserialize()
	{
		ItemManager.GetInstance().DeserializeDefaultOpenStates();
		ItemManager.GetInstance().DeserializeDefaultHeaderOpenStates();
	}

	void ~Inventory()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateInterval);
	}

	void RefreshQuantity( EntityAI item_to_refresh )
	{
		m_RightArea.RefreshQuantity( item_to_refresh );
		m_HandsArea.RefreshQuantity( item_to_refresh );
		m_LeftArea.RefreshQuantity( item_to_refresh );
	}
	
	void RefreshItemPosition( EntityAI item_to_refresh )
	{
		m_LeftArea.RefreshItemPosition( item_to_refresh );
		m_RightArea.RefreshItemPosition( item_to_refresh );
	}

	void DraggingOverHandsPanel( Widget w, int x, int y, Widget receiver )
	{
		ItemManager.GetInstance().HideDropzones();
		//GetMainPanel().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		m_HandsArea.DraggingOverHandsPanel( w, x, y, receiver );
	}
	
	void OnHandsPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		m_HandsArea.OnHandsPanelDropReceived( w, x, y, receiver );
	}

	void OnLeftPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		m_LeftArea.OnLeftPanelDropReceived( w, x, y, receiver );
	}

	void OnRightPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		if( w )
		{
			ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
			if( !ipw )
			{
				string name = w.GetName();
				name.Replace( "PanelWidget", "Render" );
				ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				if( !ipw )
				{
					ipw = ItemPreviewWidget.Cast( w );
					if( !ipw )
					{
						return;
					}
				}
			}
	
			EntityAI item = ipw.GetItem();
			if( item )
			{
				PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
				if( player && ( player.GetInventory().CanAddEntityToInventory( item ) ) || player.GetHumanInventory().HasEntityInHands( item ) )
				{
					player.PredictiveTakeEntityToInventory( FindInventoryLocationType.CARGO | FindInventoryLocationType.ATTACHMENT, item );
				}
			}
		}
	}
	
	void OnCenterPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		if( w )
		{
			ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
			if( !ipw )
			{
				string name = w.GetName();
				name.Replace( "PanelWidget", "Render" );
				ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				if( !ipw )
				{
					ipw = ItemPreviewWidget.Cast( w );
					if( !ipw )
					{
						return;
					}
				}
			}
	
			EntityAI item = ipw.GetItem();
			if( item )
			{
				PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
				if( player && ( player.GetInventory().CanAddAttachment( item ) ) || player.GetHumanInventory().HasEntityInHands( item ) )
				{
					player.PredictiveTakeEntityAsAttachment( item );
				}
				else
				{
					int slot_id = item.GetInventory().GetSlotId(0);
					EntityAI slot_item = player.GetInventory().FindAttachment( slot_id );
					if( slot_item && player.GetInventory().CanSwapEntities( item, slot_item ) )
					{
						player.PredictiveSwapEntities(item, slot_item);
					}
				}
			}
		}
	}

	void DraggingOverLeftPanel( Widget w, int x, int y, Widget receiver )
	{
		if( w )
		{
			ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
			if( !ipw )
			{
				string name = w.GetName();
				name.Replace( "PanelWidget", "Render" );
				ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				if( !ipw )
				{
					ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Preview" ) );
					if( !ipw )
					{
						ipw = ItemPreviewWidget.Cast( w );
						if( !ipw || !ipw.GetItem() )
						{
							return;
						}
					}
				}
			}
			
			ItemBase item = ItemBase.Cast( ipw.GetItem() );
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			
			if( player && item && player.CanDropEntity( item ) )
			{
				ItemManager.GetInstance().HideDropzones();
				GetMainPanel().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			}
			else if( item )
			{
				ItemManager.GetInstance().ShowSourceDropzone( item );
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			}
		}
	}

	void DraggingOverRightPanel( Widget w, int x, int y, Widget receiver )
	{
		if( w )
		{
			ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
			if( !ipw )
			{
				string name = w.GetName();
				name.Replace( "PanelWidget", "Render" );
				ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				if( !ipw )
				{
					ipw = ItemPreviewWidget.Cast( w );
					if( !ipw )
					{
						return;
					}
				}
			}
			
			EntityAI item = ipw.GetItem();
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			
			if( player && item && ( ( player.GetInventory().CanAddEntityToInventory( item ) ) || ( player.GetHumanInventory().HasEntityInHands( item ) ) ) )
			{
				InventoryLocation inv_loc = new InventoryLocation;
				player.GetInventory().FindFreeLocationFor( item, FindInventoryLocationType.ANY, inv_loc );
	
				ItemManager.GetInstance().HideDropzones();
				GetMainPanel().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				if( inv_loc.GetType() == 4 )
				{
					ItemManager.GetInstance().HideDropzones();
					GetMainPanel().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				}
	
				ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			}
			else if( item )
			{
				ItemManager.GetInstance().ShowSourceDropzone( item );
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			}
		}
	}
	
	void DraggingOverCenterPanel( Widget w, int x, int y, Widget receiver )
	{
		if( w )
		{
			ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
			if( !ipw )
			{
				string name = w.GetName();
				name.Replace( "PanelWidget", "Render" );
				ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
				if( !ipw )
				{
					ipw = ItemPreviewWidget.Cast( w );
					if( !ipw )
					{
						return;
					}
				}
			}
			
			EntityAI item = ipw.GetItem();
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			
			if( player && item )
			{
				int slot_id = item.GetInventory().GetSlotId(0);
				EntityAI slot_item = player.GetInventory().FindAttachment( slot_id );
				
				bool can_take =( player.GetInventory().CanAddAttachment( item ) || ( slot_item && player.GetInventory().CanSwapEntities( item, slot_item ) ) );
				if( can_take )
				{
					InventoryLocation inv_loc = new InventoryLocation;
					player.GetInventory().FindFreeLocationFor( item, FindInventoryLocationType.ATTACHMENT, inv_loc );
		
					ItemManager.GetInstance().HideDropzones();
					GetMainPanel().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
					if( inv_loc.GetType() == 4 )
					{
						ItemManager.GetInstance().HideDropzones();
						GetMainPanel().FindAnyWidget( "HandsPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
					}
		
					ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
				}
				else
				{
					ItemManager.GetInstance().ShowSourceDropzone( item );
					ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
				}
			}
			else
			{
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			}
		}
	}

	override void UpdateInterval()
	{
		PlayerBase player;
		
		if( GetGame().GetInput().GetActionDown( UAUIExpandCollapseContainer, false ) )
		{
			if( m_RightArea.IsActive() )
			{
				m_RightArea.ExpandCollapseContainer();
			}
			if( m_LeftArea.IsActive() )
			{
				//m_LeftArea.ExpandCollapseContainer();
			}
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIFastEquipOrSplit, false ) )
		{
			if( m_HandsArea.IsActive() )
			{
				m_HandsArea.EquipItem();
			}
			if( m_RightArea.IsActive() )
			{
				m_RightArea.EquipItem();
			}
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.EquipItem();
			}
			UpdateConsoleToolbar();
			ItemManager.GetInstance().HideTooltip();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUISelectItem , false ) )
		{
			ItemManager.GetInstance().SetItemMicromanagmentMode( true );
			
			if( m_RightArea.IsActive() )
			{
				m_RightArea.SelectItem();
			}
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.SelectItem();
			}
			if( m_HandsArea.IsActive() )
			{
				m_HandsArea.SelectItem();
			}
			UpdateConsoleToolbar();
			ItemManager.GetInstance().HideTooltip();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIFastTransferToVicinity, false ) )
		{
			if( m_HandsArea.IsActive() )
			{
				m_HandsArea.TransferItemToVicinity();
			}
			if( m_RightArea.IsActive() )
			{
				m_RightArea.TransferItemToVicinity();
			}
			UpdateConsoleToolbar();
			ItemManager.GetInstance().HideTooltip();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUIFastTransferItem, false ) )
		{
			if( ItemManager.GetInstance().IsMicromanagmentMode() )
			{
				return;
			}
			if( m_RightArea.IsActive() )
			{
				m_RightArea.TransferItem();
			}
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.TransferItem();
			}
			if( m_HandsArea.IsActive() )
			{
				m_HandsArea.TransferItem();
			}
			UpdateConsoleToolbar();
			ItemManager.GetInstance().HideTooltip();
		}
		
		if( GetGame().GetInput().GetActionDown( UAUINextUp, false ) )
		{
			ItemManager.GetInstance().HideTooltip();
			if( !ItemManager.GetInstance().IsMicromanagmentMode() )
			{
				m_RightArea.UnfocusGrid();
				m_LeftArea.UnfocusGrid();
				m_HandsArea.UnfocusGrid();
			}
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.SetPreviousActive( );
			}
			if( m_RightArea.IsActive() )
			{
				m_RightArea.SetPreviousActive( );
			}
			if( m_HandsArea.IsActive() )
			{
				m_HandsArea.SetPreviousActive();
			}
			
#ifdef PLATFORM_CONSOLE
				UpdateConsoleToolbar();
#endif		
		}

		if( GetGame().GetInput().GetActionDown( UAUINextDown, false ) )
		{
			ItemManager.GetInstance().HideTooltip();
			if( !ItemManager.GetInstance().IsMicromanagmentMode() )
			{
				m_RightArea.UnfocusGrid();
				m_LeftArea.UnfocusGrid();
				m_HandsArea.UnfocusGrid();
			}
			if( m_LeftArea.IsActive() )
			{
				m_LeftArea.SetNextActive();
			}
			if( m_RightArea.IsActive() )
			{
				m_RightArea.SetNextActive();
			}
			if( m_HandsArea.IsActive() )
			{
				m_HandsArea.SetNextActive();
			}
#ifdef PLATFORM_CONSOLE
				UpdateConsoleToolbar();
#endif
		}

		if( GetGame().GetInput().GetActionDown( UAUITabLeft, false ) )
		{
			ItemManager.GetInstance().HideTooltip();
			if( ItemManager.GetInstance().IsMicromanagmentMode() )
			{
				ItemManager.GetInstance().SetItemMoving( true );
			}
			if( m_LeftArea.IsActive() )
			{
				if( !ItemManager.GetInstance().IsMicromanagmentMode() )
				{
					m_LeftArea.UnfocusGrid();
				}
				m_LeftArea.SetActive( false );
				m_RightArea.SetActive( true );
				
#ifdef PLATFORM_CONSOLE
			UpdateConsoleToolbar();
#endif
			}
			else if( m_RightArea.IsActive() )
			{
				if( !ItemManager.GetInstance().IsMicromanagmentMode() )
				{
					m_RightArea.UnfocusGrid();
				}
				m_RightArea.SetActive( false );
				player = PlayerBase.Cast( GetGame().GetPlayer() );
				EntityAI item_in_hands = player.GetItemInHands();
				m_HandsArea.SetActive( true );

#ifdef PLATFORM_CONSOLE
			UpdateConsoleToolbar();
#endif	
			}
			else if( m_HandsArea.IsActive() )
			{
				m_HandsArea.UnfocusGrid();
				m_HandsArea.SetActive( false );
				m_LeftArea.SetActive( true );
				
#ifdef PLATFORM_CONSOLE
				UpdateConsoleToolbar();
#endif		
			}
		}

		if( GetGame().GetInput().GetActionDown( UAUITabRight, false ) )
		{
			if( ItemManager.GetInstance().IsMicromanagmentMode() )
			{
				ItemManager.GetInstance().SetItemMoving( true );
			}
			ItemManager.GetInstance().HideTooltip();
			if( m_LeftArea.IsActive() )
			{
				if( !ItemManager.GetInstance().IsMicromanagmentMode() )
				{
					m_LeftArea.UnfocusGrid();
				}
				m_LeftArea.SetActive( false );
				player = PlayerBase.Cast( GetGame().GetPlayer() );
				item_in_hands = player.GetItemInHands();
				m_HandsArea.SetActive( true );
				
#ifdef PLATFORM_CONSOLE
				UpdateConsoleToolbar();
#endif			
			}
			else if( m_RightArea.IsActive() )
			{
				if( !ItemManager.GetInstance().IsMicromanagmentMode() )
				{
					m_RightArea.UnfocusGrid();
				}
				m_RightArea.SetActive( false );
				m_LeftArea.SetActive( true );
				
#ifdef PLATFORM_CONSOLE
				UpdateConsoleToolbar();
#endif	
			}
			else if( m_HandsArea.IsActive() )
			{
				m_HandsArea.UnfocusGrid();
				m_HandsArea.SetActive( false );
				m_RightArea.SetActive( true );
				
#ifdef PLATFORM_CONSOLE
				UpdateConsoleToolbar();
#endif
	
			}
		}

#ifdef PLATFORM_CONSOLE
		//Open Quickbar radial menu
		if( GetGame().GetInput().GetActionDown( UAUIQuickbarRadialInventoryOpen, false ) )
		{
			//assign item
			EntityAI item_to_assign;
						
			if ( m_HandsArea.IsActive() )
			{
				player = PlayerBase.Cast( GetGame().GetPlayer() );
				item_to_assign = player.GetItemInHands();
			}
			else if ( m_RightArea.IsActive() )
			{
				item_to_assign = m_RightArea.GetFocusedItem();
			}
			
			if ( item_to_assign )
			{
				RadialQuickbarMenu.SetItemToAssign( item_to_assign );
				
				//open radial quickbar menu
				if ( !GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
				{
					RadialQuickbarMenu.OpenMenu( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
				}				
			}
		}
#endif

		//Open InGame menu
		if( GetGame().GetInput().GetActionDown( UAUIMenu, false ) )
		{
			if ( !GetGame().GetUIManager().IsMenuOpen( MENU_INGAME ) )
			{
				GetGame().GetMission().Pause();
			}
		}
		
		m_LeftArea.UpdateInterval();
		m_RightArea.UpdateInterval();
		m_HandsArea.UpdateInterval();
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.Inventory;
	}

	void Init() {}

	void Reset()
	{
		m_LeftArea.Refresh();
		m_RightArea.Refresh();
		m_HandsArea.Refresh();
	}
	
	void ResetFocusedContainers()
	{
		m_RightArea.UnfocusGrid();
		m_LeftArea.UnfocusGrid();
		m_HandsArea.UnfocusGrid();
		m_LeftArea.SetActive( false );
		m_HandsArea.SetActive( false );
		m_RightArea.SetActive( false );
		m_RightArea.ResetFocusedContainer();
		m_LeftArea.ResetFocusedContainer();
		m_RightArea.SetActive( true );
	}

	override void OnShow()
	{
		//start update
		SetFocus( GetMainPanel() );
		Deserialize();
		//m_MainPanel.Update(); 
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( player && player.IsPlacingLocal() )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(player.TogglePlacingLocal);
		}

		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.SetSpecialtyMeterVisibility( true );
				hud.HideQuickbar( true, true );
				hud.ToggleHud( true, true );
			}
		}
		#ifdef PLATFORM_CONSOLE
				ResetFocusedContainers();
		#endif	
		RefreshQuickbar();
		UpdateInterval();
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(this.UpdateInterval);
		UpdateConsoleToolbar();
	}

	override void OnHide()
	{
		//start update
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateInterval);
		
		Serialize();
		ItemManager.GetInstance().HideTooltip();
		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.SetSpecialtyMeterVisibility( false );
				if( hud.GetQuickBarState() )
				{
					hud.ShowQuickbar();
				}
				
				hud.ToggleHud( hud.GetHudState(), true );
			}
		}
		ItemManager.GetInstance().SetSelectedItem( NULL, NULL, NULL );
	}

	void RefreshQuickbar()
	{
#ifdef PLATFORM_CONSOLE
#else
		if ( m_Quickbar )
		{
			m_Quickbar.UpdateItems( m_QuickbarWidget );
		}
#endif	
	}


	void ShowQuickbar()
	{
#ifdef PLATFORM_CONSOLE
#else
		if ( m_QuickbarWidget )
		{
			m_QuickbarWidget.Show( true );
		}
#endif
	}
	
	#ifdef PLATFORM_XBOX
	string to_hands_swap = "<image set=\"xbox_buttons\" name=\"A\" /> To hands/swap    ";
	string drop = "<image set=\"xbox_buttons\" name=\"Y\" />(hold) Drop    ";
	string equip = "<image set=\"xbox_buttons\" name=\"X\" /> Equip    ";
	string split = "<image set=\"xbox_buttons\" name=\"X\" /> Split    ";
	string to_inventory = "<image set=\"xbox_buttons\" name=\"Y\" /> To inventory    ";
	string open_close_container = "<image set=\"xbox_buttons\" name=\"RS\" /> Open/Close container    ";
	string combine = "<image set=\"xbox_buttons\" name=\"B\" /> Combine";
	string micromanagment = "<image set=\"xbox_buttons\" name=\"A\" /> (hold) Micromanagment    ";
	string quickslot = "<image set=\"xbox_buttons\" name=\"LS\" /> (hold) Quickslot    ";
	#else
	string to_hands_swap = "<image set=\"playstation_buttons\" name=\"cross\" /> To hands/swap    ";
	string drop = "<image set=\"playstation_buttons\" name=\"triangle\" />(hold) Drop    ";
	string equip = "<image set=\"playstation_buttons\" name=\"square\" /> Equip    ";
	string split = "<image set=\"playstation_buttons\" name=\"square\" /> Split    ";
	string to_inventory = "<image set=\"playstation_buttons\" name=\"triangle\" /> To inventory    ";
	string open_close_container = "<image set=\"playstation_buttons\" name=\"R3\" /> Open/Close container    ";
	string combine = "<image set=\"playstation_buttons\" name=\"circle\" /> Combine";
	string micromanagment = "<image set=\"playstation_buttons\" name=\"cross\" /> (hold) Micromanagment    ";
	string quickslot = "<image set=\"playstation_buttons\" name=\"L3\" /> (hold) Quickslot    ";
	#endif
	
	string ConsoleToolbarTypeToString( int console_toolbar_type )
	{
		switch ( console_toolbar_type )
		{
			case ConsoleToolbarType.PLAYER_EQUIPMENT_SLOTS_ITEM:
			return to_hands_swap + drop + micromanagment + quickslot;
			case ConsoleToolbarType.PLAYER_EQUIPMENT_SLOTS_ITEM_WITH_CARGO:
			return open_close_container + to_hands_swap + drop + micromanagment + quickslot;
			case ConsoleToolbarType.PLAYER_EQUIPMENT_SLOTS_ITEM_WITH_ATTACHMENTS:
			return open_close_container + to_hands_swap + drop + micromanagment + quickslot;
			case ConsoleToolbarType.PLAYER_EQUIPMENT_SLOTS_ITEM_FREE:
			return "";
			
			case ConsoleToolbarType.PLAYER_CARGO_CONTAINER_EMPTY_CONTAINER:
			return "";
			case ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM:
			return to_hands_swap + drop + equip + micromanagment + quickslot;
			case ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM_NO_EQUIP:
			return to_hands_swap + drop + micromanagment + quickslot;
			case ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM_WITH_QUANTITY:
			return to_hands_swap + drop + split + micromanagment + quickslot;
			case ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM_WITH_ATTACHMENTS:
			return to_hands_swap + drop + equip + micromanagment + quickslot;
			case ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM_WITH_ATTACHMENTS_NO_EQUIP:
			return to_hands_swap + drop + micromanagment + quickslot;
			
			case ConsoleToolbarType.HANDS_ITEM:
			return to_inventory + drop + equip  + micromanagment + quickslot;
			case ConsoleToolbarType.HANDS_ITEM_NO_EQUIP:
			return to_inventory + drop  + micromanagment + quickslot;
			case ConsoleToolbarType.HANDS_ITEM_WITH_QUANTITY:
			return to_inventory + drop + split  + micromanagment + quickslot;
			case ConsoleToolbarType.HANDS_ITEM_EMPTY:
			return "";
			
			case ConsoleToolbarType.VICINITY_CONTAINER_LIST_ITEM_WITH_CONTAINER:
			return open_close_container + to_hands_swap + to_inventory + equip  + micromanagment + quickslot;
			case ConsoleToolbarType.VICINITY_CONTAINER_LIST_ITEM_WITH_CONTAINER_NO_EQUIP:
			return open_close_container + to_hands_swap + to_inventory  + micromanagment + quickslot;
			case ConsoleToolbarType.VICINITY_CONTAINER_LIST_ITEM:
			return to_hands_swap + to_inventory + equip  + micromanagment + quickslot;
			case ConsoleToolbarType.VICINITY_CONTAINER_LIST_ITEM_NO_EQUIP:
			return to_hands_swap + to_inventory  + micromanagment + quickslot;
			case ConsoleToolbarType.VICINITY_CONTAINER_LIST_ITEM_WITH_QUANTITY:
			return to_hands_swap + to_inventory  + micromanagment + quickslot;
			case ConsoleToolbarType.VICINITY_CONTAINER_LIST_EMPTY_ITEM:
			return "";
			
			case ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_EMPTY:
			return "";
			case ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_ITEM:
			return to_hands_swap + to_inventory + equip + micromanagment + quickslot;
			case ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_ITEM_NO_EQUIP:
			return to_hands_swap + to_inventory + micromanagment + quickslot;
			case ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_ITEM_WITH_QUANTITY:
			return to_hands_swap + to_inventory + split + micromanagment + quickslot;
		}
		return "";
	}
	
	//Console toolbar
	void UpdateConsoleToolbar()
	{
#ifdef PLATFORM_CONSOLE		
		string general_text;
		string context_text;
		
		#ifdef PLATFORM_XBOX
		general_text = "<image set=\"xbox_buttons\" name=\"LB\" />/<image set=\"xbox_buttons\" name=\"RB\" /> Switch column        <image set=\"xbox_buttons\" name=\"LT\" />/<image set=\"xbox_buttons\" name=\"RT\"/> Switch cargo container";
		#else
		general_text = "<image set=\"playstation_buttons\" name=\"L2\" />/<image set=\"playstation_buttons\" name=\"R2\" /> Switch column        <image set=\"playstation_buttons\" name=\"L1\" />/<image set=\"playstation_buttons\" name=\"R1\"/> Switch cargo container";
		#endif
		//context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_EQUIPMENT_SLOTS_ITEM );
		if ( m_LeftArea.IsActive() )
		{
			VicinityContainer vicinity_container = m_LeftArea.GetVicinityContainer();
			if( vicinity_container.IsVicinityContainerIconsActive() )
			{
				VicinityIconsContainer vicinity_icons_container = vicinity_container.GetVicinityIconsContainer();
				if( vicinity_icons_container.IsItemWithContainerActive() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_LIST_ITEM_WITH_CONTAINER );
				}
				else if( vicinity_icons_container.IsItemWithQuantityActive() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_LIST_ITEM_WITH_QUANTITY );
				}
				else if( vicinity_icons_container.IsItemActive() )
				{
					if( vicinity_icons_container.CanEquip() )
					{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_LIST_ITEM );
					}
					else
					{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_LIST_ITEM_NO_EQUIP );
					}
				}
				else if( vicinity_icons_container.IsEmptyItemActive() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_LIST_EMPTY_ITEM );
				}
				
				if( vicinity_icons_container.CanCombine() )
				{
						context_text += combine;
				}
				
				if( vicinity_icons_container.CanCombineAmmo() )
				{
					context_text += combine;
				}
			}
			else if( vicinity_container.IsItemWithCargoActive()	)
			{
				ItemWithCargo iwc = ItemWithCargo.Cast( vicinity_container.GetFocusedContainer() );
				if( iwc.IsEmpty() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_EMPTY );
				}
				else if( iwc.IsItemActive() )
				{
					if( iwc.CanEquip() )
					{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_ITEM );
					}
					else
					{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_ITEM_NO_EQUIP );
					}
				}
				else if( iwc.IsItemWithQuantityActive() )
				{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_ITEM_WITH_QUANTITY );
				}
				
				if( iwc.CanCombine() )
				{
					context_text += combine;
				}
				
				if( iwc.CanCombineAmmo() )
				{
					context_text += combine;
				}
			}
			else if( vicinity_container.IsItemWithAttachmentsActive() )
			{
				ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( vicinity_container.GetFocusedContainer() );
				if( iwca.IsEmpty() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_EMPTY );
				}
				else if( iwca.IsItemActive() )
				{
					if( iwca.CanEquip() )
					{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_ITEM );
					}
					else
					{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_ITEM_NO_EQUIP );
					}
				}
				else if( iwca.IsItemWithQuantityActive() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.VICINITY_CONTAINER_DETAILS_ITEM_WITH_QUANTITY );
				}
				
				if( iwca.CanCombine() )
				{
					context_text += combine;
				}
			}
		}
		else if ( m_RightArea.IsActive() )
		{
			PlayerContainer player_container = m_RightArea.GetPlayerContainer();
			if ( m_RightArea.IsPlayerEquipmentActive() )
			{
				if( player_container.IsItemActive() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_EQUIPMENT_SLOTS_ITEM );
				}
				else if( player_container.IsItemWithContainerActive() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_EQUIPMENT_SLOTS_ITEM_WITH_CARGO );
				}
				else if( player_container.IsEmptyItemActive() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_EQUIPMENT_SLOTS_ITEM_FREE );
				}
				
				if( player_container.CanCombine() )
				{
					context_text += combine;
				}
			}
			else if( player_container.IsItemWithCargoActive() )
			{
				ItemWithCargo iwc1 = ItemWithCargo.Cast( player_container.GetFocusedContainer() );
				if( iwc1.IsEmpty() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_CARGO_CONTAINER_EMPTY_CONTAINER );
				}
				else if( iwc1.IsItemActive() )
				{
					if( iwc1.CanEquip() )
					{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM );
					}
					else
					{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM_NO_EQUIP );
					}
				}
				else if( iwc1.IsItemWithQuantityActive() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM_WITH_QUANTITY );
				}
				
				if( iwc1.CanCombine() )
				{
					context_text += combine;
				}
				
				if( iwc1.CanCombineAmmo() )
				{
					context_text += combine;
				}
			}
			else if( player_container.IsItemWithAttachmentsActive() )
			{
				ItemWithCargoAndAttachments iwca1 = ItemWithCargoAndAttachments.Cast( player_container.GetFocusedContainer() );
				if( iwca1.IsEmpty() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_CARGO_CONTAINER_EMPTY_CONTAINER );
				}
				else if( iwca1.IsItemActive() )
				{
					if( iwca1.CanEquip() )
					{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM );
					}
					else
					{
						context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM_NO_EQUIP );
					}
				}
				else if( iwca1.IsItemWithQuantityActive() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.PLAYER_CARGO_CONTAINER_ITEM_WITH_QUANTITY );
				}
				
				if( iwca1.CanCombine() )
				{
					context_text += combine;
				}
			}
		}
		else if ( m_HandsArea.IsActive() )
		{
			if( m_HandsArea.IsItemActive() )
			{
				if( m_HandsArea.CanEquip() )
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.HANDS_ITEM );
				}
				else
				{
					context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.HANDS_ITEM_NO_EQUIP );
				}
			}
			else if( m_HandsArea.IsItemWithQuantityActive() )
			{
				context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.HANDS_ITEM_WITH_QUANTITY );
			}
			else if( m_HandsArea.IsEmpty() )
			{
				context_text = ConsoleToolbarTypeToString( ConsoleToolbarType.HANDS_ITEM_EMPTY );
			}
		}
		
		//!!!!! Richtext string must end with character or white space (no tag elements)
		//set toolbar text
		RichTextWidget general_toolbar = RichTextWidget.Cast( m_ConsoleToolbar.FindAnyWidget( "GeneralToolbarText" ) );
		general_text = general_text + " ";							//richtext end-tag issue hotfix
		general_toolbar.SetText( general_text );
		
		RichTextWidget context_toolbar = RichTextWidget.Cast( m_ConsoleToolbar.FindAnyWidget( "ContextToolbarText" ) );
		context_text = context_text + " ";					//richtext end-tag issue hotfix
		context_toolbar.SetText( context_text );
		#endif
	}
}
