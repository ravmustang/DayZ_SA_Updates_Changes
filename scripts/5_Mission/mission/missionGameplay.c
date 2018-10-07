// #include "Scripts/GUI/IngameHud.c"

class MissionGameplay extends MissionBase
{
	int		m_life_state;
	bool	m_initialized;
	bool	m_invUpdateThisFrame;
	protected UIManager	m_UIManager;
	
	Widget			m_hud_root_widget;
	ImageWidget m_microphone_icon;
	
	private ref InventoryManager 	m_drag_manager;
	private ref WidgetCache 		m_Widgets_Cache;
	
	InventoryMenu 					m_inventory_menu;
	ref InventoryMenuNew			m_inventory_menu_new;
	ref Chat 						m_chat;
	ref ActionMenu					m_actionMenu;
	ref IngameHud					m_hud;
	ref HudDebug					m_hud_debug;
	ref LogoutMenu					m_logout;
	ref DebugMonitor				m_debugMonitor;
	
	ref Timer						m_chat_channel_hide_timer;
	ref WidgetFadeTimer				m_chat_channel_fade_timer;
	
	Widget							m_chat_channel_area;
	TextWidget						m_chat_channel_text;
	NoteMenu 						m_note;
	
	protected ref Timer				m_ToggleHudTimer;
	protected const int 			HOLD_LIMIT_TIME	= 300; //ms
	protected int					m_ActionDownTime;
	protected int					m_ActionUpTime;
	protected bool 					m_InitOnce;
	
	void MissionGameplay()
	{
		DestroyAllMenus();
		m_initialized = false;
		m_hud_root_widget = NULL;
		m_chat = new Chat;
		m_actionMenu = new ActionMenu;
		m_life_state = -1;
		m_hud = new IngameHud;
		m_chat_channel_fade_timer = new WidgetFadeTimer;
		m_chat_channel_hide_timer = new Timer(CALL_CATEGORY_GUI);
		
		m_ToggleHudTimer = new Timer(CALL_CATEGORY_GUI);

		m_drag_manager = new InventoryManager;
		
		g_Game.m_loadingScreenOn = true;
		
		SyncEvents.RegisterEvents();
		g_Game.SetGameState( DayZGameState.IN_GAME );
		PlayerBase.Event_OnPlayerDeath.Insert( Pause );
	}
	
	void ~MissionGameplay()
	{
		DestroyInventory();
		PlayerBase.Event_OnPlayerDeath.Remove( Pause );
		//GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.UpdateDebugMonitor);
	#ifndef NO_GUI
		if (g_Game.GetUIManager() && g_Game.GetUIManager().ScreenFadeVisible())
		{
			GetGame().SetEVUser(0);
			g_Game.GetUIManager().ScreenFadeOut(0);
		}
	#endif

	}
	
	InventoryMenuNew GetInventory()
	{
		return m_inventory_menu_new;
	}
	
	override void OnInit()
	{
		if (m_initialized)
		{
			return;
		}
			
		PPEffects.Init();
		
		m_UIManager = GetGame().GetUIManager();
			
		g_Game.m_loadingScreenOn = true;
		m_initialized = true;

		// init hud ui
		if (!m_hud_root_widget)
		{
			m_hud_root_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_hud.layout");
			
			m_hud_root_widget.Show(false);
			m_chat.Init(m_hud_root_widget.FindAnyWidget("ChatFrameWidget"));
			m_actionMenu.Init( m_hud_root_widget.FindAnyWidget("ActionsPanel"), TextWidget.Cast( m_hud_root_widget.FindAnyWidget("DefaultActionWidget") ) );
			m_hud.Init( m_hud_root_widget.FindAnyWidget("HudPanel") );
			m_microphone_icon = ImageWidget.Cast( m_hud_root_widget.FindAnyWidget("mic") );

			// notification window
			m_notification_widget = new NotificationMessage( m_hud_root_widget );
			
			// chat channel
			m_chat_channel_area = m_hud_root_widget.FindAnyWidget("ChatChannelPanel");
			m_chat_channel_text = TextWidget.Cast( m_hud_root_widget.FindAnyWidget("ChatChannelText") );
		}
		
		// init hud ui
		
		if ( GetGame().IsDebug() )
		{
			m_hud_debug = new HudDebug;
			
			if ( !m_hud_debug.IsInitialized() )
			{
				m_hud_debug.Init( GetGame().GetWorkspace().CreateWidgets("gui/layouts/debug/day_z_hud_debug.layout") );
				
				Debug.SetEnabledLogs(PluginConfigDebugProfile.GetInstance().GetLogsEnabled());
			}
		}

		//AIBehaviourHL.RegAIBehaviour("zombie2",AIBehaviourHLZombie2,AIBehaviourHLDataZombie2);
		//RegBehaviour("zombie2",AIBehaviourHLZombie2,AIBehaviourHLDataZombie2);
		
		m_Widgets_Cache = new WidgetCache;
		
		if( GetGame().IsMultiplayer() )
		{
			OnlineServices.m_MuteUpdateAsyncInvoker.Insert( SendMuteListToServer );
		}
	}
	
