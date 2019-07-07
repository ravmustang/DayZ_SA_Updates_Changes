// load magazine composite state (attach + load chamber)
class WeaponAttachingMagStartAction extends WeaponStartAction
{
	Magazine m_newMagazine; /// magazine that will be removed from inventory
	ref InventoryLocation m_newSrc;

	void WeaponAttachingMagStartAction (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_newMagazine = NULL;
		m_newSrc = NULL;
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		
		if (!m_newSrc.IsValid())
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory m_newSrc=invalid, item not in bubble?");

		if (m_newMagazine && m_newSrc && m_newSrc.IsValid())
		{
			if (m_newSrc.GetType() == InventoryLocationType.GROUND)
			{
				// move to LH
				InventoryLocation lhand = new InventoryLocation;
				lhand.SetAttachment(e.m_player, m_newMagazine, InventorySlots.LEFTHAND);
				if (GameInventory.LocationSyncMoveEntity(m_newSrc, lhand))
				{
					wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, ok - new magazine removed from inv (inv->LHand)");
				}
				else
					Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, error - cannot new remove mag from inv");
			}
		}
		else
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, error - no magazines configured for replace (m_old=m_new=NULL)");
	}

	override void OnAbort (WeaponEventBase e)
	{
		if (m_newMagazine && m_newSrc)
		{
			if (DayZPlayerUtils.HandleDropMagazine(e.m_player, m_newMagazine))
				wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, ok - aborting, new magazine dropped to ground");
			else
				Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, error - cannot abort removal from inv (of new mag)");

			m_weapon.HideMagazine(); // force hide on abort
		}

		m_newMagazine = NULL;
		m_newSrc = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_newMagazine = NULL;
		m_newSrc = NULL;
		super.OnExit(e);
	}

