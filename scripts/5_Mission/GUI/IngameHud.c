class IngameHud extends Hud
{
	protected const float						FADE_IN_TIME = 0.3;
	protected const float						FADE_OUT_TIME = 0.3;
	protected const float						HIDE_MENU_TIME = 5;

	protected ref map<int,string>				m_StatesWidgetNames;
	protected ref map<int,ImageWidget>			m_StatesWidgets;  // [key] ImageWidget

	protected ref map<ImageWidget, int>			m_TendencyStatusCritical;	//array of icons that are blinking due to critical tendency status
	protected const float						TENDENCY_BLINK_TIME = 0.25;
	
	protected ref map<int,string>				m_BadgesWidgetNames;
	protected ref map<int,bool>					m_BadgesWidgetDisplay;
	protected ref map<int,ImageWidget>			m_BadgesWidgets;  // [key] ImageWidget

	protected ref map<int,string>				m_VehicleGearTable;

	protected Widget							m_HudPanelWidget;
	protected Widget							m_QuickbarWidget;
	protected ref InventoryQuickbar				m_Quickbar;
	
	protected Widget							m_VehiclePanel;
	protected Widget							m_VehiclePanelRPMPointer;
	
	protected ImageWidget						m_VehiclePanelBatteryIcon;
	protected ImageWidget						m_VehiclePanelLiquidIcon;
	protected ImageWidget						m_VehiclePanelFuelIcon;
	protected ImageWidget						m_VehiclePanelFuel2Icon;
	protected ProgressBarWidget					m_VehiclePanelBatteryMeter;
	protected ProgressBarWidget					m_VehiclePanelLiquidMeter;
	protected ProgressBarWidget					m_VehiclePanelFuelMeter;
	protected ProgressBarWidget					m_VehiclePanelFuel2Meter;
	
	protected TextWidget						m_VehiclePanelRPMValue;
	protected TextWidget						m_VehiclePanelSpeedValue;
	protected TextWidget						m_VehiclePanelCurrentGearValue;
	protected TextWidget						m_VehiclePanelNextGearValue;
	protected TextWidget						m_VehiclePanelPrevGearValue;
	protected ImageWidget						m_VehiclePanelEngineHealth;
	protected bool								m_InVehicleAsDriver;
	protected CarScript							m_CurrentVehicle;
	
	protected Widget							m_Notifiers;
	protected Widget							m_Badges;
	protected ref Timer							m_HideTimer;
	protected ref WidgetFadeTimer				m_FadeTimerCrosshair;
	protected ref WidgetFadeTimer				m_FadeTimerActionCursor;
//	protected ref WidgetFadeTimer				m_FadeTimerWeaponMode;
//	protected ref WidgetFadeTimer				m_FadeTimerZeroing;
	protected ref WidgetFadeTimer				m_FadeTimerZeroingOld;
	protected ref WidgetFadeTimer				m_FadeTimerMagazineName;
	protected ref WidgetFadeTimer				m_FadeTimerWeaponName;
	protected ref WidgetFadeTimer				m_FadeTimerWeaponStats;
	protected ref WidgetFadeTimer				m_FadeTimerQuickbar;
	protected ref WidgetFadeTimer				m_FadeTimerWalkieTalkie;
	protected ref WidgetFadeTimer				m_FadeTimerWalkieTalkieText;
	
	protected ProgressBarWidget					m_Stamina;
	protected Widget							m_StaminaBackground;
	protected Widget							m_SpecializationPanel;
	protected Widget							m_SpecializationIcon;
	protected Widget							m_Presence;
	protected Widget							m_StanceProne;
	protected Widget							m_StanceStand;
	protected Widget							m_StanceCrouch;
	protected Widget							m_StanceCar;
	protected Widget							m_StancePanel;
	protected Widget							m_PresenceLevel0;
	protected Widget							m_PresenceLevel1;
	protected Widget							m_PresenceLevel2;
	protected Widget							m_PresenceLevel3;
	protected Widget							m_PresenceLevel4;
//	protected TextWidget m_Zeroing;
//	protected Widget m_WeaponStats;
//	protected TextWidget m_WeaponMode;
//	protected TextWidget m_ZeroingOld;
//	protected TextWidget m_WeaponName;
//	protected TextWidget m_MagazineName;
	protected ref Timer							myTimer; //TEMP
	protected ref array<ref WidgetFadeTimer>	m_FadeTimers = new array<ref WidgetFadeTimer>;
	protected Widget							m_MouseSingleAction;
	protected Widget							m_MouseContinuousAction;
	protected Widget							m_RadialProgressBar1;
	protected Widget							m_RadialProgressBarCrossHair;
	protected Widget							m_RadialProgressBar;
	protected Widget							m_CursorWidget;
	protected ImageWidget						m_CursorIcon;
	protected Widget							m_WalkieTalkie;
	
	protected Widget							m_ActionWidget;
	protected TextWidget						m_ActionDesc;
	protected TextWidget						m_ActionItemDesc;
	protected Widget							m_ActionItemQuantityWrapper
	protected ImageWidget						m_ActionHealthMark;
	protected ProgressBarWidget					m_ActionQuantityBar;
	protected Widget							m_ActionQuantityBox;
	protected TextWidget						m_ActionQuantityBoxText;
	protected ImageWidget						m_ActionIcon;
	protected TextWidget						m_ActionButtonText;

	protected TextWidget						m_BloodType;
	protected Widget							m_ActionIconFrame;
	protected Widget							m_ActionMultipleItemsFrame;
	
	protected Widget							m_ActionTarget;
	
	// CrossHairs
	protected ImageWidget						m_PermanentCrossHair;
	
	protected bool								m_HudState;
	protected bool								m_QuickbarState;
	protected bool								m_Faded;
	protected bool								m_ZeroingKeyPressed;
		
	void IngameHud()
	{
		m_FadeTimerCrosshair			= new WidgetFadeTimer;
		m_FadeTimerActionCursor			= new WidgetFadeTimer;
//		m_FadeTimerWeaponMode = new WidgetFadeTimer;
//		m_FadeTimerZeroing = new WidgetFadeTimer;
//		 m_FadeTimerZeroingOld = new WidgetFadeTimer;
//		 m_FadeTimerMagazineName = new WidgetFadeTimer;
//		 m_FadeTimerWeaponName = new WidgetFadeTimer;
//		 m_FadeTimerWeaponStats = new WidgetFadeTimer;
		 m_FadeTimerQuickbar			= new WidgetFadeTimer;
		 m_FadeTimerWalkieTalkie		= new WidgetFadeTimer;
 		 m_FadeTimerWalkieTalkieText	= new WidgetFadeTimer;
		
		m_HideTimer = new Timer(CALL_CATEGORY_GUI);
		//m_zeroing_and_weaponmode_timer = new Timer( CALL_CATEGORY_GAMEPLAY );

		m_StatesWidgets					= new map<int, ImageWidget>; // [key] widgetName
		m_StatesWidgetNames				= new map<int, string>;
		
		m_TendencyStatusCritical		= new map<ImageWidget, int>;
		
		m_BadgesWidgets					= new map<int, ImageWidget>; // [key] widgetName
		m_BadgesWidgetNames				= new map<int, string>;
		m_BadgesWidgetDisplay			= new map<int, bool>;

		m_VehicleGearTable				= new map<int, string>;
		m_VehicleGearTable.Set( -1, "" );
		m_VehicleGearTable.Set( (int)CarGear.REVERSE, "R" );
		m_VehicleGearTable.Set( (int)CarGear.NEUTRAL, "N" );
		m_VehicleGearTable.Set( (int)CarGear.FIRST, "1" );
		m_VehicleGearTable.Set( (int)CarGear.SECOND, "2" );
		m_VehicleGearTable.Set( (int)CarGear.THIRD, "3" );
		m_VehicleGearTable.Set( (int)CarGear.FOURTH, "4" );
		m_VehicleGearTable.Set( (int)CarGear.FIFTH, "5" );
		m_VehicleGearTable.Set( (int)CarGear.SIXTH, "6" );
		m_VehicleGearTable.Set( (int)CarGear.SEVENTH, "7" );
		m_VehicleGearTable.Set( (int)CarGear.EIGTH, "8" );
	}
	
	override void Init( Widget hud_panel_widget )
	{
		m_HudPanelWidget = hud_panel_widget;
		m_HudPanelWidget.Show( true );

		// quickbar
		m_QuickbarWidget = m_HudPanelWidget.FindAnyWidget("QuickbarGrid");
		m_QuickbarWidget.Show( false );
		
		// TEMPORARY HACK!!! player is not present when Hud is being initialized 
		myTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		myTimer.Run( 1, this, "InitQuickbar" );
		
		m_CursorWidget = m_HudPanelWidget.FindAnyWidget("CursorIcons");
		m_CursorWidget.Show(true);
		Class.CastTo(m_CursorIcon, m_HudPanelWidget.FindAnyWidget("Cursor"));

		// Permanent CrossHairs
		Class.CastTo(m_PermanentCrossHair, m_HudPanelWidget.FindAnyWidget("PermanentCrossHair"));
		
		m_WalkieTalkie = m_HudPanelWidget.FindAnyWidget("WalkieTalkie");
		
		// panels
		Class.CastTo(m_Stamina, m_HudPanelWidget.FindAnyWidget("StaminaBar"));
		m_SpecializationPanel			= m_HudPanelWidget.FindAnyWidget("SpecializationPanel");
		m_SpecializationIcon			= m_HudPanelWidget.FindAnyWidget("SpecializationIcon");
		m_Presence						= m_HudPanelWidget.FindAnyWidget("PresencePanel");
		m_Badges						= hud_panel_widget.FindAnyWidget("BadgesPanel");
		m_Notifiers						= m_HudPanelWidget.FindAnyWidget("NotifiersPanel");
		
		m_VehiclePanel					= m_HudPanelWidget.FindAnyWidget("VehiclePanel");
		m_VehiclePanelRPMPointer		= m_VehiclePanel.FindAnyWidget("RpmPointer");
		m_VehiclePanelRPMValue			= TextWidget.Cast( m_VehiclePanel.FindAnyWidget("RpmLabel") );
		m_VehiclePanelSpeedValue		= TextWidget.Cast( m_VehiclePanel.FindAnyWidget("SpeedValue") );
		m_VehiclePanelCurrentGearValue	= TextWidget.Cast( m_VehiclePanel.FindAnyWidget("Current") );
		m_VehiclePanelNextGearValue		= TextWidget.Cast( m_VehiclePanel.FindAnyWidget("Next") );
		m_VehiclePanelPrevGearValue		= TextWidget.Cast( m_VehiclePanel.FindAnyWidget("Prev") );
		
		m_VehiclePanelBatteryIcon		= ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("IconBattery") );
		m_VehiclePanelLiquidIcon		= ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("IconLiquid") );
		m_VehiclePanelFuelIcon			= ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("IconFuel") );
		m_VehiclePanelFuel2Icon			= ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("IconFuel0") );
		m_VehiclePanelBatteryMeter		= ProgressBarWidget.Cast( m_VehiclePanel.FindAnyWidget("BatteryMeter") );
		m_VehiclePanelLiquidMeter		= ProgressBarWidget.Cast( m_VehiclePanel.FindAnyWidget("LiquidMeter") );
		m_VehiclePanelFuelMeter			= ProgressBarWidget.Cast( m_VehiclePanel.FindAnyWidget("FuelMeter") );
		m_VehiclePanelFuel2Meter		= ProgressBarWidget.Cast( m_VehiclePanel.FindAnyWidget("FuelMeter0") );
		
		m_VehiclePanelEngineHealth		= ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("CheckEngineIcon") );
		
		//Class.CastTo(m_Zeroing, m_HudPanelWidget.FindAnyWidget("Zeroing"));
		//Class.CastTo(m_WeaponMode, m_HudPanelWidget.FindAnyWidget("WeaponMode"));
		//m_WeaponStats = m_HudPanelWidget.FindAnyWidget("WeaponStats");
		//Class.CastTo(m_ZeroingOld, m_HudPanelWidget.FindAnyWidget("ZeroingOld"));
		//Class.CastTo(m_WeaponName, m_HudPanelWidget.FindAnyWidget("WeaponName"));
		//Class.CastTo(m_MagazineName, m_HudPanelWidget.FindAnyWidget("MagazineName"));
		m_StaminaBackground				= m_HudPanelWidget.FindAnyWidget("StaminaBackground");
		m_StaminaBackground.Show(true);
		m_StanceProne					= m_HudPanelWidget.FindAnyWidget("StanceProne");
		m_StanceCrouch					= m_HudPanelWidget.FindAnyWidget("StanceCrouch");
		m_StanceStand					= m_HudPanelWidget.FindAnyWidget("StanceStand");
		m_StanceCar						= m_HudPanelWidget.FindAnyWidget("StanceCar");
		m_StancePanel					= m_HudPanelWidget.FindAnyWidget("StancePanel");
		m_ActionTarget					= m_HudPanelWidget.FindAnyWidget("ActionTargetsCursorWidget");
		Class.CastTo(m_BloodType, m_HudPanelWidget.FindAnyWidget("BloodType") );
		
		// state notifiers
		m_StatesWidgetNames.Clear();
		m_StatesWidgets.Clear();
		m_StatesWidgetNames.Set( NTFKEY_THIRSTY, "Thirsty" );
		m_StatesWidgetNames.Set( NTFKEY_HUNGRY, "Hungry" );
		m_StatesWidgetNames.Set( NTFKEY_SICK, "Health" );
		m_StatesWidgetNames.Set( NTFKEY_BACTERIA, "Bacteria" );
		m_StatesWidgetNames.Set( NTFKEY_BLEEDISH, "Blood" );
		m_StatesWidgetNames.Set( NTFKEY_FEVERISH, "Temperature" );

		#ifndef NO_GUI
			m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_Timer.Run(0.05, this, "RefreshQuickbar", NULL, true );
			//m_Timer.Run(1, this, "CheckHudElementsVisibility", NULL, true ); //modify duration if needed, currently on 1s "update"
		#endif

			m_Notifiers.Show( true );
			// m_Notifiers.SetAlpha( 0 );
			m_Badges.Show( true );
			// m_Badges.SetAlpha( 0 );

			int i = 0;
			int key = 0;
			for ( i = 0; i < m_StatesWidgetNames.Count(); i++ )
			{
				string widget_name = m_StatesWidgetNames.GetElement(i);
				key = m_StatesWidgetNames.GetKey(i);
				ImageWidget w;
				Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( "Icon" + widget_name ) ) );
				m_StatesWidgets.Set( key, w );
				w.Show( true );
				// clear all arrows
				for ( int x = 1; x < 4; x++ )
				{
					Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( widget_name + "ArrowUp" + x.ToString() ) ) );
					w.Show( false );
					Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( widget_name + "ArrowDown" + x.ToString() ) ) );
					w.Show( false );
				}
			}

			// badges
			m_BadgesWidgetNames.Clear();
			m_BadgesWidgets.Clear();
			m_BadgesWidgetDisplay.Clear();
			m_BadgesWidgetNames.Set( NTFKEY_FRACTURE, "Fracture" );
			m_BadgesWidgetNames.Set( NTFKEY_STUFFED, "Stomach" );
			m_BadgesWidgetNames.Set( NTFKEY_SICK, "Pill" );
			m_BadgesWidgetNames.Set( NTFKEY_WETNESS, "Wetness" );
			m_BadgesWidgetNames.Set( NTFKEY_FEVERISH, "Skull" );
			// NTFKEY_SICK
			// NTFKEY_BLEEDISH
			// NTFKEY_FRACTURE
			// NTFKEY_STUFFED
			// NTFKEY_WETNESS iconDrops

			for ( i = 0; i < m_BadgesWidgetNames.Count(); i++ )
			{
				string badge_name = m_BadgesWidgetNames.GetElement(  i);
				key = m_BadgesWidgetNames.GetKey( i );
				ImageWidget badge_widget;
				Class.CastTo(badge_widget,  m_Badges.FindAnyWidget( badge_name ) );
				m_BadgesWidgets.Set( key, badge_widget );
				badge_widget.Show( false );
				m_BadgesWidgetDisplay.Set( key, false );
			}

			m_PresenceLevel0 = hud_panel_widget.FindAnyWidget("Presence0");
			m_PresenceLevel1 = hud_panel_widget.FindAnyWidget("Presence1");
			m_PresenceLevel2 = hud_panel_widget.FindAnyWidget("Presence2");
			m_PresenceLevel3 = hud_panel_widget.FindAnyWidget("Presence3");
			m_PresenceLevel4 = hud_panel_widget.FindAnyWidget("Presence4");
			m_PresenceLevel0.Show( false );
			m_PresenceLevel1.Show( false );
			m_PresenceLevel2.Show( false );
			m_PresenceLevel3.Show( false );
			m_PresenceLevel4.Show( false );
		
		/*
		else
		{
			m_StaminaBackground.Show( false );
			m_Stamina.Show( false );
			m_Presence.Show( false );
			m_StanceProne.Show( false );
			m_StanceStand.Show( false );
			m_StanceCrouch.Show( false );
			m_Badges.Show( false );
			m_Notifiers.Show( false );
		}
		*/
		
