
enum InventoryCommandType
{
	MOVE,			///< generic move, may involve animations
	SYNC_MOVE,		///< synchronous move. action is finished immeadiately, no animations involved
	HAND_EVENT,		///< event for hands
	SWAP,			///< swap two entities (simple swap of compatible objects)
	FORCESWAP,		///< Forced swap two entities. First goes to second's place, second goes "somewhere else"
	DESTROY,		///< destroy of entity right in inventory
	REPLACE			///< replace of entity in inventory (@NOTE: hands goes through HAND_EVENT)
};
enum InventoryJunctureType
{
	TAKE,			///< taking from ground
	SWAP,			///< swapping from ground
	//LOAD,			///< load mag from ground
};
enum InventoryMode
{
	PREDICTIVE,		///< 'Predictive' means that the operation uses Client-Side Prediction, i.e. the action runs the same code on both, client AND server
	LOCAL,			///< 'Local' operation executes from where it is run
	JUNCTURE,		///< 'Juncture' operation is used whenever there is possibility of race condition, i.e. two players picking same item from ground
	SERVER,			///< 'Server' mode operation is required if and only if the operation runs only on server (creates and/or destroys objects)
};

/**@class		GameInventory
 * @brief		script counterpart to engine's class Inventory
 **/
class GameInventory
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///@{ Engine native functions

	/**
	 * @fn		GetInventoryOwner
	 * @return	entity associated with this inventory
	 **/
	proto native EntityAI GetInventoryOwner ();
	/**
	 * @fn		HasEntityInInventory
	 * @brief	query inventory if item is somewhere
	 *
	 * All available inventory locations (hands, cargo, attachments, proxycargo, ...) are
	 * traversed recursively from root to leafs.
	 *
	 * @param[in]	item	\p		item to be found in inventory
	 * @return	true if found, false otherwise
	 **/
	proto native bool HasEntityInInventory (notnull EntityAI item);

	/**
	 * @brief		EnumerateInventory
	 * @param[in]	tt		used traversal type
	 * @param[out]	items		items in inventory (in order determined by traversal)
	 * @return	true if found any, false otherwise
	 **/	
	proto native bool EnumerateInventory (InventoryTraversalType tt, out array<EntityAI> items);


	///@{ cargo
	proto native CargoBase GetCargo ();
	/**
	 * @brief Create Entity of specified type in cargo of entity
	 **/
	proto native EntityAI CreateEntityInCargo (string typeName);
	/**
	 * @brief	Create Entity of specified type in cargo of entity at coordinates (row, col)
	 * @param	typeName		type to create
	 * @param	row		the row of cargo to be created at
	 * @param	col		the column of cargo to be created at
	 **/
	proto native EntityAI CreateEntityInCargoEx (string typeName, int idx, int row, int col);

	proto native bool HasEntityInCargo (notnull EntityAI e);
	proto native bool HasEntityInCargoEx (notnull EntityAI e, int idx, int row, int col);
	proto native bool CanAddEntityInCargo (notnull EntityAI e);
	proto native bool CanAddEntityInCargoEx (notnull EntityAI e, int idx, int row, int col);
	//proto native bool AddEntityInCargo (notnull EntityAI owner, EntityAI cargo);
	//proto native bool AddEntityInCargoEx (notnull EntityAI owner, notnull EntityAI e, int idx, int row, int col);
	proto native bool CanRemoveEntityInCargo (notnull EntityAI e);
	proto native bool CanRemoveEntityInCargoEx (notnull EntityAI e, int idx, int row, int col);
	///@} cargo


	///@{ attachments
	/**@fn		GetSlotId
	 * @param	index		index into array of configured slots (@see GetSlotIdCount)
	 * @return	slot where this item belongs
	 **/
	proto native int GetSlotId (int index);
	/**@fn		GetSlotIdCount
	 * @return	number of slots this item can belong to
	 **/
	proto native int GetSlotIdCount ();
	/**@fn		GetAttachmentSlotId
	 * @param	index		index of the slot for attachment (@see GetAttachmentSlotsCount)
	 * @return	slot for attachment
	 **/
	proto native int GetAttachmentSlotId (int index);
	/**@fn		GetAttachmentSlotsCount
	 * @return	number of slots for attachments
	 **/
	proto native int GetAttachmentSlotsCount ();
	/**@fn		HasInventorySlot
	 * @return	true if this entity has inventory slot with id=slotId
	 **/
	proto native bool HasInventorySlot (int slotId);
	/**@fn		AttachmentCount
	 * @brief	Returns count of attachments attached to this item
	 **/
	proto native int AttachmentCount ();
	/**@fn		CreateAttachment
	 * @brief	Create Entity of specified type as attachment of entity
	 **/
	proto native EntityAI CreateAttachment (string typeName);
	/**@fn		CreateAttachmentEx
	 * @brief	Create Entity of specified type as attachment of entity
	 * @param	typeName		type to create
	 * @param	slotId		the slot to be created in
	 **/
	proto native EntityAI CreateAttachmentEx (string typeName, int slotId);
	/**@fn		GetAttachmentFromIndex
	 * @return	attached entity by attachment index
	 * @NOTE:	index is not slot! for getting attachment by slot use FindAttachment
	 **/
	proto native EntityAI GetAttachmentFromIndex (int index);
	/**
	 * @brief	Returns attached entity in slot (you can use InventorySlots.GetSlotIdFromString(name) to get slot id)
	 **/
	proto native EntityAI FindAttachment (int slot);
	/**@fn		HasAttachment
	 * @brief	brief Returns True if entity is attached to this
	 **/
	proto native bool HasAttachment (notnull EntityAI e);
	/**@fn		HasAttachmentEx
	 * @brief	brief Returns True if entity is attached to this in slot
	 **/
	proto native bool HasAttachmentEx (notnull EntityAI e, int slot);
	/**@fn		CanAddAttachment
	 * @brief	Check if attachment can be added to any slot
	 * @return	true if entity can be added as attachment
	 **/
	proto native bool CanAddAttachment (notnull EntityAI e);
	/**@fn		CanAddAttachmentEx
	 * @brief	Check if attachment can be added to slot
	 * @NOTE:	Note that slot index IS NOT slot ID! Slot ID is defined in DZ/data/config.cpp
	 **/
	proto native bool CanAddAttachmentEx (notnull EntityAI e, int slot);

	proto native bool CanRemoveAttachment (EntityAI attachment);
	proto native bool CanRemoveAttachmentEx (EntityAI attachment, int slot);

	//proto native bool RemoveAttachment(EntityAI attachment);
	//proto native bool RemoveAttachmentEx(EntityAI attachment, int slot);

	/**
	 * @brief	Returns placeholder entity for slot (naked arms, legs etc)
	 **/
	proto native EntityAI FindPlaceholderForSlot(int slot);
	proto native bool IsPlaceholderEntity (notnull Object e);
	///@} attachments


	/**
	 * @fn		GetCurrentInventoryLocation
	 * @brief	returns information about current item location
	 * @param[out]	loc	\p		current InventoryLocation
	 * @return	true if current location retrieved (and is valid)
	 **/
	proto native bool GetCurrentInventoryLocation (out notnull InventoryLocation loc);

	/**
	 * @brief		FindFreeLocationFor
	 * @param[in]	item		item that would be placed in inventory
	 * @param[out]	loc			the InventoryLocation the item can be placed to
	 * @return	true if found any, false otherwise
	 **/
	proto native bool FindFreeLocationFor (notnull EntityAI item, FindInventoryLocationType flags, out notnull InventoryLocation loc);
		/**
	 * @brief		FindFreeLocationForEx
	 * @param[in]	item		item that would be placed in inventory
	 * @param[out]	loc			the InventoryLocation the item can be placed to
	 * @return	true if found any, false otherwise
	 **/
	proto native bool FindFreeLocationForEx (notnull EntityAI item, FindInventoryLocationType flags, notnull InventoryLocation exclude, out notnull InventoryLocation loc);

	/**
	 * @brief		FindFirstFreeLocationForNewEntity
	 * @param[in]	item_type		item type that would be placed in inventory
	 * @param[out]	loc			the InventoryLocation the item can be placed to
	 * @return	true if found any, false otherwise
	 **/
	proto native bool FindFirstFreeLocationForNewEntity (string item_type, FindInventoryLocationType flags, out notnull InventoryLocation loc);

	/**@fn			FindFreeLocationsFor
	 * @brief		searches inventory for suitable location for @item
	 * @param[in]	item		item that would be placed in inventory
	 * @param[in]	flags		flags affecting selection ( @see FindInventoryLocation )
	 * @param[out]	locs		array of InventoryLocations the item can be placed to
	 * @return		number of suitable inventory locations
	 *
	 * Example:
	 * @code
	 *   Entity bino = GetGame().CreateObject("Binoculars", "3312 0 854");
	 *   EntityAI bino2;
	 *   if (Class.CastTo(bino2, bino))
	 *   {
	 *     array<ref InventoryLocation> a = new array<ref InventoryLocation>; // 1) allocate memory for array
	 *     for (int i = 0; i < 10; ++i)
	 *     {
	 *       a.Insert(new InventoryLocation); // 2) pre-allocate inventorylocations from script. engine only fills them in, engine does not allocate memory to avoid mixing.
	 *     }
	 *
	 *     int sz = player.FindFreeLocationsFor(bino, FindInventoryLocationType.ANY_CARGO | FindInventoryLocationType.ATTACHMENT, a); // 3) ask engine to fill the locations
	 *     if (sz > 0)
	 *     {
	 *       InventoryLocation src = new InventoryLocation;
	 *       bino2.GetCurrentInventoryLocation(src); // 5) get current location
	 *
	 *       InventoryLocation dst = a[0]; // 6) get first (this is only example)
	 *
	 *       ... use src and dst
	 *     }
	 *   }
	 * @endcode
	 **/
	proto native int FindFreeLocationsFor (notnull EntityAI item, FindInventoryLocationType flags, out notnull array<ref InventoryLocation> locs);

	/**
	 * @fn		LocationCreateEntity
	 * @brief	creates new item directly at location
	 * @param[in]	type	\p		item type to be placed in inventory
	 * @return	created entity, null otherwise
	 **/
	static proto native EntityAI LocationCreateEntity (notnull InventoryLocation inv_loc, string type);
	/**
	 * @fn		LocationCreateLocalEntity
	 * @brief	creates new <b>local</b> item directly at location
	 * 			@NOTE: the item is created localy, i.e. it's not registered to network
	 * @param[in]	type	\p		item type to be placed in inventory
	 * @return	created entity, null otherwise
	 **/	
	static proto native EntityAI LocationCreateLocalEntity (notnull InventoryLocation inv_loc, string type);
	/**
	 * @fn		LocationCanAddEntity
	 * @brief	queries if the entity contained in inv_loc.m_item can be added to ground/attachment/cargo/hands/...
	 * @return true if can be added, false otherwise
	 **/
	static proto native bool LocationCanAddEntity (notnull InventoryLocation inv_loc);
	
	/**
	 * @fn		LocationTestAddEntity
	 * @brief	test if the entity contained in inv_loc.m_item can be added to ground/attachment/cargo/hands/...
	 * @return true if can be added, false otherwise
	 **/
	static proto native bool LocationTestAddEntity (notnull InventoryLocation inv_loc, bool do_resevation_check, bool do_item_check, bool do_lock_check, bool do_occupancy_test, bool do_script_check);
	/**
	 * @fn		LocationCanRemoveEntity
	 * @brief	queries if the entity contained in inv_loc.m_item can be removed from ground/attachment/cargo/hands/...
	 * @return true if can be added, false otherwise
	 **/
	static proto native bool LocationCanRemoveEntity (notnull InventoryLocation inv_loc);
	/**
	 * @fn		LocationCanMoveEntity
	 * @brief	queries if the entity contained in inv_loc.m_item can be moved to another location
	 * This is a shorthand for CanRemove + CanAdd query
	 * @return true if can be moved, false otherwise
	 **/
	static proto native bool LocationCanMoveEntity (notnull InventoryLocation src, notnull InventoryLocation dst);
	/**
	 * @fn		LocationCanAddEntity
	 * @brief	query the entity at the specific inventory location
	 * @return entity at the location, null otherwise
	 **/
	static proto native EntityAI LocationGetEntity (notnull InventoryLocation inv_loc);

	static void OnServerInventoryCommandStatic (ParamsReadContext ctx)
	{
		int tmp = -1;
		ctx.Read(tmp);

		int type = -1;
		if (!ctx.Read(type))
			return;

		InventoryLocation src = new InventoryLocation;
		InventoryLocation dst = new InventoryLocation;

		switch (type)
		{
			case InventoryCommandType.SYNC_MOVE:
			{
				src.ReadFromContext(ctx);
				dst.ReadFromContext(ctx);
				syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms ServerInventoryCommand cmd=" + typename.EnumToString(InventoryCommandType, type) + " src=" + src.DumpToString() + " dst=" + dst.DumpToString());

				if (!src.GetItem() || !dst.GetItem())
				{
					Error("[syncinv] ServerInventoryCommand (cmd=SYNC_MOVE) dropped, item not in bubble");
					break; // not in bubble
				}

				LocationSyncMoveEntity(src, dst);
				break;
			}

			case InventoryCommandType.HAND_EVENT:
			{
				HandEventBase e = HandEventBase.CreateHandEventFromContext(ctx);
				syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms ServerInventoryCommand cmd=" + typename.EnumToString(InventoryCommandType, type) + " event=" + e);
				
				if (!e.m_Entity)
				{
					Error("[syncinv] ServerInventoryCommand (cmd=HAND_EVENT) dropped, item not in bubble");
					break; // not in bubble
				}

				if (e.m_Entity.GetInventory().GetCurrentInventoryLocation(src))
				{
					dst = e.GetDst();
					e.m_Player.GetHumanInventory().PostHandEvent(e);
				}
				else
					Error("ServerInventoryCommand HandEvent - no inv loc");
				break;
			}

			case InventoryCommandType.SWAP:
			{
				EntityAI item1, item2;
				ctx.Read(item1);
				ctx.Read(item2);
				
				if (!item1 || !item2)
				{
					Error("[syncinv] ServerInventoryCommand (cmd=SWAP) dropped, item not in bubble");
					break; // not in bubble
				}

				InventoryLocation src1, src2, dst1, dst2;
				if (GameInventory.MakeSrcAndDstForSwap(item1, item2, src1, src2, dst1, dst2))
				{
					syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms ServerInventoryCommand Swap src1=" + src1.DumpToString() + "dst1=" + dst1.DumpToString() +  "src2=" + src2.DumpToString() + "dst2=" + dst2.DumpToString());

					LocationSwap(src1, src2, dst1, dst2);
				}
				else
					Error("ServerInventoryCommand MakeSrcAndDstForSwap - no inv loc");
				break;
			}

			case InventoryCommandType.FORCESWAP:
			{
				Error("[syncinv] I.ForceSwap TODO");
				/*EntityAI item1, item2;
				ctx.Read(item1);
				ctx.Read(item2);
				InventoryLocation user_dst2 = new InventoryLocation;
				user_dst2.ReadFromContext(ctx);
				
				if (!item1 || !item2 || !(user_dst2 && user_dst2.IsValid()))
				{
					Error("[syncinv] ServerInventoryCommand (cmd=FORCESWAP) dropped, item not in bubble");
					break; // not in bubble
				}

				InventoryLocation src1, src2, dst1;
				if (GameInventory.MakeSrcAndDstForForceSwap(item1, item2, src1, src2, dst1, user_dst2))
				{
					syncDebugPrint("[syncinv] t=" + GetGame().GetTime() + "ms ServerInventoryCommand Swap src1=" + src1.DumpToString() + "dst1=" + dst1.DumpToString() +  "src2=" + src2.DumpToString() + "dst2=" + dst2.DumpToString());

					LocationSwap(src1, src2, dst1, user_dst2);
				}
				else
					Error("ServerInventoryCommand MakeSrcAndDstForSwap - no inv loc");
*/
				break;
			}
		}
	}


	/**
	 * @fn		LocationAddEntity
	 * @brief	adds item to inventory location
	 * @return true if success, false otherwise
	 **/
	static proto native bool LocationAddEntity (notnull InventoryLocation inv_loc);
	/**
	 * @fn		LocationRemoveEntity
	 * @brief	removes item from inventory location
	 * @return true if success, false otherwise
	 **/
	static proto native bool LocationRemoveEntity (notnull InventoryLocation inv_loc);

	/**
	 * @fn		LocationMoveEntity
	 * @brief	removes item from current inventory location and adds it to destination
	 * @return true if success, false otherwise
	 **/
	static proto native bool LocationMoveEntity (notnull InventoryLocation src_loc, notnull InventoryLocation dst_loc);

	/**
	 * @fn		LocationSyncMoveEntity
	 * @brief	synchronously removes item from current inventory location and adds it to destination
	 *			no anims involved
	 * @return true if success, false otherwise
	 **/
	static proto native bool LocationSyncMoveEntity (notnull InventoryLocation src_loc, notnull InventoryLocation dst_loc);

	/**
	 * @fn		LocationSwap
	 * @brief	swaps two entities
	 * @return true if success, false otherwise
	 **/
	static proto native bool LocationSwap (notnull InventoryLocation src1, notnull InventoryLocation src2, notnull InventoryLocation dst1, notnull InventoryLocation dst2);

	/**
	 * @fn		ServerLocationMoveEntity
	 * @brief	removes item from current inventory location and adds it to destination + sync via inventory command
	 * @return true if success, false otherwise
	 **/
	static proto native bool ServerLocationMoveEntity (notnull EntityAI item, ParamsWriteContext ctx);

	/**
	 * @fn		ServerLocationSyncMoveEntity
	 * @brief	synchronously removes item from current inventory location and adds it to destination + sync via
	 * inventory command
	 *			no anims involved
	 * @return true if success, false otherwise
	 **/
	static proto native bool ServerLocationSyncMoveEntity (Man player, notnull EntityAI item, ParamsWriteContext ctx);

	/**
	 * @fn		ServerLocationSwap
	 * @brief	swaps two entities
	 * @return true if success, false otherwise
	 **/
	static proto native bool ServerLocationSwap (notnull EntityAI item1, notnull EntityAI item2, ParamsWriteContext ctx);

	/**
	 * @fn		ServerHandEvent
	 * @brief	hand event to clients
	 * @return true if success, false otherwise
	 **/
	static proto native bool ServerHandEvent (notnull Man player, notnull EntityAI item, ParamsWriteContext ctx);

	static proto native void PrepareDropEntityPos (EntityAI owner, notnull EntityAI item, out vector mat[4]);

	/**@fn      CanSwapEntities
	 * @brief   test if ordinary swap can be performed.
	 *
	 * Ordinary in the sense that the two items has equal sizes and can be swapped without
	 * additional space.
	 *
	 * @param [in]  item1     first item
	 * @param [in]  item2     second item
	 * @return    true if can be swapped
	 */
	static proto native bool CanSwapEntities (notnull EntityAI item1, notnull EntityAI item2);

	/**@fn      CanForceSwapEntities
	 * @brief   test if forced swap can be performed.
	 *
	 * @param [in]  item1     is the forced item (primary)
	 * @param [in]  item2     second item that will be replaced by the item1. (secondary)
	 * @param [out]  item2_dst     second item's potential destination
	 * @return    true if can be force swapped
	 */
	static proto native bool CanForceSwapEntities (notnull EntityAI item1, notnull EntityAI item2, out notnull InventoryLocation item2_dst);

	///@{ reservations
	const int c_InventoryReservationTimeoutMS = 15000;
	static proto native bool AddInventoryReservation (EntityAI item, InventoryLocation dst, int timeout_ms);
	static proto native bool ExtendInventoryReservation (EntityAI item, InventoryLocation dst, int timeout_ms);
	static proto native bool ClearInventoryReservation (EntityAI item, InventoryLocation dst);
	static proto native bool HasInventoryReservation (EntityAI item, InventoryLocation dst);
	static proto native bool GetInventoryReservationCount (EntityAI item, InventoryLocation dst);
	///@} reservations

	///@{ locks
	proto native bool CanLockInventoryWithKey (notnull EntityAI key);
	proto native bool CanUnlockInventoryWithKey (notnull EntityAI key);
	proto native void LockInventoryWithKey (notnull EntityAI key);
	proto native void UnlockInventoryWithKey (notnull EntityAI key);
	proto native bool HasKeys ();

	proto native void LockInventory (int lockType);
	proto native void UnlockInventory (int lockType);
	proto native int GetScriptLockCount ();
	proto native bool IsInventoryUnlocked ();
	proto native bool IsInventoryLocked ();
	proto native bool IsInventoryLockedForLockType (int lockType);
	proto native bool SetSlotLock (int slot, bool locked);
	proto native bool GetSlotLock (int slot);
	///@} locks

	///@{ anti-cheats
	static proto native bool CheckDropRequest (notnull Man requestingPlayer, EntityAI item);
	static proto native bool CheckTakeItemRequest (notnull Man requestingPlayer, EntityAI item, EntityAI target);
	static proto native bool CheckMoveToDstRequest (notnull Man requestingPlayer, EntityAI item, notnull InventoryLocation dst);
	static proto native bool CheckSwapItemsRequest (notnull Man requestingPlayer, EntityAI item1, EntityAI item2);
	///@} anti-cheats

