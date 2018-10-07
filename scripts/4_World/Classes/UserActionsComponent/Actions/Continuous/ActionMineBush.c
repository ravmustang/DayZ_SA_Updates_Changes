class ActionMineBushCB : ActionContinuousBaseCB
{
	private const float TIME_BETWEEN_MATERIAL_DROPS = 3;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousMineWood(TIME_BETWEEN_MATERIAL_DROPS);
	}
};

class ActionMineBush: ActionContinuousBase
{
	void ActionMineBush()
	{
		m_CallbackClass = ActionMineBushCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKBUSH;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail = "My tool is too damaged to cut.";
		m_MessageStart = "I started cutting the bush down.";
		m_MessageSuccess = "I have cut the bush down.";
		m_MessageFail = "My tool is ruined.";
		//m_AnimationOneHanded = "DRINK";
		//m_AnimationTwoHanded = "DIGGINGSHOVEL";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_MINE_BUSH;
	}
		
	override string GetText()
	{
		return "Cut the bush down";
	}

		
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject.IsBush() ) 
		{
			return true;
		}
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{		
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};