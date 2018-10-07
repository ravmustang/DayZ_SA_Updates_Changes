class HandTakingWeapon_Hide extends HandStartAction
{ };

int SlotToAnimType (notnull InventoryLocation src)
{
	if (src.GetType() == InventoryLocationType.ATTACHMENT)
	{
		switch (src.GetSlot())
		{
			/*case InventorySlots.SHOULDER:
				return WeaponHideShowTypes.HIDESHOW_SLOT_RFLLEFTBACK;
				//return WeaponHideShowTypes.HIDESHOW_SLOT_2HDLEFTBACK;
			
			case InventorySlots.MELEE:
				return WeaponHideShowTypes.HIDESHOW_SLOT_RFLRIGHTBACK;
				//return WeaponHideShowTypes.HIDESHOW_SLOT_2HDRIGHTBACK;
			
			case InventorySlots.HIPS:
				return WeaponHideShowTypes.HIDESHOW_SLOT_PISTOLBELT;
			
			case InventorySlots.VEST:
				return WeaponHideShowTypes.HIDESHOW_SLOT_PISTOLCHEST;
			
			case InventorySlots.BODY:
				return WeaponHideShowTypes.HIDESHOW_SLOT_PISTOLCHEST;
			
			case InventorySlots.VEST:
				return WeaponHideShowTypes.HIDESHOW_SLOT_KNIFEBACK;*/
			default:
				Error("[hndfsm] HandStartTakingWeapon, error - weapon in unknown slot");
		};
	}
	return -1;
}

class HandTakingWeapon_Show extends HandStartAction
{
	ref InventoryLocation m_Src;

	void HandTakingWeapon_Show (Man player = NULL, HandStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1)
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
				Error("[hndfsm] HandTakingWeapon_Show m_Src=invalid, item not in bubble?");
			}
		}
		else
			Error("[hndfsm] HandTakingWeapon_Show, error - m_Src not configured");
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


class HandWeaponTakingFromAtt extends HandStateBase
{
	EntityAI m_Weapon; /// weapon to be taken

	ref HandTakingWeapon_Hide m_Hide;
	ref HandTakingWeapon_Show m_Show;

	void HandWeaponTakingFromAtt (Man player = NULL, HandStateBase parent = NULL)
	{
		// setup nested state machine
		m_Hide = new HandTakingWeapon_Hide(player, this, WeaponActions.HIDE, -1);
		m_Show = new HandTakingWeapon_Show(player, this, WeaponActions.SHOW, -1);

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
		InventoryLocation il = new InventoryLocation;
		m_Weapon.GetInventory().GetCurrentInventoryLocation(il);
		m_Show.m_Src = il;

		int animType = SlotToAnimType(il);
		if (animType)
		{
			m_Hide.m_ActionType = animType;
			m_Show.m_ActionType = animType;
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


