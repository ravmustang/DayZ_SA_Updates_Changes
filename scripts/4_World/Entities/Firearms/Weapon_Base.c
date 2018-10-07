/**@class		AbilityRecord
 * @brief		pair ( action, actionType )
 **/
class AbilityRecord
{
	int m_action; /// corresponds to Human::actions == RELOAD, MECHANISM, ...
	int m_actionType; /// corresponds to Human::actionTypes == CHAMBERING_ONEBULLET_CLOSED, MECHANISM_CLOSED...
	void AbilityRecord (int a, int at) { m_action = a; m_actionType = at; }
};

typedef FSMTransition<WeaponStateBase, WeaponEventBase, WeaponActionBase, WeaponGuardBase> WeaponTransition; /// shorthand

/**@class		Weapon_Base
 * @brief		script base for all weapons
 *
 * @NOTE: this class is bound to core-config "Weapon_Base" config class
 **/
class Weapon_Base extends Weapon
{
	protected ref WeaponFSM m_fsm;	/// weapon state machine
	protected ref array<ref AbilityRecord> m_abilities;		/// weapon abilities
	protected int m_weaponAnimState; /// animation state the weapon is in, -1 == uninitialized
	protected bool m_receivedSyncFromRemote; /// if remote weapon, this flag waits for first stable state sync
	protected bool m_canEnterIronsights = true; /// if false, weapon goes straight into optics view
	protected ref array<string> m_ironsightsExcludingOptics; /// optics that go straight into optics view (skip ironsights)
	ref array<float> 	m_DOFProperties;
	protected ref PropertyModifiers m_PropertyModifierObject;
	protected int m_RecoilSeed;

	void Weapon_Base ()
	{
		m_weaponAnimState = -1;
		m_receivedSyncFromRemote = false;
		m_abilities = new array<ref AbilityRecord>;
		m_ironsightsExcludingOptics = new array<string>;
		m_DOFProperties = new array<float>;
		
		InitExcludedScopesArray(m_ironsightsExcludingOptics);
		InitDOFProperties(m_DOFProperties);
		
		InitStateMachine();
	}

	void InitStateMachine () { }

	bool CanProcessAction (int action, int actionType)
	{
		return false; // @TODO
	}
	/**@fn		HasActionAbility
	 * @brief	query if weapon supports action and actionType
	 * @param[in]	action	\p	one of Human.actions (i.e. RELOAD, MECHANISM, ...)
	 * @param[in]	actionType	\p	one of Human.actionTypes (i.e. CHAMBERING_ONEBULLET_CLOSED, MECHANISM_CLOSED...)
	 * @return	true if weapon supports operation
	 **/
	bool HasActionAbility (int action, int actionType)
	{
		int count = GetAbilityCount();
		for (int i = 0; i < count; ++i)
		{
			AbilityRecord rec = GetAbility(i);
			if (rec.m_action == action && rec.m_actionType == actionType)
				return true;
		}
		return false;
	}
	/**@fn		GetAbilityCount
	 * @return number of stored abilities
	 **/
	int GetAbilityCount () { return m_abilities.Count(); }
	/**@fn		GetAbility
	 * @param[in]	index	\p	index into m_abilities storage
	 * @return ability record
	 **/
	AbilityRecord GetAbility (int index) { return m_abilities.Get(index); }

	/**@fn		CanProcessWeaponEvents
	 * @return	true if weapon has running fsm
	 **/
	bool CanProcessWeaponEvents () { return m_fsm && m_fsm.IsRunning(); }

	/**@fn		GetCurrentState
	 * @brief		returns currently active state
	 * @return	current state the FSM is in (or NULL)
	 **/
	WeaponStateBase GetCurrentState () { return m_fsm.GetCurrentState(); }

	/**@fn		IsWaitingForActionFinish
	 * @brief	returns true if state machine started playing action/actionType and waits for finish
	 **/
	bool IsWaitingForActionFinish ()
	{
		return CanProcessWeaponEvents() && GetCurrentState().IsWaitingForActionFinish();
	}

	bool IsIdle ()
	{
		return CanProcessWeaponEvents() && GetCurrentState().IsIdle();
	}

	/**@fn	ProcessWeaponEvent
	 * @brief	weapon's fsm handling of events
	 * @NOTE: warning: ProcessWeaponEvent can be called only within DayZPlayer::HandleWeapons (or ::CommandHandler)
	 **/
	bool ProcessWeaponEvent (WeaponEventBase e)
	{
		SyncEventToRemote(e);

		if (m_fsm.ProcessEvent(e) == ProcessEventResult.FSM_OK)
			return true;

		//wpnDebugPrint("FSM refused to process event (no transition): src=" + GetCurrentState().ToString() + " event=" + e.ToString());
		return false;
	}
	/**@fn	ProcessWeaponAbortEvent
	 * @NOTE: warning: ProcessWeaponEvent can be called only within DayZPlayer::HandleWeapons (or ::CommandHandler)
	 **/
	bool ProcessWeaponAbortEvent (WeaponEventBase e)
	{
		SyncEventToRemote(e);
		
		ProcessEventResult aa;
		m_fsm.ProcessAbortEvent(e, aa);
		return aa == ProcessEventResult.FSM_OK;
	}