	UIManager GetUIManager()
	{
		return m_UIManager;
	}

	override void OnMissionStart()
	{
		//does not display HUD until player is fully loaded
		//m_hud_root_widget.Show(true);
		GetUIManager().ShowUICursor(false);
		g_Game.SetMissionState( DayZGame.MISSION_STATE_GAME );
	}
	
	void InitInventory()
	{
		DestroyAllMenus();
		if (m_inventory_menu_new == NULL)
		{
			m_inventory_menu_new = InventoryMenuNew.Cast( GetUIManager().EnterScriptedMenu(MENU_INVENTORY, NULL) );
			GetUIManager().HideScriptedMenu( m_inventory_menu_new );
		}
	}

	WidgetCache GetWidgetCache()
	{
		return m_Widgets_Cache;
	}
	
	void TickScheduler(float timeslice)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
				
		if( player)
			player.OnTick();
	}
	
	void SendMuteListToServer( map<string, bool> mute_list )
	{
		if( mute_list && mute_list.Count() > 0 )
		{
			if ( ScriptInputUserData.CanStoreInputUserData() )
			{
				ScriptInputUserData ctx = new ScriptInputUserData;
				ctx.Write( INPUT_UDT_USER_SYNC_PERMISSIONS );
				ref map<string, bool> mute_list2;
				if( mute_list.Count() > 4 )
				{
					mute_list2 = new map<string, bool>;
					for( int i = mute_list.Count() - 1; i >= 4; i-- )
					{
						mute_list2.Insert( mute_list.GetKey( i ), mute_list.GetElement( i ) );
						mute_list.RemoveElement( i );
					}
				}
				ctx.Write( mute_list );
				Print( mute_list );
				ctx.Send();
				SendMuteListToServer( mute_list2 );
			}
		}
	}
	
	override void OnMissionFinish()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( player != NULL )
		{
			int player_last_shaved_seconds = player.GetLastShavedSeconds();
			GetGame().SetProfileString( "lastShavedSeconds", player_last_shaved_seconds.ToString() );
		}
		Print("OnMissionFinish");
		GetUIManager().HideDialog();
		DestroyAllMenus();
		
		m_chat.Destroy();
		delete m_hud_root_widget;
		
		#ifdef PLATFORM_CONSOLE
			OnlineServices.m_MuteUpdateAsyncInvoker.Remove( SendMuteListToServer );
		#endif

		if (m_debugMonitor)
			m_debugMonitor.Hide();
		g_Game.GetUIManager().ShowUICursor(false);
		g_Game.SetMissionState( DayZGame.MISSION_STATE_FINNISH );
	}
	
	override void OnUpdate(float timeslice)
	{
		Man player = GetGame().GetPlayer();
		PlayerBase playerPB = PlayerBase.Cast(player);
		TickScheduler(timeslice);
		UpdateDummyScheduler();//for external entities
		UIScriptedMenu menu = m_UIManager.GetMenu();
		InventoryMenuNew inventory = InventoryMenuNew.Cast( m_UIManager.FindMenu(MENU_INVENTORY) );
		//m_inventory_menu_new = inventory;
		InspectMenuNew inspect = InspectMenuNew.Cast( m_UIManager.FindMenu(MENU_INSPECT) );
		Input input = GetGame().GetInput();
		
		//TODO should be switchable
		if (playerPB && playerPB.enterNoteMenuRead)
		{
			playerPB.enterNoteMenuRead = false;
			Paper paper = Paper.Cast(playerPB.GetItemInHands());
			m_note = NoteMenu.Cast( GetUIManager().EnterScriptedMenu(MENU_NOTE, menu) ); //NULL means no parent
			m_note.InitRead(paper.m_AdvancedText);
		}
		
		if (playerPB && playerPB.enterNoteMenuWrite)
		{
			playerPB.enterNoteMenuWrite = false;
			m_note = NoteMenu.Cast( GetUIManager().EnterScriptedMenu(MENU_NOTE, menu) ); //NULL means no parent
			m_note.InitWrite(playerPB.m_paper,playerPB.m_writingImplement,playerPB.m_Handwriting);
		}

#ifdef PLATFORM_CONSOLE
		//Quick Reload Weapon
		if ( !menu && input.GetActionDown( UAQuickReload, false ) )
		{
			if ( !GetGame().IsInventoryOpen() && playerPB && !playerPB.GetActionManager().FindActionTarget().GetObject() )
			{
				EntityAI entity_hands = playerPB.GetHumanInventory().GetEntityInHands();
				
				if ( entity_hands && entity_hands.IsWeapon() )
				{
					playerPB.QuickReloadWeapon( entity_hands );
				}
			}
		}

		//Switch beween weapons in quickslots 
		if( !menu && input.GetActionDown( UAUIRadialMenuPick, false ) )
		{
			if ( !GetGame().IsInventoryOpen() )
			{
				EntityAI entity_in_hands = playerPB.GetHumanInventory().GetEntityInHands();
				EntityAI quickbar_entity;
				int quickbar_index = 0;
				
				if ( entity_in_hands )
				{			
					int quickbar_entity_hands_index = playerPB.FindQuickBarEntityIndex( entity_in_hands );
					
					if ( quickbar_entity_hands_index > -1 && quickbar_entity_hands_index < MAX_QUICKBAR_SLOTS_COUNT - 1 )	//(0->8)
					{
						quickbar_index = quickbar_entity_hands_index + 1;
					}
				}

				//find next weapon
				for ( int iter = 0; iter < MAX_QUICKBAR_SLOTS_COUNT; ++iter )
				{
					quickbar_entity = playerPB.GetQuickBarEntity( quickbar_index );
					
					if ( quickbar_entity && ( quickbar_entity.IsWeapon() || ( quickbar_entity.IsMeleeWeapon() && !quickbar_entity.IsMagazine() ) ) )
					{
						break;
					}
					
					quickbar_index += 1;
					if ( quickbar_index > MAX_QUICKBAR_SLOTS_COUNT - 1 )
					{
						quickbar_index = 0;	//reset
					}
				}
				
				//swap
				int slot_id;
				if ( quickbar_index > -1 )
				{
					slot_id = quickbar_index + 1;
					if ( slot_id == MAX_QUICKBAR_SLOTS_COUNT )
					{
						slot_id = 0;
					}
					
					playerPB.RadialQuickBarSingleUse( slot_id );
				}
			}
		}

		//Gestures
		if(input.GetActionDown(UAUIGesturesOpen, false))
		{
			//open gestures menu
			if ( !GetUIManager().IsMenuOpen( MENU_GESTURES ) )
			{
				GesturesMenu.OpenMenu();
			}
		}
		
		if(input.GetActionUp(UAUIGesturesOpen, false))
		{
			//close gestures menu
			if ( GetUIManager().IsMenuOpen( MENU_GESTURES ) )
			{
				GesturesMenu.CloseMenu();
			}
		}

		//Radial quickbar
		if(input.GetActionDown(UAUIQuickbarRadialOpen, false))
		{
			//open gestures menu
			if ( !GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				RadialQuickbarMenu.OpenMenu();
			}
		}
		
		if(input.GetActionUp(UAUIQuickbarRadialOpen, false))
		{
			//close gestures menu
			if ( GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				RadialQuickbarMenu.CloseMenu();
			}
		}
		
		//Radial Quickbar from inventory
		if( GetGame().GetInput().GetActionUp( UAUIQuickbarRadialInventoryOpen, false ) )
		{
			//close radial quickbar menu
			if ( GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				RadialQuickbarMenu.CloseMenu();
				RadialQuickbarMenu.SetItemToAssign( NULL );
			}
		}		
#endif

		if (player && m_life_state == EPlayerStates.ALIVE && !player.IsUnconscious() )
		{
			// enables HUD on spawn
			if (m_hud_root_widget)
			{
				m_hud_root_widget.Show(true);
			}
			
		#ifndef NO_GUI
			// fade out black screen
			
			if ( GetUIManager().ScreenFadeVisible() )
			{
				 GetUIManager().ScreenFadeOut(0.5);
			}
			
		#endif
		
			if( input.GetActionDown(UAGear, false ) )
			{
				if( !inventory )
				{
					if( GetGame().IsOldInventory() )
					{
						ShowInventory();
					}
					else
					{
						ShowInventory2();
					}
				}
				else if( menu == inventory )
				{
					HideInventory();
				}
			}

			if (input.GetActionDown(UAChat, false))
			{
				if (menu == NULL)
				{
					m_chat_channel_hide_timer.Stop();
					m_chat_channel_fade_timer.Stop();
					m_chat_channel_area.Show(false);
					m_UIManager.EnterScriptedMenu(MENU_CHAT_INPUT, NULL);
				}
			}
			
			if ( input.GetActionDown( UAUIQuickbarToggle, false) )
			{
				SetActionDownTime( GetGame().GetTime() );
				bool hud_state = m_hud.GetHudState();
				m_ToggleHudTimer.Run( 0.3, m_hud, "ToggleHud", new Param1<bool>( !hud_state ) );
			}
			
			if ( input.GetActionUp( UAUIQuickbarToggle, false) )
			{
				SetActionUpTime( GetGame().GetTime() );
				
				if ( GetHoldActionTime() < HOLD_LIMIT_TIME )
				{
					if ( menu == NULL )
					{
						if ( m_hud.GetQuickBarState() )
						{
							m_hud.HideQuickbar( false, true );
						}
						else
						{
							m_hud.ShowQuickbar();
						}
					}
				}
				
				m_ToggleHudTimer.Stop();
			}
			
			if ( g_Game.GetInput().GetActionDown( UAZeroingUp, false) || g_Game.GetInput().GetActionDown( UAZeroingDown, false) || g_Game.GetInput().GetActionDown( UAToggleWeapons, false) )
			{
				m_hud.ZeroingKeyPress();
			}
			
			if ( menu == NULL )
			{
				m_actionMenu.Refresh();
				
				if (input.GetActionDown(UANextAction, false))
				{
					m_actionMenu.NextAction();
				}
				
				if (input.GetActionDown(UAPrevAction, false))
				{
					m_actionMenu.PrevAction();
				}
			}
			else
			{
				m_actionMenu.Hide();
			}
			
			//hologram rotation
			if (menu == NULL && playerPB.IsPlacingLocal())
			{
				if (input.GetActionUp(UANextAction, false))
				{
					playerPB.GetHologramLocal().SubtractProjectionRotation(15);
				}
				
				if (input.GetActionUp(UAPrevAction, false))
				{
					playerPB.GetHologramLocal().AddProjectionRotation(15);
				}
			}
		}
		
		// life state check
		if (player)
		{
			int life_state = player.GetPlayerState();
			
			// life state changed
			if (m_life_state != life_state)
			{
				m_life_state = life_state;
				
				if (m_life_state != EPlayerStates.ALIVE)
				{
					CloseAllMenus();
				}
			}
		}
		
		if (menu && !menu.UseKeyboard() && menu.UseMouse())
		{
			int i;
			for (i = 0; i < 5; i++)
			{
				input.DisableKey(i | INPUT_DEVICE_MOUSE);
				input.DisableKey(i | INPUT_ACTION_TYPE_DOWN_EVENT | INPUT_DEVICE_MOUSE);
				input.DisableKey(i | INPUT_ACTION_TYPE_DOUBLETAP | INPUT_DEVICE_MOUSE);
			}
			
			for (i = 0; i < 6; i++)
			{
				input.DisableKey(i | INPUT_DEVICE_MOUSE_AXIS);
			}
		}
		
		if (!m_UIManager.IsDialogVisible())
		{
			if ( menu )
			{
				if ( IsPaused() )
				{
					if(input.GetActionDown(UAUIBack, false) || input.GetActionDown(UAUIMenu, false))
					{
						Continue();
					}
				}
				else if ( menu == inventory )
				{
					if(input.GetActionDown(UAUIBack, false))
					{
						if( ItemManager.GetInstance().GetSelectedItem() == NULL )
						{
							HideInventory();
						}
					}
				}
				else if(input.GetActionDown(UAUIBack, false))
				{
					m_UIManager.Back();
				}
			}
			else if (input.GetActionDown(UAUIMenu, false))
			{
				Pause();
			}
		}
		
		UpdateDebugMonitor();
	}
	
	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		m_hud.KeyPress(key);
		
		
		//temporary
		//Gestures [.]
