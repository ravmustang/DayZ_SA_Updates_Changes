/**
 * @class		DayZPlayerInventory
 **/
class DayZPlayerInventory : HumanInventoryWithFSM
{
	protected ref HandEventBase m_PostedHandEvent; /// deferred hand event
	ref WeaponEventBase m_PostedEvent; /// deferred event
	// states with animations
	protected ref HandWeaponTakingFromAtt m_Taking;
	protected ref HandWeaponMovingToAtt m_MovingTo;
	protected ref HandWeaponSwapping m_Swapping;

	void DayZPlayerInventory () { }

	DayZPlayer GetDayZPlayerOwner () { return DayZPlayer.Cast(GetInventoryOwner()); }

	override void Init ()
	{
		hndDebugPrint("[hndfsm] Creating DayZPlayer Inventory FSM");

		m_Taking = new HandWeaponTakingFromAtt(GetManOwner(), null);
		m_MovingTo = new HandWeaponMovingToAtt(GetManOwner(), null);
		m_Swapping = new HandWeaponSwapping(GetManOwner(), null);

		// events
		HandEventBase _fin_ = new HandEventHumanCommandActionFinished;
		//HandEventBase _abt_ = new HandEventHumanCommandActionAborted;
		HandEventBase __T__ = new HandEventTake;
		HandEventBase __M__ = new HandEventMoveTo;
		HandEventBase __W__ = new HandEventSwap;
		//HandEventBase __D__ = new HandEventDropping;
		//HandEventBase __F__ = new HandEventForceSwapping;

		// setup transitions
		// @NOTE: commented out due to: https://jira.bistudio.com/browse/DAYZ-32423
		/*m_FSM.AddTransition(new HandTransition( m_Empty   , __T__,    m_Taking, NULL, new HandGuardHasAttachedWeaponInEvent(GetManOwner())));
		m_FSM.AddTransition(new HandTransition( m_Taking  , _fin_,  m_Equipped, null, null));

		m_FSM.AddTransition(new HandTransition( m_Equipped, __M__,  m_MovingTo, NULL, new HandGuardAnd(HandGuardHasWeaponInHands(GetManOwner()), new HandGuardHasRoomForAttachment(GetManOwner()))));
		m_FSM.AddTransition(new HandTransition( m_MovingTo, _fin_,  m_Empty   , null, null));

		HandGuardBase swapGuard = new HandGuardAnd(HandGuardHasWeaponInHands(GetManOwner()), new HandGuardHasAttachedWeaponInEvent(GetManOwner()));
		m_FSM.AddTransition(new HandTransition( m_Equipped, __W__,  m_Swapping, NULL, swapGuard));
		m_FSM.AddTransition(new HandTransition( m_Swapping, _fin_,  m_Equipped, null, null));*/

		super.Init(); // initialize ordinary human fsm (no anims)
	}

	/**@fn	PostHandEvent
	 * @brief	deferred hands's fsm handling of events
	 * @NOTE: "post" stores the event for later use when ::CommandHandler is being run
	 **/
	override void PostHandEvent (HandEventBase e)
	{
		// @NOTE: synchronous event from server. this happens only on client(s) and is caused by CreateVehicle and DeleteObject network messages.
		bool synchronous = e.GetEventID() == HandEventID.DESTROYED || e.GetEventID() == HandEventID.CREATED;
		// @NOTE: there is no HandleInventory(dt) called on Remote(s). Immeadiate processing as workaroud.
		DayZPlayerInstanceType inst_type = GetDayZPlayerOwner().GetInstanceType();
		bool remote =  inst_type == DayZPlayerInstanceType.INSTANCETYPE_REMOTE || inst_type == DayZPlayerInstanceType.INSTANCETYPE_AI_REMOTE;
		if (synchronous || GetInventoryOwner().IsDamageDestroyed() || remote)
		{
			hndDebugPrint("[hndfsm] SYNCHRONOUS hand event e=" + e.ToString());
			ProcessHandEvent(e);
		}
		else
		{
			if (m_PostedHandEvent == NULL)
			{
				m_PostedHandEvent = e;
				hndDebugPrint("[hndfsm] Posted event m_PostedHandEvent=" + m_PostedHandEvent.ToString());
			}
			else
				Error("[hndfsm] warning - pending hand event already posted, curr_event=" + m_PostedHandEvent.ToString() + " new_event=" + e.ToString());
		}
	}
	
