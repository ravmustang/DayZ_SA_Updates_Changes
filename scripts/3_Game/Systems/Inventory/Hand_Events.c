///@{ events

/**@enum	HandEventID
 * @brief	identifier for events mainly for rpc purposes
 **/
enum HandEventID
{
	UNKNOWN,
	TAKE,
	MOVETO,
	DROP,
	SWAP,
	FORCESWAP,
	DESTROY,
	CREATED,
	DESTROYED,
	REPLACE,
	REPLACE2,
	REPLACED,
	HUMANCOMMAND_ACTION_FINISHED,
	HUMANCOMMAND_ACTION_ABORTED,
	ANIMEVENT_CHANGE_HIDE
};

/**@class	HandEventBase
 * @brief	represents event that triggers transition from state to state
 **/
class HandEventBase
{
	int m_EventID;
	Man m_Player;
	EntityAI m_Entity;

	void HandEventBase (Man p = null, EntityAI e = NULL) { m_EventID = 0; m_Player = p; m_Entity = e; }
	HandEventID GetEventID () { return m_EventID; }

	void ReadFromContext (ParamsReadContext ctx) { }
	void WriteToContext (ParamsWriteContext ctx)
	{
		ctx.Write(m_EventID);
		ctx.Write(m_Player);
		ctx.Write(m_Entity);
	}

	InventoryLocation GetDst () { return null; }
	bool IsSwapEvent () { return false; }
	bool CheckRequest () { return true; }
	bool IsServerSideOnly () { return false; }

	static HandEventBase HandEventFactory (HandEventID id, Man p = null, EntityAI e = NULL)
	{
		switch (id)
		{
			case HandEventID.UNKNOWN: return null;
			case HandEventID.TAKE: return new HandEventTake(p, e);
			case HandEventID.MOVETO: return new HandEventMoveTo(p, e);
			case HandEventID.DROP: return new HandEventDrop(p, e);
			case HandEventID.SWAP: return new HandEventSwap(p, e);
			case HandEventID.FORCESWAP: return new HandEventForceSwap(p, e);
			case HandEventID.DESTROY: return new HandEventDestroy(p, e);
			case HandEventID.CREATED: return new HandEventCreated(p, e);
			case HandEventID.DESTROYED: return new HandEventDestroyed(p, e);
			case HandEventID.REPLACE: return new HandEventDestroyAndReplaceWithNew(p, e);
			case HandEventID.REPLACE2: return new HandEventDestroyAndReplaceWithNewElsewhere(p, e);
			case HandEventID.REPLACED: return new HandEventReplaced(p, e);
			case HandEventID.ANIMEVENT_CHANGE_HIDE: return HandAnimEventChanged(p, e);
			case HandEventID.HUMANCOMMAND_ACTION_FINISHED : return HandEventHumanCommandActionFinished(p, e);
		}
		Error("[hndfsm] HandEventFactory - unregistered hand event with id=" + id);
		return null;
	}
	
	static HandEventBase CreateHandEventFromContext (ParamsReadContext ctx)
	{
		int eventID = -1;
		ctx.Read(eventID);
		Man player;
		ctx.Read(player);
		EntityAI entity;
		ctx.Read(entity);

		HandEventBase b = HandEventFactory(eventID, player, entity);
		b.ReadFromContext(ctx);
		return b;
	}
};

class HandEventTake extends HandEventBase
{
	void HandEventTake (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.TAKE; }

	override InventoryLocation GetDst ()
	{
		InventoryLocation dst = new InventoryLocation;
		dst.SetHands(m_Player, m_Entity);
		return dst;
	}
	
	override bool CheckRequest ()
	{
		return GameInventory.CheckMoveToDstRequest(m_Player, m_Entity, GetDst()));
	}
};

class HandEventMoveTo extends HandEventBase
{
	ref InventoryLocation m_Dst; /// destination for item in hands

	void HandEventMoveTo (Man p = null, EntityAI e = NULL, InventoryLocation dst = NULL) { m_EventID = HandEventID.MOVETO; m_Dst = dst; }

	override void ReadFromContext (ParamsReadContext ctx)
	{
		m_Dst = new InventoryLocation;
		super.ReadFromContext(ctx);
		m_Dst.ReadFromContext(ctx);
	}
	override void WriteToContext (ParamsWriteContext ctx)
	{
		super.WriteToContext(ctx);
		m_Dst.WriteToContext(ctx);
	}

	override InventoryLocation GetDst () { return m_Dst; }
	
	override bool CheckRequest ()
	{
		return GameInventory.CheckMoveToDstRequest(m_Player, m_Entity, GetDst()));
	}
};

class HandEventDrop extends HandEventBase
{
	void HandEventDrop (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.DROP; }

	override InventoryLocation GetDst ()
	{
		InventoryLocation dst = new InventoryLocation;
		vector mat[4];
		Math3D.MatrixIdentity4(mat);
		mat[3] = m_Player.GetPosition();

		dst.SetGround(m_Entity, mat);
		return dst;
	}
	
