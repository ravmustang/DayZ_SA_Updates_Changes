class ActionActivateTrapCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_DEPLOY);
	}
};

class ActionActivateTrap: ActionContinuousBase
{	
	void ActionActivateTrap()
	{
		m_CallbackClass = ActionActivateTrapCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Sound = "craft_universal_0";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_ConditionItem = new CCINotPresent;
	}

	override int GetType()
	{
		return AT_ACTIVATE_TRAP;
	}

	override string GetText()
	{
		return "Activate the trap";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject != NULL && targetObject.IsInherited(TrapBase) ) 
		{
			TrapBase trap = TrapBase.Cast( targetObject );
			
			if ( trap.IsActivable() )
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		if ( targetObject != NULL && targetObject.IsInherited(TrapBase) ) 
		{
			TrapBase trap = TrapBase.Cast( targetObject );
			trap.StartActivate( action_data.m_Player );
		}
	}
};