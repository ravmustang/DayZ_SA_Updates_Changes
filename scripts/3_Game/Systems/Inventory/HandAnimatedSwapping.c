class HandSwappingAnimated_Show extends HandStartAction
{
	EntityAI m_OldEntity; /// entity to be hidden
	EntityAI m_NewEntity; /// entity to be taken

	void HandSwapingAnimated_Show (Man player = NULL, HandStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{ }

	override void OnEntry (HandEventBase e)
	{
		if (m_OldEntity && m_NewEntity)
		{
			Man player = e.m_Player;
			EntityAI itemInHands = m_OldEntity;
			EntityAI itemToHands = m_NewEntity;
			hndDebugPrint("[hndfsm] HandSwappingAnimated_Show IH=" + itemInHands + "I2H=" + itemToHands);

			HandActionSwap.Swap(player, itemInHands, itemToHands);
		}
		else
			Error("[hndfsm] HandSwappingAnimated_Show m_OldEntity=" + m_OldEntity + " or m_NewEntity=" + m_NewEntity + " is NULL");

		super.OnEntry(e);
	}

	override void OnAbort (HandEventBase e)
	{
		m_OldEntity = null;
		m_NewEntity = null;
		super.OnAbort(e);
	}

	override void OnExit (HandEventBase e)
	{
		m_OldEntity = null;
		m_NewEntity = null;
		super.OnExit(e);
	}

	override bool IsWaitingForActionFinish () { return true; }
};


class HandAnimatedSwapping extends HandStateBase
{
	EntityAI m_OldEntity; /// entity to be hidden
	EntityAI m_NewEntity; /// entity to be taken

	ref HandStartHidingAnimated m_Hide;
	ref HandSwappingAnimated_Show m_Show;

	void HandAnimatedSwapping (Man player = NULL, HandStateBase parent = NULL)
	{
		// setup nested state machine
		m_Hide = new HandStartHidingAnimated(player, this, WeaponActions.HIDE, -1);
		m_Show = new HandSwappingAnimated_Show(player, this, WeaponActions.SHOW, -1);

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
		m_OldEntity = e.m_Player.GetHumanInventory().GetEntityInHands();
		m_NewEntity = e.m_Entity;

		HandEventSwap es = HandEventSwap.Cast(e);
		if (es)
		{
			m_Show.m_OldEntity = m_OldEntity;
			m_Show.m_NewEntity = m_NewEntity;

			m_Hide.m_ActionType = es.m_AnimationID;
			m_Show.m_ActionType = es.m_Animation2ID;
		}

		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}

	override void OnAbort (HandEventBase e)
	{
		m_OldEntity = null;
		m_NewEntity = null;

		super.OnAbort(e);
	}

	override void OnExit (HandEventBase e)
	{
		m_OldEntity = null;
		m_NewEntity = null;

		super.OnExit(e);
	}
};

