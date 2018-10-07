class ActionLoadMagazineCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.MAG_EMPTY);
	}
};

class ActionLoadMagazine: ActionContinuousBase
{
	const float TARGET_DISTANCE = 2;
	
	void ActionLoadMagazine()
	{
		m_CallbackClass = ActionLoadMagazineCB;
		m_MessageStartFail = "...";
		m_MessageStart = "I started loading magazine.";
		m_MessageSuccess = "I finished loading magazine.";
		m_MessageFail = "I stoped loading magazine.";
		m_MessageCancel = "I stoped loading magazine.";	
		
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_FILLMAG;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_FILLMAG;
	}

	override int GetType()
	{
		return AT_LOAD_MAGAZINE;
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override string GetText()
	{
		return "Load magazine";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTNonRuined(TARGET_DISTANCE);
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{		
		Magazine trg;
		Magazine itm;
		if ( Class.CastTo(trg, target.GetObject()) && Class.CastTo(itm, item) && itm.GetAmmoCount() < itm.GetAmmoMax() && trg.GetAmmoCount() != 0 )
		{
			array<string> ammo_array = new array<string>;
		
			itm.ConfigGetTextArray ("ammoItems", ammo_array);
			for( int i = 0; i < ammo_array.Count(); i++ )
			{
				string AmmoType = ammo_array.Get(i);

				if(AmmoType == trg.GetType())
					return true;	
			}
		}
		return false;
	}
	
	override void OnRepeatServer( ActionData action_data )
	{
		Param1<float> timeSpendParam;
		if( !Class.CastTo(timeSpendParam, action_data.m_ActionComponent.GetACData()) ) return;
		
		Magazine trg;
		Magazine itm;
		if ( Class.CastTo(trg,  action_data.m_Target.GetObject()) && Class.CastTo(itm, action_data.m_MainItem) )
		{
			float dmg;
			string ammoType;
			
			if( trg.ServerAcquireCartridge(dmg,ammoType) )
			{
				if( !itm.ServerStoreCartridge(dmg,ammoType) )			
					trg.ServerStoreCartridge(dmg,ammoType);
			}
		}
		//if(trg.GetAmmoCount() > 0 )
			//trg.SetSynchDirty();
		//itm.SetSynchDirty();
	}

	override void OnCompleteServer( ActionData action_data )
	{
		//OnRepeat(action_data);
	}
	
	override bool CanBePerformedFromQuickbar()
	{
		return true;
	}
};