class ActionDestroyCombinationLockCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_DESTROY);
	}
};

class ActionDestroyCombinationLock: ActionContinuousBase
{	
	void ActionDestroyCombinationLock()
	{
		m_CallbackClass = ActionDestroyCombinationLockCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_MEDIUM;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_DESTROY_COMBINATION_LOCK;
	}

	override string GetText()
	{
		return "#destroy_combination_lock";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		string selection = target_object.GetActionComponentName( target.GetComponentIndex() );
		Fence fence = Fence.Cast( target_object );
		
		if ( fence && fence.IsLocked() && item.GetHealth() >= 50 && selection == "wall_interact" )
		{
			return true;
		}
		
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{	
		Fence fence = Fence.Cast( action_data.m_Target.GetObject() );
		if ( fence )
		{
			CombinationLock combination_lock = fence.GetCombinationLock();
			if ( combination_lock )
			{
				combination_lock.Unlock( fence );
				combination_lock.DecreaseHealth( 100 );
			}
		}
		
		action_data.m_MainItem.DecreaseHealth( 50 );
		
		//soft skills
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};