	override bool SaveCurrentFSMState (ParamsWriteContext ctx)
	{
		if (!super.SaveCurrentFSMState(ctx))
			return false;

		if (!OptionalLocationWriteToContext(m_newSrc, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttachingMagStartAction.SaveCurrentFSMState: cannot write m_newSrc for weapon=" + m_weapon);
			return false;
		}
		return true;
	}

	override bool LoadCurrentFSMState (ParamsReadContext ctx, int version)
	{
		if (!super.LoadCurrentFSMState(ctx, version))
			return false;

		if (!OptionalLocationReadFromContext(m_newSrc, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttachingMagStartAction.LoadCurrentFSMState: cannot read m_newSrc for weapon=" + m_weapon);
			return false;
		}
		return true;
	}
};


class RemoveNewMagazineFromInventory extends WeaponStateBase
{
	Magazine m_newMagazine; /// magazine that will be removed from inventory
	ref InventoryLocation m_newSrc;

	void RemoveNewMagazineFromInventory (Weapon_Base w = NULL, WeaponStateBase parent = NULL)
	{
		m_newMagazine = NULL;
		m_newSrc = NULL;
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		
		if (!m_newSrc.IsValid())
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory m_newSrc=invalid, item not in bubble?");

		if (m_newMagazine && m_newSrc && m_newSrc.IsValid())
		{
			InventoryLocation curr = new InventoryLocation;
			m_newMagazine.GetInventory().GetCurrentInventoryLocation(curr);

			if (m_newSrc.GetType() == InventoryLocationType.GROUND && curr.GetType() == InventoryLocationType.ATTACHMENT && curr.GetSlot() == InventorySlots.LEFTHAND)
			{
				// already in LH
			}
			else
			{
				InventoryLocation lhand = new InventoryLocation;
				lhand.SetAttachment(e.m_player, m_newMagazine, InventorySlots.LEFTHAND);
				if (GameInventory.LocationSyncMoveEntity(m_newSrc, lhand))
				{
					wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, ok - new magazine removed from inv (inv->LHand)");
				}
				else
					Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, error - cannot new remove mag from inv");
			}
		}
		else
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, error - no magazines configured for replace (m_old=m_new=NULL)");
	}

	override void OnAbort (WeaponEventBase e)
	{
		if (m_newMagazine && m_newSrc)
		{
			//bool is_single_or_server = !GetGame().IsMultiplayer() || GetGame().IsServer();
			//if (is_single_or_server)
			//{
				if (DayZPlayerUtils.HandleDropMagazine(e.m_player, m_newMagazine))
					wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, ok - aborting, new magazine dropped to ground");
				else
					Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, error - cannot abort removal from inv (of new mag)");
			//}

			m_weapon.HideMagazine(); // force hide on abort
		}

		m_newMagazine = NULL;
		m_newSrc = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_weapon.ShowMagazine();

		m_newMagazine = NULL;
		m_newSrc = NULL;
		super.OnExit(e);
	}

	override bool SaveCurrentFSMState (ParamsWriteContext ctx)
	{
		if (!super.SaveCurrentFSMState(ctx))
			return false;

		if (!ctx.Write(m_newMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory.SaveCurrentFSMState: cannot write m_newMagazine for weapon=" + m_weapon);
			return false;
		}

		if (!OptionalLocationWriteToContext(m_newSrc, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory.SaveCurrentFSMState: cannot write m_newSrc for weapon=" + m_weapon);
			return false;
		}
		return true;
	}

	override bool LoadCurrentFSMState (ParamsReadContext ctx, int version)
	{
		if (!super.LoadCurrentFSMState(ctx, version))
			return false;

		if (!ctx.Read(m_newMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory.LoadCurrentFSMState: cannot read m_newMagazine for weapon=" + m_weapon);
			return false;
		}

		if (!OptionalLocationReadFromContext(m_newSrc, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory.LoadCurrentFSMState: cannot read m_newSrc for weapon=" + m_weapon);
			return false;
		}
		return true;
	}
};


class RemoveNewMagazineFromInventory_OnEntryShowMag extends RemoveNewMagazineFromInventory
{
	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		m_weapon.ShowMagazine();
	}
};


class WeaponAttachMagazine extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;
	Magazine m_newMagazine; /// source of the cartridge

	ref WeaponAttachingMagStartAction m_start;
	ref RemoveNewMagazineFromInventory_OnEntryShowMag m_getNew;
	ref AttachNewMagazine m_attach;
	ref WeaponChamberFromAttMag_W4T m_chamber;
	ref WeaponCharging_CK m_onCK;
	ref WeaponEjectCasing m_eject;

	void WeaponAttachMagazine (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;

		// setup nested state machine
		m_start = new WeaponAttachingMagStartAction(m_weapon, this, m_action, m_actionType);
		m_eject = new WeaponEjectCasing(m_weapon, this);
		m_getNew = new RemoveNewMagazineFromInventory_OnEntryShowMag(m_weapon, this);
		m_attach = new AttachNewMagazine(m_weapon, this);
		m_chamber = new WeaponChamberFromAttMag_W4T(m_weapon, this);
		m_onCK = new WeaponCharging_CK(m_weapon, this);

		// events: MS, MA, BE, CK
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase __ms_ = new WeaponEventAnimMagazineShow;
		WeaponEventBase __so_ = new WeaponEventAnimSliderOpen;
		WeaponEventBase __ma_ = new WeaponEventAnimMagazineAttached;
		//WeaponEventBase __be_ = new WeaponEventAnimBulletEject;
		WeaponEventBase __ck_ = new WeaponEventAnimCocked;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm

		m_fsm.AddTransition(new WeaponTransition(   m_start, __ms_, m_getNew));
		m_fsm.AddTransition(new WeaponTransition(   m_start, __so_, m_eject));
		m_fsm.AddTransition(new WeaponTransition(   m_eject, __ms_, m_getNew));
		m_fsm.AddTransition(new WeaponTransition(  m_getNew, __ma_, m_attach));
		m_fsm.AddTransition(new WeaponTransition(  m_attach, __ck_, m_chamber, NULL, new GuardAnd(new WeaponGuardChamberEmpty(m_weapon), new WeaponGuardHasAmmo(m_weapon)))); // when opened, there is no __be_ event
		m_fsm.AddTransition(new WeaponTransition(  m_attach, __ck_, m_onCK, NULL, new GuardAnd(new WeaponGuardChamberEmpty(m_weapon), new GuardNot(new WeaponGuardHasAmmo(m_weapon)))));
		m_fsm.AddTransition(new WeaponTransition(  m_attach, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition( m_chamber, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition( m_onCK, _fin_, NULL));
		

		m_fsm.SetInitialState(m_start);
	}

	override void OnEntry (WeaponEventBase e)
	{
		m_newMagazine = e.m_magazine;
		m_getNew.m_newMagazine = m_newMagazine;
		m_getNew.m_newSrc = new InventoryLocation;
		m_getNew.m_newMagazine.GetInventory().GetCurrentInventoryLocation(m_getNew.m_newSrc);
		m_start.m_newMagazine = m_newMagazine;
		m_start.m_newSrc = new InventoryLocation;
		m_start.m_newMagazine.GetInventory().GetCurrentInventoryLocation(m_start.m_newSrc);

		InventoryLocation il = new InventoryLocation;
		il.SetAttachment(m_weapon, m_newMagazine, InventorySlots.MAGAZINE);
		//wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttach, attaching mag=" + m_newMagazine.ToString() +  "to loc=" + il.ToString());
		m_attach.m_newMagazine = m_newMagazine;
		m_attach.m_newDst = il;

		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}

	override void OnAbort (WeaponEventBase e)
	{
		m_newMagazine = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_newMagazine = NULL;

		super.OnExit(e);
	}

	override bool SaveCurrentFSMState (ParamsWriteContext ctx)
	{
		if (!super.SaveCurrentFSMState(ctx))
			return false;

		if (!ctx.Write(m_newMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttachMagazine.SaveCurrentFSMState: cannot write m_newMagazine for weapon=" + m_weapon);
			return false;
		}
		return true;
	}

	override bool LoadCurrentFSMState (ParamsReadContext ctx, int version)
	{
		if (!super.LoadCurrentFSMState(ctx, version))
			return false;

		if (!ctx.Read(m_newMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponAttachMagazine.LoadCurrentFSMState: cannot read m_newMagazine for weapon=" + m_weapon);
			return false;
		}
		return true;
	}
};

