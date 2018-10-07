class ActionIgniteFireplaceByAir: ActionIgniteFireplace
{	
	void ActionIgniteFireplaceByAir()
	{
		m_CallbackClass = ActionIgniteFireplaceCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BLOWFIREPLACE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_MessageStart = "I have started fire by blowing air into fireplace.";
		m_MessageSuccess = "I have started fire by blowing air into fireplace.";
		m_MessageFail = "I have failed to start fire by blowing air into fireplace.";
		m_MessageCancel = "I have stopped blowing air into fireplace.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;	
		
		m_SkipKindlingCheck = true;
	}

	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_ConditionItem = new CCINotPresent;
	}	
	
	override int GetType()
	{
		return AT_IGNITE_FIREPLACE_BY_AIR;
	}

	override string GetText()
	{
		return "Blow air into fireplace";
	}
		
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
		
		if ( target_object && target_object.IsFireplace() )
		{
			if ( fireplace_target.IsBaseFireplace() || fireplace_target.IsFireplaceIndoor() )
			{				
				if ( !fireplace_target.IsBurning() && fireplace_target.GetTemperature() >= fireplace_target.PARAM_MIN_TEMP_TO_REIGNITE )
				{
					return true;
				}
			}
		}
		
		return false;
	}
		
	override void OnCompleteServer( ActionData action_data )
	{
		FireplaceBase fireplace_target = FireplaceBase.Cast( action_data.m_Target.GetObject() );

		//start fire
		fireplace_target.StartFire();
	}
}