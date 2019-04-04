
//-----------------------------------------------------------------------------
class ItemSuppressor extends InventoryItemSuper
{
};

typedef ItemSuppressor SuppressorBase;

//-----------------------------------------------------------------------------
class ItemWatch extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class ItemTransmitter extends InventoryItemSuper
{
	proto native void SetNextChannel();
	proto native void SetPrevChannel();
	proto native float GetTunedFrequency();
	proto native void EnableBroadcast(bool state);
	proto native void EnableReceive(bool state);
	proto native bool IsBroadcasting();
	proto native bool IsReceiving();
	proto native int GetTunedFrequencyIndex();
	proto native void SetFrequencyByIndex(int index);
};

//-----------------------------------------------------------------------------
class ItemMegaphone extends InventoryItemSuper
{
	proto native bool CanSpeak();
	proto native void SetCanSpeak(bool onOff); 
}

//-----------------------------------------------------------------------------
class ItemRadio extends InventoryItemSuper
{
	proto native void TuneNext();
	proto native void TunePrev();
};

//-----------------------------------------------------------------------------
class ItemBarrel extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class UnderSlugLauncher extends Weapon
{
};

//-----------------------------------------------------------------------------
class ItemGPS extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class ItemCompass extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class CarWheel extends InventoryItemSuper
{
	//! Returns current wheel radius.
	proto native float GetRadius();

	//! Returns wheel width.
	proto native float GetWidth();

/*	Invoke car phys, when wheel was attached
	not working properly, postponed for now
	override void OnWasAttached( EntityAI parent, int slot_name )
	{
		IEntity iePrnt = IEntity.Cast( parent );
		dBodyActive( iePrnt, ActiveState.ACTIVE);
		dBodyApplyImpulse( iePrnt, "0 1 0");
	}
*/

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos)
	{
		Print("CarWheel>>> EEHitBy");
		Print( dmgZone );
		Print( damageResult );
		Print( source );
		Print( component );
		Print( damageResult.GetDamage(dmgZone, "Health") );
	}
	
	override void EEKilled(Object killer)
	{
		string newWheel = "";
		switch( GetType() )
		{
			case "HatchbackWheel":
				newWheel = "HatchbackWheel_Ruined";
			break;
			
			case "V3SWheel":
				newWheel = "V3SWheel_Ruined";
			break;
			
			case "V3SWheelDouble":
				newWheel = "V3SWheelDouble_Ruined";
			break;

			case "CivSedanWheel":
				newWheel = "CivSedanWheel_Ruined";
			break;	
		}

		if ( newWheel != "" )
		{
			ReplaceWheelLambda lambda = new ReplaceWheelLambda ( this, newWheel, NULL);
			lambda.SetTransferParams(true, true, true);
			GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda);
		}
	}

};

class ReplaceWheelLambda : TurnItemIntoItemLambda
{
	vector m_oldOri;
	void ReplaceWheelLambda (EntityAI old_item, string new_item_type, PlayerBase player) {}

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);
		m_oldOri = old_item.GetOrientation();
	}
	
	override protected void OnSuccess (EntityAI new_item)
	{
		super.OnSuccess( new_item );
		if (new_item)
			new_item.SetOrientation( m_oldOri );
	}
}

class HatchbackWheel extends CarWheel {};

class CarDoor extends InventoryItemSuper
{

	override bool CanDetachAttachment(EntityAI parent)
	{
		//parent.FindAtt
		InventoryLocation loc = new InventoryLocation();
		
		bool isPresent = GetInventory().GetCurrentInventoryLocation( loc );
		
		if ( !isPresent )
		 return false;
		
		string slotName = InventorySlots.GetSlotName( loc.GetSlot() );

		if ( slotName && CarScript.Cast( parent ).GetCarDoorsState( slotName ) == CarDoorState.DOORS_OPEN )
			return true;
		
		return false;
	}


/*
	override void OnWasAttached( EntityAI parent, int slot_name )
	{
		
		SoundParams soundParams = new SoundParams("offroad_door_close_SoundSet");
		SoundObjectBuilder soundBuilder = new SoundObjectBuilder(soundParams);
		SoundObject soundObject = soundBuilder.BuildSoundObject();
		soundObject.SetPosition(GetPosition());
		GetGame().GetSoundScene().Play3D(soundObject, soundBuilder);
	}
*/
/*
	void CloseDoors()
	{
		SetAnimationPhase();
		
	}
*/
};

