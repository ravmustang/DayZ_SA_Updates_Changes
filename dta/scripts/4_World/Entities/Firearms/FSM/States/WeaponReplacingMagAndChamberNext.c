/**@class		DetachOldMagazine
 * @brief		detaches old magazine from weapon and stores it in left hand (LH)
 **/
class DetachOldMagazine extends WeaponStateBase
{
	Magazine m_oldMagazine; /// magazine that will be detached
	ref InventoryLocation m_oldSrc;

	void DetachOldMagazine (Weapon_Base w = NULL, WeaponStateBase parent = NULL)
	{
		m_oldMagazine = NULL;
		m_oldSrc = NULL;
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		
		if (!m_oldSrc.IsValid())
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " DetachOldMagazine m_oldSrc=invalid, item not in bubble?");

		if (m_oldMagazine && m_oldSrc)
		{
			InventoryLocation lhand = new InventoryLocation;
			lhand.SetAttachment(e.m_player, m_oldMagazine, InventorySlots.LEFTHAND);
			if (GameInventory.LocationSyncMoveEntity(m_oldSrc, lhand))
			{
				wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " DetachOldMagazine, ok - old magazine removed from wpn (old->LHand)");
			}
			else
				Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " DetachOldMagazine, error - cannot remove old mag from wpn");
		}
		else
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " DetachOldMagazine, error - no magazines configured for replace (m_old=m_oldSrc=NULL)");
	}

	override void OnAbort (WeaponEventBase e)
	{
		if (m_oldMagazine && m_oldSrc)
		{
			if (DayZPlayerUtils.HandleDropMagazine(e.m_player, m_oldMagazine))
				wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " DetachOldMagazine, ok - aborting, old magazine dropped to ground");
			else
				Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " DetachOldMagazine, error - cannot abort removal from wpn (of old mag)");

			m_weapon.HideMagazine(); // force hide on abort
		}

		m_oldMagazine = NULL;
		m_oldSrc = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_oldMagazine = NULL;
		m_oldSrc = NULL;
		super.OnExit(e);
	}
	
	override bool SaveCurrentFSMState (ParamsWriteContext ctx)
	{
		if (!super.SaveCurrentFSMState(ctx))
			return false;

		if (!ctx.Write(m_oldMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " DetachOldMagazine.SaveCurrentFSMState: cannot write m_oldMagazine for weapon=" + m_weapon);
			return false;
		}

		if (!OptionalLocationWriteToContext(m_oldSrc, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " DetachOldMagazine.SaveCurrentFSMState: cannot write m_oldSrc for weapon=" + m_weapon);
			return false;
		}
		return true;
	}

	override bool LoadCurrentFSMState (ParamsReadContext ctx, int version)
	{
		if (!super.LoadCurrentFSMState(ctx, version))
			return false;

		if (!ctx.Read(m_oldMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " DetachOldMagazine.LoadCurrentFSMState: cannot read m_oldMagazine for weapon=" + m_weapon);
			return false;
		}
		if (!OptionalLocationReadFromContext(m_oldSrc, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " DetachOldMagazine.LoadCurrentFSMState: cannot read m_oldSrc for weapon=" + m_weapon);
			return false;
		}
		return true;
	}
};

/**@class		OldMagazineHide
 * @brief		hides old magazine, but keep it in LH
 * @note		on abort, old magazine is dropped to ground
 **/
class OldMagazineHide : MagazineHide
{
	Magazine m_oldMagazine; /// detached magazine that will be dropped on abort

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
	}

	override void OnAbort (WeaponEventBase e)
	{
		if (DayZPlayerUtils.HandleDropMagazine(e.m_player, m_oldMagazine))
			wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " OldMagazineHide, ok - aborting, old magazine dropped to ground");
		else
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " OldMagazineHide, error - cannot abort removal from wpn (of new mag)");

		m_oldMagazine = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_oldMagazine = NULL;

		super.OnExit(e);
	}

	override bool SaveCurrentFSMState (ParamsWriteContext ctx)
	{
		if (!super.SaveCurrentFSMState(ctx))
			return false;

		if (!ctx.Write(m_oldMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " OldMagazineHide.SaveCurrentFSMState: cannot write m_oldMagazine for weapon=" + m_weapon);
			return false;
		}
		return true;
	}

	override bool LoadCurrentFSMState (ParamsReadContext ctx, int version)
	{
		if (!super.LoadCurrentFSMState(ctx, version))
			return false;

		if (!ctx.Read(m_oldMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " OldMagazineHide.LoadCurrentFSMState: cannot read m_oldMagazine for weapon=" + m_weapon);
			return false;
		}
		return true;
	}
};

