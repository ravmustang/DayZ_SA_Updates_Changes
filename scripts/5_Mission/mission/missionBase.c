class NotificationMessage
{
	protected const float FADE_TIMEOUT = 5;
	protected const float FADE_OUT_DURATION = 1;
	protected const float FADE_IN_DURATION = 0.5;

	protected Widget				m_NotificationWidget;

	protected TextWidget			m_NotificationLabel;
	protected TextWidget			m_NotificationText;

	protected ref WidgetFadeTimer	m_FadeTimer;
	protected ref Timer				m_TimeoutTimer;

	void NotificationMessage( Widget root_widget )
	{
		m_NotificationWidget	= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/notification_info.layout", (Widget)root_widget.FindAnyWidget( "NotificationFrameWidget" ) );
		m_NotificationLabel		= TextWidget.Cast( root_widget.FindAnyWidget( "NoticiationLabellTextWidget" ) );
		m_NotificationText		= TextWidget.Cast( root_widget.FindAnyWidget( "NotificationMultilineTextWidget" ) );
		m_NotificationWidget.Show(false);

		m_FadeTimer				= new WidgetFadeTimer;
		m_TimeoutTimer			= new Timer(CALL_CATEGORY_GUI);
	}

	void ~NotificationMessage()
	{
		/*if (m_NotificationWidget) 
		{
			m_NotificationWidget.Destroy();
			m_NotificationWidget = NULL;
		}*/
	}

	void Show( string label, string text, int priority )
	{
		// disabled for now
		// waiting for new message scheduler at application side
		//   (ask [rennerale] for details if really needed)

		bool do_show = false;
		/*
		switch( priority )
		{
		case EVENT_PRIORITY_ERROR:
			m_NotificationLabel.SetColor( COLOR_RED );
			m_NotificationText.SetColor( COLOR_RED );
			break;
		case EVENT_PRIORITY_WARNING:
			m_NotificationLabel.SetColor( COLOR_YELLOW );
			m_NotificationText.SetColor( COLOR_YELLOW );
			break;
		case EVENT_PRIORITY_INFO:
			m_NotificationLabel.SetColor( COLOR_GREEN );
			m_NotificationText.SetColor( COLOR_GREEN );
			break;
		 default:
			do_show = false;
			// log about bad data here ...
			break;
		}
		*/
		if ( do_show )
		{
			if ( label != "" )
			{
				m_NotificationLabel.SetText( label );
			}
			if ( text != "" )	
			{
				m_NotificationText.SetText(text);
			}
			m_FadeTimer.FadeIn( m_NotificationWidget, FADE_IN_DURATION);
			m_TimeoutTimer.Run(FADE_TIMEOUT, m_FadeTimer, "FadeOut", new Param2<Widget, float>(m_NotificationWidget, FADE_OUT_DURATION));
		}
	}
	
	void Hide()
	{
		m_NotificationWidget.Show(false);
		m_TimeoutTimer.Stop();
		m_FadeTimer.Stop();
	}
}

class MissionBase extends Mission
{
	PluginDeveloper 		m_ModuleDeveloper;
	PluginKeyBinding		m_ModuleKeyBinding
	PluginAdditionalInfo	m_ModuleServerInfo;
	
	ref NotificationMessage m_NotificationWidget;
	ref WidgetEventHandler 	m_WidgetEventHandler;
	ref WorldData			m_WorldData;
	
	ref array<PlayerBase> m_DummyPlayers = new array<PlayerBase>;
	
	void MissionBase()
	{
		SetDispatcher(new DispatcherCaller);
		
		PluginManagerInit();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.CheckNotification, 15000, true);

		m_WidgetEventHandler = new WidgetEventHandler;
		
		//Debug.DestroyAllShapes();

		//TODO clea up after Gamescom
		m_ModuleServerInfo = PluginAdditionalInfo.Cast( GetPlugin(PluginAdditionalInfo) );
		//
		SoundSetMap.Init();

