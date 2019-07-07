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
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}
		
	override string GetText()
	{
		return "#cut_down_bush";
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

	override void OnFinishProgressServer( ActionData action_data )
	{		
		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
	}
};