/**@class		SwapOldAndNewMagazine
 * @brief		old magazine to inventory, new to left hand
 *
 * stores old magazine in left hand somewhere in inventory (or drop to ground if necessary)
 * and moves new magazine to left hand.
 * on abort new magazine is dropped to ground.
 **/
class SwapOldAndNewMagazine extends WeaponStateBase
{
	Magazine m_newMagazine; /// magazine that will be dropped on abort
	Magazine m_oldMagazine; /// detached magazine that will be stored in inventory
	ref InventoryLocation m_newSrc;
	ref InventoryLocation m_newDst;
	ref InventoryLocation m_oldSrc;
	ref InventoryLocation m_oldDst;

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		
		if (!m_oldMagazine || !m_oldDst || !m_newSrc.IsValid() || !m_oldDst.IsValid())
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, error - state misconfigured for replace! " + " m_newMagazine=" + Object.GetDebugName(m_newMagazine) + " m_oldMagazine=" + Object.GetDebugName(m_oldMagazine) + " m_newSrc=" + InventoryLocation.DumpToStringNullSafe(m_newSrc) + " m_oldDst=" + InventoryLocation.DumpToStringNullSafe(m_oldDst) + " m_oldDst=" + InventoryLocation.DumpToStringNullSafe(m_oldDst));
		}

		if (m_oldMagazine && m_oldDst)
		{
			m_weapon.ShowMagazine();
			InventoryLocation lhand_old = new InventoryLocation;
			lhand_old.SetAttachment(e.m_player, m_oldMagazine, InventorySlots.LEFTHAND);
			InventoryLocation lhand_new = new InventoryLocation;
			lhand_new.SetAttachment(e.m_player, m_newMagazine, InventorySlots.LEFTHAND);

			// 0) if magazines are of the same size AND new one is not on gnd, swap them
			if (m_oldDst.CompareLocationOnly(m_newSrc) && m_newSrc.GetType() != InventoryLocationType.GROUND)
			{
				InventoryLocation src1 = m_newSrc;
				InventoryLocation dst1 = lhand_new;
				InventoryLocation src2 = lhand_old;
				InventoryLocation dst2 = m_oldDst;
				
				if (GameInventory.LocationSwap(src1, src2, dst1, dst2))
				{
					wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, ok - old swapped with one in inventory (LH<->inv)");
					return;
				}
				else
				{
					Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, error - cannot swap magazines");
				}	
			}
			
			bool already_in_lhand = m_newSrc.GetType() == InventoryLocationType.ATTACHMENT && m_newSrc.GetSlot() == InventorySlots.LEFTHAND;
			
			// 1) else first remove new magazine from inv (if not already in LH)
			if (!already_in_lhand)
			{
				GameInventory.LocationRemoveEntity(m_newSrc);
			}

			// 2) try to store old mag
			bool stored = false;
			InventoryLocation oldDst = new InventoryLocation;
			if (e.m_player.GetInventory().FindFreeLocationForEx(m_oldMagazine, FindInventoryLocationType.ANY, m_oldSrc, oldDst)) // @NOTE: excluding m_oldSrc from candidates
			{
				wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, selected oldDst=" + InventoryLocation.DumpToStringNullSafe(oldDst));
				if (GameInventory.LocationSyncMoveEntity(lhand_old, oldDst))
				{
					wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, ok - old magazine stored in inventory (LH->inv)");
					stored = true;
				}
				else
				{
					wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, error - old magazine NOT stored in inventory, trying to find another location...");
				}
			}

			if (!stored)
			{
				wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, dropping old on ground (fall-through case)");
				DayZPlayerUtils.HandleDropMagazine(e.m_player, m_oldMagazine);
			}
		
			// 3) move new mag to LH
			if (!already_in_lhand)
			{
				GameInventory.LocationAddEntity(lhand_new);
				m_weapon.ShowMagazine();
			}
		}
		else
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, error - state misconfigured for replace! " + " m_newMagazine=" + Object.GetDebugName(m_newMagazine) + " m_oldMagazine=" + Object.GetDebugName(m_oldMagazine) + " m_newSrc=" + InventoryLocation.DumpToStringNullSafe(m_newSrc) + " m_oldDst=" + InventoryLocation.DumpToStringNullSafe(m_oldDst) + " m_oldDst=" + InventoryLocation.DumpToStringNullSafe(m_oldDst));
	}
	
	override void OnAbort (WeaponEventBase e)
	{
		wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, aborting, old magazine already stored");
		m_weapon.HideMagazine();

		if (m_newMagazine && DayZPlayerUtils.HandleDropMagazine(e.m_player, m_newMagazine))
			wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, ok - aborting, new magazine dropped to ground");
		else
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine, error - cannot abort removal from wpn (of new mag)");

		m_newMagazine = NULL;
		m_oldMagazine = NULL;
		m_newSrc = NULL;
		m_newDst = NULL;
		m_oldSrc = NULL;
		m_oldDst = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_newMagazine = NULL;
		m_oldMagazine = NULL;
		m_newSrc = NULL;
		m_newDst = NULL;
		m_oldSrc = NULL;
		m_oldDst = NULL;

		super.OnExit(e);
	}
	
	override bool SaveCurrentFSMState (ParamsWriteContext ctx)
	{
		if (!super.SaveCurrentFSMState(ctx))
			return false;

		if (!ctx.Write(m_newMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.SaveCurrentFSMState: cannot write m_newMagazine for weapon=" + m_weapon);
			return false;
		}
		
		if (!ctx.Write(m_oldMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.SaveCurrentFSMState: cannot write m_oldMagazine for weapon=" + m_weapon);
			return false;
		}

		if (!OptionalLocationWriteToContext(m_newSrc, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.SaveCurrentFSMState: cannot write m_newSrc for weapon=" + m_weapon);
			return false;
		}
		if (!OptionalLocationWriteToContext(m_newDst, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.SaveCurrentFSMState: cannot write m_newDst for weapon=" + m_weapon);
			return false;
		}
		
		if (!OptionalLocationWriteToContext(m_oldSrc, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.SaveCurrentFSMState: cannot write m_oldSrc for weapon=" + m_weapon);
			return false;
		}
		if (!OptionalLocationWriteToContext(m_oldDst, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.SaveCurrentFSMState: cannot write m_oldDst for weapon=" + m_weapon);
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
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.LoadCurrentFSMState: cannot read m_newMagazine for weapon=" + m_weapon);
			return false;
		}
		if (!ctx.Read(m_oldMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.LoadCurrentFSMState: cannot read m_oldMagazine for weapon=" + m_weapon);
			return false;
		}
		if (!OptionalLocationReadFromContext(m_newSrc, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.LoadCurrentFSMState: cannot read m_newSrc for weapon=" + m_weapon);
			return false;
		}
		if (!OptionalLocationReadFromContext(m_newDst, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.LoadCurrentFSMState: cannot read m_newDst for weapon=" + m_weapon);
			return false;
		}

		if (!OptionalLocationReadFromContext(m_oldSrc, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.LoadCurrentFSMState: cannot read m_oldSrc for weapon=" + m_weapon);
			return false;
		}
		if (!OptionalLocationReadFromContext(m_oldDst, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " SwapOldAndNewMagazine.LoadCurrentFSMState: cannot read m_oldDst for weapon=" + m_weapon);
			return false;
		}
		return true;
	}
};

/**@class		AttachNewMagazine
 * @brief		attach mag in LH into weapon
 **/
class AttachNewMagazine extends WeaponStateBase
{
	Magazine m_newMagazine; /// new magazine from inventory that will be attached
	ref InventoryLocation m_newDst;

	void AttachNewMagazine (Weapon_Base w = NULL, WeaponStateBase parent = NULL)
	{
		m_newMagazine = NULL;
		m_newDst = NULL;
	}
	
	override bool IsWaitingForActionFinish ()
	{
		return true;
	}

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);

		if (m_newMagazine && m_newDst)
		{
			InventoryLocation lhand = new InventoryLocation;
			lhand.SetAttachment(e.m_player, m_newMagazine, InventorySlots.LEFTHAND);
			if (GameInventory.LocationSyncMoveEntity(lhand, m_newDst))
			{
				wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " AttachNewMagazine, ok - attached new magazine (LHand->dst)");
			}
			else
			{
				wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " AttachNewMagazine, error - cannot attach new magazine!");
			}
		}
		else
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " AttachNewMagazine, error - no magazines configured for replace (m_new=NULL)");
	}

	override void OnAbort (WeaponEventBase e)
	{
		wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " AttachNewMagazine, aborting, but attached new magazine already in place");
		m_newMagazine = NULL;
		m_newDst = NULL;

		super.OnAbort(e);
	}

	override void OnExit (WeaponEventBase e)
	{
		m_newMagazine = NULL;
		m_newDst = NULL;
		super.OnExit(e);
	}

	override bool SaveCurrentFSMState (ParamsWriteContext ctx)
	{
		if (!super.SaveCurrentFSMState(ctx))
			return false;

		if (!ctx.Write(m_newMagazine))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " AttachNewMagazine.SaveCurrentFSMState: cannot write m_newMagazine for weapon=" + m_weapon);
			return false;
		}

		if (!OptionalLocationWriteToContext(m_newDst, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " AttachNewMagazine.SaveCurrentFSMState: cannot write m_newDst for weapon=" + m_weapon);
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
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " AttachNewMagazine.LoadCurrentFSMState: cannot read m_newMagazine for weapon=" + m_weapon);
			return false;
		}
		if (!OptionalLocationReadFromContext(m_newDst, ctx))
		{
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " AttachNewMagazine.LoadCurrentFSMState: cannot read m_newDst for weapon=" + m_weapon);
			return false;
		}
		return true;
	}
};