//		#ifndef NO_GUI
//		m_zeroing_and_weaponmode_timer.Run(0.1, this, "RefreshZeroingAndWeaponMode", NULL, true );
//		#endif
		
		SetLeftStatsVisibility( true );
		ToggleHud( g_Game.GetProfileOption( EDayZProfilesOptions.HUD ) );
		ToggleQuickBar( g_Game.GetProfileOption( EDayZProfilesOptions.QUICKBAR ) );
	}
	
	override void OnResizeScreen()
	{
		float x, y;
		m_HudPanelWidget.GetScreenSize( x, y );
		Print( "m_HudPanelWidget: " + x.ToString() + "x" + y.ToString() );
		m_HudPanelWidget.Update();
		m_Badges.Update();
		m_Notifiers.SetPos( 0, 0 );
	}
	
	override bool IsXboxDebugCursorEnabled()
	{
		#ifdef DEVELOPER
			PluginDiagMenu plugin_diag_menu = PluginDiagMenu.Cast( GetPlugin(PluginDiagMenu) );
			return plugin_diag_menu.GetXboxCursor();
		#else
			return false;
		#endif
	}

	override void Show( bool show )
	{
		m_HudPanelWidget.Show( show );
	}

	override void ShowWalkieTalkie( bool show )
	{
		m_FadeTimerWalkieTalkie.Stop();
		m_FadeTimerWalkieTalkieText.Stop();
		m_WalkieTalkie.Show( show );
	}
	
	override void ShowWalkieTalkie( int fadeOutSeconds )
	{
		m_WalkieTalkie.Show(true);
		m_FadeTimerWalkieTalkie.Stop();
		m_FadeTimerWalkieTalkieText.Stop();
		m_FadeTimerWalkieTalkie.FadeOut( m_WalkieTalkie.FindAnyWidget("Icon") , fadeOutSeconds );
		m_FadeTimerWalkieTalkieText.FadeOut( m_WalkieTalkie.FindAnyWidget("Text") , fadeOutSeconds );
	}
	
	override void SetWalkieTalkieText( string text )
	{
		TextWidget txt;
		Class.CastTo(txt, m_WalkieTalkie.FindAnyWidget("Text"));
		txt.SetText(text);
	}
	
	override void SetCursorIcon( string icon )
	{
		if ( icon.Length() == 0 || icon == CursorIcons.None )
			m_CursorIcon.Show(false);
		else
		{
			m_CursorIcon.LoadImageFile( 0, icon );
			m_CursorIcon.Show(true);
		}
	}

	override void ShowCursor()
	{
		m_CursorWidget.Show( true );
	}

	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		InventoryMenu inventory;
		MissionGameplay mission;

		UIManager manager = GetGame().GetUIManager();
		Class.CastTo(inventory, manager.FindMenu( MENU_INVENTORY ));
		Class.CastTo(mission, GetGame().GetMission());
		if(	mission )
		{
			inventory = mission.GetInventory();
		}
		
		if( inventory )
		{
			inventory.RefreshQuantity( item_to_refresh );
		}
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		InventoryMenu inventory;
		MissionGameplay mission;

		UIManager manager = GetGame().GetUIManager();
		Class.CastTo(inventory, manager.FindMenu( MENU_INVENTORY ));
		Class.CastTo(mission, GetGame().GetMission());
		if(	mission )
		{
			inventory = mission.GetInventory();
		}
		
		if( inventory )
		{
			inventory.RefreshItemPosition( item_to_refresh );
		}
	}
	
	override void HideCursor()
	{
		m_CursorWidget.Show( false );
	}
	
	override void SetCursorIconScale( string type, float percentage )
	{
		float x, y;
		m_CursorIcon.FindAnyWidget( type ).GetParent().GetSize( x, y );
		m_CursorIcon.FindAnyWidget( type ).GetParent().SetSize( x * percentage, y * percentage );
	}
	
	override void SetCursorIconOffset( string type, float x, float y )
	{
		m_CursorIcon.FindAnyWidget( type ).GetParent().SetPos( x, y );
	}
	
	override void SetCursorIconSize( string type, float x, float y )
	{
		m_CursorIcon.FindAnyWidget( type ).GetParent().SetSize( x, y );
	}