		if (GetGame().IsServer())
		{
			m_WorldData = new WorldData;
		}
	}

	void ~MissionBase()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.CheckNotification);
		PluginManagerDelete();
	}
	
	override WorldData GetWorldData()
	{
		return m_WorldData;
	}
	
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		UIScriptedMenu menu = NULL;

		switch (id)
		{
		case MENU_MAIN:
			menu = new MainMenu;
			break;
		case MENU_INGAME:
#ifdef PLATFORM_CONSOLE
			menu = new InGameMenuXbox;
#else
			menu = new InGameMenu;
#endif
			break;
		case MENU_CHARACTER:
			menu = new CharacterCreationMenu;
			break;
		case MENU_OPTIONS:
			menu = new OptionsMenu;
			break;
		case MENU_ASSIGNMENT:
			menu = new AssignmentMenu;
			break;
		case MENU_STARTUP:
			menu = new StartupMenu;
			break;
		case MENU_LOADING:
			menu = new LoadingMenu;
			break;
		case MENU_INVENTORY:
			menu = new InventoryMenu;
			break;
		case MENU_INSPECT:
			menu = new InspectMenuNew;
			break;
		case MENU_EARLYACCESS:
			menu = new EarlyAccessMenu;
			break;
		case MENU_SCRIPTCONSOLE:
			menu = new ScriptConsole;
			break;
		case MENU_SCRIPTCONSOLE_DIALOG_PRESET_NAME:
			menu = new ScriptConsoleNewPresetDialog;
			break;
		case MENU_SCRIPTCONSOLE_DIALOG_PRESET_RENAME:
			menu = new ScriptConsoleRenamePresetDialog;
			break;
		case MENU_CHAT_INPUT:
			menu = new ChatInputMenu;
			break;
		case MENU_CONTROLS_PRESET:
			menu = new PresetsMenu;
			break;
		case MENU_NOTE:
			menu = new NoteMenu;
			break;
		case MENU_MAP:
			menu = new MapMenu;
			break;
		case MENU_BOOK:
			menu = new BookMenu;
			break;
		case MENU_SCENE_EDITOR:
			menu = new SceneEditorMenu;
			break;
		case MENU_HELP_SCREEN:
			menu = new HelpScreen;
			break;
		case MENU_GESTURES:
			menu = new GesturesMenu;
			break;	
		case MENU_LOGOUT:
			menu = new LogoutMenu;
			break;
		case MENU_TITLE_SCREEN:
			menu = new TitleScreenMenu;
			break;
		case MENU_XBOX_CONTROLS:
			menu = new ControlsXbox;
			break;
		case MENU_RADIAL_QUICKBAR:
			menu = new RadialQuickbarMenu;
			break;			
		case MENU_SERVER_BROWSER:
			menu = new ServerBrowserMenuNew;
			break;
		case MENU_LOGIN_QUEUE:
			menu = new LoginQueueMenu;
			break;
		case MENU_CAMERA_TOOLS:
			menu = new CameraToolsMenu;
			break;
		case MENU_VIDEO:
			menu = new MainMenuVideo;
			break;
		case MENU_KEYBINDINGS:
			menu = new KeybindingsMenu;
			break;
		case MENU_TUTORIAL:
			menu = new TutorialsMenu;
			break;
		}

		if (menu)
		{
			menu.SetID(id);
		}

		return menu;
	}

	override UIScriptedWindow CreateScriptedWindow(int id)
	{
		UIScriptedWindow window = NULL;

		switch (id)
		{
		case GUI_WINDOW_MISSION_LOADER:
			window = new MissionLoader( GUI_WINDOW_MISSION_LOADER );
			break;
		}

		return window;
	}

	void SpawnItems()
	{
		/*vector player_pos = "2558 16 2854";
		if (g_Game.GetPlayer())
		{
			g_Game.GetPlayer().GetPosition();
		}

		Print(player_pos);
		
		Print("Spawning items.");
		ref TStringArray items = new TStringArray;
		items.Insert("Hoodie_Blue");
		items.Insert("WoolCoat_Red"); 
		items.Insert("Raincoat_Orange"); 
		items.Insert("PressVest_Blue"); 
		items.Insert("Gorka_pants_summer"); 
		items.Insert("MilitaryBoots_Black");
		items.Insert("WoodAxe");
		items.Insert("Container_FirstAidKit");
		items.Insert("Consumable_DuctTape");
		items.Insert("Fruit_AppleFresh");
		items.Insert("ItemBookHunger");
		
		for (int i = 0; i < items.Count(); i++)
		{
			string item = items.Get(i);
			vector item_pos = player_pos;
			int x = i % 4;
			int z = i / 4;
			item_pos[0] = item_pos[0] + (x * 1);
			item_pos[2] = item_pos[2] + (z * 1);
			g_Game.CreateObject(item, item_pos, false);
		}*/
	}

	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		
		if ( GetGame().IsDebug() )
		{
			if ( PluginKeyBinding.instance != NULL )
			{
				PluginKeyBinding.instance.OnKeyPress(key);
			}
		}
	}

	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);
		
		if ( GetGame().IsDebug() )
		{
			if ( PluginKeyBinding.instance != NULL )
			{
				PluginKeyBinding.instance.OnKeyRelease(key);
			}
		}
	}
	
	override void OnMouseButtonPress(int button)
	{
		super.OnMouseButtonPress(button);
		
		if ( GetGame().IsDebug() )
		{
			if ( PluginKeyBinding.instance != NULL )
			{
				PluginKeyBinding.instance.OnMouseButtonPress(button);
			}
		}
	}
	
	override void OnMouseButtonRelease(int button)
	{
		super.OnMouseButtonRelease(button);
		
		if ( GetGame().IsDebug() )
		{
			if ( PluginKeyBinding.instance != NULL )
			{
				PluginKeyBinding.instance.OnMouseButtonRelease(button);
			}
		}
	}

	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);
		
		/*
		switch(eventTypeId)
		{
		case ScriptLogEventTypeID:
			if ( GetGame().IsDebug() )
			{
				Param1<string> log_params = params;
				OnLog(log_params.param1);
			}
			
			break;
		}
		*/
	}

	void OnLog(string msg_log)
	{
		if ( PluginDeveloper.GetInstance() )
		{
			if ( GetGame().IsServer() && !GetGame().IsMultiplayer() )
			{
				PluginDeveloper.GetInstance().PrintLogClient(msg_log);
			}
			else if ( GetGame().IsMultiplayer() && GetGame().IsClient() )
			{
				PluginDeveloper.GetInstance().PrintLogClient(msg_log);
			}
		}
	}
	
	void CheckNotification()
	{
		// disabled for now
		// waiting for new message scheduler at application side
		//   (ask [rennerale] for details if really needed)
		/*
		if ( GetGame().CheckHiveEvents() )
		{
			string notification_message;
			int notification_value;
			int notification_priority
			GetGame().GetLastHiveEvent( notification_message, notification_value, notification_priority );
			notification_message = String( notification_message + " " + notification_value.ToString() + " " + notification_priority.ToString() );
			ShowNotification( "NOTIFICATION", notification_message, notification_priority );  //uncomment to test notification widget
		}
		*/
	}

	void ShowNotification( string label, string text, int priority)
	{
		if ( m_NotificationWidget )
		{
			m_NotificationWidget.Show( label, text, priority );
		}
	}
	
		
	void UpdateDummyScheduler()
	{
		for(int i = 0; i < m_DummyPlayers.Count(); i++)
		{
			if (m_DummyPlayers.Get(i))
			{
				m_DummyPlayers.Get(i).OnTick();
			}
		}
	}
	
	override void AddDummyPlayerToScheduler(Man player)
	{
		m_DummyPlayers.Insert(PlayerBase.Cast( player ));
	}
}

class MissionDummy extends MissionBase
{
	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		if (key == KeyCode.KC_Q)
		{
			// PlayerBase player = GetGame().GetPlayer();
		}
	}
}
