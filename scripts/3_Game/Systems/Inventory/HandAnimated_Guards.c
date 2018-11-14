int SlotToAnimType (notnull Man player, notnull InventoryLocation src)
{
	if (src.GetType() == InventoryLocationType.ATTACHMENT)
	{
		switch (src.GetSlot())
		{
			case InventorySlots.SHOULDER:
			{
				if (src.GetItem() && src.GetItem().IsWeapon())
				{
					return WeaponHideShowTypes.HIDESHOW_SLOT_RFLLEFTBACK;
				}
				return WeaponHideShowTypes.HIDESHOW_SLOT_2HDLEFTBACK;
			}
			case InventorySlots.MELEE:
			{
				if (src.GetItem() && src.GetItem().IsWeapon())
				{
					return WeaponHideShowTypes.HIDESHOW_SLOT_RFLRIGHTBACK;
				}
				return WeaponHideShowTypes.HIDESHOW_SLOT_2HDRIGHTBACK;
			}
			case InventorySlots.PISTOL:
			{
				EntityAI parent_item = src.GetParent(); 		// belt
				Man owner;
				if (parent_item)
					owner = parent_item.GetHierarchyRootPlayer(); 		// player
				if (owner && owner.GetInventory().FindAttachment(InventorySlots.HIPS) == parent_item) // is the pistol in a belt holster?
				{
					return WeaponHideShowTypes.HIDESHOW_SLOT_PISTOLBELT;
				}
				return WeaponHideShowTypes.HIDESHOW_SLOT_PISTOLCHEST;
			}
			case InventorySlots.KNIFE:
				return WeaponHideShowTypes.HIDESHOW_SLOT_KNIFEBACK;
			
			case InventorySlots.VEST:
			case InventorySlots.FEET:
			case InventorySlots.BODY:
			case InventorySlots.LEGS:
			case InventorySlots.BACK:
				return WeaponHideShowTypes.HIDESHOW_SLOT_KNIFEBACK; // @NOTE: this is DUMMY for "generic take" anim
			
			default:
				Print("[hndfsm] SlotToAnimType -  not animated slot in src_loc=" + src.DumpToString());
		};
		//
		//if (InventorySlots.GetSlotIdFromString("Pistol"))
	}
	else if (src.GetType() == InventoryLocationType.CARGO)
	{
		return WeaponHideShowTypes.HIDESHOW_SLOT_KNIFEBACK; // @NOTE: placeholder
	}
	return -1;
}

bool SelectAnimationOfTakeToHands (notnull Man player, notnull InventoryLocation src, notnull InventoryLocation dst, out int animType)
{
	if (player.IsInTransport())
		return false;
	if (src.GetType() == InventoryLocationType.GROUND)
		return false;

	if (src.GetItem().GetHierarchyRootPlayer() == player)
	{	
		animType = SlotToAnimType(player, src);
		if (animType != -1)
		{
			hndDebugPrint("[hndfsm] SelectAnimationOfTakeToHands - selected animType=" + animType + " for item=" + src.GetItem());
			return true;
		}
	}
	hndDebugPrint("[hndfsm] SelectAnimationOfTakeToHands - no animation");
	return false;
}

bool SelectAnimationOfMoveFromHands (notnull Man player, notnull InventoryLocation src, notnull InventoryLocation dst, out int animType)
{
	if (player.IsInTransport())
		return false;

	if (src.GetItem().GetHierarchyRootPlayer() == player)
	{	
		animType = SlotToAnimType(player, dst);
		if (animType != -1)
		{
			hndDebugPrint("[hndfsm] SelectAnimationOfMoveFromHands guard - selected animType=" + animType + " for item=" + src.GetItem());
			return true;
		}
	}
	hndDebugPrint("[hndfsm] SelectAnimationOfMoveFromHands - no animation");
	return false;
}

bool SelectAnimationOfForceSwapInHands (notnull Man player, notnull InventoryLocation old_src, notnull InventoryLocation old_dst, notnull InventoryLocation new_src, notnull InventoryLocation new_dst, out int animType1, out int animType2)
{
	if (player.IsInTransport())
		return false;

	if (old_src.GetItem().GetHierarchyRootPlayer() == player && new_src.GetItem().GetHierarchyRootPlayer() == player)
	{
		animType1 = SlotToAnimType(player, old_dst);
		animType2 = SlotToAnimType(player, new_src);
		if (animType1 != -1 || animType2 != -1)
		{
			hndDebugPrint("[hndfsm] SelectAnimationOfForceSwapInHands guard - selected animType1=" + animType1 + " animType2=" + animType2 + " for old_item=" + old_src.GetItem() + " for new_item=" + new_src.GetItem());
			return true;
		}
	}
	hndDebugPrint("[hndfsm] SelectAnimationOfForceSwapInHands - no animation");
	return false;
}


class HandSelectAnimationOfTakeToHandsEvent extends HandGuardBase
{
	void HandSelectAnimationOfTakeToHandsEvent (Man p = NULL) { }