	/**@fn	PostWeaponEvent
	 * @brief	deferred weapon's fsm handling of events
	 * @NOTE: "post" stores the event for later use when ::CommandHandler is being run
	 **/
	void PostWeaponEvent (WeaponEventBase e)
	{
		if (m_PostedEvent == NULL)
		{
			m_PostedEvent = e;
			wpnDebugPrint("[wpnfsm] Posted event m_PostedEvent=" + m_PostedEvent.ToString());
		}
		else
			Error("[wpnfsm] warning - pending event already posted, curr_event=" + m_PostedEvent.ToString() + " new_event=" + e.ToString());
	}

	void HandleWeaponEvents (float dt, out bool exitIronSights)
	{
		HumanCommandWeapons hcw = GetDayZPlayerOwner().GetCommandModifier_Weapons();

		Weapon_Base weapon;
		Class.CastTo(weapon, GetEntityInHands());

		if (hcw && weapon && weapon.CanProcessWeaponEvents())
		{
			weapon.GetCurrentState().OnUpdate(dt);

			//wpnDebugPrint("[wpnfsm] HCW: playing A=" + typename.EnumToString(WeaponActions, hcw.GetRunningAction()) + " AT=" + WeaponActionTypeToString(hcw.GetRunningAction(), hcw.GetRunningActionType()) + " fini=" + hcw.IsActionFinished());

			if (!weapon.IsIdle())
			{
				while (true)
				{
					int weaponEventId = hcw.IsEvent();
					if (weaponEventId == -1)
					{
						break;
					}

					if (weaponEventId == WeaponEvents.CHANGE_HIDE)
					{
						break;
					}

					WeaponEventBase anim_event = WeaponAnimEventFactory(weaponEventId, GetDayZPlayerOwner(), NULL);
					wpnDebugPrint("[wpnfsm] HandleWeapons: event arrived " + typename.EnumToString(WeaponEvents, weaponEventId) + "(" + weaponEventId + ")  fsm_ev=" + anim_event.ToString());
					if (anim_event != NULL)
					{
						weapon.ProcessWeaponEvent(anim_event);
					}
				}

				if (hcw.IsActionFinished())
				{
					if (weapon.IsWaitingForActionFinish())
					{
						wpnDebugPrint("[wpnfsm] Weapon event: finished! notifying waiting state=" + weapon.GetCurrentState());
						weapon.ProcessWeaponEvent(new WeaponEventHumanCommandActionFinished(GetDayZPlayerOwner()));
					}
					else
					{
						wpnDebugPrint("[wpnfsm] Weapon event: ABORT! notifying running state=" + weapon.GetCurrentState());
						weapon.ProcessWeaponAbortEvent(new WeaponEventHumanCommandActionAborted(GetDayZPlayerOwner()));
					}
				}
			}

			if (m_PostedEvent)
			{
				wpnDebugPrint("[wpnfsm] Weapon event: deferred " + m_PostedEvent);
				weapon.ProcessWeaponEvent(m_PostedEvent);
				exitIronSights = true;
				fsmDebugSpam("[wpnfsm] Weapon event: resetting deferred event" + m_PostedEvent);
				m_PostedEvent = NULL;
			}
		}
	}

	void HandleInventory (float dt)
	{
		HumanCommandWeapons hcw = GetDayZPlayerOwner().GetCommandModifier_Weapons();

		EntityAI ih = GetEntityInHands();
		Weapon_Base weapon;
		Class.CastTo(weapon, ih);

		if (hcw)
		{
			m_FSM.GetCurrentState().OnUpdate(dt);

			//hndDebugSpam("[hndfsm] HCW: playing A=" + typename.EnumToString(WeaponActions, hcw.GetRunningAction()) + " AT=" + WeaponActionTypeToString(hcw.GetRunningAction(), hcw.GetRunningActionType()) + " fini=" + hcw.IsActionFinished());

			//if (!weapon.IsIdle())
			{
				while (true)
				{
					int weaponEventId = hcw.IsEvent();
					if (weaponEventId == -1)
					{
						break;
					}

					HandEventBase anim_event = HandAnimEventFactory(weaponEventId, GetManOwner(), NULL);
					hndDebugPrint("[hndfsm] HandleInventory: event arrived " + typename.EnumToString(WeaponEvents, weaponEventId) + "(" + weaponEventId + ")  fsm_ev=" + anim_event.ToString());
					if (anim_event != NULL)
					{
						SyncHandEventToRemote(anim_event);
						ProcessHandEvent(anim_event);
					}
				}

				if (hcw.IsActionFinished())
				{

					if (m_FSM.GetCurrentState().IsWaitingForActionFinish())
					{
						hndDebugPrint("[hndfsm] Hand-Weapon event: finished! notifying waiting state=" + m_FSM.GetCurrentState());
						HandEventBase fin_event = new HandEventHumanCommandActionFinished(GetManOwner());
						SyncHandEventToRemote(fin_event);
						ProcessHandEvent(fin_event);
					}
					else
					{
						//hndDebugPrint("[hndfsm] Hand-Weapon event: ABORT! notifying running state=" + m_FSM.GetCurrentState());
						//m_FSM.ProcessHandAbortEvent(new WeaponEventHumanCommandActionAborted(GetManOwner()));
					}
				}
			}
		}
		
		if (m_PostedHandEvent)
		{
			hndDebugSpam("[hndfsm] Hand event: deferred " + m_PostedHandEvent);

			HandEventBase hndEvent = m_PostedHandEvent; // make a copy for processing and release post
			hndDebugSpam("[hndfsm] Hand event: deferred event reset to null.");
			m_PostedHandEvent = NULL;

			ProcessHandEvent(hndEvent); // process copy
		}
	}