	override bool CheckRequest ()
	{
		return GameInventory.CheckMoveToDstRequest(m_Player, m_Entity, GetDst()));
	}
};
class HandEventSwap extends HandEventBase
{
	void HandEventSwap (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.SWAP; }

	override InventoryLocation GetDst ()
	{
		InventoryLocation dst = new InventoryLocation;
		dst.SetHands(m_Player, m_Entity);
		return dst;
	}
	
	override bool IsSwapEvent () { return true; }
	
	override bool CheckRequest ()
	{
		EntityAI inHands = m_Player.GetHumanInventory().GetEntityInHands();
		if (m_Entity && inHands)
		{
			bool test1 = GameInventory.CheckSwapItemsRequest(m_Player, m_Entity, inHands);
			bool test2 = GameInventory.CanSwapEntities(m_Entity, inHands);
			return test1 && test2;
		}
		return false;
	}
};
class HandEventForceSwap extends HandEventBase
{
	ref InventoryLocation m_Dst; /// destination of item in hands

	void HandEventForceSwap (Man p = null, EntityAI e = NULL, InventoryLocation dst = NULL) { m_EventID = HandEventID.FORCESWAP; m_Dst = dst; }

	override void ReadFromContext (ParamsReadContext ctx)
	{
		m_Dst = new InventoryLocation;
		super.ReadFromContext(ctx);
		m_Dst.ReadFromContext(ctx);
	}
	override void WriteToContext (ParamsWriteContext ctx)
	{
		super.WriteToContext(ctx);
		m_Dst.WriteToContext(ctx);
	}

	override InventoryLocation GetDst () { return m_Dst; }
	override bool IsSwapEvent () { return true; }
	
	override bool CheckRequest ()
	{
		EntityAI inHands = m_Player.GetHumanInventory().GetEntityInHands();
		if (m_Entity && inHands && m_Dst && m_Dst.IsValid())
		{
			bool test1 = GameInventory.CheckSwapItemsRequest(m_Player, m_Entity, inHands);
			bool test2 = GameInventory.CanForceSwapEntities(m_Entity, inHands, m_Dst);
			return test1 && test2;
		}
		return false;
	}
};

class HandEventDestroy extends HandEventBase
{
	void HandEventDestroy (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.DESTROY; }
	
	override bool IsServerSideOnly () { return true; }
};

class HandEventCreated extends HandEventBase
{
	void HandEventCreated (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.CREATED; }
};

class HandEventDestroyed extends HandEventBase
{
	void HandEventDestroyed (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.DESTROYED; }
};

class HandEventDestroyAndReplaceWithNew extends HandEventBase
{
	string m_Type;
	ref ReplaceItemWithNewLambdaBase m_Lambda;

	void HandEventDestroyAndReplaceWithNew (Man p = null, EntityAI e = NULL, ReplaceItemWithNewLambdaBase lambda = NULL) { m_EventID = HandEventID.REPLACE; m_Lambda = lambda; }

	override void ReadFromContext (ParamsReadContext ctx)
	{
		super.ReadFromContext(ctx);
		ctx.Read(m_Type);
		// @TODO: lambda?
	}
	override void WriteToContext (ParamsWriteContext ctx)
	{
		super.WriteToContext(ctx);
		ctx.Write(m_Type);
		// @TODO: lambda?
	}
	override bool IsSwapEvent () { return true; }
	override bool IsServerSideOnly () { return true; }
};

class HandEventDestroyAndReplaceWithNewElsewhere extends HandEventDestroyAndReplaceWithNew
{
	void HandEventDestroyAndReplaceWithNewElsewhere (Man p = null, EntityAI e = NULL, ReplaceItemWithNewLambdaBase lambda = NULL) { m_EventID = HandEventID.REPLACE2; m_Lambda = lambda; }
	override bool IsServerSideOnly () { return true; }
};

class HandEventReplaced extends HandEventBase
{
	void HandEventReplaced (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.REPLACED; }
};

// anim events

class HandAnimEventChanged extends HandEventBase
{
	void HandAnimEventChanged (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.ANIMEVENT_CHANGE_HIDE; }
};

HandEventBase HandAnimEventFactory (WeaponEvents type, Man p = null, EntityAI e = NULL)
{
	switch (type)
	{
		case WeaponEvents.CHANGE_HIDE: return new HandAnimEventChanged(p, e);
	}
	return NULL;
}

/**@brief		triggered when animation action finishes
 **/
class HandEventHumanCommandActionFinished extends HandEventBase
{
	void HandEventHumanCommandActionFinished (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.HUMANCOMMAND_ACTION_FINISHED; }
};
/**@brief		triggered when animation action aborts
 **/
class HandEventHumanCommandActionAborted extends HandEventBase
{
	void HandEventHumanCommandActionAborted (Man p = null, EntityAI e = NULL) { m_EventID = HandEventID.HUMANCOMMAND_ACTION_ABORTED; }
};

///@} events