	override bool GuardCondition (HandEventBase e)
	{
		EntityAI eai = e.m_Entity;
		if (eai != NULL)
		{
			InventoryLocation src = new InventoryLocation;
			if (eai.GetInventory().GetCurrentInventoryLocation(src))
			{
				InventoryLocation dst = new InventoryLocation;
				dst.SetHands(e.m_Player, src.GetItem());
				int animType = -1;
				if (SelectAnimationOfTakeToHands(e.m_Player, src, dst, animType))
				{
					e.m_AnimationID = animType;
					return true;
				}
				return false;
			}
		}
		hndDebugPrint("[hndfsm] HandGuardHasAnimatedAtachmentInEvent guard - no entity in event");
		return false;
	}
};

class HandSelectAnimationOfMoveFromHandsEvent extends HandGuardBase
{
	protected Man m_Player;
	ref HandGuardHasRoomForItem m_HasRoomGuard = new HandGuardHasRoomForItem;

	void HandSelectAnimationOfMoveFromHandsEvent (Man p = NULL) { m_Player = p; }

	override bool GuardCondition (HandEventBase e)
	{
		if (m_HasRoomGuard.GuardCondition(e))
		{
			EntityAI eai = m_Player.GetHumanInventory().GetEntityInHands();
			if (eai)
			{
				InventoryLocation src = new InventoryLocation;
				if (eai.GetInventory().GetCurrentInventoryLocation(src))
				{
					int animType = -1;
					if (SelectAnimationOfMoveFromHands(e.m_Player, src, e.GetDst(), animType))
					{
						e.m_AnimationID = animType;
						return true;
					}
					return false;
				}
			}
		}
		return false;
	}
};

class HandSelectAnimationOfForceSwapInHandsEvent extends HandGuardBase
{
	protected Man m_Player;
	ref HandGuardHasRoomForItem m_HasRoomGuard = new HandGuardHasRoomForItem;

	void HandSelectAnimationOfForceSwapInHandsEvent (Man p = NULL) { m_Player = p; }

	bool ProcessSwapEvent (notnull HandEventBase e, notnull InventoryLocation old_dst, out int animType1, out int animType2)
	{
		EntityAI old_e = e.m_Player.GetHumanInventory().GetEntityInHands();
		EntityAI new_e = e.m_Entity;

		if (!old_e)
		{
			Error("[hndfsm] HandSelectAnimationOfForceSwapInHandsEvent - old_e (in hands) is null!");
			return false;
		}
		
		if (!new_e)
		{
			Error("[hndfsm] HandSelectAnimationOfForceSwapInHandsEvent - new_e (in hands) is null!");
			return false;
		}

		if (!old_dst || !old_dst.IsValid())
		{
			Error("[hndfsm] HandSelectAnimationOfForceSwapInHandsEvent - old_dst (dst of item in hands) is null (or invalid)!");
			return false;
		}

		InventoryLocation old_src = new InventoryLocation;
		if (!old_e.GetInventory().GetCurrentInventoryLocation(old_src))
		{
			Error("[hndfsm] HandSelectAnimationOfForceSwapInHandsEvent - old_e has no inv location");
			return false;
		}

		InventoryLocation new_src = new InventoryLocation;
		if (!new_e.GetInventory().GetCurrentInventoryLocation(new_src))
		{
			Error("[hndfsm] HandSelectAnimationOfForceSwapInHandsEvent - new_e has no inv location");
			return false;
		}

		InventoryLocation new_dst = new InventoryLocation;
		new_dst.SetHands(e.m_Player, new_src.GetItem());

		return SelectAnimationOfForceSwapInHands(e.m_Player, old_src, old_dst, new_src, new_dst, animType1, animType2);
	}

	override bool GuardCondition (HandEventBase e)
	{
		HandEventForceSwap es = HandEventForceSwap.Cast(e);
		if (es)
		{
			if (m_HasRoomGuard.GuardCondition(e))
			{
				int animType1 = -1;
				int animType2 = -1;
				if (ProcessSwapEvent(e, es.GetDst(), animType1, animType2))
				{
					e.m_AnimationID = animType1;
					es.m_Animation2ID = animType2;
					return true;
				}
			}
			else
				Error("[hndfsm] HandSelectAnimationOfForceSwapInHandsEvent - m_HasRoomGuard.GuardCondition failed");
		}
		else
			Error("[hndfsm] HandSelectAnimationOfForceSwapInHandsEvent - not a swap event");
		return false;
	}
};

class HandSelectAnimationOfSwapInHandsEvent extends HandSelectAnimationOfForceSwapInHandsEvent
{
	override bool GuardCondition (HandEventBase e)
	{
		HandEventSwap es = HandEventSwap.Cast(e);
		if (es)
		{
			int animType1 = -1;
			int animType2 = -1;
			if (ProcessSwapEvent(e, es.GetDst(), animType1, animType2))
			{
				e.m_AnimationID = animType1;
				es.m_Animation2ID = animType2;
				return true;
			}
		}
		else
			Error("[hndfsm] HandSelectAnimationOfSwapInHandsEvent - not a swap event");
		return false;
	}
};