	///@{ juncture
	/**@fn			OnInventoryJunctureFromServer
	 * @brief		reaction to engine callback
	 *	originates in:
	 *				engine - DayZPlayer::OnSyncJuncture
	 *				script - PlayerBase.OnSyncJuncture
	 **/
	override bool OnInventoryJunctureFromServer (ParamsReadContext ctx)
	{
		int tmp = -1;
		ctx.Read(tmp);
		syncDebugPrint("[syncinv] store Juncture packet");
		StoreJunctureData(ctx);
		return true;
	}

	/**@fn			OnHandleStoredJunctureData
	 * @brief		reaction to engine callback
	 *	originates in engine - DayZPlayerInventory::HandleStoredJunctureData
	 **/
	protected void OnHandleStoredJunctureData (ParamsReadContext ctx)
	{
		int tmp = -1;
		ctx.Read(tmp);
		syncDebugPrint("[syncinv] handle JunctureData packet");
		HandleInputData(true, false, ctx);
	}

	/**@fn			StoreJunctureData
	 * @brief		stores received input user data for later handling
	/aaa
	 **/
	proto native void StoreJunctureData (ParamsReadContext ctx);
	///@} juncture


	///@{ input user data
	/**@fn			OnInputUserDataProcess
	 * @brief		reaction to engine callback
	 *	originates in:
	 *				engine - DayZPlayer::OnInputUserDataReceived
	 *				script - DayZPlayerImplement.OnInputUserDataReceived
	 **/
	override bool OnInputUserDataProcess (ParamsReadContext ctx)
	{
		syncDebugPrint("[syncinv] store InputUserData packet");
		StoreInputUserData(ctx);
		return true;
	}

	/**@fn			OnHandleStoredInputUserData
	 * @brief		reaction to engine callback
	 *	originates in engine - DayZPlayerInventory::HandleStoredInputUserData
	 **/
	protected void OnHandleStoredInputUserData (ParamsReadContext ctx)
	{
		int tmp = -1;
		ctx.Read(tmp);
		syncDebugPrint("[syncinv] handle InputUserData packet");
		HandleInputData(false, false, ctx);
	}

	/**@fn			StoreInputUserData
	 * @brief		stores received input user data for later handling
	 **/
	proto native void StoreInputUserData (ParamsReadContext ctx);
	///@} input user data
	
	bool OnInputUserDataFromRemote (ParamsReadContext ctx)
	{
		syncDebugPrint("[syncinv] remote handling InputUserData packet from server");
		return HandleInputData(false, true, ctx);
	}

	override void OnServerInventoryCommand (ParamsReadContext ctx)
	{
		syncDebugPrint("[syncinv] DZPInventory command from server");
		HandleInputData(true, true, ctx);
	}