class CarRadiator extends InventoryItemSuper
{

/* MOVED TO CarScript.C
	override void OnWasAttached ( EntityAI parent, int slot_id )
	{	
		if ( GetGame().IsServer() && parent )
		{
			Car car;
		 	Class.CastTo( car, parent );

			if ( car )
				car.SetHealth( "Radiator", "Health", GetHealth() );
		}
	}

	override void OnWasDetached ( EntityAI parent, int slot_id )
	{
		if ( GetGame().IsServer() && parent )
		{
			Car car;
		 	Class.CastTo( car, parent );

			if ( car )
			{
				car.Leak( CarFluid.COOLANT, car.GetFluidFraction(CarFluid.COOLANT)*car.GetFluidCapacity(CarFluid.COOLANT) );
				car.SetHealth( "Radiator", "Health", 0);
			}
		}
	}
*/
	override void EEKilled(Object killer)
	{
		if ( GetGame().IsServer() )
		{
			Car car;
			EntityAI parent = GetHierarchyParent();
			
			Class.CastTo( car, parent );

			if ( car )
				car.LeakAll( CarFluid.COOLANT );
		}
	}

};


//-----------------------------------------------------------------------------
class Clothing extends ItemBase
{
	override bool IsClothing()
	{
		return true;
	}

	// Conditions	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo( parent ) )
		{
			return false;
		}
		
		bool is_hidden_stash_exception = false;
		
		if ( parent.IsInherited( UndergroundStash ) )
			is_hidden_stash_exception = true;
		
		if ( ( GetNumberOfItems() == 0 || !parent || parent.IsMan() || is_hidden_stash_exception ) && super.CanPutInCargo( parent ) )
		{
			return true;
		}
		
		return false;
	}

	float GetItemVisibility()
	{
		return ConfigGetFloat("visibilityModifier");
	}
};

typedef Clothing ClothingBase;

//-----------------------------------------------------------------------------
class ItemBook extends InventoryItemSuper
{
	override event bool OnUseFromInventory(Man owner)
	{
		return false;
	}
};

//-----------------------------------------------------------------------------
class ItemGrenade extends InventoryItemSuper
{
	/**@fn		SetPinned
	 * @brief	sets grenade to state GR_PINNED (if the state transition is possible)
	 * @return	true if state already set or if state transition performed, false otherwise
	 **/
	proto native bool SetPinned ();
	/**@fn		SetUnpinned
	 * @brief	sets grenade to state GR_UNPINNED (if the state transition is possible)
	 * @return	true if state already set or if state transition performed, false otherwise
	 **/
	proto native bool SetUnpinned ();
	/**@fn		SetIgnited
	 * @brief	sets grenade to state GR_TRIGGERED (if the state transition is possible)
	 * @return	true if state already set or if state transition performed, false otherwise
	 **/
	proto native bool SetIgnited ();
	/**@fn		IsPinned
	 * @return	true if grenade is pinned
	 **/
	proto native bool IsPinned ();
	/**@fn		IsActivated
	 * @return	true if grenade is GR_TRIGGERED or GR_EXPLODED
	 **/
	proto native bool IsActivated ();
};

typedef ItemGrenade GrenadeBase;

//-----------------------------------------------------------------------------
class ItemMap extends InventoryItemSuper
{
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner,new_owner);
		
		SetMapStateOpen(false, PlayerBase.Cast(old_owner));
	}
	
	//! displays open/closed selections; 1 == opened
	void SetMapStateOpen(bool state, PlayerBase player)
	{
		if (state)
		{
			ShowSelection("map_opened");
			HideSelection("map_closed");
		}
		else
		{
			ShowSelection("map_closed");
			HideSelection("map_opened");
		}
		
		if (player)
			player.SetMapOpen(state);
	}
	
	bool GetMapStateAnimation()
	{
		if (GetAnimationPhase("map_opened") == 0)
			return true;
		return false;
	}
};
