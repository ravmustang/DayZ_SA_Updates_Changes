class HandTakingAnimated_Hide extends HandStartAction
{ };

class HandTakingAnimated_Show extends HandStartAction
{
	ref InventoryLocation m_Src;

	void HandTakingAnimated_Show (Man player = NULL, HandStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
	{
		m_Src = NULL;
	}

	override void OnEntry (HandEventBase e)
	{
		super.OnEntry(e);

		if (m_Src)
		{
			if (m_Src.IsValid())
			{
				HandActionTake.TakeToHands(e.m_Player, m_Src);
			}
			else
			{
				Error("[hndfsm] HandTakingAnimated_Show m_Src=invalid, item not in bubble?");
			}
		}
		else
			Error("[hndfsm] HandTakingAnimated_Show, error - m_Src not configured");
	}

	override void OnAbort (HandEventBase e)
	{
		m_Src = NULL;
		super.OnAbort(e);
	}

	override void OnExit (HandEventBase e)
	{
		m_Src = NULL;
		super.OnExit(e);
	}

	override bool IsWaitingForActionFinish () { return true; }
};


class HandAnimatedTakingFromAtt extends HandStateBase
{
	EntityAI m_Entity; /// entity to be taken

	ref HandTakingAnimated_Hide m_Hide;
	ref HandTakingAnimated_Show m_Show;

	void HandAnimatedTakingFromAtt (Man player = NULL, HandStateBase parent = NULL)
	{
		// setup nested state machine
		m_Hide = new HandTakingAnimated_Hide(player, this, WeaponActions.HIDE, -1);
		m_Show = new HandTakingAnimated_Show(player, this, WeaponActions.SHOW, -1);

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
		m_Entity = e.m_Entity;
		InventoryLocation il = new InventoryLocation;
		m_Entity.GetInventory().GetCurrentInventoryLocation(il);
		m_Show.m_Src = il;

		m_Hide.m_ActionType = e.GetAnimationID();
		m_Show.m_ActionType = e.GetAnimationID();

		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}

	override void OnAbort (HandEventBase e)
	{
		m_Entity = null;

		super.OnAbort(e);
	}

	override void OnExit (HandEventBase e)
	{
		m_Entity = null;

		super.OnExit(e);
	}
};

