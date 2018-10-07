// *************************************************************************************
// ! PluginDayzPlayerDebugUserData
// *************************************************************************************
class PluginDayzPlayerDebugUserData
{
	void PluginDayzPlayerDebugUserData(int pUserData, bool pFullBody, int pStanceMask = -1)
	{
		m_iUserData 	= pUserData;
		m_bIsFullBody	= pFullBody;
		m_iStanceMask	= pStanceMask;
	}
		

	int		GetInt()
	{
		return m_iUserData;
	}

	bool	IsFullBody()
	{
			return m_bIsFullBody;
	}
	
	int 	GetStanceMask()
	{
			return m_iStanceMask;
	}

	protected int 	m_iUserData;
	protected bool	m_bIsFullBody;
	protected int 	m_iStanceMask;
}

// *************************************************************************************
// ! PluginDayzPlayerDebugUI 
// *************************************************************************************
class PluginDayzPlayerDebugUIHandler extends ScriptedWidgetEventHandler
{
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		return m_pPluginPlayerDebug.OnClick(w, x, y, button);
	}


	PluginDayzPlayerDebug		m_pPluginPlayerDebug;
}


// *************************************************************************************
// ! PluginDayzPlayerDebugUI 
// *************************************************************************************
class PluginDayzPlayerActionCallback extends HumanCommandActionCallback
{
	void 	PluginDayzPlayerActionCallback()
	{
		Print("Action callback created");
		EnableCancelCondition(true);
		RegisterAnimationEvent("ActionExec", 1);	// really weird id 		
		EnableStateChangeCallback();               	// enable this to get OnStateChange callbacks 

		m_fTimeStart = GetWorldTime();
	}
	
	void 	~PluginDayzPlayerActionCallback()
	{
		Print("Action callback deleted");
	}

	override void 	OnFinish(bool pCanceled)
	{
		if (pCanceled)
		{
			m_pStateWidget.SetText("Result: canceled");
		}
		else
		{
			m_pStateWidget.SetText("Result: ended");
		}
	}


	bool				CancelCondition()	
	{ 
		// Print("Action cancel condition");
		return DefaultCancelCondition(); 
	}

	override void 				OnAnimationEvent(int pEventID)
	{
		// only one event id is registered - 1 so nothing else can be delivered
		string a = "ActionExec event at: ";
		float  tFromSt = GetWorldTime() - m_fTimeStart;
		a += tFromSt.ToString();

		m_pAnimEventWidget.SetText(a);
	}

	override void 				OnStateChange(int pOldState, int pCurrentState)
	{
		string oldState = GetStateString(pOldState);
		string newState = GetStateString(pCurrentState);
		
		m_pStateWidget.SetText(oldState + "->" + newState);
	};



	//! 
	float 								m_fTimeStart;
	TextWidget							m_pStateWidget;
	TextWidget 							m_pAnimEventWidget;
}


// *************************************************************************************
// ! PluginDayzPlayerDebug  
// *************************************************************************************
class PluginDayzPlayerDebug extends PluginBase
{
	ref Timer						m_TickTimer;
	bool 								m_IsActive			= false;
	bool								m_HasFocus			= false;
	bool								m_HasServerWalk = false;
	
	Widget 								m_MainWnd;
	Widget								m_Card0;
	Widget								m_Card1;
	Widget								m_Card2;
	ButtonWidget						m_Card0Button;		
	ButtonWidget						m_Card1Button;		
	ButtonWidget						m_Card2Button;		


	EditBoxWidget 						m_PlayerStatusV;
	TextListboxWidget					m_ActionsSelector;
	ButtonWidget						m_ActionsStart;
	ButtonWidget						m_ActionsEnd;
	ButtonWidget						m_ActionsEnd2;
	ButtonWidget						m_ActionsCancel;
	ButtonWidget						m_ActionsOTA;
	ButtonWidget						m_ActionsOTL;
	TextWidget							m_ActionStateV;
	TextWidget 							m_ActionEventV;
	TextWidget 							m_ActionsGlobalStatus;

	ButtonWidget 						m_PlayerSyncCreateShadow;
	ButtonWidget 						m_PlayerSyncDelete;
	ButtonWidget 						m_PlayerServerWalk;

	TextWidget 							m_ItemAccClass;
	TextWidget 							m_ItemAccSClass;
	TextWidget 							m_ItemAccInst;

	ButtonWidget						m_ActionTypeLA;
	ButtonWidget						m_ActionTypeOA;
	ButtonWidget						m_ActionTypeFLA;
	ButtonWidget						m_ActionTypeFOA;
	ButtonWidget						m_ActionTypeLG;
	ButtonWidget						m_ActionTypeOG;
	ButtonWidget						m_ActionTypeFLG;
	ButtonWidget						m_ActionTypeFOG;


	ref PluginDayzPlayerDebugUIHandler m_pUIHandler;

	Entity 								m_pPlayerShadow;

