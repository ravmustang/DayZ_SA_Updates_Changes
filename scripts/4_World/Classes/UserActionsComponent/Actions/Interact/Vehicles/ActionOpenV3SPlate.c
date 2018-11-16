class ActionOpenV3SPlate: ActionInteractBase
{
	string m_AnimSource;

	void ActionOpenV3SPlate()
	{
		m_MessageSuccess = "";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_OPEN_V3S_PLATE;
	}

	override string GetText()
	{
		return "Open side plate";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if( !target ) return false	;
		if( !IsInReach(player, target, UAMaxDistances.DEFAULT) ) return false;
		
		Object targetObject = target.GetObject();
		Entity targetEnt = Entity.Cast(targetObject);
		
		string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
		
		
		//string test = GetGame().ConfigIsExisting("CfgVehicles" + " " + targetObject.GetTypeName().GetObject() + "AnimationSources" + selection);
		
		string test = "CfgVehicles" + " " + targetObject.GetType() + " " + "AnimationSources" + " " + selection;
		
		if ( GetGame().ConfigIsExisting("CfgVehicles" + " " + targetObject.GetType() + "AnimationSources" + selection))
		{
			
			Print("Testuju");
		}
		
		
		
		
		if ( targetEnt && selection != "" && targetEnt.GetAnimationPhase(selection) <= 0.5)
		{
			m_AnimSource = selection;
			return true;
		}

		return false;
	}
	
	override void OnCompleteServer( ActionData action_data )
	{
		Entity targetEnt;
		if ( Class.CastTo(targetEnt, action_data.m_Target.GetObject()) )
		{
			targetEnt.SetAnimationPhase(m_AnimSource, 1.0);
		}
	}

	override void OnCompleteClient( ActionData action_data )
	{
		Entity targetEnt;
		if ( Class.CastTo(targetEnt, action_data.m_Target.GetObject()) )
		{
			targetEnt.SetAnimationPhase(m_AnimSource, 1.0);
		}
	}
};