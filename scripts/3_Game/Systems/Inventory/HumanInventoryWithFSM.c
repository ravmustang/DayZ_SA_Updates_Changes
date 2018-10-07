/**
 * @class	HumanInventoryWithFSM
 * @brief	HumanInventory... with FSM (synchronous, no anims)
 **/
class HumanInventoryWithFSM : HumanInventory
{
	protected ref HandFSM m_FSM;  /// hand slot state machine
	protected ref HandStateBase m_Empty;
	protected ref HandStateBase m_Equipped;

	void HumanInventoryWithFSM ()
	{
		m_FSM = new HandFSM();

		m_Empty = new HandStateEmpty(GetManOwner());
		m_Equipped = new HandStateEquipped(GetManOwner());
	}

	override void Init ()
	{
		// setup state machine
		hndDebugPrint("[hndfsm] Initializing Human Inventory FSM");

		// events
		HandEventBase __T__ = new HandEventTake;
		HandEventBase __D__ = new HandEventDrop;
		HandEventBase __M__ = new HandEventMoveTo;
		HandEventBase __W__ = new HandEventSwap;
		HandEventBase __F__ = new HandEventForceSwap;
		HandEventBase __X__ = new HandEventDestroy;
		HandEventBase __Xd_ = new HandEventDestroyed;
		HandEventBase __R__ = new HandEventDestroyAndReplaceWithNew;
		HandEventBase __Re_ = new HandEventDestroyAndReplaceWithNewElsewhere;
		HandEventBase __Rd_ = new HandEventReplaced;
		HandEventBase __Cd_ = new HandEventCreated;

		// setup transitions
		m_FSM.AddTransition(new HandTransition( m_Empty   , __T__,  m_Equipped, new HandActionTake, new HandGuardHasItemInEvent(GetManOwner())));
		m_FSM.AddTransition(new HandTransition( m_Empty   , __Cd_,  m_Equipped, new HandActionCreated, new HandGuardHasItemInEvent(GetManOwner())));

		m_FSM.AddTransition(new HandTransition(m_Equipped, __D__,    m_Empty, new HandActionDrop, null));
		m_FSM.AddTransition(new HandTransition(m_Equipped, __M__,    m_Empty, new HandActionMoveTo, new HandGuardAnd(HandGuardHasItemInHands(GetManOwner()), new HandGuardHasRoomForItem(GetManOwner()))));
		m_FSM.AddTransition(new HandTransition(m_Equipped, __X__,    m_Empty, new HandActionDestroy, new HandGuardHasItemInHands(GetManOwner())));
		m_FSM.AddTransition(new HandTransition(m_Equipped, __Xd_,    m_Empty, new HandActionDestroyed, new HandGuardHasDestroyedItemInHands(GetManOwner())));
		m_FSM.AddTransition(new HandTransition(m_Equipped, __Xd_, m_Equipped, new HandActionDestroyed, new HandGuardNot(new HandGuardHasDestroyedItemInHands(GetManOwner()))));
		m_FSM.AddTransition(new HandTransition(m_Equipped, __R__, m_Equipped, new HandActionDestroyAndReplaceWithNew, null));
		m_FSM.AddTransition(new HandTransition(m_Equipped, __Re_, 	 m_Empty, new HandActionDestroyAndReplaceWithNewElsewhere, null));
		m_FSM.AddTransition(new HandTransition(m_Equipped, __Rd_, m_Equipped, new HandActionReplaced, null));
		m_FSM.AddTransition(new HandTransition(m_Equipped, __W__, m_Equipped, new HandActionSwap, new HandGuardHasItemInHands(GetManOwner())));
		m_FSM.AddTransition(new HandTransition(m_Equipped, __F__, m_Equipped, new HandActionForceSwap, new HandGuardHasItemInHands(GetManOwner())));

		// start FSM
		m_FSM.SetInitialState(m_Empty);
		m_FSM.Start();
	}

	bool CanProcessHandEvents () { return m_FSM && m_FSM.IsRunning(); }

	/**@fn			ProcessHandEvent
	 * @brief		hand's fsm handling of events
	 * @NOTE:		warning: ProcessHandEvent can be called only within DayZPlayer::HandleInventory (or ::CommandHandler)
	 **/
	bool ProcessHandEvent (HandEventBase e)
	{
		//SyncRemote(e);
		if (m_FSM.ProcessEvent(e) == ProcessEventResult.FSM_OK)
		{
			hndDebugSpam("[hndfsm] Processed event e=" + e.ToString());
			return true;
		}
		else
		{
			//hndDebugPrint("FSM refused to process event (no transition): src=" + GetCurrentState().ToString() + " event=" + e.ToString());
			return false;
		}
	}

	override void OnStoreLoad (ParamsReadContext ctx)
	{
		super.OnStoreLoad(ctx);
    //@TODO
		//m_FSM.OnStoreLoad(ctx);
	}
	
	override void OnStoreSave (ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
    //@TODO
		//m_FSM.OnStoreSave(ctx);
	}

	/**@fn			GetCurrentStateID
	 * @return		identifier of current stable state
	 **/
	int GetCurrentStateID () { return m_FSM.GetCurrentStateID(); }

	/**@fn			NetSyncCurrentStateID
	 * @brief		Engine callback - network synchronization of FSM's state. not intended to direct use.
	 **/
	void NetSyncCurrentStateID (int id)
	{
		m_FSM.NetSyncCurrentStateID(id);
	}
};