#ifdef DEVELOPER
	// Debug Crosshair
	override void SetPermanentCrossHair( bool show )
	{
		m_PermanentCrossHair.Show(show);
	}
#endif
	
	override void DisplayNotifier( int key, int tendency, int status )
	{
		ImageWidget w;
		Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( "Icon" + m_StatesWidgetNames.Get( key ) ) ) );
		w.Show( true );

		// --- tendency status
		//1 - white (fine) - also default
		//2 - yellow (attention)
		//3 - red (alert)
		//4 - red blinking (critical)
		//5 - turquoise
		//6 - blue
		//7 - blue blinking (critical)
		float alpha = w.GetAlpha();

		switch( status )
		{
			case 1:
				w.SetColor( ARGB( alpha * 255, 255, 255, 255 ) );	//white
				m_TendencyStatusCritical.Remove( w );		//remove from blinking group
				break;
			case 2:
				w.SetColor( ARGB( alpha * 255, 255, 255, 0 ) );		//yellow
				m_TendencyStatusCritical.Remove( w );		//remove from blinking group
				break;
			case 3:
				w.SetColor( ARGB( alpha * 255, 255, 0, 0 ) );		//red
				m_TendencyStatusCritical.Remove( w );		//remove from blinking group
				break;
			case 4:
				if ( !m_TendencyStatusCritical.Contains( w ) )
				{
					m_TendencyStatusCritical.Insert( w, ARGB( alpha * 255, 255, 0, 0 ) );	//add to blinking group
				}
				break;
			case 5:
				w.SetColor( ARGB( alpha * 255, 0, 206, 209 ) );		//turquoise
				m_TendencyStatusCritical.Remove( w );		//remove from blinking group
				break;
			case 6:
				w.SetColor( ARGB( alpha * 255, 30, 144, 255 ) );	//blue
				m_TendencyStatusCritical.Remove( w );		//remove from blinking group
				break;
			case 7:													//blue blinking
				if ( !m_TendencyStatusCritical.Contains( w ) )
				{
					m_TendencyStatusCritical.Insert( w, ARGB( alpha * 255, 30, 144, 255 ) );	//add to blinking group
				}
				break;				
			default:
				w.SetColor( ARGB( alpha * 255, 255, 255, 255 ) );	//white
				m_TendencyStatusCritical.Remove( w );		//remove from blinking group
				break;
		}
		// ---
		
		// tendency arrows
		string arrow_name = "ArrowUp";
		if ( tendency < 0 )
		{
			arrow_name = "ArrowDown";
		}
		tendency = Math.AbsInt( tendency );

		for ( int x = 1; x < 4; x++ )
		{ 
			Class.CastTo(w,  m_Notifiers.FindAnyWidget( String(  m_StatesWidgetNames.Get( key ) + "ArrowUp" + x.ToString() ) ) );
			w.Show( false );
			Class.CastTo(w,  m_Notifiers.FindAnyWidget( String(  m_StatesWidgetNames.Get( key ) + "ArrowDown" + x.ToString() ) ) );
			w.Show( false );
		}

		for ( int i = 1; i < ( tendency + 1) ; i++ )
		{
			string widget_name = m_StatesWidgetNames.Get( key ) + arrow_name + i.ToString() ;
			Class.CastTo(w,  m_Notifiers.FindAnyWidget( widget_name ) );
			w.Show( true );
		}
	}
	
	override void DisplayBadge( int key, bool show )
	{
		m_BadgesWidgetDisplay.Set( key, show );

		int x = 0;
		for ( int i = 0; i < m_BadgesWidgetDisplay.Count(); i++ )
		{
			int badge_key = m_BadgesWidgetDisplay.GetKey( i );
			string badge_name = m_BadgesWidgetNames.Get( badge_key );
			ImageWidget badge_widget
			Class.CastTo(badge_widget,  m_Badges.FindAnyWidget( badge_name ) );
			if ( badge_widget )
			{
				if ( m_BadgesWidgetDisplay.Get( badge_key ) == true )
				{
					badge_widget.SetPos ( x*0.2, 0.0, true);
					badge_widget.Show( true );
					x = x + 1;
				}
				else
				{
					badge_widget.Show( false );
				}
			}
		}
	}
	
	override void SetStamina( int value , int range )
	{
		//PlayerBase player = GetGame().GetPlayer();
		/*if( !player.m_PlayerStats )
		{
			return;
		}*/
		float sx, sy;
		float max = STAMINA_MAX;
		float percentage =  range / max;
		m_Stamina.SetCurrent( ( value / range ) * 100 );
		m_Stamina.GetSize( sx, sy );
		m_Stamina.SetSize( percentage, sy );
 		m_StaminaBackground.SetSize( 1-percentage, sy);
	}