class AttachNewMagazine_W4T extends AttachNewMagazine
{
	override bool IsWaitingForActionFinish () { return true; }
};


/**@class		WeaponReplacingMagAndChamberNext
 * @brief		replace current magazine with new one
 *
 * replace magazine, i.e. eject curr magazine from weapon, attach another (store the old one) 
 * and chamber next bullet from magazine (if possible)
 **/
class WeaponReplacingMagAndChamberNext extends WeaponStateBase
{
	WeaponActions m_action;
	int m_actionType;

	ref WeaponStartAction m_start;
	ref DetachOldMagazine m_detach;
	ref OldMagazineHide m_hideOld;
	ref SwapOldAndNewMagazine m_swapMags;
	ref AttachNewMagazine_W4T m_attach;
	ref WeaponChamberFromAttMag_W4T m_chamber;
	ref WeaponCharging_CK m_onCK;
	ref WeaponEjectCasing m_eject;

	// substates configuration
	Magazine m_oldMagazine; /// magazine that will be detached
	Magazine m_newMagazine; /// magazine that will be attached
	ref InventoryLocation m_oldSrc;
	ref InventoryLocation m_newSrc;
	ref InventoryLocation m_oldDst;
	ref InventoryLocation m_newDst;

	void WeaponReplacingMagAndChamberNext (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_action = action;
		m_actionType = actionType;
		m_newMagazine = NULL;

		// setup nested state machine
		m_start = new WeaponStartAction(m_weapon, this, m_action, m_actionType);
		m_eject = new WeaponEjectCasing(m_weapon,this);
		m_detach = new DetachOldMagazine(m_weapon, this);
		m_hideOld = new OldMagazineHide(m_weapon, this);
		m_swapMags = new SwapOldAndNewMagazine(m_weapon, this);
		m_attach = new AttachNewMagazine_W4T(m_weapon, this);
		m_chamber = new WeaponChamberFromAttMag_W4T(m_weapon, this);
		m_onCK = new WeaponCharging_CK(m_weapon, this);

		// events
		WeaponEventBase __so_ = new WeaponEventAnimSliderOpen;
		WeaponEventBase __md_ = new WeaponEventAnimMagazineDetached;
		WeaponEventBase __mh_ = new WeaponEventAnimMagazineHide;
		WeaponEventBase __ms_ = new WeaponEventAnimMagazineShow;
		WeaponEventBase __ma_ = new WeaponEventAnimMagazineAttached;
		WeaponEventBase __ck_ = new WeaponEventAnimCocked;
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
		m_fsm.AddTransition(new WeaponTransition(   m_start, __md_, m_detach));
		m_fsm.AddTransition(new WeaponTransition(   m_start, __so_, m_eject));
		m_fsm.AddTransition(new WeaponTransition(   m_eject, __md_, m_detach));
		m_fsm.AddTransition(new WeaponTransition(  m_detach, __mh_, m_hideOld));
		m_fsm.AddTransition(new WeaponTransition( m_hideOld, __ms_, m_swapMags));
		m_fsm.AddTransition(new WeaponTransition(m_swapMags, __ma_, m_attach));
		m_fsm.AddTransition(new WeaponTransition(  m_attach, __ck_, m_chamber, NULL, new GuardAnd(new WeaponGuardChamberEmpty(m_weapon), new WeaponGuardHasAmmo(m_weapon))));
		//m_fsm.AddTransition(new WeaponTransition(  m_attach, __ck_, m_onCK, NULL, new GuardAnd(new WeaponGuardChamberEmpty(m_weapon), new GuardNot(new WeaponGuardHasAmmo(m_weapon)))));
		m_fsm.AddTransition(new WeaponTransition(  m_attach, __ck_, m_onCK));
		m_fsm.AddTransition(new WeaponTransition(  m_attach, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition( m_chamber, _fin_, NULL));
		m_fsm.AddTransition(new WeaponTransition(    m_onCK, _fin_, NULL));

		m_fsm.SetInitialState(m_start);
	}
	
	override void OnEntry (WeaponEventBase e)
	{
		int mi = m_weapon.GetCurrentMuzzle();
		m_oldMagazine = m_weapon.GetMagazine(mi);
		m_newMagazine = e.m_magazine;

		wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponReplacingMagAndChamberNext, m_oldMagazine=" + m_oldMagazine + " m_newMagazine=" + m_newMagazine);

		if (m_oldMagazine && m_newMagazine)
		{
			m_oldSrc = new InventoryLocation;
			m_newSrc = new InventoryLocation;
			m_newDst = new InventoryLocation;
			m_oldDst = new InventoryLocation;

			if (!m_newMagazine.GetInventory().GetCurrentInventoryLocation(m_newSrc))
			{
				Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponReplacingMagAndChamberNext cannot get curr inv loc of NEW mag=" + Object.GetDebugName(m_newMagazine));
			}
			if (!m_oldMagazine.GetInventory().GetCurrentInventoryLocation(m_oldSrc))
			{
				Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponReplacingMagAndChamberNext cannot get curr inv loc of OLD mag=" + Object.GetDebugName(m_oldMagazine));
			}

			m_newMagazine.GetInventory().GetCurrentInventoryLocation(m_newDst);
			m_newDst.CopyLocationFrom(m_oldSrc);

			if (magazinesHaveEqualSizes(m_oldMagazine, m_newMagazine))
			{
				m_oldMagazine.GetInventory().GetCurrentInventoryLocation(m_oldDst);
				m_oldDst.CopyLocationFrom(m_newSrc);
			}
			else
			{
				wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponReplacingMagAndChamberNext, inequal sizes, state will handle this");
			}
			
			if (m_newSrc.GetType() == InventoryLocationType.GROUND)
			{
				// move to LH
				InventoryLocation lhand = new InventoryLocation;
				lhand.SetAttachment(e.m_player, m_newMagazine, InventorySlots.LEFTHAND);
				if (GameInventory.LocationSyncMoveEntity(m_newSrc, lhand))
				{
					m_newSrc = lhand;
					wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, ok - new magazine removed from inv (inv->LHand)");
				}
				else
					Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " RemoveNewMagazineFromInventory, error - cannot new remove mag from inv");
			}

			m_detach.m_oldMagazine = m_oldMagazine;
			m_detach.m_oldSrc = m_oldSrc;

			m_hideOld.m_oldMagazine = m_oldMagazine;

			m_swapMags.m_newMagazine = m_newMagazine;
			m_swapMags.m_newSrc = m_newSrc;
			m_swapMags.m_newDst = m_newDst;
			m_swapMags.m_oldMagazine = m_oldMagazine;
			m_swapMags.m_oldSrc = m_oldSrc;
			m_swapMags.m_oldDst = m_oldDst;
			
			m_attach.m_newMagazine = m_newMagazine;
			m_attach.m_newDst = m_newDst;
		}
		else
			Error("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponReplacingMagAndChamberNext, error - no magazines configured for replace (m_old=m_new=NULL)");

		super.OnEntry(e); // @NOTE: super after submachine init (prevent override from submachine start)
	}
	override void OnExit (WeaponEventBase e)
	{
		m_oldMagazine = NULL;
		m_newMagazine = NULL;
		m_oldDst = NULL;
		m_newDst = NULL;
		m_oldSrc = NULL;
		m_newSrc = NULL;
		super.OnExit(e);
	}
};

