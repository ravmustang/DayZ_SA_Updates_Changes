class ActionStripCarrierVestCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT);
	}
};

class ActionStripCarrierVest: ActionContinuousBase
{	
	void ActionStripCarrierVest()
	{
		m_CallbackClass = ActionStripCarrierVestCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		
		m_MessageStartFail = "I cannot unpack this.";
		m_MessageStart = "I have started unpacking the box.";
		m_MessageSuccess = "I have unpacked the box.";
		m_MessageFail = "I have moved and unpacking was canceled.";
		m_MessageCancel = "I stopped unpacking.";
		m_Sounds.Insert("bandage_0"); //sould be original sound effects?
		m_Sounds.Insert("bandage_1");
		m_Sounds.Insert("bandage_2");
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.GetNumberOfItems() > 0 )
		{
			return false;
		}
		return true;
	}
		
	override int GetType()
	{
		return AT_STRIP_CARRIER_VEST;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "Strip vest";
	}

	override void OnCompleteServer( ActionData action_data )
	{
		if ( action_data.m_MainItem && action_data.m_MainItem.GetHierarchyRootPlayer() == action_data.m_Player )
		{
			TStringArray results;
			Class.CastTo(results, new TStringArray);
			string itemName = action_data.m_MainItem.GetType();
			EntityAI object;
			results.Insert("PlateCarrierBlank");
			
			switch (itemName)
			{
				case "PlateCarrierHolster":
					results.Insert("CarrierHolsterSolo");
				break
				
				case "PlateCarrierPouches":
					results.Insert("CarrierPouches");
				break
				
				case "PlateCarrierComplete":
					results.Insert("CarrierPouches");
					results.Insert("CarrierHolsterSolo");
				break
			}

			for ( int i = 0; i < results.Count(); i++ )
			{
				object = EntityAI.Cast( GetGame().CreateObject(results.Get(i), action_data.m_Player.GetPosition(), false) );
				object.SetHealth("","",action_data.m_MainItem.GetHealth("",""));
			}
			action_data.m_MainItem.Delete();
			action_data.m_MainItem = NULL;
		}
	}
};