///@} Engine native functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///@{ script functions

	/**@fn		Init
	 * @brief	called by engine right after creation of entity
	 **/
	void Init () { }

	/// db load hooks
	void OnStoreLoad (ParamsReadContext ctx, int version) { }
	void OnAfterStoreLoad () { }
	/// db store hook
	void OnStoreSave (ParamsWriteContext ctx) { }

	void EEInit ()
	{
		InventoryLocation loc = new InventoryLocation;
		if (GetCurrentInventoryLocation(loc))
		{
			if (loc.GetType() == InventoryLocationType.HANDS)
			{
				Man man = Man.Cast(loc.GetParent());
				if (man)
				{
					Print("Inventory::EEInit - Man=" + man + " item=" + this);
					man.GetHumanInventory().OnEntityInHandsCreated(GetInventoryOwner());
				}
			}
		}
	}
	
	void EEDelete (EntityAI parent)
	{
		EntityAI item = GetInventoryOwner();
		if (parent)
			parent.GetInventory().ClearInventoryReservation(item, null);
	}

	/**@fn		CreateInInventory
	 * @brief	creates entity somewhere in inventory
	 *
	 * @param[in]	type	\p		item type to be placed in inventory
	 * @return	created entity or null
	 **/
	EntityAI CreateInInventory (string type)
	{
		InventoryLocation loc = new InventoryLocation;
		if (FindFirstFreeLocationForNewEntity(type, FindInventoryLocationType.CARGO | FindInventoryLocationType.ATTACHMENT, loc))
		{
			switch (loc.GetType())
			{
				case InventoryLocationType.ATTACHMENT: return loc.GetParent().GetInventory().CreateAttachmentEx(type, loc.GetSlot()); break;
				case InventoryLocationType.CARGO: return loc.GetParent().GetInventory().CreateEntityInCargoEx(type, loc.GetIdx(), loc.GetRow(), loc.GetCol()); break;
				default: Error("CreateInInventory: unknown location for item"); break;
			}
		}
		return null;
	}

	/**
	 * @fn		CanAddEntityToInventory
	 * @brief	ask inventory if item could be placed somewhere in inventory
	 *
	 * All available inventory locations (hands, cargo, attachments, proxycargo, ...) are
	 * traversed recursively from root to leafs.
	 *
	 * @param[in]	item	\p		item to be placed in inventory
	 * @return	true if item can be added, false otherwise
	 **/
	bool CanAddEntityToInventory (notnull EntityAI item)
	{
		InventoryLocation il = new InventoryLocation;
		bool result = FindFreeLocationFor(item, FindInventoryLocationType.ANY, il);
		return result;
	}
	/**
	 * @fn		AddEntityToInventory
	 * @brief	add entity somewhere in inventory
	 *
	 * All available inventory locations (hands, cargo, attachments, proxycargo, ...) are
	 * traversed recursively from root to leafs.
	 *
	 * @NOTE: AddEntityToInventory does not perform the checks the CanAddEntityToInventory does
	 * (script conditions for example).
	 * if not sure, always call CanAddEntityToInventory before AddEntityToInventory.
	 *
	 * @param[in]	item	\p		item to be placed in inventory
	 * @return	true if item can be added, false otherwise
	 **/
	bool AddEntityToInventory (notnull EntityAI item)
	{
		InventoryLocation il = new InventoryLocation;
		bool result = FindFreeLocationFor(item, FindInventoryLocationType.ANY, il);
		if (result)
			return LocationAddEntity(il);
		return result;
	}

	/**
	 * @brief Returns if entity can be removed from its current location
	 **/
	bool CanRemoveEntity ()
	{
		InventoryLocation il = new InventoryLocation;
		if (GetCurrentInventoryLocation(il))
			return LocationCanRemoveEntity(il);
		return false;
	}

	// Script version of CanAddEntity* methods based on InventoryLocation
	/**
	 * @fn		CanAddEntityInto
	 * @brief	Asks inventory if item could be placed in inventory, hands, etc.
	 *
	 * Location is based on the flags parameter. This method is used in methods that are 
	 * asking for if item can be placed in specific location, like CanAddEntityToInventory,
	 * CanAddEntityInHands, etc.
	 *
	 * @param[in]	item	\p		item to be placed in inventory
	 * @param[in]	flags	\p		FindInventoryLocationType
	 * @return	true if item can be added, false otherwise
	*/	
	bool CanAddEntityInto (notnull EntityAI item, FindInventoryLocationType flags = FindInventoryLocationType.ANY)
	{
		InventoryLocation loc = new InventoryLocation;
		return FindFreeLocationFor(item, flags, loc);
	}
	
	/**
	\brief Test if entity can be put to the inventory (Cargo, ProxyCargo, Attachment)
	*/
	bool CanAddEntityIntoInventory (notnull EntityAI item)
	{
		return CanAddEntityInto(item, FindInventoryLocationType.ANY_CARGO | FindInventoryLocationType.ATTACHMENT);
	}

	/**
	\brief Test if entity can be put into hands
	*/
	bool CanAddEntityIntoHands (notnull EntityAI item)
	{
		return CanAddEntityInto(item, FindInventoryLocationType.HANDS);
	}

	///@{ synchronization
	bool OnInputUserDataProcess (ParamsReadContext ctx) { }
	bool OnInventoryJunctureFromServer (ParamsReadContext ctx) { }
	void OnServerInventoryCommand (ParamsReadContext ctx) { }
	///@} synchronization

	/**
	 * @fn			TakeEntityToInventory
	 * @brief		Put item anywhere into this entity (as attachment or into cargo, recursively)
	 * @param[in]	mode				inventory mode
	 * @param[in]	flags				preferred location, @see FindInventoryLocationType
	 * @param[in]	item				item to be taken in inventory
	 * @return		true on success
	 *
	 * @NOTE: Predictive.* / Local.* functions:
	 * a) 'Predictive' means that the operation uses Client-Side Prediction, i.e. the
	 * action runs the same code on both, client AND server.
	 *
	 * b) 'Local' operation executes from where it is run, i.e. the operation from client
	 *		is NOT sent to server, and only client performs the inventory operation (TakeEntityTo.*)
	 *		this behaviour is necessary when inventory operations are ALREADY synchronized by another
	 *		means, like Actions.
	 *		For example @see ActionTakeItemToHands that performs OnCompleteServer
	 *		and OnCompleteClient with synchronization set to false in order to avoid duplicate
	 *		synchronization.
	**/
	bool TakeEntityToInventory (InventoryMode mode, FindInventoryLocationType flags, notnull EntityAI item)
	{
		Print("[inv] I::Take2Inv(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);

		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			InventoryLocation dst = new InventoryLocation;
			if (FindFreeLocationFor(item, flags, dst))
				return TakeToDst(mode, src, dst);

			Print("[inv] I::Take2Inv(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " Warning - no room for item");
			return false;
		}
		Error("[inv] I::Take2Inv(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " Error - src has no inventory location");
		return false;
	}

	/// helper that finds location first, then moves the entity into it
	bool TakeEntityToTargetInventory (InventoryMode mode, notnull EntityAI target, FindInventoryLocationType flags, notnull EntityAI item)
	{
		Print("[inv] I::Take2Target(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);

		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			InventoryLocation dst = new InventoryLocation;
				
			if (target.GetInventory().FindFreeLocationFor(item, flags, dst))
				return TakeToDst(mode, src, dst);

			Print("[inv] I::Take2Target(" + typename.EnumToString(InventoryMode, mode) + ") target=" + target + " item=" + item + " Warning - no room for item in target");
			return false;
		}
		Error("[inv] I::Take2Target(" + typename.EnumToString(InventoryMode, mode) + ") target=" + target + " item=" + item + " Error - src has no inventory location");
		return false;
	}

	/**@fn			TakeToDst
	 * @brief		move src to dst
	 * @param[in]	mode				inventory mode
	 * @param[in]	src					source location of the item
	 * @param[in]	dst					destination location of the item
	 **/
	bool TakeToDst (InventoryMode mode, notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		Print("[inv] I::Take2Dst(" + typename.EnumToString(InventoryMode, mode) + ") src=" + src.DumpToString() + " dst=" + dst.DumpToString());

		switch (mode)
		{
			case InventoryMode.PREDICTIVE:
				InventoryInputUserData.SendInputUserDataMove(InventoryCommandType.SYNC_MOVE, src, dst);
				return LocationSyncMoveEntity(src, dst);

			case InventoryMode.JUNCTURE:
				InventoryInputUserData.SendInputUserDataMove(InventoryCommandType.SYNC_MOVE, src, dst);
				return true;

			case InventoryMode.LOCAL:
				return LocationSyncMoveEntity(src, dst);

			case InventoryMode.SERVER:
				Error("Server side inventory command without player is not implemented.. none shall pass! (src=" + src.DumpToString() + " dst=" + dst.DumpToString() + ")");
				return false;
				//bool ret = LocationSyncMoveEntity(src, dst);
				//InventoryInputUserData.SendServerMove(nullptr, InventoryCommandType.SYNC_MOVE, src, dst);
				//return ret;
			default:
				Error("HandEvent - Invalid mode");
		}
		return false;
	}

	/**@fn			TakeEntityToCargo
	 * @brief		moves item to cargo of this intentory
	 * @param[in]	mode				inventory mode
	 * @param[in]	item				item to be put in this inventory as cargo
	 **/
	bool TakeEntityToCargo (InventoryMode mode, notnull EntityAI item)
	{
		Print("[inv] I::Take2Cgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		return TakeEntityToInventory(mode, FindInventoryLocationType.CARGO, item);
	}

	/// Put item into into cargo of another entity
	bool TakeEntityToTargetCargo (InventoryMode mode, notnull EntityAI target, notnull EntityAI item)
	{
		Print("[inv] I::Take2TargetCgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + "to cargo of target=" + target);
		return TakeEntityToTargetInventory(mode, target, FindInventoryLocationType.CARGO, item);
	}

	/**@fn			TakeEntityToCargoEx
	 * @brief		moves item on specific cargo location
	 * @param[in]	mode				inventory mode
	 * @param[in]	item				item to be put in this inventory as cargo
	 **/
	bool TakeEntityToCargoEx (InventoryMode mode, notnull EntityAI item, int idx, int row, int col)
	{
		Print("[inv] I::Take2Cgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " row=" + row + " col=" + col);
		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			InventoryLocation dst = new InventoryLocation;
			dst.SetCargo(GetInventoryOwner(), item, idx, row, col);

			return TakeToDst(mode, src, dst);
		}
		Error("[inv] I::Take2Cgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " row=" + row + " col=" + col + " Error - src has no inventory location");
		return false;
	}

	/// Put item into into cargo on specific cargo location of another entity
	bool TakeEntityToTargetCargoEx (InventoryMode mode, notnull EntityAI target, notnull EntityAI item, int idx, int row, int col)
	{
		Print("[inv] I::Take2TargetCgoEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + "to cargo of target=" + target + " row=" + row + " col=" + col);
		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			InventoryLocation dst = new InventoryLocation;
			dst.SetCargo(target, item, idx, row, col);

			return TakeToDst(mode, src, dst);
		}
		Error("[inv] I::Take2TargetCgoEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + "to cargo of target=" + target + " row=" + row + " col=" + col);
		return false;
	}

	bool TakeEntityAsAttachmentEx (InventoryMode mode, notnull EntityAI item, int slot)
	{
		Print("[inv] I::Take2AttEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " slot=" + slot);
		return TakeEntityAsTargetAttachmentEx(mode, GetInventoryOwner(), item, slot);
	}

	/// put item as attachment of target
	bool TakeEntityAsTargetAttachmentEx (InventoryMode mode, notnull EntityAI target, notnull EntityAI item, int slot)
	{
		Print("[inv] I::Take2TargetAttEx(" + typename.EnumToString(InventoryMode, mode) + ") as ATT of target=" + target + " item=" + item + " slot=" + slot);
		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			InventoryLocation dst = new InventoryLocation;
			dst.SetAttachment(target, item, slot);

			return TakeToDst(mode, src, dst);
		}
		Error("[inv] I::Take2AttEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " Error - src has no inventory location");
		return false;
	}

	bool TakeEntityAsAttachment (InventoryMode mode, notnull EntityAI item)
	{
		Print("[inv] I::Take2Att(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		return TakeEntityToInventory(mode, FindInventoryLocationType.ATTACHMENT, item);
	}

	bool TakeEntityAsTargetAttachment (InventoryMode mode, notnull EntityAI target, notnull EntityAI item)
	{
		Print("[inv] I::Take2AttEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		return TakeEntityToTargetInventory(mode, target, FindInventoryLocationType.ATTACHMENT, item);
	}

	/// helper function for swap
	static bool MakeDstForSwap (notnull ref InventoryLocation src1, notnull ref InventoryLocation src2, out ref InventoryLocation dst1, out ref InventoryLocation dst2)
	{
		if (dst1 == null)
			dst1 = new InventoryLocation;
		dst1.Copy(src1);
		dst1.CopyLocationFrom(src2);

		if (dst2 == null)
			dst2 = new InventoryLocation;
		dst2.Copy(src2);
		dst2.CopyLocationFrom(src1);
		return true;
	}

	/// helper function for swap
	static bool MakeSrcAndDstForSwap (notnull EntityAI item1, notnull EntityAI item2, out ref InventoryLocation src1, out ref InventoryLocation src2, out ref InventoryLocation dst1, out ref InventoryLocation dst2)
	{
		if (src1 == null)
			src1 = new InventoryLocation;
		if (src2 == null)
			src2 = new InventoryLocation;
		if (item1.GetInventory().GetCurrentInventoryLocation(src1) && item2.GetInventory().GetCurrentInventoryLocation(src2))
			return MakeDstForSwap(src1, src2, dst1, dst2);
		return false;
	}

	/// helper function for ForceSwap
	static bool MakeSrcAndDstForForceSwap (notnull EntityAI item1, notnull EntityAI item2, out ref InventoryLocation src1, out ref InventoryLocation src2, out ref InventoryLocation dst1, notnull InventoryLocation dst2)
	{
		if (src1 == null)
			src1 = new InventoryLocation;
		if (src2 == null)
			src2 = new InventoryLocation;
		if (item1.GetInventory().GetCurrentInventoryLocation(src1) && item2.GetInventory().GetCurrentInventoryLocation(src2))
		{
			// src1 -> dst_of(src2)
			if (dst1 == null)
				dst1 = new InventoryLocation;
			dst1.Copy(src1);
			dst1.CopyLocationFrom(src2);

			// src2 -> dst2 from user
			return true;
		}

		return false;
	}

	bool SwapEntities (InventoryMode mode, notnull EntityAI item1, notnull EntityAI item2)
	{
		InventoryLocation src1, src2, dst1, dst2;
		if (GameInventory.MakeSrcAndDstForSwap(item1, item2, src1, src2, dst1, dst2))
		{
			Print("[inv] I::Swap(" + typename.EnumToString(InventoryMode, mode) + ") src1=" + src1.DumpToString() + " --> dst1=" + dst1.DumpToString() +  " src2=" + src2.DumpToString() + "--> dst2=" + dst2.DumpToString());

			switch (mode)
			{
				case InventoryMode.PREDICTIVE:
					InventoryInputUserData.SendInputUserDataSwap(item1, item2);
					return LocationSwap(src1, src2, dst1, dst2);

				case InventoryMode.JUNCTURE:
					InventoryInputUserData.SendInputUserDataSwap(item1, item2);
					return true;

				case InventoryMode.LOCAL:
					return LocationSwap(src1, src2, dst1, dst2);

				default:
					Error("SwapEntities - HandEvent - Invalid mode");
			}
		}
		else
			Error("SwapEntities - MakeSrcAndDstForSwap - no inv loc");
		return false;
	}

	bool ForceSwapEntities (InventoryMode mode, notnull EntityAI item1, notnull EntityAI item2, notnull InventoryLocation item2_dst)
	{
		Error("I::ForceSwap TODO");
		return false;
		/*InventoryLocation src1, src2, dst1;
		if (GameInventory.MakeSrcAndDstForForceSwap(item1, item2, src1, src2, dst1, item2_dst))
		{
			Print("[inv] I::ForceSwap(" + typename.EnumToString(InventoryMode, mode) + ") src1=" + src1.DumpToString() + " --> dst1=" + dst1.DumpToString() +  " src2=" + src2.DumpToString() + " --> item2_dst=" + item2_dst.DumpToString());

			switch (mode)
			{
				case InventoryMode.PREDICTIVE:
					InventoryInputUserData.SendInputUserDataForceSwap(item1, item2, item2_dst);
					return LocationForceSwap(src1, src2, dst1, item2_dst);

				case InventoryMode.JUNCTURE:
					InventoryInputUserData.SendInputUserDataForceSwap(item1, item2, item2_dst);
					return true;

				case InventoryMode.LOCAL:
					return LocationSwap(src1, src2, dst1, item2_dst2);

				default:
					Error("ForceSwapEntities - HandEvent - Invalid mode");
			}
		}
		else
			Error("ForceSwapEntities - MakeSrcAndDstForForceSwap - no inv loc");*/
	}

	static void SetGroundPosByOwner (EntityAI owner, notnull EntityAI item, out InventoryLocation ground)
	{
		vector m4[4];
		Math3D.MatrixIdentity4(m4);
		GameInventory.PrepareDropEntityPos(owner, item, m4);
		ground.SetGround(item, m4);
	}

	bool DropEntity (InventoryMode mode, EntityAI owner, notnull EntityAI item)
	{
		Print("[inv] I::Drop(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			InventoryLocation dst = new InventoryLocation;
			SetGroundPosByOwner(owner, item, dst);

			return TakeToDst(mode, src, dst);
		}

		Error("DropEntity - No inventory location");
		return false;
	}

	bool LocalDestroyEntity (notnull EntityAI item)
	{
		Print("[inv] I::LocalDestroyEntity inv item=" + item);
		InventoryLocation src = new InventoryLocation;
		if (item.GetInventory().GetCurrentInventoryLocation(src))
		{
			if (src.GetType() == InventoryLocationType.HANDS)
				Error("[inv] Source location == HANDS, player has to handle this");

			GetGame().ObjectDelete(src.GetItem());
			return true;
		}

		Error("LocalDestroyEntity: No inventory location");
		return false;
	}

	bool ReplaceItemWithNew (InventoryMode mode, ReplaceItemWithNewLambdaBase lambda)
	{
		InventoryLocation src = new InventoryLocation;
		if (lambda.m_OldItem.GetInventory().GetCurrentInventoryLocation(src))
		{
			Print("[inv] I::ReplaceItemWithNew executing lambda=" + lambda + "on old_item=" + lambda.m_OldItem);
			if (src.GetType() == InventoryLocationType.HANDS && src.GetParent().IsAlive())
				Error("[inv] I::ReplaceItemWithNew Source location == HANDS, alive player has to handle this");

			lambda.Execute();
			return true;
		}

		Error("[inv] I::ReplaceItemWithNew - no inventory location");
		return false;
	}

///@} script functions
};