	// command handler properties
	bool								m_CH_ActionStart 	= false;
	int 								m_CH_ActionCommand 	= -1000;
	bool								m_CH_ActionCancel	= false;
	
	
	ref PluginDayzPlayerDebug_Weapons		m_Weapons;
	ref PluginDayzPlayerDebug_Ctrl			m_Controls;

	//---------------------------------------------------
    // gui stuff

	void PluginDayzPlayerDebug()
	{
		#ifndef NO_GUI
		CreateModuleWidgets();
		m_Weapons 	= new PluginDayzPlayerDebug_Weapons(m_Card1);
		m_Controls	= new PluginDayzPlayerDebug_Ctrl(m_Card2);
		#endif
	}
	
	
	void ~PluginDayzPlayerDebug()
	{
		ReleaseFocus();	

		PlayerSyncDelete();

		#ifndef NO_GUI
		m_Weapons = NULL;
		m_Controls = NULL;
		DestroyModuleWidgets();
		#endif		
	}

	void RegisterDebugItem(ItemBase item, PlayerBase player)
	{
	}
	
	
	bool IsActive()
	{
		return m_IsActive;
	}

	void ShowWidgets(bool show)
	{
		if(m_MainWnd) 
		{
			m_MainWnd.Show(show);
		}
	}

	//!
	void Show()
	{
		ShowWidgets(true);
		m_IsActive = true;

		if (!m_TickTimer)
		{
			m_TickTimer = new Timer();
			m_TickTimer.Run(0.1, this, "Tick", NULL, true);			
		}
	}

	//!
	void Hide()
	{
		ShowWidgets(false);
		m_IsActive = false;
			
		m_TickTimer = NULL;
	}

	//!
	void 	CaptureFocus()
	{
		if (!m_HasFocus)
		{
			GetGame().GetInput().ChangeGameFocus(1);
			GetGame().GetUIManager().ShowUICursor(true);
			m_HasFocus = true;
		}
	}

	//!
	void	ReleaseFocus()
	{
		if (m_HasFocus)
		{
			GetGame().GetInput().ChangeGameFocus(-1);
			GetGame().GetUIManager().ShowUICursor(false);
			m_HasFocus = false;
		}	
	}

	int 	m_CurrentMode = 0;

	//!
	void ToggleDebugWindowEvent()
	{
		m_CurrentMode++;
		if (m_CurrentMode > 2)
		{
			m_CurrentMode = 0;
		}

		ToggleDebugWindowSetMode(m_CurrentMode);
	}


	//!
	void ToggleDebugWindowEventOld()
	{
		//! act:0 foc:0 -> act:1 foc:0
		//! act:1 foc:0 -> act:0 foc:0
		//! act:1 foc:1 -> act:1 foc:0

		if(m_IsActive && !m_HasFocus)
		{	
			Hide();
		}
		else
		{
			Show();
			ReleaseFocus();
		}
	}



	void ToggleDebugWindowEventP()
	{
		//! act:0 foc:0 -> act:1 foc:1
		//! act:1 foc:0 -> act:1 foc:1
		//! act:1 foc:1 -> act:0 foc:0

		if (m_IsActive && m_HasFocus)
		{
			Hide();
			ReleaseFocus();
		}
		else
		{
			Show();
			CaptureFocus();
		}
	}


	void ToggleDebugWindowSetMode(int pMode)
	{
		switch (pMode)
		{
		case 0:		Hide(); ReleaseFocus();	break;
		case 1:		Show(); ReleaseFocus();	break;
		case 2:		Show(); CaptureFocus();	break;
		}
	}

