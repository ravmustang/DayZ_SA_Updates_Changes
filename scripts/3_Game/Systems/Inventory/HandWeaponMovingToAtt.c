class HandStartHidingWeapon extends HandStartAction
{
	void HandStartHidingWeapon (Man player = NULL, HandStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{ }

	override void OnEntry (HandEventBase e)
	{
		super.OnEntry(e);
	}

	override void OnAbort (HandEventBase e)
	{
		super.OnAbort(e);
	}

	override void OnExit (HandEventBase e)
	{
		super.OnExit(e);
	}
};


class HandWeaponMoveToDst_W4T extends HandStartAction
{
	ref InventoryLocation m_Dst;

	override void OnEntry (HandEventBase e)
	{
		Man player = e.m_Player;
		EntityAI item = m_Dst.GetItem();

		if (m_Dst && m_Dst.IsValid())
		{
			InventoryLocation src = new InventoryLocation;
			if (item.GetInventory().GetCurrentInventoryLocation(src))
			{
				GameInventory.LocationSyncMoveEntity(src, m_Dst);

				player.OnItemInHandsChanged();
			}
			else
				Error("[hndfsm] HandWeaponMoveToDst_W4T- item " + item + " has no Inventory or Location, inv=" + item.GetInventory());
		}
		else
			Error("[hndfsm] HandWeaponMoveToDst_W4T - event is not HandWeaponEventAnimationChanged");

		super.OnEntry(e);
	}

	override void OnAbort (HandEventBase e)
	{
		m_Dst = NULL;
		super.OnAbort(e);
	}

	override void OnExit (HandEventBase e)
	{
		m_Dst = NULL;
		super.OnExit(e);
	}

	override bool IsWaitingForActionFinish () { return true; }
};

class HandWeaponMovingToAtt extends HandStateBase
{
	EntityAI m_Weapon; /// weapon to be taken

	ref HandStartHidingWeapon m_Hide;
	ref HandWeaponMoveToDst_W4T m_Show;

	void HandWeaponMovingToAtt (Man player = NULL, HandStateBase parent = NULL)
	{
		// setup nested state machine
		m_Hide = new HandStartHidingWeapon(player, this, WeaponActions.HIDE, -1);
		m_Show = new HandWeaponMoveToDst_W4T(player, this, WeaponActions.SHOW, -1);

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
		m_Weapon = e.m_Entity;
		HandEventMoveTo ev_move = HandEventMoveTo.Cast(e);
		if (ev_move)
		{
			m_Show.m_Dst = ev_move.m_Dst;

			int animType = SlotToAnimType(ev_move.m_Dst);
			if (animType != -1)
			{
				m_Hide.m_ActionType = animType;
				m_Show.m_ActionType = animType;
			}
			else
				Error("[hndfsm] HandWeaponMovingToAtt - slot not mapped to anim type");
		}
		
		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}

	override void OnAbort (HandEventBase e)
	{
		m_Weapon = null;

		super.OnAbort(e);
	}

	override void OnExit (HandEventBase e)
	{
		m_Weapon = null;

		super.OnExit(e);
	}
};


