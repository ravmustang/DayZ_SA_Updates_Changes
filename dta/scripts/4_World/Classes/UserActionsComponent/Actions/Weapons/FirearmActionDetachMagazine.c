class FirearmActionDetachMagazine : FirearmActionBase
{	
	void FirearmActionDetachMagazine() 
	{
	}	
	
	override bool HasTarget()
	{
		return false;
	}
	
/*	override typename GetInputType()
	{
		return ContinuousWeaponManipulationActionInput;
	} 
*/	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTSelf;
	}
	
	override bool HasProgress()
	{
		return false;
	}
	
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) //condition for action
	{
		Weapon_Base wpn = Weapon_Base.Cast(item);
		if (wpn && wpn.CanProcessWeaponEvents())
		{
			Magazine mag = wpn.GetMagazine(wpn.GetCurrentMuzzle());
			if (player.GetWeaponManager().CanDetachMagazine(wpn,mag))
				return true;
		}
		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data)
	{
		return true;
	}
	
	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		Weapon_Base wpn = Weapon_Base.Cast( action_data.m_MainItem );
		Magazine mag = wpn.GetMagazine(wpn.GetCurrentMuzzle());
		
		InventoryLocation il = new InventoryLocation();
		action_data.m_Player.GetInventory().FindFreeLocationFor(mag, FindInventoryLocationType.ANY_CARGO, il);
		action_data.m_Player.GetWeaponManager().DetachMagazine(il);
	}
};


class ActionAdvencedDetachMagazine : ActionSequentialBase
{
	void ActionAdvencedDetachMagazine()
	{
	}
	
	override typename GetInputType()
	{
		return QuickaBarActionInput;
	}
	
	override void CreateConditionComponents() //Instantiates components, called once from ActionManager on actions construction
	{
		m_ConditionItem = new CCIDummy;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool CanBePerformedFromQuickbar()
	{
		return true;
	}
	
	/*override typename GetInputType()
	{
		return ExternalControlledActionInput;
	}*/
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Weapon_Base wpn = Weapon_Base.Cast( item );
		int mi = wpn.GetCurrentMuzzle();
		if( wpn && wpn.GetMagazine(mi) == target.GetObject() )
			return true;
		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{
		return true;
	}
	
	override void OnStageStart(SequentialActionData action_data)
	{
		if( action_data.m_ReciveEndInput )
		{
			End( action_data );
			return;
		}
		super.OnStageStart(action_data);
		Weapon_Base wpn;
		Magazine mag;
		InventoryLocation il;
		
		switch (action_data.m_Stage)
		{
			case 0:
				wpn = Weapon_Base.Cast( action_data.m_MainItem );
				mag = Magazine.Cast(action_data.m_Target.GetObject());
		
				il = new InventoryLocation();
				action_data.m_Player.GetInventory().FindFreeLocationFor(mag, FindInventoryLocationType.ANY_CARGO, il);
				action_data.m_Player.GetWeaponManager().DetachMagazine(il,this);			
				break;
			
			case 1:
				//TODO MW Remove after fix
				End( action_data );
				break;
				ClearInventoryReservation(action_data);
				
				wpn = Weapon_Base.Cast(action_data.m_MainItem);
				mag = Magazine.Cast(action_data.m_Target.GetObject());
			
				if (action_data.m_Player.GetInventory().CanSwapEntities(mag, wpn) )
				{
					action_data.m_Player.GetInventory().SwapEntities(InventoryMode.PREDICTIVE, mag, wpn);
				}
				else
				{
					il = new InventoryLocation;
					if(action_data.m_Player.GetInventory().CanForceSwapEntities(mag, wpn, il) )
					{
						action_data.m_Player.GetInventory().ForceSwapEntities(InventoryMode.PREDICTIVE, mag, wpn, il);
					}
				}
				End( action_data );
				break;
			default:
				End( action_data );
				break;
		}
	}
	
	override void OnUpdate( ActionData action_data )
	{
		super.OnUpdate( action_data );
		
		SequentialActionData ad = SequentialActionData.Cast(action_data);
		if(ad)
		{
			switch (ad.m_Stage)
			{
				case 0:
					Weapon_Base wpn = Weapon_Base.Cast(action_data.m_MainItem);
		
					if ( !wpn || ad.m_Player.GetItemInHands()!=wpn || !ad.m_Player.GetWeaponManager().IsRunning() )
					{
						SetStage(1, ad);
					}
					break;
				case 1:
					//action_data.m_Player.GetHandFs
					
					break;
				default:
					break;
			}
		}
	}
}