	void CreateModuleWidgets()
	{
		if (!m_pUIHandler)
		{
			m_pUIHandler = new PluginDayzPlayerDebugUIHandler();
			m_pUIHandler.m_pPluginPlayerDebug = this;
		}

		if (!m_MainWnd)
		{
			m_MainWnd = GetGame().GetWorkspace().CreateWidgets("gui/layouts/debug/day_z_playerdebug.layout");
			m_MainWnd.SetHandler(m_pUIHandler);
			m_MainWnd.Show(false);
		}
		
		m_Card0	= m_MainWnd.FindAnyWidget("Card0");
		m_Card1	= m_MainWnd.FindAnyWidget("Card1");
		m_Card2	= m_MainWnd.FindAnyWidget("Card2");
		m_Card0Button	= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("Card0Button") );
		m_Card1Button	= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("Card1Button") );
		m_Card2Button	= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("Card2Button") );


		m_PlayerStatusV 	= EditBoxWidget.Cast( m_MainWnd.FindAnyWidget("PlayerStatusV") );
		m_ActionsSelector	= TextListboxWidget.Cast( m_MainWnd.FindAnyWidget("ActionsSelector") );
		m_ActionsStart		= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsStart") );
		m_ActionsEnd		= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsEnd") );
		m_ActionsEnd2		= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsEnd2") );
		m_ActionsCancel		= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsCancel") );
		m_ActionsOTA			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsOTA") );
		m_ActionsOTL			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsOTL") );
		m_ActionStateV		= TextWidget.Cast( m_MainWnd.FindAnyWidget("ActionStateV") );
		m_ActionEventV		= TextWidget.Cast( m_MainWnd.FindAnyWidget("ActionEventV") );
		m_ActionsGlobalStatus	= TextWidget.Cast( m_MainWnd.FindAnyWidget("ActionGlobalStatus") );

		m_PlayerSyncCreateShadow 	= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("PlayerSyncCreate") );
		m_PlayerSyncDelete 			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("PlayerSyncDelete") );
		m_PlayerServerWalk			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("PlayerServerWalk") );

		m_ItemAccClass			= TextWidget.Cast( m_MainWnd.FindAnyWidget("ItemAccessClass") );
		m_ItemAccSClass			= TextWidget.Cast( m_MainWnd.FindAnyWidget("ItemAccessSClass") );
		m_ItemAccInst				= TextWidget.Cast( m_MainWnd.FindAnyWidget("ItemAccessInst") );

		m_ActionTypeLA			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsGroupLA") );
		m_ActionTypeOA			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsGroupOA") );
		m_ActionTypeFLA			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsGroupFLA") );
		m_ActionTypeFOA			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsGroupFOA") );
		m_ActionTypeLG			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsGroupLG") );
		m_ActionTypeOG			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsGroupOG") );
		m_ActionTypeFLG			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsGroupFLG") );
		m_ActionTypeFOG			= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("ActionsGroupFOG") );


		ActionsInit(0);

		ShowCard(0);
		
		// m_DebugAgentListWidget = m_DebugRootWidget.FindAnyWidget("w_Agents");
		// m_DebugFloatsListWidget = m_DebugRootWidget.FindAnyWidget("w_Floats");
		// m_DebugStringsListWidget = m_DebugRootWidget.FindAnyWidget("w_Strings");
		// m_DebugFloatsProperListWidget = m_DebugRootWidget.FindAnyWidget("w_FloatsProper");
		// m_ItemPreviewWidget = m_DebugRootWidget.FindAnyWidget("w_ItemPreview");
		// m_ClassNameWidget = m_DebugRootWidget.FindAnyWidget("w_ClassName");
	}
	
	void DestroyModuleWidgets()
	{
		delete m_MainWnd;
	}


	void ShowCard(int pCard)
	{
		m_Card0.Show(pCard == 0);
		m_Card1.Show(pCard == 1);
		m_Card2.Show(pCard == 2);
	}

	

	//---------------------------------------------------
    // main update

	
	void Tick()
	{
		DayZPlayer player = DayZPlayer.Cast( GetGame().GetPlayer() );
		if (!player)
		{
			return;
		}
				
		// Print("PluginDayzPlayerDebug tick");	
		UpdatePlayerStatus(player);
		
		// DbgShowBoneName(player);			
		UpdateCommandModifiers(player);

		//!
		UpdateItemAccessor(player);
		
		m_Weapons.Tick();
	}
	
	
	//---------------------------------------------------
    // Player main

	
	void UpdatePlayerStatus(DayZPlayer pPlayer)
	{
		
		ref HumanMovementState		state 	= new HumanMovementState();
		
		
		pPlayer.GetMovementState(state);
				
		string a = "Cmd:"; 
		
		if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_MOVE)
		{
			a += "MOVE:";
			a += " st:" + state.m_iStanceIdx.ToString() + ", mv:" + state.m_iMovement.ToString();
		}
		else if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_ACTION)
		{
			a += "FB ACTION:";
			a += " st:" + state.m_iStanceIdx.ToString();
		}
		else if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_MELEE)
		{
			a += "MELEE:";
			a += " st:" + state.m_iStanceIdx.ToString();
		}
		else if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_MELEE2)
		{
			a += "MELEE2:";
			a += " st:" + state.m_iStanceIdx.ToString();
		}
		else if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_FALL)
		{
			a += "FALL:";
			a += " st:" + state.m_iStanceIdx.ToString();
		}
		else if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_DEATH)
		{
			a += "now DEAD:";
			a += " st:" + state.m_iStanceIdx.ToString();
		}
		else if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_LADDER)
		{
			a += "LADDER:";
			a += " st:" + state.m_iStanceIdx.ToString();
		}
		else if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
		{
			a += "Unconscious:";
			a += " st:" + state.m_iStanceIdx.ToString();
		}
		else if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_SWIM)
		{
			a += "Swimming:";
			a += " st:" + state.m_iStanceIdx.ToString();
		}
		else
		{
			a += "UNKNOWN";
		}
		
		m_PlayerStatusV.SetText(a);
	}


	void DbgShowBoneName(DayZPlayer pPlayer)
	{
		int 	boneIndex = pPlayer.GetBoneIndexByName("Head");

		// vector 	pos			= pPlayer.GetBonePositionMS(boneIndex);
		vector 	tm[4];
		
		pPlayer.GetBoneTransformMS(boneIndex, tm);

	

		string a = "Bone index:";
		a += boneIndex.ToString();
		a += ","; 
		a += tm.ToString();
		Print(a);
	}


	void UpdateCommandModifiers(DayZPlayer pPlayer)
	{
		Actions_UpdateGlobals(pPlayer);
	
		// int nMods = pPlayer.GetCommandModifierCount();
		// Print("Number of modifiers");
		// Print(nMods);

		/*
		for (int i = 0; i < nMods; i ++)
		{
			// int a = pPlayer.GetCommandModifierId(i);

			if (pPlayer.GetCommandModifierId(i) == DayZPlayerConstants.COMMANDID_MOD_ACTION)
			{
			}
			
			// Print(a);
			
		}
		*/

		HumanCommandActionCallback clbk = pPlayer.GetCommandModifier_Action();
		if (clbk)
		{
			Print(clbk);
		};
	}


	//---------------------------------------------------
    // Actions

	void ActionsInit(int pType)
	{
			
		m_ActionsSelector.ClearItems();
		
        //! ---------------------------------------------------------
        //! ---------------------- ACTIONS --------------------------
        //! ---------------------------------------------------------
		//! looping 
		if (pType == 0)
		{
			m_ActionsSelector.AddItem("L DRINK", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_DRINK, false), 0);
			m_ActionsSelector.AddItem("L EAT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EAT, false), 0);
			m_ActionsSelector.AddItem("L EMPTY CANISTER", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EMPTYCANISTER, false), 0);
			m_ActionsSelector.AddItem("L CATCH RAIN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_CATCHRAIN, false), 0);
			m_ActionsSelector.AddItem("L EAT PILLS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EATPILLS, false), 0);
			m_ActionsSelector.AddItem("L EAT TABLETS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS, false), 0);
			m_ActionsSelector.AddItem("L EMPTY BOTTLE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EMPTYBOTTLE, false), 0);
			m_ActionsSelector.AddItem("L VIEW COMPASS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_VIEWCOMPASS, false), 0);
			m_ActionsSelector.AddItem("L DRINK CAN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_DRINKCAN, false), 0);
			m_ActionsSelector.AddItem("L WALKIETALKIE TUNE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_WALKIETALKIETUNE, false), 0);
			m_ActionsSelector.AddItem("L GIVE L ITEM", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_GIVEL, false), 0);
			m_ActionsSelector.AddItem("L GIVE R ITEM", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_GIVER, false), 0);
			m_ActionsSelector.AddItem("L SHAVE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_SHAVE, false), 0);
			m_ActionsSelector.AddItem("L FILL MAG", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_FILLMAG, false), 0);
			m_ActionsSelector.AddItem("L EMPTY MAG", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EMPTYMAG, false), 0);
			m_ActionsSelector.AddItem("L DRINK POT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_DRINKPOT, false), 0);
			m_ActionsSelector.AddItem("L EMPTY POT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EMPTYPOT, false), 0);
			m_ActionsSelector.AddItem("L EAT FRUIT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EATFRUIT, false), 0);
			m_ActionsSelector.AddItem("L TAKE TEMP SELF", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_TAKETEMPSELF, false), 0);
			m_ActionsSelector.AddItem("L VIEW MAP", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_VIEWMAP, false), 0);
			m_ActionsSelector.AddItem("L RAISE MEGAPHONE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_RAISEMEGAPHONE, false), 0);
			m_ActionsSelector.AddItem("L SEARCH INVENTORY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_SEARCHINVENTORY, false), 0);
			m_ActionsSelector.AddItem("L CRAFTING UPPDERBODY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_CRAFTING, false), 0);
			m_ActionsSelector.AddItem("L LOOK OPTICS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_LOOKOPTICS, false), 0);
			m_ActionsSelector.AddItem("L ENGINE START", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE, false), 0);
			m_ActionsSelector.AddItem("L ENGINE STOP", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE, false), 0);			
			m_ActionsSelector.AddItem("L DRIVER OPEN DOOR", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_DRIVER_DOOR_OPEN, false), 0);
			m_ActionsSelector.AddItem("L DRIVER CLOSE DOOR", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_DRIVER_DOOR_CLOSE, false), 0);
			m_ActionsSelector.AddItem("L CODRIVER OPEN DOOR", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_CODRIVER_DOOROPEN, false), 0);
			m_ActionsSelector.AddItem("L CODRIVER CLOSE DOOR", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_CODRIVER_DOORCLOSE, false), 0);
		}

        //! one time
		if (pType == 1)
		{
			m_ActionsSelector.AddItem("O PICK UP HANDS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS, false), 0);
			m_ActionsSelector.AddItem("O PICK UP INVENTORY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_INVENTORY, false), 0);
			m_ActionsSelector.AddItem("O LICK BATTERY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY, false), 0);
			m_ActionsSelector.AddItem("O LIGHT FLARE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_LIGHTFLARE, false), 0);
			m_ActionsSelector.AddItem("O LITCHEM LIGHT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_LITCHEMLIGHT, false), 0);
			m_ActionsSelector.AddItem("O UNPIN GRENAGE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_UNPINGRENAGE, false), 0);
			m_ActionsSelector.AddItem("O OPEN DOOR FW", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW, false), 0);
			m_ActionsSelector.AddItem("O OPEN LID", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_OPENLID, false), 0);
			m_ActionsSelector.AddItem("O CLOSE LID", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_CLOSELID, false), 0);
			m_ActionsSelector.AddItem("O WALKIETALKIE ON", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_WALKIETALKIEON, false), 0);
			m_ActionsSelector.AddItem("O WALKIETALKIE OFF", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_WALKIETALKIEOFF, false), 0);
			m_ActionsSelector.AddItem("O BATON EXTEND", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_BATONEXTEND, false), 0);
			m_ActionsSelector.AddItem("O BATON RETRACT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_BATONRETRACT, false), 0);
			m_ActionsSelector.AddItem("O UNLOCK HANDCUFF TARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_UNLOCKHANDCUFFTARGET, false), 0);
			m_ActionsSelector.AddItem("O FISHINGROD EXTEND", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_FISHINGRODEXTEND, false), 0);
			m_ActionsSelector.AddItem("O FISHINGROD RETRACT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_FISHINGRODRETRACT, false), 0);
			m_ActionsSelector.AddItem("O CLEAN HANDS BOTTLE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_CLEANHANDSBOTTLE, false), 0);
			m_ActionsSelector.AddItem("O OPEN FOOD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_OPENFOOD, false), 0);
			m_ActionsSelector.AddItem("O ATTACH SCOPE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_ATTACHSCOPE, false), 0);
			m_ActionsSelector.AddItem("O ATTACH BARREL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_ATTACHBARREL, false), 0);
		}

		//! fullbody looping
		if (pType == 2)
		{
			m_ActionsSelector.AddItem("FB L DRINK", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DRINK, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L EAT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_EAT, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L CATCH RAIN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CATCHRAIN, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L EAT PILLS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_EATPILLS, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L EAT TABLETS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_EATTABLETS, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L DRINK CAN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DRINKCAN, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L WALKIETALKIE TUNE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_WALKIETALKIETUNE, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L GIVE L ITEM", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_GIVEL, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L GIVE R ITEM", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_GIVER, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L FILL MAG", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_FILLMAG, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L EMPTY MAG", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_EMPTYMAG, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L DRINK POND", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DRINKPOND, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L DRINK WELL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DRINKWELL, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L FILL BOTTLE WELL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEWELL, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L FIRE ESTINGUISHER", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_FIREESTINGUISHER, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L WRING CLOTH", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_WRING, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L BERRIES", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_BERRIES, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L DIG SHOVEL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L DIG HOE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DIGHOE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L FISHING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_FISHING, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L CPR", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CPR, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L BANDAGE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_BANDAGE, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L CRAFTING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CRAFTING, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L INTERRACT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_INTERACT, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L DRINK CANNISTER", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DRINKCANISTER, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L FORCE FEED TARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_FORCEFEED, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L BANDAGE TARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L DISINFECT TARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DISINFECTTARGET, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L DEPLOY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DEPLOY, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L START FIRE WITH MATCH", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_STARTFIREMATCH, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L ANIMAL SKINNING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_ANIMALSKINNING, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L WASH HANDS WELL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_WASHHANDSWELL, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L WASH HANDS POND", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_WASHHANDSPOND, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L SALINE BLOODBAG TARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L SALINE BLOOD BAG", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAG, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L STITCH UP SELF", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_STITCHUPSELF, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L VOMIT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_VOMIT, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L CUT TIES TARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CUTTIESTARGET, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L TIE UP TARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_TIEUPTARGET, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L START FIRE WITH TORCH", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_STARTFIRETORCH, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L CHECK PULSE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CHECKPULSE, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L EMPTY SEEDS PACK", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_EMPTYSEEDSPACK, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L CLEAN WOUNDS TARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CLEANWOUNDSTARGET, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L START FIRE WITH DRILL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_STARTFIREDRILL, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L START FIRE WITH LIGHTER", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_STARTFIRELIGHTER, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L COLLECT BLOOD SELF", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_COLLECTBLOODSELF, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L POUR BOTTLE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_POURBOTTLE, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L POUR CAN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_POURCAN, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L EAT FRUIT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_EATFRUIT, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L HACK BUSH", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_HACKBUSH, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L HACK TREE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_HACKTREE, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L TAKE TEMP SELF", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_TAKETEMPSELF, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L DIG HOLE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DIGHOLE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L DIG UP CACHE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DIGUPCACHE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L DEPLOY HEAVY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_HEAVY, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L DEPLOY 1HD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_1HD, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L DEPLOY 2HD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_2HD, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L BLOW FIREPLACE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_BLOWFIREPLACE, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L VIEW MAP", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_VIEWMAP, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L VIEW COMPASS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_VIEWCOMPASS, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L FILL BOTTLE POND", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEPOND, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L PLACING HEAVY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_PLACING_HEAVY, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L PLACING 1HD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_PLACING_1HD, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L PLACING 2HD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_PLACING_2HD, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L CUT BARK", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CUTBARK, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L VIEW NOTE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_VIEWNOTE, true, DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L SEARCH INVENTORY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_SEARCHINVENTORY, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L LOOK OPTICS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_LOOKOPTICS, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L MINE ROCK", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_MINEROCK, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		
		}
		
		
        //! one time
		if (pType == 3)
		{
			m_ActionsSelector.AddItem("FB O PICK UP HANDS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_PICKUP_HANDS, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O PICK UP INVENTORY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_PICKUP_INVENTORY, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O LICK BATTERY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_LICKBATTERY, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O LIGHT FLARE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_LIGHTFLARE, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O LITCHEM LIGHT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_LITCHEMLIGHT, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O UNPIN GRENAGE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_UNPINGRENAGE, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O WALKIETALKIE ON", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_WALKIETALKIEON, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O WALKIETALKIE OFF", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_WALKIETALKIEOFF, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O HANDCUFF TARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_HANDCUFFTARGET, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB O MORPHINE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_MORPHINE, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB O INJECTION", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_INJECTION, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB O INJECTION TARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_INJECTIONTARGET, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB O DRINK SIP", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DRINKSIP, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB O CLEAN HANDS BOTTLE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CLEANHANDSBOTTLE, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O OPEN FOOD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_OPENFOOD, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O POKE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_POKE, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB O ATTACH SCOPE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_ATTACHSCOPE, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O ATTACH BARREL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_ATTACHBARREL, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
		}
        
        
    //! ---------------------------------------------------------
    //! ---------------------- GESTURES -------------------------
    //! ---------------------------------------------------------
		//! looping 
		if (pType == 4)
		{
			m_ActionsSelector.AddItem("L GREETING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_GREETING, false), 0);
			m_ActionsSelector.AddItem("L POINT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_POINT, false), 0);
			m_ActionsSelector.AddItem("L THUMB UP", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_THUMB, false), 0);
			m_ActionsSelector.AddItem("L THUMB DOWN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_THUMB, false), 0);
			m_ActionsSelector.AddItem("L SILENCE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_SILENCE, false), 0);
			m_ActionsSelector.AddItem("L TAUNT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_TAUNT, false), 0);
			m_ActionsSelector.AddItem("L TIME OUT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_TIMEOUT, false), 0);
			m_ActionsSelector.AddItem("L HEART", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_HEART, false), 0);
			m_ActionsSelector.AddItem("L FACEPALM", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_FACEPALM, false), 0);
			m_ActionsSelector.AddItem("L WATCHING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_WATCHING, false), 0);
			m_ActionsSelector.AddItem("L HOLD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_HOLD, false), 0);
			m_ActionsSelector.AddItem("L LISTENING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_LISTENING, false), 0);
			m_ActionsSelector.AddItem("L POINT AT SELF", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_POINTSELF, false), 0);
			m_ActionsSelector.AddItem("L LOOK AT ME", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_LOOKATME, false), 0);
			m_ActionsSelector.AddItem("L OKAY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_OKAY, false), 0);

			m_ActionsSelector.AddItem("L RPS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_RPS, false), 0);
		}
        
        //! one time
		if (pType == 5)
		{
			m_ActionsSelector.AddItem("O THROAT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_THROAT, false), 0);
			m_ActionsSelector.AddItem("O CLAP", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_CLAP, false), 0);
			m_ActionsSelector.AddItem("O DABBING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_DABBING, false), 0);
			m_ActionsSelector.AddItem("O MOVE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_MOVE, false), 0);
			m_ActionsSelector.AddItem("O DOWN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_DOWN, false), 0);
			m_ActionsSelector.AddItem("O COME", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_COME, false), 0);
			m_ActionsSelector.AddItem("O TAUNT KISS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_TAUNTKISS, false), 0);
			m_ActionsSelector.AddItem("O TAUNT ELBOW", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_TAUNTELBOW, false), 0);
			m_ActionsSelector.AddItem("O TAUNT THINK", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_TAUNTTHINK, false), 0);
			m_ActionsSelector.AddItem("O NOD HEAD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_NODHEAD, false), 0);
			m_ActionsSelector.AddItem("O SHAKE HEAD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_SHAKEHEAD, false), 0);
			m_ActionsSelector.AddItem("O SHRUG", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_SHRUG, false), 0);
		}


		//! fullbody looping
		if (pType == 6)
		{
			m_ActionsSelector.AddItem("FB L GREETING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_GREETING, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L POINT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_POINT, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L THUMBUP", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_THUMB, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L THUMBDOWN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_THUMB, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L SILENCE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SILENCE, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L TAUNT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_TAUNT, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L TIMEOUT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_TIMEOUT, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L HEART", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_HEART, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L WATCHING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_WATCHING, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L HOLD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_HOLD, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L LISTENING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_LISTENING, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L POINTSELF", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_POINTSELF, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L LOOKATME", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_LOOKATME, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L OKAY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_OKAY, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB L SALUTE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SALUTE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L CAMPFIRE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_CAMPFIRE, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L LYINGDOWN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_LYINGDOWN, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L SOS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SOS, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L SITA", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SITA, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L SITB", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SITB, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
			m_ActionsSelector.AddItem("FB L DABBING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_DABBING, true, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
			
			m_ActionsSelector.AddItem("TRAILER WAKE UP", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_TRAILER_WAKE_UP, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("TRAILER DEAD BODY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_TRAILER_DEAD_BODY, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("TRAILER WALKIE TALKIE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_TRAILER_WALKIE_TALKIE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("TRAILER WOUNDED", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_TRAILER_WOUNDED, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("TRAILER WALK AWAY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_TRAILER_WALK_AWAY, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		}
        
		//! fullbody one time
		if (pType == 7)
		{
			m_ActionsSelector.AddItem("FB O THROAT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_THROAT, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O MOVE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_MOVE, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O DOWN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_DOWN, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O COME", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_COME, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O TAUNT KISS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_TAUNTKISS, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O TAUNT THINK", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_TAUNTTHINK, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O DANCE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_DANCE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB O NOD HEAD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_NODHEAD, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O SHAKE HEAD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SHAKEHEAD, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
			m_ActionsSelector.AddItem("FB O SHRUG", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SHRUG, true, DayZPlayerConstants.STANCEMASK_PRONE), 0);
		}
        
        
        //! ---------------------------------------------------------
        //! ---------------------- SUICIDE --------------------------
        //! ---------------------------------------------------------
		//! fullbody looping
		if (pType == 6)
		{
			m_ActionsSelector.AddItem("FB L 1HD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_1HD, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L FIREAXE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_FIREAXE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L PITCHFORK", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_PITCHFORK, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L PISTOL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_PISTOL, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L RIFLE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_RIFLE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L SWORD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_SWORD, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L UNARMED", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_UNARMED, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L WOODAXE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_WOODAXE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
			m_ActionsSelector.AddItem("FB L SPEAR", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_SPEAR, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		}

	}


	//!
	void	Actions_Start()
	{
		int		row = m_ActionsSelector.GetSelectedRow();
		if (row == -1)
		{
			return;
		}

		//!
		DayZPlayer player = DayZPlayer.Cast( GetGame().GetPlayer() );
		if (!player)
		{
			return;
		}

		PluginDayzPlayerDebugUserData	userData;
		m_ActionsSelector.GetItemData(row, 0, userData);

		//! start action 
		PluginDayzPlayerActionCallback a;
		
		if (userData.IsFullBody())
		{
			a = PluginDayzPlayerActionCallback.Cast( player.StartCommand_Action(userData.GetInt(), PluginDayzPlayerActionCallback, userData.GetStanceMask()) );
		}
		else
		{
			a = PluginDayzPlayerActionCallback.Cast( player.AddCommandModifier_Action(userData.GetInt(), PluginDayzPlayerActionCallback) );
		}
		a.m_pStateWidget 		= m_ActionStateV;
		a.m_pAnimEventWidget	= m_ActionEventV;
		

		Print(m_ActionStateV);
		m_ActionStateV.SetText("Crash Test");
	}

	//!
	void	Actions_DoInternalCommand(int pCommandId)
	{
		//!
		DayZPlayer player = DayZPlayer.Cast( GetGame().GetPlayer() );
		if (!player)
		{
			return;
		}

		HumanCommandActionCallback clbk = player.GetCommand_Action();
		if (!clbk)
		{
			clbk = player.GetCommandModifier_Action();
			if (!clbk)
			{
				return;
			}
		}
		
		clbk.InternalCommand(pCommandId);		
	}

	//!
	void	Actions_DoCancel()
	{
		//!
		DayZPlayer player = DayZPlayer.Cast( GetGame().GetPlayer() );
		if (!player)
		{
			return;
		}

		HumanCommandActionCallback clbk = player.GetCommand_Action();
		if (!clbk)
		{
			clbk = player.GetCommandModifier_Action();
			if (!clbk)
			{
				return;
			}
		}
		clbk.Cancel();			
	}


	//! 
	void 	Actions_UpdateGlobals(DayZPlayer pPlayer)
	{
		HumanCommandActionCallback clbk = pPlayer.GetCommandModifier_Action();
		if (!clbk)
		{
			m_ActionsGlobalStatus.SetText("No Action");
		}

		m_ActionsGlobalStatus.SetText(clbk.ToString());
	}


        
    //! ---------------------------------------------------------


	void 	PlayerSyncCreate()
	{
		PlayerSyncDelete();

		vector item_position = 	GetGame().GetPlayer().GetPosition() + ( GetGame().GetPlayer().GetDirection() * 2.0);

		m_pPlayerShadow = Entity.Cast( GetGame().CreateObject("SurvivorM_Mirek", item_position, false, false) );

		DayZPlayer 	pl = DayZPlayer.Cast( GetGame().GetPlayer() );		
		pl.DebugSyncShadowSetup( DayZPlayer.Cast( m_pPlayerShadow ) );
	}

	void 	PlayerSyncDelete()
	{
		if (m_pPlayerShadow)
		{
			GetGame().ObjectDelete(m_pPlayerShadow);
			m_pPlayerShadow = NULL;
		}

		DayZPlayer 	pl = DayZPlayer.Cast( GetGame().GetPlayer() );		
		if (pl)
		{
			pl.DebugSyncShadowSetup(NULL);
		}

	}


	//---------------------------------------------------
	// item accessor

	void UpdateItemAccessor(DayZPlayer pPlayer)
	{
		m_ItemAccClass.SetText(pPlayer.DebugGetItemClass());
		m_ItemAccSClass.SetText(pPlayer.DebugGetItemSuperClass());
		m_ItemAccInst.SetText(pPlayer.DebugGetItemAnimInstance());
	}


	//---------------------------------------------------
    // window ui clicks

	//! buttons clicks
	bool	OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_Card0Button)
		{
			ShowCard(0);
		}
		else if (w == m_Card1Button)
		{
			ShowCard(1);
		}
		else if (w == m_Card2Button)
		{
			ShowCard(2);
		}
		else if (w == m_ActionsStart)
		{
			Print("PluginPlayerDebug: Action Start");
			m_CH_ActionStart = true;
			return true;
		}
		else if (w == m_ActionsEnd)
		{
			Print("PluginPlayerDebug: Action End");
			m_CH_ActionCommand = DayZPlayerConstants.CMD_ACTIONINT_END;
			return true;
		}
		else if (w == m_ActionsEnd2)
		{
			Print("PluginPlayerDebug: Action End 2");
			m_CH_ActionCommand = DayZPlayerConstants.CMD_ACTIONINT_END2;
			return true;
		}
		else if (w == m_ActionsOTA)
		{
			Print("PluginPlayerDebug: Action One Time Action");
			m_CH_ActionCommand = DayZPlayerConstants.CMD_ACTIONINT_ACTION;
			return true;
		}
		else if (w == m_ActionsOTL)
		{
			Print("PluginPlayerDebug: Action Loop");
			m_CH_ActionCommand = DayZPlayerConstants.CMD_ACTIONINT_ACTIONLOOP;
			return true;
		}
		else if (w == m_ActionsCancel)
		{
			Print("PluginPlayerDebug: Action Cancel");
			m_CH_ActionCancel = true;
			return true;
		}
		else if (w == m_PlayerSyncCreateShadow)
		{
			PlayerSyncCreate();
			return true;
		}
		else if (w == m_PlayerSyncDelete)
		{
			PlayerSyncDelete();
			return true;
		}
		else if (w == m_PlayerServerWalk)
		{
			//! send rcp 
			m_HasServerWalk = !m_HasServerWalk;
#ifdef DEVELOPER
			Param1<bool> rp = new Param1<bool>(m_HasServerWalk);
			GetGame().RPCSingleParam(GetGame().GetPlayer(), ERPCs.RPC_DAYZPLAYER_DEBUGSERVERWALK, rp, true);
#endif
		}
		else if (w == m_ActionTypeLA)
		{
			ActionsInit(0);
		}
		else if (w == m_ActionTypeOA)
		{
			ActionsInit(1);
		}
		else if (w == m_ActionTypeFLA)
		{
			ActionsInit(2);
		}
		else if (w == m_ActionTypeFOA)
		{
			ActionsInit(3);
		}
		else if (w == m_ActionTypeLG)
		{
			ActionsInit(4);
		}
		else if (w == m_ActionTypeOG)
		{
			ActionsInit(5);
		}
		else if (w == m_ActionTypeFLG)
		{
			ActionsInit(6);
		}
		else if (w == m_ActionTypeFOG)
		{
			ActionsInit(7);
		}

		if (m_Weapons.OnClick(w,x,y,button))
		{
			return true;
		}
		else 
		{
			return m_Controls.OnClick(w,x,y,button);
		}
	}

	//---------------------------------------------------
    // Global handler to handle commands from player

	bool		IsWeaponChange(out string pNewWeapon, out int pHideSlot, out int pShowSlot)
	{
		if (m_Weapons)
		{
			m_Weapons.IsWeaponChange(pNewWeapon, pHideSlot, pShowSlot);
		}
		
		return false;
	}


	//---------------------------------------------------
    // Global handler to handle commands from player

	void 	CommandHandler()
	{
		if (m_CH_ActionStart)
		{
			Actions_Start();
			m_CH_ActionStart	= false;
		}

		if (m_CH_ActionCommand != -1000)
		{
			Actions_DoInternalCommand(m_CH_ActionCommand);
			m_CH_ActionCommand = -1000;
		}

		if (m_CH_ActionCancel)
		{
			Actions_DoCancel();			
			m_CH_ActionCancel = false;		
		}

		if (m_Weapons)
		{
			m_Weapons.CommandHandler();
		}

		if (m_Controls)
		{
			m_Controls.CommandHandler();	
		}
	}

			
}
