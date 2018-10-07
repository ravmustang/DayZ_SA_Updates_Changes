class ActionRaiseAndViewCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( -1.0 );
	}
};

class ActionViewCompass : ActionContinuousBase
{
	void ActionViewCompass()
	{
		m_CallbackClass = ActionRaiseAndViewCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_VIEWCOMPASS;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_VIEWCOMPASS;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override int GetType()
	{
		return AT_VIEW_COMPASS;
	}
		
	override string GetText()
	{
		return "Use compass";
	}
	
	override bool HasProgress()
	{
		return false;
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool HasProneException()
	{
		return true;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return item.IsOpen();
	}
	
	/*override void OnCompleteServer( ActionData action_data )
	{
		if (action_data.m_Player)
		{
			string message = Rangefinder.Cast( action_data.m_MainItem ).DoMeasurement(action_data.m_Player);
			SendMessageToClient(action_data.m_Player,message);
		}
		
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}*/
}