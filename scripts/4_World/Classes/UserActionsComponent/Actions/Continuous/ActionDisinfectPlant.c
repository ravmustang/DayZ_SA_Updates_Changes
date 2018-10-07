class ActionDisinfectPlantCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousDisinfectPlant(UAQuantityConsumed.GARDEN_DISINFECT_PLANT);
	}
};

class ActionDisinfectPlant: ActionContinuousBase
{
	void ActionDisinfectPlant()
	{
		m_CallbackClass = ActionDisinfectPlantCB;
		m_MessageSuccess = "I've disinfected plant.";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISINFECTTARGET;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_FullBody = true;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_DISINFECT_PLANT;
	}
		
	override string GetText()
	{
		return "Disinfect plant";
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlantBase plant;
		if ( Class.CastTo(plant,  target.GetObject() ) && !item.IsDamageDestroyed() )
		{			
			if ( plant.IsGrowing()  &&  plant.NeedsSpraying() )
			{	
				if ( item.GetQuantity() > 0 )
				{
					return true;
				}
			}
		}
		
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		PlantBase plant;
		if ( Class.CastTo(plant, action_data.m_Target.GetObject()) )
		{
			Param1<float> nacdata = Param1<float>.Cast( action_data.m_ActionComponent.GetACData() );
			SendMessageToClient(action_data.m_Player,plant.StopInfestation( nacdata.param1 ));
		}

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};