/*	
	void RefreshZeroingAndWeaponModeOld()
	{
		PlayerBase player;
		Class.CastTo(player,  GetGame().GetPlayer() );
		
		if ( player != NULL )
		{
			EntityAI entity = player.GetHumanInventory().GetEntityInHands();
			
			if ( entity != NULL && entity.IsWeapon() )
			{	
				float zeroing = player.GetCurrentZeroing();
				
				if( m_ZeroingKeyPressed )
				{
					m_ZeroingKeyPressed = false;
					m_Faded = false;
				}
				
				if( !m_Faded )
				{
					m_FadeTimerZeroingOld.FadeOut( m_WeaponStats, 5 );
					m_FadeTimerMagazineName.FadeOut( m_WeaponName, 5 );
					m_FadeTimerWeaponName.FadeOut( m_MagazineName, 5 );
					m_FadeTimerWeaponStats.FadeOut( m_ZeroingOld, 5 );
					m_Faded = true;
				}
				
				if( entity.GetInventory().AttachmentCount() == 0 )
				{
					m_MagazineName.SetText( "" );
				}
				
				for ( int i = 0; i < entity.GetInventory().AttachmentCount(); i++ )
				{
					EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex( i );
					if ( !attachment ) continue;
					
					if ( attachment.IsMagazine() )
					{
						m_MagazineName.SetText( attachment.GetDisplayName() );
					}
					else
					{
						m_MagazineName.SetText("");
					}
				}
				
				m_WeaponName.SetText( entity.GetDisplayName() );
				m_WeaponStats.Show( true );
				m_ZeroingOld.SetText( zeroing.ToString() );
			}
			else
			{
				m_Faded = false;
				m_WeaponStats.Show( false );
			}
		}
	}
	
	void RefreshZeroingAndWeaponMode()
	{
		if( !m_FadeTimerWeaponMode || !m_FadeTimerZeroing || !m_WeaponMode || !m_Zeroing )
			return;
			
		PlayerBase player;
		Class.CastTo(player,  GetGame().GetPlayer() );
		
		if ( player != NULL )
		{
			EntityAI entity = player.GetHumanInventory().GetEntityInHands();
			
			if ( entity != NULL && entity.IsWeapon() )
			{	
				float zeroing = player.GetCurrentZeroing();
				string weaponMode = player.GetCurrentWeaponMode();
				
				if( m_ZeroingKeyPressed )
				{
					m_ZeroingKeyPressed = false;
					m_Faded = false;
				}
				
				if( !m_Faded )
				{
					m_FadeTimerWeaponMode.FadeOut(m_WeaponMode, 5);
					m_FadeTimerZeroing.FadeOut(m_Zeroing, 5);
					m_Faded = true;
				}
				
				m_Zeroing.Show( true );
				m_Zeroing.SetText( zeroing.ToString() );
				m_WeaponMode.Show( true );
				m_WeaponMode.SetText( weaponMode );
			}
			else
			{
				m_Faded = false;
				m_WeaponMode.Show( false );
				m_Zeroing.Show( false );
			}
		}
	}
*/

	bool KeyPress(int key)
	{
		return false;
	}
	
	void ZeroingKeyPress()
	{
		m_ZeroingKeyPressed = true;
	}
	
	override void InitInventory()
	{
		UIManager manager = GetGame().GetUIManager();
		InventoryMenu inventory = InventoryMenu.Cast(manager.FindMenu( MENU_INVENTORY ));
		MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());

		if(	mission )
		{
			mission.InitInventory();
		}
	}
	
	override void DisplayStance( int stance )
	{
		PlayerBase player;
		Class.CastTo(player,  GetGame().GetPlayer() );
		Car car;
		if ( !Class.CastTo(car, player.GetDrivingVehicle()) )
		{
			if( stance == 1 )
			{
				m_StanceStand.Show(true);
				m_StanceCrouch.Show(false);
				m_StanceProne.Show(false);
				m_StanceCar.Show( false );
			}
			if( stance == 2 )
			{
				m_StanceStand.Show(false);
				m_StanceCrouch.Show(true);
				m_StanceProne.Show(false);
				m_StanceCar.Show( false );
			}
			if( stance == 3 )
			{
				m_StanceStand.Show(false);
				m_StanceCrouch.Show(false);
				m_StanceProne.Show(true);
				m_StanceCar.Show( false );
			}
		}
		else
		{
			m_StanceStand.Show(false);
			m_StanceCrouch.Show(false);
			m_StanceProne.Show(false);
			m_StanceCar.Show( true );
		}
	}
	
	override void DisplayPresence()
	{
		PlayerBase player;
		Class.CastTo(player, GetGame().GetPlayer() );
		if ( player != NULL )
		{
			// TODO: rrada
			//float presence_level = player.GetPresenceInAI();
			float presence_level = 0; // tmp until we have working the GetPresenceInAI() method above
			m_PresenceLevel0.Show( false );
			m_PresenceLevel1.Show( false );
			m_PresenceLevel2.Show( false );
			m_PresenceLevel3.Show( false );
			m_PresenceLevel4.Show( false );

			if ( presence_level > 0 )
			{
				m_PresenceLevel0.Show( true );
			}
			if ( presence_level > 0.2 )
			{
				m_PresenceLevel1.Show( true );
			}
			if ( presence_level > 0.4 )
			{
				m_PresenceLevel2.Show( true );
			}
			if ( presence_level > 0.6 )
			{
				m_PresenceLevel3.Show( true );
			}
			if ( presence_level > 0.8 )
			{
				m_PresenceLevel4.Show( true );
			}
		}
	}
	
	int		m_VehicleGearCount = -1;
	int		m_VehicleRPMMax = 1;
	float	m_TimeSinceLastEngineLightChange;
	bool	m_VehicleHasOil;
	bool	m_VehicleHasCoolant;
	
	override void ShowVehicleInfo()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( player )
		{
			HumanCommandVehicle hcv = player.GetCommand_Vehicle();
			
			if ( hcv && hcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				CarScript car = CarScript.Cast( hcv.GetTransport() );
				if( car )
				{
					m_InVehicleAsDriver	= true;
					m_CurrentVehicle	= car;
					
					TFloatArray gears	= new TFloatArray;
					
					GetGame().ConfigGetFloatArray( "CfgVehicles " + m_CurrentVehicle.GetType() + " SimulationModule Gearbox ratios" , gears );
					m_VehicleRPMMax = GetGame().ConfigGetFloat( "CfgVehicles " + m_CurrentVehicle.GetType() + " SimulationModule Engine rpmRedline" );
					
					m_VehicleGearCount	= gears.Count() + 1;
					
					//m_VehicleHasOil		= GetGame().ConfigGetFloat( "CfgVehicles " + m_CurrentVehicle.GetType() + " SimulationModule Engine rpmRedline" );
					//m_VehicleHasCoolant	= GetGame().ConfigGetFloat( "CfgVehicles " + m_CurrentVehicle.GetType() + " SimulationModule Engine rpmRedline" );
					
					if( !m_VehicleHasOil )
					{
						m_VehiclePanel.FindAnyWidget( "Battery" ).Show( false );
					}
					
					if( !m_VehicleHasCoolant )
					{
						m_VehiclePanel.FindAnyWidget( "Liquid" ).Show( false );
					}
					
					m_HudPanelWidget.FindAnyWidget("PlayerPanel").Show( false );
					m_Presence.Show( false );
					m_StancePanel.Show( false );
					
					m_VehiclePanel.Show( true );
				}
			}
		}
	}
	
	override void HideVehicleInfo()
	{
		m_HudPanelWidget.FindAnyWidget("PlayerPanel").Show( true );
		m_Presence.Show( true );
		m_StancePanel.Show( true );
		
		m_VehiclePanel.Show( false );
		
		m_CurrentVehicle = null;
		m_VehicleGearCount = -1;
	}
	
	void RefreshVehicleHud( float timeslice )
	{
		if ( m_CurrentVehicle )
		{
			float rpm_value = ( m_CurrentVehicle.GetEngineRPM() / m_VehicleRPMMax ) ;
			
			m_VehiclePanelRPMPointer.SetRotation( 0, 0, rpm_value * 100 - 20, true );
			m_VehiclePanelSpeedValue.SetText( Math.Floor( m_CurrentVehicle.GetSpeedometer() ).ToString() );

			int engaged_gear = m_CurrentVehicle.GetController().GetGear();
			int prev_gear = engaged_gear - 1;
			int next_gear = engaged_gear + 1;

			if ( engaged_gear == CarGear.NEUTRAL )
			{
				prev_gear = CarGear.REVERSE;
			}
			else if ( engaged_gear == CarGear.REVERSE )
			{
				prev_gear = -1;
				next_gear = CarGear.NEUTRAL;
			}
			
			int health = m_CurrentVehicle.GetHealthLevel();
			int color;
			if( m_CurrentVehicle.IsEngineOn() && health > 0 && health < 4 )
			{
				m_VehiclePanelEngineHealth.Show( true );
				color = ItemManager.GetItemHealthColor( m_CurrentVehicle );
				
				m_VehiclePanelEngineHealth.SetColor( color );
				m_VehiclePanelEngineHealth.SetAlpha( 1 );
			}
			else if( health > 3 )
			{
				if( m_TimeSinceLastEngineLightChange > 0.7 )
				{
					m_VehiclePanelEngineHealth.Show( !m_VehiclePanelEngineHealth.IsVisible() );
					color = ItemManager.GetItemHealthColor( m_CurrentVehicle );
					m_VehiclePanelEngineHealth.SetColor( color );
					m_VehiclePanelEngineHealth.SetAlpha( 1 );
					m_TimeSinceLastEngineLightChange = 0;
				}
				m_TimeSinceLastEngineLightChange += timeslice;
			}
			else
			{
				m_VehiclePanelEngineHealth.Show( false );
			}
			
			m_VehiclePanelCurrentGearValue.SetText( m_VehicleGearTable.Get( engaged_gear ) );
			
			if( next_gear > m_VehicleGearCount )
			{
				m_VehiclePanelNextGearValue.Show( false );
			}
			else
			{
				m_VehiclePanelNextGearValue.Show( true );
			}
			
			
			m_VehiclePanelNextGearValue.SetText( m_VehicleGearTable.Get( next_gear ) );
			m_VehiclePanelPrevGearValue.SetText( m_VehicleGearTable.Get( prev_gear ) );
			
			if( !m_VehicleHasOil )
			{
				m_VehiclePanelBatteryMeter.SetCurrent( m_CurrentVehicle.GetFluidFraction( CarFluid.OIL ) );
				color = ItemManager.ColorFromFloat( m_CurrentVehicle.GetFluidFraction( CarFluid.OIL ) );
				m_VehiclePanelBatteryIcon.SetColor( color );
				m_VehiclePanelFuelIcon.SetAlpha( 1 );
			}
				
			if( !m_VehicleHasCoolant )
			{	
				m_VehiclePanelLiquidMeter.SetCurrent( m_CurrentVehicle.GetFluidFraction( CarFluid.COOLANT ) );
				color = ItemManager.ColorFromFloat( m_CurrentVehicle.GetFluidFraction( CarFluid.COOLANT ) );
				m_VehiclePanelLiquidIcon.SetColor( color );
				m_VehiclePanelFuelIcon.SetAlpha( 1 );
			}
			
			m_VehiclePanelFuelMeter.SetCurrent( m_CurrentVehicle.GetFluidFraction( CarFluid.FUEL ) );
			color = ItemManager.ColorFromFloat( m_CurrentVehicle.GetFluidFraction( CarFluid.FUEL ) );
			m_VehiclePanelFuelIcon.SetColor( color );
			m_VehiclePanelFuelIcon.SetAlpha( 1 );
		}
	}
	
	void InitQuickbar()
	{
		if (m_Quickbar == NULL)
		{
			m_Quickbar = new InventoryQuickbar( m_QuickbarWidget );
			ShowQuickbar();
		}
	}
	
	InventoryQuickbar GetQuickbar()
	{
		return m_Quickbar;
	}
	
	void ShowQuickbar( bool ignore_state = false )
	{
#ifdef PLATFORM_XBOX
		return;
#else
#ifdef PLATFORM_PS4
		return;
#endif
#endif
		
		PlayerBase player;
		Class.CastTo(player,  GetGame().GetPlayer() );
		if(!player)
		{
			return;
		}

		if ( m_QuickbarWidget && player.GetQuickBarSize() != 0 )
		{
				InventoryGrid quickbarGrid;
				m_QuickbarWidget.GetScript(quickbarGrid);
				Widget child = quickbarGrid.GetRoot().GetChildren();
				float alpha = quickbarGrid.GetRoot().GetAlpha();
				RefreshQuickbar();

				m_FadeTimers.Clear();
				
				m_QuickbarWidget.Show( true );
				if ( !ignore_state )
				{
					m_QuickbarState = true;
				}
				
				while (child)
				{
					child.Show(true);
					child.GetChildren().Show(true);
					child.SetAlpha(alpha);
					child.GetChildren().SetAlpha(1);
					child = child.GetSibling();
				}
		}
	}
	
	void HideQuickbar( bool ignore_state = false , bool instant_hide = false )
	{
		if ( m_QuickbarWidget )
		{
			InventoryGrid quickbarGrid;
			m_QuickbarWidget.GetScript(quickbarGrid);
			
			if( !instant_hide )
			{
				Widget child = quickbarGrid.GetRoot().GetChildren();
				while (child)
				{
					WidgetFadeTimer fade_timer_quickbar = new WidgetFadeTimer;
					fade_timer_quickbar.FadeOut(child, 5, true);
					m_FadeTimers.Insert(fade_timer_quickbar);
					child = child.GetSibling();
				}
			}
			else
			{
				m_QuickbarWidget.Show( false );
			}
			
			if ( !ignore_state )
			{
				m_QuickbarState = false;
			}
		}
	}
	
	void ToggleQuickBar( bool show, bool ignore_state = false )
	{
		if ( show )
		{
			ShowQuickbar( ignore_state );
		}
		else
		{
			HideQuickbar( false, ignore_state );
		}
	}
	
	void ToggleHud( bool show, bool ignore_state = false )
	{
		//You can add more widgets to toggle here
		SetLeftStatsVisibility( show );
		m_Badges.Show( show );
		m_Notifiers.Show( show );
		
		if( !ignore_state )
		{
			m_HudState = show;
			//! save it to profiles
			g_Game.SetProfileOption( EDayZProfilesOptions.HUD, show );
		}
	}
	
	bool GetQuickBarState()
	{
		return m_QuickbarState;
	}
	
	bool GetHudState()
	{
		return m_HudState;
	}
	
	void SetLeftStatsVisibility(bool visible)
	{
		m_StaminaBackground.Show(visible);
		m_Stamina.Show(visible);
		m_StancePanel.Show(visible);
		m_Presence.Show(visible);
	}

	void SetSpecialtyMeterVisibility( bool visible )
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

		if ( player && player.GetSoftSkillManager() )
		{
			m_SpecializationPanel.Show( visible );
			float x = player.GetSoftSkillManager().GetSpecialtyLevel() / 2;
			float y = -0.75;
			m_SpecializationIcon.SetPos( x, y, true );	
		}
	}
	
	void CheckHudElementsVisibility()
	{
		if (m_HudState != g_Game.GetProfileOption( EDayZProfilesOptions.HUD))
		{
			ToggleHud( g_Game.GetProfileOption( EDayZProfilesOptions.HUD ) );
		}
		if (m_QuickbarState != (g_Game.GetProfileOption( EDayZProfilesOptions.QUICKBAR)))
		{
			ToggleQuickBar( g_Game.GetProfileOption( EDayZProfilesOptions.QUICKBAR ) );
		}
	}
	
	override void RefreshQuickbar( bool itemChanged = false )
	{		
		if( itemChanged )
		{
			UIManager manager = GetGame().GetUIManager();
		
			InventoryMenu inventory = InventoryMenu.Cast(manager.FindMenu( MENU_INVENTORY ));
			MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());

			
			if(	mission )
			{
				inventory = mission.GetInventory();
			}
		
			if( inventory )
			{
				inventory.RefreshQuickbar();
			}
		}
	
		if ( m_Quickbar ) 
		{
			m_Quickbar.UpdateItems( m_QuickbarWidget );
		}
	}
	
	Widget GetHudPanelWidget()
	{
		return m_HudPanelWidget;
	}
	
	override void UpdateBloodName()
	{
		PlayerBase player;
		Class.CastTo(player, GetGame().GetPlayer() );
		
		if( player )
		{
			string blood_name = BloodTypes.GetBloodTypeName( player.GetBloodType() );
			bool blood_type_visible = player.HasBloodTypeVisible();
			
			if( blood_type_visible )
			{
				m_BloodType.Show( blood_type_visible );
				m_BloodType.SetText( blood_name );	
			}
			else
			{
				m_BloodType.Show( blood_type_visible );
				m_BloodType.SetText( "" );
			}
		}
	}

	protected int m_LastTime;
	protected float m_BlinkTime;
	override void Update( float timeslice )
	{
		super.Update( timeslice );
		
		m_ActionTarget.Update();
		
		//
		//modifiers - tendency status (critical)
		if ( m_BlinkTime > TENDENCY_BLINK_TIME )
		{
			//get color & alpha
			if ( m_TendencyStatusCritical.Count() > 0 )
			{
				for ( int i = 0; i < m_TendencyStatusCritical.Count(); ++i )
				{
					
					ImageWidget widget = m_TendencyStatusCritical.GetKey( i );
					int alpha = widget.GetAlpha() * 255;
					int color;
					int colorA = ARGB( alpha, 255, 255, 255 );
					int colorB = m_TendencyStatusCritical.Get( widget );
					
					//set new color to different color
					if ( widget.GetColor() == colorA )
					{
						color = colorB;
					}
					else
					{
						color = colorA;
					}

					widget.SetColor( color );
				}
			}
			
			m_BlinkTime = 0;	//reset
		}
		else
		{
			int current_time = GetGame().GetTime();
			float delta_time = ( current_time - m_LastTime ) / 1000;		//in seconds
			m_BlinkTime += delta_time;
		}
		
		//update time
		m_LastTime = GetGame().GetTime();
		//
		
		RefreshVehicleHud( timeslice );
	}
}