#ifdef DEVELOPER
		if ( key == KeyCode.KC_PERIOD )
		{
			//open gestures menu
			if ( !GetUIManager().IsMenuOpen( MENU_GESTURES ) )
			{
				//TODO reconnect when appropriate
				GesturesMenu.OpenMenu();
			}
		}
#endif
		/*
		//temporary
		//Radial Quickbar [,]
		if ( key == KeyCode.KC_COMMA )
		{
			//open radial quickbar menu
			if ( !GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				//TODO reconnect when appropriate
				RadialQuickbarMenu.OpenMenu();
			}
		}
		*/
	}
	
	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);
		
		//temporary
		//Gestures [.]
#ifdef DEVELOPER
		if ( key == KeyCode.KC_PERIOD )
		{
			//close gestures menu
			if ( GetUIManager().IsMenuOpen( MENU_GESTURES ) )
			{
				//TODO reconnect when appropriate
				GesturesMenu.CloseMenu();
			}
		}
#endif
		/*
		//temporary
		//Radial Quickbar [,]
		if ( key == KeyCode.KC_COMMA )
		{
			//close radial quickbar menu
			if ( GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				//TODO reconnect when appropriate
				RadialQuickbarMenu.CloseMenu();
			}
		}
		*/
	}
	
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);
		InventoryMenuNew menu;
		Man player = GetGame().GetPlayer();
		
		switch(eventTypeId)
		{
		case ChatMessageEventTypeID:
			Print( "ChatMessageEventTypeID: " );
			Print( ChatMessageEventTypeID );
			ChatMessageEventParams chat_params = ChatMessageEventParams.Cast( params );
			
			if (chat_params.param1 == CCSystem && GetProfileValueBool(SYSTEM_CHAT_MSG, true) == false) break;
			if (chat_params.param1 == CCGlobal && GetProfileValueBool(GLOBAL_CHAT_MSG, true) == false) break;
			if (chat_params.param1 == CCItemTransmitter && GetProfileValueBool(RADIO_CHAT_MSG, true) == false) break;
			if (chat_params.param1 == CCDirect && GetProfileValueBool(DIRECT_CHAT_MSG, true) == false) break;
			
			if (m_life_state == EPlayerStates.ALIVE)
			{
				m_chat.Add(chat_params);
			}
			break;
			
		case ChatChannelEventTypeID:
			ChatChannelEventParams cc_params = ChatChannelEventParams.Cast( params );
			ChatInputMenu chatMenu = ChatInputMenu.Cast( GetUIManager().FindMenu(MENU_CHAT_INPUT) );
			if (chatMenu)
			{
				chatMenu.UpdateChannel();
			}
			else
			{
				m_chat_channel_text.SetText(ChatInputMenu.GetChannelName(cc_params.param1));
				m_chat_channel_fade_timer.FadeIn(m_chat_channel_area, 0.5, true);
				m_chat_channel_hide_timer.Run(2, m_chat_channel_fade_timer, "FadeOut", new Param3<Widget, float, bool>(m_chat_channel_area, 0.5, true));
			}
			break;
			
		case WindowsResizeEventTypeID:

			if(!GetGame().IsOldInventory())
			{
				if( m_inventory_menu_new != NULL )
				{
					m_inventory_menu_new.Reset();
				}
				else if( WidgetCache.GetInstance() )
				{
					WidgetCache.GetInstance().Reset();
				}
			}
			CloseAllMenus();
			DestroyAllMenus();
			
			break;
			
		case VONStateEventTypeID:
			VONStateEventParams vonStateParams = VONStateEventParams.Cast( params );
			if (vonStateParams.param1)
			{
				m_microphone_icon.Show(true);
			}
			else
			{
				m_microphone_icon.Show(false);
			}
			break;
		
		case SetFreeCameraEventTypeID:
			SetFreeCameraEventParams set_free_camera_event_params = SetFreeCameraEventParams.Cast( params );
			PluginDeveloper plugin_developer = PluginDeveloper.Cast( GetPlugin(PluginDeveloper) );
			plugin_developer.OnSetFreeCameraEvent( PlayerBase.Cast( player ), set_free_camera_event_params.param1 );
			break;
		}
	}
	
	override void OnItemUsed(InventoryItem item, Man owner)
	{
		if (item && GetUIManager().GetMenu() == NULL)
		{
			if (item.IsInherited(ItemBook))
			{
				BookMenu bookMenu = BookMenu.Cast( GetUIManager().EnterScriptedMenu(MENU_BOOK, NULL) );
				if (bookMenu)
				{
					bookMenu.ReadBook(item);
				}
			}
		}
	}
	
	void CloseAllMenus()
	{
		HideInventory();
		GetUIManager().CloseAll();
	}
	
	void DestroyAllMenus()
	{
		if( GetUIManager() )
		{
			GetUIManager().HideDialog();
			GetUIManager().CloseAll();
		}
		
		DestroyInventory();
		
		if( m_chat )
			m_chat.Clear();
	}
	
	void MoveHudForInventory( bool inv_open )
	{
		#ifdef PLATFORM_CONSOLE
		IngameHud hud = IngameHud.Cast( GetHud() );
		if( hud )
		{
			if( inv_open )
			{
				hud.GetHudPanelWidget().SetPos( 0, -0.1 );
			}
			else
			{
				hud.GetHudPanelWidget().SetPos( 0, 0 );
			}
		}
		#endif
	}
	
	void ShowInventory()
	{
		bool init = false;
		UIScriptedMenu menu = GetUIManager().GetMenu();
		
		if (menu == NULL && GetGame().GetPlayer().GetHumanInventory().CanOpenInventory() && !GetGame().GetPlayer().IsInventorySoftLocked() && GetGame().GetPlayer().GetHumanInventory().IsInventoryUnlocked() )
		{
			if (m_inventory_menu == NULL)
			{
				m_inventory_menu = InventoryMenu.Cast( GetUIManager().EnterScriptedMenu(MENU_INVENTORY, NULL) );
			}
			else if ( GetUIManager().FindMenu(MENU_INVENTORY) == NULL )
			{
				GetUIManager().ShowScriptedMenu(m_inventory_menu, NULL);
			}
			MoveHudForInventory( true );
			init = true;
		}
		
		if (menu && menu == m_inventory_menu)
		{
			init = true;
		}
		
		if (m_inventory_menu && init)
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			vector rayStart = GetGame().GetCurrentCameraPosition();
			vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 1.5;
			vector hitPos;
			vector hitNormal;
			int hitComponentIndex;
			ref set<Object> hitObjects = new set<Object>;
			DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, hitObjects, NULL, player);
			
			Object target = NULL;
			if( hitObjects.Count() )
				target = hitObjects.Get(0);
				
			m_inventory_menu.InitContainers(EntityAI.Cast(target));

			//player.GetInventoryActionHandler().DeactiveAction();
			//player.GetCraftingManager().CancelInventoryCraft();
		}
	}
	
	void ShowInventory2()
	{
		bool init = false;
		UIScriptedMenu menu = GetUIManager().GetMenu();
		
		if (menu == NULL && GetGame().GetPlayer().GetHumanInventory().CanOpenInventory() && !GetGame().GetPlayer().IsInventorySoftLocked() )
		{
			if (m_inventory_menu_new == NULL)
			{
				m_inventory_menu_new = InventoryMenuNew.Cast( GetUIManager().EnterScriptedMenu(MENU_INVENTORY, NULL) );
			}
			else if ( GetUIManager().FindMenu(MENU_INVENTORY) == NULL )
			{
				GetUIManager().ShowScriptedMenu(m_inventory_menu_new, NULL);
			}
			MoveHudForInventory( true );
			init = true;
		}
		
		if (menu && menu == m_inventory_menu_new)
		{
			init = true;
		}
		
		if (m_inventory_menu_new && init)
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			vector rayStart = GetGame().GetCurrentCameraPosition();
			vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 1.5;
			vector hitPos;
			vector hitNormal;
			int hitComponentIndex;
			ref set<Object> hitObjects = new set<Object>;
			DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, hitObjects, NULL, player);
			
			Object target = NULL;
			if( hitObjects.Count() )
				target = hitObjects.Get(0);
				
			m_inventory_menu_new.InitContainers(EntityAI.Cast(target));
		}
	}
	
	void HideInventory()
	{
		InventoryMenuNew inventory = InventoryMenuNew.Cast( GetUIManager().FindMenu(MENU_INVENTORY) );
		if (inventory)
		{
			GetUIManager().HideScriptedMenu(inventory);
			MoveHudForInventory( false );
		}
	}
	
	void DestroyInventory()
	{
		if (m_inventory_menu)
		{
			m_inventory_menu.Close();
			m_inventory_menu = NULL;
		}
		if (m_inventory_menu_new)
		{
			m_inventory_menu_new.Close();
			m_inventory_menu_new = NULL;
		}
	}
	
	override Hud GetHud()
	{
		return m_hud;
	}
	
	HudDebug GetHudDebug()
	{
		return m_hud_debug;
	}
	
	override void RefreshCrosshairVisibility()
	{
		GetHudDebug().RefreshCrosshairVisibility();
	}
	
	override void HideCrosshairVisibility()
	{
		GetHudDebug().HideCrosshairVisibility();
	}
	
	override bool IsPaused()
	{
		return GetGame().GetUIManager().IsMenuOpen(MENU_INGAME);
	}
	
	override void Pause()
	{
		if ( IsPaused() )
		{
			return;
		} 
		
		CloseAllMenus();
		
		// open ingame menu
		GetUIManager().EnterScriptedMenu(MENU_INGAME, NULL);
	}
	
	override void Continue()
	{
		if ( !IsPaused() )
		{
			return;
		}
		
		GetUIManager().CloseMenu(MENU_INGAME);
	}
	
	override bool IsMissionGameplay()
	{
		return true;
	}
	
	override void AbortMission()
	{
		#ifdef BULDOZER
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(g_Game.RequestExit, IDC_MAIN_QUIT);
		#else
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetDayZGame().ExitToMainMenu);
		#endif
	}
	
	override void CreateLogoutMenu(UIMenuPanel parent)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		// do not show logout screen if player's dead
		if (!player || player.IsDamageDestroyed())
		{
			// exit the game immediately
			AbortMission();
			return;
		}
		
		if (parent)
			m_logout = LogoutMenu.Cast(parent.EnterScriptedMenu(MENU_LOGOUT));
		
		if (m_logout)
			m_logout.SetTimeLoading();
	}
	
	override void StartLogoutMenu(int time)
	{
		if (m_logout)
		{
			if (time > 0)
			{
				// character will be deleted from server int "time" seconds
				m_logout.SetTime(time);
				m_logout.Show();
				
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_logout.UpdateTime, 1000, true);
			}
			else
			{
				// no waiting time -> player is most likely dead
				m_logout.Exit();
			}
		}
	}
	
	override void CreateDebugMonitor()
	{
		if (!m_debugMonitor)
		{
			m_debugMonitor = new DebugMonitor();
			m_debugMonitor.Init();
		}
	}
	
	void UpdateDebugMonitor()
	{
		if (!m_debugMonitor) return;
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if (player)
		{
			DebugMonitorValues values = player.GetDebugMonitorValues();
			if (values)
			{
				m_debugMonitor.SetHealth(values.GetHealth());
				m_debugMonitor.SetBlood(values.GetBlood());
				m_debugMonitor.SetLastDamage(values.GetLastDamage());
				m_debugMonitor.SetPosition(player.GetPosition());
			}
		}
	}
	
	void SetActionDownTime( int time_down )
	{
		m_ActionDownTime = time_down;
	}
	
	void SetActionUpTime( int time_up )
	{
		m_ActionUpTime = time_up;
	}
	
	int GetActionDownTime()
	{
		return m_ActionDownTime;
	}
	
	int GetActionUpTime()
	{
		return m_ActionUpTime;
	}
	
	float GetHoldActionTime()
	{
		float hold_action_time = GetActionUpTime() - GetActionDownTime();
		return hold_action_time;
	}
}
