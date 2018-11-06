class ActionUseRangefinderCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( 1.0 );
	}
};

class ActionUseRangefinder : ActionContinuousBase
{
	void ActionUseRangefinder()
	{
		m_CallbackClass = ActionUseRangefinderCB;
		m_CommandUID = DayZPlayerConstants.CMD_GESTUREFB_LOOKOPTICS;
		m_MessageStartFail = "ActionUseRangefinder - m_MessageStartFail";
		m_MessageStart = "ActionUseRangefinder - m_MessageStart";
		m_MessageSuccess = "ActionUseRangefinder - m_MessageSuccess";
		m_MessageFail = "ActionUseRangefinder - m_MessageFail";
		m_MessageCancel = "ActionUseRangefinder - m_MessageCancel";
		////m_TimeToCompleteAction = 999; // TO DO: Must be infinite!
		//m_Animation = "close";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;	
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override int GetType()
	{
		return AT_USE_RANGE_FINDER;
	}
	
	override bool IsFullBody(PlayerBase player)
	{
		return true;
	}
		
	override string GetText()
	{
		return "#use_range_finder";
	}

	override bool HasTarget()
	{
		return false;
	}
	
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return item.GetCompEM().CanWork() && Rangefinder.Cast( item ).IsInOptics();
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{
		/*if (action_data.m_Player)
		{
			string message = Rangefinder.Cast( action_data.m_MainItem ).DoMeasurement(action_data.m_Player);
			SendMessageToClient(action_data.m_Player,message);
		}*/
		
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}