	bool CanChamberBullet (int muzzleIndex, Magazine mag)
	{
		return CanChamberFromMag(muzzleIndex, mag) && (!IsChamberFull(muzzleIndex) || IsChamberFiredOut(muzzleIndex)) );
	}

	void SetWeaponAnimState (int state)
	{
		m_weaponAnimState = state;
	}
	void ResetWeaponAnimState ()
	{
		fsmDebugSpam("[wpnfsm] resetting anim state: " + typename.EnumToString(PistolAnimState, m_weaponAnimState) + " --> " + typename.EnumToString(PistolAnimState, -1));
		m_weaponAnimState = -1;
	}
	int GetWeaponAnimState () { return m_weaponAnimState; }

	override void EEFired (int muzzleType, int mode, string ammoType)
	{
		super.EEFired(muzzleType, mode, ammoType);
		
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
		{
			// Muzzle flash & overheating effects
			ItemBase suppressor = GetAttachedSuppressor();
			ItemBase.PlayFireParticles(this, ammoType, this, suppressor, "CfgWeapons" );
			IncreaseOverheating(this, ammoType, this, suppressor, "CfgWeapons");
			
			if (suppressor)
			{
				ItemBase.PlayFireParticles(this, ammoType, suppressor, NULL, "CfgVehicles" );
				suppressor.IncreaseOverheating(this, ammoType, this, suppressor, "CfgVehicles");
			}
		}
		
		#ifdef DEVELOPER
		MiscGameplayFunctions.UnlimitedAmmoDebugCheck(this);
		#endif
	}
	
	void SyncSelectionState (bool has_bullet, bool has_mag)
	{					
		if (has_bullet)
			SelectionBulletShow();
		else
			SelectionBulletHide();
	
		if (has_mag)
			SelectionMagazineShow();
		else
			SelectionMagazineHide();
	}

	void OnStableStateEntry ()
	{
		PlayerBase p = PlayerBase.Cast(GetHierarchyParent());
		if (p && p.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
		{
			int mi = GetCurrentMuzzle();

			float ammoDamage = 0.0;
			string ammoTypeName;
			GetCartridgeInfo(mi, ammoDamage, ammoTypeName);

			InventoryLocation loc = new InventoryLocation;
			Magazine mag = GetMagazine(mi);
			if (mag)
				mag.GetInventory().GetCurrentInventoryLocation(loc);

			ScriptRemoteInputUserData ctx = new ScriptRemoteInputUserData;

			ctx.Write(INPUT_UDT_WEAPON_REMOTE_SYNC);
			ctx.Write(GetCurrentStateID());
			ctx.Write(ammoDamage);
			ctx.Write(ammoTypeName);
			loc.WriteToContext(ctx);

			wpnDebugPrint("[wpnfsm] OnStableStateEntry - sending state to remote");
			p.StoreInputForRemotes(ctx);
			
		}
		/*if (p)
		{
			p.OnWeaponActionEnd();
		}*/
		//MWBREAK
	}

	void OnSyncFromRemote (ParamsReadContext ctx)
	{
		if (m_receivedSyncFromRemote)
		{
			wpnDebugSpam("[wpnfsm] already initialized, ignoring sync-from-remote packet");
			return;
		}
		else
		{
			wpnDebugSpam("[wpnfsm] first sync-from-remote packet");
			m_receivedSyncFromRemote = true;

			int currStateID = -1;
			if (!ctx.Read(currStateID))
			{
				Error("Weapon::SyncRemote - cannot read currStateID!");
				return;
			}

			float ammoDamage = 0.0;
			if (!ctx.Read(ammoDamage))
			{
				Error("Weapon::SyncRemote - cannot read ammoDamage!");
				return;
			}
			string ammoTypeName;
			if (!ctx.Read(ammoTypeName))
			{
				Error("Weapon::SyncRemote - cannot read ammoTypeName!");
				return;
			}

			InventoryLocation loc = new InventoryLocation;
			if (!loc.ReadFromContext(ctx))
			{
				Error("Weapon::SyncRemote - cannot read mag!");
				return;
			}

			if (currStateID != GetCurrentStateID())
			{
				wpnDebugPrint("[wpnfsm] first sync-from-remote packet and state is different, syncing!");
				NetSyncCurrentStateID(currStateID);

				int mi = GetCurrentMuzzle();
				LoadChamber(mi, ammoDamage, ammoTypeName);

				if (loc.IsValid())
				{
					Magazine mag = Magazine.Cast(loc.GetItem());
					InventoryLocation src = new InventoryLocation;
					mag.GetInventory().GetCurrentInventoryLocation(src);

					GameInventory.LocationSyncMoveEntity(src, loc);
				}
				else
					Error("Weapon::SyncRemote - inventory location of mag is not valid!");
			}
		}
	}

	override void OnStoreLoad (ParamsReadContext ctx)
	{
		super.OnStoreLoad(ctx);
		if (m_fsm)
		{
			m_fsm.OnStoreLoad(ctx);
		}
		else
		{
			int dummy = 0;
			ctx.Read(dummy);
		}
	}
	
	void AfterStoreLoad ()
	{
		if (m_fsm)
		{
			int mi = GetCurrentMuzzle();
			Magazine mag = GetMagazine(mi);
			bool has_mag = mag != null;
			bool has_bullet = !IsChamberEmpty(mi);
			SyncSelectionState(has_bullet, has_mag);
		}
	}

	override void OnStoreSave (ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		if (m_fsm)
			m_fsm.OnStoreSave(ctx);
		else
		{
			int dummy = 0;
			ctx.Write(dummy);
		}
	}

	/**@fn		GetCurrentStateID
	 * @brief	tries to return identifier of current stable state
	 **/
	int GetCurrentStateID ()
	{
		if (m_fsm)
			return m_fsm.GetCurrentStateID();
		return 0;
	}

	/**@fn		RandomizeFSMState
	 * @brief	Engine callback - loot randomization of FSM's state. not intended to direct use.
	 **/
	protected void RandomizeFSMState ()
	{
		if (m_fsm)
		{
			int mi = GetCurrentMuzzle();
			Magazine mag = GetMagazine(mi);
			bool has_mag = mag != null;
			bool has_bullet = !IsChamberEmpty(mi);
			bool has_jam = IsChamberJammed(mi);
			m_fsm.RandomizeFSMState(has_bullet, has_mag, has_jam);
			SyncSelectionState(has_bullet, has_mag);
		}
	}

	/**@fn		NetSyncCurrentStateID
	 * @brief	Engine callback - network synchronization of FSM's state. not intended to direct use.
	 **/
	void NetSyncCurrentStateID (int id)
	{
		if (m_fsm)
		{
			m_fsm.NetSyncCurrentStateID(id);
			
			WeaponStableState state = WeaponStableState.Cast(GetCurrentState());
			if (state)
			{
				SyncSelectionState(state.HasBullet(), state.HasMagazine());
				state.SyncAnimState();
			}
		}
	}

	/**
	 * @brief	Load posible actions for wepon from config
	 **/
	override void SetUserActions ()
	{
		g_Game.ConfigGetIntArray("cfgWeapons " + GetType() + " ContinuousActions", m_ContinuousActions);
		g_Game.ConfigGetIntArray("cfgWeapons " + GetType() + " SingleUseActions", m_SingleUseActions);
		g_Game.ConfigGetIntArray("cfgWeapons " + GetType() + " InteractActions", m_InteractActions);
	}
	/**
	 * @brief Returns number of slots for attachments corrected for weapons
	 **/
	override int GetSlotsCountCorrect ()
	{
		int ac = GetInventory().AttachmentCount();
		int	sc = GetInventory().GetAttachmentSlotsCount() + GetMuzzleCount();
		if (ac > sc) sc = ac; // fix of some weapons which has 1 attachments but 0 slots...
		return sc;
	};

	PropertyModifiers GetPropertyModifierObject () { return m_PropertyModifierObject; }

	bool CreatePropertyModifierObject ()
	{
		if (!m_PropertyModifierObject)
		{
			m_PropertyModifierObject = new PropertyModifiers(this);
			return true;
		}
		return false;
	}

	bool RemovePropertyModifierObject ()
	{
		if (m_PropertyModifierObject)
		{
			delete m_PropertyModifierObject;
			return true;
		}
		return false;
	}
	
	void SetRecoilSeed (int seed) { m_RecoilSeed = seed; }
	int GetRecoilSeed () { return m_RecoilSeed; }

	override void OnInventoryEnter (Man player)
	{
		super.OnInventoryEnter(player);

		CreatePropertyModifierObject();
	}
	
	override void OnInventoryExit (Man player)
	{
		super.OnInventoryExit(player);

		RemovePropertyModifierObject();
	}

	override void EEItemAttached (EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if ( GetPropertyModifierObject() ) 	GetPropertyModifierObject().UpdateModifiers();
		if ( ItemOptics.Cast(item) ) 		OpticsAllowsIronsightsCheck(ItemOptics.Cast(item));
	}

	override void EEItemDetached (EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if ( GetPropertyModifierObject() ) 	GetPropertyModifierObject().UpdateModifiers();
		if ( ItemOptics.Cast(item) ) 		m_canEnterIronsights = true;
	}
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner,new_owner);
		
		// HACK "resets" optics memory on optics
		PlayerBase player;
		if ( PlayerBase.CastTo(player,old_owner) )
		{ 
			player.SetReturnToOptics(false);
		}
	}

	bool IsRemoteWeapon ()
	{
		InventoryLocation il = new InventoryLocation;
		if (GetInventory().GetCurrentInventoryLocation(il))
		{
			EntityAI parent = il.GetParent();
			DayZPlayer dayzp = DayZPlayer.Cast(parent);
			if (il.GetType() == InventoryLocationType.HANDS && dayzp)
			{
				bool remote = dayzp.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_REMOTE;
				return remote;
			}
		}
		return true;
	}

	void OnEventFromRemote (ParamsReadContext ctx)
	{
		WeaponEventBase e = CreateWeaponEventFromContext(ctx);
		if (e)
		{
			PlayerBase player = PlayerBase.Cast(GetHierarchyParent());
			player.GetWeaponManager().SetRunning(true);

			fsmDebugSpam("[wpnfsm] recv event from remote: created event=" + e);
			if(e.GetEventID() == WeaponEventID.HUMANCOMMAND_ACTION_ABORTED)
			{
				ProcessEventResult aa;
				m_fsm.ProcessAbortEvent(e, aa);
			}
			else
			{
				m_fsm.ProcessEvent(e);
			}
			player.GetWeaponManager().SetRunning(false);
		}
	}

	void SyncEventToRemote (WeaponEventBase e)
	{
		DayZPlayer p = DayZPlayer.Cast(GetHierarchyParent());
		if (p && p.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
		{
			ScriptRemoteInputUserData ctx = new ScriptRemoteInputUserData;

			ctx.Write(INPUT_UDT_WEAPON_REMOTE_EVENT);
			e.WriteToContext(ctx);

			wpnDebugPrint("[wpnfsm] send 2 remote: sending e=" + e + " id=" + e.GetEventID() + " p=" + e.m_player + "  m=" + e.m_magazine);
			p.StoreInputForRemotes(ctx);
		}
	}

	RecoilBase SpawnRecoilObject ()
	{
		return new DefaultRecoil(this);
	}

	int GetWeaponSpecificCommand (int weaponAction, int subCommand) { return subCommand; }

	bool CanFire ()
	{
		if (!IsChamberEmpty(GetCurrentMuzzle()) && !IsChamberFiredOut(GetCurrentMuzzle()) && !IsChamberJammed(GetCurrentMuzzle()))
			return true;
		return false;
	}
	
	override int GetItemWeight()
	{
		int AttachmentWeight = 0;
		float item_wetness = GetWet();
		int AttachmentsCount = GetInventory().AttachmentCount();
		int ConfWeight = ConfigGetInt("weight");
		
		for (int i = 0; i < AttachmentsCount; i++)
		{
			AttachmentWeight += GetInventory().GetAttachmentFromIndex(i).GetItemWeight();
		}
		
		return Math.Round((item_wetness + 1) * (ConfWeight + AttachmentWeight));
	}
	
	//! Initializes list of optics, that do not allow ironsights camera for the weapon
	bool InitExcludedScopesArray(out array<string> temp_array)
	{
		if (GetGame().ConfigIsExisting("cfgWeapons " + GetType() + " ironsightsExcludingOptics"))
		{
			GetGame().ConfigGetTextArray("cfgWeapons " + GetType() + " ironsightsExcludingOptics",temp_array);
			return true;
		}
		return false;
	}
	
	//! Checks if attached optic allows ironsights, sets m_canEnterIronsights accordingly
	void OpticsAllowsIronsightsCheck(ItemOptics optic)
	{
		if (!optic)
		{
			m_canEnterIronsights = true;
			return;
		}
		string type = optic.GetType();
		int can;
		can = m_ironsightsExcludingOptics.Find(type);
		if (can > -1)
		{
			m_canEnterIronsights = false;
			return;
		}
		
		m_canEnterIronsights = true;
	}
	
	bool CanEnterIronsights()
	{
		return m_canEnterIronsights;
	}
	
	//! Initializes DOF properties for weapon's ironsight/optics cameras
	bool InitDOFProperties(out array<float> temp_array)
	{
		if (GetGame().ConfigIsExisting("cfgWeapons " + GetType() + " PPDOFProperties"))
		{
			GetGame().ConfigGetFloatArray("cfgWeapons " + GetType() + " PPDOFProperties",temp_array);
			return true;
		}
		return false;
	}
	
	ref array<float> GetWeaponDOF()
	{
		return m_DOFProperties;
	}
};

