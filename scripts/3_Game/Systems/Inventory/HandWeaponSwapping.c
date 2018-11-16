class HandSwappingWeapon_Show extends HandStartAction
{
	EntityAI m_OldWeapon; /// weapon to be hidden
	EntityAI m_NewWeapon; /// weapon to be taken

	void HandSwapingWeapon_Show (Man player = NULL, HandStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{ }

	override void OnEntry (HandEventBase e)
	{
		if (m_OldWeapon && m_NewWeapon)
		{
			Man player = e.m_Player;
			EntityAI itemInHands = m_OldWeapon;
			EntityAI itemToHands = m_NewWeapon;

			hndDebugPrint("[hndfsm] HandSwappingWeapon_Show IH=" + itemInHands + "I2H=" + itemToHands);
			HandActionSwap.Swap(player, itemInHands, itemToHands);
		}
		else
		{
			Error("[hndfsm] HandSwappingWeapon_Show m_OldWeapon=" + m_OldWeapon + " or m_NewWeapon=" + m_NewWeapon + " is NULL");
		}

		super.OnEntry(e);
	}

	override void OnAbort (HandEventBase e)
	{
		m_OldWeapon = null;
		m_NewWeapon = null;
		super.OnAbort(e);
	}

	override void OnExit (HandEventBase e)
	{
		m_OldWeapon = null;
		m_NewWeapon = null;
		super.OnExit(e);
	}

	override bool IsWaitingForActionFinish () { return true; }
};



class HandWeaponSwapping extends HandStateBase
{
	EntityAI m_OldWeapon; /// weapon to be hidden
	EntityAI m_NewWeapon; /// weapon to be taken

	ref HandStartHidingWeapon m_Hide;
	ref HandSwappingWeapon_Show m_Show;

	void HandWeaponSwapping (Man player = NULL, HandStateBase parent = NULL)
	{
		// setup nested state machine
		m_Hide = new HandStartHidingWeapon(player, this, WeaponActions.HIDE, -1);
		m_Show = new HandSwappingWeapon_Show(player, this, WeaponActions.SHOW, -1);

		// events:
		HandEventBase _fin_ = new HandEventHumanCommandActionFinished;
		HandEventBase _AEh_ = new HandAnimEventChanged;

		m_FSM = new HandFSM(this); // @NOTE: set owner of the submachine fsm

		m_FSM.AddTransition(new HandTransition(   m_Hide, _AEh_,   m_Show ));
		m_FSM.AddTransition(new HandTransition(   m_Show, _fin_,     NULL ));
		
		m_FSM.SetInitialState(m_Hide);
	}

	override void OnEntry (HandEventBase e)
	{
		m_OldWeapon = e.m_Player.GetHumanInventory().GetEntityInHands();
		m_NewWeapon = e.m_Entity;
		HandEventSwap ev = HandEventSwap.Cast(e);
		if (ev)
		{
			InventoryLocation src = new InventoryLocation;
			if (m_NewWeapon.GetInventory().GetCurrentInventoryLocation(src))
			{
				m_Show.m_OldWeapon = m_OldWeapon;
				m_Show.m_NewWeapon = m_NewWeapon;

				int animTypeDst = SlotToAnimType(src);
				if (animTypeDst != -1)
				{
					m_Hide.m_ActionType = animTypeDst;
				}
				else
					Error("[hndfsm] HandWeaponSwapping - dst slot not mapped to anim type");

				int animTypeSrc = SlotToAnimType(src);
				if (animTypeSrc != -1)
				{
					m_Show.m_ActionType = animTypeSrc;
				}
				else
					Error("[hndfsm] HandWeaponSwapping - src slot not mapped to anim type");

			}
			else
				Error("[hndfsm] HandWeaponSwapping - new weapon has no inv location");
		}
		
		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}

	override void OnAbort (HandEventBase e)
	{
		m_OldWeapon = null;
		m_NewWeapon = null;

		super.OnAbort(e);
	}

	override void OnExit (HandEventBase e)
	{
		m_OldWeapon = null;
		m_NewWeapon = null;

		super.OnExit(e);
	}
};