	/**@fn			HandleInputData
	 * @brief		real processing of the input data
	 **/
	bool HandleInputData (bool juncture, bool remote, ParamsReadContext ctx)
	{
		int type = -1;
		if (!ctx.Read(type))
			return false;

		InventoryLocation src = new InventoryLocation;
		InventoryLocation dst = new InventoryLocation;

		switch (type)
		{
			case InventoryCommandType.SYNC_MOVE:
			{
				src.ReadFromContext(ctx);
				dst.ReadFromContext(ctx);

				if (remote && (!src.GetItem() || !dst.GetItem()))
				{
					Error("[syncinv] remote input (cmd=SYNC_MOVE) dropped, item not in bubble! src=" + src.DumpToString() + " dst=" + dst.DumpToString());
					break; // not in bubble
				}
				
				if (false == GameInventory.CheckMoveToDstRequest(GetManOwner(), src.GetItem(), dst))
				{
					Error("[cheat] man=" + GetManOwner() + " is cheating with cmd=" + typename.EnumToString(InventoryCommandType, type) + " src=" + src.DumpToString() + " dst=" + dst.DumpToString());
					return false; // cheater
				}
				
				syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms received cmd=" + typename.EnumToString(InventoryCommandType, type) + " src=" + src.DumpToString() + " dst=" + dst.DumpToString());
				if (!juncture && GetDayZPlayerOwner().GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
				{
					if (GetDayZPlayerOwner().NeedInventoryJunctureFromServer(src.GetItem(), src.GetParent(), dst.GetParent()))
					{
						syncDebugPrint("[syncinv] redir to juncture cmd=" + typename.EnumToString(InventoryCommandType, type) + " dst=" + dst.DumpToString());
						if (GetGame().AddInventoryJuncture(GetDayZPlayerOwner(), src.GetItem(), dst, true, GameInventory.c_InventoryReservationTimeoutMS))
							GetDayZPlayerOwner().SendSyncJuncture(DayZPlayerSyncJunctures.SJ_INVENTORY, ctx);
						else
						{
							syncDebugPrint("[syncinv] redir to juncture DENIED");
							return true; // abort, do not send anything to remotes
						}
						break;
					}
				}

				LocationSyncMoveEntity(src, dst);
				break;
			}

			case InventoryCommandType.HAND_EVENT:
			{
				HandEventBase e = HandEventBase.CreateHandEventFromContext(ctx);

				if (remote && !e.m_Entity)
				{
					Error("[syncinv] remote input (cmd=HAND_EVENT) dropped, item not in bubble");
					break; // not in bubble
				}

				if (false == e.CheckRequest())
				{
					Error("[cheat] man=" + GetManOwner() + " is cheating with cmd=" + typename.EnumToString(InventoryCommandType, type) + " event=" + e);
					return false; // cheater
				}

				syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms received cmd=" + typename.EnumToString(InventoryCommandType, type) + " event=" + e);
				if (e.m_Entity.GetInventory().GetCurrentInventoryLocation(src))
				{
					dst = e.GetDst();
					if (!juncture && GetDayZPlayerOwner().GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
					{
						if (GetDayZPlayerOwner().NeedInventoryJunctureFromServer(src.GetItem(), src.GetParent(), dst.GetParent()))
						{
							syncDebugPrint("[syncinv] redir to juncture cmd=" + typename.EnumToString(InventoryCommandType, type) + " event=" + e);
							bool test_dst_occupancy = !e.IsSwapEvent();
							if (GetGame().AddInventoryJuncture(GetDayZPlayerOwner(), e.m_Entity, dst, test_dst_occupancy, GameInventory.c_InventoryReservationTimeoutMS))
								GetDayZPlayerOwner().SendSyncJuncture(DayZPlayerSyncJunctures.SJ_INVENTORY, ctx);
							else
							{
								syncDebugPrint("[syncinv] redir to juncture DENIED");
								return true; // abort, do not send anything to remotes
							}
							break;
						}
					}

					e.m_Player.GetHumanInventory().PostHandEvent(e);
				}
				else
					Error("OnInputData HandEvent - no inv loc");
				break;
			}

			case InventoryCommandType.SWAP:
			{
				EntityAI item1, item2;
				ctx.Read(item1);
				ctx.Read(item2);
				
				if (remote && (!item1 || !item2))
				{
					Error("[syncinv] remote input (cmd=SWAP) dropped, item not in bubble");
					break; // not in bubble
				}

				if (false == GameInventory.CheckSwapItemsRequest(GetManOwner(), item1, item2))
				{
					Error("[cheat] man=" + GetManOwner() + " is cheating with cmd=" + typename.EnumToString(InventoryCommandType, type) + " src=" + src.DumpToString() + " dst=" + dst.DumpToString());
					return false; // cheater
				}

				InventoryLocation src1, src2, dst1, dst2;
				if (GameInventory.MakeSrcAndDstForSwap(item1, item2, src1, src2, dst1, dst2))
				{
					dst1.Copy(src1);
					dst1.CopyLocationFrom(src2);
					dst2.Copy(src2);
					dst2.CopyLocationFrom(src1);
					syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms received Swap src1=" + src1.DumpToString() + "dst1=" + dst1.DumpToString() +  "src2=" + src2.DumpToString() + "dst2=" + dst2.DumpToString());

					if (!juncture && GetDayZPlayerOwner().GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
					{
						bool need_j1 = GetDayZPlayerOwner().NeedInventoryJunctureFromServer(src1.GetItem(), src1.GetParent(), dst1.GetParent());
						bool need_j2 = GetDayZPlayerOwner().NeedInventoryJunctureFromServer(src2.GetItem(), src2.GetParent(), dst2.GetParent());
						if (need_j1 || need_j2)
						{
							syncDebugPrint("[syncinv] redir to juncture cmd=" + typename.EnumToString(InventoryCommandType, type) + " dst1=" + dst1.DumpToString());
							bool acq_j1 = true;
							if (need_j1)
								GetGame().AddInventoryJuncture(GetDayZPlayerOwner(), item1, dst1, false, GameInventory.c_InventoryReservationTimeoutMS);
							
							bool acq_j2 = true;
							if (need_j2)
								GetGame().AddInventoryJuncture(GetDayZPlayerOwner(), item2, dst2, false, GameInventory.c_InventoryReservationTimeoutMS);
							if (acq_j1 && acq_j2)
								GetDayZPlayerOwner().SendSyncJuncture(DayZPlayerSyncJunctures.SJ_INVENTORY, ctx);
							else
							{
								syncDebugPrint("[syncinv] redir to juncture DENIED");
								return true; // abort, do not send anything to remotes
							}
							break;
						}
					}

					LocationSwap(src1, src2, dst1, dst2);
				}
				else
					Error("MakeSrcAndDstForSwap - no inv loc");
				break;
			}

			case InventoryCommandType.FORCESWAP:
			{
				Error("[syncinv] ForceSwap TODO");
				break;
			}

			case InventoryCommandType.DESTROY:
			{
				src.ReadFromContext(ctx);
				syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms received cmd=" + typename.EnumToString(InventoryCommandType, type) + " src=" + src.DumpToString());

				if (remote && !src.GetItem())
				{
					Error("[syncinv] remote input (cmd=DESTROY) dropped, item not in bubble");
					break; // not in bubble
				}

				if (false == GameInventory.CheckDropRequest(GetManOwner(), src.GetItem()))
				{
					Error("[cheat] man=" + GetManOwner() + " is cheating with cmd=" + typename.EnumToString(InventoryCommandType, type) + " src=" + src.DumpToString());
					return false; // cheater
				}

				GetGame().ObjectDelete(src.GetItem());
				break;
			}
		}

		if (!juncture && !remote && GetDayZPlayerOwner().GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
		{
			syncDebugPrint("[syncinv] forward to remotes cmd=" + typename.EnumToString(InventoryCommandType, type));
			GetDayZPlayerOwner().StoreInputForRemotes(ctx); // @NOTE: needs to be called _after_ the operation
		}

		return true;
	}
	
	override bool TakeToDst (InventoryMode mode, notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		switch (mode)
		{
			case InventoryMode.SERVER:
			{				
				if (RedirectToHandEvent(mode, src, dst))
					return true;

				if (GetDayZPlayerOwner().NeedInventoryJunctureFromServer(src.GetItem(), src.GetParent(), dst.GetParent()))
				{
					syncDebugPrint("[syncinv] DZPI::Take2Dst(" + typename.EnumToString(InventoryMode, mode) + ") need juncture src=" + src.DumpToString() + " dst=" + dst.DumpToString());
					
					if (GetGame().AddInventoryJuncture(GetDayZPlayerOwner(), src.GetItem(), dst, true, GameInventory.c_InventoryReservationTimeoutMS))
						syncDebugPrint("[syncinv] DZPI::Take2Dst(" + typename.EnumToString(InventoryMode, mode) + ") got juncture");
					else
						syncDebugPrint("[syncinv] DZPI::Take2Dst(" + typename.EnumToString(InventoryMode, mode) + ") got juncture");
				}

				syncDebugPrint("[syncinv] DZPI::Take2Dst(" + typename.EnumToString(InventoryMode, mode) + " server sync move src=" + src.DumpToString() + " dst=" + dst.DumpToString());
				ScriptInputUserData ctx = new ScriptInputUserData;
				InventoryInputUserData.SerializeMove(ctx, InventoryCommandType.SYNC_MOVE, src, dst);
	
				GetDayZPlayerOwner().SendSyncJuncture(DayZPlayerSyncJunctures.SJ_INVENTORY, ctx);
				syncDebugPrint("[syncinv] store input for remote - DZPI::Take2Dst(" + typename.EnumToString(InventoryMode, mode) + " server sync move src=" + src.DumpToString() + " dst=" + dst.DumpToString());
				GetDayZPlayerOwner().StoreInputForRemotes(ctx); // @TODO: is this right place? maybe in HandleInputData(server=true, ...)
				return true;
			}
		}
		return super.TakeToDst(mode, src, dst);
	}

	/**@fn			NetSyncCurrentStateID
	 * @brief		Engine callback - network synchronization of FSM's state. not intended to direct use.
	 **/
	override void NetSyncCurrentStateID (int id)
	{
		super.NetSyncCurrentStateID(id);

		GetDayZPlayerOwner().GetItemAccessor().OnItemInHandsChanged();
	}

	/**@fn			OnAfterStoreLoad
	 * @brief		engine reaction to load from database
	 *	originates in:
	 *				engine - Person::BinLoad
	 *				script - PlayerBase.OnAfterStoreLoad
	 **/
	override void OnAfterStoreLoad ()
	{
		GetDayZPlayerOwner().GetItemAccessor().OnItemInHandsChanged();
	}

	/**@fn			OnEventFromRemoteWeapon
	 * @brief		reaction of remote weapon to (common user/anim/...) event sent from server
	 **/
	bool OnEventFromRemoteWeapon (ParamsReadContext ctx)
	{
		if (GetEntityInHands())
		{
			Weapon_Base wpn = Weapon_Base.Cast(GetEntityInHands());
			if (wpn)
			{
				wpn.OnEventFromRemote(ctx);
			}
			else
				Error("OnEventFromRemoteWeapon - entity in hands, but not weapon. item=" + GetEntityInHands());
		}
		else
			Error("OnEventFromRemoteWeapon - no entity in hands");
		return true;
	}

	/**@fn			OnSyncFromRemoteWeapon
	 * @brief		stable state synchronization from weapon on server
	 **/
	bool OnSyncFromRemoteWeapon (ParamsReadContext ctx)
	{
		syncDebugPrint("[syncinv] remote weapon handling of stable-sync-event from server");

		if (GetEntityInHands())
		{
			Weapon_Base wpn = Weapon_Base.Cast(GetEntityInHands());
			if (wpn)
			{
				wpn.OnSyncFromRemote(ctx);
			}
			else
				Error("OnSyncFromRemoteWeapon - entity in hands, but not weapon. item=" + GetEntityInHands());
		}
		else
			Error("OnSyncFromRemoteWeapon - no entity in hands");
		return true;
	}
	
	/**@fn			OnHandEventFromRemote
	 * @brief		reaction of remote weapon to (common user/anim/...) event sent from server
	 **/
	bool OnHandEventFromRemote (ParamsReadContext ctx)
	{
		HandEventBase e = HandEventBase.CreateHandEventFromContext(ctx);
		if (e)
		{
			hndDebugSpam("[hndfsm] recv event from remote: created event=" + e);
			m_FSM.ProcessEvent(e);
			/*if (e.GetEventID() == WeaponEventID.HUMANCOMMAND_ACTION_ABORTED)
			{
				ProcessEventResult aa;
				m_FSM.ProcessAbortEvent(e, aa);
			}
			else
			{
				m_FSM.ProcessEvent(e);
			}*/

			return true;
		}
		return false;
	}

	void SyncHandEventToRemote (HandEventBase e)
	{
		DayZPlayer p = GetDayZPlayerOwner();
		if (p && p.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
		{
			ScriptRemoteInputUserData ctx = new ScriptRemoteInputUserData;

			ctx.Write(INPUT_UDT_HAND_REMOTE_EVENT);
			e.WriteToContext(ctx);

			hndDebugPrint("[hndfsm] send 2 remote: sending e=" + e + " id=" + e.GetEventID() + " p=" + p + "  m=" + e.m_Entity);
			p.StoreInputForRemotes(ctx);
		}
	}
};

