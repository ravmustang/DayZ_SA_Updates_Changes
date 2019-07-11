class DetachMagActionReciveData : ActionReciveData
{
	ref InventoryLocation  m_ilMagazine;
}
class DetachMagActionData : ActionData
{
	ref InventoryLocation  m_ilMagazine;
}

class FirearmActionDetachMagazine : FirearmActionBase
{	
	void FirearmActionDetachMagazine() 
	{
	}	
	
	override bool HasTarget()
	{
		return true;
	}
	
	override typename GetInputType()
	{
		return QuickaBarActionInput;
	}
	
	override bool CanBePerformedFromQuickbar()
	{
		return true;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTSelf;
	}
	
	override bool HasProgress()
	{
		return false;
	}
	
	override bool InventoryReservation( ActionData action_data)
	{
		Magazine mag = Magazine.Cast(action_data.m_Target.GetObject());
		InventoryLocation il = new InventoryLocation();
		if( !action_data.m_Player.GetInventory().FindFreeLocationFor(mag, FindInventoryLocationType.ANY_CARGO, il) )
			return false;
		
		if( !super.InventoryReservation( action_data) )
			return false;

		DetachMagActionData action_data_dm = DetachMagActionData.Cast(action_data);
		if( !action_data.m_Player.GetInventory().AddInventoryReservation(mag,il,10000) )
			return false;
			
		action_data_dm.m_ReservedInventoryLocations.Insert(il);
		action_data_dm.m_ilMagazine = il;

		return true;
	}
	
	override ActionData CreateActionData()
	{
		ActionData action_data = new DetachMagActionData;
		return action_data;
	}
	
	override void WriteToContext (ParamsWriteContext ctx, ActionData action_data)
	{
		super.WriteToContext(ctx, action_data);
		
		DetachMagActionData action_data_dm;
		DetachMagActionData.CastTo(action_data_dm, action_data);

		action_data_dm.m_ilMagazine.WriteToContext(ctx);
	}
	
	override bool ReadFromContext(ParamsReadContext ctx, out ActionReciveData action_recive_data )
	{
		if(!action_recive_data)
		{
			action_recive_data = new DetachMagActionReciveData;
		}
		
		if(!super.ReadFromContext( ctx, action_recive_data ))
			return false;
		
		DetachMagActionReciveData recive_data_dm;
		recive_data_dm = DetachMagActionReciveData.Cast( action_recive_data );
		
		recive_data_dm.m_ilMagazine = new InventoryLocation;

		if(!recive_data_dm.m_ilMagazine.ReadFromContext(ctx))
			return false;
		
		return true;
	}
	
	override void HandleReciveData(ActionReciveData action_recive_data, ActionData action_data)
	{
		super.HandleReciveData(action_recive_data, action_data);
		
		DetachMagActionReciveData recive_data_dm = DetachMagActionReciveData.Cast(action_recive_data);
		DetachMagActionData action_data_dm = DetachMagActionData.Cast(action_data);
		
		action_data_dm.m_ilMagazine = recive_data_dm.m_ilMagazine;
	}
	
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) //condition for action
	{
		Weapon_Base wpn = Weapon_Base.Cast(item);
		if (wpn && wpn.CanProcessWeaponEvents())
		{
			Magazine mag = Magazine.Cast(target.GetObject());
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
		DetachMagActionData action_data_dm = DetachMagActionData.Cast(action_data);
		
		Weapon_Base wpn = Weapon_Base.Cast( action_data.m_MainItem );
		
		action_data.m_Player.GetWeaponManager().DetachMagazine(action_data_dm.m_ilMagazine,this);
	}
};


class ActionAdvancedDetachMagazine : ActionSequentialBase
{
	void ActionAdvancedDetachMagazine()
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
		InventoryLocation il = new InventoryLocation();
		
		switch (action_data.m_Stage)
		{
			case 0:
				wpn = Weapon_Base.Cast( action_data.m_MainItem );
				mag = Magazine.Cast(action_data.m_Target.GetObject());
		
				action_data.m_Player.GetInventory().FindFreeLocationFor(mag, FindInventoryLocationType.ANY_CARGO, il);
				action_data.m_Player.GetWeaponManager().DetachMagazine(il,this);			
				break;
			
			case 1:
				End( action_data );
				break;/*
				ClearInventoryReservation(action_data);
				
				wpn = Weapon_Base.Cast(action_data.m_MainItem);
				mag = Magazine.Cast(action_data.m_Target.GetObject());
			
				int ur_index = action_data.m_Player.GetHumanInventory().FindUserReservedLocationIndex(wpn);
			
				if(ur_index > -1)
					action_data.m_Player.GetHumanInventory().GetUserReservedLocation(ur_index,il);
			
				if ( action_data.m_Player.GetInventory().CanForceSwapEntities(mag, wpn, il) )
				{
					if(GetGame().IsClient() || !GetGame().IsMultiplayer())
					{
						action_data.m_Player.GetInventory().ForceSwapEntities(InventoryMode.PREDICTIVE, mag, wpn, il);
					}
				}
				else if (action_data.m_Player.GetInventory().CanSwapEntities(mag, wpn) )
				{
					action_data.m_Player.GetInventory().SwapEntities(InventoryMode.PREDICTIVE, mag, wpn);
				}
				End( action_data );
				break;*/
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